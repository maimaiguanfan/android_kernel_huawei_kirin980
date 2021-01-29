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

#include <linux/clk.h>
#include <of.h>
#include <osl_thread.h>
#include "deflate.h"
#include "socp_ind_delay.h"

struct deflate_ctrl_info g_strDeflateCtrl;
struct deflate_debug_info g_strDeflateDebug;
struct deflate_abort_info g_strDeflateAbort;
u32 g_strDeflateEnableState=0;

extern DRV_DEFLATE_CFG_STRU  g_deflate_nv_ctrl;
#define DEFDST_TASK_PRO  81

/*****************************************************************************
* �� �� ��  : deflate_get_data_buffer
*
* ��������  : ��ȡ���л�����������
*
* �������  :  pRingBuffer       ����ѯ�Ļ���buffer
                    pRWBuffer         ����Ļ���buffer
*
* �������  : ��
*
* �� �� ֵ  :  ��
*****************************************************************************/
void deflate_get_data_buffer(DEFLATE_RING_BUF_STRU *pRingBuffer, DEFLATE_BUFFER_RW_STRU *pRWBuffer)
{
    if(pRingBuffer->u32Read <= pRingBuffer->u32Write)
    {
        /* дָ����ڶ�ָ�룬ֱ�Ӽ��� */
        pRWBuffer->pBuffer = (char *)(pRingBuffer->Start + (u32)pRingBuffer->u32Read);
        pRWBuffer->u32Size = (u32)(pRingBuffer->u32Write - pRingBuffer->u32Read);
        pRWBuffer->pRbBuffer = (char *)BSP_NULL;
        pRWBuffer->u32RbSize = 0;
    }
    else
    {
        /* ��ָ�����дָ�룬��Ҫ���ǻؾ� */
        pRWBuffer->pBuffer = (char *)(pRingBuffer->Start + (u32)pRingBuffer->u32Read);
        pRWBuffer->u32Size = ((u32)((u64)pRingBuffer->End - ((u64)pRingBuffer->Start + pRingBuffer->u32Read)) + 1);
        pRWBuffer->pRbBuffer = (char *)pRingBuffer->Start;
        pRWBuffer->u32RbSize = pRingBuffer->u32Write;
    }
}
/*****************************************************************************
* �� �� ��  : deflate_read_done
*
* ��������  : ���»������Ķ�ָ��
*
* �������  :  pRingBuffer       �����µĻ���buffer
                    u32Size          ���µ����ݳ���
*
* �������  : ��
*
* �� �� ֵ  :  ��
*****************************************************************************/
void deflate_read_done(DEFLATE_RING_BUF_STRU *pRingBuffer, u32 u32Size)
{
	pRingBuffer->u32Read += u32Size;
	if(pRingBuffer->u32Read > (u32)(pRingBuffer->End - pRingBuffer->Start))
	{
    	pRingBuffer->u32Read -= pRingBuffer->u32Length;
	}
}
/*****************************************************************************
* �� �� ��  : deflate_debug
*
* ��������  :��ȡ�Ĵ���
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : ��
*****************************************************************************/
void deflate_debug(void)
{
    /*lint -save -e732*/
    DEFLATE_REG_READ(SOCP_REG_DEFLATE_GLOBALCTRL,g_strDeflateAbort.u32DeflateGlobal);
    DEFLATE_REG_READ(SOCP_REG_DEFLATE_IBUFTIMEOUTCFG,g_strDeflateAbort.u32IbufTimeoutConfig);
    DEFLATE_REG_READ(SOCP_REG_DEFLATE_RAWINT,g_strDeflateAbort.u32RawInt);
    DEFLATE_REG_READ(SOCP_REG_DEFLATE_INT,g_strDeflateAbort.u32IntState);
    DEFLATE_REG_READ(SOCP_REG_DEFLATE_INTMASK,g_strDeflateAbort.u32IntMask);
    DEFLATE_REG_READ(SOCP_REG_DEFLATE_TFRTIMEOUTCFG,g_strDeflateAbort.u32ThfTimeoutConfig);
    DEFLATE_REG_READ(SOCP_REG_DEFLATE_STATE,g_strDeflateAbort.u32DeflateState);
    DEFLATE_REG_READ(SOCP_REG_DEFLATE_ABORTSTATERECORD,g_strDeflateAbort.u32AbortStateRecord);
    DEFLATE_REG_READ(SOCP_REG_DEFLATEDEBUG_CH,g_strDeflateAbort.u32DebugChan);
    DEFLATE_REG_READ(SOCP_REG_DEFLATEDEST_BUFREMAINTHCFG,g_strDeflateAbort.u32ObufThrh);
    DEFLATE_REG_READ(SOCP_REG_DEFLATEDEST_BUFRPTR,g_strDeflateAbort.u32ReadAddr);
    DEFLATE_REG_READ(SOCP_REG_DEFLATEDEST_BUFWPTR,g_strDeflateAbort.u32WriteAddr);
    DEFLATE_REG_READ(SOCP_REG_DEFLATEDEST_BUFADDR,g_strDeflateAbort.u32StartAddr);
    DEFLATE_REG_READ(SOCP_REG_DEFLATEDEST_BUFDEPTH,g_strDeflateAbort.u32BufSize);
    DEFLATE_REG_READ(SOCP_REG_DEFLATEDEST_BUFTHRH,g_strDeflateAbort.u32IntThrh);
    DEFLATE_REG_READ(SOCP_REG_DEFLATEDEST_BUFOVFTIMEOUT,g_strDeflateAbort.u32OvfTimeoutEn);
    DEFLATE_REG_READ(SOCP_REG_SOCP_MAX_PKG_BYTE_CFG,g_strDeflateAbort.u32PkgConfig);
    DEFLATE_REG_READ(SOCP_REG_DEFLATE_OBUF_DEBUG,g_strDeflateAbort.u32ObufDebug);
    /*lint -restore +e732*/
}
/*****************************************************************************
* �� �� ��   : deflate_set
*
* ��������  :deflate���ýӿڣ�������socpĿ�Ķ�ʱ�����
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : ���óɹ����ı�ʶ��
*****************************************************************************/
u32 deflate_set(u32 u32DestChanID, DEFLATE_CHAN_CONFIG_S *pDeflateAttr)
{
    unsigned long start;
    unsigned long end;
    u32 bufThreshold;
    u32 buflength;
    u32 u32ChanID;
    u32 u32Thrh;

    struct deflate_ctrl_info * pChan;
    /* �ж��Ƿ��Ѿ���ʼ�� */
    if (!g_strDeflateCtrl.initFlag)
    {
        socp_error("the module has not been initialized!\n");
        return DEFLATE_ERR_NOT_INIT;
    }
    g_strDeflateDebug.u32DeflateDstSetCnt++;
    if (NULL == pDeflateAttr)
    {
        socp_error("the parameter is NULL!\n");
        return DEFLATE_ERR_NULL;
    }

    /* �жϲ�����Ч�� */
    u32ChanID  = DEFLATE_REAL_CHAN_ID(u32DestChanID);
    if(u32ChanID > DEFLATE_MAX_ENCDST_CHN)
    {
        socp_error("the chan id(0x%x) is invalid!\n", u32ChanID);
        return DEFLATE_ERR_INVALID_PARA;
    }
    start = (uintptr_t)pDeflateAttr->sCoderSetDstBuf.pucOutputStart;
    end   = (uintptr_t)pDeflateAttr->sCoderSetDstBuf.pucOutputEnd;
    bufThreshold = pDeflateAttr->sCoderSetDstBuf.u32Threshold*1024;//socp�Ĵ�����kbyteΪ��λ,deflate��byteΪ��λ
    u32Thrh = 16*1024;//��ֵ���

    if ((0 == start)||(0 == end) || (0 == bufThreshold)||(0 == u32Thrh))
    {
        socp_error("the parameter is NULL!\n");
        return DEFLATE_ERR_NULL;
    }

    if (0 != (start%8))
    {
        socp_error("the parameter is not 8 bytes aligned!\n");
        return DEFLATE_ERR_NOT_8BYTESALIGN;
    }
    if (start >= end)
    {
        socp_error("the buff is invalid!\n");
        return DEFLATE_ERR_INVALID_PARA;
    }

    buflength =(u32) ((end - start) + 1);

    if (0 != (buflength%8))
    {
        socp_error("the parameter is not 8 bytes aligned!\n");
        return DEFLATE_ERR_NOT_8BYTESALIGN;
    }

    /* ����������������ٴ����� */
    pChan = &g_strDeflateCtrl;

    if (!pChan->u32SetStat)
    {
        /* д����ʼ��ַ��Ŀ��buffer��ʼ��ַ�Ĵ���*/
        DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_DST_BUFADDR_L, (u32)start);
    	DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_DST_BUFADDR_H, (u32)((u64)start>>32));
    	DEFLATE_REG_WRITE(SOCP_REG_DEFLATEDEST_BUFRPTR,0);
    	DEFLATE_REG_WRITE(SOCP_REG_DEFLATEDEST_BUFRPTR,0);

        DEFLATE_REG_WRITE(SOCP_REG_DEFLATEDEST_BUFDEPTH,buflength);
        DEFLATE_REG_WRITE(SOCP_REG_DEFLATEDEST_BUFREMAINTHCFG,u32Thrh);//��ֵ���
        DEFLATE_REG_WRITE(SOCP_REG_DEFLATEDEST_BUFTHRH,bufThreshold);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_GLOBALCTRL, 22, 4,u32ChanID);

        /* ��g_strDeflateStat�б������*/
        pChan->u32ChanID                   = u32DestChanID;
        pChan->u32Thrh                     = u32Thrh;
        pChan->sDeflateDstChan.Start       = start;
        pChan->sDeflateDstChan.End         = end;
        pChan->sDeflateDstChan.u32Write    = (u32)start;
        pChan->sDeflateDstChan.u32Read     = (u32)start;
        pChan->sDeflateDstChan.u32Length   = buflength;

        /* ������ͨ���Ѿ����� */
        pChan->u32SetStat = DEFLATE_CHN_SET;
   }
   else
   {
        socp_crit("Deflate set finished!\n");
   }
    return DEFLATE_OK;
}
/*****************************************************************************
* �� �� ��   : deflate_clear
*
* ��������  :deflate�������
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : �ɹ���ʶ��
*****************************************************************************/
u32 deflate_clear(u32 u32DestChanID)
{
    u32  u32ChanID;
    u32ChanID  = DEFLATE_REAL_CHAN_ID(u32DestChanID);
    if(u32ChanID > DEFLATE_MAX_ENCDST_CHN)
    {
        socp_error("the chan id is invalid!\n");
        return DEFLATE_ERR_INVALID_PARA;
    }
    /*deflate�жϱ�־��ʼ��*/
    g_strDeflateCtrl.u32IntDeflateDstOvf           =0;
    g_strDeflateCtrl.u32IntDeflateDstTfr           =0;
    g_strDeflateCtrl.u32IntDeflateDstThresholdOvf  =0;
    g_strDeflateCtrl.u32IntDeflateWorkAbort        =0;

    /*deflateĿ��ͨ�����Գ�ʼ��*/
    g_strDeflateCtrl.u32SetStat   =0;
    g_strDeflateCtrl.u32Thrh      =0;

    return DEFLATE_OK;
}
/*****************************************************************************
* �� �� ��   : deflate_enable
*
* ��������  :deflateʹ�ܽӿ�,��SOCP��ѹ��תѹ������
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : �ɹ���ʶ��
*****************************************************************************/
u32 deflate_enable(u32 u32DestChanID)
{
    u32  u32ChanID;

    u32ChanID  = DEFLATE_REAL_CHAN_ID(u32DestChanID);
    if(u32ChanID > DEFLATE_MAX_ENCDST_CHN)
    {
        socp_error("the chan id is invalid!\n");
        return DEFLATE_ERR_INVALID_PARA;
    }
    /* �ж��Ƿ��Ѿ���ʼ�� */
    if (!g_strDeflateCtrl.initFlag)
    {
        socp_error("the module has not been initialized!\n");
        return DEFLATE_ERR_NOT_INIT;
    }
    if(DEFLATE_CHN_SET == g_strDeflateCtrl.u32SetStat)
    {

        /* ���жϣ����ж� */
        bsp_deflate_data_send_manager(COMPRESS_ENABLE_STATE);
        /*�������жϣ�������ֵ�жϺ������ж�*/
        //DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_INTMASK,0x192);
        /*�������жϣ�������ֵ�жϺ���ֵ����ж�*/
        //DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_INTMASK,0x152);

        /*ʹ��deflate*/
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_GLOBALCTRL, 21, 1, 1);
    }
    else
    {
        socp_error("Deflate Enable failed!\n");
        return DEFLATE_ERR_SET_INVALID;
    }
    return DEFLATE_OK;
}
/*****************************************************************************
* �� �� ��   : deflate_unable
*
* ��������  :deflate��ʹ�ܽӿ�,��SOCPѹ��ת��ѹ����������
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : �ɹ�����ʶ��
*****************************************************************************/
u32 deflate_disable(u32 u32DestChanID)
{
    u32 DeflateIdleState;
    u32  u32ChanID;

    u32ChanID  = DEFLATE_REAL_CHAN_ID(u32DestChanID);
    if(u32ChanID > DEFLATE_MAX_ENCDST_CHN)
    {
        socp_error("the chan id is invalid!\n");
        return DEFLATE_ERR_INVALID_PARA;
    }
    /* �ж��Ƿ��Ѿ���ʼ�� */
    if (!g_strDeflateCtrl.initFlag)
    {
        socp_error("the module has not been initialized!\n");
        return DEFLATE_ERR_NOT_INIT;
    }

    if(DEFLATE_CHN_SET == g_strDeflateCtrl.u32SetStat)
    {
        /*����ѹ��*/
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_GLOBALCTRL, 20, 1, 1);


         /*lint -save -e732*/
          /*��deflateȫ��״̬�Ĵ�������ȡdeflate����״̬*/
        DEFLATE_REG_READ(SOCP_REG_DEFLATE_GLOBALCTRL,DeflateIdleState);
        while(!(DeflateIdleState & DEFLATE_WORK_STATE))
        {
           DEFLATE_REG_READ(SOCP_REG_DEFLATE_GLOBALCTRL,DeflateIdleState);
           msleep(1);
        }
        /*lint -save -e845*/
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_GLOBALCTRL, 21, 1, 0);
         /*lint -restore +e845*/

    	DEFLATE_REG_WRITE(SOCP_REG_DEFLATEDEST_BUFRPTR ,0);
    	DEFLATE_REG_WRITE(SOCP_REG_DEFLATEDEST_BUFWPTR, 0);

        /*��ѹ���ж�״̬�������ж�״̬*/
        bsp_deflate_data_send_manager(COMPRESS_DISABLE_STATE);
    }
    else
    {
        socp_error("Deflate Unable failed!\n") ;
        return DEFLATE_ERR_SET_INVALID;
    }
    return DEFLATE_OK;
 }
