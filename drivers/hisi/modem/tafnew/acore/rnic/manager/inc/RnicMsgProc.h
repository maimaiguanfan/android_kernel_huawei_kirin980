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

#ifndef __RNIC_MSGPROC_H__
#define __RNIC_MSGPROC_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "PsCommonDef.h"
#include "NetMgrCtrlInterface.h"
#include "RnicCtx.h"
#include "RnicIfaceCfg.h"
#include "RnicIntraMsg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

/*****************************************************************************
  2 �궨��
*****************************************************************************/

/* ��װOSA������Ϣ�ӿ� */
#define RNIC_ALLOC_MSG_WITH_HDR(msg_len)\
            PS_ALLOC_MSG_WITH_HEADER_LEN(ACPU_PID_RNIC, (msg_len))

/* ��װOSA��Ϣͷ */
#define RNIC_CFG_MSG_HDR(msg, rcv_pid, msg_id)\
            ((MSG_HEADER_STRU *)(msg))->ulSenderCpuId   = VOS_LOCAL_CPUID;\
            ((MSG_HEADER_STRU *)(msg))->ulSenderPid     = ACPU_PID_RNIC;\
            ((MSG_HEADER_STRU *)(msg))->ulReceiverCpuId = VOS_LOCAL_CPUID;\
            ((MSG_HEADER_STRU *)(msg))->ulReceiverPid   = (rcv_pid);\
            ((MSG_HEADER_STRU *)(msg))->ulMsgName       = (msg_id)

#define RNIC_CFG_INTRA_MSG_HDR(msg, msg_id)\
            RNIC_CFG_MSG_HDR(msg, ACPU_PID_RNIC, msg_id)

#define RNIC_CFG_AT_MSG_HDR(msg, msg_id)\
            RNIC_CFG_MSG_HDR(msg, WUEPS_PID_AT, msg_id)

/* ��ȡOSA��Ϣ���� */
#define RNIC_GET_MSG_ENTITY(msg)\
            ((VOS_VOID *)&(((MSG_HEADER_STRU *)(msg))->ulMsgName))

/* ��ȡOSA��Ϣ���� */
#define RNIC_GET_MSG_LENGTH(msg)\
            (((MSG_HEADER_STRU *)(msg))->ulLength)

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/


/*****************************************************************************
  4 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  6 ��Ϣ����
*****************************************************************************/

/* ��Ϣ��������ָ��*/
typedef VOS_UINT32 (*RNIC_PROC_MSG_FUNC)(MsgBlock *pstMsg);

/*****************************************************************************
  7 STRUCT����
*****************************************************************************/


typedef struct
{
    VOS_MSG_HEADER                                                              /* ��Ϣͷ */        /*_H2ASN_Skip*/
    RNIC_INTRA_MSG_ID_ENUM_UINT32       enMsgId;                                /*��Ϣ����*/        /*_H2ASN_Skip*/
    RNIC_DIAL_MODE_STRU                 stDialInfo;                             /*���貦����������*/
}RNIC_NOTIFY_MSG_STRU;



typedef struct
{
    VOS_MSG_HEADER                                                              /* ��Ϣͷ */    /* _H2ASN_Skip */
    RNIC_INTRA_MSG_ID_ENUM_UINT32       enMsgId;                                /* ��ϢID */    /* _H2ASN_Skip */
} RNIC_COMM_MSG_STRU;


typedef RNIC_COMM_MSG_STRU RNIC_DEV_READY_STRU;


typedef struct
{
    VOS_UINT32                          ulSenderPid;
    VOS_UINT32                          ulMsgId;
    RNIC_PROC_MSG_FUNC                  pRnicProcMsgFunc;
}RNIC_PROC_MSG_STRU;


typedef RNIC_COMM_MSG_STRU RNIC_CCPU_RESET_IND_STRU;


/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/
VOS_UINT32 RNIC_RcvAtDsflowInd(
    MsgBlock                           *pstMsg
);
VOS_UINT32  RNIC_RcvTiDsflowStatsExpired(
    MsgBlock                           *pstMsg
);
VOS_UINT32 RNIC_RcvTiDemandDialDisconnectExpired(
    MsgBlock                           *pstMsg
);

RNIC_TIMER_ID_ENUM_UINT16 RNIC_GetDsflowTimerIdByNetId(VOS_UINT8 ucRmNetId);

VOS_UINT8 RNIC_GetNetIdByTimerId(VOS_UINT32 ulMsgId);

VOS_UINT32 RNIC_RcvAtPdnInfoCfgInd(
    MsgBlock                           *pstMsg
);
VOS_UINT32 RNIC_RcvAtPdnInfoRelInd(
    MsgBlock                           *pstMsg
);

