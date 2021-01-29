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
  1 ͷ�ļ�����
*****************************************************************************/
#include "RnicCtx.h"
#include "RnicMsgProc.h"
#include "AtRnicInterface.h"
#include "v_typdef.h"
#include "RnicTimerMgmt.h"
#include "mdrv.h"
#include "RnicEntity.h"
#include "RnicLog.h"
#include "PsTypeDef.h"
#include "PsCommonDef.h"
#include "AcpuReset.h"
#include "ImsaRnicInterface.h"
#include "RnicCdsInterface.h"
#include "NetMgrCtrlVcom.h"
#include "RnicIfaceCfg.h"
#include "RnicIfaceOndemand.h"
#include "RnicMntn.h"
#include "RnicIfaceCfg.h"
#include "rnic_dev_def.h"


/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_RNIC_PROCMSG_C

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
const RNIC_PROC_MSG_STRU g_astRnicMsgProcTab[] =
{
    /* ������ϢPID */  /* ��ϢID */                          /* ��Ϣ������ */
    /*****************************    AT-> RNIC Begin  *******************************/
    {WUEPS_PID_AT,     ID_AT_RNIC_DSFLOW_IND,                RNIC_RcvAtDsflowInd},
    {WUEPS_PID_AT,     ID_AT_RNIC_DIAL_MODE_REQ,             RNIC_RcvAtDialModeReq},
    {WUEPS_PID_AT,     ID_AT_RNIC_PDN_INFO_CFG_IND,          RNIC_RcvAtPdnInfoCfgInd},
    {WUEPS_PID_AT,     ID_AT_RNIC_PDN_INFO_REL_IND,          RNIC_RcvAtPdnInfoRelInd},
    {WUEPS_PID_AT,     ID_AT_RNIC_USB_TETHER_INFO_IND,       RNIC_RcvAtUsbTetherInfoInd},
    /*****************************    AT-> RNIC End    *******************************/

    /*****************************    TIMER -> RNIC Begin  *******************************/
    {VOS_PID_TIMER,    TI_RNIC_DSFLOW_STATS_0,               RNIC_RcvTiDsflowStatsExpired},
    {VOS_PID_TIMER,    TI_RNIC_DSFLOW_STATS_1,               RNIC_RcvTiDsflowStatsExpired},
    {VOS_PID_TIMER,    TI_RNIC_DSFLOW_STATS_2,               RNIC_RcvTiDsflowStatsExpired},
    {VOS_PID_TIMER,    TI_RNIC_DSFLOW_STATS_3,               RNIC_RcvTiDsflowStatsExpired},
    {VOS_PID_TIMER,    TI_RNIC_DSFLOW_STATS_4,               RNIC_RcvTiDsflowStatsExpired},
    {VOS_PID_TIMER,    TI_RNIC_DSFLOW_STATS_5,               RNIC_RcvTiDsflowStatsExpired},
    {VOS_PID_TIMER,    TI_RNIC_DSFLOW_STATS_6,               RNIC_RcvTiDsflowStatsExpired},
    {VOS_PID_TIMER,    TI_RNIC_DEMAND_DIAL_DISCONNECT,       RNIC_RcvTiDemandDialDisconnectExpired},
    /*****************************    TIMER -> RNIC End  *******************************/

    /*****************************    RNIC -> RNIC Begin  *******************************/
    {ACPU_PID_RNIC,    ID_RNIC_CCPU_RESET_START_IND,         RNIC_RcvCcpuResetStartInd},
    {ACPU_PID_RNIC,    ID_RNIC_CCPU_RESET_END_IND,           RNIC_RcvCcpuResetEndInd},
    {ACPU_PID_RNIC,    ID_RNIC_NETDEV_READY_IND,             RNIC_RcvNetdevReadyInd},
    {ACPU_PID_RNIC,    ID_RNIC_IMS_DATA_PROC_IND,            RNIC_ProcImsData},
    /*****************************    RNIC -> RNIC End    *******************************/

    /*****************************    IMSA -> RNIC Begin  *******************************/
    {PS_PID_IMSA,      ID_IMSA_RNIC_PDN_ACT_IND,             RNIC_RcvImsaPdnActInd},
    {PS_PID_IMSA,      ID_IMSA_RNIC_PDN_DEACT_IND,           RNIC_RcvImsaPdnDeactInd},
    {PS_PID_IMSA,      ID_IMSA_RNIC_PDN_MODIFY_IND,          RNIC_RcvImsaPdnModifyInd},
    {PS_PID_IMSA,      ID_IMSA_RNIC_RESERVED_PORTS_CONFIG_IND,   RNIC_RcvImsaReservedPortsCfgInd},
    {PS_PID_IMSA,      ID_IMSA_RNIC_SOCKET_EXCEPTION_IND,    RNIC_RcvImsaSocketExceptionInd},
    {PS_PID_IMSA,      ID_IMSA_RNIC_SIP_PORTS_RANGE_IND,     RNIC_RcvImsaSipPortRangeInd},
    /*****************************    IMSA -> RNIC End  *******************************/

    /*****************************    CDS -> RNIC Begin *******************************/
    {UEPS_PID_CDS,     ID_CDS_RNIC_IMS_DATA_IND,             RNIC_RcvCdsImsDataInd},
    /*****************************    CDS -> RNIC End   *******************************/
};

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


RNIC_TIMER_ID_ENUM_UINT16 RNIC_GetDsflowTimerIdByNetId(VOS_UINT8 ucRmNetId)
{
    RNIC_TIMER_ID_ENUM_UINT16           enTimerId;

    switch (ucRmNetId)
    {
        case RNIC_DEV_ID_RMNET0 :
            enTimerId = TI_RNIC_DSFLOW_STATS_0;
            break;

        case RNIC_DEV_ID_RMNET1 :
            enTimerId = TI_RNIC_DSFLOW_STATS_1;
            break;

        case RNIC_DEV_ID_RMNET2 :
            enTimerId = TI_RNIC_DSFLOW_STATS_2;
            break;

        case RNIC_DEV_ID_RMNET3 :
            enTimerId = TI_RNIC_DSFLOW_STATS_3;
            break;

        case RNIC_DEV_ID_RMNET4 :
            enTimerId = TI_RNIC_DSFLOW_STATS_4;
            break;

        case RNIC_DEV_ID_RMNET5 :
            enTimerId = TI_RNIC_DSFLOW_STATS_5;
            break;

        case RNIC_DEV_ID_RMNET6 :
            enTimerId = TI_RNIC_DSFLOW_STATS_6;
            break;

        default :
            enTimerId = TI_RNIC_TIMER_BUTT;
            break;
    }

    return enTimerId;
}


VOS_UINT8 RNIC_GetNetIdByTimerId(VOS_UINT32 ulMsgId)
{
    VOS_UINT8                           ucRmNedId;

    switch (ulMsgId)
    {
        case TI_RNIC_DSFLOW_STATS_0 :
            ucRmNedId = RNIC_DEV_ID_RMNET0;
            break;

        case TI_RNIC_DSFLOW_STATS_1 :
            ucRmNedId = RNIC_DEV_ID_RMNET1;
            break;

        case TI_RNIC_DSFLOW_STATS_2 :
            ucRmNedId = RNIC_DEV_ID_RMNET2;
            break;

        case TI_RNIC_DSFLOW_STATS_3 :
            ucRmNedId = RNIC_DEV_ID_RMNET3;
            break;

        case TI_RNIC_DSFLOW_STATS_4 :
            ucRmNedId = RNIC_DEV_ID_RMNET4;
            break;

        case TI_RNIC_DSFLOW_STATS_5 :
            ucRmNedId = RNIC_DEV_ID_RMNET5;
            break;

        case TI_RNIC_DSFLOW_STATS_6 :
            ucRmNedId = RNIC_DEV_ID_RMNET6;
            break;

        case TI_RNIC_DEMAND_DIAL_DISCONNECT :
        case TI_RNIC_DEMAND_DIAL_PROTECT :
            ucRmNedId = RNIC_DEV_ID_RMNET0;
            break;

        default :
            ucRmNedId = RNIC_DEV_ID_BUTT;
            break;
    }

    return ucRmNedId;
}


