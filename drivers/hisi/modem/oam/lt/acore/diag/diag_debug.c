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



/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/

#include <mdrv.h>
#include "msp_debug.h"
#include "diag_debug.h"
#include "diag_api.h"
#include "diag_cfg.h"
#include "diag_msgmsp.h"
#include "msp.h"
#include "diag_common.h"
#include "nv_stru_drv.h"
#include "acore_nv_stru_drv.h"
#include "nv_stru_pam.h"
#include "nv_stru_sys.h"
#include "soc_socp_adapter.h"
#include "diag_acore_common.h"


#define    THIS_FILE_ID        MSP_FILE_ID_DIAG_DEBUG_C

#define    DIAG_LOG_PATH       MODEM_LOG_ROOT"/drv/DIAG/"

extern VOS_UINT8 g_EventModuleCfg[DIAG_CFG_PID_NUM];
VOS_UINT32 g_DiagDebugCfg = DIAG_CFG_SWT_CLOSE;
#define DIRPAH_LEN  64
/*****************************************************************************
  2 Declare the Global Variable
*****************************************************************************/

/*****************************************************************************
  3 Function
*****************************************************************************/

/*****************************************************************************
 Function Name   : CBT : Count Branch Timestamp (��������֧��ʱ�����λ����)
 Description     : ����ͳ�ƴ��������߷�֧�����ⶨλ
*****************************************************************************/

DIAG_CBT_INFO_TBL_STRU g_astCBTInfoTbl[EN_DIAG_DEBUG_INFO_MAX] = {{0}};

DIAG_CBT_INFO_TBL_STRU* diag_DebugGetInfo(VOS_VOID)
{
   return g_astCBTInfoTbl;
}

/*****************************************************************************
 Function Name   : diag_CBT
 Description     :
 Input           :DIAG_CBT_ID_ENUM ulType
                VOS_UINT32 ulRserved1
                VOS_UINT32 ulRserved2
                VOS_UINT32 ulRserved3
*****************************************************************************/
VOS_VOID diag_CBT(DIAG_CBT_ID_ENUM ulType,
                     VOS_UINT32 ulRserved1, VOS_UINT32 ulRserved2, VOS_UINT32 ulRserved3)
{
    g_astCBTInfoTbl[ulType].ulCalledNum += 1;
    g_astCBTInfoTbl[ulType].ulRserved1 = ulRserved1;
    g_astCBTInfoTbl[ulType].ulRserved2 = ulRserved2;
    g_astCBTInfoTbl[ulType].ulRserved3 = ulRserved3;
    g_astCBTInfoTbl[ulType].ulRtcTime  = VOS_GetTick();
}


/*****************************************************************************
 Function Name   : LNR : Last N Ring buffer store (���N����Ϣѭ���洢����)
 Description     : ���������N����Ϣ
*****************************************************************************/

DIAG_LNR_INFO_TBL_STRU g_astLNRInfoTbl[EN_DIAG_LNR_INFO_MAX] = {{0}};


VOS_VOID diag_LNR(DIAG_LNR_ID_ENUM ulType, VOS_UINT32 ulRserved1, VOS_UINT32 ulRserved2)
{
    g_astLNRInfoTbl[ulType].ulRserved1[g_astLNRInfoTbl[ulType].ulCur] = ulRserved1;
    g_astLNRInfoTbl[ulType].ulRserved2[g_astLNRInfoTbl[ulType].ulCur] = ulRserved2;
    g_astLNRInfoTbl[ulType].ulCur = (g_astLNRInfoTbl[ulType].ulCur+1)%DIAG_LNR_NUMBER;
}

VOS_VOID DIAG_ShowLNR(DIAG_LNR_ID_ENUM ulType, VOS_UINT32 n)
{
    VOS_UINT32 i;
    VOS_UINT32 cur;

    cur = (g_astLNRInfoTbl[ulType].ulCur + DIAG_LNR_NUMBER - n)%DIAG_LNR_NUMBER;

    for(i = 0; i <n; i++)
    {
        diag_crit("0x%x, 0x%x.\n", g_astLNRInfoTbl[ulType].ulRserved1[cur], g_astLNRInfoTbl[ulType].ulRserved2[cur]);
        cur = (cur + 1)%DIAG_LNR_NUMBER;
    }
}



VOS_UINT32 diag_CreateDFR(VOS_CHAR *name, VOS_UINT32 ulLen, DIAG_DFR_INFO_STRU *pDfr)
{
    if((VOS_NULL == name)
        || (VOS_NULL == pDfr)
        || (0 == ulLen)
        || (ulLen%4)
        || (ulLen > DIAG_DFR_BUFFER_MAX))
    {
        return ERR_MSP_FAILURE;
    }

    if (VOS_OK != VOS_SmMCreate("DFR", VOS_SEMA4_PRIOR | VOS_SEMA4_INVERSION_SAFE, &pDfr->semid))
    {
        diag_crit("%s %d.\n", __FUNCTION__, __LINE__);
        return ERR_MSP_FAILURE;
    }

    pDfr->pData = VOS_CacheMemAllocDebug(ulLen,(VOS_UINT32)DIAG_COOKIE_CREATE_DFR);
    if(VOS_NULL == pDfr->pData)
    {
        diag_crit("%s %d.\n", __FUNCTION__, __LINE__);
        return ERR_MSP_FAILURE;
    }

    (VOS_VOID)VOS_MemSet_s(pDfr->pData, ulLen, 0, ulLen);

    (VOS_VOID)VOS_MemCpy_s(pDfr->name, DIAG_DFR_NAME_MAX, name, VOS_StrNLen(name, DIAG_DFR_NAME_MAX-1));
    pDfr->name[DIAG_DFR_NAME_MAX-1]=0;

    pDfr->ulCur = 0;
    pDfr->ulLen = ulLen;
    pDfr->ulMagicNum = DIAG_DFR_MAGIC_NUM;

    return ERR_MSP_SUCCESS;
}



