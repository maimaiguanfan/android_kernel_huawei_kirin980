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


#include <mdrv.h>
#include <linux/wakelock.h>
#include "msp_errno.h"
#include "diag_common.h"
#include "diag_debug.h"
#include "msp_service.h"
#include "soc_socp_adapter.h"
#include "diag_acore_common.h"
#include "msp_diag_comm.h"
#include "diag_service.h"


#define    THIS_FILE_ID        MSP_FILE_ID_DIAG_SERVICE_C


DIAG_DFR_INFO_STRU g_stDFRcnf;
VOS_SPINLOCK                    g_stSrvIndSrcBuffSpinLock;
VOS_SPINLOCK                    g_stSrvCnfSrcBuffSpinLock;


typedef struct
{
    VOS_SEM                 ListSem;
    LIST_S                  ListHeader;
} DIAG_SRVC_MAIN_STRU;


#define     DIAG_PKT_TIMEOUT_LEN            (32768*3)     /* 3s */

typedef struct
{
    DIAG_SERVICE_HEAD_STRU  stHead;
    VOS_UINT32              ulSlice;        /* �����ڵ��ʱ��������ڼ�ⳬʱ */
    VOS_UINT32              ulFrameDataLen;
    VOS_UINT32              ulFrameOffset;
    LIST_S                  FrameList;
    DIAG_FRAME_INFO_STRU    *pFrame;
} DIAG_SRVC_FRAME_NODE_STRU;

DIAG_SERVICE_FUNC g_fnService = VOS_NULL_PTR;
DIAG_DFR_INFO_STRU g_stDFRreq;

DIAG_SRVC_MAIN_STRU  g_stDiagSrvc;

DIAG_SRV_CTRL g_SrvCtrl = {
    SOCP_CODER_SRC_PS_IND,
    SOCP_CODER_SRC_CNF,
};

VOS_VOID diag_SvcFillHeader(DIAG_SRV_HEADER_STRU *pstSrvHeader)
{
    VOS_UINT8 auctime[8];
    VOS_UINT32 ulTimeStampLen = sizeof(pstSrvHeader->frame_header.stService.aucTimeStamp);

    /* Ĭ�ϲ��ְ������ϱ� */
    pstSrvHeader->socp_header.ulHisiMagic = DIAG_SRV_HISI_HEADER_MAGIC;
    pstSrvHeader->socp_header.ulDataLen = 0;

    pstSrvHeader->frame_header.stService.sid8b = MSP_SID_DIAG_SERVICE;
    pstSrvHeader->frame_header.stService.mdmid3b = 0;
    pstSrvHeader->frame_header.stService.rsv1b = 0;
    pstSrvHeader->frame_header.stService.ssid4b = DIAG_SSID_CPU;
    pstSrvHeader->frame_header.stService.sessionid8b = MSP_SERVICE_SESSION_ID;
    /*Ĭ��Ϊ�����ϱ�*/
    pstSrvHeader->frame_header.stService.mt2b = DIAG_MT_IND;
    pstSrvHeader->frame_header.stService.index4b = 0;
    pstSrvHeader->frame_header.stService.eof1b = 0;
    pstSrvHeader->frame_header.stService.ff1b = 0;
    pstSrvHeader->frame_header.stService.ulMsgTransId = 0;

    (VOS_VOID)mdrv_timer_get_accuracy_timestamp((VOS_UINT32*)&(auctime[4]), (VOS_UINT32*)&(auctime[0]));
    VOS_MemCpy_s(pstSrvHeader->frame_header.stService.aucTimeStamp, sizeof(pstSrvHeader->frame_header.stService.aucTimeStamp), \
                auctime, ulTimeStampLen);
}

VOS_VOID diag_PktTimeoutClear(VOS_VOID)
{
    LIST_S* me = NULL;
    DIAG_SRVC_FRAME_NODE_STRU *pTempNode = VOS_NULL_PTR;
    VOS_UINT32 ulCurSlice = mdrv_timer_get_normal_timestamp();
    VOS_UINT32 del_flag = 0;

    (VOS_VOID)VOS_SmP(g_stDiagSrvc.ListSem,0);

    blist_for_each(me, &g_stDiagSrvc.ListHeader)
    {
        if((pTempNode != VOS_NULL_PTR)&&(del_flag))
        {
            VOS_MemFree(MSP_PID_DIAG_APP_AGENT, pTempNode);
            del_flag = 0;
        }
        pTempNode = blist_entry(me, DIAG_SRVC_FRAME_NODE_STRU, FrameList);

        if((ulCurSlice > pTempNode->ulSlice) && ((ulCurSlice - pTempNode->ulSlice) > DIAG_PKT_TIMEOUT_LEN))
        {
            /*ɾ���ڵ�*/
            if((VOS_NULL_PTR != pTempNode->FrameList.next) && (VOS_NULL_PTR != pTempNode->FrameList.prev))
            {
                blist_del(&pTempNode->FrameList);
            }

            printk(KERN_ERR"[debug] diag_PktTimeoutClear delete node.\n");

            if(VOS_NULL_PTR != pTempNode->pFrame)
            {
                VOS_MemFree(MSP_PID_DIAG_APP_AGENT, pTempNode->pFrame);
            }

            del_flag = 1;
        }
    }
    if((pTempNode != VOS_NULL_PTR)&&(del_flag))
    {
            VOS_MemFree(MSP_PID_DIAG_APP_AGENT, pTempNode);
        }

    (VOS_VOID)VOS_SmV(g_stDiagSrvc.ListSem);
}


