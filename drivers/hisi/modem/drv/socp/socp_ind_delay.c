/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include "socp_ind_delay.h"
#include <securec.h>

#include "socp_balong.h"

#include "deflate.h"

#include <linux/vmalloc.h>
#include <linux/device.h>
#include <linux/of_platform.h>
#include "bsp_socp.h"

#include <linux/of_reserved_mem.h>
#include <linux/of_fdt.h>
#include <nv_stru_drv.h>

#define  THIS_MODU mod_socp
struct socp_enc_dst_log_cfg g_stEncDstBufLogConfig = {NULL,0,0,10,false,10,false,0,0};
struct socp_enc_dst_log_cfg g_stDeflateDstBufLogConfig = {NULL,0,0,10,false,10,false,0,0};
u32 g_stDeflateState =0;
socp_early_cfg_stru         g_stSocpEarlyCfg    = {NULL,0,0,0,0,0};
socp_mem_reserve_stru       g_stSocpMemReserve  = {NULL,0,0,0,0};
DRV_DEFLATE_CFG_STRU  g_deflate_nv_ctrl={0,0};
extern u32 socp_version;
u64 g_socp_dma_mask = (u64)(-1);
extern u32 g_strSocpDeflateStatus;
s32 deflate_set_compress_mode(SOCP_IND_MODE_ENUM eMode);

/*****************************************************************************
* �� �� ��  : socp_get_logbuffer_sizeparse
*
* ��������  : ��ȡsocp buffer��С
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : ��
*****************************************************************************/
s32  socp_get_logbuffer_sizeparse(void)
{
    u32      ulBufferSize;

    /* Buffer�Ĵ�С��ByteΪ��λ��ԭ���ϲ�����200M����С��1M */
    ulBufferSize = bsp_socp_get_logbuffer_size();

    if ((ulBufferSize > SOCP_MAX_MEM_SIZE )
      || (ulBufferSize < SOCP_MIN_MEM_SIZE))
    {
        socp_crit("cmdline: BuffSize=0x%x\n",ulBufferSize);
        g_stSocpEarlyCfg.ulBufUsable = BSP_FALSE;
        return -1;
    }

    /* Ϊ�˱���ulBufferSize�ĳ���8�ֽڶ���,������Ȳ���8�ֽڶ����ַҲ���� */
    if (0 != (ulBufferSize % 8))
    {
        socp_error("BuffSize no 8 byte allignment,BuffSize: 0x%x\n",ulBufferSize);
        g_stSocpEarlyCfg.ulBufUsable = BSP_FALSE;
        return -1;
    }
    g_stSocpEarlyCfg.ulBufferSize = ulBufferSize;
    socp_crit("BufferSize 0x%x, adapt buffer_size: 0x%x\n",
                ulBufferSize,
                g_stSocpEarlyCfg.ulBufferSize);

    g_stSocpEarlyCfg.ulBufUsable  =  BSP_TRUE;
    return 0;
}


/*****************************************************************************
* �� �� ��  : socp_get_logbuffer_timeparse
*
* ��������  : ��ȡsocpʱ��
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : ��
*****************************************************************************/
s32  socp_get_logbuffer_timeparse(void)
{
    u32      ulTimeout;

    /* �����ַ�������Ϊ��λ����Ҫ��ת���ɺ��룬����Ϊ1�룬������20���� */
    ulTimeout = bsp_socp_get_logbuffer_time();

    if (SOCP_MAX_TIMEOUT < ulTimeout)
    {

        ulTimeout = SOCP_MAX_TIMEOUT;
    }

    ulTimeout *= 1000;

    g_stSocpEarlyCfg.ulTimeout  = ulTimeout;

    socp_crit("early_cfg: timeout=0x%x\n", g_stSocpEarlyCfg.ulTimeout);

    return 0;
}


/*****************************************************************************
* �� �� ��  : socp_get_logbuffer_addrparse
*
* ��������  : ��ȡ�ڴ����ַ
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : ��
*****************************************************************************/
s32 socp_get_logbuffer_addrparse(void)
{
    unsigned long  ulBaseAddr;

    ulBaseAddr = bsp_socp_get_logbuffer_addr();

    /* �����ַ��32λ��ʵ��ַ������8�ֽڶ���� */
    if ((0 != (ulBaseAddr % 8))
        || (0 == ulBaseAddr))
    {
        return -1;
    }

    g_stSocpEarlyCfg.ulPhyBufferAddr = ulBaseAddr;

    return 0;
}
/*****************************************************************************
* �� �� ��  : socp_get_cmdline_param
*
* ��������  : ��ȡcmdline����
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : ��
*****************************************************************************/
void socp_get_cmdline_param(void)
{
    (void)socp_get_logbuffer_sizeparse();
    (void)socp_get_logbuffer_timeparse();
    (void)socp_get_logbuffer_addrparse();
    (void)socp_get_logbuffer_logcfg();
    return ;

}
/*****************************************************************************
* �� �� ��  : socp_get_logbuffer_logcfg
*
* ��������  : ��ȡ50M bufferʹ��λ
*
* �������  : �����ַ���
*
* �������  : ��
*
* �� �� ֵ  : �ɹ����
*****************************************************************************/
s32  socp_get_logbuffer_logcfg(void)
{
    unsigned int  ulMemLogCfg;

    ulMemLogCfg= bsp_socp_get_logbuffer_logcfg();

    g_stSocpEarlyCfg.ulLogCfg = ulMemLogCfg;

    socp_crit("early_cfg: memLogCfg=0x%x\n", g_stSocpEarlyCfg.ulLogCfg);

    return 0;
}