/*****************************************************************************
* �� �� ��   : deflate_register_read_cb
*
* ��������  : deflate ע����ص��ӿ�
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   :�ɹ�����ʶ��
*****************************************************************************/
 u32 deflate_register_read_cb(deflate_read_cb ReadCB)
 {
    /* �ж��Ƿ��Ѿ���ʼ�� */
    if (!g_strDeflateCtrl.initFlag)
    {
        socp_error("the module has not been initialized!\n");
        return DEFLATE_ERR_NOT_INIT;
    }

    /*deflate�Ƿ��Ѿ�����*/
    if(DEFLATE_CHN_SET == g_strDeflateCtrl.u32SetStat)
    {
        /* ���ö�Ӧͨ���Ļص�����*/
        g_strDeflateCtrl.read_cb = ReadCB;
        g_strDeflateDebug.u32DeflateRegReadCBCnt++;
    }
    else
    {
         socp_error("Deflate readCB  failed!\n");
         return DEFLATE_ERR_SET_INVALID;
    }
    return DEFLATE_OK;

}
/*****************************************************************************
* �� �� ��   : deflate_register_event_cb
*
* ��������  :deflate ע��event�ص��ӿ�
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : �ɹ�����ʶ��
*****************************************************************************/
 u32 deflate_register_event_cb(deflate_event_cb EventCB)
 {

    /* �ж��Ƿ��Ѿ���ʼ�� */
    if (!g_strDeflateCtrl.initFlag)
    {
        socp_error("the module has not been initialized!\n");
        return DEFLATE_ERR_NOT_INIT;
    }

   /*deflate�Ƿ��Ѿ�����*/
    if(DEFLATE_CHN_SET == g_strDeflateCtrl.u32SetStat)
    {
        /* ���ö�Ӧͨ���Ļص�����*/
        g_strDeflateCtrl.event_cb = EventCB;
        g_strDeflateDebug.u32DeflateRegEventCBCnt++;
    }
    else
    {
         socp_error("Deflate RegiseEventCB  failed!\n");
         return DEFLATE_ERR_SET_INVALID;
    }
    return DEFLATE_OK;

 }