/*****************************************************************************
 Function Name   : diag_SrvcCreatePkt
 Description     : �յ���һ���ְ�ʱ��������ͽڵ�

 History         :
    1.c64416         2015-03-18  Draft Enact

*****************************************************************************/
/*lint -save -e429*/
VOS_VOID diag_SrvcCreatePkt(DIAG_FRAME_INFO_STRU *pFrame)
{
    LIST_S* me = NULL;
    DIAG_SRVC_FRAME_NODE_STRU *pTempNode;

    /*��Ϣ���Ȳ��ܴ�����󳤶�*/
    if(pFrame->ulMsgLen + sizeof(DIAG_FRAME_INFO_STRU) > DIAG_FRAME_SUM_LEN)
    {
        diag_error("MsgLen(0x%x) too large\n", pFrame->ulMsgLen);
        return;
    }

    /* ����������Ѿ�����ͬtransid�Ľڵ���ֱ���˳� */
    (VOS_VOID)VOS_SmP(g_stDiagSrvc.ListSem, 0);

    blist_for_each(me, &g_stDiagSrvc.ListHeader)
    {
        pTempNode = blist_entry(me, DIAG_SRVC_FRAME_NODE_STRU, FrameList);

        pTempNode->stHead.index4b = pFrame->stService.index4b;
        pTempNode->stHead.eof1b   = pFrame->stService.eof1b;
        pTempNode->stHead.ff1b    = pFrame->stService.ff1b;
        if(VOS_MemCmp(&pTempNode->stHead, &pFrame->stService, sizeof(DIAG_SERVICE_HEAD_STRU)))
        {
            (VOS_VOID)VOS_SmV(g_stDiagSrvc.ListSem);

            diag_error("there is a uniform packet in list\n");
            return ;
        }
    }
    (VOS_VOID)VOS_SmV(g_stDiagSrvc.ListSem);

    /* �����ڵ㣬�����ڴ� */
    pTempNode = (DIAG_SRVC_FRAME_NODE_STRU *)VOS_MemAlloc(MSP_PID_DIAG_APP_AGENT, DYNAMIC_MEM_PT,
                                                            sizeof(DIAG_SRVC_FRAME_NODE_STRU));
    if(VOS_NULL_PTR == pTempNode)
    {
        diag_error("malloc pTempNode fail\n");
        return ;
    }

    pTempNode->ulFrameDataLen = pFrame->ulMsgLen + sizeof(DIAG_FRAME_INFO_STRU);
    pTempNode->ulFrameOffset = 0;
    pTempNode->pFrame = (DIAG_FRAME_INFO_STRU*)VOS_MemAlloc(MSP_PID_DIAG_APP_AGENT, DYNAMIC_MEM_PT,
                                                            pTempNode->ulFrameDataLen);
    if(VOS_NULL_PTR == pTempNode->pFrame)
    {
        diag_error("malloc pFrame fail\n");
        VOS_MemFree(MSP_PID_DIAG_APP_AGENT, pTempNode);
        return ;
    }

    (VOS_VOID)VOS_SmP(g_stDiagSrvc.ListSem,0);

    VOS_MemCpy_s(&pTempNode->stHead, (VOS_UINT32)sizeof(pTempNode->stHead), &pFrame->stService, sizeof(pFrame->stService));

    blist_add_tail(&pTempNode->FrameList, &g_stDiagSrvc.ListHeader);

    pTempNode->ulSlice = mdrv_timer_get_normal_timestamp();

    (VOS_VOID)VOS_SmV(g_stDiagSrvc.ListSem);

    return ;

}
/*lint -restore*/