/*lint -save -e785*/
static const struct of_device_id socp_dev_of_memalloc_match[] = {
        {.compatible = "hisilicon,socp_balong_memalloc"},
	    {},
    };
/*lint -restore +e785*/
/*****************************************************************************
* �� �� ��  : socp_memalloc_probe
*
* ��������  : 50M�ڴ����������豸��probe����
*
* �������  : �����豸pdev
*
* �������  : ��
*
* �� �� ֵ  : �ɹ����
*****************************************************************************/

static int __init socp_memalloc_probe(struct platform_device *pdev)
{
    u32 ulTimeout=0;
    s32 ret;
    struct device_node* dev ;
    struct device *pdevice = &(pdev->dev);

    if(g_stSocpEarlyCfg.ulLogCfg)
    {
        ret=of_reserved_mem_device_init(pdevice);
        if(ret<0)
        {
             socp_error("mem_device_init fail\n" );
             return -1;
        }
		socp_crit( "[init]mem_device_init sucess!\n");
    }
	else
	{
		socp_crit("socp_memalloc_probe: mdmlog not open!\n" );
		return -1;
	}

    dev = of_find_node_by_name(NULL, "hisi_mdmlog");
    if(NULL == dev)
    {
        socp_error("of_find_node_by_name failed!\n");
        return -1;
    }

    ret = of_property_read_u32_index(dev,"time",0, &ulTimeout);
    if(ret)
    {
        socp_error("of_property_read_u32_index failed!\n");
        return -1;
    }

    socp_crit("of_property_read_u32_index get time 0x%x!\n",ulTimeout);

    if (SOCP_MAX_TIMEOUT < ulTimeout)
    {
        ulTimeout = SOCP_MAX_TIMEOUT;
    }

    ulTimeout *= 1000;

    g_stSocpMemReserve.ulTimeout = ulTimeout;

    return 0;

}
/*lint -save -e785 -e64*/
static struct platform_driver socp_mem_driver = {
    .driver = {
               .name = "modem_socp_memalloc",
               .owner = THIS_MODULE,
               .of_match_table = socp_dev_of_memalloc_match,
    },
    .probe = socp_memalloc_probe,
};
/*lint -restore +e785 +e64*/

void *socp_logbuffer_memremap(unsigned long phys_addr, size_t size)
{
    unsigned long i;
    u8* vaddr;
    unsigned long npages = (PAGE_ALIGN((phys_addr & (PAGE_SIZE - 1)) + size) >> PAGE_SHIFT);
    unsigned long offset = phys_addr & (PAGE_SIZE - 1);
    struct page **pages;

    pages = vmalloc(sizeof(struct page *) * npages);
    if (!pages)
    {
        return NULL;
    }

    pages[0] = phys_to_page(phys_addr);

    for (i = 0; i < npages - 1 ; i++)
    {
        pages[i + 1] = pages[i] + 1;
    }

    vaddr = (u8*)vmap(pages, (unsigned int)npages, (unsigned long)VM_MAP, pgprot_writecombine(PAGE_KERNEL));

    if ( NULL != vaddr )

    {
        vaddr += offset;
    }

    vfree(pages);

    return (void *)vaddr;
}

/*****************************************************************************
* �� �� ��  : socp_logbuffer_mmap
*
* ��������  : �����ַת���������ڴ�
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : ��
*****************************************************************************/
s32 socp_logbuffer_mmap(void)
{
    /*step1: if logcfg is on, mmap kernel reserved memory */
    if(SOCP_RESERVED_TRUE == g_stSocpEarlyCfg.ulLogCfg)
    {
        if(BSP_TRUE == g_stSocpMemReserve.ulBufUsable)
        {
            /* ӳ�������ַ */
            g_stSocpMemReserve.pVirBuffer = socp_logbuffer_memremap(g_stSocpMemReserve.ulPhyBufferAddr,
                                                                    (size_t)g_stSocpMemReserve.ulBufferSize);
            if(NULL == g_stSocpMemReserve.pVirBuffer)
            {
                g_stSocpMemReserve.ulBufUsable = BSP_FALSE;
                return BSP_ERROR;
            }

            socp_crit("kernel reserved buffer mmap success\n");
            return BSP_OK;
        }
        else
        {
            return BSP_ERROR;
        }
    }

    /*step2: if buffer addr is invalid or buffer size is invalide,
             it means fastboot do not reserve memory to us,
             or memory reserved is invalid, do nothing*/
    if(BSP_FALSE == g_stSocpEarlyCfg.ulBufUsable)
    {
        return BSP_OK;
    }

    /*step3: memmap socp buffer, and record virtual buffer address */
    g_stSocpEarlyCfg.pVirBuffer = socp_logbuffer_memremap(g_stSocpEarlyCfg.ulPhyBufferAddr,
                                                          (size_t)g_stSocpEarlyCfg.ulBufferSize);
    if(NULL == g_stSocpEarlyCfg.pVirBuffer)
    {
        g_stSocpEarlyCfg.ulBufUsable = BSP_FALSE;
        return BSP_ERROR;
    }

    g_stSocpEarlyCfg.ulBufUsable = BSP_TRUE;

    socp_crit("fastboot resered buffer is valid\n");

    return BSP_OK;
}

