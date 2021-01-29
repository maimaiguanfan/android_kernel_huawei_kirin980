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
#ifndef _RNIC_CTX_H_
#define _RNIC_CTX_H_

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "PsLogdef.h"
#include "PsTypeDef.h"
#include "PsCommonDef.h"
#include "PsIfaceGlobalDef.h"
#include "ImmInterface.h"
#include "ImsaRnicInterface.h"
#include "AtRnicInterface.h"
#include "RnicTimerMgmt.h"
#include "NVIM_Interface.h"
#include "acore_nv_stru_gucnas.h"
#if (FEATURE_ON == FEATURE_DATA_SERVICE_NEW_PLATFORM)
#include "mdrv_eipf.h"
#include "ads_dev_i.h"
#else
#include "AdsDeviceInterface.h"
#endif
#include <linux/wakelock.h>


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#if (VOS_OS_VER != VOS_WIN32)
#pragma pack(4)
#else
#pragma pack(push, 4)
#endif

/*****************************************************************************
  2 �궨��
*****************************************************************************/

#define RNIC_NET_ID_MAX_NUM             (RNIC_DEV_ID_BUTT)
#define RNIC_3GPP_NET_ID_MAX_NUM        (RNIC_DEV_ID_RMNET6)
#define RNIC_MODEM_ID_MAX_NUM           (MODEM_ID_BUTT)

#define RNIC_IPV4_VERSION               (4)                                     /* IPͷ����IP V4�汾�� */
#define RNIC_IPV6_VERSION               (6)                                     /* IPͷ����IP V6�汾�� */
#define RNIC_MAX_IPV6_ADDR_LEN          (16)                                    /* IPV6��ַ���� */

#define RNIC_MAX_DSFLOW_BYTE            (0xFFFFFFFF)                            /* RNIC�������ֵ */

#define RNIC_DIAL_DEMA_IDLE_TIME        (600)

/* RabId�ĸ���λ��ʾModemId, 00��ʾModem0, 01��ʾModem1, 10��ʾModem1 */
#define RNIC_RABID_TAKE_MODEM_1_MASK    (0x40)                                  /* RabidЯ��Modem1������ */
#define RNIC_RABID_TAKE_MODEM_2_MASK    (0x80)                                  /* RabidЯ��Modem2������ */
#define RNIC_RABID_UNTAKE_MODEM_MASK    (0x3F)                                  /* Rabidȥ��Modem������ */

/* ���RABIDֵ */
#define RNIC_RAB_ID_MAX_NUM             (11)
#define RNIC_RAB_ID_OFFSET              (5)
/* Rab Id����Сֵ */
#define RNIC_RAB_ID_MIN                 (5)
/* Rab Id�����ֵ */
#define RNIC_RAB_ID_MAX                 (15)
#define RNIC_RAB_ID_INVALID             (0xFF)

#define RNIC_NAPI_POLL_DEFAULT_WEIGHT   (16)                                    /* NAPI һ��Poll��ѯ��Ĭ���������� */
#define RNIC_NAPI_POLL_MAX_WEIGHT       (64)                                    /* NAPI һ��Poll��ѯ��Э��ջ֧�ֵ��������� */
#define RNIC_POLL_QUEUE_DEFAULT_MAX_LEN (15000)                                 /* NAPI Poll���е���󳤶� */

#define RNIC_INVALID_IFACE_ID           (0xFF)
#define RNIC_INVALID_PDU_SESSION_ID     (0xFF)
#if (FEATURE_ON == FEATURE_DATA_SERVICE_NEW_PLATFORM)
#define RNIC_INVALID_FC_HEAD            EIPF_FCHEAD_BOTTOM
#endif

#define RNIC_RAB_ID_IS_VALID(ucRabId) \
            (((ucRabId) >= RNIC_RAB_ID_MIN) && ((ucRabId) <= RNIC_RAB_ID_MAX))

#define RNIC_RMNET_IS_VALID(RmNetId) \
            ((RmNetId) < RNIC_DEV_ID_BUTT)
#define RNIC_IFACE_ID_IS_VALID(ucIfaceId) \
            ((ucIfaceId) < PS_IFACE_ID_BUTT)
#define RNIC_MODEM_ID_IS_VALID(enModemId) \
            ((enModemId) < MODEM_ID_BUTT)