/*****************************************************************************
 Function Name   : diag_SrvcSavePkt
 Description     : �յ��ְ�ʱ�ѷְ����ݿ�����������

 History         :
    1.c64416         2015-03-18  Draft Enact

*****************************************************************************/
DIAG_FRAME_INFO_STRU * diag_SrvcSavePkt(DIAG_FRAME_INFO_STRU *pFrame, VOS_UINT32 ulDataLen)
{
    LIST_S* me = NULL;
    DIAG_SRVC_FRAME_NODE_STRU *pTempNode;
    VOS_UINT32 ulLen = 0;
    VOS_UINT32 uloffset = 0;
    VOS_UINT32 ulLocalLen = 0;

    (VOS_VOID)VOS_SmP(g_stDiagSrvc.ListSem,0);

    blist_for_each(me, &g_stDiagSrvc.ListHeader)
    {
        pTempNode = blist_entry(me, DIAG_SRVC_FRAME_NODE_STRU, FrameList);

        if(VOS_NULL_PTR == pTempNode)
        {
            /* coverity[dead_error_begin] */
            (VOS_VOID)VOS_SmV(g_stDiagSrvc.ListSem);
            return VOS_NULL_PTR;
        }

        /* �˴�ע��stService��4G ��5G������ */
        pTempNode->stHead.index4b = pFrame->stService.index4b;
        pTempNode->stHead.eof1b   = pFrame->stService.eof1b;
        pTempNode->stHead.ff1b    = pFrame->stService.ff1b;
        if(0 == VOS_MemCmp(&pTempNode->stHead, &pFrame->stService, sizeof(DIAG_SERVICE_HEAD_STRU)))
        {
            if(0 == pFrame->stService.index4b)  /* ��0֡ */
            {
                /* ��0֡��Ҫ����header, cmdid, meglen and data */
                (VOS_VOID)VOS_MemCpy_s(pTempNode->pFrame, pTempNode->ulFrameDataLen, pFrame, ulDataLen);
                pTempNode->ulFrameOffset = ulDataLen;
                diag_SaveDFR(&g_stDFRreq, (VOS_UINT8*)pFrame, ulDataLen);
            }
            else if(pFrame->stService.eof1b)  /* ���1֡ */
            {
                /* �����һ֡�⣬�Ѵ洢�����ݳ��� */
                ulLen = pTempNode->ulFrameOffset - sizeof(DIAG_FRAME_INFO_STRU);
                ulLocalLen = ulDataLen - sizeof(DIAG_SERVICE_HEAD_STRU);

                if(     (VOS_NULL_PTR == pTempNode->pFrame)
                    ||  (pTempNode->ulFrameOffset + ulLocalLen > DIAG_FRAME_SUM_LEN)
                    ||  (pTempNode->pFrame->ulMsgLen != ulLen + ulLocalLen))
                {
                    diag_error("DataLen error, ulLen:0x%x ulLocalLen:0x%x\n", ulLen, ulLocalLen);
                    (VOS_VOID)VOS_SmV(g_stDiagSrvc.ListSem);
                    return VOS_NULL_PTR;
                }

                /* δ��������ݳ��� */
                ulLen = pTempNode->pFrame->ulMsgLen - ulLen;

                /* ��ǰ��������ƫ�� */
                uloffset = pTempNode->ulFrameOffset;

                /* ���һֻ֡��Ҫ����ʣ��data */
                (VOS_VOID)VOS_MemCpy_s( ((VOS_UINT8*)pTempNode->pFrame) + uloffset, pTempNode->ulFrameDataLen - pTempNode->ulFrameOffset,
                            ((VOS_UINT8*)pFrame) + sizeof(DIAG_SERVICE_HEAD_STRU),  ulLen);
                pTempNode->ulFrameOffset += ulLen;
                diag_SaveDFR(&g_stDFRreq, (VOS_UINT8*)pFrame, ulDataLen);
            }
            else
            {
                /* ��ǰ��������ƫ�� */
                uloffset = pTempNode->ulFrameOffset;
                ulLocalLen = ulDataLen - sizeof(DIAG_SERVICE_HEAD_STRU);

                if(     (VOS_NULL_PTR == pTempNode->pFrame)
                    ||  (uloffset + ulLocalLen > DIAG_FRAME_SUM_LEN)
                    ||  (pTempNode->pFrame->ulMsgLen < (uloffset - sizeof(DIAG_FRAME_INFO_STRU) + ulLocalLen)))
                {
                    diag_error("MsgLen error, uloffset:0x%x ulLocallen:0x%x\n", uloffset, ulLocalLen);
                    (VOS_VOID)VOS_SmV(g_stDiagSrvc.ListSem);
                    return VOS_NULL_PTR;
                }

                /* �м��֡������cmdid�ͳ��ȣ�ֻ��Ҫ����data */
                (VOS_VOID)VOS_MemCpy_s( ((VOS_UINT8*)pTempNode->pFrame) + uloffset,
                            (VOS_UINT32)(pTempNode->ulFrameDataLen - uloffset),
                            ((VOS_UINT8*)pFrame) + sizeof(DIAG_SERVICE_HEAD_STRU),
                            ulLocalLen);
                pTempNode->ulFrameOffset += ulLocalLen;
                diag_SaveDFR(&g_stDFRreq, (VOS_UINT8*)pFrame, ulDataLen);
            }

            (VOS_VOID)VOS_SmV(g_stDiagSrvc.ListSem);
            return pTempNode->pFrame;
        }
    }
    (VOS_VOID)VOS_SmV(g_stDiagSrvc.ListSem);

    return (DIAG_FRAME_INFO_STRU *)VOS_NULL_PTR;
}


/*****************************************************************************
 Function Name   : diag_SrvcDestroyPkt
 Description     : ɾ������ͽڵ�

 History         :
    1.c64416         2015-03-18  Draft Enact

*****************************************************************************/
VOS_VOID diag_SrvcDestroyPkt(DIAG_FRAME_INFO_STRU *pFrame)
{
    LIST_S* me = NULL;
    DIAG_SRVC_FRAME_NODE_STRU *pTempNode;

    (VOS_VOID)VOS_SmP(g_stDiagSrvc.ListSem,0);

    blist_for_each(me, &g_stDiagSrvc.ListHeader)
    {
        pTempNode = blist_entry(me, DIAG_SRVC_FRAME_NODE_STRU, FrameList);

        if(VOS_NULL_PTR == pTempNode)
        {
            (VOS_VOID)VOS_SmV(g_stDiagSrvc.ListSem);
            return ;
        }

        /* �˴�ע��stService��4G ��5G������ */
        pTempNode->stHead.index4b = pFrame->stService.index4b;
        pTempNode->stHead.eof1b   = pFrame->stService.eof1b;
        pTempNode->stHead.ff1b    = pFrame->stService.ff1b;
        if(0 == VOS_MemCmp(&pTempNode->stHead, &pFrame->stService, sizeof(DIAG_SERVICE_HEAD_STRU)))
        {
            /*ɾ���ڵ�*/
            if((VOS_NULL_PTR != pTempNode->FrameList.next) && (VOS_NULL_PTR != pTempNode->FrameList.prev))
            {
                blist_del(&pTempNode->FrameList);
            }

            if(VOS_NULL_PTR != pTempNode->pFrame)
            {
                VOS_MemFree(MSP_PID_DIAG_APP_AGENT, pTempNode->pFrame);
            }

            VOS_MemFree(MSP_PID_DIAG_APP_AGENT, pTempNode);

            break;
        }
    }

    (VOS_VOID)VOS_SmV(g_stDiagSrvc.ListSem);

    return ;
}