/*****************************************************************************
* �� �� ��   : deflate_read_data_done
*
* ��������  : deflate read done�ӿ�
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : �ɹ�����ʶ��
*****************************************************************************/
u32 deflate_read_data_done(u32 ulDatalen)
{
 /*lint -save -e438*/
    DEFLATE_BUFFER_RW_STRU RwBuff;
    u32  uPAddr;
    u32  curmodestate;
    unsigned long lock_flag;

    /* �ж��Ƿ��Ѿ���ʼ�� */
    if (!g_strDeflateCtrl.initFlag)
    {
        socp_error("the module has not been initialized!\n");
        return DEFLATE_ERR_NOT_INIT;
    }
    curmodestate = DEFLATE_REG_GETBITS(SOCP_REG_DEFLATE_GLOBALCTRL,27,1);
    if(1==curmodestate)
    {
        socp_crit("deflate ind delay mode is cycle 0x%x!\n",curmodestate);
        return BSP_OK;
    }

    if(DEFLATE_CHN_SET == g_strDeflateCtrl.u32SetStat)
    {
       g_strDeflateDebug.u32DeflateReaddoneEtrCnt++;
       if (0 == ulDatalen)
       {
           g_strDeflateDebug.u32DeflateReaddoneZeroCnt++;
       }
       else
       {
            g_strDeflateDebug.u32DeflateReaddoneValidCnt++;
       }
       DEFLATE_REG_READ(SOCP_REG_DEFLATEDEST_BUFRPTR, uPAddr);
       g_strDeflateCtrl.sDeflateDstChan.u32Read = uPAddr;
       DEFLATE_REG_READ(SOCP_REG_DEFLATEDEST_BUFWPTR, uPAddr);
       /*lint -restore +e732*/
       g_strDeflateCtrl.sDeflateDstChan.u32Write= uPAddr;
       deflate_get_data_buffer(&g_strDeflateCtrl.sDeflateDstChan, &RwBuff);
       if(RwBuff.u32Size + RwBuff.u32RbSize < ulDatalen)
       {
            socp_error("RwBuff is not enough, ulDatalen=0x%x!\n", ulDatalen);

             /*lint -save -e550 -e845*/
            spin_lock_irqsave(&g_strDeflateCtrl.intSpinLock, lock_flag);
            DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT,  2, 1,1);
            DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK ,  2, 1,0);

            DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT,  6, 3,0x07);
            DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK ,  6, 1,0);
            spin_unlock_irqrestore(&g_strDeflateCtrl.intSpinLock, lock_flag);

            g_strDeflateDebug.u32DeflateReaddoneFailCnt++;
            return DEFLATE_ERR_INVALID_PARA;
       }
        /*���¶�ָ��*/
        deflate_read_done(&g_strDeflateCtrl.sDeflateDstChan,ulDatalen);
        /*д���ָ�뵽��ָ��Ĵ���*/
        uPAddr = g_strDeflateCtrl.sDeflateDstChan.u32Read;
        DEFLATE_REG_WRITE(SOCP_REG_DEFLATEDEST_BUFRPTR, uPAddr);

        spin_lock_irqsave(&g_strDeflateCtrl.intSpinLock, lock_flag);
        bsp_deflate_data_send_continue();
        spin_unlock_irqrestore(&g_strDeflateCtrl.intSpinLock, lock_flag);
         /*lint -restore +e550 +e845*/
        g_strDeflateDebug.u32DeflateReaddoneSucCnt++;
    }
    else
    {
        socp_error("Readdatadone failed!\n");
        return DEFLATE_ERR_SET_INVALID;
    }

     return DEFLATE_OK;
     /*lint -restore +e438*/
}
/*****************************************************************************
* �� �� ��   : deflate_get_read_buffer
*
* ��������  : ��ȡdeflate���ݿռ�
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  :�ɹ�����ʶ��
*****************************************************************************/
 u32 deflate_get_read_buffer(DEFLATE_BUFFER_RW_STRU *pBuff)
 {

    u32  uPAddr;
     /* �жϲ�����Ч�� */
    if (NULL == pBuff)
    {
        socp_error("the parameter is NULL!\n");
        return DEFLATE_ERR_NULL;
    }
    /* �ж��Ƿ��Ѿ���ʼ�� */
    if (!g_strDeflateCtrl.initFlag)
    {
        socp_error("the module has not been initialized!\n");
        return DEFLATE_ERR_NOT_INIT;
    }


    if(DEFLATE_CHN_SET == g_strDeflateCtrl.u32SetStat)
    {
        g_strDeflateDebug.u32DeflateGetReadBufEtrCnt++;
        /* ���ݶ�дָ���ȡbuffer */
        DEFLATE_REG_READ(SOCP_REG_DEFLATEDEST_BUFRPTR, uPAddr);
        g_strDeflateCtrl.sDeflateDstChan.u32Read = uPAddr;

        DEFLATE_REG_READ(SOCP_REG_DEFLATEDEST_BUFWPTR, uPAddr);
        g_strDeflateCtrl.sDeflateDstChan.u32Write= uPAddr;

        deflate_get_data_buffer(&g_strDeflateCtrl.sDeflateDstChan, pBuff);

        g_strDeflateDebug.u32DeflateGetReadBufSucCnt++;
    }
    else
    {
        socp_error("deflate set failed!\n");
        return DEFLATE_ERR_SET_INVALID;
    }

    return DEFLATE_OK;
 }