#define RNIC_BIT8_MASK(bit)             ((VOS_UINT8)(1 << (bit)))
#define RNIC_BIT8_SET(val,bit)          ((val) | RNIC_BIT8_MASK(bit))
#define RNIC_BIT8_CLR(val,bit)          ((val) & ~RNIC_BIT8_MASK(bit))
#define RNIC_BIT8_IS_SET(val,bit)       ((val) & RNIC_BIT8_MASK(bit))

#if (FEATURE_ON == FEATURE_MULTI_MODEM)
#define RNIC_RMNET_R_IS_EMC_BEAR(RmNetId) \
            ((RNIC_DEV_ID_RMNET_R_IMS01 == (RmNetId)) \
          || (RNIC_DEV_ID_RMNET_R_IMS11 == (RmNetId)))
#else
#define RNIC_RMNET_R_IS_EMC_BEAR(RmNetId) \
            (RNIC_DEV_ID_RMNET_R_IMS01 == (RmNetId))
#endif

/* ��ȡRNIC�����ĵ�ַ */
#define RNIC_GET_RNIC_CTX_ADR()                     (&g_stRnicCtx)
/* ��ȡָ�������������ĵ�ַ */
#define RNIC_GET_IFACE_CTX_ADR(index)               (&(g_stRnicCtx.astIfaceCtx[index]))
/* ��ȡָ��������PDP�����ĵ�ַ */
#define RNIC_GET_IFACE_PDN_INFO_ADR(index)          (&(g_stRnicCtx.astIfaceCtx[index].stPsIfaceInfo))
/* ��ȡIPV4����PDN����״̬ */
#define RNIC_GET_IFACE_PDN_IPV4_ACT_STATE(index)    (g_stRnicCtx.astIfaceCtx[index].stPsIfaceInfo.bitOpIpv4Act)
/* ��ȡIPV6����PDN����״̬ */
#define RNIC_GET_IFACE_PDN_IPV6_ACT_STATE(index)    (g_stRnicCtx.astIfaceCtx[index].stPsIfaceInfo.bitOpIpv6Act)

/* ��ȡ��ǰ�������� */
#define RNIC_GET_IFACE_CUR_RECV_RATE(index)         (g_stRnicCtx.astIfaceCtx[index].stDsFlowStats.ulCurrentRecvRate)
/* ��ȡ��ǰ�������� */
#define RNIC_GET_IFACE_CUR_SEND_RATE(index)         (g_stRnicCtx.astIfaceCtx[index].stDsFlowStats.ulCurrentSendRate)
/* ���õ�ǰ�������� */
#define RNIC_SET_IFACE_CUR_RECV_RATE(index, val)    (g_stRnicCtx.astIfaceCtx[index].stDsFlowStats.ulCurrentRecvRate = (val))
/* ���õ�ǰ�������� */
#define RNIC_SET_IFACE_CUR_SEND_RATE(index, val)    (g_stRnicCtx.astIfaceCtx[index].stDsFlowStats.ulCurrentSendRate = (val))
/* ��ȡ��ǰ���ձ����� */
#define RNIC_GET_IFACE_PERIOD_RECV_PKT(index)       (g_stRnicCtx.astIfaceCtx[index].stDsFlowStats.ulPeriodRecvPktNum)
/* ��ȡ��ǰ���ͱ����� */
#define RNIC_GET_IFACE_PERIOD_SEND_PKT(index)       (g_stRnicCtx.astIfaceCtx[index].stDsFlowStats.ulPeriodSendPktNum)
/* ���õ�ǰ���ձ����� */
#define RNIC_SET_IFACE_PERIOD_RECV_PKT(index, val)  (g_stRnicCtx.astIfaceCtx[index].stDsFlowStats.ulPeriodRecvPktNum = (val))
/* ���õ�ǰ���ͱ����� */
#define RNIC_SET_IFACE_PERIOD_SEND_PKT(index, val)  (g_stRnicCtx.astIfaceCtx[index].stDsFlowStats.ulPeriodSendPktNum = (val))
/* ���õ�ǰ���ͱ����� */
#define RNIC_ADD_IFACE_PERIOD_SEND_PKT(index, val)  (g_stRnicCtx.astIfaceCtx[index].stDsFlowStats.ulPeriodSendPktNum += (val))
/* �����ǰ���ձ����� */
#define RNIC_CLEAN_IFACE_PERIOD_RECV_PKT(index)     (g_stRnicCtx.astIfaceCtx[index].stDsFlowStats.ulPeriodRecvPktNum = 0)
/* �����ǰ���ͱ����� */
#define RNIC_CLEAN_IFACE_PERIOD_SEND_PKT(index)     (g_stRnicCtx.astIfaceCtx[index].stDsFlowStats.ulPeriodSendPktNum = 0)