/*****************************************************************************
 Function Name   : diag_ServiceProc
 Description     : DIAG service ������

 History         :
    1.c64416         2014-11-18  Draft Enact

*****************************************************************************/
VOS_UINT32 diag_ServiceProc(VOS_VOID *pData, VOS_UINT32 ulDatalen)
{
    VOS_UINT32 ulRet = VOS_ERR;
    DIAG_FRAME_INFO_STRU *pHeader;
    DIAG_FRAME_INFO_STRU *pProcHead = VOS_NULL_PTR;

    /*if((VOS_NULL_PTR == pData)||(ulDatalen < sizeof(DIAG_FRAME_INFO_STRU)))
    {
        return VOS_ERR;
    }*/

    if(VOS_NULL_PTR == pData)
    {
        return VOS_ERR;
    }

    pHeader = (DIAG_FRAME_INFO_STRU*)pData;

    mdrv_diag_PTR(EN_DIAG_PTR_SERVICE_IN, 1, pHeader->ulCmdId, 0);

    /* ֻ����DIAG���� */
    if(MSP_SID_DIAG_SERVICE == SERVICE_HEAD_SID(pData))
    {
        mdrv_diag_PTR(EN_DIAG_PTR_SERVICE_1, 1, pHeader->ulCmdId, 0 );

        /* ��ʼ����������˯�� */
        wake_lock(&diag_wakelock);
        if(pHeader->stService.ff1b)
        {
            /* ÿ���зְ�ʱ����Ƿ��г�ʱ�Ľڵ� */
            diag_PktTimeoutClear();

            /* index4b��Զ�������16, ����Ϣ���֡����������16,���index�����ܴ���16 */
            if(0 == pHeader->stService.index4b)
            {
                diag_SrvcCreatePkt(pHeader);
                (VOS_VOID)diag_SrvcSavePkt(pHeader, ulDatalen);
                wake_unlock(&diag_wakelock);
                return VOS_OK;
            }
            else if(pHeader->stService.eof1b)
            {
                pProcHead = diag_SrvcSavePkt(pHeader, ulDatalen);
                if(pProcHead == NULL)
                {
                    wake_unlock(&diag_wakelock);
                    return ((VOS_UINT32)VOS_NULL_PARA);
                }
                /* 5G�зְ��Ľڵ�һ�����ߵ�5G��ʽ,4G�·ְ�һ�����ߵ�4G�ĸ�ʽ */
                ulDatalen = pProcHead->ulMsgLen + sizeof(DIAG_FRAME_INFO_STRU);
            }
            else
            {
                (VOS_VOID)diag_SrvcSavePkt(pHeader, ulDatalen);
                wake_unlock(&diag_wakelock);
                return VOS_OK;
            }
        }
        else
        {
            if(ulDatalen < pHeader->ulMsgLen + sizeof(DIAG_FRAME_INFO_STRU))
            {
                wake_unlock(&diag_wakelock);
                diag_error("rev tools data len error, rev:0x%x except:0x%x\n", \
                    ulDatalen, pHeader->ulMsgLen + (VOS_UINT32)sizeof(DIAG_FRAME_INFO_STRU));
                return ERR_MSP_INVALID_PARAMETER;
            }

            pProcHead = pHeader;

            diag_SaveDFR(&g_stDFRreq, (VOS_UINT8*)pHeader, (sizeof(DIAG_FRAME_INFO_STRU) + pHeader->ulMsgLen));
        }

        if(g_fnService && pProcHead)
        {
            /* ��¼�����N��cmdid */
            diag_LNR(EN_DIAG_LNR_CMDID, pHeader->ulCmdId, VOS_GetTick());
            diag_DumpDFInfo(pProcHead);
            ulRet = g_fnService(pProcHead);
        }
        else
        {
            ulRet = ERR_MSP_NO_INITILIZATION;
            mdrv_diag_PTR(EN_DIAG_PTR_DIAG_SERVICE_ERR1, 1, pHeader->ulCmdId, 0);
        }

        if(pHeader->stService.ff1b)
        {
            diag_SrvcDestroyPkt(pHeader);
        }

        /* �������������˯�� */
        wake_unlock(&diag_wakelock);
    }
    else
    {
        mdrv_diag_PTR(EN_DIAG_PTR_DIAG_SERVICE_ERR2, 1, pHeader->ulCmdId, 0);
        ulRet = ERR_MSP_INVALID_PARAMETER;
    }

    return ulRet;
}


/*****************************************************************************
 Function Name   : diag_ServiceProcReg
 Description     : DIAG service����ע��ӿ�

 History         :
    1.c64416         2014-11-18  Draft Enact

*****************************************************************************/
VOS_VOID diag_ServiceProcReg(DIAG_SERVICE_FUNC pServiceFn)
{
    g_fnService = pServiceFn;
}

VOS_VOID diag_ServiceInit(VOS_VOID)
{
    VOS_UINT32 ret;

    /* �����ڵ㱣���ź���*/
    ret = VOS_SmBCreate("SRVC", 1, VOS_SEMA4_FIFO, &g_stDiagSrvc.ListSem);
    if(VOS_OK != ret)
    {
        diag_error("VOS_SmBCreate failed.\n");
    }

    /* ��ʼ���������� */
    blist_head_init(&g_stDiagSrvc.ListHeader);

    msp_ServiceProcReg(MSP_SID_DIAG_SERVICE, diag_ServiceProc);

    /* coverity[alloc_arg] */
    ret = diag_CreateDFR("DFRReqA", DIAG_DFR_BUFFER_MAX, &g_stDFRreq);
    if(ret)
    {
        diag_error("diag_CreateDFR DFR_Req failed\n");
    }

    /* coverity[alloc_arg] */
    ret = diag_CreateDFR("DFRCnfA", DIAG_DFR_BUFFER_MAX, &g_stDFRcnf);

    if(ret)
    {
        diag_error("diag_CreateDFR DFR_Cnf failed\n");
    }
}

