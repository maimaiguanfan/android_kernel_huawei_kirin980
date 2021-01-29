
#ifndef  __PARAM_TO_SEC_OS_H__
#define  __PARAM_TO_SEC_OS_H__

#ifdef __cplusplus
extern "C"
{
#endif


#include <osl_types.h>
#include "product_config.h"

/* ���ö�ٵ��޸���Ҫͬ���޸�һ�¼����ط�
mbb tzdriver
vendor/hisi/system/kernel/drivers/hisi/tzdriver/teek_client_id.h
mbb secos
vendor/hisi/system/secure_os/trustedcore/include/TEE/tee_common.h
vendor/hisi/system/secure_os/trustedcore/platform/balong/include/bsp_param_cfg.h
mbb��phone:ccore modem
vendor/hisi/system/kernel/drivers/hisi/modem/drv/common/include/param_cfg_to_sec.h
phone tzdriver
vendor/hisi/ap/kernel/drivers/tzdriver/teek_client_id.h
phone secos
vendor/thirdparty/secure_os/trustedcore/include/TEE/tee_common.h
vendor/thirdparty/secure_os/trustedcore/platform/kirin/secureboot/secboot.h
*/
#ifdef BSP_CONFIG_PHONE_TYPE
#ifdef FEATURE_KEEP_LEGICY_SEC_OS
enum SVC_SECBOOT_IMG_TYPE{
    MODEM,
    HIFI,
    DSP,
    XDSP,
    TAS,
    WAS,
    CAS,
    MODEM_DTB,
    ISP,
    MODEM_CERT,
    SOC_MAX
};
#else
enum SVC_SECBOOT_IMG_TYPE {
    MODEM,
    DSP,
    XDSP,
    TAS,
    WAS,
    CAS,
    MODEM_DTB,
    NVM,
    NVM_S,
    MBN_R,
    MBN_A,
    MODEM_COLD_PATCH,
    DSP_COLD_PATCH,
    MODEM_CERT,
    MAX_SOC_MODEM,
    HIFI,
    ISP,
    IVP,
    SOC_MAX
};
#endif
#else
enum SVC_SECBOOT_IMG_TYPE{
    MODEM,
    HIFI,
    DSP,
    XDSP,
    TAS,
    WAS,
    CAS,
    MODEM_DTB,
    ISP,
    NVM,
    NVM_S,
    MBN_R,
    MBN_A,
    MODEM_CERT,
    SOC_MAX,
    MAX_SOC_MODEM = SOC_MAX
};
#endif


#define PARAM_MAGIC_OFFSET            (0x0)           /* ħ��:�жϲ�����Ч�� */
#define PARAM_CFG_OFFSET              (0x8)           /* ���ò�����Ż���ַ*/

#ifdef DDR_SECURE_OS_ADDR
/*ϵͳ���������д���C�˾���ִ�е�ַ�������С�Ȳ�������ȫOS��
�����Ĵ�ŵ�ַ��֮ǰ�İ�ȫ�����ڴ�ת�Ƶ���ȫOS ���ڴ��У�
��ȫOSΪDX����1MB���ڴ�ռ䣬λ�ڰ�ȫOS�ڴ���ʼ��ַƫ��0x500000����
fastbootʹ��DX ��1M�ڴ�ռ�����16KB�����Ҫ���ݵ���ȫOS�Ĳ�����
16KB������2KB���ڴ�Ų����ĵ�ַ��Ϣ��ʣ���14KB���ڴ����Ҫ���ݵĲ���*/
#define BALONG_PARAM_BASE_ADDR        (DDR_SECURE_OS_ADDR+0x600000 -0x800)    /* 0xA07FF800--0xA07FF8FF */
#define BALONG_SEC_NV_BASE_ADDR       (DDR_SECURE_OS_ADDR+0x600000 - 0x1000)
#define SECBOOT_PARAM_CFG_BASE_ADDR    (DDR_SECURE_OS_ADDR+0x600000 -0x4000)
#define SECBOOT_PARAM_CFG_SIZE    0x3000//0x3800
#endif

/*sec boot start */
struct image_info
{
    u64 ddr_addr;
    unsigned int ddr_size;
    unsigned int unreset_dependcore;
};

struct dynamic_load
{
    u32 load_cmd;
    u32 verify_flag;
};

#ifdef FEATURE_KEEP_LEGICY_SEC_OS
struct sec_boot_modem_info
{
    struct image_info image_info[SOC_MAX];
    struct dynamic_load dynamic_load;
};
#else
struct sec_boot_modem_info
{
    struct dynamic_load dynamic_load;
    struct image_info image_info[MAX_SOC_MODEM];
};
#endif
 /*sec boot end */

typedef struct tag_PARAM_CFG
{
    u32   magic;                   /* ħ������ʶ���ò�����״̬ */
    u32   protect_barrier_size;    /* Ԥ��(4K)��ֹ���ȣ���ʼ��ΪȫF */
    u32   param_cfg_size;          /* ���ò���Ԥ��(16K)��С */
    u64   icc_channel_base_addr;
    u32   icc_channel_max_size;
    struct sec_boot_modem_info sec_boot_modem_info;
}g_param_cfg;

void set_param_cfg_to_sec(void);

void set_modem_info_to_sec(void);

#ifdef __cplusplus
}
#endif

#endif   /* __PARAM_TO_SEC_OS_H__ */