VOS_VOID diag_SaveDFR(DIAG_DFR_INFO_STRU *pDfr, VOS_UINT8 *pData, VOS_UINT32 ulLen)
{
    VOS_UINT32 ulSize;
    DIAG_DFR_HEADER_STRU stDfrHeader;

    if((VOS_NULL== pDfr)
        || (VOS_NULL == pData)
        || (DIAG_DFR_MAGIC_NUM != pDfr->ulMagicNum)
        || (ulLen > DIAG_DFR_BUFFER_MAX)
        || (pDfr->ulLen > DIAG_DFR_BUFFER_MAX))
    {
        return ;
    }

    /* ������������ڽ����У���Ҫ�ȴ������ */
    if (VOS_OK != VOS_SmP(pDfr->semid, 0))
    {
        return ;
    }

    stDfrHeader.ulStart = DIAG_DFR_START_NUM;
    stDfrHeader.ulTime  = VOS_GetSlice();

    /* ������ʼ��Ǻ�ʱ��� */
    if((pDfr->ulCur + sizeof(DIAG_DFR_HEADER_STRU)) <= pDfr->ulLen)
    {
        (VOS_VOID)VOS_MemCpy_s(&(pDfr->pData[pDfr->ulCur]), pDfr->ulLen-pDfr->ulCur, &stDfrHeader, sizeof(stDfrHeader));
    }
    else
    {
        ulSize = (pDfr->ulLen - pDfr->ulCur);
        (VOS_VOID)VOS_MemCpy_s(&(pDfr->pData[pDfr->ulCur]), ulSize, &stDfrHeader, ulSize);
        (VOS_VOID)VOS_MemCpy_s(&(pDfr->pData[0]), pDfr->ulCur,
            (((VOS_UINT8*)&stDfrHeader)+ulSize), (sizeof(stDfrHeader) - ulSize));
    }
    pDfr->ulCur = (DFR_ALIGN_WITH_4BYTE(pDfr->ulCur + sizeof(DIAG_DFR_HEADER_STRU))) % pDfr->ulLen;

    /* �������� */
    if((pDfr->ulCur + ulLen) <= pDfr->ulLen)
    {
        (VOS_VOID)VOS_MemCpy_s(&(pDfr->pData[pDfr->ulCur]), pDfr->ulLen-pDfr->ulCur, pData, ulLen);
    }
    else
    {
        ulSize = (pDfr->ulLen - pDfr->ulCur);
        (VOS_VOID)VOS_MemCpy_s(&(pDfr->pData[pDfr->ulCur]), ulSize, pData, ulSize);
        (VOS_VOID)VOS_MemCpy_s(&(pDfr->pData[0]), pDfr->ulCur, (pData + ulSize), (ulLen - ulSize));
    }
    pDfr->ulCur = (DFR_ALIGN_WITH_4BYTE(pDfr->ulCur + ulLen)) % pDfr->ulLen;

    (VOS_VOID)VOS_SmV(pDfr->semid);

    return ;
}



VOS_VOID diag_GetDFR(DIAG_DFR_INFO_STRU *pDfr)
{
    void *pFile;
    VOS_UINT32 ret, len;
    VOS_CHAR FilePath[64] = {0};
    VOS_CHAR *DirPath = DIAG_LOG_PATH;
    VOS_CHAR aucInfo[DIAG_DEBUG_INFO_LEN];

    if((VOS_NULL == pDfr) || (DIAG_DFR_MAGIC_NUM != pDfr->ulMagicNum))
    {
        diag_crit("%s %d.\n", __FUNCTION__, __LINE__);
        return ;
    }

    /* ���DIAGĿ¼���������ȴ���Ŀ¼ */
    if (VOS_OK != mdrv_file_access(DirPath, 0))
    {
        if (VOS_OK != mdrv_file_mkdir(DirPath))
        {
            diag_error(" mdrv_file_mkdir %s failed.\n", DirPath);
            return ;
        }
    }

    len = VOS_StrNLen(DIAG_LOG_PATH, DIRPAH_LEN);

    (VOS_VOID)VOS_MemCpy_s(FilePath, sizeof(FilePath), DIAG_LOG_PATH, len);

    (VOS_VOID)VOS_MemCpy_s((FilePath + len), sizeof(FilePath)-len, pDfr->name, VOS_StrNLen(pDfr->name,DIAG_DFR_NAME_MAX));

    pFile = mdrv_file_open(FilePath, "wb+");
    if(pFile == 0)
    {
        diag_error(" mdrv_file_open failed.\n");

        return ;
    }

    ret = DIAG_DebugFileHeader(pFile);
    if(VOS_OK != ret)
    {
        diag_error(" DIAG_DebugFileHeader failed\n");
        (VOS_VOID)mdrv_file_close(pFile);
        return ;
    }

    (VOS_VOID)VOS_MemSet_s(aucInfo, sizeof(aucInfo), 0, sizeof(aucInfo));
    (VOS_VOID)VOS_MemCpy_s(aucInfo, sizeof(aucInfo), "DIAG DFR info", VOS_StrNLen(("DIAG DFR info"),(DIAG_DEBUG_INFO_LEN-1)));

    /* ͨ����Ϣ */
    ret = (VOS_UINT32)mdrv_file_write(aucInfo, 1, DIAG_DEBUG_INFO_LEN, pFile);
    if(ret != DIAG_DEBUG_INFO_LEN)
    {
        diag_error("mdrv_file_write DIAG number info failed\n");
    }

    /* ��ǰָ�� */
    ret = (VOS_UINT32)mdrv_file_write(&pDfr->ulCur, 1, sizeof(pDfr->ulCur), pFile);
    if(ret != sizeof(pDfr->ulCur))
    {
        diag_error(" mdrv_file_write failed\n");
    }

    /* ���������� */
    ret = (VOS_UINT32)mdrv_file_write(&pDfr->ulLen, 1, sizeof(pDfr->ulCur), pFile);
    if(ret != sizeof(pDfr->ulCur))
    {
        diag_error(" mdrv_file_write failed\n");
    }

    ret = (VOS_UINT32)mdrv_file_write(pDfr->pData, 1, pDfr->ulLen, pFile);
    if(ret != pDfr->ulLen)
    {
        diag_error(" mdrv_file_write failed\n");
    }

    DIAG_DebugFileTail(pFile, FilePath);

    (VOS_VOID)mdrv_file_close(pFile);

    return ;
}