/*****************************************************************************
* �� �� ��  :iqi_data_buffer_write
* ��������  : д�����ݣ�����дָ��
* �������  : id����Ϣ���ݡ�����
* �������  : ��
* �� �� ֵ  : �Ƿ�ɹ���־
*****************************************************************************/
VOS_VOID diag_SrvcPackWrite(SOCP_BUFFER_RW_STRU *pRWBuffer, const VOS_VOID * pPayload, VOS_UINT32 u32DataLen )
{
    VOS_UINT32 ulTempLen;
    VOS_UINT32 ulTempLen1;

    if(u32DataLen == 0)
    {
        return;
    }

    if(pRWBuffer->u32Size > u32DataLen)
    {
       (void)VOS_MemCpy_s(((VOS_UINT8*)pRWBuffer->pBuffer), u32DataLen, pPayload, u32DataLen);
        pRWBuffer->pBuffer = pRWBuffer->pBuffer + u32DataLen;
        pRWBuffer->u32Size = pRWBuffer->u32Size - u32DataLen;
    }
    else
    {
        ulTempLen = pRWBuffer->u32Size;
        (void)VOS_MemCpy_s(((VOS_UINT8*)pRWBuffer->pBuffer), pRWBuffer->u32Size, pPayload,ulTempLen);

        ulTempLen1 = u32DataLen - pRWBuffer->u32Size;
        if(ulTempLen1)
        {
            (void)VOS_MemCpy_s(pRWBuffer->pRbBuffer, pRWBuffer->u32RbSize, ((VOS_UINT8 *)pPayload + ulTempLen) ,ulTempLen1);
        }
        pRWBuffer->pBuffer = pRWBuffer->pRbBuffer + ulTempLen1;
        pRWBuffer->u32Size = pRWBuffer->u32RbSize - ulTempLen1;
        pRWBuffer->pRbBuffer = VOS_NULL;
        pRWBuffer->u32RbSize = 0;

    }

    return;
}


/*****************************************************************************
 Function Name   : diag_SrvcPackFirst
 Description     : ���ְ�ʱ�ķ�װ����ְ�ʱ����һ���ķ�װ

 History         :
    1.c64416         2015-03-12  Draft Enact

*****************************************************************************/
VOS_UINT32 diag_SrvcPackIndSend(DIAG_MSG_REPORT_HEAD_STRU *pData)
{
    SOCP_BUFFER_RW_STRU stSocpBuf = {VOS_NULL, VOS_NULL, 0, 0};
    DIAG_SRV_SOCP_HEADER_STRU *pstSocpHeader;
    VOS_UINT32 ulTmpLen =0;
    VOS_UINT32 ret = ERR_MSP_FAILURE;
    VOS_ULONG  ulLockLevel;

    ulTmpLen = ALIGN_DDR_WITH_8BYTE(pData->ulHeaderSize + pData->ulDataSize);

    pstSocpHeader = &(((DIAG_SRV_HEADER_STRU *)(pData->pHeaderData))->socp_header);
    pstSocpHeader->ulDataLen = pData->ulHeaderSize + pData->ulDataSize - sizeof(DIAG_SRV_SOCP_HEADER_STRU);

    VOS_SpinLockIntLock(&g_stSrvIndSrcBuffSpinLock, ulLockLevel);

    if(mdrv_socp_get_write_buff(g_SrvCtrl.ulIndChannelID, &stSocpBuf))
    {
        VOS_SpinUnlockIntUnlock(&g_stSrvIndSrcBuffSpinLock, ulLockLevel);
        return ERR_MSP_GET_WRITE_BUFF_FAIL;/* ����ʧ�� */
    }
    /* �����ַת�� */
    if((stSocpBuf.u32Size + stSocpBuf.u32RbSize) >= ulTmpLen)
    {
        stSocpBuf.pBuffer = (char *)mdrv_scm_ind_src_phy_to_virt((VOS_UINT8*)stSocpBuf.pBuffer);
        stSocpBuf.pRbBuffer = (char *)mdrv_scm_ind_src_phy_to_virt((VOS_UINT8*)stSocpBuf.pRbBuffer);
    }
    else
    {
        VOS_SpinUnlockIntUnlock(&g_stSrvIndSrcBuffSpinLock, ulLockLevel);
        return ERR_MSP_NOT_FREEE_SPACE;
    }

    if(!(stSocpBuf.pBuffer) && (!stSocpBuf.pRbBuffer))
    {
        VOS_SpinUnlockIntUnlock(&g_stSrvIndSrcBuffSpinLock, ulLockLevel);
        diag_error("get virt fail, pBuffer:%pK pRbBuffer:%pK\n", stSocpBuf.pBuffer, stSocpBuf.pRbBuffer);
        return ERR_MSP_GET_VIRT_ADDR_FAIL;
    }
    diag_SrvcPackWrite(&stSocpBuf, pData->pHeaderData, pData->ulHeaderSize);
    diag_SrvcPackWrite(&stSocpBuf, pData->pData, pData->ulDataSize);

    ret = mdrv_socp_write_done(g_SrvCtrl.ulIndChannelID, ulTmpLen);
    VOS_SpinUnlockIntUnlock(&g_stSrvIndSrcBuffSpinLock, ulLockLevel);
    if(ret)
    {
        diag_debug_ind_src_lost(EN_DIAG_SRC_LOST_ALLOC, ulTmpLen);
        DIAG_DEBUG_SDM_FUN(EN_DIAG_CBT_API_PACKET_ERR_REQ, ret, __LINE__, 2);
    }

    return ERR_MSP_SUCCESS;
}