VOS_UINT32 RNIC_RcvAtDialModeReq(
    MsgBlock                           *pstMsg
)
{
    AT_RNIC_DIAL_MODE_REQ_STRU         *pstRcvInd;
    RNIC_AT_DIAL_MODE_CNF_STRU         *pstSndMsg;

    /* �ڴ���� */
    pstSndMsg = (RNIC_AT_DIAL_MODE_CNF_STRU *)PS_ALLOC_MSG(ACPU_PID_RNIC,
                        sizeof(RNIC_AT_DIAL_MODE_CNF_STRU) - VOS_MSG_HEAD_LENGTH);
    if (VOS_NULL_PTR == pstSndMsg)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_RcvAtDialModeReq: Malloc failed!");
        return VOS_ERR;
    }

    pstRcvInd                           = (AT_RNIC_DIAL_MODE_REQ_STRU *)pstMsg;

    pstSndMsg->usClientId               = pstRcvInd->usClientId;
    pstSndMsg->ulDialMode               = RNIC_GET_PROC_DIAL_MODE();
    pstSndMsg->ulIdleTime               = RNIC_GET_PROC_IDLE_TIME();
    pstSndMsg->ulEventReportFlag        = RNIC_GET_PROC_EVENT_REPORT();

    /* ͨ��ID_RNIC_AT_DIAL_MODE_CNF��Ϣ���͸�ATģ�� */
    /* �����Ϣ */
    RNIC_CFG_AT_MSG_HDR(pstSndMsg, ID_RNIC_AT_DIAL_MODE_CNF);

    /* ������Ϣ */
    if (VOS_OK != PS_SEND_MSG(ACPU_PID_RNIC, pstSndMsg))
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_RcvAtDialModeReq: Send msg failed!");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 RNIC_RcvAtDsflowInd(
    MsgBlock                           *pstMsg
)
{
    AT_RNIC_DSFLOW_IND_STRU            *pstRcvInd;
    RNIC_PS_IFACE_INFO_STRU            *pstPsIfaceInfo;
    RNIC_AT_DSFLOW_RSP_STRU            *pstDsflowRsp;

    /* �ڴ���� */
    pstDsflowRsp = (RNIC_AT_DSFLOW_RSP_STRU *)PS_ALLOC_MSG(ACPU_PID_RNIC,
                        sizeof(RNIC_AT_DSFLOW_RSP_STRU) - VOS_MSG_HEAD_LENGTH);
    if (VOS_NULL_PTR == pstDsflowRsp)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_RcvAtDsflowInd: Malloc failed!");
        return VOS_ERR;
    }

    pstRcvInd                               = (AT_RNIC_DSFLOW_IND_STRU *)pstMsg;
    pstDsflowRsp->usClientId                = pstRcvInd->usClientId;

    pstPsIfaceInfo                          = RNIC_GET_IFACE_PDN_INFO_ADR(pstRcvInd->enRnicRmNetId);

    /* ��ƷҪ��δ���Ϻţ�����Ϊ0 */
    pstDsflowRsp->stRnicDataRate.ulDLDataRate = 0;
    pstDsflowRsp->stRnicDataRate.ulULDataRate = 0;

    /* PDP�����ʱ�򣬻�ȡ��ǰ������������ */
    if ((RNIC_BIT_OPT_TRUE == pstPsIfaceInfo->bitOpIpv4Act)
     || (RNIC_BIT_OPT_TRUE == pstPsIfaceInfo->bitOpIpv6Act))
    {
        pstDsflowRsp->stRnicDataRate.ulDLDataRate = RNIC_GET_IFACE_CUR_RECV_RATE(pstRcvInd->enRnicRmNetId);
        pstDsflowRsp->stRnicDataRate.ulULDataRate = RNIC_GET_IFACE_CUR_SEND_RATE(pstRcvInd->enRnicRmNetId);
    }

    /* ͨ��ID_RNIC_AT_DSFLOW_RSP��Ϣ���͸�ATģ�� */
    /* �����Ϣ */
    RNIC_CFG_AT_MSG_HDR(pstDsflowRsp, ID_RNIC_AT_DSFLOW_RSP);

    /* ������Ϣ */
    if (VOS_OK != PS_SEND_MSG(ACPU_PID_RNIC, pstDsflowRsp))
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_RcvAtDsflowInd: Send msg failed!");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 RNIC_RcvAtPdnInfoCfgInd(
    MsgBlock                           *pstMsg
)
{
    AT_RNIC_PDN_INFO_CFG_IND_STRU      *pstRnicPdnCfgInd = VOS_NULL_PTR;
    RNIC_PS_IFACE_INFO_STRU            *pstPsIfaceInfo   = VOS_NULL_PTR;
    RNIC_IFACE_CTX_STRU                *pstRnicIfaceCtx  = VOS_NULL_PTR;
    VOS_UINT8                           ucExRabId        = RNIC_RAB_ID_INVALID;
    VOS_UINT8                           ucIpFamilyMask   = 0;

    pstRnicPdnCfgInd = (AT_RNIC_PDN_INFO_CFG_IND_STRU *)pstMsg;

    /* ���MODEMID */
    if (!RNIC_MODEM_ID_IS_VALID(pstRnicPdnCfgInd->enModemId))
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_RcvAtPdnInfoCfgInd: ModemId is invalid.");
        return VOS_ERR;
    }

    /* ���RABID */
    if (!RNIC_RAB_ID_IS_VALID(pstRnicPdnCfgInd->ucRabId))
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_RcvAtPdnInfoCfgInd: RabId is invalid.");
        return VOS_ERR;
    }

    /* ���RmnetID */
    if (!RNIC_RMNET_IS_VALID(pstRnicPdnCfgInd->ucRmNetId))
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_RcvAtPdnInfoCfgInd: RmNetId is invalid.");
        return VOS_ERR;
    }

    /* ���IP type */
    if ( (VOS_FALSE == pstRnicPdnCfgInd->bitOpIpv4PdnInfo)
      && (VOS_FALSE == pstRnicPdnCfgInd->bitOpIpv6PdnInfo) )
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_RcvAtPdnInfoCfgInd: IP type is invalid.");
        return VOS_ERR;
    }

    pstRnicIfaceCtx            = RNIC_GET_IFACE_CTX_ADR(pstRnicPdnCfgInd->ucRmNetId);
    pstRnicIfaceCtx->enIfaceId = pstRnicPdnCfgInd->ucIfaceId;

    pstPsIfaceInfo = RNIC_GET_IFACE_PDN_INFO_ADR(pstRnicPdnCfgInd->ucRmNetId);
    ucExRabId      = RNIC_BUILD_EXRABID(pstRnicPdnCfgInd->enModemId,pstRnicPdnCfgInd->ucRabId);

    /* ����PDP������ */
    pstPsIfaceInfo->enModemId    = pstRnicPdnCfgInd->enModemId;
    pstPsIfaceInfo->enRatType    = RNIC_PS_RAT_TYPE_3GPP;

    if (VOS_TRUE == pstRnicPdnCfgInd->bitOpIpv4PdnInfo)
    {
        pstPsIfaceInfo->bitOpIpv4Act    = RNIC_BIT_OPT_TRUE;
        pstPsIfaceInfo->ucIpv4RabId     = pstRnicPdnCfgInd->ucRabId;
        pstPsIfaceInfo->ucIpv4ExRabId   = ucExRabId;
        pstPsIfaceInfo->ulIpv4Addr      = pstRnicPdnCfgInd->ulIpv4Addr;
        ucIpFamilyMask = RNIC_BIT8_SET(ucIpFamilyMask, RNIC_IPV4_ADDR);
    }

    if (VOS_TRUE == pstRnicPdnCfgInd->bitOpIpv6PdnInfo)
    {
        pstPsIfaceInfo->bitOpIpv6Act    = RNIC_BIT_OPT_TRUE;
        pstPsIfaceInfo->ucIpv6RabId     = pstRnicPdnCfgInd->ucRabId;
        pstPsIfaceInfo->ucIpv6ExRabId   = ucExRabId;

        TAF_MEM_CPY_S(pstPsIfaceInfo->aucIpv6Addr,
                      sizeof(pstPsIfaceInfo->aucIpv6Addr),
                      pstRnicPdnCfgInd->aucIpv6Addr,
                      RNICITF_MAX_IPV6_ADDR_LEN);
        ucIpFamilyMask = RNIC_BIT8_SET(ucIpFamilyMask, RNIC_IPV6_ADDR);
    }

    RNIC_IFACE_PsIfaceUp(pstRnicPdnCfgInd->ucRmNetId, ucIpFamilyMask);

    return VOS_OK;
}


