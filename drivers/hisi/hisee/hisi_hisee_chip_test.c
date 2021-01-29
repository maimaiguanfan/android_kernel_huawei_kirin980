#include <asm/compiler.h>
#include <linux/compiler.h>
#include <linux/fd.h>
#include <linux/tty.h>
#include <linux/fs.h>
#include <linux/fcntl.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/syscalls.h>
#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/semaphore.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_reserved_mem.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/atomic.h>
#include <linux/notifier.h>
#include <linux/printk.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/interrupt.h>
#include <linux/hisi/ipc_msg.h>
#include <linux/hisi/hisi_rproc.h>
#include <linux/hisi/kirin_partition.h>
#include <linux/clk.h>
#include <linux/mm.h>
#include "soc_acpu_baseaddr_interface.h"
#include "soc_sctrl_interface.h"
#include "hisi_hisee.h"
#include "hisi_hisee_fs.h"
#include "hisi_hisee_power.h"
#include "hisi_hisee_upgrade.h"
#include "hisi_hisee_chip_test.h"
#include <dsm/dsm_pub.h>
#include <hitest_slt.h>

/*1:one rpmbkey, 0:error, 2:two rpmbkey*/
extern u32 get_rpmb_support_key_num(void);

/*get key from HISEE
*KEY_NOT_READY(0):KEY_READY(1):KEY_REQ_FAILED(2) for hisee key
*/
extern int get_rpmb_key_status(void);

#define SET_COS_FLASH_BUF_PARA() \
do {\
	cos_flash_buf_para[0] = HISEE_CHAR_SPACE;\
	cos_flash_buf_para[2] = '0' + COS_PROCESS_UPGRADE;\
	cos_flash_buf_para[1] = '0' + COS_FLASH_IMG_ID;\
} while (0)


#define SET_COS_DEFAULT_BUF_PARA() \
do {\
	cos_default_buf_para[0] = HISEE_CHAR_SPACE;\
	cos_default_buf_para[2] = '0' + COS_PROCESS_UPGRADE;\
	cos_default_buf_para[1] = '0' + COS_IMG_ID_0;\
} while (0)

/* check ret is ok or otherwise goto err_process*/
#define CHECK_OK(ret) do { if (HISEE_OK != (ret)) goto err_process; } while (0)


hisee_at_type g_at_cmd_type = HISEE_AT_MAX;

/* flag to indicate running status of flash otp1 */
static E_RUN_STATUS g_hisee_flash_otp1_status;

/* hisee manufacture function begin */

/* tell the flash_otp_task to write which is created by set/get efuse _securitydebug_value  */
/* this interface is defined in hisi_flash_hisee_otp.c  */
extern void release_hisee_semphore(void);/*should be semaphore; whatever..*/
/* check the flash_otp_task which is created by set/get efuse _securitydebug_value  */
/* this interface is defined in hisi_flash_hisee_otp.c  */
extern bool flash_otp_task_is_started(void);

/* set the otp1 write work status */
void hisee_chiptest_set_otp1_status(E_RUN_STATUS status)
{
	g_hisee_flash_otp1_status = status;
	pr_err("hisee set otp1 status %x\n", g_hisee_flash_otp1_status);
}

E_RUN_STATUS hisee_chiptest_get_otp1_status(void)
{
	return g_hisee_flash_otp1_status;
}

/* check otp1 write work is running */
/* flash_otp_task may not being created by set/get efuse _securitydebug_value */
bool hisee_chiptest_otp1_is_runing(void)
{
	pr_info("hisee otp1 work status %x\n", g_hisee_flash_otp1_status);
	if (RUNING == g_hisee_flash_otp1_status) {
		return true;
	}

	if (PREPARED == g_hisee_flash_otp1_status &&
	    true == flash_otp_task_is_started()) {
		return true;
	}

	return false;
}

static int otp_image_upgrade_func(void *buf, int para)
{
    int ret;
	unsigned int cos_id = COS_IMG_ID_0;
	unsigned int process_id = 0;

	ret = hisee_get_cosid_processid(buf, &cos_id, &process_id);
	if (HISEE_OK != ret) {
		pr_err("%s() hisee_get_cosid failed ret=%d\n", __func__, ret);
		set_errno_and_return(ret);
	}
	if (COS_IMG_ID_0 != cos_id) {
		pr_err("hisee:%s() cosid=%d not support otp image upgrade now, bypass!\n", __func__, cos_id);
		return ret;
	}
	ret = hisee_poweron_booting_func((void *)buf, 0);

	if (HISEE_OK == ret) {
		ret = write_hisee_otp_value(OTP_IMG_TYPE);

		(void)hisee_poweroff_func((void *)buf, (int)HISEE_PWROFF_LOCK);
	}
	check_and_print_result();

	set_errno_and_return(ret);/*lint !e1058*/
}/*lint !e715*/

static int hisee_write_rpmb_key(void *buf, int para)
{
    char *buff_virt = NULL;
    phys_addr_t buff_phy = 0;
    atf_message_header *p_message_header;
    int ret = HISEE_OK;
    int image_size = 0;

    buff_virt = (void *)dma_alloc_coherent(g_hisee_data.cma_device, SIZE_1K * 4,
                                            &buff_phy, GFP_KERNEL);
    if (buff_virt == NULL) {
        pr_err("%s(): dma_alloc_coherent failed\n", __func__);
        set_errno_and_return(HISEE_NO_RESOURCES);
    }
    memset(buff_virt, 0, SIZE_1K * 4);
    p_message_header = (atf_message_header *)buff_virt;
    set_message_header(p_message_header, CMD_WRITE_RPMB_KEY);
    image_size = HISEE_ATF_MESSAGE_HEADER_LEN;
    ret = send_smc_process(p_message_header, buff_phy, image_size,
                            HISEE_ATF_WRITE_RPMBKEY_TIMEOUT, CMD_WRITE_RPMB_KEY);
    dma_free_coherent(g_hisee_data.cma_device, (unsigned long)(SIZE_1K * 4), buff_virt, buff_phy);
    check_and_print_result();
    set_errno_and_return(ret);
}/*lint !e715*/