/* ��ȡָ������NAPI���Կ��� */
#define RNIC_GET_NAPI_FEATURE(index)                   (g_stRnicCtx.astIfaceCtx[index].stFeatureCfg.stNapiCfg.ucNapiEnable)
/* ��ȡָ������NAPI���Կ��� */
#define RNIC_GET_GRO_FEATURE(index)                    (g_stRnicCtx.astIfaceCtx[index].stFeatureCfg.stNapiCfg.ucGroEnable)
/* ��ȡ����NAPI weightֵ����ģʽ */
#define RNIC_GET_NAPI_WEIGHT_ADJ_MODE(index)           (g_stRnicCtx.astIfaceCtx[index].stFeatureCfg.stNapiCfg.enNapiWeightAdjMode)
/* ��ȡ����NAPI weightֵ */
#define RNIC_GET_NAPI_WEIGHT(index)                    (g_stRnicCtx.astIfaceCtx[index].stFeatureCfg.stNapiCfg.ucNapiPollWeight)
/* ��ȡ����NAPI Poll���г������ֵ */
#define RNIC_GET_NAPI_POLL_QUE_MAX_LEN(index)          (g_stRnicCtx.astIfaceCtx[index].stFeatureCfg.stNapiCfg.usNapiPollQueMaxLen)
/* ��ȡָ��������NAPI Weight��̬�������õ�ַ */
#define RNIC_GET_NAPI_DYNAMIC_CFG_ADR(index)           (&(g_stRnicCtx.astIfaceCtx[index].stFeatureCfg.stNapiCfg.stNapiWeightDynamicAdjCfg))
/* ��ȡ����NAPI����ÿ���հ�������λֵ1 */
#define RNIC_GET_DL_PKT_NUM_PER_SEC_LEVEL1(index)      (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(index)->stDlPktNumPerSecLevel.ulDlPktNumPerSecLevel1)
/* ��ȡ����NAPI����ÿ���հ�������λֵ2 */
#define RNIC_GET_DL_PKT_NUM_PER_SEC_LEVEL2(index)      (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(index)->stDlPktNumPerSecLevel.ulDlPktNumPerSecLevel2)
/* ��ȡ����NAPI����ÿ���հ�������λֵ3 */
#define RNIC_GET_DL_PKT_NUM_PER_SEC_LEVEL3(index)      (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(index)->stDlPktNumPerSecLevel.ulDlPktNumPerSecLevel3)
/* ��ȡ����NAPI����ÿ���հ�������λֵ4 */
#define RNIC_GET_DL_PKT_NUM_PER_SEC_LEVEL4(index)      (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(index)->stDlPktNumPerSecLevel.ulDlPktNumPerSecLevel4)

/* ��ȡ����NAPI weight��λֵ1 */
#define RNIC_GET_NAPI_WEIGHT_LEVEL1(index)             (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(index)->stNapiWeightLevel.ucNapiWeightLevel1)
/* ��ȡ����NAPI weight��λֵ2 */
#define RNIC_GET_NAPI_WEIGHT_LEVEL2(index)             (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(index)->stNapiWeightLevel.ucNapiWeightLevel2)
/* ��ȡ����NAPI weight��λֵ3 */
#define RNIC_GET_NAPI_WEIGHT_LEVEL3(index)             (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(index)->stNapiWeightLevel.ucNapiWeightLevel3)
/* ��ȡ����NAPI weight��λֵ4 */
#define RNIC_GET_NAPI_WEIGHT_LEVEL4(index)             (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(index)->stNapiWeightLevel.ucNapiWeightLevel4)