VOS_UINT32 RNIC_RcvAtPdnInfoRelInd(
    MsgBlock                           *pstMsg
)
{
    AT_RNIC_PDN_INFO_REL_IND_STRU      *pstRnicPdnRelInd = VOS_NULL_PTR;
    RNIC_PS_IFACE_INFO_STRU            *pstPsIfaceInfo   = VOS_NULL_PTR;
    VOS_UINT8                           ucIpFamilyMask   = 0;

    pstRnicPdnRelInd = (AT_RNIC_PDN_INFO_REL_IND_STRU *)pstMsg;

    /* ���RmnetID */
    if (!RNIC_RMNET_IS_VALID(pstRnicPdnRelInd->ucRmNetId))
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_RcvAtPdnInfoRelInd: RmNetId is invalid.");
        return VOS_ERR;
    }

    /* ��ȡPDP�����ĵ�ַ */
    pstPsIfaceInfo = RNIC_GET_IFACE_PDN_INFO_ADR(pstRnicPdnRelInd->ucRmNetId);

    /* ����PDP��������Ϣ */
    if (VOS_TRUE == pstRnicPdnRelInd->bitOpIpv4PdnInfo)
    {
        /* ���IPV4 PDP��������Ϣ */
        RNIC_InitIpv4PdnInfo(pstPsIfaceInfo);
        ucIpFamilyMask = RNIC_BIT8_SET(ucIpFamilyMask, RNIC_IPV4_ADDR);
    }

    if (VOS_TRUE == pstRnicPdnRelInd->bitOpIpv6PdnInfo)
    {
        /* ���IPV6 PDP��������Ϣ */
        RNIC_InitIpv6PdnInfo(pstPsIfaceInfo);
        ucIpFamilyMask = RNIC_BIT8_SET(ucIpFamilyMask, RNIC_IPV6_ADDR);
    }

    /* PDP��δ�����ʱ�� */
    if ( (RNIC_BIT_OPT_TRUE != pstPsIfaceInfo->bitOpIpv4Act)
      && (RNIC_BIT_OPT_TRUE != pstPsIfaceInfo->bitOpIpv6Act) )
    {
        pstPsIfaceInfo->enModemId = MODEM_ID_BUTT;
        pstPsIfaceInfo->enRatType = RNIC_PS_RAT_TYPE_BUTT;
    }

    RNIC_IFACE_PsIfaceDown(pstRnicPdnRelInd->ucRmNetId, ucIpFamilyMask);

    return VOS_OK;
}


VOS_UINT32 RNIC_RcvAtUsbTetherInfoInd(
    MsgBlock                           *pstMsg
)
{
    AT_RNIC_USB_TETHER_INFO_IND_STRU   *pstRnicUsbTetherInd;
    RNIC_PS_IFACE_TETHER_INFO_STRU      stTetherInfo;

    pstRnicUsbTetherInd = (AT_RNIC_USB_TETHER_INFO_IND_STRU *)pstMsg;

    stTetherInfo.enTetherConnStat = pstRnicUsbTetherInd->enTetherConnStat;
    TAF_MEM_CPY_S(stTetherInfo.aucRmnetName,
                  sizeof(stTetherInfo.aucRmnetName),
                  pstRnicUsbTetherInd->aucRmnetName,
                  sizeof(pstRnicUsbTetherInd->aucRmnetName));

    /* �������ò��ֽӿ� */
    RNIC_IFACE_TetherInfo(&stTetherInfo);

    return VOS_OK;
}


VOS_UINT32  RNIC_RcvTiDsflowStatsExpired(
    MsgBlock                           *pstMsg
)
{
    REL_TIMER_MSG                      *pstRcvMsg   = VOS_NULL_PTR;
    RNIC_TIMER_ID_ENUM_UINT16           enTimerId;
    VOS_UINT8                           ucRmNetId;

    pstRcvMsg = (REL_TIMER_MSG *)pstMsg;

    /* ���ݶ�ʱ��ID��ȡ����ID */
    ucRmNetId = RNIC_GetNetIdByTimerId(pstRcvMsg->ulName);

    /* ������Ϣ���ݻ�ö�ʱ��ID */
    enTimerId   = (VOS_UINT16)pstRcvMsg->ulName;

    RNIC_IFACE_SetDsFlowStats(ucRmNetId);

    RNIC_IFACE_AdjustNapiWeight(ucRmNetId);
    RNIC_CLEAN_IFACE_PERIOD_RECV_PKT(ucRmNetId);

    /* ÿ������ͳ�����ڽ�������Ҫ������ͳ��Byte����� */
    RNIC_IFACE_ClearDsFlowFlux(ucRmNetId);

    RNIC_StartTimer(enTimerId, TI_RNIC_DSFLOW_STATS_LEN);

    return VOS_OK;
}


VOS_UINT32 RNIC_RcvTiDemandDialDisconnectExpired(
    MsgBlock                           *pstMsg
)
{
    REL_TIMER_MSG                      *pstRcvMsg   = VOS_NULL_PTR;
    VOS_UINT8                           ucRmNetId;

    pstRcvMsg = (REL_TIMER_MSG *)pstMsg;

    /* ���ݶ�ʱ��ID��ȡ����ID */
    ucRmNetId = RNIC_GetNetIdByTimerId(pstRcvMsg->ulName);


    if (VOS_OK == RNIC_IFACE_OndemandDisconnTimeoutProc(ucRmNetId))
    {
        RNIC_StartTimer(TI_RNIC_DEMAND_DIAL_DISCONNECT, TI_RNIC_DEMAND_DIAL_DISCONNECT_LEN);
    }

    /* ���з��������ڰ���Ͽ�������Ͽ���ʱ����ʱ���� */
    RNIC_CLEAN_IFACE_PERIOD_SEND_PKT(ucRmNetId);

    return VOS_OK;
}



VOS_UINT32 RNIC_RcvCcpuResetStartInd(
    MsgBlock                           *pstMsg
)
{
    VOS_UINT8                           ucIndex;
    RNIC_CTX_STRU                      *pstRnicCtx;

    pstRnicCtx = RNIC_GET_RNIC_CTX_ADR();

    RNIC_PR_LOGI("enter, %u", VOS_GetSlice());

    /* ֹͣ���������Ķ�ʱ�� */
    RNIC_StopAllTimer();

    for (ucIndex = 0 ; ucIndex < RNIC_NET_ID_MAX_NUM ; ucIndex++)
    {
        /* ��λ���� */
        RNIC_IFACE_ResetPsIface(ucIndex);

        /* �������ͳ����Ϣ */
        RNIC_IFACE_ClearDsFlowStats(ucIndex);

        /* ��ʼ��RNIC PDN��Ϣ */
        RNIC_InitPdnInfo(&(pstRnicCtx->astIfaceCtx[ucIndex].stPsIfaceInfo));
    }

    /* ��ʼ��RNIC��ʱ�������� */
    RNIC_InitAllTimers();


    /* ��ʼ�����ŶϿ���ʱ����ʱ��������ͳ�� */
    RNIC_CLEAR_TI_DIALDOWN_EXP_CONT();

    /* �ͷ��ź�����ʹ�õ���API����������� */
    VOS_SmV(RNIC_GET_RESET_SEM());

    RNIC_PR_LOGI("leave, %u", VOS_GetSlice());

    RNIC_SndNetManagerModemResetInd();

    return VOS_OK;
}