static int set_hisee_lcs_sm_otp(void *buf, int para)
{
    char *buff_virt = NULL;
    phys_addr_t buff_phy = 0;
    atf_message_header *p_message_header;
    int ret = HISEE_OK;
    int image_size;
    unsigned int result_offset;

    buff_virt = (void *)dma_alloc_coherent(g_hisee_data.cma_device, SIZE_1K * 4,
                                            &buff_phy, GFP_KERNEL);
    if (buff_virt == NULL) {
        pr_err("%s(): dma_alloc_coherent failed\n", __func__);
        set_errno_and_return(HISEE_NO_RESOURCES);
    }
    memset(buff_virt, 0, SIZE_1K * 4);
    p_message_header = (atf_message_header *)buff_virt;
    set_message_header(p_message_header, CMD_SET_LCS_SM);

    image_size = HISEE_ATF_MESSAGE_HEADER_LEN;
    result_offset = HISEE_ATF_MESSAGE_HEADER_LEN;
    p_message_header->test_result_phy = (unsigned int)buff_phy + result_offset;
    p_message_header->test_result_size = SIZE_1K * 4 - result_offset;
    ret = send_smc_process(p_message_header, buff_phy, (unsigned int)image_size,
                            HISEE_ATF_GENERAL_TIMEOUT, CMD_SET_LCS_SM);
    if (HISEE_OK != ret) {
        pr_err("%s(): hisee reported fail code=%d\n", __func__, *((int *)(void *)(buff_virt + result_offset)));
    }

    dma_free_coherent(g_hisee_data.cma_device, (unsigned long)(SIZE_1K * 4), buff_virt, buff_phy);
    check_and_print_result();
    set_errno_and_return(ret);
}/*lint !e715*/

static int upgrade_one_file_func(char *filename, se_smc_cmd cmd)
{
    char *buff_virt;
    phys_addr_t buff_phy = 0;
    atf_message_header *p_message_header;
    int ret = HISEE_OK;
    size_t image_size;
    unsigned int result_offset;

    /* alloc coherent buff with vir&phy addr (64K for upgrade file) */
    buff_virt = (void *)dma_alloc_coherent(g_hisee_data.cma_device, HISEE_SHARE_BUFF_SIZE,
                                            &buff_phy, GFP_KERNEL);
    if (buff_virt == NULL) {
        pr_err("%s(): dma_alloc_coherent failed\n", __func__);
        set_errno_and_return(HISEE_NO_RESOURCES);
    }
    memset(buff_virt, 0, HISEE_SHARE_BUFF_SIZE);

    image_size = 0;
    /* read given file to buff */
    ret = filesys_read_img_from_file((const char *)filename, (buff_virt + HISEE_ATF_MESSAGE_HEADER_LEN), &image_size, HISEE_MAX_IMG_SIZE);
    if (ret < HISEE_OK) {
        pr_err("%s(): hisee_read_file failed, filename=%s, ret=%d\n", __func__, filename, ret);
        dma_free_coherent(g_hisee_data.cma_device, (unsigned long)HISEE_SHARE_BUFF_SIZE, buff_virt, buff_phy);
        set_errno_and_return(ret);
    }
    image_size = (image_size + HISEE_ATF_MESSAGE_HEADER_LEN);

    /* init and config the message */
    p_message_header = (atf_message_header *)buff_virt; /*lint !e826*/
    set_message_header(p_message_header, cmd);
    /* reserve 256B for test result(err code from hisee) */
    result_offset = ((u32)(image_size + SMC_TEST_RESULT_SIZE - 1)) & (~(u32)(SMC_TEST_RESULT_SIZE -1));
    if (result_offset + SMC_TEST_RESULT_SIZE <= HISEE_SHARE_BUFF_SIZE) {
        p_message_header->test_result_phy = (unsigned int)buff_phy + result_offset;
        p_message_header->test_result_size = HISEE_SHARE_BUFF_SIZE - result_offset;
    } else {
        /* this case, test_result_phy will be 0 and atf will not write test result;
         * and kernel side will record err code a meaningless val (but legal) */
        result_offset = 0;
    }

    /* smc call (synchronously) */
    ret = send_smc_process(p_message_header, buff_phy, (unsigned int)image_size,
                            HISEE_ATF_GENERAL_TIMEOUT, cmd);
    if (HISEE_OK != ret) {
        if (cmd != CMD_FACTORY_APDU_TEST) /* apdu test will not report err code */
            pr_err("%s(): hisee reported err code=%d\n", __func__, *((int *)(void *)(buff_virt + result_offset)));
    }

    dma_free_coherent(g_hisee_data.cma_device, (unsigned long)HISEE_SHARE_BUFF_SIZE, buff_virt, buff_phy);
    check_and_print_result();
    set_errno_and_return(ret);
}

static int hisee_apdu_test_func(void *buf, int para)
{
    int ret;
    ret = upgrade_one_file_func("/mnt/hisee_fs/test.apdu.bin", CMD_FACTORY_APDU_TEST);
    check_and_print_result();
    set_errno_and_return(ret);
}