#if ((FEATURE_ON == FEATURE_IMS) && (FEATURE_ON == FEATURE_DELAY_MODEM_INIT))
VOS_UINT32 RNIC_RcvImsaPdnActInd(
    MsgBlock                           *pstMsg
);
VOS_UINT32 RNIC_RcvImsaPdnDeactInd(
    MsgBlock                           *pstMsg
);

VOS_UINT32 RNIC_RcvCdsImsDataInd(
    MsgBlock                           *pstMsg
);
VOS_UINT32 RNIC_ProcImsaPdnActInd_Wifi(
    IMSA_RNIC_PDN_INFO_CONFIG_STRU     *pstPdnInfo
);
VOS_UINT32 RNIC_ProcImsaPdnActInd_Lte(
    IMSA_RNIC_PDN_INFO_CONFIG_STRU     *pstPdnInfo
);
VOS_UINT32 RNIC_ProcImsaPdnDeactInd_Wifi(
    MODEM_ID_ENUM_UINT16                enModemId,
    IMSA_RNIC_PDN_EMC_IND_ENUM_UINT8    enEmcInd
);
VOS_UINT32 RNIC_ProcImsaPdnDeactInd_Lte(
    MODEM_ID_ENUM_UINT16                enModemId,
    IMSA_RNIC_PDN_EMC_IND_ENUM_UINT8    enEmcInd
);
VOS_UINT8 RNIC_GetImsRmnetId(
    IMSA_RNIC_IMS_RAT_TYPE_ENUM_UINT8   enRatType,
    MODEM_ID_ENUM_UINT16                enModemId,
    IMSA_RNIC_PDN_EMC_IND_ENUM_UINT8    enEmcInd
);

VOS_UINT8 RNIC_GetImsEmcBearRmnetId(
    IMSA_RNIC_IMS_RAT_TYPE_ENUM_UINT8   enRatType,
    MODEM_ID_ENUM_UINT16                enModemId
);

VOS_UINT8 RNIC_GetImsNormalBearRmnetId(
    IMSA_RNIC_IMS_RAT_TYPE_ENUM_UINT8   enRatType,
    MODEM_ID_ENUM_UINT16                enModemId
);

VOS_UINT32 RNIC_RcvImsaReservedPortsCfgInd(
    MsgBlock                           *pstMsg
);

VOS_UINT32 RNIC_RcvImsaSocketExceptionInd(
    MsgBlock                           *pstMsg
);

VOS_UINT32 RNIC_RcvImsaSipPortRangeInd(
    MsgBlock                           *pstMsg
);
#endif

VOS_UINT32 RNIC_RcvAtUsbTetherInfoInd(
    MsgBlock                           *pstMsg
);

VOS_UINT32 RNIC_RcvAtDialModeReq(
    MsgBlock                           *pstMsg
);
VOS_UINT32 RNIC_RcvCcpuResetStartInd(
    MsgBlock                           *pstMsg
);
VOS_UINT32 RNIC_RcvCcpuResetEndInd(
    MsgBlock                           *pstMsg
);
VOS_UINT32 RNIC_RcvNetdevReadyInd(
    MsgBlock                           *pstMsg
);
VOS_UINT32 RNIC_RcvImsaPdnModifyInd(
    MsgBlock                           *pstMsg
);
VOS_UINT32 RNIC_ProcImsData(
    MsgBlock                           *pMsg
);
VOS_UINT32 RNIC_SendDialInfoMsg(
    RNIC_INTRA_MSG_ID_ENUM_UINT32       enMsgId
);

VOS_VOID RNIC_FillNetManagerMsgPdnCfgInfo(
    NM_PDN_INFO_CONFIG_STRU            *pDestPdnInfo,
    IMSA_RNIC_PDN_INFO_CONFIG_STRU     *pSrcPdnInfo
);
VOS_VOID RNIC_SndNetManagerPdpActInd(
    MsgBlock                           *pstMsg
);
VOS_VOID RNIC_SndNetManagerPdpDeactInd(
    MsgBlock                           *pstMsg
);
VOS_VOID RNIC_SndNetManagerPdpModifyInd(
    MsgBlock                           *pstMsg
);
VOS_VOID RNIC_SndNetManagerModemResetInd(VOS_VOID);
VOS_VOID RNIC_SndNetManagerReservedPortCfgInd(
    MsgBlock                           *pstMsg
);

VOS_VOID RNIC_SndNetManagerSocketExceptInd(
    MsgBlock                           *pstMsg
);

VOS_VOID RNIC_SndNetManagerSipPortRangeInd(
    MsgBlock                           *pstMsg
);

#if (VOS_OS_VER == VOS_WIN32)
#pragma pack()
#else
#pragma pack(0)
#endif


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of RnicMsgProc.h */