/* log2.0 2014-03-19 Begin:*/
/*****************************************************************************
* �� �� ��  : bsp_socp_get_log_cfg
*
* ��������  : ��ȡLOG2.0 SOCPˮ�ߡ���ʱ������Ϣ
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : SOCP_ENC_DST_BUF_LOG_CFG_STRUָ��
*****************************************************************************/
struct socp_enc_dst_log_cfg * bsp_socp_get_log_cfg(void)
{
    return &g_stEncDstBufLogConfig;
}
/*****************************************************************************
* �� �� ��  : bsp_socp_get_log_ind_mode
*
* ��������  : �ϱ�ģʽ�ӿ�
*
* �������  : ģʽ����
*
* �������  : ��
*
* �� �� ֵ  : BSP_S32 BSP_OK:�ɹ� BSP_ERROR:ʧ��
*****************************************************************************/
s32  bsp_socp_get_log_ind_mode(u32 *LofgIndMode)
{
    *LofgIndMode = g_stEncDstBufLogConfig.currentMode;
    return  BSP_OK;
}

u32 bsp_socp_get_sd_logcfg(SOCP_ENC_DST_BUF_LOG_CFG_STRU* cfg)
{
    struct socp_enc_dst_log_cfg* LogCfg;
    u32  deflate_stat;
    if(NULL == cfg)
        return BSP_ERR_SOCP_INVALID_PARA;

    LogCfg = bsp_socp_get_log_cfg();

    cfg->logOnFlag       = LogCfg->logOnFlag;
    cfg->BufferSize      = LogCfg->BufferSize;

    deflate_stat =bsp_socp_compress_status();
    if(1 == deflate_stat)
    {
        cfg->overTime        = g_stDeflateDstBufLogConfig.overTime;
    }
    else
    {

        cfg->overTime        = LogCfg->overTime;
    }
    cfg->pVirBuffer      = LogCfg->pVirBuffer;
    cfg->ulCurTimeout    = LogCfg->ulCurTimeout;
    cfg->ulPhyBufferAddr = LogCfg->ulPhyBufferAddr;

    return BSP_OK;
}
/*****************************************************************************
* �� �� ��  : socp_logbuffer_dmalloc
* ��������  : ��̬�����ڴ�
* �������  : �豸�ڵ�
* �������  : ��
* �� �� ֵ  : ���óɹ�����־
*****************************************************************************/
s32 socp_logbuffer_dmalloc(struct device_node* dev)
{
    dma_addr_t      ulAddress = 0;
    u8              *pucBuf;
    int             ret;
    u32             aulDstChan[SOCP_DST_CHAN_CFG_BUTT]={0};
    u32             size;
    struct device dev1;

    ret = of_property_read_u32_array(dev, "dst_chan_cfg", aulDstChan, (size_t)SOCP_DST_CHAN_CFG_BUTT);
    if(ret)
    {
        socp_crit("dts don't config dmalloc logbuffer size, use default size 1M!\n");
        size = 1*1024*1024;
    }
    else
    {
        socp_crit("of_property_read_u32_array get size 0x%x!\n", aulDstChan[SOCP_DST_CHAN_CFG_SIZE]);
        size = aulDstChan[SOCP_DST_CHAN_CFG_SIZE];
    }
    memset_s(&dev1,sizeof(dev1),0,sizeof(dev1));
    dma_set_mask_and_coherent(&dev1, g_socp_dma_mask);
    of_dma_configure(&dev1, NULL);
    pucBuf =(u8 *) dma_alloc_coherent(&dev1, (size_t)size, &ulAddress, GFP_KERNEL);

    if(BSP_NULL == pucBuf)
    {
        socp_error("logbuffer_dmalloc buffer failed\n");
        return BSP_ERROR;
    }

    socp_crit("socp_logbuffer_dmalloc success\n");

    g_stEncDstBufLogConfig.ulPhyBufferAddr  = (unsigned long)ulAddress;
    g_stEncDstBufLogConfig.pVirBuffer       = pucBuf;
    g_stEncDstBufLogConfig.ulCurTimeout     = 10; /* ʹ��Ĭ��ֵ */
    g_stEncDstBufLogConfig.BufferSize       = size;
    g_stEncDstBufLogConfig.logOnFlag        = SOCP_DST_CHAN_DTS;

    g_stDeflateDstBufLogConfig.ulPhyBufferAddr  = (unsigned long)ulAddress;
    g_stDeflateDstBufLogConfig.pVirBuffer       = pucBuf;
    g_stDeflateDstBufLogConfig.ulCurTimeout     = 10; /* ʹ��Ĭ��ֵ */
    g_stDeflateDstBufLogConfig.BufferSize       = size;
    g_stDeflateDstBufLogConfig.logOnFlag        = SOCP_DST_CHAN_DTS;
    return BSP_OK;
}