static int hisee_verify_isd_key(hisee_cos_imgid_type cos_id)
{
    char *buff_virt;
    phys_addr_t buff_phy = 0;
    atf_message_header *p_message_header;
    int ret = HISEE_OK;
    unsigned int image_size;

	if (COS_IMG_ID_0 != cos_id) {
		/*TODO:can do more actions in future if necessary*/
		pr_err("hisee:%s() cosid=%d not support verify_isd_key now, bypass!\n", __func__, cos_id);
		return ret;
	}

    buff_virt = (void *)dma_alloc_coherent(g_hisee_data.cma_device, (unsigned long)SIZE_1K * 4,
                                            &buff_phy, GFP_KERNEL);
    if (buff_virt == NULL) {
        pr_err("%s(): dma_alloc_coherent failed\n", __func__);
        set_errno_and_return(HISEE_NO_RESOURCES);
    }
    memset(buff_virt, 0, (unsigned long)SIZE_1K * 4);
    p_message_header = (atf_message_header *)buff_virt;  /*lint !e826*/
    set_message_header(p_message_header, CMD_HISEE_VERIFY_KEY);
    image_size = HISEE_ATF_MESSAGE_HEADER_LEN;
    ret = send_smc_process(p_message_header, buff_phy, image_size,
                            HISEE_ATF_GENERAL_TIMEOUT, CMD_HISEE_VERIFY_KEY);
    dma_free_coherent(g_hisee_data.cma_device, (unsigned long)(SIZE_1K * 4), buff_virt, buff_phy);
    if (HISEE_OK == ret) {
		pr_err("hisee:%s() run success!", __func__);
    } else {
		pr_err("hisee:%s() run failed!", __func__);
    }
	return ret;
}


/****************************************************************************//**
 * @brief      : bypass_write_casd_key
 * @param[in]  : NA
 * @return     : ::int
 * @note       : send NULL to bypass
********************************************************************************/
static int bypass_write_casd_key(void)
{
    char *buff_virt;
    phys_addr_t buff_phy = 0;
    atf_message_header *p_message_header;
    int ret = HISEE_OK;

    /* alloc coherent buff with vir&phy addr */
    buff_virt = (void *)dma_alloc_coherent(g_hisee_data.cma_device, SIZE_4K,
                                            &buff_phy, GFP_KERNEL);
    if (NULL == buff_virt) {
        pr_err("%s(): dma_alloc_coherent failed\n", __func__);
        set_errno_and_return(HISEE_NO_RESOURCES);
    }
    memset(buff_virt, 0, SIZE_4K);

    /* init and config the message */
    p_message_header = (atf_message_header *)buff_virt; /*lint !e826*/
    set_message_header(p_message_header, CMD_HISEE_WRITE_CASD_KEY);

    /* smc call (synchronously) */
    ret = send_smc_process(p_message_header, buff_phy, HISEE_ATF_MESSAGE_HEADER_LEN,
            HISEE_ATF_GENERAL_TIMEOUT, CMD_HISEE_WRITE_CASD_KEY);
    if (HISEE_OK != ret) {
        pr_err("%s() fail\n", __func__);
    }

    dma_free_coherent(g_hisee_data.cma_device, (unsigned long)SIZE_4K, buff_virt, buff_phy);
    check_and_print_result();
    set_errno_and_return(ret);
}

/****************************************************************************//**
 * @brief      : hisee_write_casd_key
 * @param[in]  : NA
 * @return     : ::int
 * @note       :
********************************************************************************/
static int hisee_write_casd_key(void)
{
    int ret;
    if (false == get_support_casd()) {
        return HISEE_OK;
    }

    if (true == get_bypass_casd()) {
        /* in default, do not write casd key */
        ret = bypass_write_casd_key();
    } else {
        ret = upgrade_one_file_func(HISEE_CASD_IMG_FULLNAME, CMD_HISEE_WRITE_CASD_KEY);
    }
    check_and_print_result();
    set_errno_and_return(ret);
}

static int g_hisee_flag_protect_lcs = 0;
int hisee_debug(void)
{
    return g_hisee_flag_protect_lcs;
}

static int hisee_write_rpmb_key_process(void *buf)
{
    int ret = HISEE_ERROR;
    int ret_pm;
    int write_rpmbkey_try = 5;

    while (write_rpmbkey_try--) {
        ret = hisee_write_rpmb_key(NULL, 0);
        if (HISEE_OK == ret) {
            break;
        }

        ret_pm = hisee_poweroff_func(buf, HISEE_PWROFF_LOCK);
        CHECK_OK(ret_pm);
        ret_pm = hisee_poweron_upgrade_func(buf, 0);
        CHECK_OK(ret_pm);
        hisee_mdelay(DELAY_FOR_HISEE_POWERON_UPGRADE); /*lint !e744 !e747 !e748*/
    }

err_process:
    check_and_print_result();
    return ret;
}

static int hisee_apdu_test_process(hisee_cos_imgid_type cos_id)
{
    int ret;

	if (COS_IMG_ID_0 != cos_id) {
		ret = wait_hisee_ready(HISEE_STATE_COS_READY, DELAY_FOR_HISEE_POWERON_BOOTING);
		if (HISEE_OK != ret) {
			pr_err("hisee:%s(): wait_hisee_ready failed,retcode=%d\n", __func__, ret);
		}
		return ret;
	}
    ret = hisee_apdu_test_func(NULL, 0);
    CHECK_OK(ret);

    /* send command to delete test applet */
    ret = send_apdu_cmd(HISEE_DEL_TEST_APPLET);
    CHECK_OK(ret);

err_process:
    check_and_print_result();
    return ret;
}

static int hisee_manufacture_set_lcs_sm(unsigned int hisee_lcs_mode)
{
    int ret = HISEE_OK;
    if (HISEE_DM_MODE_MAGIC == hisee_lcs_mode && g_hisee_flag_protect_lcs == 0) {
        ret = set_hisee_lcs_sm_otp(NULL, 0);
        CHECK_OK(ret);

        ret = set_hisee_lcs_sm_efuse();
        CHECK_OK(ret);
    }

err_process:
    return ret;
}

extern char *g_patch_buff_virt;
extern phys_addr_t g_patch_buff_phy;

/* poweron booting hisee in misc upgrade mode
 * and do write casd key and misc image upgrade in order
 * go through: hisee misc ready -> write casd key to nvm -> misc upgrade to nvm -> hisee cos ready. */