/* ��������NAPI���Կ��� */
#define RNIC_SET_NAPI_FEATURE(index, val)              (g_stRnicCtx.astIfaceCtx[index].stFeatureCfg.stNapiCfg.ucNapiEnable = (val))
/* ��������NAPI���Կ��� */
#define RNIC_SET_GRO_FEATURE(index, val)               (g_stRnicCtx.astIfaceCtx[index].stFeatureCfg.stNapiCfg.ucGroEnable = (val))
/* ��������NAPI weightֵ����ģʽ */
#define RNIC_SET_NAPI_WEIGHT_ADJ_MODE(index, val)      (g_stRnicCtx.astIfaceCtx[index].stFeatureCfg.stNapiCfg.enNapiWeightAdjMode = (val))
/* ��������NAPI weightֵ */
#define RNIC_SET_NAPI_WEIGHT(index, val)               (g_stRnicCtx.astIfaceCtx[index].stFeatureCfg.stNapiCfg.ucNapiPollWeight = (val))
/* ��������NAPI Poll���г������ֵ */
#define RNIC_SET_NAPI_POLL_QUE_MAX_LEN(index, val)     (g_stRnicCtx.astIfaceCtx[index].stFeatureCfg.stNapiCfg.usNapiPollQueMaxLen = (val))

/* ��������NAPI����ÿ���հ�������λֵ1 */
#define RNIC_SET_DL_PKT_NUM_PER_SEC_LEVEL1(index, val) (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(index)->stDlPktNumPerSecLevel.ulDlPktNumPerSecLevel1 = (val))
/* ��������NAPI����ÿ���հ�������λֵ2 */
#define RNIC_SET_DL_PKT_NUM_PER_SEC_LEVEL2(index, val) (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(index)->stDlPktNumPerSecLevel.ulDlPktNumPerSecLevel2 = (val))
/* ��������NAPI����ÿ���հ�������λֵ3 */
#define RNIC_SET_DL_PKT_NUM_PER_SEC_LEVEL3(index, val) (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(index)->stDlPktNumPerSecLevel.ulDlPktNumPerSecLevel3 = (val))
/* ��������NAPI����ÿ���հ�������λֵ4 */
#define RNIC_SET_DL_PKT_NUM_PER_SEC_LEVEL4(index, val) (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(index)->stDlPktNumPerSecLevel.ulDlPktNumPerSecLevel4 = (val))

/* ��������NAPI weight��λֵ1 */
#define RNIC_SET_NAPI_WEIGHT_LEVEL1(index, val)        (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(index)->stNapiWeightLevel.ucNapiWeightLevel1 = (val))
/* ��������NAPI weight��λֵ2 */
#define RNIC_SET_NAPI_WEIGHT_LEVEL2(index, val)        (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(index)->stNapiWeightLevel.ucNapiWeightLevel2 = (val))
/* ��������NAPI weight��λֵ3 */
#define RNIC_SET_NAPI_WEIGHT_LEVEL3(index, val)        (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(index)->stNapiWeightLevel.ucNapiWeightLevel3 = (val))
/* ��������NAPI weight��λֵ4 */
#define RNIC_SET_NAPI_WEIGHT_LEVEL4(index, val)        (RNIC_GET_NAPI_DYNAMIC_CFG_ADR(index)->stNapiWeightLevel.ucNapiWeightLevel4 = (val))

/* ��ȡRNIC�����ĵ�ַ */
#define RNIC_GET_RNIC_TIMER_ADR()                      (g_stRnicCtx.astTimerCtx)

#if (defined(CONFIG_BALONG_SPE))
/* ��ȡָ��������SPE���õ�ַ */
#define RNIC_GET_SPE_CFG_ADR(index)                    (&(g_stRnicCtx.astIfaceCtx[index].stFeatureCfg.stSpeCfg))
#endif

/* ���貦�Žڵ���� */
#define RNIC_GET_DIAL_MODE_ADR()                       (&(g_stRnicCtx.stDialMode))
#define RNIC_GET_PROC_DIAL_MODE()                      (g_stRnicCtx.stDialMode.enDialMode)
#define RNIC_SET_PROC_DIAL_MODE(val)                   (g_stRnicCtx.stDialMode.enDialMode = (val))
#define RNIC_GET_PROC_IDLE_TIME()                      (g_stRnicCtx.stDialMode.ulIdleTime)
#define RNIC_SET_PROC_IDLE_TIME(val)                   (g_stRnicCtx.stDialMode.ulIdleTime = (val))
#define RNIC_GET_PROC_EVENT_REPORT()                   (g_stRnicCtx.stDialMode.enEventReportFlag)
#define RNIC_SET_PROC_EVENT_REPORT(val)                (g_stRnicCtx.stDialMode.enEventReportFlag = (val))

/* ��ȡ��ǰ��IPFģʽ */
#define RNIC_GET_IPF_MODE()                            (g_stRnicCtx.ucIpfMode)
/* ��ȡ��λ�ź� */
#define RNIC_GET_RESET_SEM()                           (g_stRnicCtx.hResetSem)