/*****************************************************************************
* �� �� ��  : bsp_socp_timeout_init
*
* ��������  : ģ���ʼ��Ŀ��buffer�ϱ���ʱ���õĺ���
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : ��ʼ���ɹ��ı�ʶ��
*****************************************************************************/
s32 bsp_socp_logbuffer_init(struct device_node* dev)
{
    s32 ret;

    /*step1: if modlog config enabled, use kernel reserved buffer */
    if(SOCP_RESERVED_TRUE ==g_stSocpEarlyCfg.ulLogCfg)
    {
        /*step1.1: if kernel reserved buffer is valid, enable ind delay */
        if(BSP_TRUE == g_stSocpMemReserve.ulBufUsable)
        {
            g_stEncDstBufLogConfig.pVirBuffer       = g_stSocpMemReserve.pVirBuffer;
            g_stEncDstBufLogConfig.ulPhyBufferAddr  = g_stSocpMemReserve.ulPhyBufferAddr;
            g_stEncDstBufLogConfig.BufferSize       = g_stSocpMemReserve.ulBufferSize;
            g_stEncDstBufLogConfig.overTime         = g_stSocpMemReserve.ulTimeout;
            g_stEncDstBufLogConfig.logOnFlag        = SOCP_DST_CHAN_DELAY;
            g_stDeflateDstBufLogConfig.pVirBuffer       = g_stSocpMemReserve.pVirBuffer;
            g_stDeflateDstBufLogConfig.ulPhyBufferAddr  = g_stSocpMemReserve.ulPhyBufferAddr;
            g_stDeflateDstBufLogConfig.BufferSize       = g_stSocpMemReserve.ulBufferSize;
            g_stDeflateDstBufLogConfig.overTime         = g_stSocpMemReserve.ulTimeout;
            g_stDeflateDstBufLogConfig.logOnFlag        = SOCP_DST_CHAN_DELAY;

        }
        /*step1.2: if kernel reserved buffer is invalid, disable ind delay, use 1M malloc buffer */
        else
        {
            g_stEncDstBufLogConfig.logOnFlag        = SOCP_DST_CHAN_NOT_CFG;
            g_stDeflateDstBufLogConfig.logOnFlag        = SOCP_DST_CHAN_NOT_CFG;

        }
    }
    /*step2: if modlog config is not parsed, use fastboot reserved memory*/
    else
    {
        /*step2.1: if fastboot reserved memory is valid, enable ind delay*/
        if(BSP_TRUE == g_stSocpEarlyCfg.ulBufUsable)
        {
            g_stEncDstBufLogConfig.pVirBuffer       = g_stSocpEarlyCfg.pVirBuffer;
            g_stEncDstBufLogConfig.ulPhyBufferAddr  = g_stSocpEarlyCfg.ulPhyBufferAddr;
            g_stEncDstBufLogConfig.BufferSize       = g_stSocpEarlyCfg.ulBufferSize;
            g_stEncDstBufLogConfig.overTime         = g_stSocpEarlyCfg.ulTimeout;
            g_stEncDstBufLogConfig.logOnFlag        = SOCP_DST_CHAN_DELAY;
            g_stDeflateDstBufLogConfig.pVirBuffer       = g_stSocpEarlyCfg.pVirBuffer;
            g_stDeflateDstBufLogConfig.ulPhyBufferAddr  = g_stSocpEarlyCfg.ulPhyBufferAddr;
            g_stDeflateDstBufLogConfig.BufferSize       = g_stSocpEarlyCfg.ulBufferSize;
            g_stDeflateDstBufLogConfig.overTime         = g_stSocpEarlyCfg.ulTimeout;
            g_stDeflateDstBufLogConfig.logOnFlag        = SOCP_DST_CHAN_DELAY;

        }
        /*step2.2: if fastboot reserved memory is invalid, use 1M malloc buffer, disable ind delay*/
        else
        {
             g_stEncDstBufLogConfig.logOnFlag=SOCP_DST_CHAN_NOT_CFG;
             g_stDeflateDstBufLogConfig.logOnFlag=SOCP_DST_CHAN_NOT_CFG;
        }
    }

    /*step3: use dmalloc buffer, disable ind delay*/
    /* �ӳ��ϱ�û��ʱ�ж�DTSI���Ƿ�����Ŀ��ͨ����buffer size */
    if(SOCP_DST_CHAN_NOT_CFG == g_stEncDstBufLogConfig.logOnFlag)
    {
        ret = socp_logbuffer_dmalloc(dev);
        if(ret)
        {
            socp_error("of_property_read_u32_array failed!\n");
            return BSP_ERROR;
        }
    }

    return BSP_OK;
}


/*****************************************************************************
* �� �� ��  : bsp_socp_timeout_init
*
* ��������  : ģ���ʼ��Ŀ��buffer�ϱ���ʱ���õĺ���
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : ��ʼ���ɹ��ı�ʶ��
*****************************************************************************/
void bsp_socp_timeout_init(void)
{
    bsp_socp_set_timeout(SOCP_TIMEOUT_TRF,  SOCP_MIN_TIMEOUT*2289/1000);

    return;
}