static int hisee_poweron_booting_misc_process(void *buf)
{
    cosimage_version_info misc_version;
	/*3 characters: space,cos_id=0,process_id=COS_PROCESS_CHIP_TEST*/
	char cos_default_buf_para[MAX_CMD_BUFF_PARAM_LEN] = {0};
	unsigned int cos_id = 0;
	unsigned int process_id = 0;
	int ret;
	hisee_img_file_type img_type = MISC3_IMG_TYPE;

	SET_COS_DEFAULT_BUF_PARA();
	ret = hisee_get_cosid_processid(buf, &cos_id, &process_id);
	CHECK_OK(ret);

	cos_default_buf_para[1] = '0' + cos_id;
	if (COS_IMG_ID_0 != cos_id) {
		ret = hisee_poweron_booting_func((void *)cos_default_buf_para, HISEE_POWER_ON_BOOTING);
		pr_err("hisee:%s() cosid=%d not support misc booting now, bypass!\n", __func__, cos_id);
		CHECK_OK(ret);
		/* wait hisee cos ready for later process */
		ret = wait_hisee_ready(HISEE_STATE_COS_READY, 30000);
		goto err_process;
	}

    /* poweron booting hisee and set the flag for the process */
    ret = hisee_poweron_booting_func((void *)cos_default_buf_para, HISEE_POWER_ON_BOOTING_MISC);
    CHECK_OK(ret);

    /* wait hisee ready for receiving images */
    ret = wait_hisee_ready(HISEE_STATE_MISC_READY, 30000);
    CHECK_OK(ret);

    /* write casd key should be combined with misc image upgrade and be in right order */
    ret = hisee_write_casd_key();
    CHECK_OK(ret);

    /* cos patch upgrade only supported in this function */
    ret = hisee_cos_patch_read(img_type + (HISEE_MAX_MISC_IMAGE_NUMBER * cos_id));
    CHECK_OK(ret);

    /* misc image upgrade only supported in this function */
    ret = misc_image_upgrade_func(NULL, cos_id);
    CHECK_OK(ret);

    /* wait hisee cos ready for later process */
    ret = wait_hisee_ready(HISEE_STATE_COS_READY, 30000);
    CHECK_OK(ret);

    /* write current misc version into record area */
    if (g_misc_version[cos_id]) {
		access_hisee_image_partition((char *)&misc_version, MISC_VERSION_READ_TYPE);
        misc_version.magic = HISEE_SW_VERSION_MAGIC_VALUE;
        misc_version.img_version_num[cos_id] = (unsigned char)g_misc_version[cos_id];
        access_hisee_image_partition((char *)&misc_version, MISC_VERSION_WRITE_TYPE);
    }

err_process:
    check_and_print_result();
    return ret;
}

/*************************************************************
����ԭ�ͣ�int run_hisee_nvmformat(void)
�������ܣ�ִ��hisee nvm format������������SMCָ�����ATFִ��nvm format����
������
���룺��
������ޡ�
����ֵ��HISEE_OK:ִ��hisee nvm format���������ɹ�������HISEE_OK:ִ��hisee nvm format��������ʧ��
ǰ��������cos flash���������ɹ�������cos flash�������С�
���������� ��
*************************************************************/
static int run_hisee_nvmformat(void)
{
	char *buff_virt = NULL;
    phys_addr_t buff_phy = 0;
    atf_message_header *p_message_header;
    int ret = HISEE_OK;
    int image_size;

    buff_virt = (void *)dma_alloc_coherent(g_hisee_data.cma_device, SIZE_1K * 4,
                                            &buff_phy, GFP_KERNEL);
    if (buff_virt == NULL) {
        pr_err("%s(): dma_alloc_coherent failed\n", __func__);
        set_errno_and_return(HISEE_NO_RESOURCES);
    }

    memset(buff_virt, 0, SIZE_1K * 4);
    p_message_header = (atf_message_header *)buff_virt;
    set_message_header(p_message_header, CMD_FORMAT_RPMB);

    image_size = HISEE_ATF_MESSAGE_HEADER_LEN;
    ret = send_smc_process(p_message_header, buff_phy, (unsigned int)image_size,
                            HISEE_ATF_NVM_FORMAT_TIMEOUT, CMD_FORMAT_RPMB);
    if (HISEE_OK != ret) {
        pr_err("%s(): hisee reported fail code=%d\n", __func__, ret);
    }

    dma_free_coherent(g_hisee_data.cma_device, (unsigned long)(SIZE_1K * 4), buff_virt, buff_phy);
    check_and_print_result();
    set_errno_and_return(ret);
}

/*************************************************************
����ԭ�ͣ�int cos_flash_image_upgrade_func(void *buf, unsigned int factory_test_flg)
�������ܣ�ִ��cos_flash����������ϵ硢�����������ܣ���cos_flash����������encos������Ȼ���hisee�µ�
������
���룺buf:���浱ǰ������Ӧ��cos_id��process_id���ַ���ָ�룬����ΪNULL(��Ĭ��ֵ)��
	  factory_test_flg:ָʾ��cos flash�����������ڲ���������������adb shell������ΪHISEE_FACTORY_TEST_VERSION
	  ��ʾ��������������
������ޡ�
����ֵ��HISEE_OK:cos_flash���������ɹ�������HISEE_OK:cos_flash��������ʧ��
ǰ��������/mnt/hisee_fs/Ŀ¼�´���cos_flash.img
���������� ��
*************************************************************/
static int cos_flash_image_upgrade_func(void *buf, unsigned int factory_test_flg)
{
	char cos_flash_buf_para[MAX_CMD_BUFF_PARAM_LEN] = {0};
	int ret, ret1;

	ret = hisee_poweroff_func(buf, 0);
	if (HISEE_OK != ret) {
		pr_err("hisee:%s() poweroff failed. retcode=%d\n", __func__, ret);
		ret = HISEE_MULTICOS_POWEROFF_ERROR;
		goto err_process;
	}
	SET_COS_FLASH_BUF_PARA();

	ret = hisee_poweron_upgrade_func(cos_flash_buf_para, 0);
	if (HISEE_OK != ret) {
		pr_err("hisee:%s() power failed, ret = %d\n", __func__, ret);
		ret = HISEE_MULTICOS_POWERON_UPGRADE_ERROR;
		goto err_process;
	}

	hisee_mdelay(200);
	ret1 = cos_upgrade_image_read(COS_FLASH_IMG_ID, COS_FLASH_IMG_TYPE);
	if (HISEE_OK != ret1) {
		pr_err("hisee:%s upgrade failed, ret = %d\n", __func__, ret1);
		ret = HISEE_MULTICOS_IMG_UPGRADE_ERROR;
	}
	ret1 = hisee_poweroff_func(cos_flash_buf_para, 0);
	if (HISEE_OK != ret1) {
		pr_err("hisee:%s() poweroff failed. retcode=%d\n", __func__, ret1);
		if (HISEE_OK == ret) {
			ret = HISEE_MULTICOS_POWEROFF_ERROR;
		}
		goto err_process;
	}

err_process:
    check_and_print_result();
    return ret;
}