VOS_UINT32 RNIC_RcvCcpuResetEndInd(
    MsgBlock                           *pstMsg
)
{
    RNIC_NORMAL_LOG(ACPU_PID_RNIC, "RNIC_RcvCcpuResetEndInd: rcv ID_CCPU_RNIC_RESET_END_IND");
    return VOS_OK;
}


VOS_UINT32 RNIC_RcvNetdevReadyInd(
    MsgBlock                           *pstMsg
)
{
    /* ����RNIC�������� */
    RNIC_IFACE_SetFeatureCfg();

    /* ����SPE���� */


    return VOS_OK;
}


VOS_UINT32 RNIC_ProcImsaPdnActInd_Wifi(
    IMSA_RNIC_PDN_INFO_CONFIG_STRU     *pstPdnInfo
)
{
    RNIC_PS_IFACE_INFO_STRU            *pstPsIfaceInfo = VOS_NULL_PTR;
    VOS_UINT8                           ucIpFamilyMask = 0;
    VOS_UINT8                           ucRmNetId;

    /* ���IP type */
    if ( (VOS_FALSE == pstPdnInfo->bitOpIpv4PdnInfo)
      && (VOS_FALSE == pstPdnInfo->bitOpIpv6PdnInfo) )
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_ProcImsaPdnActInd_Wifi: IP type is invalid.");
        return VOS_ERR;
    }

    /* ָ��һ��ר�ŵ���������VT��Ƶ���ݴ��� */
    ucRmNetId = RNIC_GetImsRmnetId(IMSA_RNIC_IMS_RAT_TYPE_WIFI, pstPdnInfo->enModemId, pstPdnInfo->enEmcInd);
    if (RNIC_DEV_ID_BUTT == ucRmNetId)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_ProcImsaPdnActInd_Wifi: ucRmNetId butt.");
        return VOS_ERR;
    }

    pstPsIfaceInfo = RNIC_GET_IFACE_PDN_INFO_ADR(ucRmNetId);

    /* ����PDP��������Ϣ */
    pstPsIfaceInfo->enRatType = RNIC_PS_RAT_TYPE_IWLAN;
    pstPsIfaceInfo->enModemId = pstPdnInfo->enModemId;

    /* IPV4���� */
    if (VOS_TRUE == pstPdnInfo->bitOpIpv4PdnInfo)
    {
        pstPsIfaceInfo->bitOpIpv4Act = RNIC_BIT_OPT_TRUE;
        ucIpFamilyMask = RNIC_BIT8_SET(ucIpFamilyMask, RNIC_IPV4_ADDR);
    }

    /* IPV6���� */
    if (VOS_TRUE == pstPdnInfo->bitOpIpv6PdnInfo)
    {
        pstPsIfaceInfo->bitOpIpv6Act = RNIC_BIT_OPT_TRUE;
        ucIpFamilyMask = RNIC_BIT8_SET(ucIpFamilyMask, RNIC_IPV6_ADDR);
    }

    RNIC_IFACE_PsIfaceUp(ucRmNetId, ucIpFamilyMask);

    return VOS_OK;
}


VOS_UINT32 RNIC_ProcImsaPdnActInd_Lte(
    IMSA_RNIC_PDN_INFO_CONFIG_STRU     *pstPdnInfo
)
{
    RNIC_PS_IFACE_INFO_STRU            *pstPsIfaceInfo = VOS_NULL_PTR;
    VOS_UINT8                           ucIpFamilyMask = 0;
    VOS_UINT8                           ucRmNetId;
    VOS_UINT8                           ucExRabId;

    /* ���IP type */
    if ( (VOS_FALSE == pstPdnInfo->bitOpIpv4PdnInfo)
      && (VOS_FALSE == pstPdnInfo->bitOpIpv6PdnInfo) )
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_ProcImsaPdnActInd_Lte: IP type is invalid.");
        return VOS_ERR;
    }

    /* ָ��һ��ר�ŵ���������VT��Ƶ���ݴ��� */
    ucRmNetId = RNIC_GetImsRmnetId(IMSA_RNIC_IMS_RAT_TYPE_LTE, pstPdnInfo->enModemId, pstPdnInfo->enEmcInd);
    if (RNIC_DEV_ID_BUTT == ucRmNetId)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_ProcImsaPdnActInd_Lte: ucRmNetId butt.");
        return VOS_ERR;
    }

    /* ���RABID */
    if (!RNIC_RAB_ID_IS_VALID(pstPdnInfo->ucRabId))
    {
        RNIC_INFO_LOG(ACPU_PID_RNIC,
            "RNIC_ProcImsaPdnActInd_Lte: RabId is invalid.");
        return VOS_ERR;
    }

    ucExRabId       = RNIC_BUILD_EXRABID(pstPdnInfo->enModemId, pstPdnInfo->ucRabId);
    pstPsIfaceInfo  = RNIC_GET_IFACE_PDN_INFO_ADR(ucRmNetId);

    /* ����PDP��������Ϣ */
    pstPsIfaceInfo->enRatType = RNIC_PS_RAT_TYPE_3GPP;
    pstPsIfaceInfo->enModemId = pstPdnInfo->enModemId;

    /* IPV4���� */
    if (VOS_TRUE == pstPdnInfo->bitOpIpv4PdnInfo)
    {
        pstPsIfaceInfo->bitOpIpv4Act  = RNIC_BIT_OPT_TRUE;
        pstPsIfaceInfo->ucIpv4RabId   = pstPdnInfo->ucRabId;
        pstPsIfaceInfo->ucIpv4ExRabId = ucExRabId;
        ucIpFamilyMask = RNIC_BIT8_SET(ucIpFamilyMask, RNIC_IPV4_ADDR);
    }

    /* IPV6���� */
    if (VOS_TRUE == pstPdnInfo->bitOpIpv6PdnInfo)
    {
        pstPsIfaceInfo->bitOpIpv6Act  = RNIC_BIT_OPT_TRUE;
        pstPsIfaceInfo->ucIpv6RabId   = pstPdnInfo->ucRabId;
        pstPsIfaceInfo->ucIpv6ExRabId = ucExRabId;
        ucIpFamilyMask = RNIC_BIT8_SET(ucIpFamilyMask, RNIC_IPV6_ADDR);
    }

    RNIC_IFACE_PsIfaceUp(ucRmNetId, ucIpFamilyMask);

    return VOS_OK;
}


VOS_UINT32 RNIC_RcvImsaPdnActInd(
    MsgBlock                           *pstMsg
)
{
    IMSA_RNIC_PDN_ACT_IND_STRU         *pstRcvInd   = VOS_NULL_PTR;
    IMSA_RNIC_PDN_INFO_CONFIG_STRU     *pstPdnInfo  = VOS_NULL_PTR;
    VOS_UINT32                          ulResult;

    pstRcvInd   = (IMSA_RNIC_PDN_ACT_IND_STRU *)pstMsg;
    pstPdnInfo  =  &(pstRcvInd->stPdnInfo);

    if (IMSA_RNIC_IMS_RAT_TYPE_WIFI == pstPdnInfo->enRatType)
    {
        ulResult = RNIC_ProcImsaPdnActInd_Wifi(pstPdnInfo);
    }
    else if (IMSA_RNIC_IMS_RAT_TYPE_LTE == pstPdnInfo->enRatType)
    {
        ulResult = RNIC_ProcImsaPdnActInd_Lte(pstPdnInfo);
    }
    else
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_RcvImsaPdnActInd rat type abnormal");
        ulResult = VOS_ERR;
    }

    /* ��ǰnetmanager ֻ��up vt������volte��RNIC_RMNET_ID_EMC0����Ҳ����netmanager up��
       vowifi����ͨ���������ͽ�����������������mapcon��up��down�ģ����Դ˴������߼��ݲ��޸� */
    if ((VOS_OK == ulResult)
     && (IMSA_RNIC_PDN_FOR_EMC != pstPdnInfo->enEmcInd))
    {
        RNIC_SndNetManagerPdpActInd(pstMsg);
    }

    return ulResult;
}