/*****************************************************************************
 Function Name   : DIAG_ShowLogCfg
 Description     : �������������PID��ѯlog���ÿ�����Ϣ
*****************************************************************************/
VOS_VOID DIAG_ShowLogCfg(VOS_UINT32 ulModuleId)
{
    VOS_UINT32 level;

    /*���DIAG�Ƿ��ʼ����HSO�Ƿ�������*/
    if(!DIAG_IS_CONN_ON)
    {
        diag_crit("δ���ӹ������!\n");
    }

    if(DIAG_CFG_MODULE_IS_INVALID((VOS_INT32)ulModuleId))
    {
        diag_crit("�Ƿ�PID. PID��Χ(%d -- %d)!\n", VOS_PID_DOPRAEND, VOS_PID_BUTT);
        return;
    }

    diag_crit("��ӡ�ܿ��� 0x%x, ȫFF��ʾ��Ч.\n", g_PrintTotalCfg);

    /* level�д洢��ֵ(0|ERROR|WARNING|NORMAL|INFO|0|0|0) bit 6-3 �ֱ��ʾERROR-INFO */
    level = g_PrintModuleCfg[ulModuleId - VOS_PID_DOPRAEND];
    if(level & 0x08)
    {
        diag_crit("PID %d �Ĵ�ӡ����Ϊ info.\n", ulModuleId);
    }
    else if(level & 0x10)
    {
        diag_crit("PID %d �Ĵ�ӡ����Ϊ normal.\n", ulModuleId);
    }
    else if(level & 0x20)
    {
        diag_crit("PID %d �Ĵ�ӡ����Ϊ warning.\n", ulModuleId);
    }
    else if(level & 0x40)
    {
        diag_crit("PID %d �Ĵ�ӡ����Ϊ error.\n", ulModuleId);
    }
    else
    {
        diag_crit("PID %d �Ĵ�ӡ����Ϊ off.\n", ulModuleId);
    }

    diag_crit("��ӡʧ���ܴ��� %d.\n", g_astCBTInfoTbl[EN_DIAG_CBT_API_PRINTFV_ERR].ulCalledNum);
    diag_crit("��ӡ�ɹ��ܴ��� %d.\n", g_astCBTInfoTbl[EN_DIAG_CBT_API_PRINTFV_OK].ulCalledNum);
}

/*****************************************************************************
 Function Name   : DIAG_ShowEventCfg
 Description     : ��ѯEVENT���ÿ�����Ϣ
*****************************************************************************/
VOS_VOID DIAG_ShowEventCfg(VOS_UINT32 ulpid)
{
    /*���DIAG�Ƿ��ʼ����HSO�Ƿ�������*/
    if(!DIAG_IS_CONN_ON)
    {
        diag_crit("δ���ӹ������!\n");
    }
    else if(!DIAG_IS_EVENT_ON)
    {
        diag_crit("event�ܿ���δ��!\n");
    }
    else
    {
        if(DIAG_CFG_MODULE_IS_INVALID(ulpid))
        {
            diag_crit("�Ƿ�PID: A(%d -- %d), C(%d -- %d)!\n",
                VOS_PID_CPU_ID_1_DOPRAEND, VOS_CPU_ID_1_PID_BUTT,
                VOS_PID_CPU_ID_0_DOPRAEND, VOS_CPU_ID_0_PID_BUTT);
        }
        else
        {
            if(g_EventModuleCfg[ulpid - VOS_PID_DOPRAEND])
            {
                diag_crit("ulpid %d event�����Ѵ�!\n", ulpid);
            }
            else
            {
                diag_crit("ulpid %d event�����ѹر�!\n", ulpid);
            }
        }
    }

    diag_crit("Event�ϱ�ʧ���ܴ��� %d.\n", g_astCBTInfoTbl[EN_DIAG_CBT_API_EVENT_ERR].ulCalledNum);
    diag_crit("Event�ϱ��ɹ��ܴ��� %d.\n", g_astCBTInfoTbl[EN_DIAG_CBT_API_EVENT_OK].ulCalledNum);
}

/*****************************************************************************
 Function Name   : DIAG_ShowAirCfg
 Description     : ��ѯ�տ����ÿ�����Ϣ
*****************************************************************************/
VOS_VOID DIAG_ShowAirCfg(VOS_VOID)
{
    /*���DIAG�Ƿ��ʼ����HSO�Ƿ�������*/
    if(!DIAG_IS_CONN_ON)
    {
        diag_crit("δ���ӹ������!\n");
    }
    else if(!DIAG_IS_LT_AIR_ON)
    {
        diag_crit("�տڿ���δ��!\n");
    }
    else
    {
        diag_crit("�տڿ����Ѵ�!\n");
    }
    diag_crit("�տ���Ϣ�ϱ�ʧ���ܴ��� %d.\n", g_astCBTInfoTbl[EN_DIAG_CBT_API_AIR_ERR].ulCalledNum);
    diag_crit("�տ���Ϣ�ϱ��ɹ��ܴ��� %d.\n", g_astCBTInfoTbl[EN_DIAG_CBT_API_AIR_OK].ulCalledNum);
}