/*************************************************************
����ԭ�ͣ�int cos_flash_image_boot_func(void)
�������ܣ�ִ��cos_flash�����������ع��ܣ�Hisee coe����cos flash����
������
���룺��
������ޡ�
����ֵ��HISEE_OK:cos_flash���������ɹ�������HISEE_OK:cos_flash��������ʧ��
ǰ��������cos_flash_image_upgrade_func()�Ѿ�ִ�гɹ�����ʾcos_flash�����Ѿ�
			�������ұ��浽��ȫDDR�У����м䲻��ִ�������������ϵ����
���������� ��
*************************************************************/
static int cos_flash_image_boot_func(void)
{
	int ret;
	char cos_flash_buf_para[MAX_CMD_BUFF_PARAM_LEN] = {0};

	SET_COS_FLASH_BUF_PARA();
	ret = hisee_poweron_booting_func((void *)cos_flash_buf_para, HISEE_POWER_ON_BOOTING);
	CHECK_OK(ret);
	/* wait hisee ready for receiving images */
    ret = wait_hisee_ready(HISEE_STATE_COS_READY, 30000);
    CHECK_OK(ret);

err_process:
    check_and_print_result();
    return ret;
}

/*************************************************************
����ԭ�ͣ�int sm_write_rpmb_key_process(void *buf)
�������ܣ�hisee SMģʽ��дhisee ���rpmbkey�Ĺ���
������
���룺�ޡ�
������ޡ�
����ֵ��HISEE_OK:SMģʽ��дhisee ���rpmbkey�ɹ���SMģʽ��дhisee ���rpmbkeyʧ��
ǰ��������cos_flash��Ӧ��cos��������Ѿ������ϵ�ɹ���hisee�������ص���cos_flash����
���������� ��
*************************************************************/
static int sm_write_rpmb_key_process(void)
{
    int ret = HISEE_OK;
    int write_rpmbkey_try = 5;
	char cos_flash_buf_para[MAX_CMD_BUFF_PARAM_LEN] = {0};

	if ((SINGLE_RPMBKEY_NUM < get_rpmb_support_key_num()) &&
		(KEY_NOT_READY == get_rpmb_key_status())) {
		SET_COS_FLASH_BUF_PARA();
	    while (write_rpmbkey_try--) {
	        ret = hisee_write_rpmb_key(NULL, 0);
	        if (HISEE_OK == ret) {
	            break;
	        }
	        ret = hisee_poweroff_func((void *)cos_flash_buf_para, 0);
	        CHECK_OK(ret);
	        ret = hisee_poweron_booting_func((void *)cos_flash_buf_para, 0);
	        CHECK_OK(ret);
	        hisee_mdelay(DELAY_FOR_HISEE_POWERON_BOOTING); /*lint !e744 !e747 !e748*/
	    }
	}
err_process:
    check_and_print_result();
    return ret;
}