VOS_UINT32 RNIC_ProcImsaPdnDeactInd_Wifi(
    MODEM_ID_ENUM_UINT16                enModemId,
    IMSA_RNIC_PDN_EMC_IND_ENUM_UINT8    enEmcInd
)
{
    RNIC_PS_IFACE_INFO_STRU            *pstPsIfaceInfo = VOS_NULL_PTR;
    VOS_UINT8                           ucIpFamilyMask = 0;
    VOS_UINT8                           ucRmNetId;

    /* ָ��һ��ר�ŵ���������VT��Ƶ���ݴ��� */
    ucRmNetId = RNIC_GetImsRmnetId(IMSA_RNIC_IMS_RAT_TYPE_WIFI, enModemId, enEmcInd);
    if (RNIC_DEV_ID_BUTT == ucRmNetId)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_ProcImsaPdnDeactInd_Wifi: ucRmNetId butt.");
        return VOS_ERR;
    }

    /* ��ȡPDP�����ĵ�ַ */
    pstPsIfaceInfo = RNIC_GET_IFACE_PDN_INFO_ADR(ucRmNetId);
    pstPsIfaceInfo->enModemId = MODEM_ID_BUTT;
    pstPsIfaceInfo->enRatType = RNIC_PS_RAT_TYPE_BUTT;

    /* �����IPV4 PDPȥ���� */
    if (RNIC_BIT_OPT_TRUE == pstPsIfaceInfo->bitOpIpv4Act)
    {
        /* ���IPV4 PDP��������Ϣ */
        RNIC_InitIpv4PdnInfo(pstPsIfaceInfo);
        ucIpFamilyMask = RNIC_BIT8_SET(ucIpFamilyMask, RNIC_IPV4_ADDR);
    }

    if (RNIC_BIT_OPT_TRUE == pstPsIfaceInfo->bitOpIpv6Act)
    {
        /* ���IPV6 PDP��������Ϣ */
        RNIC_InitIpv6PdnInfo(pstPsIfaceInfo);
        ucIpFamilyMask = RNIC_BIT8_SET(ucIpFamilyMask, RNIC_IPV6_ADDR);
    }

    RNIC_IFACE_PsIfaceDown(ucRmNetId, ucIpFamilyMask);

    return VOS_OK;
}


VOS_UINT32 RNIC_ProcImsaPdnDeactInd_Lte(
    MODEM_ID_ENUM_UINT16                enModemId,
    IMSA_RNIC_PDN_EMC_IND_ENUM_UINT8    enEmcInd
)
{
    RNIC_PS_IFACE_INFO_STRU            *pstPsIfaceInfo = VOS_NULL_PTR;
    VOS_UINT8                           ucIpFamilyMask = 0;
    VOS_UINT8                           ucRmNetId;

    /* ָ��һ��ר�ŵ���������VT��Ƶ���ݴ��� */
    ucRmNetId = RNIC_GetImsRmnetId(IMSA_RNIC_IMS_RAT_TYPE_LTE, enModemId, enEmcInd);
    if (RNIC_DEV_ID_BUTT == ucRmNetId)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_ProcImsaPdnDeactInd_Lte: ucRmNetId butt.");
        return VOS_ERR;
    }

    pstPsIfaceInfo = RNIC_GET_IFACE_PDN_INFO_ADR(ucRmNetId);
    pstPsIfaceInfo->enModemId = MODEM_ID_BUTT;
    pstPsIfaceInfo->enRatType = RNIC_PS_RAT_TYPE_BUTT;

    /* �����IPV4 PDPȥ���� */
    if (RNIC_BIT_OPT_TRUE == pstPsIfaceInfo->bitOpIpv4Act)
    {
        /* ���IPV4 PDP��������Ϣ */
        RNIC_InitIpv4PdnInfo(pstPsIfaceInfo);
        ucIpFamilyMask = RNIC_BIT8_SET(ucIpFamilyMask, RNIC_IPV4_ADDR);
    }

    if (RNIC_BIT_OPT_TRUE == pstPsIfaceInfo->bitOpIpv6Act)
    {
        /* ���IPV6 PDP��������Ϣ */
        RNIC_InitIpv6PdnInfo(pstPsIfaceInfo);
        ucIpFamilyMask = RNIC_BIT8_SET(ucIpFamilyMask, RNIC_IPV6_ADDR);
    }

    RNIC_IFACE_PsIfaceDown(ucRmNetId, ucIpFamilyMask);

    return VOS_OK;
}


VOS_UINT32 RNIC_RcvImsaPdnDeactInd(
    MsgBlock                           *pstMsg
)
{
    IMSA_RNIC_PDN_DEACT_IND_STRU       *pstRcvInd;
    MODEM_ID_ENUM_UINT16                enModemId;
    VOS_UINT32                          ulResult;

    pstRcvInd   = (IMSA_RNIC_PDN_DEACT_IND_STRU *)pstMsg;
    enModemId   = pstRcvInd->enModemId;

    if (IMSA_RNIC_IMS_RAT_TYPE_WIFI == pstRcvInd->enRatType)
    {
        ulResult = RNIC_ProcImsaPdnDeactInd_Wifi(enModemId, pstRcvInd->enEmcInd);
    }
    else if (IMSA_RNIC_IMS_RAT_TYPE_LTE == pstRcvInd->enRatType)
    {
        ulResult = RNIC_ProcImsaPdnDeactInd_Lte(enModemId, pstRcvInd->enEmcInd);
    }
    else
    {
        RNIC_ERROR_LOG1(ACPU_PID_RNIC, "RNIC_RcvImsaPdnDeactInd rat type abnormal", pstRcvInd->enRatType);
        ulResult = VOS_ERR;
    }

    if ((VOS_OK == ulResult)
     && (IMSA_RNIC_PDN_FOR_EMC != pstRcvInd->enEmcInd))
    {
        RNIC_SndNetManagerPdpDeactInd(pstMsg);
    }

    return ulResult;
}


VOS_UINT32 RNIC_RcvImsaPdnModifyInd(
    MsgBlock                           *pstMsg
)
{
    IMSA_RNIC_PDN_MODIFY_IND_STRU      *pstRcvInd   = VOS_NULL_PTR;
    IMSA_RNIC_PDN_INFO_CONFIG_STRU     *pstPdnInfo  = VOS_NULL_PTR;
    VOS_UINT32                          ulResult;

    pstRcvInd   = (IMSA_RNIC_PDN_MODIFY_IND_STRU *)pstMsg;
    pstPdnInfo  = &(pstRcvInd->stPdnInfo);

    /* ��ǰRNIC_RMNET_ID_EMC0ֻ�ṩ��lteʹ�ã��ҽ�������ǰ���漰�л�,
       ��modify��Ϣ��Emc��־����ʱ����Ϊ�쳣 */
    if (IMSA_RNIC_PDN_FOR_EMC == pstPdnInfo->enEmcInd)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_RcvImsaPdnModifyInd: abnormal rcv emc modify ind.");
        return VOS_ERR;
    }

    if (IMSA_RNIC_IMS_RAT_TYPE_LTE == pstPdnInfo->enRatType)
    {
        RNIC_ProcImsaPdnDeactInd_Wifi(pstPdnInfo->enModemId, IMSA_RNIC_PDN_NOT_FOR_EMC);
        RNIC_ProcImsaPdnActInd_Lte(pstPdnInfo);
        ulResult = VOS_OK;
    }
    else if (IMSA_RNIC_IMS_RAT_TYPE_WIFI == pstPdnInfo->enRatType)
    {
        RNIC_ProcImsaPdnDeactInd_Lte(pstPdnInfo->enModemId, IMSA_RNIC_PDN_NOT_FOR_EMC);
        RNIC_ProcImsaPdnActInd_Wifi(pstPdnInfo);
        ulResult = VOS_OK;
    }
    else
    {
        RNIC_ERROR_LOG1(ACPU_PID_RNIC, "RNIC_RcvImsaPdnModifyInd: rat type is ", pstPdnInfo->enRatType);
        ulResult = VOS_ERR;
    }

    if ((VOS_OK == ulResult)
     && (IMSA_RNIC_PDN_FOR_EMC != pstPdnInfo->enEmcInd))
    {
        RNIC_SndNetManagerPdpModifyInd(pstMsg);
    }

    return ulResult;
}