/*****************************************************************************
 Function Name   : DIAG_ShowLayerCfg
 Description     : �������������PID��ѯ�����Ϣ���ÿ�����Ϣ
*****************************************************************************/
VOS_VOID DIAG_ShowLayerCfg(VOS_UINT32 ulModuleId, VOS_UINT32 ulSrcDst)
{
    VOS_UINT32 ulOffset = 0;
    VOS_UINT32 ulState = 0;

    /*���DIAG�Ƿ��ʼ����HSO�Ƿ�������*/
    if(!DIAG_IS_CONN_ON)
    {
        diag_crit("δ���ӹ������!\n");
    }

    /* 0��ʾԴģ�� */
    if(DIAG_CMD_LAYER_MOD_SRC == ulSrcDst)
    {
        if(DIAG_CFG_LAYER_MODULE_IS_ACORE(ulModuleId))
        {
            ulOffset = DIAG_CFG_LAYER_MODULE_ACORE_OFFSET(ulModuleId);

            if(g_ALayerSrcModuleCfg[ulOffset])
            {
                ulState = 1;
            }
        }
        else if(DIAG_CFG_LAYER_MODULE_IS_CCORE(ulModuleId))
        {
            ulOffset = DIAG_CFG_LAYER_MODULE_CCORE_OFFSET(ulModuleId);

            if(g_CLayerSrcModuleCfg[ulOffset])
            {
                ulState = 1;
            }
        }
        else
        {
            diag_crit("�Ƿ�PID: A(%d -- %d), C(%d -- %d)!\n",
                VOS_PID_CPU_ID_1_DOPRAEND, VOS_CPU_ID_1_PID_BUTT,
                VOS_PID_CPU_ID_0_DOPRAEND, VOS_CPU_ID_0_PID_BUTT);

            return ;
        }

        diag_crit("Դģ��%d����Ϊ %s .\n", ulModuleId, ulState ? "��״̬":"�ر�״̬");
    }
    else
    {
        if(DIAG_CFG_LAYER_MODULE_IS_ACORE(ulModuleId))
        {
            ulOffset = DIAG_CFG_LAYER_MODULE_ACORE_OFFSET(ulModuleId);

            if(g_ALayerDstModuleCfg[ulOffset])
            {
                ulState = 1;
            }
        }
        else if(DIAG_CFG_LAYER_MODULE_IS_CCORE(ulModuleId))
        {
            ulOffset = DIAG_CFG_LAYER_MODULE_CCORE_OFFSET(ulModuleId);

            if(g_CLayerDstModuleCfg[ulOffset])
            {
                ulState = 1;
            }
        }
        else
        {
            diag_crit("�Ƿ�PID: A(%d -- %d), C(%d -- %d)!\n",
                VOS_PID_CPU_ID_1_DOPRAEND, VOS_CPU_ID_1_PID_BUTT,
                VOS_PID_CPU_ID_0_DOPRAEND, VOS_CPU_ID_0_PID_BUTT);

            return ;
        }

        diag_crit("Ŀ��ģ��%d����Ϊ %s .\n", ulModuleId, ulState ? "��״̬":"�ر�״̬");
    }

    diag_crit("�����ϱ������Ϣ�ϱ�ʧ���ܴ��� %d.\n", g_astCBTInfoTbl[EN_DIAG_CBT_API_TRACE_ERR].ulCalledNum);
    diag_crit("�����ϱ������Ϣ�ϱ��ɹ��ܴ��� %d.\n", g_astCBTInfoTbl[EN_DIAG_CBT_API_TRACE_OK].ulCalledNum);

    diag_crit("vos���������Ϣ�ϱ�ʧ���ܴ��� %d.\n", g_astCBTInfoTbl[EN_DIAG_API_MSG_LAYER_ERR].ulCalledNum);
    diag_crit("vos���������Ϣ�ϱ��ɹ��ܴ��� %d.\n", g_astCBTInfoTbl[EN_DIAG_API_MSG_LAYER_OK].ulCalledNum);
    diag_crit("vos���������Ϣ���˴��� %d: srcid 0x%x, dstid 0x%x, msgid 0x%x.\n",
        g_astCBTInfoTbl[EN_DIAG_API_MSG_LAYER_MATCH].ulCalledNum,
        g_astCBTInfoTbl[EN_DIAG_API_MSG_LAYER_MATCH].ulRserved1,
        g_astCBTInfoTbl[EN_DIAG_API_MSG_LAYER_MATCH].ulRserved2,
        g_astCBTInfoTbl[EN_DIAG_API_MSG_LAYER_MATCH].ulRserved3);
}

/*****************************************************************************
 Function Name   : DIAG_ShowUsrCfg
 Description     : ��ѯ�û������ÿ�����Ϣ
*****************************************************************************/
VOS_VOID DIAG_ShowUsrCfg(VOS_VOID)
{
    diag_crit("�û�����Ϣ�ϱ�ʧ���ܴ��� %d.\n", g_astCBTInfoTbl[EN_DIAG_CBT_API_USER_ERR].ulCalledNum);
    diag_crit("�û�����Ϣ�ϱ��ɹ��ܴ��� %d.\n", g_astCBTInfoTbl[EN_DIAG_CBT_API_USER_OK].ulCalledNum);
}

/*****************************************************************************
 Function Name   : DIAG_ShowTrans
 Description     : ��ѯ���n��͸���ϱ������Ϣ
*****************************************************************************/
VOS_VOID DIAG_ShowTrans(VOS_UINT32 n)
{
    diag_crit("͸����Ϣ�ϱ�ʧ���ܴ��� %d.\n", g_astCBTInfoTbl[EN_DIAG_CBT_API_TRANS_ERR].ulCalledNum);
    diag_crit("͸����Ϣ�ϱ��ɹ��ܴ��� %d.\n", g_astCBTInfoTbl[EN_DIAG_CBT_API_TRANS_OK].ulCalledNum);

    diag_crit("��� %d ��͸���ϱ�����Ϣ����:\n", n);

    DIAG_ShowLNR(EN_DIAG_LNR_TRANS_IND, n);

}