/*************************************************************
����ԭ�ͣ�int load_cos_flash_do_factory_test(void *buf)
�������ܣ�ִ��ֻ����cos flash����֧�ֵ���صĲ�������������
		  ����cos flash���񣬼���cos flash���������������к󣬸����豸״̬����
		  дhisee rpmb region��rpmbkey������run_hisee_nvmformat()���NVM format�Ĳ�����
������
���룺buf:���浱ǰ������Ӧ��cos_id��process_id���ַ���ָ�룬����ΪNULL(��Ĭ��ֵ)��
	  para:ָʾ����cos flash������DM�»���SMģʽ�µĲ��߲���
������ޡ�
����ֵ��HISEE_OK:ִ��ֻ����cos flash����֧�ֵ���صĲ������������ɹ���
		����HISEE_OK:ִ��ֻ����cos flash����֧�ֵ���صĲ�����������ʧ��
ǰ��������
���������� ��
*************************************************************/
int load_cos_flash_do_factory_test(void *buf, int para)
{
	int ret1, ret;
	unsigned int cos_flash_exist_flg = HISEE_FALSE;
	void *p_curr_cos_buf;
	/*3 characters: space,cos_id=3,process_id=COS_PROCESS_CHIP_TEST*/
	char cos_flash_buf_para[MAX_CMD_BUFF_PARAM_LEN] = {0};
	/*3 characters: space,cos_id=0,process_id=COS_PROCESS_CHIP_TEST*/
	char cos_default_buf_para[MAX_CMD_BUFF_PARAM_LEN] = {0};

	ret = check_cos_flash_file_exist(&cos_flash_exist_flg);
	if (HISEE_OK != ret || HISEE_FALSE == cos_flash_exist_flg) {
		pr_err("hisee:%s() cos flash file not exist.\n", __func__);
		if (KEY_READY == get_rpmb_key_status()) {
			ret = HISEE_OK;
		} else {
			ret = HISEE_MULTICOS_COS_FLASH_FILE_ERROR;
		}
		goto err_process;
	}
	if ((NULL == buf) || (HISEE_CHAR_NEWLINE == *(char *)buf)
			|| ('\0' == *(char *)buf)) {
		SET_COS_DEFAULT_BUF_PARA();
		p_curr_cos_buf = (void *)cos_default_buf_para;
	} else {
		p_curr_cos_buf = buf;
	}
	SET_COS_FLASH_BUF_PARA();
	ret = cos_flash_image_upgrade_func(p_curr_cos_buf, para);
	CHECK_OK(ret);
	ret = cos_flash_image_boot_func();
	CHECK_OK(ret);
	hisee_mdelay(DELAY_FOR_HISEE_POWERON_BOOTING); /*lint !e744 !e747 !e748*/

	ret = sm_write_rpmb_key_process();
	if (HISEE_OK != ret) {
		pr_err("hisee:%s() sm_write_rpmb_key_process failed, ret=%d\n", __func__, ret);
		goto poweroff_process;
	}

	ret = run_hisee_nvmformat();
	if (HISEE_OK != ret) {
		pr_err("hisee:%s() run_hisee_nvmformat failed, ret=%d\n", __func__, ret);
		goto poweroff_process;
	}
	ret = hisee_poweroff_func((void *)cos_flash_buf_para, 0);
	CHECK_OK(ret);
	/* wait hisee power down, if timeout or fail, return errno */
	ret = wait_hisee_ready(HISEE_STATE_POWER_DOWN, DELAY_FOR_HISEE_POWEROFF);
	CHECK_OK(ret);

	if (HISEE_FACTORY_TEST_VERSION == para) {
		/*ɾ��/mnt/hisee_fs/cos_flash.img���Ƿ���Ҫ����ע��*/
		filesys_rm_cos_flash_file();
		/* poweron upgrading hisee */
		ret = hisee_poweron_upgrade_func(p_curr_cos_buf, 0);
	    CHECK_OK(ret);
		hisee_mdelay(DELAY_FOR_HISEE_POWERON_UPGRADE); /*lint !e744 !e747 !e748*/
	} else if (HISEE_SERVICE_WORK_VERSION == para) {
		/* poweron booting hisee */
	    ret = hisee_poweron_booting_func(p_curr_cos_buf, HISEE_POWER_ON_BOOTING);
		CHECK_OK(ret);
	}
	goto err_process;
poweroff_process:
	ret1 =  hisee_poweroff_func((void *)cos_flash_buf_para, 0);
	if (HISEE_OK != ret1) {
		pr_err("hisee:%s() hisee_poweroff_func failed1, ret=%d\n", __func__, ret1);
	}
err_process:
    check_and_print_result();
    return ret;
}



static int hisee_manufacture_image_upgrade_process(void *buf, unsigned int hisee_lcs_mode)
{
    int ret;
	unsigned int cos_id = COS_IMG_ID_0;
	unsigned int process_id = 0;

    ret = hisee_poweroff_func(buf, HISEE_PWROFF_LOCK);
    CHECK_OK(ret);

    /* wait hisee power down, if timeout or fail, return errno */
    ret = wait_hisee_ready(HISEE_STATE_POWER_DOWN, DELAY_FOR_HISEE_POWEROFF);
    CHECK_OK(ret);

    ret = hisee_poweron_upgrade_func(buf, 0);
    CHECK_OK(ret);
    hisee_mdelay(DELAY_FOR_HISEE_POWERON_UPGRADE); /*lint !e744 !e747 !e748*/

    if (HISEE_DM_MODE_MAGIC == hisee_lcs_mode) {
		/*DM mode can write rpmbkey multiple with no harm*/
        ret = hisee_write_rpmb_key_process(buf);
        CHECK_OK(ret);
    }
	ret = hisee_get_cosid_processid(buf, &cos_id, &process_id);
	if (HISEE_OK != ret) {
		pr_err("hisee:%s() hisee_get_cosid failed ret=%d\n", __func__, ret);
		goto err_process;
	}
	if (COS_IMG_ID_0 == cos_id) {/*only cos0 support load cos_flash image*/
		ret = load_cos_flash_do_factory_test(buf, HISEE_FACTORY_TEST_VERSION);
		CHECK_OK(ret);
	}

    ret = cos_image_upgrade_func(buf, HISEE_FACTORY_TEST_VERSION);
    CHECK_OK(ret);
    hisee_mdelay(DELAY_FOR_HISEE_POWEROFF); /*lint !e744 !e747 !e748*/

    ret = hisee_poweron_booting_misc_process(buf);
    CHECK_OK(ret);

    if (HISEE_DM_MODE_MAGIC == hisee_lcs_mode) {
        ret = otp_image_upgrade_func(buf, 0);
        CHECK_OK(ret);
    }

err_process:
    check_and_print_result();
    return ret;
}

static int hisee_total_manufacture_func(void *buf, int para)
{
    int ret, ret1;
    unsigned int hisee_lcs_mode = 0;
	void *p_buff_para;
	unsigned int cos_id;
	char factory_slt_test_para[MAX_CMD_BUFF_PARAM_LEN] = {0};
	unsigned int cos_image_num;

	p_buff_para = (void *)factory_slt_test_para;
	cos_image_num = HISEE_SUPPORT_COS_FILE_NUMBER;

	factory_slt_test_para[0] = HISEE_CHAR_SPACE;/*space character*/
	factory_slt_test_para[2] = '0' + COS_PROCESS_UPGRADE;
    ret = get_hisee_lcs_mode(&hisee_lcs_mode);
	CHECK_OK(ret);


	for (cos_id = 0; cos_id < cos_image_num; cos_id++) {
		/* If there is no image for current cos id in hisee_img, bypass upgrading. */
		if (HISEE_COS_EXIST != g_hisee_data.hisee_img_head.is_cos_exist[cos_id]) {
			pr_err("%s: there is no cos%d image in hisee_img!\n", __func__, cos_id);
			continue;
		}

		factory_slt_test_para[1] = '0' + cos_id;
	    ret = hisee_manufacture_image_upgrade_process(p_buff_para, hisee_lcs_mode);
	    CHECK_OK(ret);

	    ret = hisee_verify_isd_key(cos_id);
	    CHECK_OK(ret);

	    ret = hisee_apdu_test_process(cos_id);
	    CHECK_OK(ret);
	    pr_err("hisee:%s() cos_id=%d, success!\n", __func__, cos_id);
	}
	ret = hisee_manufacture_set_lcs_sm(hisee_lcs_mode);
	CHECK_OK(ret);
	pr_err("hisee:%s() set hisee to SM state succes\n", __func__);
	ret = HISEE_OK;
err_process:
    ret1 = hisee_poweroff_func(p_buff_para, HISEE_PWROFF_LOCK);
    if (HISEE_OK == ret) {
		ret = ret1;
    }
    hisee_mdelay(DELAY_FOR_HISEE_POWEROFF);

    if (HISEE_OK == ret) {
        hisee_chiptest_set_otp1_status(PREPARED);
        release_hisee_semphore();/*sync signal for flash_otp_task*/
    }

    set_errno_and_return(ret);
}