/* ����Ͽ��������� */
#define RNIC_GET_TI_DIALDOWN_EXP_CONT()                (g_stRnicCtx.ulTiDialDownExpCount)
#define RNIC_CLEAR_TI_DIALDOWN_EXP_CONT()              (g_stRnicCtx.ulTiDialDownExpCount = 0)
#define RNIC_ADD_TI_DIALDOWN_EXP_CONT()                (g_stRnicCtx.ulTiDialDownExpCount++)

/* Tethering���� */
#define RNIC_GET_TETHER_ORIG_GRO_FEATURE()             (g_stRnicCtx.stTetherInfo.ucOrigGroEnable)
#define RNIC_SET_TETHER_ORIG_GRO_FEATURE(val)          (g_stRnicCtx.stTetherInfo.ucOrigGroEnable = (val))


/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/


enum RNIC_NETCARD_STATUS_TYPE_ENUM
{
    RNIC_NETCARD_STATUS_CLOSED,                                                 /* RNICΪ�ر�״̬ */
    RNIC_NETCARD_STATUS_OPENED,                                                 /* RNICΪ��״̬ */
    RNIC_NETCARD_STATUS_BUTT
};
typedef VOS_UINT8 RNIC_NETCARD_STATUS_ENUM_UINT8;


enum RNIC_FLOW_CTRL_STATUS_ENUM
{
    RNIC_FLOW_CTRL_STATUS_STOP          = 0x00,                                 /* ����ֹͣ */
    RNIC_FLOW_CTRL_STATUS_START         = 0x01,                                 /* �������� */
    RNIC_FLOW_CTRL_STATUS_BUTT          = 0xFF
};
typedef VOS_UINT32 RNIC_FLOW_CTRL_STATUS_ENUM_UINT32;


enum RNIC_IPF_MODE_ENUM
{
    RNIC_IPF_MODE_INT  = 0x00,                                                   /* �ж������� */
    RNIC_IPF_MODE_THRD = 0x01,                                                   /* �߳������� */
    RNIC_IPF_MODE_BUTT
};
typedef VOS_UINT8 RNIC_IPF_MODE_ENUM_UINT8;


enum RNIC_FEATURE_ENABLE_ENUM
{
    RNIC_FEATURE_OFF   = 0x00,
    RNIC_FEATURE_ON    = 0x01,
    RNIC_FEATURE_BUTT
};
typedef VOS_UINT8 RNIC_FEATURE_ENABLE_ENUM_UINT8;


enum RNIC_DIAL_MODE_ENUM
{
    RNIC_DIAL_MODE_MANUAL,                                                      /*Manual dial mode*/
    RNIC_DIAL_MODE_DEMAND_CONNECT,                                              /*Demand dial mode*/
    RNIC_DIAL_MODE_DEMAND_DISCONNECT,                                           /*Demand dial mode*/
    RNIC_DIAL_MODE_BUTT
};
typedef VOS_UINT32 RNIC_DIAL_MODE_ENUM_UINT32;

enum RNIC_DIAL_EVENT_REPORT_FLAG_ENUM
{
    RNIC_FORBID_EVENT_REPORT            = 0x0000,                               /*����Ӧ���ϱ�*/
    RNIC_ALLOW_EVENT_REPORT             = 0X0001,                               /*�����Ӧ���ϱ�*/

    RNIC_DIAL_EVENT_REPORT_FLAG_BUTT
};
typedef VOS_UINT32 RNIC_DIAL_EVENT_REPORT_FLAG_ENUM_UINT32;


enum RNIC_PS_RAT_TYPE_ENUM
{
    RNIC_PS_RAT_TYPE_3GPP               = 0x00,
    RNIC_PS_RAT_TYPE_IWLAN              = 0x01,
    RNIC_PS_RAT_TYPE_BUTT
};
typedef VOS_UINT8 RNIC_PS_RAT_TYPE_ENUM_UINT8;

/*****************************************************************************
  4 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  6 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  7 STRUCT����
*****************************************************************************/

typedef VOS_VOID (*RNIC_SPE_RX_PUSH_CB)(VOS_VOID);
typedef VOS_INT (*RNIC_SPE_MEM_RECYCLE_CB)(IMM_ZC_STRU *pstImmZc);