/*****************************************************************************
 Function Name   : DIAG_ShowTrans
 Description     : ��ѯ���n��͸���ϱ������Ϣ
*****************************************************************************/
VOS_VOID DIAG_ShowPsTransCmd(VOS_UINT32 n)
{
    diag_crit("��� %d ��͸���ϱ�����Ϣ����:\n", n);
    DIAG_ShowLNR(EN_DIAG_LNR_PS_TRANS, n);
}

/*****************************************************************************
 Function Name   : DIAG_ShowLost
 Description     : ��ѯ�ϱ�ʧ�ܶ�������Ϣ
*****************************************************************************/
VOS_VOID DIAG_ShowLost(VOS_VOID)
{
    diag_crit("���������socp�ӿ��ϱ���ʧ�ܴ��� %d.\n", g_astCBTInfoTbl[EN_DIAG_CBT_API_PACKET_ERR_REQ].ulCalledNum);
    diag_crit("���һ�ε�ʧ�ܵĴ����� %d, �к� %d.\n",
        g_astCBTInfoTbl[EN_DIAG_CBT_API_PACKET_ERR_REQ].ulRserved1, g_astCBTInfoTbl[EN_DIAG_CBT_API_PACKET_ERR_REQ].ulRserved2);
}

extern HTIMER g_DebugTimer;

/*****************************************************************************
 Function Name   : diag_ReportMntn
 Description     : ͨ������ͨ����ʱ�ϱ���ά�ɲ���Ϣ
*****************************************************************************/
DIAG_MNTN_SRC_INFO_STRU g_ind_src_mntn_info = {};

void diag_reset_src_mntn_info(void)
{
    VOS_MemSet_s(&g_ind_src_mntn_info, sizeof(g_ind_src_mntn_info), 0, sizeof(g_ind_src_mntn_info));
}
void diag_debug_ind_src_lost(VOS_UINT32 type, VOS_UINT32 len)
{
    g_ind_src_mntn_info.ulDeltaLostTimes++;
    g_ind_src_mntn_info.ulDeltaLostLen += len;
    g_ind_src_mntn_info.aulCurFailNum[type]++;
    g_ind_src_mntn_info.aulCurFailLen[type] += len;
}


/*****************************************************************************
 Function Name   : diag_ReportMntn
 Description     : ͨ������ͨ����ʱ�ϱ���ά�ɲ���Ϣ
*****************************************************************************/
VOS_VOID diag_ReportSrcMntn(VOS_VOID)
{
    DIAG_SRV_HEADER_STRU        stSrvHeader;
    DIAG_DEBUG_SRC_MNTN_STRU    stDiagInfo = {};
    SOCP_BUFFER_RW_STRU         stSocpBuff = {VOS_NULL};
    DIAG_MSG_REPORT_HEAD_STRU   stDiagHead;
    VOS_UINT32          ulRet;
    static VOS_UINT32   last_slice = 0;
    VOS_UINT32          current_slice;
    VOS_ULONG           ulLockLevel;

    (VOS_VOID)mdrv_socp_get_write_buff(SCM_CODER_SRC_LOM_IND, &stSocpBuff);
    stDiagInfo.pstMntnInfo.ulLeftSize     = stSocpBuff.u32Size + stSocpBuff.u32RbSize;

    current_slice = mdrv_timer_get_normal_timestamp();
    stDiagInfo.pstMntnInfo.ulDeltaTime  = DIAG_SLICE_DELTA(last_slice, current_slice);

    stDiagInfo.pstMntnInfo.ulDeltaLostTimes = g_ind_src_mntn_info.ulDeltaLostTimes;
    stDiagInfo.pstMntnInfo.ulDeltaLostLen   = g_ind_src_mntn_info.ulDeltaLostLen;
    (VOS_VOID)VOS_MemCpy_s(stDiagInfo.pstMntnInfo.aulCurFailNum, sizeof(stDiagInfo.pstMntnInfo.aulCurFailNum),
                g_ind_src_mntn_info.aulCurFailNum, sizeof(g_ind_src_mntn_info.aulCurFailNum));
    (VOS_VOID)VOS_MemCpy_s(stDiagInfo.pstMntnInfo.aulCurFailLen, sizeof(stDiagInfo.pstMntnInfo.aulCurFailLen),
                g_ind_src_mntn_info.aulCurFailLen, sizeof(g_ind_src_mntn_info.aulCurFailLen));

    /*������Ϣ�ϱ�����*/
    stDiagInfo.pstMntnInfo.ulTraceNum       = g_astCBTInfoTbl[EN_DIAG_CBT_API_TRACE_OK].ulCalledNum;
    stDiagInfo.pstMntnInfo.ulEventNum       = g_astCBTInfoTbl[EN_DIAG_CBT_API_EVENT_OK].ulCalledNum;
    stDiagInfo.pstMntnInfo.ulLogNum         = g_astCBTInfoTbl[EN_DIAG_CBT_API_PRINTFV_OK].ulCalledNum;
    stDiagInfo.pstMntnInfo.ulAirNum         = g_astCBTInfoTbl[EN_DIAG_CBT_API_AIR_OK].ulCalledNum;
    stDiagInfo.pstMntnInfo.ulTransNum       = g_astCBTInfoTbl[EN_DIAG_CBT_API_TRANS_OK].ulCalledNum;

    /*����Դ������*/
    stDiagInfo.pstMntnInfo.ulThrputEnc      = mdrv_GetThrputInfo(EN_DIAG_THRPUT_DATA_CHN_ENC);
    /*��������*/
    stDiagInfo.pstMntnInfo.ulDeltaLostTimes = g_ind_src_mntn_info.ulDeltaLostTimes;

    stDiagInfo.pstMntnHead.ulModuleId       = DIAG_AGENT_PID;

    VOS_SpinLockIntLock(&g_DiagLogPktNum.ulTransLock, ulLockLevel);
    stDiagInfo.pstMntnHead.ulNo     = (g_DiagLogPktNum.ulTransNum)++;
    VOS_SpinUnlockIntUnlock(&g_DiagLogPktNum.ulTransLock, ulLockLevel);

    /* �������ͷ */
    diag_SvcFillHeader((DIAG_SRV_HEADER_STRU *)&stSrvHeader);
    DIAG_SRV_SET_MODEM_ID(&stSrvHeader.frame_header, DIAG_MODEM_0);
    DIAG_SRV_SET_TRANS_ID(&stSrvHeader.frame_header, g_ulTransId++);
    DIAG_SRV_SET_COMMAND_ID(&stSrvHeader.frame_header, DIAG_MSG_TYPE_MSP,\
                            DIAG_MODE_COMM, DIAG_MSG_STAT, DIAG_DEBUG_SRC_MNTN_CMDID);
    DIAG_SRV_SET_MSG_LEN(&stSrvHeader.frame_header, sizeof(stDiagInfo));

    stDiagHead.ulHeaderSize     = sizeof(stSrvHeader);
    stDiagHead.pHeaderData      = (VOS_VOID*)&stSrvHeader;

    stDiagHead.ulDataSize       = sizeof(stDiagInfo);
    stDiagHead.pData            = (VOS_VOID*)&stDiagInfo;

    ulRet = diag_ServicePackData(&stDiagHead);
    if(!ulRet)
    {
        /*���ͳɹ���������ؼ�¼*/
        VOS_MemSet_s(&g_ind_src_mntn_info, sizeof(g_ind_src_mntn_info), 0, sizeof(g_ind_src_mntn_info));
    }
    return ;
}