/*****************************************************************************
 Function Name   : diag_ServicePackData
 Description     : DIAG service�����ϱ����ݽӿ�

 History         :
    1.c64416         2014-11-18  Draft Enact
    2.c64416         2015-03-14  �����ְ��������
                    ��֡�ṹ���ƣ��ְ����������Լ��:
                    A. ��һ����ulCmdId��ulMsgLen�������ֱ�Ӹ�����
                    B. �����һ���⣬����ÿ�������뱣֤����󳤶����
                    C. transid��timestamp��Ϊ����һ��ְ��ı�־

*****************************************************************************/
VOS_UINT32 diag_ServicePackData(DIAG_MSG_REPORT_HEAD_STRU *pData)
{
    DIAG_MSG_REPORT_HEAD_STRU stReportInfo;
    DIAG_FRAME_INFO_STRU      *pstFrameHeader;
    VOS_UINT32 ret = ERR_MSP_FAILURE;
    VOS_INT32  lDataLen = 0;         /* �����ܳ��� */
    VOS_UINT32 ulCurlen = 0;        /* ��ǰ�ѷ�������ݳ��� */
    VOS_UINT32 ulIndex = 0;
    VOS_UINT32 ulFrameHeaderSize = 0;
    VOS_UINT32 ulSendLen = 0;

    ulFrameHeaderSize = pData->ulHeaderSize - sizeof(DIAG_SRV_SOCP_HEADER_STRU);

    /* ��Ҫ�������ݵ��ܳ��� */
    lDataLen = (VOS_INT32)(ulFrameHeaderSize + pData->ulDataSize);
    if(lDataLen > (VOS_INT32)(DIAG_FRAME_SUM_LEN - 15*sizeof(DIAG_FRAME_INFO_STRU)))
    {
        diag_error("report length error(0x%x).\n", lDataLen);
        return ERR_MSP_FAILURE;
    }

    diag_ThroughputSave(EN_DIAG_THRPUT_DATA_CHN_ENC, (VOS_UINT32)lDataLen);
    pstFrameHeader = &(((DIAG_SRV_HEADER_STRU *)(pData->pHeaderData))->frame_header);

    ulCurlen = (VOS_UINT32)lDataLen;
    if(lDataLen > DIAG_FRAME_MAX_LEN)
    {
        ulCurlen = DIAG_FRAME_MAX_LEN;
        pstFrameHeader->stService.ff1b = 1;
    }
    else
    {
        pstFrameHeader->stService.ff1b = 0;
    }

    stReportInfo.pHeaderData    = pData->pHeaderData;
    stReportInfo.ulHeaderSize   = pData->ulHeaderSize;
    stReportInfo.pData          = pData->pData;
    stReportInfo.ulDataSize     = ulCurlen  - ulFrameHeaderSize;
    ulSendLen += stReportInfo.ulDataSize;

    /* ���ڷְ�ʱ��һ������cmdid��Ҫ��䣬������û�У����Ե�һ���������� */
    ret = diag_SrvcPackIndSend(&stReportInfo);
    if(ret)
    {
        return ERR_MSP_FAILURE;
    }

    /* ��Ҫ�ְ� */
    if(pstFrameHeader->stService.ff1b)
    {
        /* ʣ���û�з��͵����ݵĳ��� */
        lDataLen = lDataLen - (VOS_INT32)ulCurlen;

        while(lDataLen > 0)
        {
            pstFrameHeader->stService.index4b = ++ulIndex;
            if( (lDataLen + sizeof(DIAG_SERVICE_HEAD_STRU)) > DIAG_FRAME_MAX_LEN )
            {
                ulCurlen = DIAG_FRAME_MAX_LEN - sizeof(DIAG_SERVICE_HEAD_STRU);
            }
            else
            {
                ulCurlen = (VOS_UINT32)lDataLen;
                pstFrameHeader->stService.eof1b = 1;    /* ��¼�ְ�������� */
            }
            stReportInfo.pHeaderData    = pData->pHeaderData;
            stReportInfo.ulHeaderSize   = sizeof(DIAG_SRV_SOCP_HEADER_STRU) + sizeof(DIAG_SERVICE_HEAD_STRU);
            stReportInfo.pData          = (VOS_UINT8*)(pData->pData) + ulSendLen;
            stReportInfo.ulDataSize     = ulCurlen;
            ulSendLen += stReportInfo.ulDataSize;

            ret = diag_SrvcPackIndSend(&stReportInfo);
            if(ret)
            {
                return ERR_MSP_FAILURE;
            }

            lDataLen -= (VOS_INT32)ulCurlen;
        }
    }

    return ERR_MSP_SUCCESS;
}