VOS_UINT32 RNIC_RcvImsaReservedPortsCfgInd(
    MsgBlock                           *pstMsg
)
{
    IMSA_RNIC_RESERVED_PORTS_CONFIG_IND_STRU         *pstRcvInd    = VOS_NULL_PTR;
    IMSA_RNIC_IMS_PORT_INFO_STRU                     *pstPortInfo  = VOS_NULL_PTR;

    pstRcvInd    = (IMSA_RNIC_RESERVED_PORTS_CONFIG_IND_STRU *)pstMsg;
    pstPortInfo  =  &(pstRcvInd->stImsPortInfo);

    if (pstPortInfo->ulImsPortRangeNum > IMSA_RNIC_IMS_PORT_RANGE_GROUP_MAX_NUM)
    {
        RNIC_ERROR_LOG1(ACPU_PID_RNIC, "RNIC_RcvImsaReservedPortsCfgInd: ulImsPortRangeNum too large: ",pstPortInfo->ulImsPortRangeNum);
        return VOS_ERR;
    }

    RNIC_SndNetManagerReservedPortCfgInd(pstMsg);

    return VOS_OK;
}


VOS_UINT32 RNIC_RcvImsaSocketExceptionInd(
    MsgBlock                           *pstMsg
)
{
    RNIC_SndNetManagerSocketExceptInd(pstMsg);
    return VOS_OK;
}


VOS_UINT32 RNIC_RcvImsaSipPortRangeInd(
    MsgBlock                           *pstMsg
)
{
    IMSA_RNIC_SIP_PORT_RANGE_IND_STRU   *pstRcvInd = VOS_NULL_PTR;

    pstRcvInd    = (IMSA_RNIC_SIP_PORT_RANGE_IND_STRU *)pstMsg;

    if (pstRcvInd->usSipPortRangeNum > IMSA_RNIC_SIP_PORT_RANGE_GROUP_MAX_NUM)
    {
        RNIC_ERROR_LOG1(ACPU_PID_RNIC, "RNIC_RcvImsaSipPortRangeInd: usSipPortRangeNum too large: ",pstRcvInd->usSipPortRangeNum);
        return VOS_ERR;
    }

    RNIC_SndNetManagerSipPortRangeInd(pstMsg);

    return VOS_OK;
}


VOS_UINT8 RNIC_GetImsRmnetId(
    IMSA_RNIC_IMS_RAT_TYPE_ENUM_UINT8   enRatType,
    MODEM_ID_ENUM_UINT16                enModemId,
    IMSA_RNIC_PDN_EMC_IND_ENUM_UINT8    enEmcInd
)
{
    RNIC_NORMAL_LOG3(ACPU_PID_RNIC, "RNIC_GetImsRmnetId: rat modemid emc is ", enRatType, enModemId, enEmcInd);

    if (IMSA_RNIC_PDN_FOR_EMC == enEmcInd)
    {
        return RNIC_GetImsEmcBearRmnetId(enRatType, enModemId);
    }
    else if (IMSA_RNIC_PDN_NOT_FOR_EMC == enEmcInd)
    {
        return RNIC_GetImsNormalBearRmnetId(enRatType, enModemId);
    }
    else
    {
        RNIC_NORMAL_LOG1(ACPU_PID_RNIC, "RNIC_GetImsRmnetId: enEmcInd abnormal, ", enEmcInd);
        return RNIC_DEV_ID_BUTT;
    }
}


VOS_UINT8 RNIC_GetImsEmcBearRmnetId(
    IMSA_RNIC_IMS_RAT_TYPE_ENUM_UINT8   enRatType,
    MODEM_ID_ENUM_UINT16                enModemId
)
{
    /* ��ǰRNIC_RMNET_ID_EMC0ֻ�ṩ��lteʹ�ã�����ֻ��Ҫ�жϽ��뼼��Ϊlte��ʱEmcInd��־ */
    if ((IMSA_RNIC_IMS_RAT_TYPE_LTE == enRatType)
     && (MODEM_ID_0 == enModemId))
    {
        return RNIC_DEV_ID_RMNET_EMC0;
    }

    if ((IMSA_RNIC_IMS_RAT_TYPE_WIFI == enRatType)
     && (MODEM_ID_0 == enModemId))
    {
        return RNIC_DEV_ID_RMNET_R_IMS01;
    }

    if ((IMSA_RNIC_IMS_RAT_TYPE_WIFI == enRatType)
     && (MODEM_ID_1 == enModemId))
    {
        return RNIC_DEV_ID_RMNET_R_IMS11;
    }

    return RNIC_DEV_ID_BUTT;
}


VOS_UINT8 RNIC_GetImsNormalBearRmnetId(
    IMSA_RNIC_IMS_RAT_TYPE_ENUM_UINT8   enRatType,
    MODEM_ID_ENUM_UINT16                enModemId
)
{
    /* vowifiʱΪ���ݰ�ת��������volteʱΪvt���� */
    if ((MODEM_ID_0 == enModemId)
     && (IMSA_RNIC_IMS_RAT_TYPE_LTE == enRatType))
    {
        return RNIC_DEV_ID_RMNET_IMS00;
    }

    if ((MODEM_ID_0 == enModemId)
     && (IMSA_RNIC_IMS_RAT_TYPE_WIFI == enRatType))
    {
        return RNIC_DEV_ID_RMNET_R_IMS00;
    }

    if ((MODEM_ID_1 == enModemId)
     && (IMSA_RNIC_IMS_RAT_TYPE_LTE == enRatType))
    {
        return RNIC_DEV_ID_RMNET_IMS10;
    }

    if ((MODEM_ID_1 == enModemId)
     && (IMSA_RNIC_IMS_RAT_TYPE_WIFI == enRatType))
    {
        return RNIC_DEV_ID_RMNET_R_IMS10;
    }

    return RNIC_DEV_ID_BUTT;
}


VOS_UINT32 RNIC_ProcImsData(MsgBlock *pMsg)
{
    RNIC_IMS_DATA_PROC_IND_STRU        *pstDataProcInd = VOS_NULL_PTR;
    RNIC_IFACE_CTX_STRU                *pstIfaceCtx    = VOS_NULL_PTR;

    pstDataProcInd = (RNIC_IMS_DATA_PROC_IND_STRU *)pMsg;
    pstIfaceCtx    = RNIC_GET_IFACE_CTX_ADR(pstDataProcInd->enRmNetId);

    RNIC_SendVoWifiUlData(pstIfaceCtx);

    return VOS_OK;
}


VOS_UINT32 RNIC_RcvCdsImsDataInd(
    MsgBlock                           *pstMsg
)
{
    CDS_RNIC_IMS_DATA_IND_STRU         *pstImsDataInd = VOS_NULL_PTR;
    VOS_UINT32                          ulRet;
    VOS_UINT8                           ucRmNetId;

    pstImsDataInd = (CDS_RNIC_IMS_DATA_IND_STRU *)pstMsg;

    ucRmNetId = RNIC_GetImsRmnetId(IMSA_RNIC_IMS_RAT_TYPE_WIFI,
                                    pstImsDataInd->usModemId,
                                    (IMSA_RNIC_PDN_EMC_IND_ENUM_UINT8)pstImsDataInd->enDataType);

    if (ucRmNetId >= RNIC_DEV_ID_BUTT)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_RcvCdsImsDataInd: ucRmNetId butt.");
        return VOS_ERR;
    }

    ulRet = RNIC_RecvVoWifiDlData(ucRmNetId, pstImsDataInd);

    return ulRet;
}


RNIC_PROC_MSG_FUNC RNIC_GetProcMsgFunc(
    VOS_UINT32                          ulMsgId,
    VOS_UINT32                          ulSenderPid
)
{
    VOS_UINT32                           i;
    VOS_UINT32                           ulTableSize;

    /* ��ȡ����������� */
    ulTableSize = sizeof(g_astRnicMsgProcTab)/sizeof(RNIC_PROC_MSG_STRU);

    /* g_astRnicMsgProcTab���������Ϣ����ķַ� */
    for (i = 0; i < ulTableSize; i++)
    {
        if ( (g_astRnicMsgProcTab[i].ulMsgId == ulMsgId)
          && (g_astRnicMsgProcTab[i].ulSenderPid == ulSenderPid) )
        {
            return g_astRnicMsgProcTab[i].pRnicProcMsgFunc;
        }
    }

    RNIC_ERROR_LOG2(ACPU_PID_RNIC,"RNIC_GetProcMsgFunc:SendPid and MsgId is", ulSenderPid, ulMsgId);

    return VOS_NULL_PTR;
}