/*****************************************************************************
* �� �� ��   : deflate_int_handler
*
* ��������  :deflate�жϴ���ӿ�
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
u32 deflate_int_handler(void)
 {
    /*lint -save -e438*/
    u32 bHandle = false;
    u32  intState = 0;
    unsigned long lock_flag;
    u32 mask;
    if(DEFLATE_CHN_SET == g_strDeflateCtrl.u32SetStat)
    {
        DEFLATE_REG_READ(SOCP_REG_DEFLATE_INT, intState);
        /*��ֵ�����ж�*/
        if(intState&DEFLATE_TFR_MASK)
        {
             spin_lock_irqsave(&g_strDeflateCtrl.intSpinLock, lock_flag);
             DEFLATE_REG_READ(SOCP_REG_DEFLATE_INTMASK,mask);
             DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_INTMASK,intState|mask);
             DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_RAWINT,intState);
             g_strDeflateCtrl.u32IntDeflateDstTfr |=  intState;
             spin_unlock_irqrestore(&g_strDeflateCtrl.intSpinLock, lock_flag);
             bHandle = TRUE;
        }
        /*buffer��ֵ���*/
        else if(intState&DEFLATE_THROVF_MASK)
        {
             spin_lock_irqsave(&g_strDeflateCtrl.intSpinLock, lock_flag);
             DEFLATE_REG_READ(SOCP_REG_DEFLATE_INTMASK,mask);
             DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_INTMASK,intState|mask);
             DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_RAWINT,intState);
             g_strDeflateCtrl.u32IntDeflateDstThresholdOvf |=  intState;
             spin_unlock_irqrestore(&g_strDeflateCtrl.intSpinLock, lock_flag);
             bHandle = TRUE;
        }
        /*buffer�����ж�*/
        else if(intState&DEFLATE_OVF_MASK)
        {
             spin_lock_irqsave(&g_strDeflateCtrl.intSpinLock, lock_flag);
             DEFLATE_REG_READ(SOCP_REG_DEFLATE_INTMASK,mask);
             DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_INTMASK,intState|mask);
             DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_RAWINT,intState);
             g_strDeflateCtrl.u32IntDeflateDstOvf |=  intState;
             spin_unlock_irqrestore(&g_strDeflateCtrl.intSpinLock, lock_flag);
             bHandle = TRUE;
        }
        /*�쳣�ж�*/
        else if(intState&DEFLATE_WORK_ABORT_MASK)
        {    spin_lock_irqsave(&g_strDeflateCtrl.intSpinLock, lock_flag);
             DEFLATE_REG_READ(SOCP_REG_DEFLATE_INTMASK,mask);
             DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_INTMASK,intState|mask);
             DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_RAWINT,intState);
             g_strDeflateCtrl.u32IntDeflateWorkAbort |=  intState;
             spin_unlock_irqrestore(&g_strDeflateCtrl.intSpinLock, lock_flag);
             bHandle = TRUE;
        }
       /*ѭ��ģʽ�ж�*/
        else if(intState&DEFLATE_CYCLE_MASK)
        {
             spin_lock_irqsave(&g_strDeflateCtrl.intSpinLock, lock_flag);
             DEFLATE_REG_READ(SOCP_REG_DEFLATE_INTMASK,mask);
             DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_INTMASK,intState|mask);
             DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_RAWINT,intState);
             g_strDeflateCtrl.u32IntDeflateCycle |=  intState;
             spin_unlock_irqrestore(&g_strDeflateCtrl.intSpinLock, lock_flag);
             bHandle = TRUE;
        }
        /*����ģʽ�ж�*/
        else if(intState&DEFLATE_NOCYCLE_MASK)
        {
             spin_lock_irqsave(&g_strDeflateCtrl.intSpinLock, lock_flag);
             DEFLATE_REG_READ(SOCP_REG_DEFLATE_INTMASK,mask);
             DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_INTMASK,intState|mask);
             DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_RAWINT,intState);
             g_strDeflateCtrl.u32IntDeflateNoCycle |=  intState;
             spin_unlock_irqrestore(&g_strDeflateCtrl.intSpinLock, lock_flag);
             bHandle = TRUE;
        }
        else
        {
             bHandle = FALSE;
        }

   }
   if(bHandle)
   {
        osl_sem_up(&g_strDeflateCtrl.task_sem);
   }
   /*lint -restore +e438*/
   return DEFLATE_OK;
 }