/*****************************************************************************
 �ṹ����  : RNIC_DSFLOW_STATS_STRU
 �ṹ˵��  : ����ͳ�ƽṹ
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulCurrentRecvRate;                      /* ��ǰ�������ʣ�����PDP�����1������ʣ�ȥ�������� */
    VOS_UINT32                          ulCurrentSendRate;                      /* ��ǰ�������ʣ�����PDP�����1������ʣ�ȥ�������� */

    VOS_UINT32                          ulPeriodRecvPktNum;                     /* �����յ����ݰ�����,ͳ��һ�����ŶϿ���ʱ���������յ��ĸ�������ʱ����� */
    VOS_UINT32                          ulPeriodSendPktNum;                     /* ���з�����Ч����,ͳ��һ�����ŶϿ���ʱ���������յ��ĸ�������ʱ����� */
} RNIC_DSFLOW_STATS_STRU;


typedef struct
{
    VOS_UINT8                           aucRmnetName[RNIC_RMNET_NAME_MAX_LEN];  /* Rmnet������ */
    AT_RNIC_USB_TETHER_CONN_ENUM_UINT8  enTetherConnStat;                       /* Tethering����״̬ */
    VOS_UINT8                           ucOrigGroEnable;                        /* Tethering����״̬ */
    VOS_UINT8                           aucRsv[6];                              /* ����λ */
} RNIC_TETHER_INFO_STRU;


typedef struct
{
    RNIC_DIAL_MODE_ENUM_UINT32                              enDialMode;         /* Dialģʽ */
    VOS_UINT32                                              ulIdleTime;         /* ��ʱ�����ȣ���λΪ�� */
    RNIC_DIAL_EVENT_REPORT_FLAG_ENUM_UINT32                 enEventReportFlag;  /*�Ƿ��Ӧ���ϱ���ʶ*/
    VOS_UINT32                                              ulReserved;
}RNIC_DIAL_MODE_STRU;


typedef struct
{
    VOS_UINT8                                     ucNapiEnable;                 /* NAPI���� */
    NAPI_WEIGHT_ADJ_MODE_ENUM_UINT8               enNapiWeightAdjMode;          /* NAPI Weight����ģʽ, 0: ��̬ģʽ��1����̬����ģʽ */
    VOS_UINT8                                     ucNapiPollWeight;             /* RNIC����NAPI��ʽһ��poll��������� */
    VOS_UINT8                                     ucGroEnable;                  /* GRO���� */
    VOS_UINT16                                    usNapiPollQueMaxLen;          /* Napi poll�������֧�ֳ��� */
    VOS_UINT8                                     aucReserved[2];               /* ����λ */
    RNIC_NAPI_WEIGHT_DYNAMIC_ADJ_CFG_STRU         stNapiWeightDynamicAdjCfg;    /* Napi Weight��̬�������� */

} RNIC_NAPI_CFG_STRU;

#if (defined(CONFIG_BALONG_SPE))

typedef struct
{
    VOS_UINT32                          ulTdDepth;                              /* TD��� */
    VOS_UINT32                          ulRdDepth;                              /* RD��� */
    RNIC_SPE_RX_PUSH_CB                 pSpeRxPushFunc;                         /* SPE�����ж���ɻص� */
    RNIC_SPE_MEM_RECYCLE_CB             pSpeMemRecylceFunc;                     /* SPE�ڴ���ջص� */

} RNIC_SPE_CFG_STRU;
#endif


typedef struct
{
    RNIC_NAPI_CFG_STRU                  stNapiCfg;                              /* NAPI,GRO�������� */
#if (defined(CONFIG_BALONG_SPE))
    RNIC_SPE_CFG_STRU                   stSpeCfg;                               /* SPE���� */
#endif
}RNIC_FEATURE_CFG_STRU;


typedef struct
{
    VOS_UINT32                          bitOpIpv4Act : 1;
    VOS_UINT32                          bitOpIpv6Act : 1;
    VOS_UINT32                          bitOpSpare   : 30;

    VOS_UINT8                           ucIpv4RabId;
    VOS_UINT8                           ucIpv4ExRabId;                          /* ��չ����:Modem ID + Rab ID */
    VOS_UINT8                           aucReserved1[2];
    VOS_UINT32                          ulIpv4Addr;                             /* IPv4��ַ */

    VOS_UINT8                           ucIpv6RabId;
    VOS_UINT8                           ucIpv6ExRabId;                          /* ��չ����:Modem ID + Rab ID */

    VOS_UINT8                           aucReserved2[2];
    VOS_UINT8                           aucIpv6Addr[RNIC_MAX_IPV6_ADDR_LEN];    /* ��AT������IPV6��ַ���ȣ�������":" */

    MODEM_ID_ENUM_UINT16                enModemId;                              /* ���������ĸ�modem */
    RNIC_PS_RAT_TYPE_ENUM_UINT8         enRatType;                              /* psע���� */
    VOS_UINT8                           aucReserved3[5];

    IMM_ZC_HEAD_STRU                    stImsQue;                               /* IMS�������ݷ��ͻ������ */

}RNIC_PS_IFACE_INFO_STRU;