void bsp_socp_set_mode_direct(void)
{
    u32 ret;

    if(SOCP_IND_MODE_DIRECT == g_stEncDstBufLogConfig.currentMode)
    {
        socp_crit("the ind mode direct is already config!\n");
        return;
    }

    if(socp_version < SOCP_206_VERSION)
    {
        (void)bsp_socp_set_timeout(SOCP_TIMEOUT_TRF, 0x17);
    }
    else
    {
        (void)bsp_socp_set_timeout(SOCP_TIMEOUT_TRF_LONG, SOCP_TIMEOUT_TRF_LONG_MIN);
    }
    g_stEncDstBufLogConfig.ulCurTimeout = 10;
    bsp_socp_set_enc_dst_threshold((bool)FALSE,SOCP_CODER_DST_OM_IND);
    ret = bsp_socp_encdst_set_cycle(SOCP_CODER_DST_OM_IND, SOCP_IND_MODE_DIRECT);
    if(BSP_OK != ret)
    {
        socp_error("direct mode config failed!\n");
    }
    else
    {
        g_stEncDstBufLogConfig.currentMode = SOCP_IND_MODE_DIRECT;
        socp_crit("direct mode config sucess!\n");
    }
}

void bsp_socp_set_mode_delay(void)
{
    u32 time;
    u32 ret;

    if(SOCP_IND_MODE_DELAY == g_stEncDstBufLogConfig.currentMode)
    {
        socp_crit("socp:the ind mode delay is already config!\n");
        return;
    }

    /* if logbuffer is not configed, can't enable delay mode*/
    if(g_stEncDstBufLogConfig.logOnFlag == SOCP_DST_CHAN_DELAY)
    {
        if(socp_version < SOCP_206_VERSION)
        {
            time = (g_stEncDstBufLogConfig.overTime * 2289)/1000;
            (void)bsp_socp_set_timeout(SOCP_TIMEOUT_TRF, time);
            g_stEncDstBufLogConfig.ulCurTimeout = g_stEncDstBufLogConfig.overTime;
        }
        else
        {
           (void)bsp_socp_set_timeout(SOCP_TIMEOUT_TRF_LONG, SOCP_TIMEOUT_TRF_LONG_MAX);
        }
        g_stEncDstBufLogConfig.ulCurTimeout = SOCP_TIMEOUT_TRF_LONG_MAX;
        bsp_socp_set_enc_dst_threshold((bool)TRUE,SOCP_CODER_DST_OM_IND);
        ret = bsp_socp_encdst_set_cycle(SOCP_CODER_DST_OM_IND, SOCP_IND_MODE_DELAY);
        if(BSP_OK != ret)
        {
            socp_error("delay mode config failed!\n");
        }
        else
        {
            g_stEncDstBufLogConfig.currentMode = SOCP_IND_MODE_DELAY;
            socp_crit("delay mode config sucess!\n");
        }
    }
    else
    {
        socp_crit("delay mode can't config:mem can't be setted!\n");
    }
}

void bsp_socp_set_mode_cycle(void)
{
    u32 ret;

    if(SOCP_IND_MODE_CYCLE == g_stEncDstBufLogConfig.currentMode)
    {
        socp_crit("the ind mode cycle is already config!\n");
        return;
    }

    /* if logbuffer is not configed, can't enable cycle mode*/
    /* ���dst��ʽmalloc 10M�ڴ�,Ҳ֧������ѭ��ģʽ��֧������10Mlogץȡ*/
    if(((g_stEncDstBufLogConfig.logOnFlag == SOCP_DST_CHAN_DELAY)&&(BSP_TRUE == g_stSocpMemReserve.ulBufUsable))||
        (g_stEncDstBufLogConfig.logOnFlag == SOCP_DST_CHAN_DTS))
    {
        ret = bsp_socp_encdst_set_cycle(SOCP_CODER_DST_OM_IND, SOCP_IND_MODE_CYCLE);
        if(BSP_OK != ret)
        {
            socp_error("cycle mode config failed!\n");
        }
        else
        {
            g_stEncDstBufLogConfig.currentMode = SOCP_IND_MODE_CYCLE;
            socp_crit("the ind cycle mode config sucess!\n");
        }
    }
    else
    {
        socp_crit("ind delay can't config:mem can't be setted!\n");
    }
}

/*****************************************************************************
* �� �� ��  : bsp_socp_set_ind_mode
*
* ��������  : �ϱ�ģʽ�ӿ�
*
* �������  : ģʽ����
*
* �������  : ��
*
* �� �� ֵ  : BSP_S32 BSP_OK:�ɹ� BSP_ERROR:ʧ��
*****************************************************************************/
s32 bsp_socp_set_ind_mode(SOCP_IND_MODE_ENUM eMode)
{
    switch (eMode)
    {
        case SOCP_IND_MODE_DIRECT:
        {
            bsp_socp_set_mode_direct();
            break;
        }
        case SOCP_IND_MODE_DELAY:
        {
            bsp_socp_set_mode_delay();
            break;

        }
        case SOCP_IND_MODE_CYCLE:
        {
            bsp_socp_set_mode_cycle();
            break;
        }
        default:
        {
            socp_error("set invalid mode: %d!\n",g_stEncDstBufLogConfig.currentMode);
            return  BSP_ERROR;
        }
    }

    return BSP_OK ;
 }



/*****************************************************************************
  �� �� ��  : bsp_socp_compress_status
  ��������  : ��ѯ��ǰѹ��ģʽ�Ƿ���
  �������  : ��
  �������  : ��
  �� �� ֵ  : ��

*****************************************************************************/

s32 bsp_socp_compress_status(void)
{
    return g_strSocpDeflateStatus;
}
/*****************************************************************************
  �� �� ��  : bsp_socp_get_cfg_ind_mode
  ��������  : ��ѯ��ȡ��ǰ�ϱ�ģʽ
  �������  : ��
  �������  : ��
  �� �� ֵ  : ��

*****************************************************************************/