VOS_VOID diag_ReportDstMntn(VOS_VOID)
{
    DIAG_SRV_HEADER_STRU        stSrvHeader;
    DIAG_DEBUG_DST_LOST_STRU    stDiagInfo = {};
    SOCP_BUFFER_RW_STRU         stSocpBuff = {NULL};
    DIAG_MNTN_DST_INFO_STRU     stDstInfo;
    DIAG_MSG_REPORT_HEAD_STRU   stDiagHead;
    VOS_UINT32                  ulRet;
    VOS_UINT32                  current_slice = 0;
    static  VOS_UINT32          last_slice = 0;
    VOS_ULONG  ulLockLevel;

    mdrv_diag_get_dst_mntn_info(&stDstInfo);

    (VOS_VOID)VOS_MemCpy_s(&stDiagInfo.pstMntnInfo, sizeof(stDiagInfo.pstMntnInfo), &stDstInfo, sizeof(stDstInfo));

    stDiagInfo.pstMntnInfo.ulChannelId = SOCP_CODER_DST_OM_IND;
    (VOS_VOID)VOS_MemCpy_s(stDiagInfo.pstMntnInfo.chanName, sizeof(stDiagInfo.pstMntnInfo.chanName), "ind_dst", sizeof("ind_dst"));

    (void)DRV_SOCP_GET_READ_BUFF(SOCP_CODER_DST_OM_IND, &stSocpBuff);
    stDiagInfo.pstMntnInfo.ulUseSize = stSocpBuff.u32RbSize + stSocpBuff.u32Size;

    current_slice = mdrv_timer_get_normal_timestamp();
    stDiagInfo.pstMntnInfo.ulDeltaTime = DIAG_SLICE_DELTA(last_slice, current_slice);
    last_slice = current_slice;

    stDiagInfo.pstMntnInfo.ulThrputPhy      = mdrv_GetThrputInfo(EN_DIAG_THRPUT_DATA_CHN_PHY);
    stDiagInfo.pstMntnInfo.ulThrputCb       = mdrv_GetThrputInfo(EN_DIAG_THRPUT_DATA_CHN_CB);


    stDiagInfo.pstMntnHead.ulModuleId       = DIAG_AGENT_PID;

    VOS_SpinLockIntLock(&g_DiagLogPktNum.ulTransLock, ulLockLevel);
    stDiagInfo.pstMntnHead.ulNo     = (g_DiagLogPktNum.ulTransNum)++;
    VOS_SpinUnlockIntUnlock(&g_DiagLogPktNum.ulTransLock, ulLockLevel);

    /* �������ͷ */
    diag_SvcFillHeader((DIAG_SRV_HEADER_STRU *)&stSrvHeader);
    DIAG_SRV_SET_MODEM_ID(&stSrvHeader.frame_header, DIAG_MODEM_0);
    DIAG_SRV_SET_TRANS_ID(&stSrvHeader.frame_header, g_ulTransId++);
    DIAG_SRV_SET_COMMAND_ID(&stSrvHeader.frame_header, DIAG_MSG_TYPE_MSP,\
                            DIAG_MODE_COMM, DIAG_MSG_STAT, DIAG_DEBUG_DST_MNTN_CMDID);
    DIAG_SRV_SET_MSG_LEN(&stSrvHeader.frame_header, sizeof(stDiagInfo));

    stDiagHead.ulHeaderSize     = sizeof(stSrvHeader);
    stDiagHead.pHeaderData      = (VOS_VOID*)&stSrvHeader;

    stDiagHead.ulDataSize       = sizeof(stDiagInfo);
    stDiagHead.pData            = (VOS_VOID*)&stDiagInfo;

    ulRet = diag_ServicePackData(&stDiagHead);
    if(!ulRet)
    {
        mdrv_diag_reset_dst_mntn_info();
    }
    return;
}

VOS_VOID diag_ReportMntn(VOS_VOID)
{
    /* ����log */
    if(!DIAG_IS_POLOG_ON)
    {
        /* HIDSδ���� */
        if(!DIAG_IS_CONN_ON)
        {
            return;
        }
    }

    /*Դ��ά����Ϣ�ϱ�*/
    diag_ReportSrcMntn();

    /*Ŀ�Ķ�ά����Ϣ�ϱ�*/
    diag_ReportDstMntn();

    return;
}