VOS_UINT32 RNIC_GetModem0Pid(
    VOS_UINT32                          ulSenderPid
)
{
    VOS_UINT32                          ulModem0Pid;

    switch (ulSenderPid)
    {
        case I0_PS_PID_IMSA:
        case I1_PS_PID_IMSA:

            ulModem0Pid = PS_PID_IMSA;
            break;

        default:
            ulModem0Pid = ulSenderPid;
            break;
    }

    return ulModem0Pid;
}


VOS_UINT32 RNIC_ProcMsg (MsgBlock *pstMsg)
{
    MSG_HEADER_STRU                    *pstMsgHeader = VOS_NULL_PTR;
    REL_TIMER_MSG                      *pstTimerMsg  = VOS_NULL_PTR;
    RNIC_PROC_MSG_FUNC                  pProcMsgFunc = VOS_NULL_PTR;
    VOS_UINT32                          ulRst;
    VOS_UINT32                          ulMsgName;
    VOS_UINT32                          ulSenderPid;

    if (VOS_NULL_PTR == pstMsg)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_ProcMsg: Msg is invalid!");
        return VOS_ERR;
    }

    pstMsgHeader = (MSG_HEADER_STRU *)pstMsg;

    ulSenderPid = pstMsgHeader->ulSenderPid;

    /* ��ʱ��Ϣ��ʽֹͣ��ʱ�� */
    if (VOS_PID_TIMER == ulSenderPid)
    {
        pstTimerMsg = (REL_TIMER_MSG *)pstMsg;
        ulMsgName   = pstTimerMsg->ulName;
        RNIC_StopTimer((VOS_UINT16)ulMsgName);
    }
    else
    {
        ulMsgName   = pstMsgHeader->ulMsgName;
    }

    /* ��SenderPid ͳһתΪmodem0 Pid */
    ulSenderPid = RNIC_GetModem0Pid(ulSenderPid);

    /* �õ���Ϣ������ */
    pProcMsgFunc = RNIC_GetProcMsgFunc(ulMsgName, ulSenderPid);

    if (VOS_NULL_PTR != pProcMsgFunc)
    {
        ulRst = pProcMsgFunc(pstMsg);
        if (VOS_ERR == ulRst)
        {
            RNIC_NORMAL_LOG(ACPU_PID_RNIC, "RNIC_ProcMsgEntry:proc func error!");
            return VOS_ERR;
        }
    }
    else
    {
        RNIC_ERROR_LOG2(ACPU_PID_RNIC,"RNIC_ProcMsgEntry:SendPid and MsgId is", ulSenderPid, ulMsgName);
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 RNIC_SendDialInfoMsg(
    RNIC_INTRA_MSG_ID_ENUM_UINT32       enMsgId
)
{
    RNIC_NOTIFY_MSG_STRU               *pstDialInfo;
    RNIC_DIAL_MODE_STRU                *pstDialMode;


    /* �ڴ���� */
    pstDialInfo = (RNIC_NOTIFY_MSG_STRU *)PS_ALLOC_MSG(ACPU_PID_RNIC,
                                                      sizeof(RNIC_NOTIFY_MSG_STRU) - VOS_MSG_HEAD_LENGTH);
    if (VOS_NULL_PTR == pstDialInfo)
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_SendDialInfoMsg: Malloc failed!");
        return VOS_ERR;
    }

    pstDialMode                         = RNIC_GET_DIAL_MODE_ADR();

    /* �����Ϣ */
    RNIC_CFG_INTRA_MSG_HDR(pstDialInfo, enMsgId);

    TAF_MEM_CPY_S(&(pstDialInfo->stDialInfo), sizeof(pstDialInfo->stDialInfo), pstDialMode, sizeof(RNIC_DIAL_MODE_STRU));

    if (VOS_OK != PS_SEND_MSG(ACPU_PID_RNIC, pstDialInfo))
    {
        RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_SendDialInfoMsg: Send msg failed!");
        return VOS_ERR;
    }

    return VOS_OK;

}


VOS_VOID RNIC_FillNetManagerMsgPdnCfgInfo(
    NM_PDN_INFO_CONFIG_STRU            *pDestPdnInfo,
    IMSA_RNIC_PDN_INFO_CONFIG_STRU     *pSrcPdnInfo
)
{
    /* ��stRnicNmMsg�в������и�ֵ */
    pDestPdnInfo->bitOpIpv4PdnInfo     = pSrcPdnInfo->bitOpIpv4PdnInfo;
    pDestPdnInfo->bitOpIpv6PdnInfo     = pSrcPdnInfo->bitOpIpv6PdnInfo;
    pDestPdnInfo->bitOpMtuInfo         = pSrcPdnInfo->bitOpMtuInfo;

    pDestPdnInfo->enModemId    = pSrcPdnInfo->enModemId;
    pDestPdnInfo->enRatType    = pSrcPdnInfo->enRatType;
    pDestPdnInfo->ulMtuSize    = pSrcPdnInfo->ulMtuSize;

    /* ��ipv4��pdn���и�ֵ */
    TAF_MEM_CPY_S(&(pDestPdnInfo->stIpv4PdnInfo),
                  sizeof(NM_IPV4_PDN_INFO_STRU),
                  &(pSrcPdnInfo->stIpv4PdnInfo),
                  sizeof(IMSA_RNIC_IPV4_PDN_INFO_STRU));

    TAF_MEM_CPY_S(&(pDestPdnInfo->stIpv6PdnInfo),
                  sizeof(NM_IPV6_PDN_INFO_STRU),
                  &(pSrcPdnInfo->stIpv6PdnInfo),
                  sizeof(IMSA_RNIC_IPV6_PDN_INFO_STRU));
    return;
}


VOS_VOID RNIC_FillNetManagerMsgReservedPortCfgInfo(
    NM_RESERVED_PORTS_CONFIG_STRU      *pDestReservedProtInfo,
    IMSA_RNIC_IMS_PORT_INFO_STRU       *pSrcReservedProtInfo
)
{
    /* ��stRnicNmMsg�в������и�ֵ */
    pDestReservedProtInfo->ulImsPortRangeNum = pSrcReservedProtInfo->ulImsPortRangeNum;
    pDestReservedProtInfo->enModemId         = pSrcReservedProtInfo->enModemId;

    TAF_MEM_CPY_S(pDestReservedProtInfo->astImsPortRange,
                  (sizeof(NM_IMS_PORT_RANGE_STRU) * NM_IMS_PORT_RANGE_GROUP_MAX_NUM),
                  pSrcReservedProtInfo->astImsPortRange,
                  (sizeof(IMSA_RNIC_IMS_PORT_RANGE_STRU) * IMSA_RNIC_IMS_PORT_RANGE_GROUP_MAX_NUM));

    return;
}


VOS_VOID RNIC_FillNetManagerMsgSipPortRangeInfo(
    NM_SIP_PORTS_RANGE_STRU            *pDestSipProtRange,
    IMSA_RNIC_SIP_PORT_RANGE_IND_STRU  *pSrcSipProtRange
)
{
    /* ��stRnicNmMsg�в������и�ֵ */
    pDestSipProtRange->usSipPortRangeNum = pSrcSipProtRange->usSipPortRangeNum;
    pDestSipProtRange->enModemId         = pSrcSipProtRange->enModemId;

    TAF_MEM_CPY_S(pDestSipProtRange->astSipPortRange,
                  (sizeof(NM_IMS_PORT_RANGE_STRU) * NM_SIP_PORT_RANGE_GROUP_MAX_NUM),
                  pSrcSipProtRange->astSipPortRange,
                  (sizeof(IMSA_RNIC_IMS_PORT_RANGE_STRU) * IMSA_RNIC_SIP_PORT_RANGE_GROUP_MAX_NUM));

    return;
}