s32 bsp_socp_get_cfg_ind_mode(u32 *CfgIndMode)
{
    u32 ulDeflateState;
    ulDeflateState = bsp_socp_compress_status();
    if(SOCP_COMPRESS == ulDeflateState)
    {
        bsp_deflate_get_log_ind_mode(CfgIndMode);
        socp_crit("deflate channel is open!\n");
        return BSP_OK;
    }
    else
    {
        bsp_socp_get_log_ind_mode(CfgIndMode);
        socp_crit("socp channel is open!\n");
        return BSP_OK;
    }
}
/*****************************************************************************
  �� �� ��  : bsp_socp_set_cfg_ind_mode
  ��������  : ���õ�ǰ�ϱ�ģʽ
  �������  : ��
  �������  : ��
  �� �� ֵ  : ��

*****************************************************************************/

s32 bsp_socp_set_cfg_ind_mode(SOCP_IND_MODE_ENUM eMode)
{
    u32 ulDeflateState;
    ulDeflateState = bsp_socp_compress_status();
    if(SOCP_NO_COMPRESS==ulDeflateState)
    {
        bsp_socp_set_ind_mode(eMode);
        socp_crit("socp channel is open!\n");
        return BSP_OK;
    }
    else
    {
        bsp_deflate_set_ind_mode(eMode);
        socp_crit("deflate channel is open!\n");
    }
    return   BSP_OK;
}
/*****************************************************************************
  �� �� ��  : bsp_socp_get_cps_ind_mode
  ��������  : ��ȡ��ǰѹ��ģʽ
  �������  : ��
  �������  : ��
  �� �� ֵ  : ��

*****************************************************************************/
s32 bsp_socp_get_cps_ind_mode(u32 *CpsIndMode)
{
    u32 ulDeflateState;
    ulDeflateState = bsp_socp_compress_status();
    if(SOCP_COMPRESS==ulDeflateState)
    {
        *CpsIndMode = DEFLATE_IND_COMPRESS ;
        socp_crit("deflate channel is open!\n");
        return BSP_OK;
    }
    else
    {
        *CpsIndMode = DEFLATE_IND_NO_COMPRESS;
        socp_crit("socp channel is open!\n");
        return BSP_OK;
    }

}
/*****************************************************************************
  �� �� ��  : bsp_socp_set_cps_ind_mode
  ��������  : ���õ�ǰѹ��ģʽ
  �������  : ��
  �������  : ��
  �� �� ֵ  : ��

*****************************************************************************/
s32 bsp_socp_set_cps_ind_mode(DEFLATE_IND_COMPRESSS_ENUM eMode)
{
    u32 ulDeflateState;
    if(0 == g_deflate_nv_ctrl.deflate_enable)
    {
        socp_crit("deflate nv not open!\n");
        return BSP_ERROR;

    }
    ulDeflateState = bsp_socp_compress_status();
    if(DEFLATE_IND_NO_COMPRESS==eMode)
    {

        if(SOCP_NO_COMPRESS== ulDeflateState)
        {
           socp_crit("deflate:compress disable is already config!\n");
           return  BSP_OK;
        }

        else
        {
            bsp_socp_compress_disable(SOCP_CODER_DST_OM_IND);
            socp_crit("deflate:compress disable is config!\n");
            return  BSP_OK;
        }

    }
    else if (DEFLATE_IND_COMPRESS == eMode)
    {
        if(SOCP_COMPRESS == ulDeflateState)
        {
           socp_crit("deflate:compress enable is already config!\n");
           return  BSP_OK;

        }
        else
        {
           bsp_socp_compress_enable(SOCP_CODER_DST_OM_IND);
           socp_crit("deflate:compress enbale is config!\n");
           return  BSP_OK;
        }
    }

    else
    {
        socp_error("deflate invalid mode: %d!\n",eMode);
        return BSP_ERROR;
    }

}
/*****************************************************************************
* �� �� ��  : socp_ind_delay_init
*
* ��������  : ģ���ʼ������
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : ��ʼ���ɹ��ı�ʶ��
*****************************************************************************/
s32 bsp_socp_ind_delay_init(void)
{
    s32 ret;
    struct device_node* dev;

    dev = of_find_compatible_node(NULL,NULL,"hisilicon,socp_balong_app");
    if(NULL == dev)
    {
        socp_error("Socp dev find failed\n");
        return BSP_ERROR;
    }
    /*��ȡcmdline����*/
    (void)socp_get_cmdline_param();

    /*��ȡSOCPԤ���ڴ���Ϣ*/
    bsp_socp_get_mem_reserve_stru(&g_stSocpMemReserve);

    /*��ȡ���ڴ���������ת��*/
    (void)socp_logbuffer_mmap();
	ret = platform_driver_register(&socp_mem_driver); /*lint !e64*/
    if(ret)
    {
        socp_error("platform driver register failed!\n");
        return BSP_ERROR;
    }

    ret = bsp_socp_logbuffer_init(dev);
    if(ret)
    {
        socp_error("socp dst logbuffer init faield!\n");
        return ret;
    }

    /* init timeout */
    bsp_socp_timeout_init();

    socp_crit("socp_ind_delay init sucess!\n");

    return BSP_OK;
}