VOS_VOID diag_StopMntnTimer(VOS_VOID)
{
    /* ɾ����ʱ�� */
    if(DIAG_CFG_SWT_CLOSE == g_DiagDebugCfg)
    {
        diag_crit("mntn is not active\n");
        return;
    }

    g_DiagDebugCfg = DIAG_CFG_SWT_CLOSE;

    (VOS_VOID)VOS_StopRelTimer(&g_DebugTimer);
}

VOS_VOID diag_StartMntnTimer(VOS_VOID)
{
    VOS_UINT32          ulCnfRst = 0;
    VOS_UINT32          ulCurretPort;
    VOS_UINT32          ulDiagTimeLength;
    DIAG_CHANNLE_PORT_CFG_STRU    stPortCfg = {};

    if(DIAG_CFG_SWT_OPEN == g_DiagDebugCfg)
    {
        diag_crit("mntn is actived\n");
        return;
    }

    g_DiagDebugCfg = DIAG_CFG_SWT_OPEN;

    /* ������ʱ���ϱ���ά�ɲ���Ϣ�����߶�λ�������� */
    /* ��ȡOM���������ͨ�� */
    ulCnfRst =  mdrv_nv_read(NV_ID_DRV_DIAG_PORT, &stPortCfg, (VOS_UINT32)sizeof(DIAG_CHANNLE_PORT_CFG_STRU));
    if(ulCnfRst != VOS_OK)
    {
        diag_error("get current port fail\n");
    }
    ulCurretPort = stPortCfg.enPortNum;

    if(CPM_OM_PORT_TYPE_VCOM == ulCurretPort)
    {
        ulDiagTimeLength = DIAG_HIDP_DEBUG_TIMER_LEN;
    }
    else
    {
        ulDiagTimeLength = DIAG_DEBUG_TIMER_LEN;
    }

    ulCnfRst = VOS_StartRelTimer(&g_DebugTimer, MSP_PID_DIAG_APP_AGENT, ulDiagTimeLength, DIAG_DEBUG_TIMER_NAME, \
                            DIAG_DEBUG_TIMER_PARA, VOS_RELTIMER_LOOP, VOS_TIMER_NO_PRECISION);
    if(ulCnfRst != ERR_MSP_SUCCESS)
    {
        diag_error("start dbug timer fail\n");
    }
}

/* EVENT�ϱ�����ӿ� */
VOS_VOID DIAG_DebugEventReport(VOS_UINT32 ulpid)
{
    DIAG_EVENT_IND_STRU stEvent = {0};
    VOS_UINT32 aulData[4] = {0x12345678, 0x34567890, 0xabcdef00, 0x12345678};

    stEvent.ulModule    = DIAG_GEN_MODULE(1, 2);
    stEvent.ulPid       = ulpid;
    stEvent.ulEventId   = 0x13579;
    stEvent.ulLength    = 16;
    stEvent.pData       = aulData;

    (VOS_VOID)DIAG_EventReport(&stEvent);
}


/* �����Ϣ�ϱ�����ӿ� */
VOS_VOID DIAG_DebugLayerReport(VOS_UINT32 ulsndpid, VOS_UINT32 ulrcvpid, VOS_UINT32 ulMsg)
{
    DIAG_DATA_MSG_STRU *pDataMsg;
    VOS_UINT32 aulData[4] = {0x12345678, 0x34567890, 0xabcdef00, 0x12345678};

    diag_crit("pid %d send to %d, msgid 0x%x.\n", ulsndpid, ulsndpid, ulMsg);

    pDataMsg = (DIAG_DATA_MSG_STRU*)VOS_AllocMsg(ulsndpid, (sizeof(DIAG_DATA_MSG_STRU)+ 16 - VOS_MSG_HEAD_LENGTH));

    if (pDataMsg != NULL)
    {
        pDataMsg->ulReceiverPid = ulrcvpid;
        pDataMsg->ulMsgId       = ulMsg;
        pDataMsg->ulLen         = 16;
        (VOS_VOID)VOS_MemCpy_s(pDataMsg->pContext,pDataMsg->ulLen,aulData,sizeof(aulData));

        DIAG_TraceReport(pDataMsg);

        (VOS_VOID)VOS_FreeMsg(ulsndpid, pDataMsg);
    }
}

/* �����Ϣ�ϱ�����ӿ� */
VOS_VOID DIAG_DebugVosLayerReport(VOS_UINT32 ulsndpid, VOS_UINT32 ulrcvpid, VOS_UINT32 ulMsg)
{
    DIAG_DATA_MSG_STRU *pDataMsg;
    VOS_UINT32 aulData[4] = {0x12345678, 0x34567890, 0xabcdef00, 0x12345678};

    diag_crit("pid %d send to %d, msgid 0x%x.\n", ulsndpid, ulsndpid, ulMsg);

    pDataMsg = (DIAG_DATA_MSG_STRU*)VOS_AllocMsg(ulsndpid, (sizeof(DIAG_DATA_MSG_STRU)+ 16 - VOS_MSG_HEAD_LENGTH));

    if (pDataMsg != NULL)
    {
        pDataMsg->ulReceiverPid = ulrcvpid;
        pDataMsg->ulMsgId       = ulMsg;
        pDataMsg->ulLen         = 16;
        (VOS_VOID)VOS_MemCpy_s(pDataMsg->pContext,pDataMsg->ulLen,aulData,sizeof(aulData));

        DIAG_LayerMsgReport(pDataMsg);

        (VOS_VOID)VOS_FreeMsg(ulsndpid, pDataMsg);
    }
}


/* log�ϱ�����ӿ� */
VOS_VOID DIAG_DebugLogReport(VOS_UINT32 ulpid, VOS_UINT32 level)
{
    VOS_UINT32 ulMod = DIAG_GEN_LOG_MODULE(1, 2, level);
    (VOS_VOID)DIAG_LogReport(ulMod, ulpid, __FILE__, __LINE__, "DIAG TEST.\n");
}