VOS_VOID RNIC_SndNetManagerPdpActInd(
    MsgBlock                           *pstMsg
)
{
    IMSA_RNIC_PDN_ACT_IND_STRU         *pstRcvInd;
    NM_MSG_STRU                         stRnicNmMsg;

    TAF_MEM_SET_S(&stRnicNmMsg, sizeof(NM_MSG_STRU), 0x00, sizeof(NM_MSG_STRU));

    pstRcvInd               = (IMSA_RNIC_PDN_ACT_IND_STRU *)pstMsg;
    stRnicNmMsg.enMsgId     = ID_NM_PDN_ACT_IND;
    stRnicNmMsg.ulMsgLen    = sizeof(NM_PDN_INFO_CONFIG_STRU);

    RNIC_FillNetManagerMsgPdnCfgInfo(&(stRnicNmMsg.unMsgInfo.stPdnCfgInfo), &(pstRcvInd->stPdnInfo));

    /* ���������豸�ṩ�ķ��ͽӿڷ�����Ϣ */
    NM_CTRL_SendMsg(&stRnicNmMsg, sizeof(NM_MSG_STRU));

    return;
}


VOS_VOID RNIC_SndNetManagerPdpDeactInd(
    MsgBlock                           *pstMsg
)
{
    IMSA_RNIC_PDN_DEACT_IND_STRU       *pstRcvInd;
    NM_MSG_STRU                         stRnicNmMsg;

    pstRcvInd                           = (IMSA_RNIC_PDN_DEACT_IND_STRU *)pstMsg;
    TAF_MEM_SET_S(&stRnicNmMsg, sizeof(NM_MSG_STRU), 0x00, sizeof(NM_MSG_STRU));

    stRnicNmMsg.enMsgId     = ID_NM_PDN_DEACT_IND;
    stRnicNmMsg.ulMsgLen    = sizeof(NM_PDN_DEACT_IND_STRU);

    /* ��stRnicNmMsg�в������и�ֵ */
    stRnicNmMsg.unMsgInfo.stPdnDeactInd.enModemId   = pstRcvInd->enModemId;
    stRnicNmMsg.unMsgInfo.stPdnDeactInd.enRatType   = pstRcvInd->enRatType;

    /* ���������豸�ṩ�ķ��ͽӿڷ�����Ϣ */
    NM_CTRL_SendMsg(&stRnicNmMsg, sizeof(NM_MSG_STRU));

    return;
}


VOS_VOID RNIC_SndNetManagerPdpModifyInd(
    MsgBlock                           *pstMsg
)
{
    IMSA_RNIC_PDN_MODIFY_IND_STRU      *pstRcvInd;
    NM_MSG_STRU                         stRnicNmMsg;

    TAF_MEM_SET_S(&stRnicNmMsg, sizeof(NM_MSG_STRU), 0x00, sizeof(NM_MSG_STRU));

    pstRcvInd               = (IMSA_RNIC_PDN_MODIFY_IND_STRU *)pstMsg;
    stRnicNmMsg.enMsgId     = ID_NM_PDN_MODIFY_IND;
    stRnicNmMsg.ulMsgLen    = sizeof(NM_PDN_INFO_CONFIG_STRU);

    RNIC_FillNetManagerMsgPdnCfgInfo(&(stRnicNmMsg.unMsgInfo.stPdnCfgInfo), &(pstRcvInd->stPdnInfo));

    /* ���������豸�ṩ�ķ��ͽӿڷ�����Ϣ */
    NM_CTRL_SendMsg(&stRnicNmMsg, sizeof(NM_MSG_STRU));

    return;
}


VOS_VOID RNIC_SndNetManagerModemResetInd(VOS_VOID)
{
    NM_MSG_STRU                    stRnicNmMsg;

    TAF_MEM_SET_S(&stRnicNmMsg, sizeof(NM_MSG_STRU), 0x00, sizeof(NM_MSG_STRU));

    stRnicNmMsg.enMsgId     = ID_NM_MODEM_RESET_IND;
    stRnicNmMsg.ulMsgLen    = 0;

    /* ���������豸�ṩ�ķ��ͽӿڷ�����Ϣ */
    NM_CTRL_SendMsg(&stRnicNmMsg, sizeof(NM_MSG_STRU));

    return;
}


VOS_VOID RNIC_SndNetManagerReservedPortCfgInd(
    MsgBlock                           *pstMsg
)
{
    IMSA_RNIC_RESERVED_PORTS_CONFIG_IND_STRU         *pstRcvInd;
    NM_MSG_STRU                                       stRnicNmMsg;

    TAF_MEM_SET_S(&stRnicNmMsg, sizeof(NM_MSG_STRU), 0x00, sizeof(NM_MSG_STRU));

    pstRcvInd               = (IMSA_RNIC_RESERVED_PORTS_CONFIG_IND_STRU *)pstMsg;
    stRnicNmMsg.enMsgId     = ID_NM_RESERVED_PORTS_CONFIG_IND;
    stRnicNmMsg.ulMsgLen    = sizeof(NM_RESERVED_PORTS_CONFIG_STRU);

    RNIC_FillNetManagerMsgReservedPortCfgInfo(&(stRnicNmMsg.unMsgInfo.stPortsCfgInfo), &(pstRcvInd->stImsPortInfo));

    /* ���������豸�ṩ�ķ��ͽӿڷ�����Ϣ */
    NM_CTRL_SendMsg(&stRnicNmMsg, sizeof(NM_MSG_STRU));

    return;
}


VOS_VOID RNIC_SndNetManagerSocketExceptInd(
    MsgBlock                           *pstMsg
)
{
    IMSA_RNIC_SOCKET_EXCEPTION_IND_STRU               *pstRcvInd;
    NM_MSG_STRU                                        stRnicNmMsg;

    TAF_MEM_SET_S(&stRnicNmMsg, sizeof(NM_MSG_STRU), 0x00, sizeof(NM_MSG_STRU));

    pstRcvInd               = (IMSA_RNIC_SOCKET_EXCEPTION_IND_STRU *)pstMsg;
    stRnicNmMsg.enMsgId     = ID_NM_SOCKET_EXCEPTION_IND;
    stRnicNmMsg.ulMsgLen    = sizeof(NM_SOCKET_EXCEPTION_IND_STRU);

    /* ��Ϣ�帳ֵ */
    stRnicNmMsg.unMsgInfo.stSocketExceptionInd.sSocketErrorNo = pstRcvInd->sSocketErrorNo;
    RNIC_FillNetManagerMsgPdnCfgInfo(&(stRnicNmMsg.unMsgInfo.stSocketExceptionInd.stPdnInfo), &(pstRcvInd->stPdnInfo));

    /* ���������豸�ṩ�ķ��ͽӿڷ�����Ϣ */
    NM_CTRL_SendMsg(&stRnicNmMsg, sizeof(NM_MSG_STRU));

    return;
}


VOS_VOID RNIC_SndNetManagerSipPortRangeInd(
    MsgBlock                           *pstMsg
)
{
    IMSA_RNIC_SIP_PORT_RANGE_IND_STRU  *pstRcvInd;
    NM_MSG_STRU                         stRnicNmMsg;

    TAF_MEM_SET_S(&stRnicNmMsg, sizeof(NM_MSG_STRU), 0x00, sizeof(NM_MSG_STRU));

    pstRcvInd               = (IMSA_RNIC_SIP_PORT_RANGE_IND_STRU *)pstMsg;
    stRnicNmMsg.enMsgId     = ID_NM_SIP_PORT_RANGE_IND;
    stRnicNmMsg.ulMsgLen    = sizeof(NM_SIP_PORTS_RANGE_STRU);

    RNIC_FillNetManagerMsgSipPortRangeInfo(&(stRnicNmMsg.unMsgInfo.stSipPortRangeInd), pstRcvInd);

    /* ���������豸�ṩ�ķ��ͽӿڷ�����Ϣ */
    NM_CTRL_SendMsg(&stRnicNmMsg, sizeof(NM_MSG_STRU));

    return;
}