static int factory_test_body(void *arg)
{
    int ret;

    if (g_hisee_data.factory_test_state != HISEE_FACTORY_TEST_RUNNING) {
        pr_err("%s hisee factory test state error: %x\n", __func__, g_hisee_data.factory_test_state);
	ret = HISEE_FACTORY_STATE_ERROR;
	goto exit;
    }
    ret = hisee_total_manufacture_func(NULL, 0);
    if (HISEE_OK != ret)
        g_hisee_data.factory_test_state = HISEE_FACTORY_TEST_FAIL;
    else
        g_hisee_data.factory_test_state = HISEE_FACTORY_TEST_SUCCESS;

exit:
    check_and_print_result();
    set_errno_and_return(ret);
} /*lint !e715*/

int hisee_parallel_manufacture_func(void *buf, int para)
{
    int ret = HISEE_OK;
    struct task_struct *factory_test_task = NULL;

    if (HISEE_FACTORY_TEST_RUNNING != g_hisee_data.factory_test_state &&
        false == hisee_chiptest_otp1_is_runing()) {
        g_hisee_data.factory_test_state = HISEE_FACTORY_TEST_RUNNING;
        factory_test_task = kthread_run(factory_test_body, NULL, "factory_test_body");
        if (!factory_test_task) {
            ret = HISEE_THREAD_CREATE_ERROR;
            g_hisee_data.factory_test_state = HISEE_FACTORY_TEST_FAIL;
            pr_err("hisee err create factory_test_task failed\n");
        }
    }
    set_errno_and_return(ret);
}/*lint !e715*/
/* hisee manufacture function end */

/* hisee slt test function begin */
/* hisee slt test function end */

/****************************************************************************//**
 * @brief      : hisee_temp_limit_cfg
 * @param[in]  : flag, on or off
 * @return     : void
 * @note       :
********************************************************************************/
static void hisee_temp_limit_cfg(hisee_tmp_cfg_state flag)
{
	unsigned int value;
	static void __iomem *hisee_temp_addr = 0;

	if (!hisee_temp_addr) {
		hisee_temp_addr = ioremap(HISEE_HIGH_TEMP_PROTECT_ADDR, sizeof(unsigned int));
		if (!hisee_temp_addr) {
			pr_err("hisee init temp addr failed!\n");
			return;
		}
	}
	value = readl(hisee_temp_addr);
	if (HISEE_TEMP_CFG_OFF == flag) {
		value |= BIT(HISEE_HIGH_TEMP_PROTECT_DISABLE_BIT);
	} else {
		value &= (~BIT(HISEE_HIGH_TEMP_PROTECT_DISABLE_BIT));
	}
	writel(value, hisee_temp_addr);
}

/****************************************************************************//**
 * @brief      : hisee_factory_check
 *                  send smc to atf to check factory check values.
 * @param[in]  : NA
 * @return     : ::int
 * @note       :
********************************************************************************/
static int hisee_factory_check(void)
{
    char *buff_virt = NULL;
    phys_addr_t buff_phy = 0;
    atf_message_header *p_message_header;
    int ret = HISEE_OK;
    int image_size;
    unsigned int result_offset;
    u64 result_addr;

    buff_virt = (void *)dma_alloc_coherent(g_hisee_data.cma_device, SIZE_1K * 4,
                                            &buff_phy, GFP_KERNEL);
    if (buff_virt == NULL) {
        pr_err("%s(): dma_alloc_coherent failed\n", __func__);
        set_errno_and_return(HISEE_NO_RESOURCES);
    }
    memset(buff_virt, 0, SIZE_1K * 4);
    p_message_header = (atf_message_header *)buff_virt;
    set_message_header(p_message_header, CMD_HISEE_FACTORY_CHECK);

    image_size = HISEE_ATF_MESSAGE_HEADER_LEN;
    result_offset = HISEE_ATF_MESSAGE_HEADER_LEN;
    p_message_header->test_result_phy = (unsigned int)buff_phy + result_offset;
    p_message_header->test_result_size = SIZE_1K * 4 - result_offset;
    ret = send_smc_process(p_message_header, buff_phy, (unsigned int)image_size,
                            HISEE_ATF_GENERAL_TIMEOUT, CMD_HISEE_FACTORY_CHECK);
    if (HISEE_OK != ret) {
        result_addr = (u64)(buff_virt + result_offset);
        pr_err("%s(): hisee reported fail code=%x %x\n", __func__, *((unsigned int *)(void *)(result_addr)),
			*((unsigned int *)(void *)(result_addr + sizeof(unsigned int))));
    }

    dma_free_coherent(g_hisee_data.cma_device, (unsigned long)(SIZE_1K * 4), buff_virt, buff_phy);
    check_and_print_result();
    set_errno_and_return(ret);
}/*lint !e715*/