/* ͸���ϱ�����ӿ� */
VOS_VOID DIAG_DebugTransReport(VOS_UINT32 ulpid)
{
    DIAG_TRANS_IND_STRU std;
    VOS_UINT32 aulData[80] = {0x12345678, 0x34567890, 0xabcdef00, 0x12345678};

    std.ulModule    = DIAG_GEN_MODULE(1, 2);;
    std.ulPid       = ulpid;
    std.ulMsgId     = 0x9753;
    std.ulLength    = 80;
    std.pData       = aulData;

    (VOS_VOID)DIAG_TransReport(&std);
}


/* ��俪�ص���ӿ� */
VOS_VOID DIAG_DebugLayerCfg(VOS_UINT32 ulModuleId, VOS_UINT8 ucFlag)
{
    VOS_UINT32 ulOffset = 0;

    if(DIAG_CFG_LAYER_MODULE_IS_ACORE(ulModuleId))
    {
        ulOffset = DIAG_CFG_LAYER_MODULE_ACORE_OFFSET(ulModuleId);

        g_ALayerSrcModuleCfg[ulOffset] = ucFlag;
        g_ALayerDstModuleCfg[ulOffset] = ucFlag;
    }
    else if(DIAG_CFG_LAYER_MODULE_IS_CCORE(ulModuleId))
    {
        ulOffset = DIAG_CFG_LAYER_MODULE_CCORE_OFFSET(ulModuleId);

        g_CLayerSrcModuleCfg[ulOffset] = ucFlag;
        g_CLayerDstModuleCfg[ulOffset] = ucFlag;
    }
    else
    {
        diag_crit("�Ƿ�PID: A(%d -- %d), C(%d -- %d)!\n",
            VOS_PID_CPU_ID_1_DOPRAEND, VOS_CPU_ID_1_PID_BUTT,
            VOS_PID_CPU_ID_0_DOPRAEND, VOS_CPU_ID_0_PID_BUTT);

        return ;
    }

}

#define GEN_MODULE_ID(modem_id, modeid, level, pid)     ((modem_id<<24)|(modeid<<16)|(level << 12)|(pid))

VOS_UINT32 DIAG_ApiTest(VOS_UINT8* pstReq)
{
    DIAG_MSG_A_TRANS_C_STRU *pstInfo;
    DIAG_AIR_IND_STRU       airmsg;
    VOS_UINT32              data = 0x1234567;
    VOS_UINT32              ulModuleId = 0;
    VOS_UINT32              ulLen;
    DIAG_FRAME_INFO_STRU    *pstDiagHead = VOS_NULL;
    VOS_VOID                *ptr;

    diag_crit("DIAG_ApiTest start\n");

    ptr = VOS_MemAlloc(MSP_PID_DIAG_APP_AGENT, DYNAMIC_MEM_PT, 9*1024);
    if(!ptr)
    {
        diag_error("alloc fail\n");
    }

    (VOS_VOID)DIAG_DebugLogReport(MSP_PID_DIAG_APP_AGENT, 1);

    (VOS_VOID)DIAG_DebugTransReport(MSP_PID_DIAG_APP_AGENT);
    (VOS_VOID)DIAG_TransReport(VOS_NULL);

    (VOS_VOID)DIAG_DebugEventReport(MSP_PID_DIAG_APP_AGENT);
    (VOS_VOID)DIAG_EventReport(VOS_NULL);

    ulModuleId = GEN_MODULE_ID(0, 1, 1, MSP_PID_DIAG_APP_AGENT);
    airmsg.ulPid = MSP_PID_DIAG_APP_AGENT;
    airmsg.ulDirection = 0x3;
    airmsg.ulLength = sizeof(data);
    airmsg.ulModule = ulModuleId;
    airmsg.ulMsgId = 0x123;
    airmsg.pData = (VOS_VOID *)&data;
    (VOS_VOID)DIAG_AirMsgReport(&airmsg);

    if(ptr)
    {
        airmsg.ulPid = MSP_PID_DIAG_APP_AGENT;
        airmsg.ulDirection = 0x3;
        airmsg.ulLength = 9*1024;
        airmsg.ulModule = ulModuleId;
        airmsg.ulMsgId = 0x123;
        airmsg.pData = (VOS_VOID *)&data;
        (VOS_VOID)DIAG_AirMsgReport(&airmsg);
    }

    (VOS_VOID)DIAG_AirMsgReport(VOS_NULL);

    (VOS_VOID)DIAG_DebugLayerReport(DOPRA_PID_TIMER, MSP_PID_DIAG_APP_AGENT, 0x123);
    (VOS_VOID)DIAG_TraceReport(VOS_NULL);

    (VOS_VOID)DIAG_DebugVosLayerReport(DOPRA_PID_TIMER, MSP_PID_DIAG_APP_AGENT, 0x123);
    (VOS_VOID)DIAG_LayerMsgReport(VOS_NULL);

    (VOS_VOID)DIAG_ErrorLog(VOS_NULL, 0, 0, 0, VOS_NULL, 0);

    VOS_MemFree(MSP_PID_DIAG_APP_AGENT, ptr);

    /* �޸ĺ�ӿ�Ҳ֧���ڴ��ڵ��� */
    if(pstReq)
    {
        pstDiagHead = (DIAG_FRAME_INFO_STRU*)(pstReq);

        DIAG_MSG_ACORE_CFG_PROC(ulLen, pstDiagHead, pstInfo, data);
        DIAG_MSG_SEND_CFG_TO_NRM(ulLen, pstDiagHead, pstInfo, data);
    }

    diag_crit("DIAG_ApiTest end\n");

    return VOS_OK;
DIAG_ERROR:
    return 0;
}

EXPORT_SYMBOL(DIAG_ApiTest);