/*****************************************************************************
* �� �� ��  : bsp_deflate_get_log_ind_mode
*
* ��������  : �ϱ�ģʽ�ӿ�
*
* �������  : ģʽ����
*
* �������  : ��
*
* �� �� ֵ  : BSP_S32 BSP_OK:�ɹ� BSP_ERROR:ʧ��
*****************************************************************************/

s32  bsp_deflate_get_log_ind_mode(u32 *CfgIndMode)
{

    *CfgIndMode = g_stDeflateDstBufLogConfig.currentMode;

    return  BSP_OK;
}

/*****************************************************************************
* �� �� ��  : bsp_deflate_get_compress_mode
*
* ��������  : �ϱ�ģʽ�ӿ�
*
* �������  : ģʽ����
*
* �������  : ��
*
* �� �� ֵ  : BSP_S32 BSP_OK:�ɹ� BSP_ERROR:ʧ��
*****************************************************************************/

s32  bsp_deflate_get_compress_mode(u32 *CpsIndMode)
{
    *CpsIndMode = g_stDeflateDstBufLogConfig.cpsMode;

    return  BSP_OK;
}

/*****************************************************************************
* �� �� ��  : bsp_deflate_set_ind_mode
*
* ��������  : �ϱ�ģʽ�ӿ�
*
* �������  : ģʽ����
*
* �������  : ��
*
* �� �� ֵ  : BSP_S32 BSP_OK:�ɹ� BSP_ERROR:ʧ��
*****************************************************************************/
s32 bsp_deflate_set_ind_mode(SOCP_IND_MODE_ENUM eMode)
{
    u32 ret;
    switch (eMode)
    {
        case SOCP_IND_MODE_DIRECT:
        {

            if(SOCP_IND_MODE_DIRECT == g_stDeflateDstBufLogConfig.currentMode)
            {
                socp_crit("deflate direct mode is already config %d!\n",g_stDeflateDstBufLogConfig.currentMode);
                break;
            }
            (void)deflate_set_time(SOCP_IND_MODE_DIRECT);
            g_stDeflateDstBufLogConfig.ulCurTimeout = DEFLATE_TIMEOUT_INDIRECT;
            deflate_set_dst_threshold((bool)FALSE);
            deflate_set_cycle_mode( SOCP_IND_MODE_DIRECT);
            g_stDeflateDstBufLogConfig.currentMode = eMode;
            socp_crit("deflate direct mode config sucess %d!\n",g_stDeflateDstBufLogConfig.currentMode);

            break;
        }
        case SOCP_IND_MODE_DELAY:
        {

            if(SOCP_IND_MODE_DELAY == g_stDeflateDstBufLogConfig.currentMode)
            {
                socp_crit("deflate delay mode is already config %d!\n",g_stDeflateDstBufLogConfig.currentMode);
                break;
            }

            /* if logbuffer is not configed, can't enable delay mode*/
            if(g_stDeflateDstBufLogConfig.logOnFlag == DEFLATE_DST_CHAN_DELAY)
            {

                ret =deflate_set_time(SOCP_IND_MODE_DELAY);
                if(0!=ret)
                {
                    socp_error("deflate set time error !\n");
                    break;

                }
                g_stDeflateDstBufLogConfig.ulCurTimeout = DEFLATE_TIMEOUT_DEFLATY;;
                deflate_set_dst_threshold((bool)TRUE);
                deflate_set_cycle_mode( SOCP_IND_MODE_DELAY);
                g_stDeflateDstBufLogConfig.currentMode = eMode;
                socp_crit("deflate delay mode config sucess %d!\n",g_stDeflateDstBufLogConfig.currentMode);

            }
            else
            {
                socp_crit("deflate delay can't config:mem can't be setted!\n");
                return DEFLATE_ERR_MEM_NOT_ENOUGH;
            }
            break;
        }
        case SOCP_IND_MODE_CYCLE:
        {
            if(SOCP_IND_MODE_CYCLE == g_stDeflateDstBufLogConfig.currentMode)
            {
                socp_crit("deflate cycle mode is already config %d!\n",g_stDeflateDstBufLogConfig.currentMode);
                break;
            }

            /* if logbuffer is not configed, can't enable cycle mode*/
            if(((g_stEncDstBufLogConfig.logOnFlag == DEFLATE_DST_CHAN_DELAY)&&(BSP_TRUE == g_stSocpMemReserve.ulBufUsable))
                ||(g_stEncDstBufLogConfig.logOnFlag == SOCP_DST_CHAN_DTS))
            {

                deflate_set_cycle_mode( SOCP_IND_MODE_CYCLE);

                g_stDeflateDstBufLogConfig.currentMode = eMode;
                socp_crit("deflate cycle cycle mode config sucess %d!\n",g_stDeflateDstBufLogConfig.currentMode);
            }
            else
            {
                socp_crit("deflate:ind delay can't config:mem can't be setted!\n");
                return DEFLATE_ERR_MEM_NOT_ENOUGH;
            }
            break;
        }

        default:
        {
            socp_error("deflate set invalid mode: %d!\n",g_stDeflateDstBufLogConfig.currentMode);
            return  BSP_ERROR;
        }
    }
    return BSP_OK ;
 }