/*****************************************************************************
* �� �� ��   : deflate_cycle
*
* ��������  : deflate ѭ��ģʽ����
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
void deflate_cycle(void)
{
    unsigned long lock_flag;
    /* ���ͨ���Ƿ�����*/
    if (DEFLATE_CHN_SET == g_strDeflateCtrl.u32SetStat)
    {


         /*���α���ж�*/
         spin_lock_irqsave(&g_strDeflateCtrl.intSpinLock, lock_flag);
         DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT,  2, 1,1);
         DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK ,  2, 1,1);
         DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT,  6, 3,0x07);
         DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK ,  6, 3,0x07);
          /*�������ж�*/
         DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT,   10, 1,1);
         DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK,  10, 1,0);
         spin_unlock_irqrestore(&g_strDeflateCtrl.intSpinLock, lock_flag);
         if (g_strDeflateCtrl.event_cb)
        {

            (void)g_strDeflateCtrl.event_cb(g_strDeflateCtrl.u32ChanID,DEFLATE_EVENT_CYCLE,0);

        }

    }
    return ;

}
void deflate_nocycle(void)
{
    unsigned long lock_flag;

    /* ���ͨ���Ƿ�����*/
    if (DEFLATE_CHN_SET == g_strDeflateCtrl.u32SetStat)
    {

         /*�򿪱���ж�*/
         spin_lock_irqsave(&g_strDeflateCtrl.intSpinLock, lock_flag);
         DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT,  2, 1,1);
         DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK ,  2, 1,0);
         DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT,  6, 3,0x07);
         DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK ,  6, 3,0);
          /*��ѭ��ģʽ�ж�*/
         DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT,  9, 1,1);
         DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK,  9, 1,0);
         spin_lock_irqsave(&g_strDeflateCtrl.intSpinLock, lock_flag);
         if (g_strDeflateCtrl.event_cb)
         {

            (void)g_strDeflateCtrl.event_cb(g_strDeflateCtrl.u32ChanID,DEFLATE_EVENT_NOCYCLE,0);

         }

    }
    return ;

}
void deflate_thresholdovf(void)
{
    /* ���ͨ���Ƿ�����*/
    if (DEFLATE_CHN_SET == g_strDeflateCtrl.u32SetStat)
    {
        if (g_strDeflateCtrl.event_cb)
        {
            g_strDeflateDebug.u32DeflateTskThresholdOvfCbOriCnt++;
            (void)g_strDeflateCtrl.event_cb(g_strDeflateCtrl.u32ChanID,DEFLATE_EVENT_THRESHOLD_OVERFLOW,0);
            g_strDeflateDebug.u32DeflateTskThresholdOvfCbCnt++;
         }
        if (g_strDeflateCtrl.read_cb)

        {
           g_strDeflateDebug.u32DeflateTskThresholdOvfCbOriCnt++;
           (void)g_strDeflateCtrl.read_cb(g_strDeflateCtrl.u32ChanID);
           g_strDeflateDebug.u32DeflateTskThresholdOvfCbCnt++;
         }
    }

    return ;
}
void deflate_ovf(void)
{
    /* ���ͨ���Ƿ�����*/
    if (DEFLATE_CHN_SET == g_strDeflateCtrl.u32SetStat)
    {
        if (g_strDeflateCtrl.event_cb)
        {
            g_strDeflateDebug.u32DeflateTskOvfCbOriCnt++;
            (void)g_strDeflateCtrl.event_cb(g_strDeflateCtrl.u32ChanID,DEFLATE_EVENT_OVERFLOW,0);
            g_strDeflateDebug.u32DeflateTskOvfCbCnt++;
        }
        if (g_strDeflateCtrl.read_cb)
        {

            g_strDeflateDebug.u32DeflateTskOvfCbOriCnt++;
            (void)g_strDeflateCtrl.read_cb(g_strDeflateCtrl.u32ChanID);
            g_strDeflateDebug.u32DeflateTskTrfCbCnt++;
        }
    }
    return ;
}
/*****************************************************************************
* �� �� ��   : deflate_task
*
* ��������  : deflate ������ӿ�
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
void deflate_task(void)
 {

    u32 IntTfrState = 0;
    u32 IntOvfState = 0;
    u32 IntThresholdOvfState = 0;
    u32 IntCycleState =0;
    u32 IntNoCycleState =0;
    u32 IntWorkAortState =0;
    unsigned long lock_flag;

    while(1)
    {
        osl_sem_down(&g_strDeflateCtrl.task_sem);
         /*lint -save -e550*/
        spin_lock_irqsave(&g_strDeflateCtrl.intSpinLock, lock_flag);

        IntTfrState = g_strDeflateCtrl.u32IntDeflateDstTfr;
        g_strDeflateCtrl.u32IntDeflateDstTfr = 0;

        IntThresholdOvfState = g_strDeflateCtrl.u32IntDeflateDstThresholdOvf;
        g_strDeflateCtrl.u32IntDeflateDstThresholdOvf = 0;

        IntOvfState = g_strDeflateCtrl.u32IntDeflateDstOvf;
        g_strDeflateCtrl.u32IntDeflateDstOvf = 0;


        IntCycleState=g_strDeflateCtrl.u32IntDeflateCycle;
        g_strDeflateCtrl.u32IntDeflateCycle =0;

        IntNoCycleState=g_strDeflateCtrl.u32IntDeflateNoCycle;
        g_strDeflateCtrl.u32IntDeflateNoCycle =0;

        IntWorkAortState = g_strDeflateCtrl.u32IntDeflateWorkAbort;
        g_strDeflateCtrl.u32IntDeflateWorkAbort = 0;

        spin_unlock_irqrestore(&g_strDeflateCtrl.intSpinLock, lock_flag);
       /*lint -restore +e550*/
        /* �������ж�*/
        if (IntTfrState)
        {

             /* ���ͨ���Ƿ�����*/
            if (DEFLATE_CHN_SET == g_strDeflateCtrl.u32SetStat)
            {
                if (g_strDeflateCtrl.read_cb)
                {
                    g_strDeflateDebug.u32DeflateTskTrfCbOriCnt++;
                    (void)g_strDeflateCtrl.read_cb(g_strDeflateCtrl.u32ChanID);
                    g_strDeflateDebug.u32DeflateTskTrfCbCnt++;
                }
            }
        }
        /* ����Ŀ�� buffer ��ֵ����ж�*/
        if (IntThresholdOvfState)
        {
            deflate_thresholdovf();

        }
        /*����buffer�����ж�*/
        if (IntOvfState)
        {
            deflate_ovf();

        }
       if(IntCycleState)
       {
            deflate_cycle();

       }
       if(IntNoCycleState)
       {
            deflate_nocycle();
       }
       /*�����쳣�ж�*/
       if (IntWorkAortState)
       {
           /* ���ͨ���Ƿ�����*/
           if (DEFLATE_CHN_SET == g_strDeflateCtrl.u32SetStat)
           {
                if (g_strDeflateCtrl.event_cb)
                {
                     g_strDeflateDebug.u32DeflateTskIntWorkAortCbOriCnt++;
                     (void)g_strDeflateCtrl.event_cb(g_strDeflateCtrl.u32ChanID, DEFLATE_EVENT_WORK_ABORT,0);
                     deflate_debug();
                     g_strDeflateDebug.u32DeflateTskIntWorkAortCbCnt++;
                 }

          }
      }
  }
    //  return DEFLATE_OK;
}