/****************************************************************************//**
 * @brief      : hisee_factory_check_body
 * @param[in]  : buf: include the information of cos id, processor id.
 * @param[in]  : para: to indicate it is in which mode, like factory or usr.
 * @return     : ::int
 * @note       :
********************************************************************************/
static int hisee_factory_check_body(void *arg)
{
	int ret = HISEE_OK;
	int ret1;
	void *p_buff_para;
	unsigned int cos_id;
	char factory_slt_test_para[MAX_CMD_BUFF_PARAM_LEN] = {0};
	unsigned int cos_image_num;

	p_buff_para = (void *)factory_slt_test_para;
	cos_image_num = HISEE_SUPPORT_COS_FILE_NUMBER;
	factory_slt_test_para[0] = HISEE_CHAR_SPACE;
	factory_slt_test_para[2] = '0' + COS_PROCESS_UPGRADE;

	/*To avoid enter into the otp flash process in case the status is PREPARED.*/
	hisee_chiptest_set_otp1_status(NO_NEED);
	hisee_temp_limit_cfg(HISEE_TEMP_CFG_OFF);

	for (cos_id = 0; cos_id < cos_image_num; cos_id++) {
		/* If there is no image for current cos id in hisee_img, bypass upgrading. */
		if (HISEE_COS_EXIST != g_hisee_data.hisee_img_head.is_cos_exist[cos_id]) {
			pr_err("%s: there is no cos%d image in hisee_img!\n", __func__, cos_id);
			continue;
		}
		factory_slt_test_para[1] = '0' + cos_id;
		ret = hisee_poweroff_func(p_buff_para, HISEE_PWROFF_LOCK);
		CHECK_OK(ret);
		/* wait hisee power down, if timeout or fail, return errno */
		ret = wait_hisee_ready(HISEE_STATE_POWER_DOWN, DELAY_FOR_HISEE_POWEROFF);
		CHECK_OK(ret);

		ret = hisee_poweron_booting_func(p_buff_para, 0);
		CHECK_OK(ret);

		ret = wait_hisee_ready(HISEE_STATE_COS_READY, HISEE_ATF_GENERAL_TIMEOUT);
		CHECK_OK(ret);

		ret = hisee_factory_check();
		CHECK_OK(ret);
		pr_err("hisee:%s() cos_id=%d, success!\n", __func__, cos_id);
	}
err_process:
	if (HISEE_OK != ret) {
		g_hisee_data.factory_test_state = HISEE_FACTORY_TEST_FAIL;
	} else {
		g_hisee_data.factory_test_state = HISEE_FACTORY_TEST_SUCCESS;
	}
	ret1 = hisee_poweroff_func(p_buff_para, HISEE_PWROFF_LOCK);
	if (HISEE_OK == ret){
		ret = ret1;
	}
	hisee_temp_limit_cfg(HISEE_TEMP_CFG_ON);;
	set_errno_and_return(ret);
}

/****************************************************************************//**
 * @brief      : hisee_factory_check_func
 * @param[in]  : buf: include the information of cos id, processor id.
 * @param[in]  : para: to indicate it is in which mode, like factory or usr.
 * @return     : ::int
 * @note       :
********************************************************************************/
int hisee_factory_check_func(void *buf, int para)
{
    int ret = HISEE_OK;
    struct task_struct *factory_check_test_task = NULL;

    if (HISEE_FACTORY_TEST_RUNNING != g_hisee_data.factory_test_state) {
        g_hisee_data.factory_test_state = HISEE_FACTORY_TEST_RUNNING;
        factory_check_test_task = kthread_run(hisee_factory_check_body, NULL, "factory_check_test_body");
        if (!factory_check_test_task) {
            ret = HISEE_THREAD_CREATE_ERROR;
            g_hisee_data.factory_test_state = HISEE_FACTORY_TEST_FAIL;
            pr_err("hisee err create factory_check_test_task failed\n");
        } else
            pr_err("%s() success!\n", __func__);
    }
    set_errno_and_return(ret);
}



/****************************************************************************//**
 * @brief      : hisee_at_result_show, use this function to print AT result
 * @param[in]  : dev
 * @param[in]  : attr
 * @param[in]  : buf
 * @return     : ::ssize_t
 * @note       :
********************************************************************************/
ssize_t hisee_at_result_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    int ret = HISEE_ERROR;

    if (NULL == buf) {
        pr_err("%s buf paramters is null\n", __func__);
        set_errno_and_return(HISEE_INVALID_PARAMS);
    }
    *buf = 0;
    ret = atomic_read(&g_hisee_errno);

    switch (g_at_cmd_type) {
        case HISEE_AT_CASD:
            if (HISEE_OK == ret) {
                snprintf(buf, (size_t)HISEE_BUF_SHOW_LEN, "^HISEE:CASD,%d", g_hisee_data.casd_data.curr_pack);
            } else if (HISI_CASD_HASH_ERROR == ret) {
                snprintf(buf, (size_t)HISEE_BUF_SHOW_LEN, "^HISEE:CASD,%d,TRANSMIT,-1", g_hisee_data.casd_data.curr_pack);
            } else {
                snprintf(buf, (size_t)HISEE_BUF_SHOW_LEN, "^HISEE:CASD,%d,TRANSMIT,0", g_hisee_data.casd_data.curr_pack);
            }
            break;
        case HISEE_AT_VERIFYCASD:
            if (HISEE_OK == ret) {
                snprintf(buf, (size_t)HISEE_BUF_SHOW_LEN, "^HISEE:VERIFYCASD,%d", g_hisee_data.casd_data.curr_pack);
            } else {
                snprintf(buf, (size_t)HISEE_BUF_SHOW_LEN, "^HISEE:VERIFYCASD,%d,VERIFY,-2", g_hisee_data.casd_data.curr_pack);
            }
            break;
        default:
            snprintf(buf, (size_t)HISEE_BUF_SHOW_LEN, "^HISEE:Para Error");
            break;
    }
    g_at_cmd_type = HISEE_AT_MAX;
    return (ssize_t)strlen(buf);
}