/*****************************************************************************
* �� �� ��  : bsp_socp_read_cur_mode
*
* ��������  : ѭ��ģʽ��ѯ�ӿ�
*
* �������  : ͨ��ID
*
* �������  : ��
*
* �� �� ֵ  : ��ʼ���ɹ��ı�ʶ��
*****************************************************************************/
u32 bsp_socp_read_cur_mode(u32 u32DestChanID)
{
    u32 u32modestate;
    u32 u32ChanID = SOCP_REAL_CHAN_ID(u32DestChanID);

    /*lint -save -e647*/
    u32modestate = SOCP_REG_GETBITS(SOCP_REG_ENCDEST_SBCFG(u32ChanID),1,1);
    /*lint -restore +e647*/
    return u32modestate;
}

/*****************************************************************************
* �� �� ��  : bsp_socp_logbuffer_cfgshow
*
* ��������  : ��ӡ��ʱд���������Ϣ
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : ��
*****************************************************************************/
void bsp_socp_logbuffer_cfgshow(void)
{

    socp_crit("socp_logbuffer_cfgshow: overTime           %d\n", g_stEncDstBufLogConfig.overTime);
    socp_crit("socp_logbuffer_cfgshow: Current Time Out   %d\n", g_stEncDstBufLogConfig.ulCurTimeout);
    socp_crit("socp_logbuffer_cfgshow: BufferSize         %d\n", g_stEncDstBufLogConfig.BufferSize);
    socp_crit("socp_logbuffer_cfgshow: logOnFlag          %d\n", g_stEncDstBufLogConfig.logOnFlag);
    socp_crit("socp_logbuffer_cfgshow: PhyBufferAddr      0x%lx\n", g_stEncDstBufLogConfig.ulPhyBufferAddr);
    socp_crit("socp_logbuffer_cfgshow: currentmode        %d\n", g_stEncDstBufLogConfig.currentMode);

    return;
}
EXPORT_SYMBOL(bsp_socp_logbuffer_cfgshow);

/*****************************************************************************
* �� �� ��  : bsp_deflate_logbuffer_cfgshow
*
* ��������  : ��ӡ��ʱд���������Ϣ
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : ��
*****************************************************************************/
void bsp_deflate_logbuffer_cfgshow(void)
{

    socp_crit("deflate_logbuffer_cfgshow: overTime           %d\n", g_stDeflateDstBufLogConfig.overTime);
    socp_crit("deflate_logbuffer_cfgshow: Current Time Out   %d\n", g_stDeflateDstBufLogConfig.ulCurTimeout);
    socp_crit("deflate_logbuffer_cfgshow: BufferSize         %d\n", g_stDeflateDstBufLogConfig.BufferSize);
    socp_crit("deflate_logbuffer_cfgshow: logOnFlag          %d\n", g_stDeflateDstBufLogConfig.logOnFlag);
    socp_crit("deflate_logbuffer_cfgshow: PhyBufferAddr      0x%lx\n", g_stDeflateDstBufLogConfig.ulPhyBufferAddr);
    socp_crit("deflate_logbuffer_cfgshow: currentmode        %d\n", g_stDeflateDstBufLogConfig.currentMode);

    return;
}
EXPORT_SYMBOL(bsp_deflate_logbuffer_cfgshow);

/*****************************************************************************
* �� �� ��  : socp_logbuffer_early_cfgshow
*
* ��������  : cmdline��ʽ�����ڴ���Ϣ
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : ��
*****************************************************************************/
void bsp_socp_logbuffer_early_cfgshow(void)
{

    socp_crit("socp_logbuffer_early_cfgshow: BufferSize         %d\n", g_stSocpEarlyCfg.ulBufferSize);
    socp_crit("socp_logbuffer_early_cfgshow: BufUsable          %d\n", g_stSocpEarlyCfg.ulBufUsable);
    socp_crit("socp_logbuffer_early_cfgshow: logCfg             %d\n", g_stSocpEarlyCfg.ulLogCfg);
    socp_crit("socp_logbuffer_early_cfgshow: PhyBufferAddr      0x%lx\n", g_stSocpEarlyCfg.ulPhyBufferAddr);
    socp_crit("socp_logbuffer_early_cfgshow: time               %d\n", g_stSocpEarlyCfg.ulTimeout);


    return;
}
EXPORT_SYMBOL(bsp_socp_logbuffer_early_cfgshow);

/*****************************************************************************
* �� �� ��  : socp_logbuffer_memreseve_cfgshow
*
* ��������  : kernelԤ���ڴ�������Ϣ
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : ��
*****************************************************************************/
void bsp_socp_logbuffer_memreserve_cfgshow(void)
{

    socp_crit("socp_logbuffer_memreserve_cfgshow: BufferSize         %d\n", g_stSocpMemReserve.ulBufferSize);
    socp_crit("socp_logbuffer_memreserve_cfgshow: BufUsable          %d\n", g_stSocpMemReserve.ulBufUsable);
    socp_crit("socp_logbuffer_memreserve_cfgshow: PhyBufferAddr      0x%lx\n", g_stSocpMemReserve.ulPhyBufferAddr);
    socp_crit("socp_logbuffer_memreserve_cfgshow: time               %d\n", g_stSocpMemReserve.ulTimeout);


    return;
}
EXPORT_SYMBOL(bsp_socp_logbuffer_memreserve_cfgshow);

EXPORT_SYMBOL(bsp_socp_get_log_cfg);