void deflate_set_dst_threshold(bool mode)
{
    u32 bufLength;
    u32 threshold;

    DEFLATE_REG_READ(SOCP_REG_DEFLATEDEST_BUFDEPTH,bufLength);
    if(mode == true)/*trueΪ��Ҫ����ʱ�ϱ��ĳ���*/
    {
        threshold = (bufLength >> 2)*3;
    }
    else
    {
        threshold = 0x1000;
    }

    DEFLATE_REG_WRITE(SOCP_REG_DEFLATEDEST_BUFTHRH,threshold);

    socp_crit(" set encdst threshold success! 0x%x\n", threshold);

    return;
}


/*****************************************************************************
* �� �� ��  : deflate_set_cycle_mode
*
* ��������  : SOCPѭ��ģʽ����
*
* �������  : ͨ���š�ģʽ
*
* �������  : ��
*
* �� �� ֵ  : ��
*****************************************************************************/

s32 deflate_set_cycle_mode(u32 cycle)
{
    u32 u32modestate;
    u32 waittime = 10000;
    unsigned long lock_flag;

    u32modestate =  DEFLATE_REG_GETBITS(SOCP_REG_DEFLATE_GLOBALCTRL,27,1);
    if ((0==cycle||1==cycle) && u32modestate)
    {
        u32 i;
        spin_lock_irqsave(&g_strDeflateCtrl.intSpinLock, lock_flag);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_GLOBALCTRL,26,1,0);
        spin_unlock_irqrestore(&g_strDeflateCtrl.intSpinLock, lock_flag);
        for(i=0; i<waittime; i++)
        {
            u32modestate =  DEFLATE_REG_GETBITS(SOCP_REG_DEFLATE_GLOBALCTRL,27,1);
            if(0 == u32modestate)
            {
                break;
            }
        }

        if(waittime == i)
        {
            socp_error("set encdst cycle off timeout!\n");
            return DEFLATE_ERR_GET_CYCLE;
        }
        bsp_deflate_data_send_manager(COMPRESS_ENABLE_STATE);
        return DEFLATE_OK;
    }
    else if((2==cycle) && (!u32modestate))
    {
        u32 i;

        bsp_deflate_data_send_manager(COMPRESS_DISABLE_STATE);
        spin_lock_irqsave(&g_strDeflateCtrl.intSpinLock, lock_flag);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_GLOBALCTRL,26,1,1);
        spin_unlock_irqrestore(&g_strDeflateCtrl.intSpinLock, lock_flag);
        for(i=0; i<waittime; i++)
        {
            u32modestate =  DEFLATE_REG_GETBITS(SOCP_REG_DEFLATE_GLOBALCTRL,27,1);
            if(1 == u32modestate)
            {
                break;
            }
        }

        if(waittime == i)
        {
            socp_error("set encdst cycle on timeout!\n");
            return DEFLATE_ERR_GET_CYCLE;
        }
    }

    return BSP_OK;


}
/*****************************************************************************
* �� �� ��  : deflate_set_time
*
* ��������  : DEFLATEʱ������
*
* �������  : ͨ���š�ģʽ
*
* �������  : ��
*
* �� �� ֵ  : ��
*****************************************************************************/

s32 deflate_set_time(u32 mode)
{

    if((SOCP_IND_MODE_DIRECT != mode)&&(SOCP_IND_MODE_DELAY != mode)
            &&(SOCP_IND_MODE_CYCLE != mode))
    {

        socp_error("mode error !\n");
        return DEFLATE_ERR_IND_MODE;

    }
    if(SOCP_IND_MODE_DIRECT == mode)
    {
        DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_TFRTIMEOUTCFG, DEFLATE_TIMEOUT_INDIRECT);
        return DEFLATE_OK;

    }
    else if(SOCP_IND_MODE_DELAY == mode)
    {
        DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_TFRTIMEOUTCFG, DEFLATE_TIMEOUT_DEFLATY);
        return DEFLATE_OK;

    }
    else
    {
        socp_crit("cycle mode need not set time !\n");
        return DEFLATE_OK;
    }


}
/*****************************************************************************
* �� �� ��  : bsp_deflate_data_send_manager
*
* ��������  : deflate����Ŀ�Ķ��ϱ�����
* �������  : EncDestChanID: ����Ŀ�Ķ�ͨ����
*             bEnable: �ж�ʹ��
*
* �������  : ��
*
* �� �� ֵ  : ��
*****************************************************************************/
void bsp_deflate_data_send_manager(u32 bEnable)
{
    unsigned long lock_flag;


    if((COMPRESS_DISABLE_STATE == bEnable) && (g_strDeflateEnableState == COMPRESS_ENABLE_STATE))
    {
        spin_lock_irqsave(&g_strDeflateCtrl.intSpinLock, lock_flag);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT,  2, 1,1);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK ,  2, 1,1);

        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT,  6, 3,0x07);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK ,  6, 3,0x07);

        g_strDeflateEnableState = COMPRESS_DISABLE_STATE;
        spin_unlock_irqrestore(&g_strDeflateCtrl.intSpinLock, lock_flag);
    }
    else if((COMPRESS_ENABLE_STATE == bEnable) && (g_strDeflateEnableState == COMPRESS_DISABLE_STATE))
    {
        spin_lock_irqsave(&g_strDeflateCtrl.intSpinLock, lock_flag);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT,  2, 1,1);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK ,  2, 1,0);

        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT,  6, 3,0x07);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK ,  6, 3,0);
        g_strDeflateEnableState = COMPRESS_ENABLE_STATE;

        spin_unlock_irqrestore(&g_strDeflateCtrl.intSpinLock, lock_flag);
    }
    else
    {
        socp_crit(" deflate has enabled.\n");
    }

}
/*****************************************************************************
* �� �� ��  : bsp_deflate_data_send_continue
*
* ��������  : deflateĿ�Ķ������ϱ�ʹ��


* ע    ��  : �ú�������ʱ����Ҫ�����߱�֤ͬ��

* �������  :        bEnable: �ж�ʹ��
*
* �������  : ��
*
* �� �� ֵ  : ��
*****************************************************************************/
void bsp_deflate_data_send_continue(void)
{
    if(COMPRESS_ENABLE_STATE == g_strDeflateEnableState)
    {
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT,  2, 1,1);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK ,  2, 1,0);

        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT,  6, 3,0x07);
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK ,  6, 3,0);
    }
    else
    {
        socp_crit("deflate g_SocpEnableState = %d\n", g_strDeflateEnableState);
    }
}