/*****************************************************************************
 Function Name   : diag_SrvcPackFirst
 Description     : ���ְ�ʱ�ķ�װ����ְ�ʱ����һ���ķ�װ

 History         :
    1.c64416         2015-03-12  Draft Enact

*****************************************************************************/
VOS_UINT32 diag_SrvcPackCnfSend(DIAG_MSG_REPORT_HEAD_STRU *pData)
{
    SOCP_BUFFER_RW_STRU         stSocpBuf = {VOS_NULL, VOS_NULL, 0, 0};
    DIAG_SRV_SOCP_HEADER_STRU*  pstSocpHeader;
    VOS_UINT32 ulTmpLen =0;
    VOS_UINT32 ret = ERR_MSP_FAILURE;
    VOS_ULONG  ulLockLevel;

    ulTmpLen = ALIGN_DDR_WITH_8BYTE(pData->ulHeaderSize + pData->ulDataSize);

    pstSocpHeader = &(((DIAG_SRV_HEADER_STRU *)(pData->pHeaderData))->socp_header);
    pstSocpHeader->ulDataLen = pData->ulHeaderSize + pData->ulDataSize - sizeof(DIAG_SRV_SOCP_HEADER_STRU);

    VOS_SpinLockIntLock(&g_stSrvCnfSrcBuffSpinLock, ulLockLevel);

    if(mdrv_socp_get_write_buff(g_SrvCtrl.ulCnfChannelID, &stSocpBuf))
    {
        VOS_SpinUnlockIntUnlock(&g_stSrvCnfSrcBuffSpinLock, ulLockLevel);
        return ERR_MSP_GET_WRITE_BUFF_FAIL;/* ����ʧ�� */
    }
    /* �����ַת�� */
    if((stSocpBuf.u32Size + stSocpBuf.u32RbSize) >= ulTmpLen)
    {
        stSocpBuf.pBuffer = (char *)mdrv_scm_cnf_src_phy_to_virt((VOS_UINT8*)stSocpBuf.pBuffer);
        stSocpBuf.pRbBuffer = (char *)mdrv_scm_cnf_src_phy_to_virt((VOS_UINT8*)stSocpBuf.pRbBuffer);
    }
    else
    {
        VOS_SpinUnlockIntUnlock(&g_stSrvCnfSrcBuffSpinLock, ulLockLevel);
        return ERR_MSP_NOT_FREEE_SPACE;
    }

    if(!(stSocpBuf.pBuffer) && (!stSocpBuf.pRbBuffer))
    {
        VOS_SpinUnlockIntUnlock(&g_stSrvIndSrcBuffSpinLock, ulLockLevel);
        diag_error("get virt fail, pBuffer:%pK pRbBuffer:%pK\n", stSocpBuf.pBuffer, stSocpBuf.pRbBuffer);
        return ERR_MSP_GET_VIRT_ADDR_FAIL;
    }

    diag_SrvcPackWrite(&stSocpBuf, pData->pHeaderData, pData->ulHeaderSize);
    diag_SrvcPackWrite(&stSocpBuf, pData->pData, pData->ulDataSize);

    ret = mdrv_socp_write_done(g_SrvCtrl.ulCnfChannelID, ulTmpLen);
    VOS_SpinUnlockIntUnlock(&g_stSrvCnfSrcBuffSpinLock, ulLockLevel);
    if(ret)
    {
        DIAG_DEBUG_SDM_FUN(EN_DIAG_CBT_API_PACKET_ERR_REQ, ret, __LINE__, 4);
    }

    return ERR_MSP_SUCCESS;
}