typedef struct
{
    RNIC_DEV_ID_ENUM_UINT8              enRmNetId;                              /* �����豸��Ӧ������ID */
    PS_IFACE_ID_ENUM_UINT8              enIfaceId;
    VOS_UINT8                           aucReserved[6];
    RNIC_PS_IFACE_INFO_STRU             stPsIfaceInfo;                          /* �����豸��Ӧ��PDN��Ϣ */
    RNIC_FEATURE_CFG_STRU               stFeatureCfg;                           /* RNIC�������� */
    RNIC_DSFLOW_STATS_STRU              stDsFlowStats;                          /* ������Ϣ */

}RNIC_IFACE_CTX_STRU;


typedef struct
{
    /* RNIC�����豸������ */
    RNIC_IFACE_CTX_STRU                 astIfaceCtx[RNIC_NET_ID_MAX_NUM];        /* �����豸��������Ϣ */

    /* RNIC��ʱ�������� */
    RNIC_TIMER_CTX_STRU                 astTimerCtx[RNIC_MAX_TIMER_NUM];

    /* RNIC���������� */
    RNIC_DIAL_MODE_STRU                 stDialMode;                             /* ����ģʽ */
    VOS_UINT32                          ulTiDialDownExpCount;                   /* ���ŶϿ���ʱ����ʱ��������ͳ�� */
    VOS_UINT8                           ucIpfMode;                              /* IPF����ADS�������ݵ�ģʽ, 0: �ж�������(Ĭ��)��1���߳������� */
    VOS_UINT8                           aucRsv[3];
    VOS_SEM                             hResetSem;                              /* �������ź��������ڸ�λ���� */
    RNIC_TETHER_INFO_STRU               stTetherInfo;                           /* Tethering��Ϣ */
    struct wake_lock                    stOnDemandDisconnWakeLock;
}RNIC_CTX_STRU;

/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/

extern RNIC_CTX_STRU                    g_stRnicCtx;

/*****************************************************************************
  10 ��������
*****************************************************************************/
VOS_VOID RNIC_InitCtx(VOS_VOID);
VOS_VOID RNIC_InitPdnInfo(RNIC_PS_IFACE_INFO_STRU *pstPdnInfo);
VOS_VOID RNIC_InitNapiCfg(
    RNIC_DEV_ID_ENUM_UINT8              enRmNetId,
    TAF_NV_RNIC_NAPI_CFG_STRU          *pstNapiCfg
);
VOS_VOID RNIC_InitIpv4PdnInfo(RNIC_PS_IFACE_INFO_STRU *pstPdnInfo);
VOS_VOID RNIC_InitIpv6PdnInfo(RNIC_PS_IFACE_INFO_STRU *pstPdnInfo);
VOS_VOID RNIC_InitOnDemandDialInfo(VOS_VOID);
RNIC_IFACE_CTX_STRU* RNIC_GetNetCntxtByRmNetId(RNIC_DEV_ID_ENUM_UINT8 enRmNetId);
VOS_VOID RNIC_InitResetSem(VOS_VOID);
VOS_VOID RNIC_ReadNapiCfg(
    TAF_NV_RNIC_NAPI_CFG_STRU          *pstNapiCfg
);
VOS_VOID RNIC_CheckNetIfCfgValid(
    TAF_NV_RNIC_NAPI_CFG_STRU          *pstNapiCfg
);
VOS_VOID RNIC_InitTetherInfo(VOS_VOID);

#if (defined(CONFIG_BALONG_SPE))
VOS_VOID RNIC_InitSpeCfg(RNIC_DEV_ID_ENUM_UINT8 enRmNetId);
#endif



#if (VOS_OS_VER != VOS_WIN32)
#pragma pack()
#else
#pragma pack(pop)
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* _RNIC_CTX_H_ */