s32 deflate_init(void)
{
    s32 ret;
    struct clk  *cDeflate;
    struct device_node* dev ;
    struct socp_enc_dst_log_cfg *cDeflateConfig  ;
    socp_compress_ops_stru ops;
    unsigned long lock_flag;

    ret = bsp_nvm_read(NV_ID_DRV_DEFLATE, (u8 *)&g_deflate_nv_ctrl, sizeof(DRV_DEFLATE_CFG_STRU));
    if(ret)
    {
       g_deflate_nv_ctrl.deflate_enable =0;
       socp_error("deflate read nv fail!\n");
       return ret;
    }
    if(!g_deflate_nv_ctrl.deflate_enable)
    {
        socp_error("deflate_init:deflate is diabled!\n");
        return DEFLATE_OK;
    }

    if(BSP_TRUE == g_strDeflateCtrl.initFlag)
    {
        socp_error("deflate init already!\n");
        return DEFLATE_OK;
    }
    spin_lock_init(&g_strDeflateCtrl.intSpinLock);
    osl_sem_init(0, &g_strDeflateCtrl.task_sem);

    /*����dts,��ȡdeflate����ַ����ӳ��*/
    dev = of_find_compatible_node(NULL,NULL,"hisilicon,deflate_balong_app");
    if(NULL == dev)
    {
        socp_error("deflate dev find failed!\n");
        return DEFLATE_ERROR;
    }
    g_strDeflateCtrl.baseAddr = (void*)of_iomap(dev,0);
    if(NULL == g_strDeflateCtrl.baseAddr)
    {
        socp_error("deflate base addr is error!\n");
        return DEFLATE_ERROR;
    }
    /*deflate�жϱ�־��ʼ��*/
    g_strDeflateCtrl.u32IntDeflateDstOvf    =0;
    g_strDeflateCtrl.u32IntDeflateDstTfr    =0;
    g_strDeflateCtrl.u32IntDeflateDstThresholdOvf  =0;
    g_strDeflateCtrl.u32IntDeflateWorkAbort  =0 ;

    /*deflateĿ��ͨ�����Գ�ʼ��*/
    g_strDeflateCtrl.u32SetStat   =0;
    g_strDeflateCtrl.u32Thrh      =0;

    /* ��defalteʱ�� */
	cDeflate = clk_get(NULL, "clk_socp_deflat");
    if(IS_ERR(cDeflate))
    {
        socp_error("deflate prepare clk fail!\n");
        return DEFLATE_ERROR;
    }
    clk_prepare(cDeflate);
	if(BSP_OK !=clk_enable(cDeflate))
	{
		socp_error("deflate clk enable failed!\n");
		return DEFLATE_ERROR;
	}
    /*����deflate���� */
    ret = osl_task_init("deflateProc",DEFDST_TASK_PRO, 4096, (OSL_TASK_FUNC) deflate_task,NULL, &g_strDeflateCtrl.taskid);
    if (BSP_OK != ret)
    {
        socp_error("create task failed!\n");
        return DEFLATE_ERROR;
    }
    /*deflate��λ*/
    DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_GLOBALCTRL, 1, 1, 1);

    /*���������ʱ�ϱ�����Ҫ���ó�ʱ�Ĵ���*/
    cDeflateConfig=bsp_socp_get_log_cfg();
    DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_TFRTIMEOUTCFG, DEFLATE_TIMEOUT_INDIRECT);

     /*lint -save -e845*/
    /*����buffer���������*/
    DEFLATE_REG_SETBITS(SOCP_REG_DEFLATEDEST_BUFOVFTIMEOUT, 31, 1, 0);
    /*ʹ��255��ѹ������*/
    DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_GLOBALCTRL, 10, 8, 0x5);
    DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_GLOBALCTRL, 18, 1, 0);
    /*bypass ģʽ*/
    DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_GLOBALCTRL,  0, 1, 0);
    /*ibuf��ʱ����*/
    DEFLATE_REG_WRITE(SOCP_REG_DEFLATE_IBUFTIMEOUTCFG,0xFFFFFFFF);
    /* ���жϣ������ж� */
    spin_lock_irqsave(&g_strDeflateCtrl.intSpinLock, lock_flag);
    DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT, 1, 2, 0x03);
    DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK, 1, 2,0x03 );
    DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT, 4, 1, 1);
    DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK, 4, 1, 1);
    DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_RAWINT, 6, 3, 0x07);
    DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_INTMASK, 6, 3, 0x07);
    spin_unlock_irqrestore(&g_strDeflateCtrl.intSpinLock, lock_flag);
    ops.isr                 = deflate_int_handler;
    ops.register_Eventcb    = deflate_register_event_cb;
    ops.register_Readcb     = deflate_register_read_cb;
    ops.enable              = deflate_enable;
    ops.disable             = deflate_disable;
    ops.set                 = deflate_set;
    ops.getbuffer           = deflate_get_read_buffer;
    ops.readdone            = deflate_read_data_done;
    ops.clear               = deflate_clear;
    bsp_socp_register_compress(&ops);

    /* ���ó�ʼ��״̬ */
    g_strDeflateCtrl.initFlag = BSP_TRUE;
    socp_crit("[init]Deflate init succeed\n");
    return DEFLATE_OK;
}