/*****************************************************************************
 Function Name   : diag_ServicePackData
 Description     : DIAG service�����ϱ����ݽӿ�

 History         :
    1.c64416         2014-11-18  Draft Enact
    2.c64416         2015-03-14  �����ְ��������
                    ��֡�ṹ���ƣ��ְ����������Լ��:
                    A. ��һ����ulCmdId��ulMsgLen�������ֱ�Ӹ�����
                    B. �����һ���⣬����ÿ�������뱣֤����󳤶����
                    C. transid��timestamp��Ϊ����һ��ְ��ı�־

*****************************************************************************/
VOS_UINT32 diag_ServicePackCnfData(DIAG_MSG_REPORT_HEAD_STRU *pData)
{
    DIAG_MSG_REPORT_HEAD_STRU stReportInfo;
    DIAG_FRAME_INFO_STRU      *pstFrameHeader;
    VOS_UINT32 ret = ERR_MSP_FAILURE;
    VOS_INT32  lDataLen = 0;         /* �����ܳ��� */
    VOS_UINT32 ulCurlen = 0;        /* ��ǰ�ѷ�������ݳ��� */
    VOS_UINT32 ulSplit = 0;
    VOS_UINT32 ulIndex = 0;
    VOS_UINT32 ulFrameHeaderSize = 0;
    VOS_UINT32 ulSendLen = 0;

    ulFrameHeaderSize = pData->ulHeaderSize - sizeof(DIAG_SRV_SOCP_HEADER_STRU);
    pstFrameHeader = &(((DIAG_SRV_HEADER_STRU *)(pData->pHeaderData))->frame_header);

    mdrv_diag_PTR(EN_DIAG_PTR_SERVICE_PACKETDATA, 1, pstFrameHeader->ulCmdId, 0);

    /* ��Ҫ�������ݵ��ܳ��� */
    lDataLen = (VOS_INT32)(ulFrameHeaderSize + pData->ulDataSize);
    if(lDataLen > (VOS_INT32)(DIAG_FRAME_SUM_LEN - 15*sizeof(DIAG_FRAME_INFO_STRU)))
    {
        diag_error("report length error(0x%x).\n", lDataLen);
        return ERR_MSP_FAILURE;
    }

    diag_ThroughputSave(EN_DIAG_THRPUT_DATA_CHN_ENC, (VOS_UINT32)lDataLen);

    ulCurlen = (VOS_UINT32)lDataLen;
    if(lDataLen > DIAG_FRAME_MAX_LEN)
    {
        ulCurlen = DIAG_FRAME_MAX_LEN;
        ulSplit = 1;
    }
    else
    {
        ulSplit = 0;
    }

    /* ��������ͷ */
    pstFrameHeader = &(((DIAG_SRV_HEADER_STRU *)(pData->pHeaderData))->frame_header);
    pstFrameHeader->stService.mt2b = DIAG_MT_CNF;
    pstFrameHeader->stService.ff1b = ulSplit;

    stReportInfo.pHeaderData    = pData->pHeaderData;
    stReportInfo.ulHeaderSize   = pData->ulHeaderSize;
    stReportInfo.pData          = pData->pData;
    stReportInfo.ulDataSize     = ulCurlen  - ulFrameHeaderSize;
    ulSendLen += stReportInfo.ulDataSize;

    /* ���ڷְ�ʱ��һ������cmdid��Ҫ��䣬������û�У����Ե�һ���������� */
    ret = diag_SrvcPackCnfSend(&stReportInfo);
    if(ret)
    {
        return ERR_MSP_FAILURE;
    }

    /* ��Ҫ�ְ� */
    if(ulSplit)
    {
        /* ʣ���û�з��͵����ݵĳ��� */
        lDataLen = lDataLen - (VOS_INT32)ulCurlen;

        while(lDataLen > 0)
        {
            pstFrameHeader->stService.index4b = ++ulIndex;
            if( (lDataLen + sizeof(DIAG_SERVICE_HEAD_STRU)) > DIAG_FRAME_MAX_LEN )
            {
                ulCurlen = DIAG_FRAME_MAX_LEN - sizeof(DIAG_SERVICE_HEAD_STRU);
            }
            else
            {
                ulCurlen = (VOS_UINT32)lDataLen;
                pstFrameHeader->stService.eof1b = 1;    /* ��¼�ְ�������� */
            }
            stReportInfo.pHeaderData    = pData->pHeaderData;
            stReportInfo.ulHeaderSize   = sizeof(DIAG_SRV_SOCP_HEADER_STRU) + sizeof(DIAG_SERVICE_HEAD_STRU);
            stReportInfo.pData          = (VOS_UINT8*)(pData->pData) + ulSendLen;
            stReportInfo.ulDataSize     = ulCurlen;
            ulSendLen += stReportInfo.ulDataSize;

            ret = diag_SrvcPackCnfSend(&stReportInfo);
            if(ret)
            {
                return ERR_MSP_FAILURE;
            }

            lDataLen -= (VOS_INT32)ulCurlen;
        }
    }

    return ERR_MSP_SUCCESS;
}


VOS_UINT32 diag_ServicePacketResetData(DIAG_MSG_REPORT_HEAD_STRU *pData)
{
    DIAG_MSG_REPORT_HEAD_STRU   stReportInfo;
    DIAG_FRAME_INFO_STRU*       pstFrameHeader;
    VOS_UINT32 ret = ERR_MSP_FAILURE;
    VOS_INT32  lDataLen = 0;         /* �����ܳ��� */
    VOS_UINT32 ulCurlen = 0;        /* ��ǰ�ѷ�������ݳ��� */

    pstFrameHeader = &(((DIAG_SRV_HEADER_STRU *)(pData->pHeaderData))->frame_header);

    /* ��Ҫ�������ݵ��ܳ��� */
    lDataLen = (VOS_INT32)(pData->ulHeaderSize + pData->ulDataSize);
    if(lDataLen - sizeof(DIAG_SRV_SOCP_HEADER_STRU) > (VOS_INT32)(DIAG_FRAME_SUM_LEN - 15*sizeof(DIAG_FRAME_INFO_STRU)))
    {
        diag_error("report length error(0x%x).\n", lDataLen);
        return ERR_MSP_FAILURE;
    }

    ulCurlen = (VOS_UINT32)lDataLen;
    if(lDataLen > DIAG_FRAME_MAX_LEN)
    {
        return ERR_MSP_INALID_LEN_ERROR;
    }

    /* ��������ͷ */
    pstFrameHeader = &(((DIAG_SRV_HEADER_STRU *)(pData->pHeaderData))->frame_header);
    pstFrameHeader->stService.mt2b = DIAG_MT_IND;
    pstFrameHeader->stService.ff1b = 0;
    pstFrameHeader->ulMsgLen = (VOS_UINT32)lDataLen - sizeof(DIAG_SRV_HEADER_STRU);

    stReportInfo.pHeaderData    = pData->pHeaderData;
    stReportInfo.ulHeaderSize   = pData->ulHeaderSize;
    stReportInfo.pData          = pData->pData;
    stReportInfo.ulDataSize     = ulCurlen  - pData->ulHeaderSize;

    /* ���ڷְ�ʱ��һ������cmdid��Ҫ��䣬������û�У����Ե�һ���������� */
    ret = diag_SrvcPackCnfSend(&stReportInfo);
    if(ret)
    {
        return ERR_MSP_FAILURE;
    }

    return ERR_MSP_SUCCESS;
}

VOS_VOID DIAG_DebugDFR(VOS_VOID)
{
    diag_crit("%d, %s, %d.\n", g_stDFRreq.ulMagicNum, g_stDFRreq.name, g_stDFRreq.ulLen);
    diag_GetDFR(&g_stDFRreq);
    diag_crit("%d, %s, %d.\n", g_stDFRcnf.ulMagicNum, g_stDFRcnf.name, g_stDFRcnf.ulLen);
    diag_GetDFR(&g_stDFRcnf);
}