s32 deflate_stop(u32 u32DstChanID)
{
    u32 u32RealChanID;
    u32 u32ChanType;

    /* �ж��Ƿ��Ѿ���ʼ�� */
    if (!g_strDeflateCtrl.initFlag)
    {
        socp_error("the module has not been initialized!\n");
        return DEFLATE_ERR_NOT_INIT;
    }

    /* �ж�ͨ��ID�Ƿ���Ч */
    u32RealChanID = DEFLATE_REAL_CHAN_ID(u32DstChanID);
    u32ChanType = DEFLATE_REAL_CHAN_TYPE(u32DstChanID);

    /* ����ͨ�� */
    if (DEFLATE_CODER_DEST_CHAN  == u32ChanType)
    {
        if (u32RealChanID < DEFLATE_MAX_ENCDST_CHN )
        {

            if (DEFLATE_CHN_SET!= g_strDeflateCtrl.u32SetStat)
            {
                socp_error("encoder src not allocated!\n" );
                return DEFLATE_ERR_INVALID_CHAN;
            }
        }
        else
        {
            socp_error("enc dst 0x%x is valid!\n", u32DstChanID);
            return DEFLATE_ERROR ;
        }

        /*ֹͣdeflate*/
        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_GLOBALCTRL, 21, 1, 0);
      /*lint -restore +e845*/

        /* ��λͨ�� */

        DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_GLOBALCTRL,1, 1, 1);
    }
    return DEFLATE_OK;
}
/*****************************************************************************
* �� �� ��   : deflate_help
*
* ��������  :deflate ��ӡ��Ϣ
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
 void deflate_help(void)
 {
   socp_crit("deflate_show_debug_gbl: �鿴ȫ��ͳ����Ϣ:ͨ�����롢���ú��ж��ܼ���\n");
   socp_crit("deflate_show_dest_chan_cur: �鿴ѹ��Ŀ��ͨ����Ϣ\n");
 }
void deflate_show_debug_gbl(void)
{
    socp_crit("\r DEFLATE ȫ��״̬ά����Ϣ:\n");
    socp_crit("\r deflate����ַ:                                            : 0x%pK\n",
           g_strDeflateCtrl.baseAddr);
    socp_crit("\r deflate���ñ���Ŀ��ͨ���Ĵ���                             : 0x%x\n",
           (s32)g_strDeflateDebug.u32DeflateDstSetCnt);
    socp_crit("\r deflate���ñ���Ŀ��ͨ���ɹ��Ĵ���                         : 0x%x\n",
           (s32)g_strDeflateDebug.u32DeflateDstSetSucCnt);
    socp_crit("\r deflateע��deflateĿ��ͨ�������ݻص���������              : 0x%x\n",
           (s32)g_strDeflateDebug.u32DeflateRegReadCBCnt);
    socp_crit("\r deflateע��deflateĿ��ͨ���쳣�¼��ص���������            : 0x%x\n",
           (s32)g_strDeflateDebug.u32DeflateRegEventCBCnt);
    socp_crit("\r deflate���Ի�ȡdeflateĿ��buffer����                      : 0x%x\n",
           (s32)g_strDeflateDebug.u32DeflateGetReadBufEtrCnt);
    socp_crit("\r deflate��ȡdeflateĿ��buffer�ɹ�����                      : 0x%x\n",
           (s32)g_strDeflateDebug.u32DeflateGetReadBufSucCnt);
    socp_crit("\r deflate���Զ�ȡdeflateĿ�����ݴ���                        : 0x%x\n",
           (s32)g_strDeflateDebug.u32DeflateReaddoneEtrCnt);
    socp_crit("\r deflate���Զ�ȡdeflateĿ�����ݳ��ȵ���0����               : 0x%x\n",
           (s32)g_strDeflateDebug.u32DeflateReaddoneZeroCnt);
    socp_crit("\r deflate��ȡdeflateĿ�����ݳ��Ȳ�����0����                 : 0x%x\n",
           (s32)g_strDeflateDebug.u32DeflateReaddoneValidCnt);
    socp_crit("\r deflate��ȡdeflateĿ������ʧ�ܵĴ���                      : 0x%x\n",
           (s32)g_strDeflateDebug.u32DeflateReaddoneFailCnt);
    socp_crit("\r deflate��ȡdeflateĿ�����ݳɹ��Ĵ���                      : 0x%x\n",
           (s32)g_strDeflateDebug.u32DeflateReaddoneSucCnt);
    socp_crit("\r deflate�������ж�����Ĵ���                             : 0x%x\n",
           (s32)g_strDeflateDebug.u32DeflateTskTrfCbOriCnt);
    socp_crit("\r deflate�����괫���ж�����Ĵ���                           : 0x%x\n",
           (s32)g_strDeflateDebug.u32DeflateTskTrfCbCnt);
    socp_crit("\r deflate���������жϵĴ���                                 : 0x%x\n",
           (s32)g_strDeflateDebug.u32DeflateTskOvfCbOriCnt);
    socp_crit("\r deflate�����������жϵĴ���*                              : 0x%x\n",
           (s32)g_strDeflateDebug.u32DeflateTskOvfCbCnt);
    socp_crit("\r deflate������ֵ����Ĵ���                                 : 0x%x\n",
           (s32)g_strDeflateDebug.u32DeflateTskThresholdOvfCbOriCnt);
    socp_crit("\r deflate��������ֵ����Ĵ���                               : 0x%x\n",
           (s32)g_strDeflateDebug.u32DeflateTskThresholdOvfCbCnt);
    socp_crit("\r deflate�����쳣�Ĵ���                                     : 0x%x\n",
           (s32)g_strDeflateDebug.u32DeflateTskIntWorkAortCbOriCnt);
    socp_crit("\r deflate�������쳣�Ĵ���                                   : 0x%x\n",
           (s32)g_strDeflateDebug.u32DeflateTskIntWorkAortCbCnt);
}
u32 deflate_show_dest_chan_cur(void)
{

    socp_crit("��ʼ��״̬:0x%x\n",g_strDeflateCtrl.initFlag);
    socp_crit("ͨ��ID:0x%x\n",g_strDeflateCtrl.u32ChanID);
    socp_crit(" ͨ������״̬:%d\n",g_strDeflateCtrl.u32SetStat);
    socp_crit("ͨ��buffer ��ʼ��ַ:0x%pK\n",(char *)g_strDeflateCtrl.sDeflateDstChan.Start);
    socp_crit("ͨ��buffer ������ַ:0x%pK\n",(char *)g_strDeflateCtrl.sDeflateDstChan.End);
    socp_crit("ͨ��buffer ��ָ��:0x%x\n",g_strDeflateCtrl.sDeflateDstChan.u32Read);
    socp_crit("ͨ��buffer дָ��:0x%x\n",g_strDeflateCtrl.sDeflateDstChan.u32Write);
    socp_crit("ͨ��buffer ����:0x%x\n",g_strDeflateCtrl.sDeflateDstChan.u32Length);
    return BSP_OK;
}

static s32 deflate_suspend(struct device *dev)
{
    u32 DeflateIdleState;

    /*����ѹ��*/
    DEFLATE_REG_SETBITS(SOCP_REG_DEFLATE_GLOBALCTRL, 20, 1, 1);

      /*��deflateȫ��״̬�Ĵ�������ȡdeflate����״̬*/
    DEFLATE_REG_READ(SOCP_REG_DEFLATE_GLOBALCTRL,DeflateIdleState);
    while(!(DeflateIdleState & DEFLATE_WORK_STATE))
    {

       DEFLATE_REG_READ(SOCP_REG_DEFLATE_GLOBALCTRL,DeflateIdleState);
    }

    socp_crit("open signle packet compress!\n");
    return 0;
}
static s32 deflate_resume(struct device *dev)
{
    return 0;
}
static const struct dev_pm_ops deflate_pm_ops ={
    .suspend = deflate_suspend,
    .resume  = deflate_resume,
};

#define DEFLATE_PM_OPS (&deflate_pm_ops)

static struct platform_driver modem_deflate_drv = {
    .driver     = {
        .name     = "modem_deflate",
        .owner    = (struct module *)(unsigned long)THIS_MODULE,
        .pm       = DEFLATE_PM_OPS,
    },
};

static struct platform_device modem_deflate_device = {
    .name = "modem_deflate",
    .id = 0,
    .dev = {
    .init_name = "modem_deflate",
    },
};

int deflate_dev_init(void)
{
    u32 ret;
    if(!g_deflate_nv_ctrl.deflate_enable)
    {
        socp_error("deflate is diabled!\n");
        return DEFLATE_OK;
    }
    ret = (u32)platform_device_register(&modem_deflate_device);
    if(ret)
    {
        socp_crit("modem_deflate_device fail !\n");
        return -1;
    }

    ret = (u32)platform_driver_register(&modem_deflate_drv);
    if(ret)
    {
        socp_crit("modem_deflate_drv fail !\n");
        platform_device_unregister(&modem_deflate_device);
        return -1;
    }
    return 0;
}


