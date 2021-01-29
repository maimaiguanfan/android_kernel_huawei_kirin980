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
#include "AtDataProc.h"
#include "ATCmdProc.h"
#include "AtCsdInterface.h"
#include "AtTafAgentInterface.h"
#include "AtCtxPacket.h"



/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_AT_DATAPROC_C

/*****************************************************************************
   2 ȫ�ֱ�������
*****************************************************************************/

/* HiLinkģʽ: ����ģʽ������ģʽ */
AT_HILINK_MODE_ENUM_U8                  g_enHiLinkMode = AT_HILINK_NORMAL_MODE;

/* Э��ջ����PDP��������� */
TAF_PDP_TYPE_ENUM_UINT8                g_enAtNdisActPdpType;

/* Э��ջ����PDP��������� */
TAF_PDP_TYPE_ENUM_UINT8                g_enAtFirstNdisActPdpType;

/* ����NDIS���������õĲ��� */
AT_DIAL_PARAM_STRU                      gstAtNdisAddParam = {0};

/* NDIS PDPʵ�� */
AT_PDP_ENTITY_STRU                      g_stAtNdisDhcpPara;

/* ����NDISCONN �����ʱ�Ĳ���, ����NDISCONN ��ѯ���� */
AT_NDISCONN_PARA_STRU                   g_stAtNdisConnPara;

/* Э��ջ����PDP��������� */
TAF_PDP_TYPE_ENUM_UINT8                 g_enAtAppActPdpType;

/* Э��ջ����PDP��������� */
TAF_PDP_TYPE_ENUM_UINT8                 g_enAtAppFirstActPdpType;

/* ���沦�������õĲ��� */
AT_DIAL_PARAM_STRU                      g_stAtAppDialPara = {0};

/* APP PDPʵ�� */
AT_PDP_ENTITY_STRU                      g_stAtAppPdpEntity;

/* RA��Ϣ����ز��� */
AT_IPV6_RA_INFO_STRU                    g_stAtAppRaInfo = {0};

/* IP*/
VOS_UINT32                              g_ulIpAddr = 0;

/* ����ָ����FC ID��Ӧ��FC Pri */
AT_FCID_MAP_STRU                        g_stFcIdMaptoFcPri[FC_ID_BUTT];

/* ^DCONN�ϱ������� */
AT_PS_REPORT_CONN_RESULT_STRU           g_astAtRptConnectedResultTab[] =
{
    /* ��ϢID */                        /* ��Ϣ������ */
    {AT_APP_USER,                       AT_PS_ReportDCONN}
};

/* ^DEND�ϱ������� */
AT_PS_REPORT_END_RESULT_STRU            g_astAtRptEndedResultTab[] =
{
    /* ��ϢID */                        /* ��Ϣ������ */
    {AT_APP_USER,                       AT_PS_ReportDEND}
};

/* ���ص�ע�ᴦ������ */
AT_PS_REG_FC_POINT_STRU                 g_astAtRegFCPointTab[] =
{
    /* ��ϢID */                        /* ��Ϣ������ */
    {AT_APP_USER,                       AT_PS_RegAppFCPoint}
};

/* ���ص�ȥע�ᴦ������ */
AT_PS_DEREG_FC_POINT_STRU               g_astAtDeRegFCPointTab[] =
{
    /* ��ϢID */                        /* ��Ϣ������ */
    {AT_APP_USER,                       AT_PS_DeRegAppFCPoint}
};

AT_PS_SND_PDP_ACT_IND_STRU              g_astAtSndPdpActIndTab[] =
{
    /* ��ϢID */                        /* ��Ϣ������ */
    {AT_APP_USER,                       AT_PS_SndRnicPdpActInd}
};

AT_PS_SND_PDP_DEACT_IND_STRU            g_astAtSndPdpDeActIndTab[] =
{
    /* ��ϢID */                        /* ��Ϣ������ */
    {AT_APP_USER,                       AT_PS_SndRnicPdpDeactInd}
};

AT_CHDATA_RNIC_RMNET_ID_STRU            g_astAtChdataRnicRmNetIdTab[] =
{
    {AT_CH_DATA_CHANNEL_ID_1, RNIC_DEV_ID_RMNET0, PS_IFACE_ID_RMNET0, {0, 0}},
    {AT_CH_DATA_CHANNEL_ID_2, RNIC_DEV_ID_RMNET1, PS_IFACE_ID_RMNET1, {0, 0}},
    {AT_CH_DATA_CHANNEL_ID_3, RNIC_DEV_ID_RMNET2, PS_IFACE_ID_RMNET2, {0, 0}},
    {AT_CH_DATA_CHANNEL_ID_4, RNIC_DEV_ID_RMNET3, PS_IFACE_ID_RMNET3, {0, 0}},
    {AT_CH_DATA_CHANNEL_ID_5, RNIC_DEV_ID_RMNET4, PS_IFACE_ID_RMNET4, {0, 0}}
    ,
    {AT_CH_DATA_CHANNEL_ID_6, RNIC_DEV_ID_RMNET5, PS_IFACE_ID_RMNET5, {0, 0}},
    {AT_CH_DATA_CHANNEL_ID_7, RNIC_DEV_ID_RMNET6, PS_IFACE_ID_RMNET6, {0, 0}}
};

AT_PS_WLAN_PDN_ACT_ERR_CODE_MAP_STRU           g_astTafPsWlanPdnActErrCodeMapTbl[] =
{
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_PARA_ERROR,              WIFI_IMSA_PDN_ACT_RESULT_PARAM_ERR                            },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_LINKLOST,                WIFI_IMSA_PDN_ACT_RESULT_LINKLOST                             },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_TIMEOUT,                 WIFI_IMSA_PDN_ACT_RESULT_TIMEOUT                              },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_SM_ERROR,                WIFI_IMSA_PDN_ACT_RESULT_MAPCON_STATE_ERR                     },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_OTHER_ERROR,             WIFI_IMSA_PDN_ACT_RESULT_OTHER_FAILURE                        },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_SIM_AUTH_NO_RESP,        WIFI_IMSA_PDN_ACT_RESULT_SIM_AUTH_NO_RESP                     },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_AUTH_NETWORK_ERROR,      WIFI_IMSA_PDN_ACT_RESULT_AUTH_NETWORK_ERROR                   },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_PARSE_NETWORK_PAYLOAD_ERROR,     WIFI_IMSA_PDN_ACT_RESULT_PARSE_NETWORK_PAYLOAD_ERROR  },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_ADD_SA_SP_ERROR,         WIFI_IMSA_PDN_ACT_RESULT_ADD_SA_AP_ERROR                      },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_CLIENT_PAYLOAD_ERROR,    WIFI_IMSA_PDN_ACT_RESULT_CLIENT_PAYLOAD_ERROR                 },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_MSG_NETWORK_OVERTIME_ERROR,      WIFI_IMSA_PDN_ACT_RESULT_MSG_NETWORK_OVERTIME_ERROR   },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_ENC_ALG_ERROR,           WIFI_IMSA_PDN_ACT_RESULT_ENC_ALG_ERROR                        },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_APN_ID_ERROR,            WIFI_IMSA_PDN_ACT_RESULT_APN_ID_ERROR                         },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_NETWORK_AUTH_SIM_ERROR,  WIFI_IMSA_PDN_ACT_RESULT_NETWORK_AUTH_SIM_ERROR               },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_STATUS_ERROR,            WIFI_IMSA_PDN_ACT_RESULT_STATUS_ERROR                         },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_SA_INIT_ERROR,           WIFI_IMSA_PDN_ACT_RESULT_SA_INIT_ERROR                        },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_NON_3GPP_ACCESS_TO_EPC_NOT_ALLOWED,  WIFI_IMSA_PDN_ACT_RESULT_NON_3GPP_ACCESS_TO_EPC_NOT_ALLOWED   },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_USER_UNKNOWN,            WIFI_IMSA_PDN_ACT_RESULT_USER_UNKNOWN                         },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_PLMN_NOT_ALLOWED,        WIFI_IMSA_PDN_ACT_RESULT_PLMN_NOT_ALLOWED                     },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_AUTHORIZATION_REJECTED,  WIFI_IMSA_PDN_ACT_RESULT_AUTHORIZATION_REJECTED               },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_RAT_TYPE_NOT_ALLOWED,    WIFI_IMSA_PDN_ACT_RESULT_RAT_TYPE_NOT_ALLOWED                 },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_NETWORK_FAILURE,         WIFI_IMSA_PDN_ACT_RESULT_NETWORK_FAILURE                      },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_NO_APN_SUBSCRIPTION,     WIFI_IMSA_PDN_ACT_RESULT_NO_APN_SUBSCRIPTION                  },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_PDN_CONNECTION_REJECTION,WIFI_IMSA_PDN_ACT_RESULT_PDN_CONNECTION_REJECTION             },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_CONNECTION_REACHED,      WIFI_IMSA_PDN_ACT_RESULT_CONNECTION_REACHED                   },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_PROTECLE_ERROR,          WIFI_IMSA_PDN_ACT_RESULT_PROTECLE_ERROR                       },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_INTERNAL_ADDRESS_ERROR,  WIFI_IMSA_PDN_ACT_RESULT_INTERNAL_ADDRESS_ERROR               },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_IPV4_ADDRESS_CONFLICT,   WIFI_IMSA_PDN_ACT_RESULT_IPV4_ADDRESS_CONFLICT                },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_CERT_ERROR,              WIFI_IMSA_PDN_ACTIVE_RESULT_CERT_ERROR                        },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_NOT_SUPPORT_CONCURRENCE, WIFI_IMSA_PDN_ACTIVE_RESULT_BEARS_CONCURRENT_NOT_SUPPORT      },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_SHUTDOWN,                WIFI_IMSA_PDN_ACTIVE_RESULT_SHUTDOWN                          },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_DONE_ERROR,              WIFI_IMSA_PDN_ACTIVE_RESULT_DONE_ERROR                        },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_REDIRECT_FAILURE,        WIFI_IMSA_PDN_ACTIVE_RESULT_REDIRECT_FAILURE                  },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_ROAM_FORBID,             WIFI_IMSA_PDN_ACT_RESULT_ROAM_FORBID                          },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_HIFI_STATUS_OVERTIME,    WIFI_IMSA_PDN_ACT_RESULT_HIFI_STATUS_OVERTIME                 },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_IKED_ABNORMAL,           WIFI_IMSA_PDN_ACT_RESULT_IKED_ABNORMAL                        },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_UNKNOWN_NETWORK_ERROR,   WIFI_IMSA_PDN_ACT_RESULT_UNKNOWN_NETWORK_ERROR                },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_SWITCH_CACHE_MAX_REACHED,   WIFI_IMSA_PDN_ACT_RESULT_SWITCH_CACHE_MAX_REACHED          },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_APN_MAX_REACHED,         WIFI_IMSA_PDN_ACT_RESULT_APN_MAX_REACHED                      },
    { TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_REQUEST_REPEATED,        WIFI_IMSA_PDN_ACT_RESULT_REQUEST_REPEATED                     },
};

AT_PS_WLAN_PDN_DEACT_ERR_CODE_MAP_STRU           g_astTafPsWlanPdnDeActErrCodeMapTbl[] =
{
    { TAF_PS_CAUSE_WLAN_PDN_DEACT_RESULT_KEEP_LIVE,             WIFI_IMSA_PDN_DEACT_CAUSE_KEEP_LIVE                           },
    { TAF_PS_CAUSE_WLAN_PDN_DEACT_RESULT_NETWORK_DELETE,        WIFI_IMSA_PDN_DEACT_CAUSE_NETWORK_DELETE                      },
    { TAF_PS_CAUSE_WLAN_PDN_DEACT_RESULT_IKED_RESET,            WIFI_IMSA_PDN_DEACT_CAUSE_IKED_RESET                          },
    { TAF_PS_CAUSE_WLAN_PDN_DEACT_RESULT_REKEY_ERROR,           WIFI_IMSA_PDN_DEACT_CAUSE_REKEY_ERROR                         },
    { TAF_PS_CAUSE_WLAN_PDN_DEACT_RESULT_MAPCONN_TEAR,          WIFI_IMSA_PDN_DEACT_CAUSE_MAPCON_TEAR                         },
    { TAF_PS_CAUSE_WLAN_PDN_DEACT_RESULT_BEARS_CONNCURRENT_NOT_SUPPOER, WIFI_IMSA_PDN_DEACT_CAUSE_BEARS_CONCURRENT_NOT_SUPPORT},
};

/*****************************************************************************
   3 ��������������
*****************************************************************************/

extern const TAF_CHAR                       *g_PppDialRateDisplay[];

extern const AT_DISPLAY_RATE_PAIR_STRU      g_ucLTERateDisplay[AT_UE_LTE_CATEGORY_NUM_MAX];

extern const VOS_CHAR                       *g_ucDialRateDisplayNv[];

/*****************************************************************************
   4 ����ʵ��
*****************************************************************************/

VOS_VOID AT_PS_SetPsCallErrCause(
    VOS_UINT16                          usClientId,
    TAF_PS_CAUSE_ENUM_UINT32            enPsErrCause
)
{
    AT_MODEM_PS_CTX_STRU               *pstPsModemCtx = VOS_NULL_PTR;

    pstPsModemCtx = AT_GetModemPsCtxAddrFromClientId(usClientId);

    pstPsModemCtx->enPsErrCause = enPsErrCause;

    return;
}


TAF_PS_CAUSE_ENUM_UINT32 AT_PS_GetPsCallErrCause(
    VOS_UINT16                          usClientId
)
{
    AT_MODEM_PS_CTX_STRU               *pstPsModemCtx = VOS_NULL_PTR;

    pstPsModemCtx = AT_GetModemPsCtxAddrFromClientId(usClientId);

    return pstPsModemCtx->enPsErrCause;
}

VOS_UINT32 AT_GetLanAddr32(
  VOS_UINT8                            *pucAddr
)
{
    VOS_UINT32                          ulAddr;

    ulAddr = *pucAddr++;
    ulAddr <<= 8;
    ulAddr |= *pucAddr++;
    ulAddr <<= 8;
    ulAddr |= *pucAddr++;
    ulAddr <<= 8;
    ulAddr |= *pucAddr;

    return ulAddr;
}


VOS_VOID AT_GetIpv4AddrFromLanAddr32(
    VOS_UINT32                          ulAddr,
    VOS_UINT8                          *pucAddr
)
{
    *pucAddr++ = (ulAddr & 0xFF000000) >> 24;
    *pucAddr++ = (ulAddr & 0x00FF0000) >> 16;
    *pucAddr++ = (ulAddr & 0x0000FF00) >> 8;
    *pucAddr++ = (ulAddr & 0x000000FF);

    return;
}


VOS_UINT32 AT_DHCPGetIPMask(
    VOS_UINT32                          ulIpAddrHL
)
{
    VOS_UINT32 ulMask;

    if (AT_IS_CLASS_A_IPV4_ADDR(ulIpAddrHL))
    {
        ulMask = AT_IPV4_CLASS_A_SUBNET_MASK;
    }
    else if (AT_IS_CLASS_B_IPV4_ADDR(ulIpAddrHL))
    {
        ulMask = AT_IPV4_CLASS_B_SUBNET_MASK;
    }
    else
    {
        ulMask = AT_IPV4_CLASS_C_SUBNET_MASK;
    }

    return ulMask;
}

VOS_UINT32 AT_DHCPGetGateWay(
    VOS_UINT32                          ulIpAddrHL,
    VOS_UINT32                          ulMaskHL
)
{
    VOS_UINT32 ulGateWay;

    ulGateWay = 0;

    ulGateWay = (ulIpAddrHL & ulMaskHL) + 1;

    while (ulGateWay == ulIpAddrHL)
    {
        ulGateWay ++;
    };

    return ulGateWay;
}


VOS_UINT32 AT_Ipv4AddrAtoi(
    VOS_CHAR                           *pcString,
    VOS_UINT8                          *pucNumber
)
{
    VOS_UINT32                          i = 0;
    VOS_UINT32                          ulStrLen = 0;
    VOS_UINT32                          ulNumLen = 0;
    VOS_UINT32                          ulDotNum = 0;
    VOS_UINT32                          ulValTmp = 0;
    VOS_UINT8                           aucAddr[4] = {0};


    if ( (VOS_NULL_PTR == pcString) || (VOS_NULL_PTR == pucNumber) )
    {
        return VOS_ERR;
    }

    ulStrLen = VOS_StrLen(pcString);

    if ( ulStrLen > VOS_StrLen("255.255.255.255") )
    {
        AT_NORM_LOG("AT_Ipv4AddrAtoi: PCSCF IPV4 address length out of range");
        return VOS_ERR;
    }

    for ( i = 0; i < ulStrLen; i++ )
    {
        if ( (pcString[i] >= '0') && (pcString[i] <= '9') )
        {
            ulValTmp = (ulValTmp * 10) + (pcString[i] - '0');

            ulNumLen++;
            continue;
        }
        else if ( pcString[i] == '.' )
        {
            if ( (0 == ulNumLen) || (ulNumLen > 3) )
            {
                AT_NORM_LOG("AT_Ipv4AddrAtoi: the number between dots is out of range");
                return VOS_ERR;
            }

            if ( ulValTmp > 255 )
            {
                AT_NORM_LOG("AT_Ipv4AddrAtoi: the number is larger than 255");
                return VOS_ERR;
            }

            aucAddr[ulDotNum] = (VOS_UINT8)ulValTmp;

            ulValTmp = 0;
            ulNumLen = 0;

            /* ͳ��'.'�ĸ��� */
            ulDotNum++;

            continue;
        }
        else
        {
            AT_NORM_LOG("AT_Ipv4AddrAtoi: character not number nor dot, return ERROR");
            /* ����ֱֵ�ӷ���ʧ�� */
            return VOS_ERR;
        }
    }

    /* �������3��'.'�򷵻�ʧ�� */
    if ( 3 != ulDotNum )
    {
        AT_NORM_LOG("AT_Ipv4AddrAtoi: dot number is not 3");
        return VOS_ERR;
    }

    /* �������ַ���ȡֵ */
    if (ulValTmp > 255)
    {
        AT_NORM_LOG("AT_Ipv4AddrAtoi: last number is larger than 255");
        return VOS_ERR;
    }

    aucAddr[ulDotNum] = (VOS_UINT8)ulValTmp;

    TAF_MEM_CPY_S(pucNumber, TAF_IPV4_ADDR_LEN, aucAddr, 4);

    return VOS_OK;
}


VOS_UINT32 AT_Ipv4AddrItoa(
    VOS_CHAR                           *pcString,
    VOS_UINT8                          *pucNumber
)
{
    VOS_UINT32                          ret = VOS_OK;

    if ( (VOS_NULL_PTR == pcString) || (VOS_NULL_PTR == pucNumber) )
    {
        ret = VOS_ERR;
    }
    else
    {
        TAF_MEM_SET_S(pcString, TAF_MAX_IPV4_ADDR_STR_LEN, 0x00, TAF_MAX_IPV4_ADDR_STR_LEN);
        VOS_sprintf_s(pcString, TAF_MAX_IPV4_ADDR_STR_LEN, "%d.%d.%d.%d", pucNumber[0], pucNumber[1], pucNumber[2], pucNumber[3]);
    }

    return ret;
}


VOS_UINT32 AT_Ipv4Addr2Str(
    VOS_CHAR                           *pcString,
    VOS_UINT8                          *pucNumber
)
{
    if ( (VOS_NULL_PTR == pcString) || (VOS_NULL_PTR == pucNumber) )
    {
        return VOS_ERR;
    }

    TAF_MEM_SET_S(pcString, TAF_MAX_IPV4_ADDR_STR_LEN, 0x00, TAF_MAX_IPV4_ADDR_STR_LEN);

    /* ��������ַΪ�գ��򷵻ؿ��ַ��� */
    if ((0 == pucNumber[0])
      &&(0 == pucNumber[1])
      &&(0 == pucNumber[2])
      &&(0 == pucNumber[3]))
    {
        return VOS_OK;
    }

    VOS_sprintf_s(pcString, TAF_MAX_IPV4_ADDR_STR_LEN, "%d.%d.%d.%d", pucNumber[0], pucNumber[1], pucNumber[2], pucNumber[3]);

    return VOS_OK;
}


VOS_UINT32 AT_Ipv6AddrAtoi(
    VOS_CHAR                           *pcString,
    VOS_UINT8                          *pucNumber
)
{
    VOS_UINT32                          i = 0;
    VOS_UINT32                          ulStrLen = 0;
    VOS_UINT32                          ulNumLen = 0;
    VOS_UINT32                          ulDotNum = 0;
    VOS_UINT32                          ulValTmp = 0;
    VOS_UINT8                           aucAddr[TAF_IPV6_ADDR_LEN] = {0};


    if ( (VOS_NULL_PTR == pcString) || (VOS_NULL_PTR == pucNumber) )
    {
        return VOS_ERR;
    }

    ulStrLen = VOS_StrLen(pcString);

    if ( ulStrLen > VOS_StrLen("255.255.255.255.255.255.255.255.255.255.255.255.255.255.255.255") )
    {
        return VOS_ERR;
    }

    for ( i = 0; i < ulStrLen; i++ )
    {
        if ( (pcString[i] >= '0') && (pcString[i] <= '9') )
        {
            ulValTmp = (ulValTmp * 10) + (pcString[i] - '0');

            ulNumLen++;
            continue;
        }
        else if ( pcString[i] == '.' )
        {
            if ( (0 == ulNumLen) || (ulNumLen > 3) )
            {
                return VOS_ERR;
            }

            if ( ulValTmp > 255 )
            {
                return VOS_ERR;
            }

            aucAddr[ulDotNum] = (VOS_UINT8)ulValTmp;

            ulValTmp = 0;
            ulNumLen = 0;

            /* ͳ��'.'�ĸ��� */
            ulDotNum++;

            continue;
        }
        else
        {
            /* ����ֱֵ�ӷ���ʧ�� */
            return VOS_ERR;
        }
    }

    /* �������3��'.'�򷵻�ʧ�� */
    if ((TAF_IPV6_ADDR_LEN - 1) != ulDotNum )
    {
        return VOS_ERR;
    }

    /* �������ַ���ȡֵ */
    if (ulValTmp > 255)
    {
        return VOS_ERR;
    }

    aucAddr[ulDotNum] = (VOS_UINT8)ulValTmp;

    TAF_MEM_CPY_S(pucNumber, TAF_IPV6_ADDR_LEN, aucAddr, TAF_IPV6_ADDR_LEN);

    return VOS_OK;
}


VOS_UINT32  AT_LenStr2IpAddr(
    VOS_UINT8                           *pucStr,
    VOS_UINT8                           *pucIpAddr
)
{
    VOS_UINT8 i;
    VOS_UINT8 j;
    VOS_UINT8 ucValue;

    i                = 0;
    j                = 0;
    ucValue          = 0;

    if (0 == pucStr[0])
    {
       /*ADDR����Ϊ0��ֱ�ӳ��ȸ�ֵ����*/
        pucIpAddr[0] = 0;
        pucIpAddr[1] = 0;
        pucIpAddr[2] = 0;
        pucIpAddr[3] = 0;
        return AT_SUCCESS;
    }

    if (pucStr[0] == '.')
    {
        /*�����1����Ч�ַ���'.'��IP��ַ�ǷǷ���*/
        return AT_FAILURE;
    }

    for (i = 0; (i <= AT_AP_MAX_IPV4_ADDR_LEN) && (0 != pucStr[i]) ; i++)
    {
        /*�ӵ�1����Ч�ַ���ʼ���*/
        if (((pucStr[i] < 0x30) || (pucStr[i] > 0x39)) && (pucStr[i] != '.'))
        {
            /*����'0'-'9'���ַ��Ƿ�*/
            return AT_FAILURE;
        }
        if (pucStr[i] != '.')
        {
            /*�������Ч�ַ���ת��Ϊ����*/
            if (((ucValue * 10) + (pucStr[i] - 0x30)) <= 255)
            {
                /*�ַ���ת��Ϊ��ЧIP��λֵ*/
                ucValue = (TAF_UINT8)((ucValue * 10) + (pucStr[i] - 0x30));
            }
            else
            {
                /*����255����*/
                return AT_FAILURE;
            }
        }
        else
        {   /*����ַ���'.'��ǰһλ��ֵ�Ѿ��������*/
            /*����ַ���'.'��ǰһλ��ֵ�Ѿ��������*/
            if (j <= 3)
            {
               /*���汾ֻ֧��IPV4��ַ*/
                pucIpAddr[j] = ucValue;
                ucValue      = 0;

                /*��ʼ��һ����Ч�ַ��εĳ����ۼ�*/
                j++;
            }
            else
            {
                /*����4��IPλ�Σ��Ƿ�*/
                return AT_FAILURE;
            }
        }
    }

    if (j == 3)
    {
        pucIpAddr[j] = ucValue;
        return AT_SUCCESS;
    }
    else
    {
        return AT_FAILURE;
    }
}


VOS_VOID AT_Ipv6LenStrToAddrProc(
    VOS_UINT8                          *pucStr,
    VOS_UINT8                          *pucIpAddr,
    VOS_UINT8                           ucColonCount,
    VOS_UINT8                           ucDotCount
)
{
    VOS_UINT8                           i;
    VOS_UINT8                           j;
    VOS_UINT16                          usValue;            /*Ipv6ʮ������ת����*/
    VOS_UINT8                           ucValue;            /*Ipv4ʮ������ת����*/

    usValue                             = 0;
    ucValue                             = 0;
    j                                   = 0;

    for (i = 0; ((i < TAF_MAX_IPV6_ADDR_COLON_STR_LEN) && ('\0' != pucStr[i])); i++)
    {
        if ((':' != pucStr[i]) && ('.' != pucStr[i]))
        {
            usValue <<= 4;

            if ((pucStr[i] >= '0') &&(pucStr[i] <= '9'))
            {
                /* ʮ���Ƹ�ʽת�� */
                usValue += (pucStr[i] - '0');
                ucValue  = (VOS_UINT8)((ucValue * 10) + (pucStr[i] - 0x30));
            }
            else
            {
                /* ʮ�����Ƹ�ʽת�� */
                 usValue += (pucStr[i] - 'A' + 10);
            }
        }
        else if (':' == pucStr[i])
        {
            /* IPV6ʮ������ȡ�߰�λ���� */
            pucIpAddr[j] = (VOS_UINT8)((usValue >> 8) & 0x00FF);
            j++;
            /* IPV6ʮ������ȡ�Ͱ�λ���� */
            pucIpAddr[j] = (VOS_UINT8)(usValue & 0x00FF);
            j++;
            usValue      = 0;
        }
        else
        {
            /* IPV4ʮ����ת�� */
            pucIpAddr[j] = ucValue;
            j++;
            ucValue      = 0;
        }
    }

    /* ƥ�����һ��ת�� */
    if (AT_MAX_IPV6_STR_COLON_NUM == ucColonCount)
    {
        pucIpAddr[j] = (VOS_UINT8)((usValue >> 8) & 0x00FF);
        j++;
        pucIpAddr[j] = (VOS_UINT8)(usValue & 0x00FF);
        j++;
    }

    if (AT_MAX_IPV6_STR_DOT_NUM == ucDotCount)
    {
        pucIpAddr[j] = ucValue;
        /* j++; */
    }

    return;
}


VOS_VOID  AT_Ipv6LenStrToAddrProcCompressed(
    VOS_UINT8                          *pucStr,
    VOS_UINT8                           ucColonCount,
    VOS_UINT8                           ucDotCount,
    VOS_UINT8                           ucStrlen,
    VOS_UINT8                           ucIdxPos
)
{
    VOS_UINT8                           i;

    /* ���ַ�����ѹ��λ�ÿ�ʼ˳������ƶ� */
    for (i = ucStrlen; i >= ucIdxPos; i--)
    {
        if (AT_MAX_IPV6_STR_DOT_NUM != ucDotCount)
        {
            pucStr[i + AT_MAX_IPV6_STR_COLON_NUM - ucColonCount] =  pucStr[i];
        }
        else
        {
            pucStr[i + AT_MAX_IPV4V6_STR_COLON_NUM - ucColonCount] =  pucStr[i];
        }
    }

    /* ����ѹ����ð�� */
    if (AT_MAX_IPV6_STR_DOT_NUM != ucDotCount)
    {
        for (i = ucIdxPos; i < (ucIdxPos + AT_MAX_IPV6_STR_COLON_NUM - ucColonCount); i++)
        {
            pucStr[i + 1] =  ':';
        }
    }
    else
    {
        for (i = ucIdxPos; i < (ucIdxPos + AT_MAX_IPV4V6_STR_COLON_NUM - ucColonCount); i++)
        {
            pucStr[i + 1] =  ':';
        }
    }

    return;
}


VOS_UINT32 AT_Ipv6LenStrToAddrAccess(
    VOS_UINT8                          *pucStr,
    VOS_UINT8                          *pucColonCount,
    VOS_UINT8                          *pucDotCount,
    VOS_UINT8                          *pucStrlen,
    VOS_UINT8                          *pucIdxPos
)
{
    VOS_UINT32                          i;

    for (i = 0; ((i < TAF_MAX_IPV6_ADDR_COLON_STR_LEN) && ('\0' != pucStr[i])); i++)
    {
        if ( (pucStr[i] < '0' || pucStr[i] > '9')
          && (pucStr[i] < 'A' || pucStr[i] > 'F')
          && ('.' != pucStr[i])
          && (':' != pucStr[i]) )
        {
            return VOS_ERR;
        }
        /* ȡ�ò���ð������λ�� */
        if ((i > 0)
         && (':' == pucStr[i - 1])
         && (':' == pucStr[i]))
        {
            *pucIdxPos = (VOS_UINT8)i;
        }

        /* ͳ��ð�Ÿ��� */
        if (':' == pucStr[i])
        {
            (*pucColonCount)++;
        }

        /*ͳ�Ƶ�Ÿ���*/
        if ('.' == pucStr[i])
        {
            (*pucDotCount)++;
        }
    }

    *pucStrlen = (VOS_UINT8)i;

    return VOS_OK;
}


VOS_UINT32  AT_Ipv6LenStrToAddr(
    VOS_UINT8                          *pucStr,
    VOS_UINT8                          *pucIpAddr
)
{
    VOS_UINT8                           ucColonCount;       /* �ַ�����ð�Ÿ��� */
    VOS_UINT8                           ucDotCount;         /* �ַ����е�Ÿ��� */
    VOS_UINT8                           ucStrlen;           /* �ַ������� */
    VOS_UINT8                           ucIdxPos;           /* ��Ҫ����ð�ŵ�λ�� */
    VOS_UINT8                           pucStrTmp[TAF_MAX_IPV6_ADDR_COLON_STR_LEN];

    TAF_MEM_SET_S(pucStrTmp, sizeof(pucStrTmp), 0x00, TAF_MAX_IPV6_ADDR_COLON_STR_LEN);

    TAF_MEM_CPY_S(pucStrTmp, sizeof(pucStrTmp), pucStr, TAF_MAX_IPV6_ADDR_COLON_STR_LEN);
    ucColonCount                        = 0;
    ucDotCount                          = 0;
    ucStrlen                            = 0;
    ucIdxPos                            = 0;

    /* ����IPV6��ַ�ַ��� */
    if (VOS_OK != AT_Ipv6LenStrToAddrAccess(pucStrTmp, &ucColonCount,
                                           &ucDotCount, &ucStrlen, &ucIdxPos))
    {
        return VOS_ERR;
    }

    /* �ַ���Ϊ�շ���ʧ�� */
    if (ucStrlen == 0)
    {
        return VOS_ERR;
    }

    /* ð�Ÿ�������7���ߵ�Ÿ���������3����ʧ�� */
    if ( (ucColonCount > AT_MAX_IPV6_STR_COLON_NUM)
      || ((AT_MAX_IPV6_STR_DOT_NUM != ucDotCount) && (0 != ucDotCount)) )
    {
        return VOS_ERR;
    }

    if ( (AT_MAX_IPV6_STR_COLON_NUM == ucColonCount)
      || ( (AT_MAX_IPV4V6_STR_COLON_NUM == ucColonCount)
        && (AT_MAX_IPV6_STR_DOT_NUM == ucDotCount) ) )
    {
        /* ��ѹ����ʽ���� */
        AT_Ipv6LenStrToAddrProc(pucStrTmp, pucIpAddr, ucColonCount, ucDotCount);
    }
    else
    {
        /* ѹ����ʽ���� */
        AT_Ipv6LenStrToAddrProcCompressed(pucStrTmp, ucColonCount, ucDotCount, ucStrlen, ucIdxPos);
        if (ucDotCount != AT_MAX_IPV6_STR_DOT_NUM)
        {
            /* ��IPV4ӳ��IPV6��ַ��ʽ */
            AT_Ipv6LenStrToAddrProc(pucStrTmp, pucIpAddr, AT_MAX_IPV6_STR_COLON_NUM, ucDotCount);
        }
        else
        {
            /* IPV4ӳ��IPV6��ַ��ʽ */
            AT_Ipv6LenStrToAddrProc(pucStrTmp, pucIpAddr, AT_MAX_IPV4V6_STR_COLON_NUM, ucDotCount);
        }
    }

    return VOS_OK;
}


VOS_VOID AT_PcscfIpv4Addr2Str(
    VOS_CHAR                           *pcString,
    VOS_UINT8                          *pucNumber
)
{

    TAF_MEM_SET_S(pcString, TAF_MAX_IPV4_ADDR_STR_LEN, 0, TAF_MAX_IPV4_ADDR_STR_LEN);

    VOS_sprintf_s(pcString, TAF_MAX_IPV4_ADDR_STR_LEN, "%d.%d.%d.%d", pucNumber[0], pucNumber[1], pucNumber[2], pucNumber[3]);
}


VOS_UINT32 AT_PcscfIpv6StrToAddr(
    VOS_UINT8                          *pucStr,
    VOS_UINT8                          *pucIpAddr,
    VOS_UINT8                           ucColonCount
)
{
    VOS_UINT8                           i;
    VOS_UINT8                           j;
    VOS_UINT16                          usValue;            /*Ipv6ʮ������ת����*/
    VOS_UINT8                           ucValue;            /*Ipv4ʮ������ת����*/
    VOS_UINT32                          ulNumLen;

    usValue                             = 0;
    ucValue                             = 0;
    j                                   = 0;
    ulNumLen                            = 0;

    if ((VOS_NULL_PTR == pucStr)
     || (VOS_NULL_PTR == pucIpAddr))
    {
        AT_NORM_LOG("AT_PcscfIpv6StrToAddr: input ptr is NULL, return ERROR");
        return VOS_ERR;
    }

    for (i = 0; ((i < TAF_MAX_IPV6_ADDR_COLON_STR_LEN) && ('\0' != pucStr[i])); i++)
    {
        /* ƥ���ַ� */
        if (':' != pucStr[i])
        {
            usValue <<= 4;

            if ((pucStr[i] >= '0') &&(pucStr[i] <= '9'))
            {
                /* ʮ���Ƹ�ʽת�� */
                usValue += (pucStr[i] - '0');
                ucValue  = (VOS_UINT8)((ucValue * 10) + (pucStr[i] - 0x30));
            }
            else
            {
                /* ʮ�����Ƹ�ʽת�� */
                 usValue += ((pucStr[i] - 'A') + 10);
            }

            ulNumLen++;
        }
        /* ƥ�䵽ð�� */
        else
        {
            /* ð��֮����ַ�����4������Ϊ��ʽ���� */
            if (4 < ulNumLen)
            {
                AT_ERR_LOG("AT_PcscfIpv6StrToAddr: the number of char betwwen colons is more than 4, return ERROR");
                return VOS_ERR;
            }

            /* IPV6ʮ������ȡ�߰�λ���� */
            pucIpAddr[j] = (VOS_UINT8)((usValue >> 8) & 0x00FF);
            j++;
            /* IPV6ʮ������ȡ�Ͱ�λ���� */
            pucIpAddr[j] = (VOS_UINT8)(usValue & 0x00FF);
            j++;
            usValue      = 0;
            ulNumLen     = 0;
        }
    }

    /* ƥ�����һ��ת�� */
    if (AT_MAX_IPV6_STR_COLON_NUM == ucColonCount)
    {
        pucIpAddr[j] = (VOS_UINT8)((usValue >> 8) & 0x00FF);
        j++;
        pucIpAddr[j] = (VOS_UINT8)(usValue & 0x00FF);
        j++;
    }

    return VOS_OK;
}


VOS_UINT32  AT_PortAtoI(
    VOS_CHAR                           *pcString,
    VOS_UINT32                         *pulValue
)
{
    VOS_CHAR                           *pucTmp;
    VOS_UINT32                          ulRet;

    if ((VOS_NULL_PTR == pcString)
     || (VOS_NULL_PTR == pulValue))
    {
        AT_ERR_LOG("AT_PortAtoI: pcString or pulValue is NULL, return ERROR");
        return VOS_ERR;
    }

    pucTmp  = pcString;
    ulRet   = 0;

    *pulValue = 0;

    for (pucTmp = pcString; *pucTmp != '\0'; pucTmp++)
    {
        /* ������, �򷵻�ʧ��*/
        if (('0' > *pucTmp) || ('9' < *pucTmp))
        {
            AT_ERR_LOG("AT_PortAtoI: Not all number type in pcString , return ERROR");
            return VOS_ERR;
        }

        ulRet = (ulRet * 10) + (*pucTmp - '0');

        if (IMS_PCSCF_ADDR_PORT_MAX < ulRet)
        {
            AT_ERR_LOG("AT_PortAtoI: Port number is larger than 65535, return ERROR");
            return VOS_ERR;
        }
    }

    if ((0 < ulRet)
     && (IMS_PCSCF_ADDR_PORT_MAX >= ulRet))
    {
        *pulValue = (VOS_UINT32)ulRet;

        return VOS_OK;
    }

    AT_ERR_LOG("AT_PortAtoI: return ERROR");
    return VOS_ERR;
}


VOS_UINT32 AT_CheckPcscfIpv6Addr(
    VOS_CHAR                           *pucIpv6Str,
    VOS_UINT32                         *pulPortExistFlg
)
{
    VOS_CHAR                           *pucIpv6Start = VOS_NULL_PTR;
    VOS_CHAR                           *pucIpv6End   = VOS_NULL_PTR;

    if ((VOS_NULL_PTR == pucIpv6Str)
     || (VOS_NULL_PTR == pulPortExistFlg))
    {
        AT_NORM_LOG("AT_CheckPcscfIpv6Addr: input ptr is NULL, return ERROR");
        return VOS_ERR;
    }

    pucIpv6End   = VOS_StrStr(pucIpv6Str, "]");
    pucIpv6Start = VOS_StrStr(pucIpv6Str, "[");

    if ((VOS_NULL_PTR == pucIpv6End)
     && (VOS_NULL_PTR == pucIpv6Start))
    {
        AT_NORM_LOG("AT_CheckPcscfIpv6Addr: NO [ ] symbol in IPV6 addr, port not exists");
        *pulPortExistFlg = VOS_FALSE;
        return VOS_OK;
    }

    if ((VOS_NULL_PTR != pucIpv6End)
     && (VOS_NULL_PTR != pucIpv6Start)
     && (pucIpv6End > pucIpv6Start))
    {
        if ((pucIpv6End - pucIpv6Start) > TAF_MAX_IPV6_ADDR_COLON_STR_LEN)
        {
            AT_ERR_LOG("AT_CheckPcscfIpv6Addr: length of IPV6 addr in [] is larger than 39, return ERROR");
            return VOS_ERR;
        }

        AT_NORM_LOG("AT_CheckPcscfIpv6Addr: Have both [ ] symbol in IPV6 addr");
        *pulPortExistFlg = VOS_TRUE;

        return VOS_OK;
    }

    AT_ERR_LOG("AT_CheckPcscfIpv6Addr: IPV6 addr format incorrect");
    return VOS_ERR;
}



VOS_UINT32  AT_ParsePortFromPcscfIpv6Addr(
    VOS_UINT8                          *pucStr,
    VOS_UINT8                          *pucIpv6Addr,
    VOS_UINT32                         *pulPortExistFlg,
    VOS_UINT32                         *pulPortNum

)
{
    VOS_CHAR                           *pucIpv6Start = VOS_NULL_PTR;
    VOS_CHAR                           *pucIpv6End   = VOS_NULL_PTR;
    VOS_CHAR                           *pucIpv6Str   = VOS_NULL_PTR;
    VOS_CHAR                           *pcStrPort    = VOS_NULL_PTR;

    if ((VOS_NULL_PTR == pucStr)
     || (VOS_NULL_PTR == pucIpv6Addr)
     || (VOS_NULL_PTR == pulPortExistFlg)
     || (VOS_NULL_PTR == pulPortNum))
    {
        AT_ERR_LOG("AT_ParsePortFromPcscfIpv6Addr: input ptr is NULL, return ERROR");
        return VOS_ERR;
    }

    if (VOS_OK != AT_CheckPcscfIpv6Addr((VOS_CHAR*)pucStr, pulPortExistFlg))
    {
        AT_ERR_LOG("AT_ParsePortFromPcscfIpv6Addr: AT_CheckPcscfIpv6Addr FAIL, return ERROR");
        return VOS_ERR;
    }

    /* [ipv6]:port��ʽ���ݴ�����ȡipv6�˿ں� */
    pucIpv6Str   = (VOS_CHAR*)pucStr;
    pucIpv6End   = VOS_StrStr(pucIpv6Str, "]");
    pucIpv6Start = VOS_StrStr(pucIpv6Str, "[");

    if (VOS_TRUE == *pulPortExistFlg)
    {
        if ( (VOS_NULL_PTR == pucIpv6End)
          || (VOS_NULL_PTR == pucIpv6Start))
        {
            AT_ERR_LOG("AT_ParsePortFromPcscfIpv6Addr: NO [ ] symbol in IPV6 addr, return ERROR");
            return VOS_ERR;
        }

        /* �����������ڵ�IPV6��ַ��Ϣ���Ƴ��˿ں� */
        TAF_MEM_CPY_S(pucIpv6Addr, TAF_MAX_IPV6_ADDR_COLON_STR_LEN, pucIpv6Start + 1, (VOS_UINT32)((pucIpv6End - pucIpv6Start) - 1));

        /* ��¼�ָ���ַ�Ͷ˿ڵ�ð�ŵ�ַ */
        pcStrPort = VOS_StrStr(pucIpv6End, ":");

        /* [ipv6]:port��ʽû��ð�ţ�����ERROR*/
        if (VOS_NULL_PTR == pcStrPort)
        {
            *pulPortExistFlg = VOS_FALSE;
            AT_ERR_LOG("AT_ParsePortFromPcscfIpv6Addr: IPV6 Port colon missed, return ERROR ");
            return VOS_ERR;
        }

        /* ð��ǰ�治��"]"������ERROR */
        if (pcStrPort != pucIpv6End + 1)
        {
            *pulPortExistFlg = VOS_FALSE;
            AT_ERR_LOG("AT_ParsePortFromPcscfIpv6Addr: IPV6 Port colon location incorrect, return ERROR ");
            return VOS_ERR;
        }

        /* [ipv6]:port��ʽ�˿ڽ������󣬷���ERROR*/
        if (VOS_OK != AT_PortAtoI(pcStrPort+1, pulPortNum))
        {
            *pulPortExistFlg = VOS_FALSE;
            AT_ERR_LOG("AT_ParsePortFromPcscfIpv6Addr: IPV6 Port decode ERROR");
            return VOS_ERR;
        }

        *pulPortExistFlg = VOS_TRUE;
    }
    else
    {
        /* û�ж˿ںţ�������ַ���ֻ����IPV6��ַ */
        TAF_MEM_CPY_S(pucIpv6Addr, TAF_MAX_IPV6_ADDR_COLON_STR_LEN, pucStr, TAF_MAX_IPV6_ADDR_COLON_STR_LEN);
    }

    return VOS_OK;
}


VOS_UINT32  AT_ParseAddrFromPcscfIpv6Addr(
    VOS_UINT8                          *pucStr,
    VOS_UINT8                          *pucIpAddr
)
{
    VOS_UINT8                           ucColonCount;       /* �ַ�����ð�Ÿ��� */
    VOS_UINT8                           ucDotCount;         /* �ַ����е�Ÿ��� */
    VOS_UINT8                           ucStrlen;           /* �ַ������� */
    VOS_UINT8                           ucIdxPos;           /* ��Ҫ����ð�ŵ�λ�� */
    VOS_UINT32                          ulResult;

    ucColonCount = 0;
    ucDotCount   = 0;
    ucStrlen     = 0;
    ucIdxPos     = 0xFF;

    if ((VOS_NULL_PTR == pucStr)
     || (VOS_NULL_PTR == pucIpAddr))
    {
        AT_NORM_LOG("AT_ParseAddrFromPcscfIpv6Addr: input ptr is NULL, return ERROR");
        return VOS_ERR;
    }

    /* ����IPV6��ַ�ַ��� */
    if (VOS_OK != AT_Ipv6LenStrToAddrAccess(pucStr, &ucColonCount, &ucDotCount, &ucStrlen, &ucIdxPos))
    {
        AT_ERR_LOG("AT_ParseAddrFromPcscfIpv6Addr: AT_Ipv6LenStrToAddrAccess FAIL, return ERROR");
        return VOS_ERR;
    }

    /* ��֧��IPV4IPV6����͵ĸ�ʽ */
    if (0 != ucDotCount)
    {
        AT_ERR_LOG("AT_ParseAddrFromPcscfIpv6Addr: There have dot symbol in address format, return ERROR");
        return VOS_ERR;
    }

    /* �ַ���Ϊ�շ���ʧ�� */
    if (0 == ucStrlen)
    {
        AT_ERR_LOG("AT_ParseAddrFromPcscfIpv6Addr: IP address length is 0, return ERROR");
        return VOS_ERR;
    }

    /* ð�Ÿ�������7�򷵻�ʧ�� */
    if (ucColonCount > AT_MAX_IPV6_STR_COLON_NUM)
    {
        AT_ERR_LOG("AT_ParseAddrFromPcscfIpv6Addr: IPV6 address Colon number is larger than 7, return ERROR");
        return VOS_ERR;
    }

    if (AT_MAX_IPV6_STR_COLON_NUM == ucColonCount)
    {
        /* ��ѹ����ʽ���� */
        ulResult = AT_PcscfIpv6StrToAddr(pucStr, pucIpAddr, ucColonCount);
    }
    else
    {
        if (0xFF != ucIdxPos)
        {
            /* ѹ����ʽ���� */
            AT_Ipv6LenStrToAddrProcCompressed(pucStr, ucColonCount, ucDotCount, ucStrlen, ucIdxPos);
            /* ӳ��IPV6��ַ��ʽ */
            ulResult = AT_PcscfIpv6StrToAddr(pucStr, pucIpAddr, AT_MAX_IPV6_STR_COLON_NUM);
        }
        /* ѹ��IPV6��ַ���Ҳ����������ڵ�ð�ţ���ʽ���� */
        else
        {
            AT_ERR_LOG("AT_ParseAddrFromPcscfIpv6Addr: Can not find two consecutive colons in compressed IPV6 address , return ERROR");
            return VOS_ERR;
        }
    }

    return ulResult;
}


VOS_UINT32  AT_Ipv6PcscfDataToAddr(
    VOS_UINT8                          *pucStr,
    VOS_UINT8                          *pucIpAddr,
    VOS_UINT32                         *pulPortExistFlg,
    VOS_UINT32                         *pulPortNum

)
{
    VOS_UINT8                           pucStrTmp[TAF_MAX_IPV6_ADDR_COLON_STR_LEN];

    TAF_MEM_SET_S(pucStrTmp, TAF_MAX_IPV6_ADDR_COLON_STR_LEN, 0, TAF_MAX_IPV6_ADDR_COLON_STR_LEN);

    if ((VOS_NULL_PTR == pucStr)
     || (VOS_NULL_PTR == pucIpAddr)
     || (VOS_NULL_PTR == pulPortExistFlg)
     || (VOS_NULL_PTR == pulPortNum))
    {
        AT_NORM_LOG("AT_Ipv6PcscfDataToAddr: input ptr is NULL, return ERROR");
        return VOS_ERR;
    }

    if (VOS_OK != AT_ParsePortFromPcscfIpv6Addr(pucStr, pucStrTmp, pulPortExistFlg, pulPortNum))
    {
        AT_ERR_LOG("AT_Ipv6PcscfDataToAddr: AT_ParsePortFromPcscfIpv6Addr FAIL, return ERROR");
        return VOS_ERR;
    }

    /* ��IPV6��ַ��ʽת��Ϊ��д */
    VOS_StrToUpper((VOS_CHAR*)pucStrTmp);

    if (VOS_OK != AT_ParseAddrFromPcscfIpv6Addr(pucStrTmp, pucIpAddr))
    {
        AT_ERR_LOG("AT_Ipv6PcscfDataToAddr: AT_ParseAddrFromPcscfIpv6Addr FAIL, return ERROR");
        return VOS_ERR;
    }

    return VOS_OK;
}



VOS_UINT32 AT_Ipv6AddrToStr(
    VOS_UINT8                           aucAddrStr[],
    VOS_UINT8                           aucIpAddr[],
    AT_IPV6_STR_TYPE_ENUM_UINT8         enIpStrType
)
{
    VOS_UINT8                          *pucBuffer;
    VOS_UINT16                          ausAddrValue[AT_IPV6_STR_MAX_TOKENS];
    VOS_UINT16                          usAddrNum;
    VOS_UINT8                           ucDelimiter;
    VOS_UINT8                           ucTokensNum;
    VOS_UINT8                           ucRadix;
    VOS_UINT32                          i;

    pucBuffer                           = aucAddrStr;

    TAF_MEM_SET_S(ausAddrValue, sizeof(ausAddrValue), 0x00, sizeof(ausAddrValue));

    /* ����IP�ַ�����ʽ�������, ���ö�Ӧ��ת������ */
    switch (enIpStrType)
    {
        case AT_IPV6_STR_TYPE_HEX:
            ucDelimiter = AT_IPV6_STR_DELIMITER;
            ucTokensNum = 8;
            ucRadix     = 16;
            break;

        case AT_IPV6_STR_TYPE_DEC:
            ucDelimiter = AT_IPV4_STR_DELIMITER;
            ucTokensNum = 16;
            ucRadix     = 10;
            break;

        default:
            return VOS_ERR;
    }

    /* ����IP�ַ�����ʽ�������, ��ȡ�ֶε�IP��ַ��ֵ */
    for (i = 0; i < ucTokensNum; i++)
    {
        usAddrNum = *aucIpAddr++;

        if (AT_IPV6_STR_TYPE_HEX == enIpStrType)
        {
            usAddrNum <<= 8;
            usAddrNum  |= *aucIpAddr++;
        }

        ausAddrValue[i] = usAddrNum;
    }

    /* ����IP��ַ�ֶ�, �����зָ�����ǵ�IP��ַ�ַ��� */
    for (i=0; i < ucTokensNum; i++)
    {
        pucBuffer    = (VOS_UINT8*)AT_Itoa(ausAddrValue[i],
                                               (VOS_CHAR*)pucBuffer,
                                               ucRadix,
                                               (TAF_MAX_IPV6_ADDR_DOT_STR_LEN - (VOS_UINT32)(aucAddrStr - pucBuffer)));
        *pucBuffer++ = ucDelimiter;
    }

    /* ȡ�����һ���ָ���, �����ַ��������� */
    if (aucAddrStr != pucBuffer)
    {
        *(--pucBuffer) = '\0';
    }

    return VOS_OK;
}


VOS_UINT32 AT_Ipv6Addr2DecString(
    VOS_CHAR                           *pcIpv6FormatStr,
    VOS_UINT8                           aucIpv6Addr[]
)
{
    VOS_UINT32                          ulLength;
    VOS_UINT32                          ulLoop;
    VOS_INT                             iRslt;

    ulLength = 0;

    /* ѭ����ӡ10���Ƶ��IPv6��ַ */
    for (ulLoop = 0; ulLoop < AT_IPV6_ADDR_DEC_TOKEN_NUM; ulLoop++)
    {
        /* ��ӡ��ָ��� */
        if (0 != ulLoop)
        {
            *(pcIpv6FormatStr + ulLength) = AT_IP_STR_DOT_DELIMITER;
            ulLength ++;
        }

        iRslt = VOS_sprintf_s(pcIpv6FormatStr + ulLength,
                              AT_IPV6_ADDR_DEC_FORMAT_STR_LEN - ulLength,
                              "%d",
                              aucIpv6Addr[ulLoop]);

        if (iRslt <= 0)
        {
            AT_WARN_LOG("AT_Ipv6Addr2DecString: Print IPv6 Addr Failed!");
            return 0;
        }

        ulLength += iRslt;
    }

    return ulLength;
}


VOS_VOID AT_ConvertIpv6AddrToHexAddrAndGetMaxZeroCnt(
    VOS_UINT8                           aucIpv6Addr[],
    VOS_UINT16                          ausIpv6HexAddr[],
    VOS_UINT32                         *pulZeroStartIndex,
    VOS_UINT32                         *pulZeroMaxCnt
)
{
    VOS_UINT32                          ulLoop;
    VOS_UINT32                          ulZeroTmpIndex;
    VOS_UINT32                          ulZeroTmpCnt;

    ulZeroTmpIndex      = 0;
    ulZeroTmpCnt        = 0;

    /* ѭ��ת��IPv6��ʽ��ַ����ͳ���������� */
    for (ulLoop = 0; ulLoop < AT_IPV6_ADDR_HEX_TOKEN_NUM; ulLoop++)
    {
        /* �ϲ��ֽ� */
        ausIpv6HexAddr[ulLoop] = *(aucIpv6Addr + ulLoop + ulLoop);
        ausIpv6HexAddr[ulLoop] <<= 8;
        ausIpv6HexAddr[ulLoop] |= *(aucIpv6Addr + ulLoop + ulLoop + 1);

        if (0 == ausIpv6HexAddr[ulLoop])
        {
            /* ���16�����ֶ�Ϊ0�����¼��ʱcnt��Index */
            if (0 == ulZeroTmpCnt)
            {
                ulZeroTmpIndex = ulLoop;
            }

            ulZeroTmpCnt++;
        }
        else
        {
            /* ���16�����ֶβ�Ϊ0�����ж��Ƿ������������¼ */
            if (ulZeroTmpCnt > *pulZeroMaxCnt)
            {
                *pulZeroStartIndex    = ulZeroTmpIndex;
                *pulZeroMaxCnt        = ulZeroTmpCnt;
            }

            ulZeroTmpCnt    = 0;
            ulZeroTmpIndex  = 0;
        }
    }


    /* �ж��Ƿ������������¼ */
    if (ulZeroTmpCnt > *pulZeroMaxCnt)
    {
        *pulZeroStartIndex    = ulZeroTmpIndex;
        *pulZeroMaxCnt        = ulZeroTmpCnt;
    }

    return;
}


VOS_UINT32 AT_Ipv6Addr2HexString(
    VOS_CHAR                           *pcIpv6FormatStr,
    VOS_UINT8                           aucIpv6Addr[]
)
{
    VOS_UINT32                          ulLength;
    AT_COMM_PS_CTX_STRU                *pstCommPsCtx        = VOS_NULL_PTR;
    VOS_CHAR                           *pcFormatStr         = VOS_NULL_PTR;
    VOS_UINT32                          ulLoop;
    VOS_UINT16                          ausIpv6HexAddr[AT_IPV6_ADDR_HEX_TOKEN_NUM];
    VOS_UINT32                          ulZeroStartIndex;
    VOS_UINT32                          ulZeroMaxCnt;
    VOS_INT                             iRslt;

    /* �ֲ�������ʼ�� */
    ulLength            = 0;
    pstCommPsCtx        = AT_GetCommPsCtxAddr();
    ulZeroStartIndex    = 0;
    ulZeroMaxCnt        = 0;
    TAF_MEM_SET_S(ausIpv6HexAddr, sizeof(ausIpv6HexAddr), 0, sizeof(ausIpv6HexAddr));

    /* �����Ƿ���ǰ����ȷ�ϴ�ӡ��ʽ */
    pcFormatStr = (VOS_FALSE == pstCommPsCtx->bitOpIpv6LeadingZeros) ? "%04X" : "%X";

    /* ת��IPv6��ַΪ16Bit HEX���ͣ���ͳ�������������� */
    AT_ConvertIpv6AddrToHexAddrAndGetMaxZeroCnt(aucIpv6Addr, ausIpv6HexAddr, &ulZeroStartIndex, &ulZeroMaxCnt);

    /* ѭ����ӡ16���Ƶ��IPv6��ַ */
    for (ulLoop = 0; ulLoop < AT_IPV6_ADDR_HEX_TOKEN_NUM; ulLoop++)
    {
        /* ������ѹ�����ܣ��Ҵ�����������㣬��ѹ����ӡ */
        if ( (VOS_FALSE != pstCommPsCtx->bitOpIpv6CompressZeros)
          && (ulZeroMaxCnt > 0) )
        {
            /* ��һ��0����ӡð�� */
            if (ulZeroStartIndex == ulLoop)
            {
                *(pcIpv6FormatStr + ulLength) = AT_IP_STR_COLON_DELIMITER;
                ulLength ++;
                continue;
            }

            /* ����0����ӡ */
            if ( (ulLoop > ulZeroStartIndex)
              && (ulLoop < (ulZeroStartIndex + ulZeroMaxCnt)) )
            {
                /* ���һλΪ0����Ҫ���ӡһ��ð�� */
                if (ulLoop == (AT_IPV6_ADDR_HEX_TOKEN_NUM - 1))
                {
                    *(pcIpv6FormatStr + ulLength) = AT_IP_STR_COLON_DELIMITER;
                    ulLength ++;
                }

                continue;
            }
        }

        /* ��ӡð�ŷָ��� */
        if (0 != ulLoop)
        {
            *(pcIpv6FormatStr + ulLength) = AT_IP_STR_COLON_DELIMITER;
            ulLength ++;
        }

        iRslt = VOS_sprintf_s(pcIpv6FormatStr + ulLength,
                              AT_IPV6_ADDR_COLON_FORMAT_STR_LEN - ulLength,
                              pcFormatStr,
                              ausIpv6HexAddr[ulLoop]);

        if (iRslt <= 0)
        {
            AT_WARN_LOG("AT_Ipv6Addr2HexString: Print IPv6 Addr Failed!");
            return 0;
        }

        ulLength += iRslt;
    }

    return ulLength;
}


VOS_UINT32 AT_Ipv6AddrMask2FormatString(
    VOS_CHAR                           *pcIpv6FormatStr,
    VOS_UINT8                           aucIpv6Addr[],
    VOS_UINT8                           aucIpv6Mask[]
)
{
    VOS_UINT32                          ulLength;
    AT_COMM_PS_CTX_STRU                *pstCommPsCtx        = VOS_NULL_PTR;
    VOS_INT                             iRslt;

    /* �ֲ�������ʼ�� */
    ulLength        = 0;
    pstCommPsCtx    = AT_GetCommPsCtxAddr();
    iRslt           = 0;

    /* ����Ҫ��IPv6��ַ����������ӡIPv6�������� */
    if (VOS_NULL_PTR == aucIpv6Addr)
    {
        AT_WARN_LOG("AT_Ipv6AddrMask2FormatString: No IPv6 Address!");
        *pcIpv6FormatStr = '\0';
        return 0;
    }

    if (VOS_FALSE == pstCommPsCtx->bitOpIpv6AddrFormat)
    {
        /* 10���Ƶ�ָ�ʽ��ӡIPv6��ַ */
        ulLength = AT_Ipv6Addr2DecString(pcIpv6FormatStr, aucIpv6Addr);

        /* ��ӡ�������� */
        if (VOS_NULL_PTR != aucIpv6Mask)
        {
            /* ʹ�õ�ָ�IP��ַ���������� */
            *(pcIpv6FormatStr + ulLength) = AT_IP_STR_DOT_DELIMITER;
            ulLength ++;

            /* 10���Ƶ�ָ�ʽ��ӡIPv6�������� */
            ulLength += AT_Ipv6Addr2DecString(pcIpv6FormatStr + ulLength, aucIpv6Mask);
        }
    }
    else
    {
        /* 16����ð�ŷָ���ʽ��ӡIPv6��ַ */
        ulLength = AT_Ipv6Addr2HexString(pcIpv6FormatStr, aucIpv6Addr);

        /* ��ӡ�������� */
        if (VOS_NULL_PTR != aucIpv6Mask)
        {
            /* �ж����������ʽ */
            if (VOS_FALSE == pstCommPsCtx->bitOpIpv6SubnetNotation)
            {
                /* �������������ַ��IPv6��ַͨ���ո�ָ� */
                /* ʹ�õ�ָ�IP��ַ���������� */
                *(pcIpv6FormatStr + ulLength) = ' ';
                ulLength ++;

                /* 10���Ƶ�ָ�ʽ��ӡIPv6�������� */
                ulLength += AT_Ipv6Addr2HexString(pcIpv6FormatStr + ulLength, aucIpv6Mask);
            }
            else
            {
                /* б�߷ָ�����ǰ׺��������IPv6��ַ */
                /* ʹ��б�߷ָ�IP��ַ���������� */
                *(pcIpv6FormatStr + ulLength) = '/';
                ulLength ++;

                iRslt = VOS_sprintf_s(pcIpv6FormatStr + ulLength,
                                      AT_IPV6_ADDR_MASK_FORMAT_STR_LEN - ulLength,
                                      "%d",
                                      AT_CalcIpv6PrefixLength(aucIpv6Mask));
                if (iRslt <= 0)
                {
                    AT_WARN_LOG("AT_Ipv6AddrMask2FormatString: Print IPv6 Subnet Failed!");
                    *pcIpv6FormatStr = '\0';
                    return 0;
                }

                ulLength += iRslt;
            }
        }
    }

    /* ��󲹳��ַ��������� */
    *(pcIpv6FormatStr + ulLength) = '\0';

    return ulLength;
}


VOS_UINT8 AT_CalcIpv6PrefixLength(VOS_UINT8 *pucLocalIpv6Mask)
{
    VOS_UINT32                          i                   = 0;
    VOS_UINT8                           j                   = 0;
    VOS_UINT8                           ucMaskLength        = 0;

    if ( VOS_NULL_PTR == pucLocalIpv6Mask )
    {
        return ucMaskLength;
    }

    for(i = 0; i < AT_IPV6_STR_MAX_TOKENS; i++)
    {
        if (0xFF == *(pucLocalIpv6Mask + i))
        {
            ucMaskLength = ucMaskLength + 8;
        }
        else
        {
            for (j = 0; j < 8; j++)
            {
                if (0 != ((*(pucLocalIpv6Mask + i)) & ((VOS_UINT32)1 << (7 - j))))
                {
                    ucMaskLength ++;
                }
                else
                {
                    break;
                }
            }
            break;
        }
    }

    return ucMaskLength;
}


VOS_VOID AT_GetIpv6MaskByPrefixLength(VOS_UINT8 ucLocalIpv6Prefix, VOS_UINT8 *pucLocalIpv6Mask)
{
    VOS_UINT8                           ucNum                   = 0;
    VOS_UINT8                           i                       = 0;

    ucNum = ucLocalIpv6Prefix/8;

    if ( VOS_NULL_PTR == pucLocalIpv6Mask )
    {
        return;
    }

    for (i = 0; i < ucNum; i ++)
    {
        *(pucLocalIpv6Mask + i) = 0xFF;
    }

    ucNum = ucLocalIpv6Prefix % 8;

    if (0 != ucNum)
    {
        *(pucLocalIpv6Mask + i) = 0xFF & ((VOS_UINT32)0xFF << (8 - ucNum));

    }

    return;
}


VOS_CHAR* AT_Itoa(
    VOS_UINT16                          usValue,
    VOS_CHAR                           *pcStr,
    VOS_UINT16                          usRadix,
    VOS_UINT32                          ulLength
)
{
    if (usRadix == 16)
    {
        pcStr += VOS_sprintf_s(pcStr, ulLength, "%x", usValue);
    }
    else if(usRadix == 10)
    {
        pcStr += VOS_sprintf_s(pcStr, ulLength, "%d", usValue);
    }
    else
    {
    }
    return pcStr;
}


VOS_UINT32 AT_FindIpv6AddrZeroFieldsToBeCompressed(
    VOS_UINT8                          *pucZeroFieldStart,
    VOS_UINT8                          *pucZeroFieldCount,
    VOS_UINT16                          ausAddrValue[],
    VOS_UINT8                           ucTokensNum
)
{
    VOS_UINT8                           ucStart;
    VOS_UINT8                           ucCount;
    VOS_UINT8                           i;

    ucStart                             = 0xFF;
    ucCount                             = 0;

    for (i = 0; i < ucTokensNum - 1; i++)
    {
        if ( (0x0000 == ausAddrValue[i])
          && (0x0000 == ausAddrValue[i + 1]) )
        {
            /* ��¼��ֵ����Ϊ0��IP��ַ����ʼλ�� */
            if (0xFF == ucStart)
            {
                ucStart = i;
            }

            /* ������ֵ����Ϊ0��IP��ַ�θ��� */
            ucCount++;
        }
        else
        {
            /* ���´�ѹ����IP��ַ��λ��, �Լ�IP��ַ�θ��� */
            if (0xFF != ucStart)
            {
                if (ucCount > *pucZeroFieldCount)
                {
                    *pucZeroFieldStart = ucStart;
                    *pucZeroFieldCount = ucCount;
                }

                ucStart = 0xFF;
                ucCount = 0;
            }
        }
    }

    /* ��ֵ����Ϊ0��IP��ַ���ڽ�βʱ, ��Ҫ����һ�δ�ѹ����IP��ַ��λ��,
       �Լ�IP��ַ�θ��� */
    if (0xFF != ucStart)
    {
        if (ucCount > *pucZeroFieldCount)
        {
            *pucZeroFieldStart = ucStart;
            *pucZeroFieldCount = ucCount;
        }
    }

    return VOS_OK;
}


VOS_UINT32 AT_ConvertIpv6AddrToCompressedStr(
    VOS_UINT8                           aucAddrStr[],
    VOS_UINT8                           aucIpAddr[],
    VOS_UINT8                           ucTokensNum
)
{
    VOS_UINT8                          *pucBuffer;
    VOS_UINT16                          ausAddrValue[8]; /*TAF_IPV6_STR_RFC2373_TOKENS]; */
    VOS_UINT16                          usAddrNum;
    VOS_UINT8                           ucDelimiter;
    VOS_UINT8                           ucRadix;
    VOS_UINT8                           ucZeroFieldStart;
    VOS_UINT8                           ucZeroFieldCount;
    VOS_UINT8                           i;

    TAF_MEM_SET_S(ausAddrValue, sizeof(ausAddrValue), 0x00, sizeof(ausAddrValue));

    pucBuffer                           = aucAddrStr;
    ucDelimiter                         = TAF_IPV6_STR_DELIMITER;
    ucRadix                             = 16;
    ucZeroFieldStart                    = 0xFF;
    ucZeroFieldCount                    = 0;

    /* ����IP�ַ�����ʽ�������, ��ȡ�ֶε�IP��ַ��ֵ */
    for (i = 0; i < ucTokensNum; i++)
    {
        usAddrNum = *aucIpAddr++;

        usAddrNum <<= 8;
        usAddrNum  |= *aucIpAddr++;

        ausAddrValue[i] = usAddrNum;
    }

    /* �ҳ���Ҫʹ��"::"��ʾ��IP��ַ�ε���ʼλ��  */
    AT_FindIpv6AddrZeroFieldsToBeCompressed(&ucZeroFieldStart,
                                            &ucZeroFieldCount,
                                            ausAddrValue,
                                            ucTokensNum);

    /* ����IP��ַ�ֶ�, �����зָ�����ǵ�IP��ַ�ַ��� */
    for (i=0; i < ucTokensNum; i++)
    {
        if (ucZeroFieldStart == i)
        {
            *pucBuffer++ = ucDelimiter;

            i += ucZeroFieldCount;

            /* ����ѵ�IP��ַ�ֶε����һ��, ��Ҫ����ָ��� */
            if ((ucTokensNum - 1) == i)
            {
                *pucBuffer++ = ucDelimiter;
            }
        }
        else
        {
            /* �����IP��ַ�ֶεĵ�һ��, ����Ҫ����ָ��� */
            if (0 != i)
            {
                *pucBuffer++ = ucDelimiter;
            }
            pucBuffer    = (VOS_UINT8*)AT_Itoa(ausAddrValue[i],
                                               (VOS_CHAR*)pucBuffer,
                                               ucRadix,
                                               (TAF_MAX_IPV6_ADDR_COLON_STR_LEN - (VOS_UINT32)(aucAddrStr - pucBuffer)));
        }
    }

    /* �����ַ��������� */
    if (aucAddrStr != pucBuffer)
    {
        *pucBuffer = '\0';
    }

    return VOS_OK;
}


VOS_UINT16 AT_CalcIpHdrCRC16(
    VOS_UINT8                          *pucData,
    VOS_UINT16                          usSize
)
{
    VOS_UINT16                         *pusBuffer;
    VOS_UINT32                          ulCheckSum;

    pusBuffer    = (VOS_UINT16 *)pucData;
    ulCheckSum = 0;

    while (usSize > 1)
    {
        ulCheckSum += *pusBuffer++;
        usSize     -= sizeof(VOS_UINT16);
    }

    if (usSize)
    {
        ulCheckSum += *(VOS_UINT8 *)pusBuffer;
    }

    ulCheckSum  = (ulCheckSum >> 16) + (ulCheckSum & 0xffff);
    ulCheckSum += (ulCheckSum >> 16);

    return (VOS_UINT16)(~ulCheckSum);
}


VOS_UINT32 AT_BuildUdpHdr(
    AT_UDP_PACKET_FORMAT_STRU          *pstUdpPkt,
    VOS_UINT16                          usLen,
    VOS_UINT32                          ulSrcAddr,
    VOS_UINT32                          ulDstAddr,
    VOS_UINT16                          usSrcPort,
    VOS_UINT16                          usDstPort
)
{
    static VOS_UINT16                   usIdentification = 0;

    /* ���ָ��Ϸ��� */
    if (VOS_NULL_PTR == pstUdpPkt)
    {
        return VOS_ERR;
    }

    /* ��дIPͷ */
    pstUdpPkt->stIpHdr.ucIpVer          = AT_IP_VERSION;
    pstUdpPkt->stIpHdr.ucIpHdrLen       = 5;
    pstUdpPkt->stIpHdr.ucServiceType    = 0x00;
    pstUdpPkt->stIpHdr.usTotalLen       = VOS_HTONS(usLen + AT_IP_HDR_LEN + AT_UDP_HDR_LEN);
    pstUdpPkt->stIpHdr.usIdentification = VOS_HTONS(usIdentification);
    pstUdpPkt->stIpHdr.usOffset         = 0;
    pstUdpPkt->stIpHdr.ucTTL            = AT_IP_DEF_TTL;
    pstUdpPkt->stIpHdr.ucProtocol       = AT_IPPROTO_UDP;
    pstUdpPkt->stIpHdr.ulSrcAddr        = VOS_HTONL(ulSrcAddr);
    pstUdpPkt->stIpHdr.ulDstAddr        = VOS_HTONL(ulDstAddr);
    pstUdpPkt->stIpHdr.usCheckSum       = AT_CalcIpHdrCRC16((VOS_UINT8 *)&pstUdpPkt->stIpHdr, AT_IP_HDR_LEN);

    /* ��дUDPͷ */
    pstUdpPkt->stUdpHdr.usSrcPort       = VOS_HTONS(usSrcPort);
    pstUdpPkt->stUdpHdr.usDstPort       = VOS_HTONS(usDstPort);
    pstUdpPkt->stUdpHdr.usLen           = VOS_HTONS(usLen + AT_UDP_HDR_LEN);
    pstUdpPkt->stUdpHdr.usChecksum      = 0;

    usIdentification++;

    return VOS_OK;
}


VOS_UINT32 AT_DHCPCheckCfg(
    AT_DHCP_SETUP_PARAM_ST             *ptrDHCPCfg
)
{
    if ( (0 == ptrDHCPCfg->ulIPAddr)
      || (0 == (0xffffffffU - ptrDHCPCfg->ulIPAddr)) )
    {
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32  AT_DHCPServerSetUp(
    AT_DHCP_SETUP_PARAM_ST              *ptrDHCPParam,
    AT_DHCP_CONFIG_STRU                 *ptrDHCPConfig
)
{
    if (VOS_ERR == AT_DHCPCheckCfg(ptrDHCPParam))
    {
        AT_ERR_LOG("AT_DHCPServerSetUp, ERROR, DHCP Config IP Address is Error!" );
        return VOS_ERR;
    }

      /*�������롢����*/
    ptrDHCPConfig->ulIPAddr     = ptrDHCPParam->ulIPAddr;
    ptrDHCPConfig->ulSubNetMask =
      AT_DHCPGetIPMask(ptrDHCPParam->ulIPAddr);
    ptrDHCPConfig->ulGateWay    =
      AT_DHCPGetGateWay(ptrDHCPParam->ulIPAddr,ptrDHCPConfig->ulSubNetMask);
    ptrDHCPConfig->ulPrimDNS    = ptrDHCPParam->ulPrimDNS;
    ptrDHCPConfig->ulSndDNS     = ptrDHCPParam->ulSndDNS;

    return VOS_OK;
}


VOS_VOID AT_GetDhcpPara(
    AT_DHCP_PARA_STRU                  *pstConfig,
    AT_IPV4_DHCP_PARAM_STRU            *pstIpv4Dhcp
)
{
    /* ��DHCP����ת��Ϊ������ */
    pstConfig->stDhcpCfg.ulIPAddr     = VOS_HTONL(pstIpv4Dhcp->ulIpv4Addr);
    pstConfig->stDhcpCfg.ulSubNetMask = VOS_HTONL(pstIpv4Dhcp->ulIpv4NetMask);
    pstConfig->stDhcpCfg.ulGateWay    = VOS_HTONL(pstIpv4Dhcp->ulIpv4GateWay);
    pstConfig->stDhcpCfg.ulPrimDNS    = VOS_HTONL(pstIpv4Dhcp->ulIpv4PrimDNS);
    pstConfig->stDhcpCfg.ulSndDNS     = VOS_HTONL(pstIpv4Dhcp->ulIpv4SecDNS);

    return;
}


VOS_UINT64  AT_AtoI(
    VOS_UINT8                         *pString
)
{
    VOS_UINT8   *pucTmp;
    VOS_UINT64   ullRet;

    pucTmp  = pString;
    ullRet  = 0;

    for (pucTmp = pString ; *pucTmp != '\0' ; pucTmp++)
    {
        /* ������,�򲻴���*/
        if ((*pucTmp < '0') || (*pucTmp > '9'))
        {
            continue;
        }

        ullRet = (ullRet * 10) + (*pucTmp - '0');
    }

    return ullRet;
}


VOS_INT32  AT_AtoInt(
    VOS_UINT8                          *pString,
    VOS_INT32                          *pOut
)
{
    VOS_UINT8                          *pucTmp;
    VOS_INT32                           lFlag;     /* negative flag */

    pucTmp  = pString;
    lFlag   = 0;

    if ('-' == *pucTmp)
    {
        lFlag = VOS_TRUE;
        pucTmp ++;
    }

    for (; *pucTmp != '\0' ; pucTmp++)
    {
        /* ������, ֱ�ӷ��ش��� */
        if ((*pucTmp < '0') || (*pucTmp > '9'))
        {
            return VOS_ERR;
        }

        *pOut = (*pOut * 10) + (*pucTmp - '0');
    }

    if (VOS_TRUE == lFlag)
    {
        *pOut = (0 - (*pOut));
    }

    return VOS_OK;
}


VOS_UINT32 AT_GetDisplayRate(
    VOS_UINT16                          usClientId,
    AT_DISPLAY_RATE_STRU               *pstSpeed
)
{
      if (VOS_ERR == Taf_GetDisplayRate(usClientId, pstSpeed))
      {
          AT_ERR_LOG("AT_GetDisplayRate : ERROR : at get rate Error!");
          return VOS_ERR;
      }
      pstSpeed->ucDlSpeed[AT_AP_SPEED_STRLEN] = '\0';
      pstSpeed->ucUlSpeed[AT_AP_SPEED_STRLEN] = '\0';

      return VOS_OK;
}


VOS_VOID AT_CtrlConnIndProc(
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent,
    AT_USER_TYPE                        ucUserType
)
{
    AT_DHCP_SETUP_PARAM_ST              stParam;
    AT_DHCP_CONFIG_STRU                 stConfig;

    TAF_MEM_SET_S(&stParam, sizeof(stParam), 0x00, sizeof(AT_DHCP_SETUP_PARAM_ST));
    TAF_MEM_SET_S(&stConfig, sizeof(stConfig), 0x00, sizeof(AT_DHCP_CONFIG_STRU));

    /* ���PS�¼���DNS��ַ�����¼DNS��ַ*/
    if (pstEvent->stDns.bitOpPrimDnsAddr)
    {
        stParam.ulPrimDNS = AT_GetLanAddr32(pstEvent->stDns.aucPrimDnsAddr);
    }

    if (pstEvent->stDns.bitOpSecDnsAddr)
    {
        stParam.ulSndDNS = AT_GetLanAddr32(pstEvent->stDns.aucSecDnsAddr);
    }

    /* ��¼IP��ַ*/
    stParam.ulIPAddr = AT_GetLanAddr32(pstEvent->stPdpAddr.aucIpv4Addr);

    if (VOS_ERR == AT_DHCPServerSetUp(&stParam, &stConfig))
    {
        AT_ERR_LOG("AT_CtrlConnIndProc : ERROR AT_DHCPServerSetUp Error!" );

        return;
    }
    else
    {
        if (AT_NDIS_USER == ucUserType)
        {
            AT_NdisAddrProc(&stConfig, pstEvent);
        }

        if (AT_APP_USER == ucUserType)
        {
            AT_AppPdpAddrProc(&g_stAtAppPdpEntity, &stConfig, pstEvent);
        }

        /* ɾ��AT_USBCOM_USER�����֧ */
    }

    return;
}


VOS_UINT32 Taf_GetDisplayRate(
    VOS_UINT16                          usClientId,
    AT_DISPLAY_RATE_STRU               *pstSpeed
)
{
    PPP_RATE_DISPLAY_ENUM_UINT32        enRateDisplay;
    AT_DISPLAY_RATE_STRU                stDialRateTmp;
    TAF_AGENT_SYS_MODE_STRU             stSysMode;
    VOS_UINT32                          ulRet;
    VOS_UINT32                          ulDlSpeedLen      = 0;
    VOS_UINT32                          ulUlSpeedLen      = 0;
    VOS_UINT32                          ulNvDialRateIndex;
    VOS_UINT8                           ucSubSysMode;
    VOS_UINT8                           ucDlCategoryIndex = 0;
    VOS_UINT8                           ucUlCategoryIndex = 0;

    /* ������ʼ�� */
    TAF_MEM_SET_S(&stSysMode, sizeof(stSysMode), 0x00, sizeof(stSysMode));
    TAF_MEM_SET_S(&stDialRateTmp, sizeof(AT_DISPLAY_RATE_STRU), 0x00, sizeof(AT_DISPLAY_RATE_STRU));

    /* ��C�˻�ȡucRatType��ucSysSubMode */
    ulRet = TAF_AGENT_GetSysMode(usClientId, &stSysMode);

    if (VOS_OK != ulRet)
    {
        stSysMode.enRatType     = TAF_PH_INFO_GSM_RAT;
        stSysMode.enSysSubMode  = TAF_SYS_SUBMODE_BUTT;
    }

    ucSubSysMode = stSysMode.enSysSubMode;
    switch(ucSubSysMode)
    {
        case TAF_SYS_SUBMODE_GSM:
            ulNvDialRateIndex =    g_stDialConnectDisplayRate.ucGprsConnectRate;
            break;

        case TAF_SYS_SUBMODE_GPRS:
            ulNvDialRateIndex =    g_stDialConnectDisplayRate.ucGprsConnectRate;

            break;

        case TAF_SYS_SUBMODE_EDGE:
            ulNvDialRateIndex =    g_stDialConnectDisplayRate.ucEdgeConnectRate;
            break;

        case TAF_SYS_SUBMODE_WCDMA:
            ulNvDialRateIndex =    g_stDialConnectDisplayRate.ucWcdmaConnectRate;
            break;

        case TAF_SYS_SUBMODE_HSDPA:
        case TAF_SYS_SUBMODE_HSDPA_HSUPA:
            ulNvDialRateIndex =    g_stDialConnectDisplayRate.ucDpaConnectRate;
            break;

        default:
            ulNvDialRateIndex = 0;
            break;
    }

    if ((0 == ulNvDialRateIndex) || (ulNvDialRateIndex > 6))
    {
        if ( (TAF_PH_INFO_WCDMA_RAT == stSysMode.enRatType)
          || (TAF_PH_INFO_TD_SCDMA_RAT == stSysMode.enRatType) )
        {
            enRateDisplay = AT_GetRateDisplayIndexForWcdma(&g_stAtDlRateCategory);

            ulDlSpeedLen = VOS_StrLen((TAF_CHAR *)g_PppDialRateDisplay[enRateDisplay]);
            ulUlSpeedLen = VOS_StrLen((TAF_CHAR *)g_PppDialRateDisplay[enRateDisplay]);
            TAF_MEM_CPY_S(stDialRateTmp.ucDlSpeed, (VOS_SIZE_T)sizeof(stDialRateTmp.ucDlSpeed), g_PppDialRateDisplay[enRateDisplay], ulDlSpeedLen);
            TAF_MEM_CPY_S(stDialRateTmp.ucUlSpeed, (VOS_SIZE_T)sizeof(stDialRateTmp.ucUlSpeed), g_PppDialRateDisplay[enRateDisplay], ulUlSpeedLen);
        }
        else if (TAF_PH_INFO_LTE_RAT == stSysMode.enRatType)
        {
            ucDlCategoryIndex = AT_GetLteUeDlCategoryIndex();
            ucUlCategoryIndex = AT_GetLteUeUlCategoryIndex();

            ulDlSpeedLen = VOS_StrLen((TAF_CHAR *)(g_ucLTERateDisplay[ucDlCategoryIndex].acStrDlSpeed));
            TAF_MEM_CPY_S(stDialRateTmp.ucDlSpeed, (VOS_SIZE_T)sizeof(stDialRateTmp.ucDlSpeed), g_ucLTERateDisplay[ucDlCategoryIndex].acStrDlSpeed, ulDlSpeedLen);
            ulUlSpeedLen = VOS_StrLen((TAF_CHAR *)(g_ucLTERateDisplay[ucUlCategoryIndex].acStrUlSpeed));
            TAF_MEM_CPY_S(stDialRateTmp.ucUlSpeed, (VOS_SIZE_T)sizeof(stDialRateTmp.ucUlSpeed), g_ucLTERateDisplay[ucUlCategoryIndex].acStrUlSpeed, ulUlSpeedLen);
        }
        else
        {
            enRateDisplay = AT_GetRateDisplayIndexForGsm(&stSysMode);

            ulDlSpeedLen = VOS_StrLen((TAF_CHAR *)g_PppDialRateDisplay[enRateDisplay]);
            ulUlSpeedLen = VOS_StrLen((TAF_CHAR *)g_PppDialRateDisplay[enRateDisplay]);
            TAF_MEM_CPY_S(stDialRateTmp.ucDlSpeed, (VOS_SIZE_T)sizeof(stDialRateTmp.ucDlSpeed), g_PppDialRateDisplay[enRateDisplay], ulDlSpeedLen);
            TAF_MEM_CPY_S(stDialRateTmp.ucUlSpeed, (VOS_SIZE_T)sizeof(stDialRateTmp.ucUlSpeed), g_PppDialRateDisplay[enRateDisplay], ulUlSpeedLen);
        }
    }
    else
    {
        ulDlSpeedLen = VOS_StrLen((TAF_CHAR *)g_ucDialRateDisplayNv[ulNvDialRateIndex - 1]);
        ulUlSpeedLen = VOS_StrLen((TAF_CHAR *)g_ucDialRateDisplayNv[ulNvDialRateIndex - 1]);
        TAF_MEM_CPY_S(stDialRateTmp.ucDlSpeed, (VOS_SIZE_T)sizeof(stDialRateTmp.ucDlSpeed), g_ucDialRateDisplayNv[ulNvDialRateIndex - 1], ulDlSpeedLen);
        TAF_MEM_CPY_S(stDialRateTmp.ucUlSpeed, (VOS_SIZE_T)sizeof(stDialRateTmp.ucUlSpeed), g_ucDialRateDisplayNv[ulNvDialRateIndex - 1], ulUlSpeedLen);
    }

    /*��CONNECT����������Ϣ*/
    TAF_MEM_CPY_S(pstSpeed->ucDlSpeed, AT_AP_SPEED_STRLEN + 1, stDialRateTmp.ucDlSpeed, (VOS_UINT16)ulDlSpeedLen);
    TAF_MEM_CPY_S(pstSpeed->ucUlSpeed, AT_AP_SPEED_STRLEN + 1, stDialRateTmp.ucUlSpeed, (VOS_UINT16)ulUlSpeedLen);
    pstSpeed->ucDlSpeed[ulDlSpeedLen] = '\0';
    pstSpeed->ucUlSpeed[ulUlSpeedLen] = '\0';

    return VOS_OK;
}


VOS_VOID AT_GetPdpContextFromAtDialParam(
    TAF_PDP_PRIM_CONTEXT_EXT_STRU      *pstPdpPrimContextExt,
    AT_DIAL_PARAM_STRU                 *pstDialParam
)
{
    VOS_UINT32                          ulRet;

    TAF_MEM_SET_S((VOS_VOID *)pstPdpPrimContextExt, sizeof(TAF_PDP_PRIM_CONTEXT_EXT_STRU), 0x00, sizeof(TAF_PDP_PRIM_CONTEXT_EXT_STRU));

    pstPdpPrimContextExt->bitOpPdpType      = VOS_TRUE;
    pstPdpPrimContextExt->bitOpPdpDcomp     = VOS_FALSE;
    pstPdpPrimContextExt->bitOpPdpHcomp     = VOS_FALSE;
    pstPdpPrimContextExt->enPdpType         = pstDialParam->enPdpType;
    pstPdpPrimContextExt->ucCid             = pstDialParam->ucCid;

    /* ��ȡAPN */
    if (0 != pstDialParam->ucAPNLen)
    {
        pstPdpPrimContextExt->bitOpApn      = VOS_TRUE;
        TAF_MEM_CPY_S((VOS_CHAR*)pstPdpPrimContextExt->aucApn,
                   sizeof(pstPdpPrimContextExt->aucApn),
                   (VOS_CHAR*)pstDialParam->aucAPN,
                   pstDialParam->ucAPNLen);
    }

    /* ��ȡ��̬IP��ַ��AT������PDP���ͣ�APS��Ҫ���ǣ���IPV6������̬IP��ַ */
    if (VOS_TRUE == pstDialParam->ulIPv4ValidFlag )
    {
        ulRet = AT_LenStr2IpAddr(pstDialParam->aucIPv4Addr,pstPdpPrimContextExt->stPdpAddr.aucIpv4Addr);
        if (AT_SUCCESS == ulRet )
        {
            pstPdpPrimContextExt->bitOpPdpAddr        = VOS_TRUE;
            pstPdpPrimContextExt->stPdpAddr.enPdpType = TAF_PDP_IPV4;
        }
    }
}


VOS_VOID AT_GetPsDialParamFromAtDialParam(
    TAF_PS_DIAL_PARA_STRU              *pstPsDialParam,
    AT_DIAL_PARAM_STRU                 *pstDialParam
)
{
    TAF_MEM_SET_S((VOS_VOID *)pstPsDialParam, sizeof(TAF_PS_DIAL_PARA_STRU), 0x00, sizeof(TAF_PS_DIAL_PARA_STRU));

    pstPsDialParam->enPdpType       = pstDialParam->enPdpType;
    pstPsDialParam->ucCid           = pstDialParam->ucCid;

    /* ��ȡAPN */
    if (0 != pstDialParam->ucAPNLen)
    {
        pstPsDialParam->bitOpApn        = VOS_TRUE;
        TAF_MEM_CPY_S((VOS_CHAR*)pstPsDialParam->aucApn,
                   sizeof(pstPsDialParam->aucApn),
                   (VOS_CHAR*)pstDialParam->aucAPN,
                   pstDialParam->ucAPNLen);
    }

    /* ������֤��Ϣ */
    pstPsDialParam->bitOpAuthType   = VOS_TRUE;
    pstPsDialParam->enAuthType      = (PPP_AUTH_TYPE_ENUM_UINT8)pstDialParam->usAuthType;

    if (0 != pstDialParam->usPasswordLen)
    {
        pstPsDialParam->bitOpPassWord = VOS_TRUE;
        TAF_MEM_CPY_S((VOS_VOID *)pstPsDialParam->aucPassWord,
                   sizeof(pstPsDialParam->aucPassWord),
                   (VOS_VOID *)pstDialParam->aucPassword,
                   pstDialParam->usPasswordLen);
    }
    else
    {
        pstPsDialParam->bitOpPassWord = VOS_FALSE;
    }

    if (0 != pstDialParam->usUsernameLen)
    {
        pstPsDialParam->bitOpUserName = VOS_TRUE;
        TAF_MEM_CPY_S((VOS_VOID *)pstPsDialParam->aucUserName,
                   sizeof(pstPsDialParam->aucUserName),
                   (VOS_VOID *)pstDialParam->aucUsername,
                   pstDialParam->usUsernameLen);
    }
    else
    {
        pstPsDialParam->bitOpUserName = VOS_FALSE;
    }

    pstPsDialParam->ucBitRatType   = pstDialParam->ucBitRatType;

}



VOS_UINT32 AT_GetFcPriFromMap(
    FC_ID_ENUM_UINT8                   enFcId,
    AT_FCID_MAP_STRU                  *pstFcIdMap
)
{
    if (enFcId >= FC_ID_BUTT)
    {
        return VOS_ERR;
    }

    if ((FC_ID_MODEM == enFcId)
     || (FC_ID_NIC_1 == enFcId)
     || (FC_ID_NIC_2 == enFcId)
     || (FC_ID_NIC_3 == enFcId)
     || (FC_ID_NIC_4 == enFcId)
     || (FC_ID_NIC_5 == enFcId)
     || (FC_ID_NIC_6 == enFcId)
     || (FC_ID_NIC_7 == enFcId)
     || (FC_ID_DIPC_1 == enFcId)
     || (FC_ID_DIPC_2 == enFcId)
     || (FC_ID_DIPC_3 == enFcId))
    {
        pstFcIdMap->ulUsed  = g_stFcIdMaptoFcPri[enFcId].ulUsed;
        pstFcIdMap->enFcPri = g_stFcIdMaptoFcPri[enFcId].enFcPri;

        return VOS_OK;
    }

    return VOS_ERR;
}





VOS_VOID AT_NotifyFcWhenPdpModify(
    TAF_PS_CALL_PDP_MODIFY_CNF_STRU    *pstEvent,
    FC_ID_ENUM_UINT8                    enFcId
)
{
    FC_PRI_ENUM_UINT8                   enFCPriCurrent;
    AT_FCID_MAP_STRU                    stFCPriOrg;

    if (VOS_OK == AT_GetFcPriFromMap(enFcId, &stFCPriOrg))
    {
        if (TAF_USED == pstEvent->bitOpUmtsQos)
        {
            enFCPriCurrent = AT_GetFCPriFromQos(&pstEvent->stUmtsQos);
        }
        else
        {
            enFCPriCurrent = FC_PRI_FOR_PDN_NONGBR;
        }

        if ( (VOS_TRUE == stFCPriOrg.ulUsed)
           && (enFCPriCurrent > stFCPriOrg.enFcPri))
        {
            /* ���ݷ���QOS���ı����ص�����ȼ�*/
            AT_ChangeFCPoint(&pstEvent->stCtrl,enFCPriCurrent,enFcId);
        }
    }

    return;
}



/***************************************************************************
               ���´���ʵ��NDIS��ع���
*****************************************************************************/


VOS_UINT8* AT_PutNetworkAddr32(
  VOS_UINT8                            *pucAddr,
  VOS_UINT32                            ulAddr
)
{
    *pucAddr = (ulAddr >> 24);
    pucAddr++;

    *pucAddr = ((ulAddr >> 16) & 0xFF);
    pucAddr++;

    *pucAddr = ((ulAddr >> 8) & 0xFF);
    pucAddr++;

    *pucAddr = (ulAddr & 0xFF);
    pucAddr++;

    return pucAddr;
}


VOS_UINT32 AT_CovertArrayToU32(
  VOS_UINT8                            *pucAddr
)
{
    VOS_UINT32                          ulAddr;

    pucAddr += 4;

    pucAddr--;
    ulAddr = *(pucAddr);

    ulAddr <<= 8;
    pucAddr--;
    ulAddr |= *(pucAddr);

    ulAddr <<= 8;
    pucAddr--;
    ulAddr |= *(pucAddr);

    ulAddr <<= 8;
    pucAddr--;
    ulAddr |= *(pucAddr);

    return ulAddr;
}


VOS_UINT32 AT_Get3gppSmCauseByPsCause(
    TAF_PS_CAUSE_ENUM_UINT32            enCause
)
{
    VOS_UINT32                          ul3gppSmCause;

    if ( (enCause >= TAF_PS_CAUSE_SM_NW_SECTION_BEGIN)
      && (enCause <= TAF_PS_CAUSE_SM_NW_SECTION_END) )
    {
        ul3gppSmCause = enCause - TAF_PS_CAUSE_SM_NW_SECTION_BEGIN;
    }
    /* E5�����翨��PDP DEACTIVEʱ�ϱ������36��ԭ��ֵ */
    else if (TAF_PS_CAUSE_SUCCESS == enCause)
    {
        ul3gppSmCause = TAF_PS_CAUSE_SM_NW_REGULAR_DEACTIVATION - TAF_PS_CAUSE_SM_NW_SECTION_BEGIN;
    }
    else
    {
        ul3gppSmCause = AT_NDISSTAT_ERR_UNKNOWN;
    }

    return ul3gppSmCause;
}


VOS_VOID AT_SaveNdisConnDailPara(
    AT_PARSE_PARA_TYPE_STRU            *pstParaList
)
{
    AT_NDISCONN_PARA_STRU               *pstNdisDailPara;
    VOS_UINT16                          usStrLen;

    pstNdisDailPara = AT_GetNdisConnParaAddr();

    TAF_MEM_SET_S(pstNdisDailPara, sizeof(AT_NDISCONN_PARA_STRU), 0x00, sizeof(AT_NDISCONN_PARA_STRU));

    pstNdisDailPara->ulConnectState = pstParaList[AT_NDIS_CONN_IDX].ulParaValue;
    pstNdisDailPara->ucCID = (VOS_UINT8)pstParaList[AT_NDIS_CID_IDX].ulParaValue;

    usStrLen = pstParaList[AT_NDIS_APN_IDX].usParaLen;
    if ( usStrLen > TAF_MAX_APN_LEN )
    {
        usStrLen = TAF_MAX_APN_LEN;
    }
    TAF_MEM_CPY_S((VOS_CHAR*)pstNdisDailPara->aucAPN,
               sizeof(pstNdisDailPara->aucAPN),
               (VOS_CHAR*)pstParaList[AT_NDIS_APN_IDX].aucPara, usStrLen);

    usStrLen = pstParaList[AT_NDIS_NAME_IDX].usParaLen;
    if ( usStrLen > TAF_MAX_AUTHDATA_USERNAME_LEN )
    {
        usStrLen = TAF_MAX_AUTHDATA_USERNAME_LEN;
    }
    TAF_MEM_CPY_S((VOS_CHAR*)pstNdisDailPara->aucUsername,
               sizeof(pstNdisDailPara->aucUsername),
               (VOS_CHAR*)pstParaList[AT_NDIS_NAME_IDX].aucPara, usStrLen);

    usStrLen = pstParaList[AT_NDIS_PWD_IDX].usParaLen;
    if ( usStrLen > TAF_MAX_AUTHDATA_PASSWORD_LEN )
    {
        usStrLen = TAF_MAX_AUTHDATA_PASSWORD_LEN;
    }
    TAF_MEM_CPY_S((VOS_CHAR*)pstNdisDailPara->aucPassword,
               sizeof(pstNdisDailPara->aucPassword),
               (VOS_CHAR*)pstParaList[AT_NDIS_PWD_IDX].aucPara, usStrLen);

    pstNdisDailPara->usAuthType = AT_CtrlGetPDPAuthType(pstParaList[AT_NDIS_AUTH_IDX].ulParaValue,
                                                        pstParaList[AT_NDIS_AUTH_IDX].usParaLen);
}


VOS_UINT32 AT_SendNdisRelReq(
    TAF_PS_CALL_PDP_DEACTIVATE_CNF_STRU        *pstEvent
)
{
    AT_NDIS_PDNINFO_REL_REQ_STRU       *pstNdisRelReq   = VOS_NULL_PTR;
    MODEM_ID_ENUM_UINT16                enModemId;
    VOS_UINT32                          ulRet;

    enModemId                           = MODEM_ID_0;

    ulRet = AT_GetModemIdFromClient(AT_NDIS_GET_USR_PORT_INDEX(), &enModemId);

    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_SendNdisRelReq:Get Modem Id fail");
        return VOS_ERR;
    }

    /* ����AT_NDIS_PDNINFO_REL_REQ_STRU��Ϣ */
    /*lint -save -e516 */
    pstNdisRelReq = (AT_NDIS_PDNINFO_REL_REQ_STRU *)AT_ALLOC_MSG_WITH_HDR(
                            sizeof(AT_NDIS_PDNINFO_REL_REQ_STRU));
    /*lint -restore */
    if (VOS_NULL_PTR == pstNdisRelReq)
    {
        AT_WARN_LOG("AT_SendNdisIPv4PdnInfoCfgReq: alloc msg fail!");
        return VOS_ERR;
    }

    /* ��ʼ����Ϣ */
    TAF_MEM_SET_S(AT_GET_MSG_ENTITY(pstNdisRelReq), AT_GET_MSG_LENGTH(pstNdisRelReq),
                  0x00, AT_GET_MSG_LENGTH(pstNdisRelReq));

    /* ��д��Ϣͷ */
    AT_CFG_NDIS_MSG_HDR(pstNdisRelReq, ID_AT_NDIS_PDNINFO_REL_REQ);

    /* ��д��Ϣ�� */
    pstNdisRelReq->enModemId = enModemId;
    pstNdisRelReq->ucRabId   = pstEvent->ucRabId;

    /* ������Ϣ */
    if (VOS_OK != PS_SEND_MSG(WUEPS_PID_AT, pstNdisRelReq))
    {
        AT_WARN_LOG("AT_SendNdisRelReq: Send msg fail!");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_ActiveUsbNet(VOS_VOID)
{
    VOS_UINT32                          ulLinkstus;
    VOS_INT32                           lRtn;
    NCM_IOCTL_CONNECTION_SPEED_S        stNcmConnectSpeed;
    AT_DISPLAY_RATE_STRU                stSpeed;

    TAF_MEM_SET_S(&stSpeed, sizeof(stSpeed), 0x00, (VOS_SIZE_T)(sizeof(AT_DISPLAY_RATE_STRU)));

    if (VOS_OK != AT_GetDisplayRate(AT_CLIENT_ID_NDIS, &stSpeed))
    {
        AT_ERR_LOG("AT_ActiveUsbNet : ERROR : AT_GetDisplayRate Error!");
    }
    /* ������ʳ���U32�ķ�Χ��ȡ���ֵ0xffffffff */
    stNcmConnectSpeed.u32DownBitRate   = (AT_AtoI(stSpeed.ucDlSpeed) >= 0xffffffff) ? 0xffffffff : (VOS_UINT32)AT_AtoI(stSpeed.ucDlSpeed);
    stNcmConnectSpeed.u32UpBitRate     = (AT_AtoI(stSpeed.ucUlSpeed) >= 0xffffffff) ? 0xffffffff : (VOS_UINT32)AT_AtoI(stSpeed.ucUlSpeed);

    lRtn        = mdrv_udi_ioctl(g_ulAtUdiNdisHdl, NCM_IOCTL_CONNECTION_SPEED_CHANGE_NOTIF, (VOS_VOID *)(&stNcmConnectSpeed));
    if (0 != lRtn)
    {
        AT_ERR_LOG("AT_ActiveUsbNet, Ctrl Speed Fail!" );
        return VOS_ERR;
    }

    ulLinkstus  = NCM_IOCTL_CONNECTION_LINKUP;
    lRtn        = mdrv_udi_ioctl (g_ulAtUdiNdisHdl, NCM_IOCTL_NETWORK_CONNECTION_NOTIF, &ulLinkstus);
    if (0 != lRtn)
    {
        AT_ERR_LOG("AT_ActiveUsbNet, Active usb net Fail!" );
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_DeActiveUsbNet(VOS_VOID)
{
    VOS_UINT32  ulLinkstus;
    VOS_INT32   lRtn;

    /*ȥ����Ѻ�BSPȷ�ϣ����������ȥ�����ȥ���û��Ӱ��*/
    ulLinkstus = NCM_IOCTL_CONNECTION_LINKDOWN;

    lRtn  = mdrv_udi_ioctl (g_ulAtUdiNdisHdl, NCM_IOCTL_NETWORK_CONNECTION_NOTIF, (VOS_VOID*)(&ulLinkstus));
    if(0 != lRtn)
    {
        AT_ERR_LOG("AT_ActiveUsbNet, Deactive usb net Fail!" );
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_SendNdisIPv4PdnInfoCfgReq(
    AT_CLIENT_TAB_INDEX_UINT8           usClientId,
    AT_IPV4_DHCP_PARAM_STRU            *pstIPv4DhcpParam
)
{
    AT_COMM_PS_CTX_STRU                *pstPsCntxt      = VOS_NULL_PTR;
    AT_NDIS_PDNINFO_CFG_REQ_STRU       *pstNdisCfgReq   = VOS_NULL_PTR;
    VOS_UINT32                          ulRelt;
    MODEM_ID_ENUM_UINT16                enModemId;

    enModemId   = MODEM_ID_0;
    pstPsCntxt  = AT_GetCommPsCtxAddr();
    ulRelt      = AT_GetModemIdFromClient(usClientId, &enModemId);

    if (VOS_OK != ulRelt)
    {
        AT_ERR_LOG("AT_SendNdisIPv4PdnInfoCfgReq:Get Modem Id fail");
        return VOS_ERR;
    }

    /* ����AT_NDIS_PDNINFO_CFG_REQ_STRU��Ϣ */
    /*lint -save -e516 */
    pstNdisCfgReq = (AT_NDIS_PDNINFO_CFG_REQ_STRU *)AT_ALLOC_MSG_WITH_HDR(
                            sizeof(AT_NDIS_PDNINFO_CFG_REQ_STRU));
    /*lint -restore */
    if (VOS_NULL_PTR == pstNdisCfgReq)
    {
        AT_WARN_LOG("AT_SendNdisIPv4PdnInfoCfgReq: alloc msg fail!");
        return VOS_ERR;
    }

    /* ��ʼ����Ϣ */
    TAF_MEM_SET_S(AT_GET_MSG_ENTITY(pstNdisCfgReq), AT_GET_MSG_LENGTH(pstNdisCfgReq),
                  0x00, AT_GET_MSG_LENGTH(pstNdisCfgReq));

    /* ��д��Ϣͷ */
    AT_CFG_NDIS_MSG_HDR(pstNdisCfgReq, ID_AT_NDIS_PDNINFO_CFG_REQ);

    /* ��д��Ϣ�� */
    pstNdisCfgReq->bitOpIpv4PdnInfo       = VOS_TRUE;
    pstNdisCfgReq->enModemId              = enModemId;
    pstNdisCfgReq->ulHandle               = g_ulAtUdiNdisHdl;

    /* ������Ϣ */
    if (0 != pstIPv4DhcpParam->ucRabId)
    {
        pstNdisCfgReq->ucRabId = pstIPv4DhcpParam->ucRabId;
    }

    /* ��дIPv4��ַ */
    if (0 != pstIPv4DhcpParam->ulIpv4Addr)
    {
        pstNdisCfgReq->stIpv4PdnInfo.bitOpPdnAddr     = VOS_TRUE;
        AT_PutNetworkAddr32(pstNdisCfgReq->stIpv4PdnInfo.stPDNAddrInfo.aucIpV4Addr,
                            pstIPv4DhcpParam->ulIpv4Addr);
    }

    /* ��д�����ַ */
    if (0 != pstIPv4DhcpParam->ulIpv4NetMask)
    {
        AT_PutNetworkAddr32(pstNdisCfgReq->stIpv4PdnInfo.stSubnetMask.aucIpV4Addr,
                            pstIPv4DhcpParam->ulIpv4NetMask);
    }

    /* ��д���ص�ַ */
    if (0 != pstIPv4DhcpParam->ulIpv4GateWay)
    {
        AT_PutNetworkAddr32(pstNdisCfgReq->stIpv4PdnInfo.stGateWayAddrInfo.aucIpV4Addr,
                            pstIPv4DhcpParam->ulIpv4GateWay);
    }

    /* ��д��DNS��ַ */
    if (0 != pstIPv4DhcpParam->ulIpv4PrimDNS)
    {
        pstNdisCfgReq->stIpv4PdnInfo.bitOpDnsPrim     = VOS_TRUE;
        AT_PutNetworkAddr32(pstNdisCfgReq->stIpv4PdnInfo.stDnsPrimAddrInfo.aucIpV4Addr,
                            pstIPv4DhcpParam->ulIpv4PrimDNS);

    }

    /* ��д��DNS��ַ */
    if (0 != pstIPv4DhcpParam->ulIpv4SecDNS)
    {
        pstNdisCfgReq->stIpv4PdnInfo.bitOpDnsSec      = VOS_TRUE;
        AT_PutNetworkAddr32(pstNdisCfgReq->stIpv4PdnInfo.stDnsSecAddrInfo.aucIpV4Addr,
                          pstIPv4DhcpParam->ulIpv4SecDNS);

    }

    /* ��д��WINS��ַ */
    if (0 != pstIPv4DhcpParam->ulIpv4PrimWINNS)
    {
        pstNdisCfgReq->stIpv4PdnInfo.bitOpWinsPrim    = VOS_TRUE;
        AT_PutNetworkAddr32(pstNdisCfgReq->stIpv4PdnInfo.stWinsPrimAddrInfo.aucIpV4Addr,
                            pstIPv4DhcpParam->ulIpv4PrimWINNS);
    }

    /* ��д��WINS��ַ */
    if (0 != pstIPv4DhcpParam->ulIpv4SecWINNS)
    {
        pstNdisCfgReq->stIpv4PdnInfo.bitOpWinsSec     = VOS_TRUE;
        AT_PutNetworkAddr32(pstNdisCfgReq->stIpv4PdnInfo.stWinsSecAddrInfo.aucIpV4Addr,
                            pstIPv4DhcpParam->ulIpv4SecWINNS);
    }

    if (pstIPv4DhcpParam->stIpv4PcscfList.ucIpv4PcscfAddrNum > 0)
    {
        AT_PutNetworkAddr32(pstNdisCfgReq->stIpv4PdnInfo.stPcscfPrimAddrInfo.aucIpV4Addr,
                            pstIPv4DhcpParam->stIpv4PcscfList.aulIpv4PcscfAddrList[0]);

        pstNdisCfgReq->stIpv4PdnInfo.bitOpPcscfPrim = VOS_TRUE;
    }

    if (pstIPv4DhcpParam->stIpv4PcscfList.ucIpv4PcscfAddrNum > 1)
    {
        AT_PutNetworkAddr32(pstNdisCfgReq->stIpv4PdnInfo.stPcscfSecAddrInfo.aucIpV4Addr,
                            pstIPv4DhcpParam->stIpv4PcscfList.aulIpv4PcscfAddrList[1]);

        pstNdisCfgReq->stIpv4PdnInfo.bitOpPcscfSec    = VOS_TRUE;
    }

    pstNdisCfgReq->lSpePort   = pstPsCntxt->lSpePort;
    pstNdisCfgReq->ulIpfFlag  = pstPsCntxt->ulIpfPortFlg;

    /* ������Ϣ */
    if (VOS_OK != PS_SEND_MSG(WUEPS_PID_AT, pstNdisCfgReq))
    {
        AT_WARN_LOG("AT_SendNdisIPv4PdnInfoCfgReq: Send msg fail!");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_VOID AT_RcvNdisPdnInfoCfgCnf(
    AT_NDIS_PDNINFO_CFG_CNF_STRU       *pstNdisPdnInfoCfgCnf
)
{
    if (AT_NDIS_PDNCFG_CNF_FAIL == pstNdisPdnInfoCfgCnf->enResult)
    {
        AT_ERR_LOG("AT_RcvNdisPdnInfoRelCnf: Failed.");
    }

    return;
}


VOS_VOID AT_RcvNdisPdnInfoRelCnf(
    AT_NDIS_PDNINFO_REL_CNF_STRU       *pstNdisPdnInfoRelCnf
)
{
    if (AT_NDIS_SUCC != pstNdisPdnInfoRelCnf->enResult)
    {
        AT_ERR_LOG("AT_RcvNdisPdnInfoRelCnf: Failed.");
    }

    return;
}



VOS_VOID AT_NdisConfigIpv6Dns(VOS_VOID)
{
    AT_PDP_ENTITY_STRU                 *pstNdisPdpEntity;
    NCM_IPV6DNS_S                       stIPv6Dns;
    VOS_INT32                           lRslt;

    pstNdisPdpEntity = AT_NDIS_GetPdpEntInfoAddr();
    /*lint -save -e516 */
    stIPv6Dns.pu8Ipv6DnsInfo = (unsigned char*)PS_MEM_ALLOC(
                                    WUEPS_PID_AT,
                                    BSP_NCM_IPV6_DNS_LEN);
    /*lint -restore */
    if (VOS_NULL_PTR == stIPv6Dns.pu8Ipv6DnsInfo)
    {
        AT_ERR_LOG("AT_NdisConfigIpv6Dns:Invalid stIPv6Dns.pu8Ipv6DnsInfo");
        return;
    }

    TAF_MEM_SET_S(stIPv6Dns.pu8Ipv6DnsInfo, BSP_NCM_IPV6_DNS_LEN, 0x00, BSP_NCM_IPV6_DNS_LEN);

    /* �ϱ��������DNS���ȹ̶�Ϊ32(Primary DNS LEN + Secondary DNS LEN) */
    stIPv6Dns.u32Length = BSP_NCM_IPV6_DNS_LEN;

    /*�����DNS����Ҫ����DRV�Ľӿ��ϱ�DNS��PC*/
    if (VOS_TRUE == pstNdisPdpEntity->stIpv6Dhcp.bitOpIpv6PriDns)
    {
        TAF_MEM_CPY_S(stIPv6Dns.pu8Ipv6DnsInfo,
                   BSP_NCM_IPV6_DNS_LEN,
                   pstNdisPdpEntity->stIpv6Dhcp.aucIpv6PrimDNS,
                   AT_MAX_IPV6_DNS_LEN);
    }

    if (VOS_TRUE == pstNdisPdpEntity->stIpv6Dhcp.bitOpIpv6SecDns)
    {
        TAF_MEM_CPY_S(stIPv6Dns.pu8Ipv6DnsInfo + AT_MAX_IPV6_DNS_LEN,
                   BSP_NCM_IPV6_DNS_LEN - AT_MAX_IPV6_DNS_LEN,
                   pstNdisPdpEntity->stIpv6Dhcp.aucIpv6SecDNS,
                   AT_MAX_IPV6_DNS_LEN);
    }

    /* ���õ�������DNS��Ϣ */
    lRslt = mdrv_udi_ioctl(g_ulAtUdiNdisHdl, NCM_IOCTL_SET_IPV6_DNS, &stIPv6Dns);
    if (0 != lRslt)
    {
        AT_ERR_LOG("AT_NdisConfigIpv6Dns, DRV_UDI_IOCTL Fail!" );
    }

    /* �ͷ�������ڴ� */
    /*lint -save -e516 */
    PS_MEM_FREE(WUEPS_PID_AT, stIPv6Dns.pu8Ipv6DnsInfo);
    /*lint -restore */
}


VOS_VOID  AT_NdisActCnfBackProc(
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU        *pstEvent
)
{
    AT_COMM_PS_CTX_STRU                *pstCommPsCtx = VOS_NULL_PTR;
    VOS_UINT8                           ucCid;
    VOS_UINT32                          ulRet;

    pstCommPsCtx = AT_GetCommPsCtxAddr();

    /* ����ԭ��ֵ��ԭ��ֵΪ52����Ҫ������һ��PDP���� */
    if ( (VOS_FALSE == pstEvent->bitOpCause)
      || ( (VOS_TRUE == pstEvent->bitOpCause)
        && (TAF_PS_CAUSE_SM_NW_SINGLE_ADDR_BEARERS_ONLY_ALLOWED == pstEvent->enCause) ) )
    {
        if (TAF_PDP_IPV4 == pstEvent->stPdpAddr.enPdpType)
        {
            if((VOS_FALSE == pstEvent->bitOpCause)
            && (VOS_FALSE == pstCommPsCtx->stRedialForNwCauseCfg.ucRedialForNoCausePolicy))
            {
                AT_NdisStateChangeProc(TAF_PDP_IPV6, AT_PDP_STATUS_DEACT, TAF_PS_CAUSE_SM_NW_PDP_TYPE_IPV4_ONLY_ALLOWED);
            }
            else
            {
                if (AT_PDP_STATE_IDLE == AT_NdisGetState(TAF_PDP_IPV6))
                {
                    /* ����һ��δ�����CID���м��� */
                    ulRet       = TAF_AGENT_FindCidForDial(pstEvent->stCtrl.usClientId, &ucCid);
                    if (VOS_OK == ulRet)
                    {
                        AT_NdisPdpActOrig(pstEvent->stCtrl.usClientId, ucCid, TAF_PDP_IPV6);
                    }
                }
            }
        }
        else if (TAF_PDP_IPV6 == pstEvent->stPdpAddr.enPdpType)
        {
            if((VOS_FALSE == pstEvent->bitOpCause)
            && (VOS_FALSE == pstCommPsCtx->stRedialForNwCauseCfg.ucRedialForNoCausePolicy))
            {
                AT_NdisStateChangeProc(TAF_PDP_IPV4, AT_PDP_STATUS_DEACT, TAF_PS_CAUSE_SM_NW_PDP_TYPE_IPV6_ONLY_ALLOWED);
            }
            else
            {
                if (AT_PDP_STATE_IDLE == AT_NdisGetState(TAF_PDP_IPV4))
                {
                    /* ����һ��δ�����CID���м��� */
                    ulRet       = TAF_AGENT_FindCidForDial(pstEvent->stCtrl.usClientId, &ucCid);
                    if (VOS_OK == ulRet)
                    {
                        AT_NdisPdpActOrig(pstEvent->stCtrl.usClientId, ucCid, TAF_PDP_IPV4);
                    }
                }
            }
        }
        else
        {
            ;
        }

        return;
    }
    /* ����ԭ��ֵ, ֪ͨAPP���Ž�� */
    switch (pstEvent->enCause)
    {
        /* ԭ��ֵ#50 (IPv4 ONLY), ֪ͨAPPģ��IPv6����ʧ�� */
        case TAF_PS_CAUSE_SM_NW_PDP_TYPE_IPV4_ONLY_ALLOWED:
        case TAF_PS_CAUSE_PDP_TYPE_IPV4_ONLY_ALLOWED:

            AT_NdisStateChangeProc(TAF_PDP_IPV6, AT_PDP_STATUS_DEACT, pstEvent->enCause);

            break;

        /* ԭ��ֵ#51 (IPv6 ONLY), ֪ͨAPPģ��IPv4����ʧ�� */
        case TAF_PS_CAUSE_SM_NW_PDP_TYPE_IPV6_ONLY_ALLOWED:
        case TAF_PS_CAUSE_PDP_TYPE_IPV6_ONLY_ALLOWED:
            AT_NdisStateChangeProc(TAF_PDP_IPV4, AT_PDP_STATUS_DEACT, pstEvent->enCause);

            break;

        /* ����ԭ��ֵ, ������ */
        default:
            AT_NORM_LOG1("AT_NdisActCnfBackProc: Other <Cause>", pstEvent->enCause);
            break;
    }

    return;
}


VOS_VOID AT_NdisIpv6ActCnfProc(
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU        *pstEvent
)
{
    /* ��ʼ�� */
    TAF_MEM_SET_S(&g_stAtNdisDhcpPara.stIpv6Dhcp,
               sizeof(g_stAtNdisDhcpPara.stIpv6Dhcp),
               0x00,
               sizeof(AT_IPV6_DHCP_PARAM_STRU));

    g_stAtNdisDhcpPara.ucIpv6Cid   = pstEvent->ucCid;

    /* ����IPV6��ַ */
    AT_NdisCtrlConnIpv6IndProc(pstEvent);

    /* NDIS�û����͵�PDP�����,�������ص�ע����� */
    AT_ProcNdisRegFCPoint(pstEvent, TAF_PDP_IPV6);

    /* ͨ�������ϱ�IPv6 DNS */
    AT_NdisConfigIpv6Dns();

    /* �����IPV4V6����Ҫ������һ��PDP */
    if (TAF_PDP_IPV4V6 == g_enAtFirstNdisActPdpType)
    {
        switch (g_enAtNdisActPdpType)
        {
            case TAF_PDP_IPV6:
                /* �������������PDP���ˣ���ǰ��PDP����ܾ��󣬷ֱ���IPV4��
                   IPV6��PDP����, IPV6������ٷ���IPV4��PDP���� */
                break;

            /* ������PDP������˹���ʵ�� */
            case TAF_PDP_IPV4V6:
                /* ����PDP�л���IDLE̬ */
               AT_NdisSetState(g_enAtNdisActPdpType, AT_PDP_STATE_IDLE);

               AT_NdisActCnfBackProc(pstEvent);
               break;

            default:
               AT_WARN_LOG("AT_NdisIpv6ActCnfProc:g_enAtNdisActPdpType is error!");
               break;
        }
    }

    return;
}


VOS_VOID AT_NdisIpv4v6ActCnfProc(
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU        *pstEvent
)
{
    VOS_UINT8                           aucIpv6Dns[AT_MAX_IPV6_DNS_LEN * 2];

    /* ��ʼ�� */
    TAF_MEM_SET_S(aucIpv6Dns, sizeof(aucIpv6Dns), 0x00, sizeof(aucIpv6Dns));

    TAF_MEM_SET_S(&g_stAtNdisDhcpPara.stIpv6Dhcp,
               sizeof(g_stAtNdisDhcpPara.stIpv6Dhcp),
               0x00,
               sizeof(AT_IPV6_DHCP_PARAM_STRU));

    g_stAtNdisDhcpPara.ucIpv4v6Cid   = pstEvent->ucCid;

    /* ����IPV4���͵�DHCP */
    AT_CtrlConnIndProc(pstEvent, AT_NDIS_USER);

    /* ����IPV6��ַ*/
    AT_NdisCtrlConnIpv6IndProc(pstEvent);

    /* NDIS�û����͵�PDP״̬�ı��,�������ص�ע����� */
    AT_ProcNdisRegFCPoint(pstEvent, TAF_PDP_IPV4V6);

    /* ��IPV4��PDN��Ϣ���͸�NDISģ�� */
    AT_SendNdisIPv4PdnInfoCfgReq(AT_NDIS_GET_USR_PORT_INDEX(),
                                 &(g_stAtNdisDhcpPara.stIpv4Dhcp));

    /* ֪ͨNDIS PDP����ɹ�,ipv6��PDN��Ϣ���յ�RA����ʱ���� */

    /* ͨ�������ϱ�IPv6 DNS */
    AT_NdisConfigIpv6Dns();

    return;
}


VOS_UINT32 AT_SendNdisIPv6PdnInfoCfgReq(
    MODEM_ID_ENUM_UINT16                 enModemId,
    TAF_PS_IPV6_INFO_IND_STRU           *pIPv6RaNotify
)
{
    AT_COMM_PS_CTX_STRU                *pstPsCntxt      = VOS_NULL_PTR;
    AT_NDIS_PDNINFO_CFG_REQ_STRU       *pstNdisCfgReq   = VOS_NULL_PTR;
    AT_PDP_ENTITY_STRU                 *pstNdisPdpEntity;

    /* ��ʼ�� */
    pstPsCntxt          = AT_GetCommPsCtxAddr();
    pstNdisPdpEntity    = AT_NDIS_GetPdpEntInfoAddr();

    /* ����AT_NDIS_PDNINFO_CFG_REQ_STRU��Ϣ */
    /*lint -save -e516 */
    pstNdisCfgReq = (AT_NDIS_PDNINFO_CFG_REQ_STRU *)AT_ALLOC_MSG_WITH_HDR(
                            sizeof(AT_NDIS_PDNINFO_CFG_REQ_STRU));
    /*lint -restore */
    if (VOS_NULL_PTR == pstNdisCfgReq)
    {
        AT_WARN_LOG("AT_SendNdisIPv6PdnInfoCfgReq: alloc msg fail!");
        return VOS_ERR;
    }

    /* ��ʼ����Ϣ */
    TAF_MEM_SET_S(AT_GET_MSG_ENTITY(pstNdisCfgReq), AT_GET_MSG_LENGTH(pstNdisCfgReq),
                  0x00, AT_GET_MSG_LENGTH(pstNdisCfgReq));

    /* ��д��Ϣͷ */
    AT_CFG_NDIS_MSG_HDR(pstNdisCfgReq, ID_AT_NDIS_PDNINFO_CFG_REQ);

    /* ��д��Ϣ�� */
    pstNdisCfgReq->ulHandle               = g_ulAtUdiNdisHdl;
    pstNdisCfgReq->bitOpIpv6PdnInfo       = VOS_TRUE;
    pstNdisCfgReq->enModemId              = enModemId;
    pstNdisCfgReq->ucRabId                = pIPv6RaNotify->ucRabId;

    /* �������DNS */
    pstNdisCfgReq->stIpv6PdnInfo.stDnsSer.ucSerNum    = 0;
    if (VOS_TRUE == pstNdisPdpEntity->stIpv6Dhcp.bitOpIpv6PriDns)
    {
        TAF_MEM_CPY_S((VOS_VOID *)pstNdisCfgReq->stIpv6PdnInfo.stDnsSer.aucPriServer,
                    sizeof(pstNdisCfgReq->stIpv6PdnInfo.stDnsSer.aucPriServer),
                    pstNdisPdpEntity->stIpv6Dhcp.aucIpv6PrimDNS,
                    TAF_IPV6_ADDR_LEN);
        pstNdisCfgReq->stIpv6PdnInfo.stDnsSer.ucSerNum += 1;
    }

    if (VOS_TRUE == pstNdisPdpEntity->stIpv6Dhcp.bitOpIpv6SecDns)
    {
        TAF_MEM_CPY_S((VOS_VOID *)pstNdisCfgReq->stIpv6PdnInfo.stDnsSer.aucSecServer,
                    sizeof(pstNdisCfgReq->stIpv6PdnInfo.stDnsSer.aucSecServer),
                    pstNdisPdpEntity->stIpv6Dhcp.aucIpv6SecDNS,
                    TAF_IPV6_ADDR_LEN);
        pstNdisCfgReq->stIpv6PdnInfo.stDnsSer.ucSerNum += 1;
    }

    /* ���MTU */
    if (VOS_TRUE == pIPv6RaNotify->stIpv6RaInfo.bitOpMtu)
    {
        pstNdisCfgReq->stIpv6PdnInfo.ulBitOpMtu   = VOS_TRUE;
        pstNdisCfgReq->stIpv6PdnInfo.ulMtu        = pIPv6RaNotify->stIpv6RaInfo.ulMtu;
    }

    pstNdisCfgReq->stIpv6PdnInfo.ulBitCurHopLimit = pIPv6RaNotify->stIpv6RaInfo.ulBitCurHopLimit;
    pstNdisCfgReq->stIpv6PdnInfo.ulBitM           = pIPv6RaNotify->stIpv6RaInfo.ulBitM;
    pstNdisCfgReq->stIpv6PdnInfo.ulBitO           = pIPv6RaNotify->stIpv6RaInfo.ulBitO;
    pstNdisCfgReq->stIpv6PdnInfo.ulPrefixNum      = pIPv6RaNotify->stIpv6RaInfo.ulPrefixNum;
    TAF_MEM_CPY_S((VOS_VOID *)pstNdisCfgReq->stIpv6PdnInfo.astPrefixList,
                sizeof(pstNdisCfgReq->stIpv6PdnInfo.astPrefixList),
                (VOS_VOID *)pIPv6RaNotify->stIpv6RaInfo.astPrefixList,
                sizeof(TAF_PDP_IPV6_PREFIX_STRU)*TAF_MAX_PREFIX_NUM_IN_RA);

    /* ��дINTERFACE��ȡIPV6��ַ�ĺ�8�ֽ�����дINTERFACE */
    TAF_MEM_CPY_S((VOS_VOID*)pstNdisCfgReq->stIpv6PdnInfo.aucInterfaceId,
                sizeof(pstNdisCfgReq->stIpv6PdnInfo.aucInterfaceId),
               (VOS_VOID*)pstNdisPdpEntity->stIpv6Dhcp.aucIpv6Addr,
                sizeof(VOS_UINT8)*AT_NDIS_IPV6_IFID_LENGTH);

    /* �������PCSCF��ַ  */
    pstNdisCfgReq->stIpv6PdnInfo.stPcscfSer.ucSerNum      = 0;
    if (pstNdisPdpEntity->stIpv6Dhcp.stIpv6PcscfList.ucIpv6PcscfAddrNum > 0)
    {
        pstNdisCfgReq->stIpv6PdnInfo.stPcscfSer.ucSerNum++;

        TAF_MEM_CPY_S((VOS_VOID *)pstNdisCfgReq->stIpv6PdnInfo.stPcscfSer.aucPriServer,
                      sizeof(pstNdisCfgReq->stIpv6PdnInfo.stPcscfSer.aucPriServer),
                      pstNdisPdpEntity->stIpv6Dhcp.stIpv6PcscfList.astIpv6PcscfAddrList[0].aucPcscfAddr,
                      sizeof(pstNdisPdpEntity->stIpv6Dhcp.stIpv6PcscfList.astIpv6PcscfAddrList[0].aucPcscfAddr));
    }

    if (pstNdisPdpEntity->stIpv6Dhcp.stIpv6PcscfList.ucIpv6PcscfAddrNum > 1)
    {
        pstNdisCfgReq->stIpv6PdnInfo.stPcscfSer.ucSerNum++;

        TAF_MEM_CPY_S((VOS_VOID *)pstNdisCfgReq->stIpv6PdnInfo.stPcscfSer.aucSecServer,
                      sizeof(pstNdisCfgReq->stIpv6PdnInfo.stPcscfSer.aucSecServer),
                      pstNdisPdpEntity->stIpv6Dhcp.stIpv6PcscfList.astIpv6PcscfAddrList[1].aucPcscfAddr,
                      sizeof(pstNdisPdpEntity->stIpv6Dhcp.stIpv6PcscfList.astIpv6PcscfAddrList[1].aucPcscfAddr));
    }

    pstNdisCfgReq->lSpePort   = pstPsCntxt->lSpePort;
    pstNdisCfgReq->ulIpfFlag  = pstPsCntxt->ulIpfPortFlg;

    /* ������Ϣ */
    if (VOS_OK != PS_SEND_MSG(WUEPS_PID_AT, pstNdisCfgReq))
    {
        AT_WARN_LOG("AT_SendNdisIPv6PdnInfoCfgReq: Send msg fail!");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_VOID  AT_NdisCtrlConnIpv6IndProc(
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU        *pstEvent
)
{
    g_stAtNdisDhcpPara.stIpv6Dhcp.ucRabId        = pstEvent->ucRabId;
    g_stAtNdisDhcpPara.stIpv6Dhcp.ucPduSessionId = pstEvent->ucPduSessionId;

    /* ����IPV6��IP��ַ����ʽΪ������ */
    TAF_MEM_CPY_S(g_stAtNdisDhcpPara.stIpv6Dhcp.aucIpv6Addr,
               sizeof(g_stAtNdisDhcpPara.stIpv6Dhcp.aucIpv6Addr),
               pstEvent->stPdpAddr.aucIpv6Addr,
               TAF_IPV6_ADDR_LEN);

    /* ����IPV6������DNS��ַ����ʽΪ������ */
    AT_SaveIPv6Dns(pstEvent, &g_stAtNdisDhcpPara);

    /* ����IPV6������PCSCF��ַ����ʽΪ������ */
    AT_SaveIPv6Pcscf(pstEvent, &g_stAtNdisDhcpPara);

    return;
}


VOS_VOID AT_NdisIpv6DhcpRest(VOS_VOID)
{
    TAF_MEM_SET_S(&g_stAtNdisDhcpPara.stIpv6Dhcp,
               sizeof(g_stAtNdisDhcpPara.stIpv6Dhcp),
               0x00,
               sizeof(AT_IPV6_DHCP_PARAM_STRU));

    return;
}


VOS_UINT32 AT_NdisCheckIpv6PdpState(
    AT_PDP_STATE_ENUM_U8                enPdpState
)
{
    if ( (enPdpState == g_stAtNdisDhcpPara.enIpv6State)
      || (enPdpState == g_stAtNdisDhcpPara.enIpv4v6State) )
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}



VOS_UINT32 AT_NdisCheckIpv4PdpState(
    AT_PDP_STATE_ENUM_U8                enPdpState
)
{
    if ( (enPdpState == g_stAtNdisDhcpPara.enIpv4State)
      || (enPdpState == g_stAtNdisDhcpPara.enIpv4v6State) )
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 AT_NdisCheckPdpState(
    AT_PDP_STATE_ENUM_U8                enPdpState
)
{
    if ( (enPdpState == g_stAtNdisDhcpPara.enIpv4State)
      || (enPdpState == g_stAtNdisDhcpPara.enIpv6State)
      || (enPdpState == g_stAtNdisDhcpPara.enIpv4v6State) )
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 AT_NdisCheckPdpIdleState(VOS_VOID)
{
    /*������е�PDP������IDLE;�����һ������DEACTINGҲ����OK*/
    if ( (AT_PDP_STATE_DEACTING == g_stAtNdisDhcpPara.enIpv4State)
      || (AT_PDP_STATE_DEACTING == g_stAtNdisDhcpPara.enIpv6State)
      || (AT_PDP_STATE_DEACTING == g_stAtNdisDhcpPara.enIpv4v6State)
      || ( (AT_PDP_STATE_IDLE == g_stAtNdisDhcpPara.enIpv4State)
        && (AT_PDP_STATE_IDLE == g_stAtNdisDhcpPara.enIpv6State)
        && (AT_PDP_STATE_IDLE == g_stAtNdisDhcpPara.enIpv4v6State) ) )
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_VOID  AT_NdisAddrProc(
    AT_DHCP_CONFIG_STRU                *pstConfig,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    VOS_UINT32                          ulIpAddr;
    VOS_UINT8                           ucIndex;

    g_stAtNdisDhcpPara.stIpv4Dhcp.ucRabId                 = pstEvent->ucRabId;
    g_stAtNdisDhcpPara.stIpv4Dhcp.ucPduSessionId          = pstEvent->ucPduSessionId;
    g_stAtNdisDhcpPara.stIpv4Dhcp.ulIpv4Addr              = pstConfig->ulIPAddr;
    g_stAtNdisDhcpPara.stIpv4Dhcp.ulIpv4GateWay           = pstConfig->ulGateWay;
    g_stAtNdisDhcpPara.stIpv4Dhcp.ulIpv4NetMask           = pstConfig->ulSubNetMask;


    /* ����û���������DNS����ʹ���û����õ�DNS�����緵�ص�DNS��ʹ�� */
    if (VOS_TRUE == gstAtNdisAddParam.ulPrimIPv4DNSValidFlag)
    {
        if (VOS_ERR == AT_LenStr2IpAddr(gstAtNdisAddParam.aucPrimIPv4DNSAddr, (VOS_UINT8 *)&ulIpAddr))
        {
            ulIpAddr = 0;
        }

        if (0 != ulIpAddr)
        {
            g_stAtNdisDhcpPara.stIpv4Dhcp.ulIpv4PrimDNS       = VOS_NTOHL(ulIpAddr);
            g_stAtNdisDhcpPara.stIpv4Dhcp.bitOpIpv4PriDns     = VOS_TRUE;
        }
    }
    else
    {
        if ( 0 != pstConfig->ulPrimDNS )
        {
            g_stAtNdisDhcpPara.stIpv4Dhcp.bitOpIpv4PriDns = VOS_TRUE;
            g_stAtNdisDhcpPara.stIpv4Dhcp.ulIpv4PrimDNS   = pstConfig->ulPrimDNS;
        }
        else
        {
            g_stAtNdisDhcpPara.stIpv4Dhcp.bitOpIpv4PriDns = VOS_FALSE;
        }
    }

    /* ����û������˸�DNS����ʹ���û����õ�DNS�����緵�ص�DNS��ʹ�� */
    if (VOS_TRUE == gstAtNdisAddParam.ulSndIPv4DNSValidFlag)
    {
        if (VOS_ERR == AT_LenStr2IpAddr(gstAtNdisAddParam.aucSndIPv4DNSAddr, (VOS_UINT8 *)&ulIpAddr))
        {
            ulIpAddr = 0;
        }

        if (0 != ulIpAddr)
        {
            g_stAtNdisDhcpPara.stIpv4Dhcp.ulIpv4SecDNS    = VOS_NTOHL(ulIpAddr);
            g_stAtNdisDhcpPara.stIpv4Dhcp.bitOpIpv4SecDns = VOS_TRUE;
        }
    }
    else
    {
        if ( 0 != pstConfig->ulSndDNS)
        {
            g_stAtNdisDhcpPara.stIpv4Dhcp.bitOpIpv4SecDns = VOS_TRUE;
            g_stAtNdisDhcpPara.stIpv4Dhcp.ulIpv4SecDNS    = pstConfig->ulSndDNS;
        }
        else
        {
            g_stAtNdisDhcpPara.stIpv4Dhcp.bitOpIpv4SecDns = VOS_FALSE;
        }
    }

    /* ����û���������WINS����ʹ���û����õ�WINS�����緵�ص�WINS��ʹ�� */
    if (VOS_TRUE == gstAtNdisAddParam.ulPrimIPv4WINNSValidFlag)
    {
        if (VOS_ERR == AT_LenStr2IpAddr(gstAtNdisAddParam.aucPrimIPv4WINNSAddr, (VOS_UINT8 *)&ulIpAddr))
        {
            ulIpAddr = 0;
        }

        if (0 != ulIpAddr)
        {
            g_stAtNdisDhcpPara.stIpv4Dhcp.ulIpv4PrimWINNS     = VOS_NTOHL(ulIpAddr);
            g_stAtNdisDhcpPara.stIpv4Dhcp.bitOpIpv4PriWINNS   = VOS_TRUE;
        }
    }
    else
    {
        g_stAtNdisDhcpPara.stIpv4Dhcp.ulIpv4PrimWINNS     = 0;
        g_stAtNdisDhcpPara.stIpv4Dhcp.bitOpIpv4PriWINNS   = VOS_FALSE;
    }

    /* ����û������˸�WINS����ʹ���û����õ�WINS�����緵�ص�WINS��ʹ�� */
    if (VOS_TRUE == gstAtNdisAddParam.ulSndIPv4WINNSValidFlag)
    {
        if (VOS_ERR == AT_LenStr2IpAddr(gstAtNdisAddParam.aucSndIPv4WINNSAddr, (VOS_UINT8 *)&ulIpAddr))
        {
            ulIpAddr = 0;
        }

        if (0 != ulIpAddr)
        {
            g_stAtNdisDhcpPara.stIpv4Dhcp.ulIpv4SecWINNS      = VOS_NTOHL(ulIpAddr);
            g_stAtNdisDhcpPara.stIpv4Dhcp.bitOpIpv4SecWINNS   = VOS_TRUE;
        }
    }
    else
    {
        g_stAtNdisDhcpPara.stIpv4Dhcp.ulIpv4SecWINNS      = 0;
        g_stAtNdisDhcpPara.stIpv4Dhcp.bitOpIpv4SecWINNS   = VOS_FALSE;
    }

    /* ��ȡPCSCF��ַ */
    g_stAtNdisDhcpPara.stIpv4Dhcp.stIpv4PcscfList.ucIpv4PcscfAddrNum = AT_MIN(TAF_PCSCF_ADDR_MAX_NUM,
                                                                              pstEvent->stIpv4PcscfList.ucIpv4PcscfAddrNum);

    for (ucIndex = 0; ucIndex < g_stAtNdisDhcpPara.stIpv4Dhcp.stIpv4PcscfList.ucIpv4PcscfAddrNum; ucIndex++)
    {
        g_stAtNdisDhcpPara.stIpv4Dhcp.stIpv4PcscfList.aulIpv4PcscfAddrList[ucIndex] = AT_GetLanAddr32(pstEvent->stIpv4PcscfList.astIpv4PcscfAddrList[ucIndex].aucPcscfAddr);
    }

    return;
}


VOS_VOID AT_NdisStateChangeProc(
    VOS_UINT8                           ucPdpType,
    AT_PDP_STATUS_ENUM_UINT32           enStat,
    TAF_PS_CAUSE_ENUM_UINT32            enCause
)
{
    VOS_UINT16                          usLength;
    VOS_UINT8                           aucAtStrIpv4[] = "IPV4";
    VOS_UINT8                           aucAtStrIpv6[] = "IPV6";

    VOS_UINT32                          ul3gppSmCause;

    usLength = 0;

    if (AT_PDP_STATUS_DEACT == enStat)
    {
        ul3gppSmCause = AT_Get3gppSmCauseByPsCause(enCause);

        switch (ucPdpType)
        {
            case TAF_PDP_IPV4:
                usLength = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN, (VOS_CHAR*)pgucAtSndCodeAddr, (VOS_CHAR*)pgucAtSndCodeAddr + usLength,
                                            "%s^NDISSTAT:%d,%d,,\"%s\"%s",
                                            gaucAtCrLf,
                                            enStat,
                                            ul3gppSmCause,
                                            aucAtStrIpv4,
                                            gaucAtCrLf);
                break;

            case TAF_PDP_IPV6:
                usLength = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN, (VOS_CHAR*)pgucAtSndCodeAddr, (VOS_CHAR*)pgucAtSndCodeAddr + usLength,
                                            "%s^NDISSTAT:%d,%d,,\"%s\"%s",
                                            gaucAtCrLf,
                                            enStat,
                                            ul3gppSmCause,
                                            aucAtStrIpv6,
                                            gaucAtCrLf);
                break;

            case TAF_PDP_IPV4V6:

                usLength  = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN, (VOS_CHAR*)pgucAtSndCodeAddr, (VOS_CHAR*)pgucAtSndCodeAddr + usLength,
                                            "%s^NDISSTAT:%d,%d,,\"%s\"%s",
                                            gaucAtCrLf,
                                            enStat,
                                            ul3gppSmCause,
                                            aucAtStrIpv4,
                                            gaucAtCrLf);

                At_SendResultData(AT_NDIS_GET_USR_PORT_INDEX(), pgucAtSndCodeAddr, usLength);

                usLength  = 0;

                usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN, (VOS_CHAR*)pgucAtSndCodeAddr, (VOS_CHAR*)pgucAtSndCodeAddr + usLength,
                                            "%s^NDISSTAT:%d,%d,,\"%s\"%s",
                                            gaucAtCrLf,
                                            enStat,
                                            ul3gppSmCause,
                                            aucAtStrIpv6,
                                            gaucAtCrLf);
                break;

            default:
                AT_ERR_LOG("AT_NdisStateChangeProc:ERROR: Wrong PDP type!");
                return;
        }
    }
    else
    {
        switch (ucPdpType)
        {
            case TAF_PDP_IPV4:
                usLength = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN, (VOS_CHAR*)pgucAtSndCodeAddr, (VOS_CHAR*)pgucAtSndCodeAddr + usLength,
                                            "%s^NDISSTAT:%d,,,\"%s\"%s",
                                            gaucAtCrLf,
                                            enStat,
                                            aucAtStrIpv4,
                                            gaucAtCrLf);
                break;

            case TAF_PDP_IPV6:
                usLength = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN, (VOS_CHAR*)pgucAtSndCodeAddr, (VOS_CHAR*)pgucAtSndCodeAddr + usLength,
                                            "%s^NDISSTAT:%d,,,\"%s\"%s",
                                            gaucAtCrLf,
                                            enStat,
                                            aucAtStrIpv6,
                                            gaucAtCrLf);
                break;

            case TAF_PDP_IPV4V6:
                usLength  = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN, (VOS_CHAR*)pgucAtSndCodeAddr, (VOS_CHAR*)pgucAtSndCodeAddr + usLength,
                                            "%s^NDISSTAT:%d,,,\"%s\"%s",
                                            gaucAtCrLf,
                                            enStat,
                                            aucAtStrIpv4,
                                            gaucAtCrLf);

                At_SendResultData(AT_NDIS_GET_USR_PORT_INDEX(), pgucAtSndCodeAddr, usLength);

                usLength  = 0;

                usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN, (VOS_CHAR*)pgucAtSndCodeAddr, (VOS_CHAR*)pgucAtSndCodeAddr + usLength,
                                            "%s^NDISSTAT:%d,,,\"%s\"%s",
                                            gaucAtCrLf,
                                            enStat,
                                            aucAtStrIpv6,
                                            gaucAtCrLf);
                break;

            default:
                AT_ERR_LOG("AT_NdisStateChangeProc:ERROR: Wrong PDP type!");
                return;
        }
    }

    At_SendResultData(AT_NDIS_GET_USR_PORT_INDEX(), pgucAtSndCodeAddr, usLength);

    return;
}


AT_PDP_STATE_ENUM_U8 AT_NdisGetState(
    VOS_UINT8                           ucPdpType
)
{
    if (TAF_PDP_IPV4 == ucPdpType )
    {
        return g_stAtNdisDhcpPara.enIpv4State;
    }
    else if (TAF_PDP_IPV6 == ucPdpType )
    {
        return g_stAtNdisDhcpPara.enIpv6State;
    }
    else
    {
        return g_stAtNdisDhcpPara.enIpv4v6State;
    }
}


VOS_VOID AT_NdisSetState(
    VOS_UINT8                           ucPdpType,
    AT_PDP_STATE_ENUM_U8                enState
)
{
    if (TAF_PDP_IPV4 == ucPdpType)
    {
        g_stAtNdisDhcpPara.enIpv4State   = enState;
    }
    else if (TAF_PDP_IPV6 == ucPdpType)
    {
        g_stAtNdisDhcpPara.enIpv6State   = enState;
    }
    else if (TAF_PDP_IPV4V6 == ucPdpType)
    {
        g_stAtNdisDhcpPara.enIpv4v6State = enState;
    }
    else
    {}



    return;
}


VOS_VOID AT_NdisDhcpRest(VOS_VOID)
{
    TAF_MEM_SET_S(&g_stAtNdisDhcpPara.stIpv4Dhcp,
               sizeof(g_stAtNdisDhcpPara.stIpv4Dhcp),
               0x00,
               sizeof(AT_IPV4_DHCP_PARAM_STRU));

    return;
}


VOS_VOID AT_NdisGetDhcpPara(
    AT_DHCP_PARA_STRU                  *pstConfig
)
{
    /* ��DHCP����ת��Ϊ������ */
    pstConfig->stDhcpCfg.ulIPAddr     = VOS_HTONL(g_stAtNdisDhcpPara.stIpv4Dhcp.ulIpv4Addr);
    pstConfig->stDhcpCfg.ulSubNetMask = VOS_HTONL(g_stAtNdisDhcpPara.stIpv4Dhcp.ulIpv4NetMask);
    pstConfig->stDhcpCfg.ulGateWay    = VOS_HTONL(g_stAtNdisDhcpPara.stIpv4Dhcp.ulIpv4GateWay);
    pstConfig->stDhcpCfg.ulPrimDNS    = VOS_HTONL(g_stAtNdisDhcpPara.stIpv4Dhcp.ulIpv4PrimDNS);
    pstConfig->stDhcpCfg.ulSndDNS     = VOS_HTONL(g_stAtNdisDhcpPara.stIpv4Dhcp.ulIpv4SecDNS);
    return;
}


VOS_VOID  AT_ProcNdisDeRegFCPoint(
    TAF_PS_CALL_PDP_DEACTIVATE_CNF_STRU  *pstEvent,
    TAF_PDP_TYPE_ENUM_UINT8               enPdpType
)
{
    VOS_UINT32                          ulRet;
    MODEM_ID_ENUM_UINT16                enModemId;

    enModemId                           = MODEM_ID_0;

    ulRet = AT_GetModemIdFromClient((AT_CLIENT_TAB_INDEX_UINT8)(pstEvent->stCtrl.usClientId), &enModemId);

    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_ProcNdisDeRegFCPoint:Get Modem Id fail");
        return;
    }

    /* ȥע��NDIS�˿ڵ����ص� */
    AT_DeRegNdisFCPoint(pstEvent->ucRabId, enModemId);

}


VOS_VOID  AT_ProcNdisRegFCPoint(
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType
)
{
    VOS_UINT32                          ulRet;
    AT_FCID_MAP_STRU                    stFCPriOrg;
    FC_PRI_ENUM_UINT8                   enFCPriCurrent;
    MODEM_ID_ENUM_UINT16                enModemId;

    enModemId                           = MODEM_ID_0;

    ulRet = AT_GetModemIdFromClient((AT_CLIENT_TAB_INDEX_UINT8)(pstEvent->stCtrl.usClientId), &enModemId);

    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_ProcNdisRegFCPoint:Get Modem Id fail");
        return;
    }

    ulRet = AT_GetFcPriFromMap(FC_ID_NIC_1 ,&stFCPriOrg);
    if (VOS_OK == ulRet)
    {
        /* ���FC IDδע�ᣬ��ôע������ص㡣Ŀǰֻ֧��һ������.*/
        if (VOS_TRUE != stFCPriOrg.ulUsed)
        {
            /* ע��NDIS�˿ڵ����ص� */
            AT_RegNdisFCPoint(pstEvent, FC_ID_NIC_1, enModemId);
        }
        else
        {
            if (TAF_USED == pstEvent->bitOpUmtsQos)
            {
                enFCPriCurrent = AT_GetFCPriFromQos(&pstEvent->stUmtsQos);
            }
            else
            {
                enFCPriCurrent = FC_PRI_FOR_PDN_NONGBR;
            }

            /* �����ǰFC���ȼ���֮ǰ���ص�FC���ȼ��ߣ���ô�������ȼ���*/
            if(enFCPriCurrent > stFCPriOrg.enFcPri)
            {
                AT_ChangeFCPoint(&pstEvent->stCtrl,enFCPriCurrent, FC_ID_NIC_1);
            }
        }
    }

}


VOS_VOID  AT_NdisIpv4ActCnfProc(
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU        *pstEvent
)
{
    VOS_UINT8                           ucCid;
    VOS_UINT32                          ulRet;

    g_stAtNdisDhcpPara.ucIpv4Cid = pstEvent->ucCid;

    /* ����IPV4���͵�DHCP */
    AT_CtrlConnIndProc(pstEvent, AT_NDIS_USER);

    /* NDIS�û����͵�PDP״̬�ı��,�������ص�ע����� */
    AT_ProcNdisRegFCPoint(pstEvent, TAF_PDP_IPV4);

    /* ��IPV4��PDN��Ϣ���͸�NDISģ�� */
    AT_SendNdisIPv4PdnInfoCfgReq(AT_NDIS_GET_USR_PORT_INDEX(),
                                 &(g_stAtNdisDhcpPara.stIpv4Dhcp));

    /* �����IPV4V6����Ҫ������һ��PDP */
    if (TAF_PDP_IPV4V6 == g_enAtFirstNdisActPdpType)
    {
        switch (g_enAtNdisActPdpType)
        {
            /* �������������PDP���ˣ���ǰ��PDP����ܾ��󣬷ֱ���IPV4��
               IPV6��PDP������У�TAF_PDP_IPV6���쳣������û�����IPV6��PDP
               ���������ظ�IPV4�ļ����ʱҲ��Ҫ���·���IPV6��PDP���� */
            case TAF_PDP_IPV4:
                if (AT_PDP_STATE_IDLE == AT_NdisGetState(TAF_PDP_IPV6))
                {
                    /* ����һ��δ�����CID���м��� */
                    ulRet       = TAF_AGENT_FindCidForDial(pstEvent->stCtrl.usClientId, &ucCid);
                    if (VOS_OK == ulRet )
                    {
                        AT_NdisPdpActOrig(pstEvent->stCtrl.usClientId, ucCid, TAF_PDP_IPV6);
                    }
                }
                break;

            /* ������PDP������˹���ʵ�� */
            case TAF_PDP_IPV4V6:
                /* ����PDP�л���IDLE̬ */
                AT_NdisSetState(g_enAtNdisActPdpType, AT_PDP_STATE_IDLE);

                AT_NdisActCnfBackProc(pstEvent);
                break;

            default:
                AT_WARN_LOG("AT_NdisIpv4ActCnfProc:g_enAtNdisActPdpType is error!");
                break;
        }
    }

    return;
}


VOS_VOID  AT_NdisPsRspPdpActEvtCnfProc(
    VOS_UINT8                           ucIndex,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    /*��������*/
    AT_ActiveUsbNet();

    /* ֪ͨPC���� */
    AT_NdisStateChangeProc(pstEvent->stPdpAddr.enPdpType, AT_PDP_STATE_ACTED, TAF_PS_CAUSE_SUCCESS);

    /* ����PDP ����״̬�л���act̬ */
    AT_NdisSetState(pstEvent->stPdpAddr.enPdpType, AT_PDP_STATE_ACTED);

    /* ����PDP���ͷֱ���*/
    switch ( pstEvent->stPdpAddr.enPdpType )
    {
        case TAF_PDP_IPV4:
            AT_NdisIpv4ActCnfProc(pstEvent);
            break;

        case TAF_PDP_IPV6:
            AT_NdisIpv6ActCnfProc(pstEvent);
            break;

        case TAF_PDP_IPV4V6:
            AT_NdisIpv4v6ActCnfProc(pstEvent);
            break;

        default:
            AT_WARN_LOG("AT_NdisPsRspPdpActEvtCnfProc:pdp type invaild!");
            break;
    }

    return;
}


VOS_VOID AT_NdisPdpActOrig(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCid,
    TAF_PDP_TYPE_ENUM_UINT8             ucPdpType
)
{
    TAF_PS_DIAL_PARA_STRU               stDialParaInfo;

    TAF_MEM_SET_S(&stDialParaInfo, sizeof(stDialParaInfo), 0x00, sizeof(TAF_PS_DIAL_PARA_STRU));

    /* ����QOS���� */
    if (VOS_OK != AT_PS_SetQosPara(usClientId, AT_NDIS_GET_USR_CID(), ucCid))
    {
        return;
    }

    AT_GetPsDialParamFromAtDialParam(&stDialParaInfo,&gstAtNdisAddParam);
    stDialParaInfo.ucCid        = ucCid;
    stDialParaInfo.enPdpType    = ucPdpType;

    stDialParaInfo.ucRmnetId    = PS_IFACE_ID_NDIS0;

    /* ����PDP���� */
    if (VOS_OK == TAF_PS_CallOrig(WUEPS_PID_AT,
                                  AT_PS_BuildNdisExClientId(AT_NDIS_GET_USR_PORT_INDEX(), usClientId),
                                  0, &stDialParaInfo))
    {
        if (TAF_PDP_IPV4 == stDialParaInfo.enPdpType)
        {
            g_stAtNdisDhcpPara.ucIpv4Cid = ucCid;
        }
        else
        {
            g_stAtNdisDhcpPara.ucIpv6Cid = ucCid;
        }

        g_enAtNdisActPdpType = ucPdpType;

        AT_NdisSetState(ucPdpType, AT_PDP_STATE_ACTING);
    }

    return;
}



VOS_VOID  AT_NdisPsRspPdpActEvtRejProc(
    VOS_UINT8                           ucIndex,
    TAF_PS_CALL_PDP_ACTIVATE_REJ_STRU  *pstEvent
)
{
    AT_PDP_ENTITY_STRU                 *pstNdisPdpEntity;
    AT_COMM_PS_CTX_STRU                *pstCommPsCtx = VOS_NULL_PTR;

    AT_PDP_STATE_ENUM_U8                enPreIpv4State;

    /* ��ʼ�� */
    pstNdisPdpEntity = AT_NDIS_GetPdpEntInfoAddr();
    pstCommPsCtx     = AT_GetCommPsCtxAddr();
    enPreIpv4State   = pstNdisPdpEntity->enIpv4State;

    /* �����IPV4 ONLY��IPV6 ONLYֱ���ϱ�����ʧ�ܡ�IPV4V6
       ����Ҫ�Ӿ���ʵ�ֱַ���IPv4��IPv6���͵�PDP���� */
    if (TAF_PDP_IPV4V6 == g_enAtFirstNdisActPdpType)
    {
        if (TAF_PDP_IPV4V6 == g_enAtNdisActPdpType)
        {
            /* �����������Ҫ�ֱ���IPv4��IPv6��PDP���� */
            if ( (TAF_PS_CAUSE_SM_NW_UNKNOWN_PDP_ADDR_OR_TYPE == pstEvent->enCause)
              && (VOS_TRUE == pstCommPsCtx->stRedialForNwCauseCfg.ucRedialForNoPdpTypeCausePolicy))
            {
                /* �����𲦺ŵ�PDPtype״̬�л���IDLE */
                AT_NdisSetState(g_enAtNdisActPdpType, AT_PDP_STATE_IDLE);

                AT_NdisPdpActOrig(pstEvent->stCtrl.usClientId, pstEvent->ucCid, TAF_PDP_IPV4);
            }
            else
            {
                /* ����PDP�л���IDLE̬ */
                AT_NdisSetState(g_enAtNdisActPdpType, AT_PDP_STATE_IDLE);

                /* ֪ͨPDP����ʧ�� */
                AT_NdisStateChangeProc(g_enAtNdisActPdpType, AT_PDP_STATUS_DEACT, pstEvent->enCause);

                /* ���NDIS���Ų��� */
                TAF_MEM_SET_S(&gstAtNdisAddParam, sizeof(gstAtNdisAddParam), 0x00, sizeof(AT_DIAL_PARAM_STRU));
            }
        }
        else if ( TAF_PDP_IPV4 == g_enAtNdisActPdpType )
        {
            /* �û�����IPv4v6���͵�PDP����, ���ұ�����ܾ�, ԭ��Ϊ28, Э��ջ��Ҫ
               �ֱ���IPv4/IPv6���͵�PDP����, Э��ջ���ȷ���IPv4, �ٷ���IPv6,
               ���IPV4���͵�PDP�����ٴα�����ܾ�, Э��ջ����Ҫ����IPV6���͵�
               PDP����Ϊ�˷�ֹPDP����Ƕ��, ���IPv6���͵�PDP����ʧ��, �����ٳ���
               IPv4���͵�PDP���� */
            AT_NdisSetState(g_enAtNdisActPdpType, AT_PDP_STATE_IDLE);

            /* ֪ͨPDP����ʧ�� */
            AT_NdisStateChangeProc(g_enAtNdisActPdpType, AT_PDP_STATUS_DEACT, pstEvent->enCause);

            if ( (AT_PDP_STATE_IDLE == AT_NdisGetState(TAF_PDP_IPV6))
              && (AT_PDP_STATE_ACTING == enPreIpv4State) )
            {
                /* ���IPv6�����Ƿ��Ѿ�����, ���������, ����IPv6���͵�PDP���� */
                AT_NdisPdpActOrig(pstEvent->stCtrl.usClientId, pstEvent->ucCid, TAF_PDP_IPV6);
            }
            else
            {
                /* ���NDIS���Ų��� */
                TAF_MEM_SET_S(&gstAtNdisAddParam, sizeof(gstAtNdisAddParam), 0x00, sizeof(AT_DIAL_PARAM_STRU));
            }
        }
        else
        {
            /* ���IPv6����, �Ͳ���Ҫ�ٳ���IPv4, ��Ϊǰ���Ѿ������IPv4���͵�PDP
               ����, ����ٷ���IPv4���͵�PDP����Ļ�, ���ܻᵼ��PDP����Ƕ�� */
             AT_NdisSetState(g_enAtNdisActPdpType, AT_PDP_STATE_IDLE);

             /* ֪ͨPDP����ʧ�� */
            AT_NdisStateChangeProc(g_enAtNdisActPdpType, AT_PDP_STATUS_DEACT, pstEvent->enCause);

             /* ���NDIS���Ų��� */
             TAF_MEM_SET_S(&gstAtNdisAddParam, sizeof(gstAtNdisAddParam), 0x00, sizeof(AT_DIAL_PARAM_STRU));
        }
    }
    else
    {
        /* ����PDP�л���IDLE̬ */
        AT_NdisSetState(g_enAtNdisActPdpType, AT_PDP_STATE_IDLE);

        /* ֪ͨPDP����ʧ�� */
        AT_NdisStateChangeProc(g_enAtNdisActPdpType, AT_PDP_STATUS_DEACT, pstEvent->enCause);

        /* ���NDIS���Ų��� */
        TAF_MEM_SET_S(&gstAtNdisAddParam, sizeof(gstAtNdisAddParam), 0x00, sizeof(AT_DIAL_PARAM_STRU));
    }

    return;
}


VOS_VOID AT_NdisIPv4DeactiveCnfProc(
    TAF_PS_CALL_PDP_DEACTIVATE_CNF_STRU *pstEvent
)
{
    AT_PDP_STATE_ENUM_U8                enState;

    AT_NdisSetState(TAF_PDP_IPV4, AT_PDP_STATE_IDLE);
    AT_NdisDhcpRest();

    /* NDIS�û����͵�PDP״̬�ı��Ĵ������� */
    AT_ProcNdisDeRegFCPoint(pstEvent, TAF_PDP_IPV4);

    /* �������һ��PDP�����ڼ���״̬����Ҫ����ȥ���� */
    enState     = AT_NdisGetState(TAF_PDP_IPV6);
    if ( (AT_PDP_STATE_IDLE     != enState)
      && (AT_PDP_STATE_DEACTING != enState) )
    {
        if (AT_SUCCESS == TAF_PS_CallEnd(WUEPS_PID_AT,
                                         AT_PS_BuildNdisExClientId(AT_NDIS_GET_USR_PORT_INDEX(), pstEvent->stCtrl.usClientId),
                                         0,
                                         g_stAtNdisDhcpPara.ucIpv6Cid))
        {
            AT_NdisSetState(TAF_PDP_IPV6, AT_PDP_STATE_DEACTING);
        }
    }
}


VOS_VOID AT_NdisIPv6DeactiveCnfProc(
    TAF_PS_CALL_PDP_DEACTIVATE_CNF_STRU *pstEvent
)
{
    AT_PDP_STATE_ENUM_U8                enState;

    AT_NdisSetState(TAF_PDP_IPV6, AT_PDP_STATE_IDLE);

    AT_NdisIpv6DhcpRest();

    /* NDIS�û����͵�PDP״̬�ı��Ĵ������� */
    AT_ProcNdisDeRegFCPoint(pstEvent, TAF_PDP_IPV6);

    /*�������һ��PDP�����ڼ���״̬����Ҫ����ȥ����*/
    enState = AT_NdisGetState(TAF_PDP_IPV4);
    if ( (AT_PDP_STATE_IDLE     != enState)
      && (AT_PDP_STATE_DEACTING != enState) )
    {
        if (AT_SUCCESS == TAF_PS_CallEnd(WUEPS_PID_AT,
                                         AT_PS_BuildNdisExClientId(AT_NDIS_GET_USR_PORT_INDEX(), pstEvent->stCtrl.usClientId),
                                         0,
                                         g_stAtNdisDhcpPara.ucIpv4Cid))
        {
            AT_NdisSetState(TAF_PDP_IPV4,AT_PDP_STATE_DEACTING);
        }
    }
}


VOS_VOID AT_NdisIPv4v6DeactiveCnfProc(
    TAF_PS_CALL_PDP_DEACTIVATE_CNF_STRU *pstEvent
)
{
    AT_NdisDhcpRest();

    AT_NdisIpv6DhcpRest();

    /* NDIS�û����͵�PDP״̬�ı��Ĵ������� */
    AT_ProcNdisDeRegFCPoint(pstEvent, TAF_PDP_IPV4V6);
}


VOS_VOID  AT_NdisPsRspPdpDeactEvtCnfProc(
    VOS_UINT8                           ucIndex,
    TAF_PS_CALL_PDP_DEACTIVATE_CNF_STRU *pstEvent
)
{
    /* ֪ͨPCȥ���� */
    AT_NdisStateChangeProc(pstEvent->enPdpType, AT_PDP_STATUS_DEACT, pstEvent->enCause);

    /* ֪ͨTTFȥ������Ӧ��RAB */
    AT_SendNdisRelReq(pstEvent);

    /* �л�NDIS״̬��IDLE */
    AT_NdisSetState(pstEvent->enPdpType, AT_PDP_STATE_IDLE);

    if (TAF_PDP_IPV4 == pstEvent->enPdpType)
    {
        AT_NdisIPv4DeactiveCnfProc(pstEvent);
    }
    else if (TAF_PDP_IPV6 == pstEvent->enPdpType)
    {
        AT_NdisIPv6DeactiveCnfProc(pstEvent);
    }
    else if (TAF_PDP_IPV4V6 == pstEvent->enPdpType)
    {
        AT_NdisIPv4v6DeactiveCnfProc(pstEvent);
    }
    else
    {
        ;
    }

    /* ���NDIS���Ų��� */
    if ( (AT_PDP_STATE_IDLE == g_stAtNdisDhcpPara.enIpv4State)
      && (AT_PDP_STATE_IDLE == g_stAtNdisDhcpPara.enIpv6State)
      && (AT_PDP_STATE_IDLE == g_stAtNdisDhcpPara.enIpv4v6State) )
    {
        TAF_MEM_SET_S(&gstAtNdisAddParam, sizeof(gstAtNdisAddParam), 0x00, sizeof(AT_DIAL_PARAM_STRU));
    }

    /* ֪ͨ����ȥ���� */
    AT_DeActiveUsbNet();

    return;
}


VOS_VOID  AT_NdisPsRspPdpDeactivatedEvtProc(
    VOS_UINT8                           ucIndex,
    TAF_PS_CALL_PDP_DEACTIVATE_IND_STRU *pstEvent
)
{

    /* �ϱ�^NDISSTATָʾNDIS״̬ */
    AT_NdisStateChangeProc(pstEvent->enPdpType, AT_PDP_STATUS_DEACT, pstEvent->enCause);

    /* ֪ͨTTFȥ������Ӧ��RAB */
    AT_SendNdisRelReq(pstEvent);

    /* �л�NDIS״̬��IDLE */
    AT_NdisSetState(pstEvent->enPdpType, AT_PDP_STATE_IDLE);

    if (TAF_PDP_IPV4 == pstEvent->enPdpType)
    {
        AT_NdisIPv4DeactiveCnfProc(pstEvent);
    }
    else if (TAF_PDP_IPV6 == pstEvent->enPdpType)
    {
        AT_NdisIPv6DeactiveCnfProc(pstEvent);
    }
    else if (TAF_PDP_IPV4V6 == pstEvent->enPdpType)
    {
        AT_NdisIPv4v6DeactiveCnfProc(pstEvent);
    }
    else
    {
        ;
    }

    /* ���NDIS���Ų��� */
    if ( (AT_PDP_STATE_IDLE == g_stAtNdisDhcpPara.enIpv4State)
      && (AT_PDP_STATE_IDLE == g_stAtNdisDhcpPara.enIpv6State)
      && (AT_PDP_STATE_IDLE == g_stAtNdisDhcpPara.enIpv4v6State) )
    {
        TAF_MEM_SET_S(&gstAtNdisAddParam, sizeof(gstAtNdisAddParam), 0x00, sizeof(AT_DIAL_PARAM_STRU));
    }

    /* ֪ͨ����ȥ���� */
    AT_DeActiveUsbNet();

    return;
}


VOS_UINT32 AT_EnableNdisFlowCtl(
    VOS_UINT32                          ulParam1,
    VOS_UINT32                          ulParam2
)
{
    /* ͨ��udi_ioctl����ʹ������ */
    VOS_UINT32  ulEnbflg = NCM_IOCTL_FLOW_CTRL_ENABLE;

    if (0 != mdrv_udi_ioctl (g_ulAtUdiNdisHdl, NCM_IOCTL_FLOW_CTRL_NOTIF, (VOS_VOID*)(&ulEnbflg)))
    {
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_DisableNdisFlowCtl(
    VOS_UINT32                          ulParam1,
    VOS_UINT32                          ulParam2
)
{
    /* ͨ��udi_ioctl����ȥʹ������ */
    VOS_UINT32  ulEnbflg = NCM_IOCTL_FLOW_CTRL_DISABLE;

    if (0 != mdrv_udi_ioctl (g_ulAtUdiNdisHdl, NCM_IOCTL_FLOW_CTRL_NOTIF, (VOS_VOID*)(&ulEnbflg)))
    {
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_RegNdisFCPoint(
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent,
    FC_ID_ENUM_UINT8                    enFcId,
    MODEM_ID_ENUM_UINT16                enModemId
)
{
    FC_REG_POINT_STRU                   stRegFcPoint;
    VOS_UINT32                          ulRet;
    FC_PRI_ENUM_UINT8                   enFCPri;

    TAF_MEM_SET_S(&stRegFcPoint, sizeof(stRegFcPoint), 0x00, sizeof(FC_REG_POINT_STRU));

    /* ����ͨ����RABIDӳ���ϵ */
    FC_ChannelMapCreate(enFcId, pstEvent->ucRabId, enModemId);

    stRegFcPoint.enFcId             = enFcId;

    /* ����������������ȼ�RAB QoS���ȼ�������,���ȼ��ı�ʱ����Ҫ�ı����ȼ� */
    /*  FC_PRI_3        ��������ȼ��ĳ���
        FC_PRI_4        ��NONGBR����
        FC_PRI_5        ��GBR���� */

    if (TAF_USED == pstEvent->bitOpUmtsQos)
    {
        enFCPri = AT_GetFCPriFromQos(&pstEvent->stUmtsQos);
    }
    else
    {
        enFCPri = FC_PRI_FOR_PDN_NONGBR;
    }
    stRegFcPoint.enFcPri            = enFCPri;
    stRegFcPoint.enPolicyId         = FC_POLICY_ID_MEM;
    stRegFcPoint.enModemId          = enModemId;
    stRegFcPoint.pClrFunc           = AT_DisableNdisFlowCtl;
    stRegFcPoint.pSetFunc           = AT_EnableNdisFlowCtl;
    stRegFcPoint.pRstFunc           = AT_ResetFlowCtl;
    stRegFcPoint.ulParam2           = enFcId;
    stRegFcPoint.ulParam1           = (VOS_UINT32)pstEvent->stCtrl.usClientId;

    /* ע�����ص�,��Ҫ�ֱ�ע��MEM,CPU,CDS��GPRS�� */
    ulRet = FC_RegPoint(&stRegFcPoint);
    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_RegNdisFCPoint: ERROR: reg mem point Failed.");
        return VOS_ERR;
    }

    stRegFcPoint.enPolicyId         = FC_POLICY_ID_CPU_A;
    ulRet = FC_RegPoint(&stRegFcPoint);
    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_RegNdisFCPoint: ERROR: reg a cpu point Failed.");
        return VOS_ERR;
    }

    stRegFcPoint.enPolicyId         = FC_POLICY_ID_CDS;
    ulRet = FC_RegPoint(&stRegFcPoint);
    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_RegNdisFCPoint: ERROR: reg cds point Failed.");
        return VOS_ERR;
    }

    stRegFcPoint.enPolicyId         = FC_POLICY_ID_GPRS;
    ulRet = FC_RegPoint(&stRegFcPoint);
    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_RegNdisFCPoint: ERROR: reg gprs point Failed.");
        return VOS_ERR;
    }


    /* ����FCID��FC Pri��ӳ���ϵ */
    g_stFcIdMaptoFcPri[FC_ID_NIC_1].ulUsed      = VOS_TRUE;
    g_stFcIdMaptoFcPri[FC_ID_NIC_1].enFcPri     = enFCPri;
    /* ��һ�������϶��RABID�������������Ҫ�����RABID��¼���� */
    g_stFcIdMaptoFcPri[FC_ID_NIC_1].ulRabIdMask |= ((VOS_UINT32)1 << (pstEvent->ucRabId));
    g_stFcIdMaptoFcPri[FC_ID_NIC_1].enModemId   = enModemId;

    /* ��������Ϣ */
    AT_MNTN_TraceRegFcPoint((VOS_UINT8)pstEvent->stCtrl.usClientId, AT_FC_POINT_TYPE_NDIS);

    return VOS_OK;
}


VOS_UINT32 AT_DeRegNdisFCPoint(
    VOS_UINT8                           ucRabId,
    MODEM_ID_ENUM_UINT16                enModemId
)
{
    FC_ID_ENUM_UINT8                    enFcId;
    VOS_UINT32                          ulRet;

    /* �ڵ���FC_DeRegPointǰ,�ȵ���FC_ChannelMapDelete */
    FC_ChannelMapDelete(ucRabId, enModemId);

    /* ĿǰNDISֻ֧��һ������ */
    enFcId = FC_ID_NIC_1;

    ulRet = FC_DeRegPoint(enFcId, enModemId);
    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_DeRegNdisFCPoint: ERROR: de reg point Failed.");
        return VOS_ERR;
    }

    /* ���FCID��FC Pri��ӳ���ϵ */
    g_stFcIdMaptoFcPri[FC_ID_NIC_1].ulUsed      = VOS_FALSE;
    g_stFcIdMaptoFcPri[FC_ID_NIC_1].enFcPri     = FC_PRI_BUTT;
    /* ��һ�������϶��RABID�������������Ҫ����Ӧ��RABID��������� */
    g_stFcIdMaptoFcPri[FC_ID_NIC_1].ulRabIdMask &= ~((VOS_UINT32)1 << ucRabId);
    g_stFcIdMaptoFcPri[FC_ID_NIC_1].enModemId   = MODEM_ID_BUTT;

    /* ��������Ϣ */
    AT_MNTN_TraceDeregFcPoint(AT_CLIENT_TAB_NDIS_INDEX, AT_FC_POINT_TYPE_NDIS);

    return VOS_OK;
}


VOS_VOID AT_NDIS_ParseUsrInfo(
    VOS_UINT8                           ucIndex,
    AT_PS_USER_INFO_STRU               *pstUsrInfo
)
{
    AT_CLIENT_TAB_INDEX_UINT8           enUsrIndex;

    if (VOS_TRUE == AT_CheckNdisUser(ucIndex))
    {
        enUsrIndex = ucIndex;
    }
    else
    {
        enUsrIndex = AT_CLIENT_TAB_NDIS_INDEX;
    }

    pstUsrInfo->enPortIndex = ucIndex;
    pstUsrInfo->enUserIndex = enUsrIndex;
    pstUsrInfo->ucUsrType   = (AT_USER_TYPE)gastAtClientTab[enUsrIndex].UserType;
    pstUsrInfo->ucUsrCid    = (VOS_UINT8)gastAtParaList[0].ulParaValue;

    return;
}


VOS_VOID AT_NDIS_ConnStatusChgProc(NCM_IOCTL_CONNECT_STUS_E enStatus)
{
    if (NCM_IOCTL_STUS_BREAK == enStatus)
    {
        /* ������е�PDP������IDLE������OK; �����һ������DEACTINGҲ����OK */
        if (VOS_TRUE == AT_NdisCheckPdpIdleState())
        {
            return;
        }

        if ( (AT_PDP_STATE_ACTING == g_stAtNdisDhcpPara.enIpv4v6State)
          || (AT_PDP_STATE_ACTED  == g_stAtNdisDhcpPara.enIpv4v6State) )
        {
            if (VOS_OK == TAF_PS_CallEnd(WUEPS_PID_AT,
                                         AT_PS_BuildNdisExClientId(AT_NDIS_GET_USR_PORT_INDEX(), AT_NDIS_GET_CLIENT_ID()),
                                         0,
                                         g_stAtNdisDhcpPara.ucIpv4v6Cid) )
            {
                AT_NdisSetState(TAF_PDP_IPV4V6, AT_PDP_STATE_DEACTING);
            }
        }
        else
        {
            if ( (AT_PDP_STATE_ACTING == g_stAtNdisDhcpPara.enIpv4State)
              || (AT_PDP_STATE_ACTED  == g_stAtNdisDhcpPara.enIpv4State) )
            {
                if (VOS_OK == TAF_PS_CallEnd(WUEPS_PID_AT,
                                             AT_PS_BuildNdisExClientId(AT_NDIS_GET_USR_PORT_INDEX(), AT_NDIS_GET_CLIENT_ID()),
                                             0,
                                             g_stAtNdisDhcpPara.ucIpv4Cid))
                {
                    AT_NdisSetState(TAF_PDP_IPV4, AT_PDP_STATE_DEACTING);
                }
            }

            if ( (AT_PDP_STATE_ACTING == g_stAtNdisDhcpPara.enIpv6State)
              || (AT_PDP_STATE_ACTED  == g_stAtNdisDhcpPara.enIpv6State) )
            {
                if (VOS_OK == TAF_PS_CallEnd(WUEPS_PID_AT,
                                             AT_PS_BuildNdisExClientId(AT_NDIS_GET_USR_PORT_INDEX(), AT_NDIS_GET_CLIENT_ID()),
                                             0,
                                             g_stAtNdisDhcpPara.ucIpv6Cid))
                {
                    AT_NdisSetState(TAF_PDP_IPV6, AT_PDP_STATE_DEACTING);
                }
            }
        }
    }

    return;
}

/***************************************************************************
               ���´���ʵ��APP��ع���
*****************************************************************************/


VOS_UINT32 AT_CalcIpv6LanAddrFromIpv6Prefix(
    VOS_UINT8                          *pucPrefix,
    VOS_UINT32                          ulPrefixByteLen,
    VOS_UINT8                          *pucIpv6LanAddr,
    VOS_UINT32                          ulIpv6LanAddrLen
)
{
    VOS_UINT16                         *pausAddr;
    VOS_UINT8                           aucLanAddr[AT_MAC_LEN] = { 0x58, 0x2C, 0x80, 0x13, 0x92, 0x08 };

    if ( (ulIpv6LanAddrLen < TAF_IPV6_ADDR_LEN )
      || (ulPrefixByteLen > AT_IPV6_ADDR_PREFIX_BYTE_LEN) )
    {
        return VOS_ERR;
    }

    pausAddr = (VOS_UINT16 *)pucIpv6LanAddr;

    /* ����IPv6��ַǰ׺ */
    TAF_MEM_CPY_S(pucIpv6LanAddr, TAF_IPV6_ADDR_LEN, pucPrefix, ulPrefixByteLen);

    /*--------------------------------------------------------------
       MAC��ַ���ɽӿ�ID����16λ��0xFFFE����MAC��ַ�м�
       MAC��ַǰ24λ + 0xFFFE + MAC��ַ��24λ
       �ٽ�U/Lλ(ȫ��/���ص�ַ,MAC��ַ����ֽڵĵ�һλ)ȡ��
    --------------------------------------------------------------*/
    pausAddr[4] = ((VOS_UINT16 *)aucLanAddr)[0] ^ VOS_HTONS(0x0200);
    pausAddr[5] = VOS_HTONS(0x00ff) | ((VOS_UINT16 *)aucLanAddr)[1];
    pausAddr[6] = VOS_HTONS(0xfe00) | (((VOS_UINT16 *)aucLanAddr)[1] & VOS_HTONS(0x00ff));
    pausAddr[7] = ((VOS_UINT16 *)aucLanAddr)[2];

    return VOS_OK;
}


VOS_VOID AT_AppRcvIpv6LanAddr(
    VOS_UINT8                           aucLanAddr[],
    VOS_UINT32                          ulLanAddrByteLen,
    VOS_UINT32                          ulPrefixBitLen
)
{
    VOS_UINT16                          usLength;
    VOS_UINT8                           aucIpv6AddrStr[TAF_MAX_IPV6_ADDR_COLON_STR_LEN];

    /* ��ȫ�ֵ�ַת��Ϊѹ����ʽ */
    AT_ConvertIpv6AddrToCompressedStr(aucIpv6AddrStr,
                                      aucLanAddr,
                                      TAF_IPV6_STR_RFC2373_TOKENS);

    /* �ϱ�LANȫ�ֵ�ַ */
    usLength = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                      (VOS_CHAR*)pgucAtSndCodeAddr,
                                      (VOS_CHAR*)pgucAtSndCodeAddr,
                                      "%s^APLANADDR: \"%s\",%d%s",
                                      gaucAtCrLf,
                                      aucIpv6AddrStr,
                                      ulPrefixBitLen,
                                      gaucAtCrLf);
    At_SendResultData(AT_APP_GET_USR_PORT_INDEX(), pgucAtSndCodeAddr, usLength);

    return;
}


VOS_VOID AT_AppRcvIpv6Prefix(
    VOS_UINT8                           aucPrefix[],
    VOS_UINT32                          ulPrefixBitLen
)
{
    VOS_UINT8                           aucPrefixStr[TAF_MAX_IPV6_ADDR_COLON_STR_LEN];
    VOS_UINT16                          usLength;
    VOS_UINT8                           ucTokensNum;

    /* ��ǰ׺ת����ѹ����ʽ */
    ucTokensNum = TAF_IPV6_STR_RFC2373_TOKENS;
    AT_ConvertIpv6AddrToCompressedStr(aucPrefixStr, aucPrefix, ucTokensNum);

    /* �ϱ�^APPREFIX: <PrefixAddr> */
    usLength = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                      (VOS_CHAR*)pgucAtSndCodeAddr,
                                      (VOS_CHAR*)pgucAtSndCodeAddr,
                                      "%s^APPREFIX: \"%s/%d\"%s",
                                      gaucAtCrLf,
                                      aucPrefixStr,
                                      ulPrefixBitLen,
                                      gaucAtCrLf);
    At_SendResultData(AT_APP_GET_USR_PORT_INDEX(), pgucAtSndCodeAddr, usLength);
    return;
}


VOS_VOID AT_AppRcvPrefixIpv6PreferredLifetime(
    VOS_UINT32                          ulPrefixPreferredLifetime
)
{
    VOS_UINT16                          usLength;

    /* �ϱ�^APPREFERREDLIFETIME: <PreferredLifetime> */
    usLength = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                      (VOS_CHAR*)pgucAtSndCodeAddr,
                                      (VOS_CHAR*)pgucAtSndCodeAddr,
                                      "%s^APPREFERREDLIFETIME: %u%s",
                                      gaucAtCrLf,
                                      ulPrefixPreferredLifetime,
                                      gaucAtCrLf);
    At_SendResultData(AT_APP_GET_USR_PORT_INDEX(), pgucAtSndCodeAddr, usLength);

    return;
}


VOS_VOID AT_AppRcvPrefixIpv6ValidLifetime(
    VOS_UINT32                          ulValidLifetime
)
{
    VOS_UINT16                          usLength;

    /* �ϱ�^APVALIDLIFETIME: <ValidLifetime> */
    usLength = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                      (VOS_CHAR*)pgucAtSndCodeAddr,
                                      (VOS_CHAR*)pgucAtSndCodeAddr,
                                      "%s^APVALIDLIFETIME: %u%s",
                                      gaucAtCrLf,
                                      ulValidLifetime,
                                      gaucAtCrLf);
    At_SendResultData(AT_APP_GET_USR_PORT_INDEX(), pgucAtSndCodeAddr, usLength);

    return;
}


VOS_VOID AT_AppRcvIpv6LinkMtu(
    VOS_UINT32                          ulMtu
)
{
    VOS_UINT16                          usLength;

    /* �ϱ�^IPV6MTU: <MTU> */
    usLength = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                      (VOS_CHAR*)pgucAtSndCodeAddr,
                                      (VOS_CHAR*)pgucAtSndCodeAddr,
                                      "%s^APIPV6MTU: %d%s",
                                      gaucAtCrLf,
                                      ulMtu,
                                      gaucAtCrLf);
    At_SendResultData(AT_APP_GET_USR_PORT_INDEX(), pgucAtSndCodeAddr, usLength);
    return;
}


VOS_VOID AT_AppRcvEnableIpv6Prefix(VOS_VOID)
{
    VOS_UINT16                          usLength;
    VOS_UINT8                           ucStatus;

    ucStatus                            = 1;

    /* �ϱ�^APIPV6ENABLE: <status> */
    usLength = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                      (VOS_CHAR*)pgucAtSndCodeAddr,
                                      (VOS_CHAR*)pgucAtSndCodeAddr,
                                      "%s^APIPV6ENABLE: %d%s",
                                      gaucAtCrLf,
                                      ucStatus,
                                      gaucAtCrLf);
    At_SendResultData(AT_APP_GET_USR_PORT_INDEX(), pgucAtSndCodeAddr, usLength);

    return;
}


VOS_VOID AT_AppProcIpv6RaInfo(TAF_PS_IPV6_INFO_IND_STRU *pstRaInfoNotifyInd)
{
    AT_PDP_ENTITY_STRU                 *pstAppPdpEntity;
    AT_IPV6_RA_INFO_STRU               *pstAppRaInfoAddr;
    VOS_UINT8                           aucIpv6LanAddr[TAF_IPV6_ADDR_LEN] = {0};

    pstAppRaInfoAddr = AT_APP_GetRaInfoAddr();
    pstAppPdpEntity  = AT_APP_GetPdpEntInfoAddr();

    if (0 == pstRaInfoNotifyInd->stIpv6RaInfo.ulPrefixNum)
    {
        AT_NORM_LOG("AT_AppProcIpv6RaInfo: No IPv6 prefix address in RA.");
        return;
    }

    /* ��ȡ��IPv6��ַǰ׺��, �ϱ������ӽ��^DCONN */
    if (VOS_FALSE == pstAppRaInfoAddr->bitOpPrefixAddr)
    {
        /* ֪ͨAPPģ��IPv6���ųɹ� */
        AT_AppSndCallConnectedResult(pstRaInfoNotifyInd->ucCid, TAF_PDP_IPV6);
    }

    /* ��¼IPv6ǰ׺ */
    pstAppRaInfoAddr->bitOpPrefixAddr        = VOS_TRUE;
    pstAppRaInfoAddr->ulPrefixBitLen         = pstRaInfoNotifyInd->stIpv6RaInfo.astPrefixList[0].ulBitPrefixLen;
    TAF_MEM_CPY_S(pstAppRaInfoAddr->aucPrefixAddr,
               sizeof(pstAppRaInfoAddr->aucPrefixAddr),
               pstRaInfoNotifyInd->stIpv6RaInfo.astPrefixList[0].aucPrefix,
               TAF_IPV6_ADDR_LEN);

    /* ��¼Preferred Lifetime */
    pstAppRaInfoAddr->bitOpPreferredLifetime = VOS_TRUE;
    pstAppRaInfoAddr->ulPreferredLifetime    = pstRaInfoNotifyInd->stIpv6RaInfo.astPrefixList[0].ulPreferredLifeTime;

    /* ��¼Valid Lifetime */
    pstAppRaInfoAddr->bitOpValidLifetime     = VOS_TRUE;
    pstAppRaInfoAddr->ulValidLifetime        = pstRaInfoNotifyInd->stIpv6RaInfo.astPrefixList[0].ulValidLifeTime;

    /* ��¼IPv6 MTU */
    if (VOS_TRUE == pstRaInfoNotifyInd->stIpv6RaInfo.bitOpMtu)
    {
        pstAppRaInfoAddr->bitOpMtuSize       = VOS_TRUE;
        pstAppRaInfoAddr->ulMtuSize          = pstRaInfoNotifyInd->stIpv6RaInfo.ulMtu;
    }

    /* ����IPv6ȫ�ֵ�ַ */
    AT_CalcIpv6LanAddrFromIpv6Prefix(pstRaInfoNotifyInd->stIpv6RaInfo.astPrefixList[0].aucPrefix,
                                     pstRaInfoNotifyInd->stIpv6RaInfo.astPrefixList[0].ulBitPrefixLen/8,
                                     aucIpv6LanAddr,
                                     TAF_IPV6_ADDR_LEN);

    /* ��¼IPv6ȫ�ֵ�ַ */
    pstAppRaInfoAddr->bitOpLanAddr           = VOS_TRUE;
    TAF_MEM_CPY_S(pstAppRaInfoAddr->aucLanAddr, sizeof(pstAppRaInfoAddr->aucLanAddr), aucIpv6LanAddr, TAF_IPV6_ADDR_LEN);

    /* ����DHCPV6��Ϣ�е�IPv6ȫ�ֵ�ַ */
    TAF_MEM_CPY_S(pstAppPdpEntity->stIpv6Dhcp.aucIpv6Addr, sizeof(pstAppPdpEntity->stIpv6Dhcp.aucIpv6Addr), aucIpv6LanAddr, TAF_IPV6_ADDR_LEN);

    /* �����ϱ�IPV6��ַǰ׺ */
    AT_AppRcvIpv6Prefix(pstAppRaInfoAddr->aucPrefixAddr, pstAppRaInfoAddr->ulPrefixBitLen);

    /* �����ϱ�ǰ׺����ʹ�������� */
    AT_AppRcvPrefixIpv6PreferredLifetime(pstAppRaInfoAddr->ulPreferredLifetime);

    /* �����ϱ�ǰ׺��Ч������ */
    AT_AppRcvPrefixIpv6ValidLifetime(pstAppRaInfoAddr->ulValidLifetime);

    /* �����ϱ�IPV6ȫ�ֵ�ַ */
    AT_AppRcvIpv6LanAddr(pstAppRaInfoAddr->aucLanAddr, TAF_IPV6_ADDR_LEN, pstAppRaInfoAddr->ulPrefixBitLen);

    /* ֪ͨAPP�����µ�IPV6ǰ׺ */
    AT_AppRcvEnableIpv6Prefix();

    return;
}


VOS_VOID AT_AppCtrlConnIpv6IndProc(
    AT_PDP_ENTITY_STRU                 *pstAppPdpEntity,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    pstAppPdpEntity->stIpv6Dhcp.ucRabId         = pstEvent->ucRabId;
    pstAppPdpEntity->stIpv6Dhcp.ucPduSessionId  = pstEvent->ucPduSessionId;

    /* ����IPv6��IP��ַ����ʽΪ������ */
    if (TAF_PDP_IPV6 == (pstEvent->stPdpAddr.enPdpType & TAF_PDP_IPV6))
    {
        TAF_MEM_CPY_S(pstAppPdpEntity->stIpv6Dhcp.aucIpv6Addr,
                   sizeof(pstAppPdpEntity->stIpv6Dhcp.aucIpv6Addr),
                   pstEvent->stPdpAddr.aucIpv6Addr,
                   TAF_IPV6_ADDR_LEN);
    }

    /* ����IPV6������DNS��ַ����ʽΪ������ */
    AT_SaveIPv6Dns(pstEvent, pstAppPdpEntity);

    /* ����IPV6������PCSCF��ַ����ʽΪ������ */
    AT_SaveIPv6Pcscf(pstEvent, pstAppPdpEntity);

    return;
}


VOS_VOID AT_AppDhcpv6Reset(VOS_VOID)
{
    AT_PDP_ENTITY_STRU                 *pstAppPdpEntity;
    AT_IPV6_RA_INFO_STRU               *pucAppRaInfoAddr;

    pstAppPdpEntity                     = AT_APP_GetPdpEntInfoAddr();
    pucAppRaInfoAddr                    = AT_APP_GetRaInfoAddr();

    TAF_MEM_SET_S(&pstAppPdpEntity->stIpv6Dhcp,
               sizeof(pstAppPdpEntity->stIpv6Dhcp),
               0x00,
               sizeof(AT_IPV6_DHCP_PARAM_STRU));

    TAF_MEM_SET_S(pucAppRaInfoAddr,
               sizeof(AT_IPV6_RA_INFO_STRU),
               0x00,
               sizeof(AT_IPV6_RA_INFO_STRU));

    return;
}


VOS_VOID AT_AppDhcpReset(VOS_VOID)
{
    AT_PDP_ENTITY_STRU                 *pstAppPdpEntity;

    pstAppPdpEntity                     = AT_APP_GetPdpEntInfoAddr();

    TAF_MEM_SET_S(&pstAppPdpEntity->stIpv4Dhcp,
               sizeof(pstAppPdpEntity->stIpv4Dhcp),
               0x00,
               sizeof(AT_IPV4_DHCP_PARAM_STRU));

    return;
}


VOS_VOID AT_AppPdpAddrProc(
    AT_PDP_ENTITY_STRU                 *pstAppPdpEntity,
    AT_DHCP_CONFIG_STRU                *pstDhcpConfig,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    AT_DIAL_PARAM_STRU                 *pstAppDialPara;
    VOS_UINT32                          ulIpAddr;
    VOS_UINT8                           ucIndex;

    pstAppDialPara                      = AT_APP_GetDailParaAddr();
    ulIpAddr                            = 0;

    pstAppPdpEntity->stIpv4Dhcp.ucRabId                 = pstEvent->ucRabId;
    pstAppPdpEntity->stIpv4Dhcp.ucPduSessionId          = pstEvent->ucPduSessionId;
    pstAppPdpEntity->stIpv4Dhcp.ulIpv4Addr              = pstDhcpConfig->ulIPAddr;
    pstAppPdpEntity->stIpv4Dhcp.ulIpv4GateWay           = pstDhcpConfig->ulGateWay;
    pstAppPdpEntity->stIpv4Dhcp.ulIpv4NetMask           = pstDhcpConfig->ulSubNetMask;

    /* ����û���������DNS����ʹ���û����õ�DNS�����緵�ص�DNS��ʹ�� */
    if (VOS_TRUE == pstAppDialPara->ulPrimIPv4DNSValidFlag)
    {
        if (VOS_ERR == AT_LenStr2IpAddr(pstAppDialPara->aucPrimIPv4DNSAddr,
                                        (VOS_UINT8*)&ulIpAddr))
        {
            ulIpAddr = 0;
        }

        pstAppPdpEntity->stIpv4Dhcp.bitOpIpv4PriDns     = VOS_TRUE;
        pstAppPdpEntity->stIpv4Dhcp.ulIpv4PrimDNS       = VOS_NTOHL(ulIpAddr);
    }
    else
    {
        if (0 != pstDhcpConfig->ulPrimDNS)
        {
            pstAppPdpEntity->stIpv4Dhcp.bitOpIpv4PriDns = VOS_TRUE;
            pstAppPdpEntity->stIpv4Dhcp.ulIpv4PrimDNS   = pstDhcpConfig->ulPrimDNS;
        }
        else
        {
            pstAppPdpEntity->stIpv4Dhcp.bitOpIpv4PriDns = VOS_FALSE;
        }
    }

    /* ����û������˸�DNS����ʹ���û����õ�DNS�����緵�ص�DNS��ʹ�� */
    if (VOS_TRUE == pstAppDialPara->ulSndIPv4DNSValidFlag)
    {
        if (VOS_ERR == AT_LenStr2IpAddr(pstAppDialPara->aucSndIPv4DNSAddr,
                                        (VOS_UINT8*)&ulIpAddr))
        {
            ulIpAddr = 0;
        }

        pstAppPdpEntity->stIpv4Dhcp.bitOpIpv4SecDns     = VOS_TRUE;
        pstAppPdpEntity->stIpv4Dhcp.ulIpv4SecDNS        = VOS_NTOHL(ulIpAddr);
    }
    else
    {
        if (0 != pstDhcpConfig->ulSndDNS)
        {
            pstAppPdpEntity->stIpv4Dhcp.bitOpIpv4SecDns = VOS_TRUE;
            pstAppPdpEntity->stIpv4Dhcp.ulIpv4SecDNS    = pstDhcpConfig->ulSndDNS;
        }
        else
        {
            pstAppPdpEntity->stIpv4Dhcp.bitOpIpv4SecDns = VOS_FALSE;
        }
    }

    pstAppPdpEntity->stIpv4Dhcp.bitOpIpv4PriWINNS       = VOS_FALSE;
    pstAppPdpEntity->stIpv4Dhcp.bitOpIpv4SecWINNS       = VOS_FALSE;

    /* ��ȡPCSCF��ַ */
    pstAppPdpEntity->stIpv4Dhcp.stIpv4PcscfList.ucIpv4PcscfAddrNum = AT_MIN(TAF_PCSCF_ADDR_MAX_NUM,
                                                                            pstEvent->stIpv4PcscfList.ucIpv4PcscfAddrNum);

    for (ucIndex = 0; ucIndex < pstAppPdpEntity->stIpv4Dhcp.stIpv4PcscfList.ucIpv4PcscfAddrNum; ucIndex++)
    {
        pstAppPdpEntity->stIpv4Dhcp.stIpv4PcscfList.aulIpv4PcscfAddrList[ucIndex] = AT_GetLanAddr32(pstEvent->stIpv4PcscfList.astIpv4PcscfAddrList[ucIndex].aucPcscfAddr);
    }


    return;
}



VOS_UINT32 AT_AppCheckPdpIdleState(VOS_VOID)
{
    AT_PDP_ENTITY_STRU                 *pstAppPdpEntity;

    pstAppPdpEntity                     = AT_APP_GetPdpEntInfoAddr();

    /* ������е�PDP������IDLE, �����һ������DEACTIVATINGҲ����TRUE */
    if ( (AT_PDP_STATE_DEACTING == pstAppPdpEntity->enIpv4State)
      || (AT_PDP_STATE_DEACTING == pstAppPdpEntity->enIpv6State)
      || (AT_PDP_STATE_DEACTING == pstAppPdpEntity->enIpv4v6State)
      || ( (AT_PDP_STATE_IDLE == pstAppPdpEntity->enIpv4State)
        && (AT_PDP_STATE_IDLE == pstAppPdpEntity->enIpv6State)
        && (AT_PDP_STATE_IDLE == pstAppPdpEntity->enIpv4v6State) ) )
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}

VOS_UINT32 AT_AppCheckIpv4PdpState(
    AT_PDP_STATE_ENUM_U8                enPdpState
)
{
    AT_PDP_ENTITY_STRU                 *pstAppPdpEntity;

    pstAppPdpEntity                     = AT_APP_GetPdpEntInfoAddr();

    if ( (enPdpState == pstAppPdpEntity->enIpv4State)
      || (enPdpState == pstAppPdpEntity->enIpv4v6State) )
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 AT_AppCheckIpv6PdpState(
    AT_PDP_STATE_ENUM_U8                enPdpState
)
{
    AT_PDP_ENTITY_STRU                 *pstAppPdpEntity;

    pstAppPdpEntity                     = AT_APP_GetPdpEntInfoAddr();

    if ( (enPdpState == pstAppPdpEntity->enIpv6State)
      || (enPdpState == pstAppPdpEntity->enIpv4v6State) )
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


AT_PDP_STATE_ENUM_U8 AT_AppGetPdpStateByCid(
    VOS_UINT8                           ucCid
)
{
    AT_PDP_ENTITY_STRU                 *pstAppPdpEntity;

    pstAppPdpEntity                     = AT_APP_GetPdpEntInfoAddr();

    if (ucCid == pstAppPdpEntity->ucIpv4Cid)
    {
        return pstAppPdpEntity->enIpv4State;
    }
    else if (ucCid == pstAppPdpEntity->ucIpv6Cid)
    {
        return pstAppPdpEntity->enIpv6State;
    }
    else if (ucCid == pstAppPdpEntity->ucIpv4v6Cid)
    {
        return pstAppPdpEntity->enIpv4v6State;
    }
    else
    {
        return AT_PDP_STATE_IDLE;
    }
}


AT_PDP_STATE_ENUM_U8 AT_AppGetPdpState(
    VOS_UINT8                           ucPdpType
)
{
    AT_PDP_ENTITY_STRU                 *pstAppPdpEntity;
    AT_PDP_STATE_ENUM_U8                enPdpState;

    pstAppPdpEntity                     = AT_APP_GetPdpEntInfoAddr();

    switch (ucPdpType)
    {
        case TAF_PDP_IPV4:
            enPdpState = pstAppPdpEntity->enIpv4State;
            break;

        case TAF_PDP_IPV6:
            enPdpState = pstAppPdpEntity->enIpv6State;
            break;

        case TAF_PDP_IPV4V6:
            enPdpState = pstAppPdpEntity->enIpv4v6State;
            break;

        default:
            AT_ERR_LOG("AT_AppGetPdpState:ERROR: PDP type is invalid!");
            enPdpState = AT_PDP_STATE_IDLE;
            break;
    }

    return enPdpState;
}


VOS_VOID AT_AppSetPdpState(
    VOS_UINT8                           ucPdpType,
    AT_PDP_STATE_ENUM_U8                enPdpState
)
{
    AT_PDP_ENTITY_STRU                 *pstAppPdpEntity;

    pstAppPdpEntity                     = AT_APP_GetPdpEntInfoAddr();

    switch (ucPdpType)
    {
        case TAF_PDP_IPV4:
            pstAppPdpEntity->enIpv4State    = enPdpState;
            break;

        case TAF_PDP_IPV6:
            pstAppPdpEntity->enIpv6State    = enPdpState;
            break;

        case TAF_PDP_IPV4V6:
            pstAppPdpEntity->enIpv4v6State  = enPdpState;
            break;

        default:
            AT_ERR_LOG("AT_AppSetPdpState:ERROR: PDP type is invalid!");
            break;
    }

    return;
}


VOS_VOID AT_AppSndCallConnectedResult(
    VOS_UINT8                           ucCid,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType
)
{
    VOS_UINT16                          usLength = 0;

    switch (enPdpType)
    {
        case TAF_PDP_IPV4:
            usLength = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                              (VOS_CHAR *)pgucAtSndCodeAddr,
                                              (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                              "%s^DCONN:%d,\"IPV4\"%s",
                                              gaucAtCrLf,
                                              ucCid,
                                              gaucAtCrLf);
            break;

        case TAF_PDP_IPV6:
            usLength = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                              (VOS_CHAR *)pgucAtSndCodeAddr,
                                              (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                              "%s^DCONN:%d,\"IPV6\"%s",
                                              gaucAtCrLf,
                                              ucCid,
                                              gaucAtCrLf);
            break;

        default:
            AT_ERR_LOG("AT_AppSndConnectResult: PDP type is invalid in ^DCONN.");
            return;
    }

    At_SendResultData(AT_APP_GET_USR_PORT_INDEX(), pgucAtSndCodeAddr, usLength);

    return;
}


VOS_VOID AT_AppSndCallEndedResult(
    VOS_UINT8                           ucCid,
    TAF_PS_CAUSE_ENUM_UINT32            enCause,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType
)
{
    VOS_UINT16                          usLength = 0;

    switch (enPdpType)
    {
        case TAF_PDP_IPV4:

            usLength = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                              (VOS_CHAR *)pgucAtSndCodeAddr,
                                              (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                              "%s^DEND:%d,%d,\"IPV4\"%s",
                                              gaucAtCrLf,
                                              ucCid,
                                              enCause,
                                              gaucAtCrLf);

            break;

        case TAF_PDP_IPV6:

            usLength = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                              (VOS_CHAR *)pgucAtSndCodeAddr,
                                              (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                              "%s^DEND:%d,%d,\"IPV6\"%s",
                                              gaucAtCrLf,
                                              ucCid,
                                              enCause,
                                              gaucAtCrLf);

            break;

        default:
            AT_ERR_LOG("AT_AppSndCallEndedResult: PDP type is invalid in ^DEND.");
            return;
    }

    At_SendResultData(AT_APP_GET_USR_PORT_INDEX(), pgucAtSndCodeAddr, usLength);

    return;
}


VOS_UINT32 AT_SendRnicIpv4ActInd(VOS_UINT8 ucRmNetId)
{
    AT_RNIC_PDN_INFO_CFG_IND_STRU      *pstMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulRslt;
    MODEM_ID_ENUM_UINT16                enModemId;

    enModemId     = MODEM_ID_0;

    /* ͨ��client���modemid */
    ulRslt = AT_GetModemIdFromClient(g_stAtAppPdpEntity.stUsrInfo.enPortIndex, &enModemId);
    if (VOS_OK != ulRslt)
    {
        AT_ERR_LOG("AT_SendRnicIpv4ActInd: Get modem id fail.");
        return VOS_ERR;
    }

    /* ����AT_RNIC_PDN_INFO_CFG_IND_STRU��Ϣ */
    /*lint -save -e516 */
    pstMsg = (AT_RNIC_PDN_INFO_CFG_IND_STRU *)AT_ALLOC_MSG_WITH_HDR(
                            sizeof(AT_RNIC_PDN_INFO_CFG_IND_STRU));
    /*lint -restore */
    if (VOS_NULL_PTR == pstMsg)
    {
        AT_WARN_LOG("AT_NotifyRnicIpv4ActInd: alloc msg fail!");
        return VOS_ERR;
    }

    /* ��ʼ����Ϣ */
    TAF_MEM_SET_S(AT_GET_MSG_ENTITY(pstMsg), AT_GET_MSG_LENGTH(pstMsg),
                  0x00, AT_GET_MSG_LENGTH(pstMsg));

    /* ��д��Ϣͷ */
    AT_CFG_RNIC_MSG_HDR(pstMsg, ID_AT_RNIC_PDN_INFO_CFG_IND);

    /* ��д��Ϣ�� */
    pstMsg->bitOpIpv4PdnInfo = VOS_TRUE;
    pstMsg->bitOpIpv6PdnInfo = VOS_FALSE;
    pstMsg->ucRmNetId        = ucRmNetId;
    pstMsg->ucIfaceId        = PS_IFACE_ID_RMNET0;

    pstMsg->ucRabId          = g_stAtAppPdpEntity.stIpv4Dhcp.ucRabId;

    pstMsg->enModemId        = enModemId;

    pstMsg->ulIpv4Addr       = g_stAtAppPdpEntity.stIpv4Dhcp.ulIpv4Addr;

    if (VOS_OK != PS_SEND_MSG(WUEPS_PID_AT, pstMsg))
    {
        AT_WARN_LOG("AT_NotifyRnicIpv4ActInd: Send msg fail!");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_SendRnicIpv6ActInd(VOS_UINT8 ucRmNetId)
{
    AT_RNIC_PDN_INFO_CFG_IND_STRU      *pstMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulRslt;
    MODEM_ID_ENUM_UINT16                enModemId;

    enModemId     = MODEM_ID_0;

    /* ͨ��client���modemid */
    ulRslt = AT_GetModemIdFromClient(g_stAtAppPdpEntity.stUsrInfo.enPortIndex, &enModemId);

    if (VOS_OK != ulRslt)
    {
        AT_ERR_LOG("AT_SendRnicIpv6ActInd: Get modem id fail.");
        return VOS_ERR;
    }

    /* ����AT_RNIC_PDN_INFO_CFG_IND_STRU��Ϣ */
    /*lint -save -e516 */
    pstMsg = (AT_RNIC_PDN_INFO_CFG_IND_STRU *)AT_ALLOC_MSG_WITH_HDR(
                            sizeof(AT_RNIC_PDN_INFO_CFG_IND_STRU));
    /*lint -restore */
    if (VOS_NULL_PTR == pstMsg)
    {
        AT_WARN_LOG("AT_SendRnicIpv6ActInd: alloc msg fail!");
        return VOS_ERR;
    }

    /* ��ʼ����Ϣ */
    TAF_MEM_SET_S(AT_GET_MSG_ENTITY(pstMsg), AT_GET_MSG_LENGTH(pstMsg),
                  0x00, AT_GET_MSG_LENGTH(pstMsg));

    /* ��д��Ϣͷ */
    AT_CFG_RNIC_MSG_HDR(pstMsg, ID_AT_RNIC_PDN_INFO_CFG_IND);

    /* ��д��Ϣ�� */
    pstMsg->bitOpIpv4PdnInfo    = VOS_FALSE;
    pstMsg->bitOpIpv6PdnInfo    = VOS_TRUE;
    pstMsg->ucRmNetId           = ucRmNetId;
    pstMsg->ucIfaceId           = PS_IFACE_ID_RMNET0;

    pstMsg->ucRabId             = g_stAtAppPdpEntity.stIpv6Dhcp.ucRabId;

    pstMsg->enModemId           = enModemId;


    TAF_MEM_CPY_S(pstMsg->aucIpv6Addr,
               sizeof(pstMsg->aucIpv6Addr),
               g_stAtAppPdpEntity.stIpv6Dhcp.aucIpv6Addr,
               TAF_IPV6_ADDR_LEN);

    if (VOS_OK != PS_SEND_MSG(WUEPS_PID_AT, pstMsg))
    {
        AT_WARN_LOG("AT_NotifyRnicIpv6ActInd: Send msg fail!");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_SendRnicIpv4v6ActInd(VOS_UINT8 ucRmNetId)
{
    AT_RNIC_PDN_INFO_CFG_IND_STRU      *pstMsg;
    VOS_UINT32                          ulRslt;
    MODEM_ID_ENUM_UINT16                enModemId;

    enModemId     = MODEM_ID_0;

    /* ͨ��client���modemid */
    ulRslt = AT_GetModemIdFromClient(g_stAtAppPdpEntity.stUsrInfo.enPortIndex, &enModemId);

    if (VOS_OK != ulRslt)
    {
        AT_ERR_LOG("AT_SendRnicIpv4v6ActInd: Get modem id fail.");
        return VOS_ERR;
    }

    /* ����AT_RNIC_PDN_INFO_CFG_IND_STRU��Ϣ */
    /*lint -save -e516 */
    pstMsg = (AT_RNIC_PDN_INFO_CFG_IND_STRU *)AT_ALLOC_MSG_WITH_HDR(
                            sizeof(AT_RNIC_PDN_INFO_CFG_IND_STRU));
    /*lint -restore */
    if (VOS_NULL_PTR == pstMsg)
    {
        AT_WARN_LOG("AT_NotifyRnicIpv6ActInd: alloc msg fail!");
        return VOS_ERR;
    }

    /* ��ʼ����Ϣ */
    TAF_MEM_SET_S(AT_GET_MSG_ENTITY(pstMsg), AT_GET_MSG_LENGTH(pstMsg),
                  0x00, AT_GET_MSG_LENGTH(pstMsg));

    /* ��д��Ϣͷ */
    AT_CFG_RNIC_MSG_HDR(pstMsg, ID_AT_RNIC_PDN_INFO_CFG_IND);

    /* ��д��Ϣ�� */
    pstMsg->bitOpIpv6PdnInfo    = VOS_TRUE;
    pstMsg->bitOpIpv4PdnInfo    = VOS_TRUE;
    pstMsg->ucRmNetId           = ucRmNetId;
    pstMsg->ucIfaceId           = PS_IFACE_ID_RMNET0;

    pstMsg->ucRabId             = g_stAtAppPdpEntity.stIpv4Dhcp.ucRabId;

    pstMsg->ulIpv4Addr          = g_stAtAppPdpEntity.stIpv4Dhcp.ulIpv4Addr;
    pstMsg->enModemId           = enModemId;


    TAF_MEM_CPY_S(pstMsg->aucIpv6Addr,
               sizeof(pstMsg->aucIpv6Addr),
               g_stAtAppPdpEntity.stIpv6Dhcp.aucIpv6Addr,
               TAF_IPV6_ADDR_LEN);

    if (VOS_OK != PS_SEND_MSG(WUEPS_PID_AT, pstMsg))
    {
        AT_WARN_LOG("AT_NotifyRnicIpv6ActInd: Send msg fail!");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_VOID AT_AppSndRnicPdpActInd(
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType
)
{
    VOS_UINT32                          ulResult;

    /* ֪ͨRNIC PDP����ɹ� */
    if ( TAF_PDP_IPV4== enPdpType )
    {
        AT_SendRnicIpv4ActInd(RNIC_DEV_ID_RMNET0);
    }
    if ( TAF_PDP_IPV6== enPdpType )
    {
        AT_SendRnicIpv6ActInd(RNIC_DEV_ID_RMNET0);
    }

    if ( TAF_PDP_IPV4V6== enPdpType )
    {
        ulResult = AT_SendRnicIpv4v6ActInd(RNIC_DEV_ID_RMNET0);
        if (VOS_OK != ulResult)
        {
            AT_ERR_LOG("AT_AppSndRnicPdpActInd: ERROR: Send Rnic Ipv4 Act Ind To Rnic Failed.");
        }
    }
    return;
}


VOS_UINT32 AT_SendRnicPdpDeactInd(
    VOS_UINT8                           ucRmNetId,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType
)
{
    AT_RNIC_PDN_INFO_REL_IND_STRU      *pstMsg;

    /* ����AT_RNIC_PDN_INFO_REL_IND_STRU��Ϣ */
    /*lint -save -e516 */
    pstMsg = (AT_RNIC_PDN_INFO_REL_IND_STRU *)AT_ALLOC_MSG_WITH_HDR(
                            sizeof(AT_RNIC_PDN_INFO_REL_IND_STRU));
    /*lint -restore */
    if (VOS_NULL_PTR == pstMsg)
    {
        AT_WARN_LOG("AT_NotifyRnicPdpDeactInd: alloc msg fail!");
        return VOS_ERR;
    }

    /* ��ʼ����Ϣ */
    TAF_MEM_SET_S(AT_GET_MSG_ENTITY(pstMsg), AT_GET_MSG_LENGTH(pstMsg),
                  0x00, AT_GET_MSG_LENGTH(pstMsg));

    /* ��д��Ϣͷ */
    AT_CFG_RNIC_MSG_HDR(pstMsg, ID_AT_RNIC_PDN_INFO_REL_IND);

    /* ��д��Ϣ�� */

    pstMsg->bitOpIpv4PdnInfo = ((TAF_PDP_IPV4 == (enPdpType & TAF_PDP_IPV4)) ? VOS_TRUE : VOS_FALSE);
    pstMsg->bitOpIpv6PdnInfo = ((TAF_PDP_IPV6 == (enPdpType & TAF_PDP_IPV6)) ? VOS_TRUE : VOS_FALSE);
    pstMsg->ucRmNetId        = ucRmNetId;
    pstMsg->ucIfaceId        = PS_IFACE_ID_RMNET0;

    if (VOS_OK != PS_SEND_MSG(WUEPS_PID_AT, pstMsg))
    {
        AT_WARN_LOG("AT_NotifyRnicPdpDeactInd: Send msg fail!");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_VOID AT_AppPdpActOrig(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCid,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType
)
{
    TAF_PS_DIAL_PARA_STRU               stDialParaInfo;
    TAF_PDP_PRIM_CONTEXT_EXT_STRU       stPdpPrimContextExt;

    AT_GetPdpContextFromAtDialParam(&stPdpPrimContextExt,AT_APP_GetDailParaAddr());
    stPdpPrimContextExt.enPdpType   = enPdpType;
    stPdpPrimContextExt.ucCid       = ucCid;

    /* ����QOS���� */
    if (VOS_OK != AT_PS_SetQosPara(usClientId, AT_APP_GET_USR_CID(), ucCid))
    {
        return;
    }

    AT_GetPsDialParamFromAtDialParam(&stDialParaInfo,AT_APP_GetDailParaAddr());
    stDialParaInfo.enPdpType    = enPdpType;
    stDialParaInfo.ucCid        = ucCid;
    stDialParaInfo.ucRmnetId    = PS_IFACE_ID_RMNET0;

    /* ����PDP���� */
    if ( VOS_OK == TAF_PS_CallOrig(WUEPS_PID_AT,
                                   AT_PS_BuildExClientId(usClientId),
                                   0, &stDialParaInfo) )
    {
        if (TAF_PDP_IPV4 == enPdpType)
        {
            g_stAtAppPdpEntity.ucIpv4Cid = ucCid;
        }
        else
        {
            g_stAtAppPdpEntity.ucIpv6Cid = ucCid;
        }

        AT_APP_SetActPdpType(enPdpType);

        AT_AppSetPdpState(enPdpType, AT_PDP_STATE_ACTING);
    }

    return;
}


VOS_VOID  AT_ProcAppRegFCPoint(
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    VOS_UINT32                          ulResult;
    AT_FCID_MAP_STRU                    stFCPriOrg;
    FC_ID_ENUM_UINT8                    enDefaultFcId;

    /* APP����Ĭ��ʹ��NDIS����1�����ص� */
    enDefaultFcId = FC_ID_NIC_1;

    ulResult = AT_GetFcPriFromMap(enDefaultFcId ,&stFCPriOrg);
    if (VOS_OK == ulResult)
    {
        /* ���FC IDδע�ᣬ��ôע������ص㡣Ŀǰֻ֧��һ������.*/
        if (VOS_TRUE != stFCPriOrg.ulUsed)
        {
            /* ע��APP����ʹ�õ����ص�(Ĭ��ʹ������1) */
            AT_AppRegFCPoint(enDefaultFcId, pstEvent, RNIC_DEV_ID_RMNET0);
        }
        else
        {
            /* APP����ֻʹ����͵�����QOS���ȼ�FC_PRI_FOR_PDN_LOWEST */
            AT_NORM_LOG("AT_ProcAppRegFCPoint: No need to change the default QOS priority.");
        }
    }

    return;
}


VOS_VOID  AT_SaveIPv6Pcscf(
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent,
    AT_PDP_ENTITY_STRU                 *pstPdpEntity
)
{
    VOS_UINT8                           ucIndex;

    pstPdpEntity->stIpv6Dhcp.stIpv6PcscfList.ucIpv6PcscfAddrNum = AT_MIN(TAF_PCSCF_ADDR_MAX_NUM,
                                                                         pstEvent->stIpv6PcscfList.ucIpv6PcscfAddrNum);

    for (ucIndex = 0; ucIndex < pstPdpEntity->stIpv6Dhcp.stIpv6PcscfList.ucIpv6PcscfAddrNum; ucIndex++)
    {
        TAF_MEM_CPY_S(pstPdpEntity->stIpv6Dhcp.stIpv6PcscfList.astIpv6PcscfAddrList[ucIndex].aucPcscfAddr,
                      TAF_IPV6_ADDR_LEN,
                      pstEvent->stIpv6PcscfList.astIpv6PcscfAddrList[ucIndex].aucPcscfAddr,
                      TAF_IPV6_ADDR_LEN);
    }
}

VOS_VOID  AT_SaveIPv6Dns(
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent,
    AT_PDP_ENTITY_STRU                 *pstPdpEntity
)
{
    pstPdpEntity->stIpv6Dhcp.bitOpIpv6PriDns    = VOS_FALSE;
    pstPdpEntity->stIpv6Dhcp.bitOpIpv6SecDns    = VOS_FALSE;

    /* ��������DNS���յ�RA����ʱ��Ҫ����������ʱ��Ҫ֪ͨDRV */
    if (VOS_TRUE == pstEvent->stIpv6Dns.bitOpPrimDnsAddr)
    {
        TAF_MEM_CPY_S((VOS_VOID*)pstPdpEntity->stIpv6Dhcp.aucIpv6PrimDNS,
                sizeof(pstPdpEntity->stIpv6Dhcp.aucIpv6PrimDNS),
                pstEvent->stIpv6Dns.aucPrimDnsAddr,
                TAF_IPV6_ADDR_LEN);
        pstPdpEntity->stIpv6Dhcp.bitOpIpv6PriDns = VOS_TRUE;
    }

    if (VOS_TRUE == pstEvent->stIpv6Dns.bitOpSecDnsAddr)
    {
        TAF_MEM_CPY_S((VOS_VOID*)pstPdpEntity->stIpv6Dhcp.aucIpv6SecDNS,
                   sizeof(pstPdpEntity->stIpv6Dhcp.aucIpv6SecDNS),
                   pstEvent->stIpv6Dns.aucSecDnsAddr,
                   TAF_IPV6_ADDR_LEN);
        pstPdpEntity->stIpv6Dhcp.bitOpIpv6SecDns = VOS_TRUE;
    }

}


VOS_VOID AT_AppActCnfBackProc(
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    AT_COMM_PS_CTX_STRU                *pstCommPsCtx = VOS_NULL_PTR;
    VOS_UINT8                           ucCid;
    VOS_UINT32                          ulRet;

    pstCommPsCtx = AT_GetCommPsCtxAddr();

    /* ����ԭ��ֵ��ԭ��ֵΪ52����Ҫ������һ��PDP���� */
    if ( (VOS_FALSE == pstEvent->bitOpCause)
      || ( (VOS_TRUE == pstEvent->bitOpCause)
        && (TAF_PS_CAUSE_SM_NW_SINGLE_ADDR_BEARERS_ONLY_ALLOWED == pstEvent->enCause) ) )
    {
        if (TAF_PDP_IPV4 == pstEvent->stPdpAddr.enPdpType)
        {
            if ((VOS_FALSE == pstEvent->bitOpCause)
             && (VOS_FALSE == pstCommPsCtx->stRedialForNwCauseCfg.ucRedialForNoCausePolicy))
            {
                AT_AppSndCallEndedResult(pstEvent->ucCid,
                                         TAF_PS_CAUSE_SM_NW_PDP_TYPE_IPV4_ONLY_ALLOWED,
                                         TAF_PDP_IPV6);
            }
            else
            {
                if (AT_PDP_STATE_IDLE == AT_AppGetPdpState(TAF_PDP_IPV6))
                {
                    /* ����һ��δ�����CID���м��� */
                    ulRet = TAF_AGENT_FindCidForDial(pstEvent->stCtrl.usClientId, &ucCid);
                    if (VOS_OK == ulRet)
                    {
                        AT_AppPdpActOrig(pstEvent->stCtrl.usClientId, ucCid, TAF_PDP_IPV6);
                    }
                }
            }
        }
        else if (TAF_PDP_IPV6 == pstEvent->stPdpAddr.enPdpType)
        {
            if ((VOS_FALSE == pstEvent->bitOpCause)
             && (VOS_FALSE == pstCommPsCtx->stRedialForNwCauseCfg.ucRedialForNoCausePolicy))
            {
                AT_AppSndCallEndedResult(pstEvent->ucCid,
                                         TAF_PS_CAUSE_SM_NW_PDP_TYPE_IPV6_ONLY_ALLOWED,
                                         TAF_PDP_IPV4);
            }
            else
            {
                if (AT_PDP_STATE_IDLE == AT_AppGetPdpState(TAF_PDP_IPV4))
                {
                    /* ����һ��δ�����CID���м��� */
                    ulRet = TAF_AGENT_FindCidForDial(pstEvent->stCtrl.usClientId, &ucCid);
                    if (VOS_OK == ulRet)
                    {
                        AT_AppPdpActOrig(pstEvent->stCtrl.usClientId, ucCid, TAF_PDP_IPV4);
                    }
                }
            }
        }
        else
        {
            ;
        }
    }
    else
    {
        /* ����ԭ��ֵ, ֪ͨAPP���Ž�� */
        switch (pstEvent->enCause)
        {
            /* ԭ��ֵ#50 (IPv4 ONLY), ֪ͨAPPģ��IPv6����ʧ�� */
            case TAF_PS_CAUSE_SM_NW_PDP_TYPE_IPV4_ONLY_ALLOWED:
            case TAF_PS_CAUSE_PDP_TYPE_IPV4_ONLY_ALLOWED:
                AT_AppSndCallEndedResult(pstEvent->ucCid,
                                         pstEvent->enCause,
                                         TAF_PDP_IPV6);

                break;

            /* ԭ��ֵ#51 (IPv6 ONLY), ֪ͨAPPģ��IPv4����ʧ�� */
            case TAF_PS_CAUSE_SM_NW_PDP_TYPE_IPV6_ONLY_ALLOWED:
            case TAF_PS_CAUSE_PDP_TYPE_IPV6_ONLY_ALLOWED:
                AT_AppSndCallEndedResult(pstEvent->ucCid,
                                         pstEvent->enCause,
                                         TAF_PDP_IPV4);

                break;

            /* ����ԭ��ֵ, ������ */
            default:
                AT_NORM_LOG1("AT_AppActCnfBackProc: Other <Cause>", pstEvent->enCause);
                break;
        }
    }

    return;
}


VOS_VOID AT_AppIpv4PdpActCnfProc(
    AT_PDP_ENTITY_STRU                 *pstAppPdpEntity,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    VOS_UINT8                           ucCid;
    VOS_UINT32                          ulRet;

    /* ���DHCP��Ϣ */
    AT_AppDhcpReset();

    /* ��¼IPv4����PDP��Ӧ��CID */
    pstAppPdpEntity->ucIpv4Cid          = pstEvent->ucCid;

    /* ����IPv4����PDP״̬�л�������״̬ */
    AT_AppSetPdpState(TAF_PDP_IPV4, AT_PDP_STATE_ACTED);

    /* ����IPv4���͵�DHCP */
    AT_CtrlConnIndProc(pstEvent, AT_APP_USER);

    /* ֪ͨAPPģ��IPv4���� */
    AT_AppSndCallConnectedResult(pstEvent->ucCid, TAF_PDP_IPV4);

    /* ע�����ص� */
    AT_ProcAppRegFCPoint(pstEvent);

    /* �ϱ�RNIC������Ϣ */
    AT_AppSndRnicPdpActInd(TAF_PDP_IPV4);

    /* �����IPV4V6����Ҫ������һ��PDP */
    if (TAF_PDP_IPV4V6 == AT_APP_GetFirstActPdpType())
    {
        switch (AT_APP_GetActPdpType())
        {
            /* �������������PDP���ˣ���ǰ��PDP����ܾ��󣬷ֱ���IPV4��
               IPV6��PDP������У�TAF_PDP_IPV6���쳣������û�����IPV6��PDP
               ���������ظ�IPV4�ļ����ʱҲ��Ҫ���·���IPV6��PDP���� */
            case TAF_PDP_IPV4:
                if (AT_PDP_STATE_IDLE == AT_AppGetPdpState(TAF_PDP_IPV6))
                {
                    /* ����һ��δ�����CID���м��� */
                    ulRet = TAF_AGENT_FindCidForDial(pstEvent->stCtrl.usClientId, &ucCid);
                    if (VOS_OK == ulRet )
                    {
                        AT_AppPdpActOrig(pstEvent->stCtrl.usClientId, ucCid, TAF_PDP_IPV6);
                    }
                }
                break;

            /* ������PDP������˹���ʵ�� */
            case TAF_PDP_IPV4V6:
                /* ����PDP�л���IDLE̬ */
                AT_AppSetPdpState(TAF_PDP_IPV4V6, AT_PDP_STATE_IDLE);

                AT_AppActCnfBackProc(pstEvent);
                break;

            default:
                AT_WARN_LOG("AT_AppIpv4ActCnfProc:WARNING: PDP type is invalid!");
                break;
        }
    }

    return;
}


VOS_VOID AT_AppIpv6PdpActCnfProc(
    AT_PDP_ENTITY_STRU                 *pstAppPdpEntity,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    /* ���DHCPv6��Ϣ */
    AT_AppDhcpv6Reset();

    /* ��¼PDP���Ͷ�Ӧ��CID */
    pstAppPdpEntity->ucIpv6Cid  = pstEvent->ucCid;

    /* ����IPv6����״̬�л�������״̬ */
    AT_AppSetPdpState(TAF_PDP_IPV6, AT_PDP_STATE_ACTED);

    /* ����IPV6��ַ */
    AT_AppCtrlConnIpv6IndProc(pstAppPdpEntity, pstEvent);

    /* IPv6���͵ļ���ָʾ��Ҫ�ڻ�ȡRA��Ϣ���ϱ� */

    /* ע�����ص� */
    AT_ProcAppRegFCPoint(pstEvent);

    /* �ϱ�RNIC������Ϣ */
    AT_AppSndRnicPdpActInd(TAF_PDP_IPV6);

    /* �����IPV4V6����Ҫ������һ��PDP */
    if (TAF_PDP_IPV4V6 == AT_APP_GetFirstActPdpType())
    {
        switch (AT_APP_GetActPdpType())
        {
            case TAF_PDP_IPV6:
                /* �������������PDP���ˣ���ǰ��PDP����ܾ��󣬷ֱ���IPV4��
                   IPV6��PDP����, IPV6������ٷ���IPV4��PDP���� */
                break;

            /* ������PDP������˹���ʵ�� */
            case TAF_PDP_IPV4V6:
                /* ����PDP�л���IDLE̬ */
               AT_AppSetPdpState(AT_APP_GetActPdpType(), AT_PDP_STATE_IDLE);

               AT_AppActCnfBackProc(pstEvent);
               break;

            default:
               AT_WARN_LOG("AT_AppIpv6ActCnfProc: PDP type is invalid!");
               break;
        }
    }

    return;
}


VOS_VOID AT_AppIpv4v6PdpActCnfProc(
    AT_PDP_ENTITY_STRU                 *pstAppPdpEntity,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    /* ���DHCP��Ϣ */
    AT_AppDhcpReset();

    /* ���DHCPv6��Ϣ */
    AT_AppDhcpv6Reset();

    /* ��¼PDP���Ͷ�Ӧ��CID */
    pstAppPdpEntity->ucIpv4v6Cid   = pstEvent->ucCid;

    /* ����IPv6����״̬�л�������״̬ */
    AT_AppSetPdpState(TAF_PDP_IPV4V6, AT_PDP_STATE_ACTED);

    /* ����IPV4��ַ */
    AT_CtrlConnIndProc(pstEvent, AT_APP_USER);

    /* ����IPV6��ַ*/
    AT_AppCtrlConnIpv6IndProc(pstAppPdpEntity, pstEvent);

    /* ֪ͨAPPģ��IPv4����, IPv6���͵ļ���ָʾ��Ҫ�ڻ�ȡRA��Ϣ���ϱ� */
    AT_AppSndCallConnectedResult(pstEvent->ucCid, TAF_PDP_IPV4);

    /* ע�����ص� */
    AT_ProcAppRegFCPoint(pstEvent);

    /* �ϱ�RNIC������Ϣ */
    AT_AppSndRnicPdpActInd(TAF_PDP_IPV4V6);

    return;
}


VOS_VOID AT_AppIpv4PdpDeactivatedProc(
    AT_PDP_ENTITY_STRU                  *pstAppPdpEntity,
    TAF_PS_CALL_PDP_DEACTIVATE_CNF_STRU *pstEvent
)
{
    AT_PDP_STATE_ENUM_U8                enPdpState;

    /* ��IPv4���͵�PDP�л���IDLE̬ */
    AT_AppSetPdpState(TAF_PDP_IPV4, AT_PDP_STATE_IDLE);

    /* ���DHCP��Ϣ */
    AT_AppDhcpReset();

    /* ֪ͨAPPģ��IPv4�Ͽ� */
    AT_AppSndCallEndedResult(pstEvent->ucCid,
                             pstEvent->enCause,
                             TAF_PDP_IPV4);

    /* ֪ͨRNIC PDPȥ����ɹ� */
    AT_SendRnicPdpDeactInd(RNIC_DEV_ID_RMNET0, TAF_PDP_IPV4);

    /* ȥע��APP����ʹ�õ����ص�(Ĭ��ʹ������1) */
    AT_AppDeRegFCPoint(FC_ID_NIC_1, pstEvent);

   /* ���IPv4���͵�PDP�����ڼ���״̬, ��Ҫ����ȥ���� */
    enPdpState  = AT_AppGetPdpState(TAF_PDP_IPV6);
    if ( (AT_PDP_STATE_IDLE     != enPdpState)
      && (AT_PDP_STATE_DEACTING != enPdpState) )
    {
        if ( VOS_OK == TAF_PS_CallEnd(WUEPS_PID_AT,
                                      AT_PS_BuildExClientId(pstEvent->stCtrl.usClientId),
                                      0,
                                      pstAppPdpEntity->ucIpv6Cid) )
        {
            AT_AppSetPdpState(TAF_PDP_IPV6, AT_PDP_STATE_DEACTING);
        }
        else
        {
            AT_ERR_LOG("AT_AppIpv4DeactCnfProc:ERROR: Deactivate PDP failed!");
        }
    }
}


VOS_VOID AT_AppIpv6PdpDeactivatedProc(
    AT_PDP_ENTITY_STRU                  *pstAppPdpEntity,
    TAF_PS_CALL_PDP_DEACTIVATE_CNF_STRU *pstEvent
)
{
    AT_PDP_STATE_ENUM_U8                enPdpState;

    /* ��IPv6���͵�PDP�л���IDLE̬ */
    AT_AppSetPdpState(TAF_PDP_IPV6, AT_PDP_STATE_IDLE);

    /* ���DHCPv6��Ϣ */
    AT_AppDhcpv6Reset();

    /* ֪ͨAPPģ��IPv6�Ͽ� */
    AT_AppSndCallEndedResult(pstEvent->ucCid,
                             pstEvent->enCause,
                             TAF_PDP_IPV6);

    /* ֪ͨRNIC PDPȥ����ɹ� */
    AT_SendRnicPdpDeactInd(RNIC_DEV_ID_RMNET0, TAF_PDP_IPV6);

    /* ȥע��APP����ʹ�õ����ص�(Ĭ��ʹ������1) */
    AT_AppDeRegFCPoint(FC_ID_NIC_1, pstEvent);

    /* ���IPv4���͵�PDP�����ڼ���״̬, ��Ҫ����ȥ���� */
    enPdpState  = AT_AppGetPdpState(TAF_PDP_IPV4);
    if ( (AT_PDP_STATE_IDLE     != enPdpState)
      && (AT_PDP_STATE_DEACTING != enPdpState) )
    {
        if ( VOS_OK == TAF_PS_CallEnd(WUEPS_PID_AT,
                                      AT_PS_BuildExClientId(pstEvent->stCtrl.usClientId),
                                      0,
                                      pstAppPdpEntity->ucIpv4Cid) )
        {
            AT_AppSetPdpState(TAF_PDP_IPV4, AT_PDP_STATE_DEACTING);
        }
        else
        {
            AT_ERR_LOG("AT_AppIpv6DeactCnfProc:ERROR: Deactivate PDP failed!");
        }
    }
}


VOS_VOID AT_AppIpv4v6PdpDeactivatedProc(
    AT_PDP_ENTITY_STRU                  *pstAppPdpEntity,
    TAF_PS_CALL_PDP_DEACTIVATE_CNF_STRU *pstEvent
)
{
    /* ��IPv6���͵�PDP�л���IDLE̬ */
    AT_AppSetPdpState(TAF_PDP_IPV4V6, AT_PDP_STATE_IDLE);

    /* ���DHCP��Ϣ */
    AT_AppDhcpReset();

    /* ���DHCPv6��Ϣ */
    AT_AppDhcpv6Reset();

    /* ֪ͨAPPģ��IPv4�Ͽ� */
    AT_AppSndCallEndedResult(pstEvent->ucCid,
                             pstEvent->enCause,
                             TAF_PDP_IPV4);

    /* ֪ͨAPPģ��IPv6�Ͽ� */
    AT_AppSndCallEndedResult(pstEvent->ucCid,
                             pstEvent->enCause,
                             TAF_PDP_IPV6);

    /* ֪ͨRNIC PDPȥ����ɹ� */
    AT_SendRnicPdpDeactInd(RNIC_DEV_ID_RMNET0, TAF_PDP_IPV4V6);

    /* ȥע��APP����ʹ�õ����ص�(Ĭ��ʹ������1) */
    AT_AppDeRegFCPoint(FC_ID_NIC_1, pstEvent);

    return;
}


VOS_VOID AT_AppIpv4PdpActRejProc(
    AT_PDP_ENTITY_STRU                 *pstAppPdpEntity,
    TAF_PS_CALL_PDP_ACTIVATE_REJ_STRU  *pstEvent
)
{
    AT_PDP_STATE_ENUM_U8                enPreIpv4State;

    enPreIpv4State = pstAppPdpEntity->enIpv4State;

    /* �û�����IPv4v6���͵�PDP����, ���ұ�����ܾ�, ԭ��Ϊ28, Э��ջ��Ҫ
       �ֱ���IPv4/IPv6���͵�PDP����, Э��ջ���ȷ���IPv4, �ٷ���IPv6,
       ���IPv4���͵�PDP�����ٴα�����ܾ�, Э��ջ����Ҫ����IPV6���͵�
       PDP����Ϊ�˷�ֹPDP����Ƕ��, ���IPv6���͵�PDP����ʧ��, �����ٳ���
       IPv4���͵�PDP���� */

    /* ��IPv4���͵�PDP״̬�л���IDLE */
    AT_AppSetPdpState(TAF_PDP_IPV4, AT_PDP_STATE_IDLE);

    /* �ϱ�IPv4����ʧ�� */
    AT_AppSndCallEndedResult(pstEvent->ucCid,
                             pstEvent->enCause,
                             TAF_PDP_IPV4);
    if ((TAF_PDP_IPV4V6 == AT_APP_GetFirstActPdpType())
     && (AT_PDP_STATE_ACTING == enPreIpv4State) )
    {
        /* ���IPv6�����Ƿ��Ѿ�����, ���������, ����IPv6���͵�PDP���� */
        if (AT_PDP_STATE_IDLE == AT_AppGetPdpState(TAF_PDP_IPV6))
        {
            /* ����IPv6���͵�PDP�������� */
            AT_AppPdpActOrig(pstEvent->stCtrl.usClientId, pstEvent->ucCid, TAF_PDP_IPV6);

            /* ������Ų����Ĵ�����Ҫ�����һ��PDP�������̽�����, �˴�ֱ�ӷ��� */
            return;
        }

        /* ������Ų����Ĵ�����Ҫ�����һ��PDP�������̽�����, �˴�ֱ�ӷ��� */
        return;
    }

    /* ������Ų��� */
    TAF_MEM_SET_S(AT_APP_GetDailParaAddr(), sizeof(AT_DIAL_PARAM_STRU), 0x00, sizeof(AT_DIAL_PARAM_STRU));

    return;
}


VOS_VOID AT_AppIpv6PdpActRejProc(
    AT_PDP_ENTITY_STRU                 *pstAppPdpEntity,
    TAF_PS_CALL_PDP_ACTIVATE_REJ_STRU  *pstEvent
)
{
    /* ���IPv6����, �Ͳ���Ҫ�ٳ���IPv4, ��Ϊǰ���Ѿ������IPv4���͵�PDP
       ����, ����ٷ���IPv4���͵�PDP����Ļ�, ���ܻᵼ��PDP����Ƕ�� */

    /* ��IPv6���͵�PDP״̬�л���IDLE */
    AT_AppSetPdpState(TAF_PDP_IPV6, AT_PDP_STATE_IDLE);

    /* �ϱ�IPv6����ʧ�� */
    AT_AppSndCallEndedResult(pstEvent->ucCid,
                             pstEvent->enCause,
                             TAF_PDP_IPV6);

    /* ������Ų��� */
    TAF_MEM_SET_S(AT_APP_GetDailParaAddr(), sizeof(AT_DIAL_PARAM_STRU), 0x00, sizeof(AT_DIAL_PARAM_STRU));

    return;
}


VOS_VOID AT_AppIpv4v6PdpActRejProc(
    AT_PDP_ENTITY_STRU                 *pstAppPdpEntity,
    TAF_PS_CALL_PDP_ACTIVATE_REJ_STRU  *pstEvent
)
{
    AT_COMM_PS_CTX_STRU                *pstCommPsCtx = VOS_NULL_PTR;

    pstCommPsCtx    = AT_GetCommPsCtxAddr();

    /* ��IPv4v6���͵�PDP״̬�л���IDLE */
    AT_AppSetPdpState(TAF_PDP_IPV4V6, AT_PDP_STATE_IDLE);

    /* �����������Ҫ�ֱ���IPv4��IPv6��PDP���� */
    if ( (TAF_PS_CAUSE_SM_NW_UNKNOWN_PDP_ADDR_OR_TYPE == pstEvent->enCause)
      && (VOS_TRUE == pstCommPsCtx->stRedialForNwCauseCfg.ucRedialForNoPdpTypeCausePolicy))
    {
        /* ����IPv4���͵�PDP���� */
        AT_AppPdpActOrig(pstEvent->stCtrl.usClientId, pstEvent->ucCid, TAF_PDP_IPV4);
    }
    else
    {
        /* �ϱ�IPv4����ʧ�� */
        AT_AppSndCallEndedResult(pstEvent->ucCid,
                                 pstEvent->enCause,
                                 TAF_PDP_IPV4);

        /* �ϱ�IPv6����ʧ�� */
        AT_AppSndCallEndedResult(pstEvent->ucCid,
                                 pstEvent->enCause,
                                 TAF_PDP_IPV6);

        /* ������Ų��� */
        TAF_MEM_SET_S(AT_APP_GetDailParaAddr(), sizeof(AT_DIAL_PARAM_STRU), 0x00, sizeof(AT_DIAL_PARAM_STRU));
    }

    return;
}


VOS_VOID AT_AppPsRspEvtPdpActCnfProc(
    VOS_UINT8                           ucIndex,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    AT_PDP_ENTITY_STRU                 *pstAppPdpEntity;

    pstAppPdpEntity                     = AT_APP_GetPdpEntInfoAddr();

    /* ����PDP���ͷֱ���*/
    switch ( pstEvent->stPdpAddr.enPdpType )
    {
        case TAF_PDP_IPV4:
            /* ����IPv4��PDP����ɹ��¼� */
            AT_AppIpv4PdpActCnfProc(pstAppPdpEntity, pstEvent);
            break;

        case TAF_PDP_IPV6:
            /* ����IPv6��PDP����ɹ��¼� */
            AT_AppIpv6PdpActCnfProc(pstAppPdpEntity, pstEvent);
            break;

        case TAF_PDP_IPV4V6:
            /* ����IPv4v6��PDP����ɹ��¼� */
            AT_AppIpv4v6PdpActCnfProc(pstAppPdpEntity, pstEvent);
            break;

        default:
            AT_WARN_LOG("AT_AppPsRspPdpActEvtCnfProc:WARNING: PDP type is invaild!");
            break;
    }

    return;
}


VOS_VOID AT_AppPsRspEvtPdpActRejProc(
    VOS_UINT8                           ucIndex,
    TAF_PS_CALL_PDP_ACTIVATE_REJ_STRU  *pstEvent
)
{
    AT_PDP_ENTITY_STRU                 *pstAppPdpEntity;

    pstAppPdpEntity                     = AT_APP_GetPdpEntInfoAddr();

    switch (AT_APP_GetActPdpType())
    {
        case TAF_PDP_IPV4:
            AT_AppIpv4PdpActRejProc(pstAppPdpEntity, pstEvent);
            break;

        case TAF_PDP_IPV6:
            AT_AppIpv6PdpActRejProc(pstAppPdpEntity, pstEvent);
            break;

        case TAF_PDP_IPV4V6:
            AT_AppIpv4v6PdpActRejProc(pstAppPdpEntity, pstEvent);
            break;

        default:
            AT_WARN_LOG("AT_AppPsRspEvtPdpActRejProc:WARNING: PDP type is invaild!");
            break;
    }

    return;
}


VOS_VOID AT_AppPsRspEvtPdpDeactCnfProc(
    VOS_UINT8                           ucIndex,
    TAF_PS_CALL_PDP_DEACTIVATE_CNF_STRU *pstEvent
)
{
    AT_PDP_ENTITY_STRU                 *pstAppPdpEntity;
    AT_DIAL_PARAM_STRU                 *pstAppDialPara;

    pstAppPdpEntity                     = AT_APP_GetPdpEntInfoAddr();
    pstAppDialPara                      = AT_APP_GetDailParaAddr();

    switch (pstEvent->enPdpType)
    {
        case TAF_PDP_IPV4:
            AT_AppIpv4PdpDeactivatedProc(pstAppPdpEntity, pstEvent);
            break;

        case TAF_PDP_IPV6:
            AT_AppIpv6PdpDeactivatedProc(pstAppPdpEntity, pstEvent);
            break;

        case TAF_PDP_IPV4V6:
            AT_AppIpv4v6PdpDeactivatedProc(pstAppPdpEntity, pstEvent);
            break;

        default:
            AT_WARN_LOG("AT_AppPsRspPdpDeactEvtCnfProc:WARNING: Invalid PDP type.");
            break;
    }

    /* ������Ų��� */
    if ( (AT_PDP_STATE_IDLE == pstAppPdpEntity->enIpv4State)
      && (AT_PDP_STATE_IDLE == pstAppPdpEntity->enIpv6State)
      && (AT_PDP_STATE_IDLE == pstAppPdpEntity->enIpv4v6State) )
    {
        TAF_MEM_SET_S(pstAppDialPara, sizeof(AT_DIAL_PARAM_STRU), 0x00, sizeof(AT_DIAL_PARAM_STRU));
    }

    return;
}


VOS_VOID AT_AppPsRspEvtPdpDeactivatedProc(
    VOS_UINT8                           ucIndex,
    TAF_PS_CALL_PDP_DEACTIVATE_IND_STRU *pstEvent
)
{
    AT_PDP_ENTITY_STRU                 *pstAppPdpEntity;
    AT_DIAL_PARAM_STRU                 *pstAppDialPara;

    pstAppPdpEntity                     = AT_APP_GetPdpEntInfoAddr();
    pstAppDialPara                      = AT_APP_GetDailParaAddr();

    switch (pstEvent->enPdpType)
    {
        case TAF_PDP_IPV4:
            AT_AppIpv4PdpDeactivatedProc(pstAppPdpEntity, pstEvent);
            break;

        case TAF_PDP_IPV6:
            AT_AppIpv6PdpDeactivatedProc(pstAppPdpEntity, pstEvent);
            break;

        case TAF_PDP_IPV4V6:
            AT_AppIpv4v6PdpDeactivatedProc(pstAppPdpEntity, pstEvent);
            break;

        default:
            AT_WARN_LOG("AT_AppPsRspPdpDeactEvtCnfProc:WARNING: Invalid PDP type.");
            break;
    }

    /* ���APP���Ų��� */
    if ( (AT_PDP_STATE_IDLE == pstAppPdpEntity->enIpv4State)
      && (AT_PDP_STATE_IDLE == pstAppPdpEntity->enIpv6State)
      && (AT_PDP_STATE_IDLE == pstAppPdpEntity->enIpv4v6State) )
    {
        TAF_MEM_SET_S(pstAppDialPara, sizeof(AT_DIAL_PARAM_STRU), 0x00, sizeof(AT_DIAL_PARAM_STRU));
    }

    return;
}


VOS_UINT32 AT_AppSetFlowCtrl(
    VOS_UINT32                          ulParam1,
    VOS_UINT32                          ulParam2
)
{
    VOS_UINT32                          ulRslt;

    ulRslt = RNIC_StartFlowCtrl((VOS_UINT8)ulParam1);

    if (VOS_OK != ulRslt)
    {
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_AppClearFlowCtrl(
    VOS_UINT32                          ulParam1,
    VOS_UINT32                          ulParam2
)
{
    VOS_UINT32                          ulRslt;

    ulRslt = RNIC_StopFlowCtrl((VOS_UINT8)ulParam1);

    if (VOS_OK != ulRslt)
    {
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_AppRegFCPoint(
    FC_ID_ENUM_UINT8                    enFcId,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent,
    VOS_UINT8                           ucRmNetId
)
{
    FC_REG_POINT_STRU                   stRegFcPoint;
    FC_PRI_ENUM_UINT8                   enDefaultFcPri;
    VOS_UINT32                          ulRslt;
    MODEM_ID_ENUM_UINT16                enModemId;

    enModemId = MODEM_ID_0;

    TAF_MEM_SET_S(&stRegFcPoint, sizeof(stRegFcPoint), 0x00, sizeof(FC_REG_POINT_STRU));

    ulRslt = AT_GetModemIdFromClient((AT_CLIENT_TAB_INDEX_UINT8)pstEvent->stCtrl.usClientId, &enModemId);

    if (VOS_OK != ulRslt)
    {
        AT_ERR_LOG("AT_AppRegFCPoint: Get modem id fail.");
        return VOS_ERR;
    }

    /* ����ͨ����RABIDӳ���ϵ */
    FC_ChannelMapCreate(enFcId, pstEvent->ucRabId, enModemId);

    /* ����������������ȼ�RAB QoS���ȼ�������,���ȼ��ı�ʱ����Ҫ�ı����ȼ� */
    /*  FC_PRI_3        ��������ȼ��ĳ���
        FC_PRI_4        ��NONGBR����
        FC_PRI_5        ��GBR���� */
    enDefaultFcPri          = FC_PRI_FOR_PDN_LOWEST;
    stRegFcPoint.enFcId     = enFcId;
    stRegFcPoint.enFcPri    = enDefaultFcPri;

    stRegFcPoint.enModemId  = enModemId;
    stRegFcPoint.pClrFunc   = AT_AppClearFlowCtrl;
    stRegFcPoint.pSetFunc   = AT_AppSetFlowCtrl;

    /* Paramter1����ΪRmNetId, Paramter2����ΪFCID */
    stRegFcPoint.ulParam1   = ucRmNetId;
    stRegFcPoint.ulParam2   = enFcId;
    stRegFcPoint.pRstFunc   = AT_ResetFlowCtl;

    /* ע�����ص�, ��Ҫ�ֱ�ע��MEM��CDS */
    stRegFcPoint.enPolicyId = FC_POLICY_ID_MEM;
    ulRslt = FC_RegPoint(&stRegFcPoint);
    if (VOS_OK != ulRslt)
    {
        AT_ERR_LOG("AT_AppRegFCPoint: ERROR: reg mem point Failed.");
        return VOS_ERR;
    }


    stRegFcPoint.enPolicyId = FC_POLICY_ID_CDS;
    ulRslt = FC_RegPoint(&stRegFcPoint);
    if (VOS_OK != ulRslt)
    {
        AT_ERR_LOG("AT_AppRegFCPoint: ERROR: reg CDS point Failed.");
        return VOS_ERR;
    }

    /* ����FCID��FC Pri��ӳ���ϵ */
    g_stFcIdMaptoFcPri[enFcId].ulUsed       = VOS_TRUE;
    g_stFcIdMaptoFcPri[enFcId].enFcPri      = enDefaultFcPri;
    /* ��һ�������϶��RABID�������������Ҫ�����RABID��¼���� */
    g_stFcIdMaptoFcPri[enFcId].ulRabIdMask  |= ((VOS_UINT32)1 << (pstEvent->ucRabId));
    g_stFcIdMaptoFcPri[enFcId].enModemId    = enModemId;

    /* ��������Ϣ */
    AT_MNTN_TraceRegFcPoint((VOS_UINT8)pstEvent->stCtrl.usClientId, AT_FC_POINT_TYPE_RMNET);

    return VOS_OK;
}


VOS_UINT32 AT_AppDeRegFCPoint(
    FC_ID_ENUM_UINT8                     enFcId,
    TAF_PS_CALL_PDP_DEACTIVATE_CNF_STRU *pstEvent
)
{
    VOS_UINT32                          ulRslt;
    MODEM_ID_ENUM_UINT16                enModemId;

    enModemId = MODEM_ID_0;

    ulRslt = AT_GetModemIdFromClient((AT_CLIENT_TAB_INDEX_UINT8)pstEvent->stCtrl.usClientId, &enModemId);

    if (VOS_OK != ulRslt)
    {
        AT_ERR_LOG("AT_AppDeRegFCPoint: Get modem id fail.");
        return VOS_ERR;
    }

    /* �ڵ���FC_DeRegPointǰ,�ȵ���FC_ChannelMapDelete */
    FC_ChannelMapDelete(pstEvent->ucRabId, enModemId);

    ulRslt = FC_DeRegPoint(enFcId, enModemId);
    if (VOS_OK != ulRslt)
    {
        AT_ERR_LOG("AT_AppDeRegFCPoint: ERROR: de reg point Failed.");
        return VOS_ERR;
    }

    /* ���FCID��FC Pri��ӳ���ϵ */
    g_stFcIdMaptoFcPri[enFcId].ulUsed       = VOS_FALSE;
    g_stFcIdMaptoFcPri[enFcId].enFcPri      = FC_PRI_BUTT;
    /* ��һ�������϶��RABID�������������Ҫ����Ӧ��RABID��������� */
    g_stFcIdMaptoFcPri[enFcId].ulRabIdMask &= ~((VOS_UINT32)1 << pstEvent->ucRabId);
    g_stFcIdMaptoFcPri[enFcId].enModemId    = MODEM_ID_BUTT;

    /* ��������Ϣ */
    AT_MNTN_TraceDeregFcPoint((VOS_UINT8)pstEvent->stCtrl.usClientId, AT_FC_POINT_TYPE_RMNET);

    return VOS_OK;
}


VOS_VOID AT_APP_ParseUsrInfo(
    VOS_UINT8                           ucIndex,
    AT_PS_USER_INFO_STRU               *pstUsrInfo
)
{
    AT_CLIENT_TAB_INDEX_UINT8           enUserIndex;

    if (VOS_TRUE == AT_CheckAppUser(ucIndex))
    {
        enUserIndex = ucIndex;
    }
    else
    {
        enUserIndex = AT_CLIENT_TAB_APP_INDEX;
    }

    pstUsrInfo->enPortIndex = ucIndex;
    pstUsrInfo->enUserIndex = enUserIndex;
    pstUsrInfo->ucUsrType   = (AT_USER_TYPE)gastAtClientTab[enUserIndex].UserType;
    pstUsrInfo->ucUsrCid    = (VOS_UINT8)gastAtParaList[0].ulParaValue;

    return;
}


FC_PRI_ENUM_UINT8 AT_GetFCPriFromQos(
    TAF_UMTS_QOS_STRU                  *pstUmtsQos
)
{

    FC_PRI_ENUM_UINT8                   enFCPri;
    VOS_UINT8                           ucTrafficClass;

    /* ��ʼ�� */
    enFCPri         = FC_PRI_FOR_PDN_NONGBR;

    ucTrafficClass              = pstUmtsQos->ucTrafficClass;

    /* ����QOS trafficClass������ȡQCI */
    if (AT_QOS_TRAFFIC_CLASS_CONVERSATIONAL == ucTrafficClass)
    {
        enFCPri = FC_PRI_FOR_PDN_GBR;
    }
    else if (AT_QOS_TRAFFIC_CLASS_STREAMING == ucTrafficClass)
    {
        enFCPri = FC_PRI_FOR_PDN_GBR;
    }
    else if (AT_QOS_TRAFFIC_CLASS_INTERACTIVE == ucTrafficClass)
    {
        enFCPri = FC_PRI_FOR_PDN_NONGBR;
    }
    else
    {
        enFCPri = FC_PRI_FOR_PDN_LOWEST;
    }

    return enFCPri;
}


VOS_VOID  AT_ModemPsRspPdpDeactivatedEvtProc(
    VOS_UINT8                           ucIndex,
    TAF_PS_CALL_PDP_DEACTIVATE_CNF_STRU *pstEvent
)
{
    AT_UART_LINE_CTRL_STRU             *pstLineCtrl = VOS_NULL_PTR;
    VOS_UINT32                          ulATHCmdFlg;
    VOS_UINT32                          ulResult;
    VOS_UINT16                          usLength;

    pstLineCtrl = AT_GetUartLineCtrlInfo();
    usLength    = 0;
    ulResult    = AT_FAILURE;
    ulATHCmdFlg = (AT_CMD_H_PS_SET == AT_PS_GET_CURR_CMD_OPT(ucIndex)) ?
                  VOS_TRUE : VOS_FALSE;

    /* ȥע��Modem�˿ڵ����ص� */
    AT_DeRegModemPsDataFCPoint(ucIndex, pstEvent->ucRabId);

    if (AT_PPP_DATA_MODE == gastAtClientTab[ucIndex].DataMode)
    {
        /* �ͷ�PPPʵ�� & HDLCȥʹ�� */
        PPP_RcvAtCtrlOperEvent(gastAtClientTab[ucIndex].usPppId, PPP_AT_CTRL_REL_PPP_REQ);
        PPP_RcvAtCtrlOperEvent(gastAtClientTab[ucIndex].usPppId, PPP_AT_CTRL_HDLC_DISABLE);

        if (AT_DATA_MODE == gastAtClientTab[ucIndex].Mode)
        {
            /* �������ⵥAT2D13296���ڱ���ȥ�����龰�£���PPP������
               PPP_AT_CTRL_REL_PPP_REQ�󣬲�������������̬�����ǵ�
               ��PPP��ӦAT_PPP_PROTOCOL_REL_IND_MSG֮������������̬ */
            /* ��������ʱ�������ڵȴ�PPP��ӦAT_PPP_PROTOCOL_REL_IND_MSG */
            AT_STOP_TIMER_CMD_READY(ucIndex);

            if (AT_SUCCESS != At_StartTimer(AT_PPP_PROTOCOL_REL_TIME, ucIndex))
            {
                AT_ERR_LOG("At_UsbModemStatusPreProc:ERROR:Start Timer fail");
            }

            /* ���õ�ǰ�������� */
            gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_WAIT_PPP_PROTOCOL_REL_SET;

            return;
        }
    }
    else if (AT_IP_DATA_MODE == gastAtClientTab[ucIndex].DataMode)
    {
        /* �ͷ�PPPʵ�� & HDLCȥʹ�� */
        PPP_RcvAtCtrlOperEvent(gastAtClientTab[ucIndex].usPppId, PPP_AT_CTRL_REL_PPP_RAW_REQ);
        PPP_RcvAtCtrlOperEvent(gastAtClientTab[ucIndex].usPppId, PPP_AT_CTRL_HDLC_DISABLE);
    }
    else
    {
        /* ��������ģʽ�����ô��� */
        AT_WARN_LOG("TAF_PS_EVT_PDP_DEACTIVATED OTHER MODE");
    }

    /* ��������ģʽ */
    At_SetMode(ucIndex, AT_CMD_MODE,AT_NORMAL_MODE);

    if (VOS_TRUE == ulATHCmdFlg)
    {
        ulResult = AT_OK;
    }
    else
    {
        ulResult = AT_NO_CARRIER;
    }

    AT_STOP_TIMER_CMD_READY(ucIndex);
    gstAtSendData.usBufLen = usLength;
    At_FormatResultData(ucIndex,ulResult);

    /* ATH�Ͽ�PPP����ʱ, ���DCD�ź�ģʽΪCONNECT ON, ��Ҫ����DCD�ź� */
    if ( (AT_OK == ulResult)
      && (AT_UART_DCD_MODE_CONNECT_ON == pstLineCtrl->enDcdMode) )
    {
        AT_CtrlDCD(ucIndex, AT_IO_LEVEL_LOW);
    }

    return;
}


VOS_VOID  AT_ModemPsRspPdpActEvtRejProc(
    VOS_UINT8                           ucIndex,
    TAF_PS_CALL_PDP_ACTIVATE_REJ_STRU  *pstEvent
)
{
    VOS_UINT32                          ulResult;
    VOS_UINT16                          usLength;

    ulResult                            = AT_FAILURE;
    usLength                            = 0;

    if ( (AT_CMD_D_PPP_CALL_SET == gastAtClientTab[ucIndex].CmdCurrentOpt)
      || (AT_CMD_PPP_ORG_SET == gastAtClientTab[ucIndex].CmdCurrentOpt) )
    {
        ulResult = AT_NO_CARRIER;
        PPP_RcvAtCtrlOperEvent(gastAtClientTab[ucIndex].usPppId, PPP_AT_CTRL_REL_PPP_REQ);

        /* ��PPP����HDLCȥʹ�ܲ��� */
        PPP_RcvAtCtrlOperEvent(gastAtClientTab[ucIndex].usPppId, PPP_AT_CTRL_HDLC_DISABLE);

        /* ��������ģʽ */
        At_SetMode(ucIndex,AT_CMD_MODE,AT_NORMAL_MODE);

    }
    else if (AT_CMD_D_IP_CALL_SET == gastAtClientTab[ucIndex].CmdCurrentOpt)
    {
        ulResult = AT_ERROR;
        PPP_RcvAtCtrlOperEvent(gastAtClientTab[ucIndex].usPppId, PPP_AT_CTRL_REL_PPP_RAW_REQ);

        /* ��PPP����HDLCȥʹ�ܲ��� */
        PPP_RcvAtCtrlOperEvent(gastAtClientTab[ucIndex].usPppId, PPP_AT_CTRL_HDLC_DISABLE);


    }
    else
    {
        ulResult = AT_ERROR;
    }

    AT_STOP_TIMER_CMD_READY(ucIndex);
    gstAtSendData.usBufLen = usLength;
    At_FormatResultData(ucIndex,ulResult);

    return;
}


VOS_VOID AT_FillPppIndConfigInfoPara(
    AT_PPP_IND_CONFIG_INFO_STRU        *pstPppIndConfigInfo,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    /* ��дIP��ַ */
    TAF_MEM_CPY_S(pstPppIndConfigInfo->aucIpAddr,
               sizeof(pstPppIndConfigInfo->aucIpAddr),
               pstEvent->stPdpAddr.aucIpv4Addr,
               TAF_IPV4_ADDR_LEN);

    /* ��дDNS��ַ */
    if (VOS_TRUE == pstEvent->stDns.bitOpPrimDnsAddr)
    {
        pstPppIndConfigInfo->stPcoIpv4Item.bitOpPriDns = VOS_TRUE;

        TAF_MEM_CPY_S(pstPppIndConfigInfo->stPcoIpv4Item.aucPriDns,
               sizeof(pstPppIndConfigInfo->stPcoIpv4Item.aucPriDns),
               pstEvent->stDns.aucPrimDnsAddr,
               TAF_IPV4_ADDR_LEN);
    }

    if (VOS_TRUE == pstEvent->stDns.bitOpSecDnsAddr)
    {
        pstPppIndConfigInfo->stPcoIpv4Item.bitOpSecDns = VOS_TRUE;

        TAF_MEM_CPY_S(pstPppIndConfigInfo->stPcoIpv4Item.aucSecDns,
                       sizeof(pstPppIndConfigInfo->stPcoIpv4Item.aucSecDns),
                       pstEvent->stDns.aucSecDnsAddr,
                       TAF_IPV4_ADDR_LEN);

    }

    /* ��дNBNS��ַ */
    if (VOS_TRUE == pstEvent->stNbns.bitOpPrimNbnsAddr)
    {
        pstPppIndConfigInfo->stPcoIpv4Item.bitOpPriNbns = VOS_TRUE;

        TAF_MEM_CPY_S(pstPppIndConfigInfo->stPcoIpv4Item.aucPriNbns,
                   sizeof(pstPppIndConfigInfo->stPcoIpv4Item.aucPriNbns),
                   pstEvent->stNbns.aucPrimNbnsAddr,
                   TAF_IPV4_ADDR_LEN);
    }

    if (VOS_TRUE == pstEvent->stNbns.bitOpSecNbnsAddr)
    {
        pstPppIndConfigInfo->stPcoIpv4Item.bitOpSecNbns = VOS_TRUE;

        TAF_MEM_CPY_S(pstPppIndConfigInfo->stPcoIpv4Item.aucSecNbns,
                   sizeof(pstPppIndConfigInfo->stPcoIpv4Item.aucSecNbns),
                   pstEvent->stNbns.aucSecNbnsAddr,
                   TAF_IPV4_ADDR_LEN);
    }

    /* ��дGATE WAY��ַ */
    if (VOS_TRUE == pstEvent->stGateWay.bitOpGateWayAddr)
    {
        pstPppIndConfigInfo->stPcoIpv4Item.bitOpGateWay = VOS_TRUE;

        TAF_MEM_CPY_S(pstPppIndConfigInfo->stPcoIpv4Item.aucGateWay,
                   sizeof(pstPppIndConfigInfo->stPcoIpv4Item.aucGateWay),
                   pstEvent->stGateWay.aucGateWayAddr,
                   TAF_IPV4_ADDR_LEN);
    }


}


VOS_VOID AT_ModemPsRspPdpActEvtCnfProc(
    VOS_UINT8                           ucIndex,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    AT_PPP_IND_CONFIG_INFO_STRU         stPppIndConfigInfo;

    /* ע��Modem�˿ڵ����ص� */
    AT_RegModemPsDataFCPoint(ucIndex, pstEvent, FC_ID_MODEM);

    /* ��ʼ�� */
    TAF_MEM_SET_S(&stPppIndConfigInfo, sizeof(stPppIndConfigInfo),
                  0x00, sizeof(AT_PPP_IND_CONFIG_INFO_STRU));

    /* �������������� */
    AT_STOP_TIMER_CMD_READY(ucIndex);


    if (TAF_PDP_PPP == pstEvent->stPdpAddr.enPdpType)
    {
        At_SetMode(ucIndex, AT_DATA_MODE, AT_IP_DATA_MODE);

        /* ע���������ݷ��ͺ��� */
        Ppp_RegDlDataCallback(gastAtClientTab[ucIndex].usPppId);

        At_FormatResultData(ucIndex, AT_CONNECT);
    }
    else if (TAF_PDP_IPV4 == pstEvent->stPdpAddr.enPdpType)
    {
        /* ��дIP��ַ, DNS, NBNS */
        AT_FillPppIndConfigInfoPara(&stPppIndConfigInfo, pstEvent);

        /* ��AUTH��IPCP֡����PPP����: */
        Ppp_RcvConfigInfoInd(gastAtClientTab[ucIndex].usPppId, &stPppIndConfigInfo);
    }
    else
    {
        /* �������Ͳ������� */
    }

    return;
}


VOS_VOID  AT_ModemPsRspPdpDeactEvtCnfProc(
    VOS_UINT8                           ucIndex,
    TAF_PS_CALL_PDP_DEACTIVATE_CNF_STRU *pstEvent
)
{
    AT_UART_LINE_CTRL_STRU             *pstLineCtrl = VOS_NULL_PTR;
    VOS_UINT32                          ulModemUsrFlg;
    VOS_UINT32                          ulATHCmdFlg;
    VOS_UINT32                          ulResult;
    VOS_UINT16                          usLength;

    pstLineCtrl     = AT_GetUartLineCtrlInfo();
    ulModemUsrFlg   = AT_CheckModemUser(ucIndex);
    usLength        = 0;
    ulResult        = AT_FAILURE;
    ulATHCmdFlg     = (AT_CMD_H_PS_SET == gastAtClientTab[ucIndex].CmdCurrentOpt) ?
                      VOS_TRUE : VOS_FALSE;

    if ( (AT_MODEM_USER != gastAtClientTab[ucIndex].UserType)
      && (AT_HSUART_USER != gastAtClientTab[ucIndex].UserType) )
    {
        return;
    }

    if (AT_PPP_DATA_MODE == gastAtClientTab[ucIndex].DataMode)
    {
        /* �ͷ�PPPʵ�� & HDLCȥʹ�� */
        PPP_RcvAtCtrlOperEvent(gastAtClientTab[ucIndex].usPppId, PPP_AT_CTRL_REL_PPP_REQ);
        PPP_RcvAtCtrlOperEvent(gastAtClientTab[ucIndex].usPppId, PPP_AT_CTRL_HDLC_DISABLE);
    }
    else if (AT_IP_DATA_MODE == gastAtClientTab[ucIndex].DataMode)
    {
        /* �ͷ�PPPʵ�� & HDLCȥʹ�� */
        PPP_RcvAtCtrlOperEvent(gastAtClientTab[ucIndex].usPppId, PPP_AT_CTRL_REL_PPP_RAW_REQ);

        /* ��PPP����HDLCȥʹ�ܲ��� */
        PPP_RcvAtCtrlOperEvent(gastAtClientTab[ucIndex].usPppId, PPP_AT_CTRL_HDLC_DISABLE);
    }
    else
    {
        /* ��������ģʽ�����ô��� */
        AT_WARN_LOG("TAF_PS_EVT_PDP_DEACTIVE_CNF OTHER MODE");
    }

    /* ȥע��Modem�˿ڵ����ص� */
    AT_DeRegModemPsDataFCPoint(ucIndex, pstEvent->ucRabId);

    /* ��������ģʽ */
    At_SetMode(ucIndex, AT_CMD_MODE, AT_NORMAL_MODE);

    /* Ϊ�˹��Linux��̨���β���ʧ�����⣬�������¹�ܷ���:
       PDP�����Ͽ����̽����󣬽��յ�TAF_PS_EVT_PDP_DEACTIVE_CNF�¼����жϲ���
       ϵͳ�Ƿ�ΪLinux����������ԭ�����̴������ǣ����ٷ���"NO CARRIER" */
    if ((VOS_OK == DRV_GET_LINUXSYSTYPE()) && (VOS_TRUE == ulModemUsrFlg))
    {
        /* ԭ�������У���At_FormatResultData�����ڲ���������"NO CARRIER"֮��
           ��Ҫ��DCD���ͣ��ڴ˹�ܷ����У�������"NO CARRIER"����DCD�źŵ�����
           �����Ծ���Ҫ����
        */
        AT_STOP_TIMER_CMD_READY(ucIndex);
        AT_CtrlDCD(ucIndex, AT_IO_LEVEL_LOW);
        return;
    }

    if (VOS_TRUE == ulATHCmdFlg)
    {
        ulResult = AT_OK;
    }
    else
    {
        ulResult = AT_NO_CARRIER;
    }

    AT_STOP_TIMER_CMD_READY(ucIndex);
    gstAtSendData.usBufLen = usLength;
    At_FormatResultData(ucIndex,ulResult);

    /* ATH�Ͽ�PPP����ʱ, ���DCD�ź�ģʽΪCONNECT ON, ��Ҫ����DCD�ź� */
    if ( (AT_OK == ulResult)
      && (AT_UART_DCD_MODE_CONNECT_ON == pstLineCtrl->enDcdMode) )
    {
        AT_CtrlDCD(ucIndex, AT_IO_LEVEL_LOW);
    }

    return;
}


VOS_VOID AT_MODEM_ProcCallEndCnfEvent(
    VOS_UINT8                           ucIndex,
    TAF_PS_CALL_END_CNF_STRU           *pstEvent
)
{
    AT_UART_LINE_CTRL_STRU             *pstLineCtrl = VOS_NULL_PTR;
    AT_DCE_MSC_STRU                     stDecMsc;
    VOS_UINT32                          ulModemUsrFlg;
    VOS_UINT32                          ulATHCmdFlg;
    VOS_UINT32                          ulResult;
    VOS_UINT16                          usLength;

    pstLineCtrl     = AT_GetUartLineCtrlInfo();
    ulModemUsrFlg   = AT_CheckModemUser(ucIndex);
    ulResult        = AT_FAILURE;
    usLength        = 0;
    ulATHCmdFlg     = (AT_CMD_H_PS_SET == AT_PS_GET_CURR_CMD_OPT(ucIndex)) ?
                      VOS_TRUE : VOS_FALSE;

    TAF_MEM_SET_S(&stDecMsc, sizeof(stDecMsc), 0x00, sizeof(AT_DCE_MSC_STRU));

    /* ��鵱ǰ�û��Ĳ������� */
    if ( (AT_CMD_PS_DATA_CALL_END_SET != AT_PS_GET_CURR_CMD_OPT(ucIndex))
      && (AT_CMD_H_PS_SET != AT_PS_GET_CURR_CMD_OPT(ucIndex)) )
    {
        return;
    }

    /* PS��������ڴ���Ͽ�����, ֱ�ӷ��� */
    if (TAF_ERR_NO_ERROR == pstEvent->enCause)
    {
        return;
    }

    /* �ͷ�PPPʵ�� & HDLCȥʹ�� */
    if (AT_PPP_DATA_MODE == AT_PS_GET_CURR_DATA_MODE(ucIndex))
    {
        PPP_RcvAtCtrlOperEvent(AT_PS_GET_PPPID(ucIndex), PPP_AT_CTRL_REL_PPP_REQ);
        PPP_RcvAtCtrlOperEvent(AT_PS_GET_PPPID(ucIndex), PPP_AT_CTRL_HDLC_DISABLE);
    }
    else if (AT_IP_DATA_MODE == AT_PS_GET_CURR_DATA_MODE(ucIndex))
    {
        PPP_RcvAtCtrlOperEvent(AT_PS_GET_PPPID(ucIndex), PPP_AT_CTRL_REL_PPP_RAW_REQ);
        PPP_RcvAtCtrlOperEvent(AT_PS_GET_PPPID(ucIndex), PPP_AT_CTRL_HDLC_DISABLE);
    }
    else
    {
        ;
    }

    /* ��������ģʽ */
    At_SetMode(ucIndex, AT_CMD_MODE, AT_NORMAL_MODE);

    /* Ϊ�˹��Linux��̨���β���ʧ�����⣬�������¹�ܷ���:
       PDP�����Ͽ����̽����󣬽��յ�TAF_PS_EVT_PDP_DEACTIVE_CNF�¼����жϲ���
       ϵͳ�Ƿ�ΪLinux����������ԭ�����̴������ǣ����ٷ���"NO CARRIER" */
    if ((VOS_OK == DRV_GET_LINUXSYSTYPE()) && (VOS_TRUE == ulModemUsrFlg))
    {
        /* ԭ�������У���At_FormatResultData�����ڲ���������"NO CARRIER"֮��
           ��Ҫ��DCD���ͣ��ڴ˹�ܷ����У�������"NO CARRIER"����DCD�źŵ�����
           �����Ծ���Ҫ����
        */
        AT_STOP_TIMER_CMD_READY(ucIndex);
        AT_CtrlDCD(ucIndex, AT_IO_LEVEL_LOW);
        return;
    }

    if (VOS_TRUE == ulATHCmdFlg)
    {
        ulResult = AT_OK;
    }
    else
    {
        ulResult = AT_NO_CARRIER;
    }

    AT_STOP_TIMER_CMD_READY(ucIndex);
    gstAtSendData.usBufLen = usLength;
    At_FormatResultData(ucIndex, ulResult);

    /* ATH�Ͽ�PPP����ʱ, ���DCD�ź�ģʽΪCONNECT ON, ��Ҫ����DCD�ź� */
    if ( (AT_OK == ulResult)
      && (AT_UART_DCD_MODE_CONNECT_ON == pstLineCtrl->enDcdMode) )
    {
        AT_CtrlDCD(ucIndex, AT_IO_LEVEL_LOW);
    }

    return;
}


VOS_VOID  AT_AnswerPdpActInd(
    VOS_UINT8                           ucIndex,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    VOS_UINT16                          usPppId;
    VOS_UINT32                          ulRslt;

    usPppId     = 0;

    if (TAF_PDP_PPP == pstEvent->stPdpAddr.enPdpType)
    {
        if (VOS_OK != Ppp_CreateRawDataPppReq(&usPppId))
        {
            ulRslt = AT_ERROR;
        }
        else
        {
            /* ��¼PPP id��Index�Ķ�Ӧ��ϵ */
            gastAtPppIndexTab[usPppId]          = ucIndex;

            /* ����PPP id */
            gastAtClientTab[ucIndex].usPppId    = usPppId;


            /* ע��Modem�˿ڵ����ص� */
            AT_RegModemPsDataFCPoint(ucIndex, pstEvent, FC_ID_MODEM);

            /* �л�����ͨ�� */
            At_SetMode(ucIndex, AT_DATA_MODE, AT_IP_DATA_MODE);

            /* ע���������ݷ��ͺ��� */
            Ppp_RegDlDataCallback(usPppId);

            ulRslt = AT_CONNECT;
        }

        AT_STOP_TIMER_CMD_READY(ucIndex);
        At_FormatResultData(ucIndex, ulRslt);
    }
    else
    {
        /* ��������....*/
        AT_STOP_TIMER_CMD_READY(ucIndex);
        At_FormatResultData(ucIndex, AT_CONNECT);
    }

    return;
}


TAF_UINT32 At_RcvTeConfigInfoReq(
    TAF_UINT16                          usPppId,
    AT_PPP_REQ_CONFIG_INFO_STRU        *pstPppReqConfigInfo
)
{
    if (AT_MAX_CLIENT_NUM <= usPppId)
    {
        AT_WARN_LOG("At_RcvTeConfigInfoReq usPppId Wrong");
        return AT_FAILURE;
    }

    if (TAF_NULL_PTR == pstPppReqConfigInfo)
    {
        AT_WARN_LOG("At_RcvTeConfigInfoReq pPppReqConfigInfo NULL");
        return AT_FAILURE;
    }

    if (AT_CMD_D_PPP_CALL_SET != gastAtClientTab[gastAtPppIndexTab[usPppId]].CmdCurrentOpt)
    {
        AT_WARN_LOG("At_RcvTeConfigInfoReq NOT AT_CMD_D_PPP_CALL_SET");
        return AT_FAILURE;
    }

    if ( VOS_OK == TAF_PS_PppDialOrig(WUEPS_PID_AT,
                                      AT_PS_BuildExClientId(gastAtClientTab[gastAtPppIndexTab[usPppId]].usClientId),
                                      0,
                                      gastAtClientTab[gastAtPppIndexTab[usPppId]].ucCid,
                                      (TAF_PPP_REQ_CONFIG_INFO_STRU *)pstPppReqConfigInfo) )
    {
        /* ����ʱ�� */
        if (AT_SUCCESS != At_StartTimer(AT_ACT_PDP_TIME, gastAtPppIndexTab[usPppId]))
        {
            AT_ERR_LOG("At_RcvTeConfigInfoReq:ERROR:Start Timer");
            return AT_FAILURE;
        }

        /* ���õ�ǰ�������� */
        gastAtClientTab[gastAtPppIndexTab[usPppId]].CmdCurrentOpt = AT_CMD_PPP_ORG_SET;

        return AT_SUCCESS;
    }
    else
    {
        return AT_FAILURE;
    }
}


TAF_UINT32 At_RcvPppReleaseInd(
    TAF_UINT16                          usPppId
)
{
    TAF_UINT8                           aucEventInfo[4];
    AT_PPP_RELEASE_IND_MSG_STRU        *pMsg;
    VOS_UINT32                          ulLength;

    if (AT_MAX_CLIENT_NUM <= usPppId)
    {
        AT_WARN_LOG("At_RcvPppReleaseInd usPppId Wrong");
        return AT_FAILURE;
    }

    /* EVENT- At_RcvPppReleaseInd:usPppId / gastAtPppIndexTab[usPppId] */
    aucEventInfo[0] = (TAF_UINT8)(usPppId >> 8);
    aucEventInfo[1] = (TAF_UINT8)usPppId;
    aucEventInfo[2] = gastAtPppIndexTab[usPppId];
    AT_EventReport(WUEPS_PID_AT, NAS_OM_EVENT_DTE_RELEASE_PPP,
                   aucEventInfo, (VOS_UINT32)sizeof(aucEventInfo));

    /* ��ATģ�鷢��AT_PPP_RELEASE_IND_MSG */
    ulLength       = sizeof(AT_PPP_RELEASE_IND_MSG_STRU) - VOS_MSG_HEAD_LENGTH;
    /*lint -save -e516 */
    pMsg = (AT_PPP_RELEASE_IND_MSG_STRU *)PS_ALLOC_MSG(PS_PID_APP_PPP, ulLength);/*lint !e830*/
    /*lint -restore */
    if (VOS_NULL_PTR == pMsg)
    {
        /* ��ӡ������Ϣ---������Ϣ��ʧ�� */
        AT_ERR_LOG( "At_RcvPppReleaseInd:ERROR:Allocates a message packet for AT_PPP_RELEASE_IND_MSG_STRU msg FAIL!" );
        return AT_FAILURE;
    }
    /* ��д��Ϣͷ */
    pMsg->MsgHeader.ulSenderCpuId   = VOS_LOCAL_CPUID;
    pMsg->MsgHeader.ulSenderPid     = PS_PID_APP_PPP;
    pMsg->MsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;
    pMsg->MsgHeader.ulReceiverPid   = WUEPS_PID_AT;
    pMsg->MsgHeader.ulLength        = ulLength;

    pMsg->MsgHeader.ulMsgName       = AT_PPP_RELEASE_IND_MSG;
    /* ��д��Ϣ�� */
    pMsg->ucIndex                   = gastAtPppIndexTab[usPppId];

    /* ���͸���Ϣ */
    if (VOS_OK != PS_SEND_MSG(PS_PID_APP_PPP, pMsg))
    {
        /* ��ӡ������Ϣ---������Ϣʧ�� */
        AT_WARN_LOG( "At_RcvPppReleaseInd:WARNING:SEND AT_PPP_RELEASE_IND_MSG msg FAIL!" );
        return AT_FAILURE;
    }
    else
    {
        /* ��ӡ������Ϣ---��������Ϣ */
        AT_WARN_LOG( "At_RcvPppReleaseInd:NORMAL:SEND AT_PPP_RELEASE_IND_MSG Msg" );
        return AT_SUCCESS;
    }
}


TAF_VOID At_PppReleaseIndProc(
    TAF_UINT8                           ucIndex
)
{
    if (AT_MAX_CLIENT_NUM <= ucIndex)
    {
        AT_WARN_LOG("At_PppReleaseIndProc:ERROR:ucIndex is abnormal!");
        return;
    }

    if (AT_CMD_WAIT_PPP_PROTOCOL_REL_SET == gastAtClientTab[ucIndex].CmdCurrentOpt)
    {
        /* ��ATͨ���л�Ϊ����ģʽ */
        At_SetMode(ucIndex, AT_CMD_MODE, AT_NORMAL_MODE);

        /*ֹͣ��ʱ��*/
        AT_STOP_TIMER_CMD_READY(ucIndex);

        /*�ظ�NO CARRIER*/
        gstAtSendData.usBufLen = 0;
        At_FormatResultData(ucIndex, AT_NO_CARRIER);

        return;
    }

    if (AT_CMD_PS_DATA_CALL_END_SET == gastAtClientTab[ucIndex].CmdCurrentOpt)
    {
        return;
    }

    if ( VOS_OK == TAF_PS_CallEnd(WUEPS_PID_AT,
                                  AT_PS_BuildExClientId(gastAtClientTab[ucIndex].usClientId),
                                  0,
                                  gastAtClientTab[ucIndex].ucCid) )
    {
        /* ����ʱ�� */
        if (AT_SUCCESS != At_StartTimer(AT_DETACT_PDP_TIME, ucIndex))
        {
            AT_ERR_LOG("At_PppReleaseIndProc:ERROR:Start Timer");
            return;
        }

        /* ���õ�ǰ�������� */
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_PS_DATA_CALL_END_SET;
    }

    return;
}


TAF_UINT32 At_PsRab2PppId(
    TAF_UINT8                           ucExRabId,
    TAF_UINT16                         *pusPppId
)
{
    TAF_UINT16                          usPppId;
    TAF_UINT8                           ucIndex;

    if (VOS_NULL_PTR == pusPppId)
    {
        AT_WARN_LOG("At_PsRab2PppId, pusPppId NULL");
        return TAF_FAILURE;
    }

    /* ͨ��PPP ID������ң�Ч�ʻ�Ƚϸ� */
    for (usPppId = 1; usPppId <= PPP_MAX_ID_NUM; usPppId++)
    {
        ucIndex = gastAtPppIndexTab[usPppId];

        if (AT_MAX_CLIENT_NUM <= ucIndex)
        {
            continue;
        }

        if ( AT_CLIENT_USED != gastAtClientTab[ucIndex].ucUsed )
        {
            continue;
        }

        if (AT_DATA_MODE != gastAtClientTab[ucIndex].Mode)
        {
            continue;
        }

        if ( (AT_PPP_DATA_MODE != gastAtClientTab[ucIndex].DataMode)
          && (AT_IP_DATA_MODE != gastAtClientTab[ucIndex].DataMode) )
        {
            continue;
        }

        if (gastAtClientTab[ucIndex].ucExPsRabId == ucExRabId)
        {
            *pusPppId = usPppId;    /* ���ؽ�� */
            return TAF_SUCCESS;
        }

    }

    AT_LOG1("AT, At_PsRab2PppId, WARNING, Get PppId from Rab <1> Fail", ucExRabId);

    return TAF_FAILURE;
} /* At_PsRab2PppId */



TAF_UINT32 At_PppId2PsRab(
    TAF_UINT16                          usPppId,
    TAF_UINT8                          *pucExRabId
)
{
    TAF_UINT8                           ucIndex;


    if ((usPppId < 1) || (usPppId > PPP_MAX_ID_NUM))
    {
        TAF_LOG1(WUEPS_PID_AT, 0, PS_LOG_LEVEL_WARNING,
            "AT, At_PppId2PsRab, WARNING, PppId <1> Wrong", usPppId);
        return TAF_FAILURE;
    }

    if (TAF_NULL_PTR == pucExRabId)
    {
        AT_WARN_LOG("AT, At_PppId2PsRab, WARNING, pucRabId NULL");
        return TAF_FAILURE;
    }

    ucIndex = gastAtPppIndexTab[usPppId];

    if (AT_MAX_CLIENT_NUM <= ucIndex)
    {
        AT_LOG1("AT, At_PppId2PsRab, WARNING, index <1> Wrong", ucIndex);
        return TAF_FAILURE;
    }

    if (AT_DATA_MODE != gastAtClientTab[ucIndex].Mode)
    {
        AT_LOG1("AT, At_PppId2PsRab, WARNING, Mode <1> Wrong", gastAtClientTab[ucIndex].Mode);
        return TAF_FAILURE;
    }

    if ( (AT_PPP_DATA_MODE != gastAtClientTab[ucIndex].DataMode)
      && (AT_IP_DATA_MODE  != gastAtClientTab[ucIndex].DataMode) )
    {
        AT_LOG1("AT, At_PppId2PsRab, WARNING, DataMode <1> Wrong", gastAtClientTab[ucIndex].DataMode);
        return TAF_FAILURE;
    }

    *pucExRabId = gastAtClientTab[ucIndex].ucExPsRabId;

    return TAF_SUCCESS;
} /* At_PppId2PsRab */




VOS_UINT32 AT_RegModemPsDataFCPoint(
    VOS_UINT8                           ucIndex,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent,
    FC_ID_ENUM_UINT8                    enFcId
)
{
    FC_REG_POINT_STRU                   stRegFcPoint;
    VOS_UINT32                          ulRet;
    FC_PRI_ENUM_UINT8                   enFcPri;
    MODEM_ID_ENUM_UINT16                enModemId;
    AT_UART_CTX_STRU                   *pstUartCtx = VOS_NULL_PTR;

    pstUartCtx = AT_GetUartCtxAddr();

    /* UART�˿����عر�ʱ��ע�����ص� */
    if ( (VOS_TRUE == AT_CheckHsUartUser(ucIndex))
      && (AT_UART_FC_DTE_BY_DCE_NONE == pstUartCtx->stFlowCtrl.enDteByDce) )
    {
        return VOS_ERR;
    }

    enModemId = MODEM_ID_0;

    TAF_MEM_SET_S(&stRegFcPoint, sizeof(stRegFcPoint), 0x00, sizeof(FC_REG_POINT_STRU));

    ulRet = AT_GetModemIdFromClient((AT_CLIENT_TAB_INDEX_UINT8)pstEvent->stCtrl.usClientId, &enModemId);

    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_RegModemPsDataFCPoint: Get modem id fail.");
        return VOS_ERR;
    }

    /* ����ͨ����RABIDӳ���ϵ */
    FC_ChannelMapCreate(enFcId, pstEvent->ucRabId, enModemId);

    stRegFcPoint.enFcId             = enFcId;

    /* ����������������ȼ�RAB QoS���ȼ�������,���ȼ��ı�ʱ����Ҫ�ı����ȼ� */
    /*  FC_PRI_3        ��������ȼ��ĳ���
        FC_PRI_4        ��NONGBR����
        FC_PRI_5        ��GBR���� */
    if (TAF_USED == pstEvent->bitOpUmtsQos)
    {
        enFcPri = AT_GetFCPriFromQos(&pstEvent->stUmtsQos);
    }
    else
    {
        enFcPri = FC_PRI_FOR_PDN_NONGBR;
    }
    stRegFcPoint.enFcPri            = enFcPri;
    stRegFcPoint.enPolicyId         = FC_POLICY_ID_MEM;
    stRegFcPoint.pClrFunc           = AT_MODEM_StopFlowCtrl;
    stRegFcPoint.pSetFunc            = AT_MODEM_StartFlowCtrl;

    stRegFcPoint.ulParam1           = (VOS_UINT32)g_alAtUdiHandle[ucIndex];
    stRegFcPoint.enModemId          = enModemId;
    stRegFcPoint.ulParam2           = enFcId;
    stRegFcPoint.pRstFunc           = AT_ResetFlowCtl;

    /* ע�����ص�,��Ҫ�ֱ�ע��MEM,CPU,CDS��GPRS�� */
    ulRet = FC_RegPoint(&stRegFcPoint);
    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_RegModemFCPoint: ERROR: FC RegPoint MEM Failed.");
        return VOS_ERR;
    }

    stRegFcPoint.enPolicyId         = FC_POLICY_ID_CPU_A;
    ulRet = FC_RegPoint(&stRegFcPoint);
    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_RegModemFCPoint: ERROR: FC RegPoint A CPU Failed.");
        return VOS_ERR;
    }

    stRegFcPoint.enPolicyId         = FC_POLICY_ID_CDS;
    ulRet = FC_RegPoint(&stRegFcPoint);
    if (VOS_OK != ulRet)
    {
        return VOS_ERR;
    }

    stRegFcPoint.enPolicyId         = FC_POLICY_ID_GPRS;
    ulRet = FC_RegPoint(&stRegFcPoint);
    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_RegModemFCPoint: ERROR: FC RegPoint GPRS Failed.");
        return VOS_ERR;
    }

    stRegFcPoint.enPolicyId         = FC_POLICY_ID_CDMA;
    ulRet = FC_RegPoint(&stRegFcPoint);
    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_RegModemFCPoint: ERROR: reg CDMA point Failed.");
        return VOS_ERR;
    }

    /* ����FCID��FC Pri��ӳ���ϵ */
    g_stFcIdMaptoFcPri[FC_ID_MODEM].ulUsed      = VOS_TRUE;
    g_stFcIdMaptoFcPri[FC_ID_MODEM].enFcPri     = enFcPri;
    /* ��һ�������϶��RABID�������������Ҫ�����RABID��¼���� */
    g_stFcIdMaptoFcPri[FC_ID_MODEM].ulRabIdMask |= ((VOS_UINT32)1 << (pstEvent->ucRabId));
    g_stFcIdMaptoFcPri[FC_ID_MODEM].enModemId   = enModemId;

    /* ��������Ϣ */
    AT_MNTN_TraceRegFcPoint(ucIndex, AT_FC_POINT_TYPE_MODEM_PS);

    return VOS_OK;
}


VOS_UINT32 AT_DeRegModemPsDataFCPoint(
    VOS_UINT8                           ucIndex,
    VOS_UINT8                           ucRabId
)
{
    VOS_UINT32                          ulRet;
    MODEM_ID_ENUM_UINT16                enModemId;
    AT_UART_CTX_STRU                   *pstUartCtx = VOS_NULL_PTR;

    pstUartCtx = AT_GetUartCtxAddr();

    /* UART�˿����عر�ʱ��ע�����ص� */
    if ( (VOS_TRUE == AT_CheckHsUartUser(ucIndex))
        && (AT_UART_FC_DTE_BY_DCE_NONE == pstUartCtx->stFlowCtrl.enDteByDce) )
    {
        return VOS_ERR;
    }

    enModemId = MODEM_ID_0;

    ulRet = AT_GetModemIdFromClient(ucIndex, &enModemId);
    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_DeRegModemPsDataFCPoint: Get modem id fail.");
        return VOS_ERR;
    }
    /* ɾ������ģ��ӳ���ϵ */
    FC_ChannelMapDelete(ucRabId, enModemId);

    ulRet = FC_DeRegPoint(FC_ID_MODEM, enModemId);
    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_DeRegModemPsDataFCPoint: ERROR: FC DeRegPoint Failed.");
        return VOS_ERR;
    }

    /* ���FCID��FC Pri��ӳ���ϵ */
    g_stFcIdMaptoFcPri[FC_ID_MODEM].ulUsed      = VOS_FALSE;
    g_stFcIdMaptoFcPri[FC_ID_MODEM].enFcPri     = FC_PRI_BUTT;
    /* ��һ�������϶��RABID�������������Ҫ����Ӧ��RABID��������� */
    g_stFcIdMaptoFcPri[FC_ID_MODEM].ulRabIdMask &= ~((VOS_UINT32)1 << ucRabId);
    g_stFcIdMaptoFcPri[FC_ID_MODEM].enModemId   = MODEM_ID_BUTT;

    /* ��������Ϣ */
    AT_MNTN_TraceDeregFcPoint(ucIndex, AT_FC_POINT_TYPE_MODEM_PS);

    return VOS_OK;
}


VOS_UINT32 AT_ChangeFCPoint(
    TAF_CTRL_STRU                       *pstCtrl,
    FC_PRI_ENUM_UINT8                    enFCPri,
    FC_ID_ENUM_UINT8                     enFcId
)
{
    VOS_UINT32                          ulRet;
    MODEM_ID_ENUM_UINT16                enModemId;

    enModemId = MODEM_ID_0;

    ulRet = AT_GetModemIdFromClient((AT_CLIENT_TAB_INDEX_UINT8)pstCtrl->usClientId, &enModemId);

    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_ChangeFCPoint: Get modem id fail.");
        return VOS_ERR;
    }

    if (FC_ID_BUTT != enFcId)
    {
        ulRet = FC_ChangePoint(enFcId, FC_POLICY_ID_MEM, enFCPri, enModemId);
        if (VOS_OK != ulRet)
        {
            AT_ERR_LOG("AT_ChangeFCPoint: ERROR: Change FC Point Failed.");
        }

        ulRet = FC_ChangePoint(enFcId, FC_POLICY_ID_CPU_A , enFCPri, enModemId);
        if (VOS_OK != ulRet)
        {
            AT_ERR_LOG("AT_ChangeFCPoint: ERROR: Change FC Point Failed.");
        }

        ulRet = FC_ChangePoint(enFcId, FC_POLICY_ID_CDS, enFCPri, enModemId);
        if (VOS_OK != ulRet)
        {
            AT_ERR_LOG("AT_ChangeFCPoint: ERROR: Change FC Point Failed.");
        }

        ulRet = FC_ChangePoint(enFcId, FC_POLICY_ID_GPRS, enFCPri, enModemId);
        if (VOS_OK != ulRet)
        {
            AT_ERR_LOG("AT_ChangeFCPoint: ERROR: Change FC Point Failed.");
        }
    }

    return VOS_OK;
}


VOS_UINT32 AT_EnableHsicFlowCtl(
    VOS_UINT32                          ulUdiHdl,
    VOS_UINT32                          ulParam2
)
{
    VOS_UINT32  ulEnbflg = ACM_IOCTL_FLOW_CONTROL_ENABLE;

    if (0 != mdrv_udi_ioctl ((VOS_INT)ulUdiHdl, ACM_IOCTL_FLOW_CONTROL, (VOS_VOID*)(&ulEnbflg)))
    {
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_DisableHsicFlowCtl(
    VOS_UINT32                          ulUdiHdl,
    VOS_UINT32                          ulParam2
)
{
    VOS_UINT32  ulEnbflg = ACM_IOCTL_FLOW_CONTROL_DISABLE;

    if (0 != mdrv_udi_ioctl ((VOS_INT)ulUdiHdl, ACM_IOCTL_FLOW_CONTROL, (VOS_VOID*)(&ulEnbflg)))
    {
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_RegHsicFCPoint(
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    FC_REG_POINT_STRU                   stRegFcPoint;
    VOS_UINT32                          ulRet;
    FC_PRI_ENUM_UINT8                   enFCPri;
    UDI_DEVICE_ID_E                     enDataChannelId;
    FC_ID_ENUM_UINT8                    enFcId;
    MODEM_ID_ENUM_UINT16                enModemId;
    AT_MODEM_PS_CTX_STRU               *pstPsModemCtx = VOS_NULL_PTR;

    enModemId = MODEM_ID_0;

    TAF_MEM_SET_S(&stRegFcPoint, sizeof(stRegFcPoint), 0x00, sizeof(FC_REG_POINT_STRU));

    ulRet = AT_GetModemIdFromClient((AT_CLIENT_TAB_INDEX_UINT8)pstEvent->stCtrl.usClientId, &enModemId);
    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_RegHsicFCPoint: Get modem id fail.");
        return VOS_ERR;
    }

    pstPsModemCtx = AT_GetModemPsCtxAddrFromModemId(enModemId);

    /* Ѱ�����׵�ͨ��ID */
    if ((VOS_TRUE == pstPsModemCtx->astChannelCfg[pstEvent->ucCid].ulUsed)
      &&(AT_PS_INVALID_RMNET_ID != pstPsModemCtx->astChannelCfg[pstEvent->ucCid].ulRmNetId))
    {
        enDataChannelId = (UDI_DEVICE_ID_E)pstPsModemCtx->astChannelCfg[pstEvent->ucCid].ulRmNetId;
    }
    else
    {
        AT_ERR_LOG("AT_RegHsicFCPoint: ERROR: data channel id is abnormal.");
        return VOS_ERR;
    }

    switch ( enDataChannelId )
    {
        case UDI_ACM_HSIC_ACM1_ID:
        case UDI_NCM_HSIC_NCM0_ID:
            enFcId = FC_ID_DIPC_1;
            break;
        case UDI_ACM_HSIC_ACM3_ID:
        case UDI_NCM_HSIC_NCM1_ID:
            enFcId = FC_ID_DIPC_2;
            break;
        case UDI_ACM_HSIC_ACM5_ID:
        case UDI_NCM_HSIC_NCM2_ID:
            enFcId = FC_ID_DIPC_3;
            break;
        default:
            AT_WARN_LOG("AT_RegHsicFCPoint: WARNING: data channel id is abnormal.");
            return VOS_ERR;
    }

    /* ����ͨ����RABIDӳ���ϵ */
    FC_ChannelMapCreate(enFcId, pstEvent->ucRabId, enModemId);

    stRegFcPoint.enFcId             = enFcId;
    /* ����������������ȼ�RAB QoS���ȼ�������,���ȼ��ı�ʱ����Ҫ�ı����ȼ� */
    /*  FC_PRI_3        ��������ȼ��ĳ���
        FC_PRI_4        ��NONGBR����
        FC_PRI_5        ��GBR���� */
    if (TAF_USED == pstEvent->bitOpUmtsQos)
    {
        enFCPri = AT_GetFCPriFromQos(&pstEvent->stUmtsQos);
    }
    else
    {
        enFCPri = FC_PRI_FOR_PDN_NONGBR;
    }
    stRegFcPoint.enFcPri            = enFCPri;
    stRegFcPoint.enPolicyId         = FC_POLICY_ID_MEM;
    stRegFcPoint.pClrFunc           = AT_DisableHsicFlowCtl;
    stRegFcPoint.pSetFunc           = AT_EnableHsicFlowCtl;
    stRegFcPoint.ulParam1           = (VOS_UINT32)DIPC_GetDevHandleByRabId(pstEvent->ucRabId);
    stRegFcPoint.enModemId          = enModemId;
    stRegFcPoint.ulParam2           = enFcId;
    stRegFcPoint.pRstFunc           = AT_ResetFlowCtl;

    /* ע�����ص�,��Ҫ�ֱ�ע��MEM,CPU,CDS��GPRS�� */
    ulRet = FC_RegPoint(&stRegFcPoint);
    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_RegHsicFCPoint: ERROR: reg mem point Failed.");
        return VOS_ERR;
    }

    stRegFcPoint.enPolicyId         = FC_POLICY_ID_CPU_A;
    ulRet = FC_RegPoint(&stRegFcPoint);
    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_RegHsicFCPoint: ERROR: reg a cpu point Failed.");
        return VOS_ERR;
    }

    stRegFcPoint.enPolicyId         = FC_POLICY_ID_CDS;
    ulRet = FC_RegPoint(&stRegFcPoint);
    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_RegHsicFCPoint: ERROR: reg cds point Failed.");
        return VOS_ERR;
    }

    stRegFcPoint.enPolicyId         = FC_POLICY_ID_GPRS;
    ulRet = FC_RegPoint(&stRegFcPoint);
    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_RegHsicFCPoint: ERROR: reg gprs point Failed.");
        return VOS_ERR;
    }

    /* ����FCID��FC Pri��ӳ���ϵ */
    g_stFcIdMaptoFcPri[enFcId].ulUsed       = VOS_TRUE;
    g_stFcIdMaptoFcPri[enFcId].enFcPri      = enFCPri;
    /* ��һ�������϶��RABID�������������Ҫ�����RABID��¼���� */
    g_stFcIdMaptoFcPri[enFcId].ulRabIdMask  |= ((VOS_UINT32)1 << (pstEvent->ucRabId));
    g_stFcIdMaptoFcPri[enFcId].enModemId    = enModemId;

    /* ��������Ϣ */
    AT_MNTN_TraceRegFcPoint((VOS_UINT8)pstEvent->stCtrl.usClientId, AT_FC_POINT_TYPE_HSIC);

    return VOS_OK;
}


VOS_UINT32 AT_DeRegHsicFCPoint(
    TAF_PS_CALL_PDP_DEACTIVATE_CNF_STRU *pstEvent
)
{
    FC_ID_ENUM_UINT8                    enFcId;
    VOS_UINT32                          ulRet;
    UDI_DEVICE_ID_E                     enDataChannelId;
    MODEM_ID_ENUM_UINT16                enModemId;
    AT_MODEM_PS_CTX_STRU               *pstPsModemCtx = VOS_NULL_PTR;

    enModemId = MODEM_ID_0;

    ulRet = AT_GetModemIdFromClient((AT_CLIENT_TAB_INDEX_UINT8)pstEvent->stCtrl.usClientId, &enModemId);
    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_DeRegHsicFCPoint: Get modem id fail.");
        return VOS_ERR;
    }

    pstPsModemCtx = AT_GetModemPsCtxAddrFromModemId(enModemId);

    /* Ѱ�����׵�ͨ��ID */
    if ((VOS_TRUE == pstPsModemCtx->astChannelCfg[pstEvent->ucCid].ulUsed)
      &&(AT_PS_INVALID_RMNET_ID != pstPsModemCtx->astChannelCfg[pstEvent->ucCid].ulRmNetId))
    {
        enDataChannelId = (UDI_DEVICE_ID_E)pstPsModemCtx->astChannelCfg[pstEvent->ucCid].ulRmNetId;
    }
    else
    {
        AT_ERR_LOG("AT_DeRegHsicFCPoint: ERROR: data channel id is abnormal.");
        return VOS_ERR;
    }

    switch ( enDataChannelId )
    {
        case UDI_ACM_HSIC_ACM1_ID:
        case UDI_NCM_HSIC_NCM0_ID:
            enFcId = FC_ID_DIPC_1;
            break;
        case UDI_ACM_HSIC_ACM3_ID:
        case UDI_NCM_HSIC_NCM1_ID:
            enFcId = FC_ID_DIPC_2;
            break;
        case UDI_ACM_HSIC_ACM5_ID:
        case UDI_NCM_HSIC_NCM2_ID:
            enFcId = FC_ID_DIPC_3;
            break;
        default:
            AT_WARN_LOG("AT_DeRegHsicFCPoint: WARNING: data channel id is abnormal.");
            return VOS_ERR;
    }

    /* ɾ������ģ��ӳ���ϵ */
    FC_ChannelMapDelete(pstEvent->ucRabId, enModemId);

    ulRet = FC_DeRegPoint(enFcId, enModemId);
    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_DeRegHsicFCPoint: ERROR: de reg point Failed.");
        return VOS_ERR;
    }

    /* ���FCID��FC Pri��ӳ���ϵ */
    g_stFcIdMaptoFcPri[enFcId].ulUsed       = VOS_FALSE;
    g_stFcIdMaptoFcPri[enFcId].enFcPri      = FC_PRI_BUTT;
    /* ��һ�������϶��RABID�������������Ҫ����Ӧ��RABID��������� */
    g_stFcIdMaptoFcPri[enFcId].ulRabIdMask &= ~((VOS_UINT32)1 << pstEvent->ucRabId);
    g_stFcIdMaptoFcPri[enFcId].enModemId    = MODEM_ID_BUTT;

    /* ��������Ϣ */
    AT_MNTN_TraceDeregFcPoint((VOS_UINT8)pstEvent->stCtrl.usClientId, AT_FC_POINT_TYPE_HSIC);


    return VOS_OK;
}



VOS_VOID  AT_HsicPsRspEvtPdpActCnfProc(
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    /* ��DIPC�ϱ�PDP�Ѿ������¼� */
    AT_SndDipcPdpActInd(pstEvent->stCtrl.usClientId, pstEvent->ucCid, pstEvent->ucRabId);

    /* ��FCע�����ص� */
    AT_RegHsicFCPoint(pstEvent);

    /* ��ָ��CID��PDP�ļ���״̬����Ϊ����̬ */
    AT_SetAtChdataCidActStatus(pstEvent->stCtrl.usClientId, pstEvent->ucCid, VOS_TRUE);
    return;
}



VOS_VOID  AT_HsicPsRspEvtPdpDeactCnfProc(
    TAF_PS_CALL_PDP_DEACTIVATE_CNF_STRU *pstEvent
)
{
    /* ��DIPC�ϱ�PDP�Ѿ�ȥ�����¼� */
    AT_SndDipcPdpDeactInd(pstEvent->ucRabId);

    /* ��FCȥע�����ص� */
    AT_DeRegHsicFCPoint(pstEvent);

    /* ������CID������ͨ����ӳ���ϵ */
    AT_CleanAtChdataCfg(pstEvent->stCtrl.usClientId, pstEvent->ucCid, AT_PS_MAX_CALL_NUM, TAF_PS_APN_DATA_SYS_CELLULAR);

    return;
}



VOS_VOID  AT_HsicPsRspEvtPdpDeactivatedProc(
    VOS_UINT8                           ucIndex,
    TAF_PS_CALL_PDP_DEACTIVATE_IND_STRU *pstEvent
)
{
    VOS_UINT16  usLength;

    usLength    = 0;

    /* ��DIPC�ϱ�PDP�Ѿ�ȥ�����¼� */
    AT_SndDipcPdpDeactInd(pstEvent->ucRabId);

    /* ��FCȥע�����ص� */
    AT_DeRegHsicFCPoint(pstEvent);

    /* ������CID������ͨ����ӳ���ϵ */
    AT_CleanAtChdataCfg(pstEvent->stCtrl.usClientId, pstEvent->ucCid, AT_PS_MAX_CALL_NUM, TAF_PS_APN_DATA_SYS_CELLULAR);

    switch (pstEvent->enPdpType)
    {
        case TAF_PDP_IPV4:

            usLength  = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN, (VOS_CHAR*)pgucAtSndCodeAddr, (VOS_CHAR*)pgucAtSndCodeAddr + usLength,
                                        "%s^DEND:%d,%d,\"%s\"%s",
                                        gaucAtCrLf,
                                        pstEvent->ucCid,
                                        pstEvent->enCause,
                                        "IPV4",
                                        gaucAtCrLf);

            break;

        case TAF_PDP_IPV6:

            usLength  = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN, (VOS_CHAR*)pgucAtSndCodeAddr, (VOS_CHAR*)pgucAtSndCodeAddr + usLength,
                                        "%s^DEND:%d,%d,\"%s\"%s",
                                        gaucAtCrLf,
                                        pstEvent->ucCid,
                                        pstEvent->enCause,
                                        "IPV6",
                                        gaucAtCrLf);

            break;

        case TAF_PDP_IPV4V6:

            usLength  = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN, (VOS_CHAR*)pgucAtSndCodeAddr, (VOS_CHAR*)pgucAtSndCodeAddr + usLength,
                                        "%s^DEND:%d,%d,\"%s\"%s",
                                        gaucAtCrLf,
                                        pstEvent->ucCid,
                                        pstEvent->enCause,
                                        "IPV4",
                                        gaucAtCrLf);
            usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN, (VOS_CHAR*)pgucAtSndCodeAddr, (VOS_CHAR*)pgucAtSndCodeAddr + usLength,
                                        "%s^DEND:%d,%d,\"%s\"%s",
                                        gaucAtCrLf,
                                        pstEvent->ucCid,
                                        pstEvent->enCause,
                                        "IPV6",
                                        gaucAtCrLf);

            break;

        default:
            AT_ERR_LOG("AT_HsicPsRspEvtPdpDeactivatedProc:ERROR: PDP type is invalid!");
            return;
    }

    At_SendResultData(ucIndex, pgucAtSndCodeAddr, usLength);

    return;
}


VOS_VOID AT_PS_AddIpAddrRabIdMap(
    VOS_UINT16                          usClientId,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    AT_MODEM_PS_CTX_STRU               *pstPsModemCtx = VOS_NULL_PTR;
    VOS_UINT32                          ulIpAddr;

    pstPsModemCtx = AT_GetModemPsCtxAddrFromClientId(usClientId);
    ulIpAddr      = 0;

    if (!AT_PS_IS_RABID_VALID(pstEvent->ucRabId))
    {
        return;
    }

    if (pstEvent->bitOpPdpAddr)
    {
        ulIpAddr = AT_GetLanAddr32(pstEvent->stPdpAddr.aucIpv4Addr);
        pstPsModemCtx->aulIpAddrRabIdMap[pstEvent->ucRabId - AT_PS_RABID_OFFSET] = ulIpAddr;
    }

    return;
}


VOS_VOID AT_PS_DeleteIpAddrRabIdMap(
    VOS_UINT16                          usClientId,
    TAF_PS_CALL_PDP_DEACTIVATE_CNF_STRU *pstEvent
)
{
    AT_MODEM_PS_CTX_STRU               *pstPsModemCtx = VOS_NULL_PTR;

    pstPsModemCtx = AT_GetModemPsCtxAddrFromClientId(usClientId);

    if (!AT_PS_IS_RABID_VALID(pstEvent->ucRabId))
    {
        return;
    }

    pstPsModemCtx->aulIpAddrRabIdMap[pstEvent->ucRabId - AT_PS_RABID_OFFSET] = 0;

    return;
}


VOS_UINT32 AT_PS_GetIpAddrByRabId(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucRabId
)
{
    AT_MODEM_PS_CTX_STRU               *pstPsModemCtx = VOS_NULL_PTR;
    VOS_UINT32                          ulIpAddr;

    pstPsModemCtx = AT_GetModemPsCtxAddrFromClientId(usClientId);
    ulIpAddr      = 0;

    if (AT_PS_IS_RABID_VALID(ucRabId))
    {
        ulIpAddr = pstPsModemCtx->aulIpAddrRabIdMap[ucRabId - AT_PS_RABID_OFFSET];
    }

    return ulIpAddr;
}


VOS_UINT32 AT_ResetFlowCtl(
    VOS_UINT32                          ulParam1,
    VOS_UINT32                          ulParam2
)
{
    VOS_UINT8                           ucRabIdIndex;
    VOS_UINT32                          ulRabIdMask;
    VOS_UINT32                          ulRet;
    FC_ID_ENUM_UINT8                    enFcId;
    MODEM_ID_ENUM_UINT16                enModemId;

    enFcId      = (FC_ID_ENUM_UINT8)ulParam2;
    enModemId   = g_stFcIdMaptoFcPri[enFcId].enModemId;
    ulRabIdMask = g_stFcIdMaptoFcPri[enFcId].ulRabIdMask;

    if (VOS_TRUE == g_stFcIdMaptoFcPri[enFcId].ulUsed)
    {
        for (ucRabIdIndex = AT_PS_MIN_RABID; ucRabIdIndex <= AT_PS_MAX_RABID; ucRabIdIndex++)
        {
            if (1 == ((ulRabIdMask >> ucRabIdIndex) & 0x1))
            {
                FC_ChannelMapDelete(ucRabIdIndex, enModemId);
            }
        }

        ulRet = FC_DeRegPoint(enFcId, enModemId);
        if (VOS_OK != ulRet)
        {
            AT_ERR_LOG("AT_ResetFlowCtl: ERROR: DeReg point failed.");
            return VOS_ERR;
        }

        g_stFcIdMaptoFcPri[enFcId].ulUsed       = VOS_FALSE;
        g_stFcIdMaptoFcPri[enFcId].enFcPri      = FC_PRI_BUTT;
        g_stFcIdMaptoFcPri[enFcId].ulRabIdMask  = 0;
        g_stFcIdMaptoFcPri[enFcId].enModemId    = MODEM_ID_BUTT;

    }

    return VOS_OK;
}


VOS_UINT32 AT_PS_GetChDataValueFromRnicRmNetId(
    RNIC_DEV_ID_ENUM_UINT8              enRnicRmNetId,
    AT_CH_DATA_CHANNEL_ENUM_UINT32     *penDataChannelId
)
{
    VOS_UINT32                          i;
    AT_CHDATA_RNIC_RMNET_ID_STRU       *pstChdataRnicRmNetIdTab;

    pstChdataRnicRmNetIdTab = AT_PS_GET_CHDATA_RNIC_RMNET_ID_TBL_PTR();

    for (i = 0; i < AT_PS_GET_CHDATA_RNIC_RMNET_ID_TBL_SIZE() ; i++)
    {
        if (enRnicRmNetId == pstChdataRnicRmNetIdTab[i].enRnicRmNetId)
        {
            *penDataChannelId = pstChdataRnicRmNetIdTab[i].enChdataValue;
            break;
        }
    }

    if (i >= AT_PS_GET_CHDATA_RNIC_RMNET_ID_TBL_SIZE())
    {
        return VOS_ERR;
    }


    return VOS_OK;
}


AT_CHDATA_RNIC_RMNET_ID_STRU *AT_PS_GetChDataCfgByChannelId(
    VOS_UINT8                           ucIndex,
    AT_CH_DATA_CHANNEL_ENUM_UINT32      enDataChannelId
)
{
    AT_CHDATA_RNIC_RMNET_ID_STRU       *pstChdataRnicRmNetIdTab = VOS_NULL_PTR;
    AT_CHDATA_RNIC_RMNET_ID_STRU       *pstChDataConfig         = VOS_NULL_PTR;
    VOS_UINT32                          ulRslt;
    VOS_UINT32                          i;
    MODEM_ID_ENUM_UINT16                enModemId;

    pstChdataRnicRmNetIdTab = AT_PS_GET_CHDATA_RNIC_RMNET_ID_TBL_PTR();

    ulRslt = AT_GetModemIdFromClient(ucIndex, &enModemId);
    if (VOS_OK != ulRslt)
    {
        return VOS_NULL_PTR;
    }

    /**
     * MODEM0 RNIC ID Ϊ 0~2�� CHDATA��ֵΪ 1~3
     * MODEM1 RNIC ID Ϊ 3~4�� CHDATA��ֵΪ 4~5
     * MODEM2 RNIC ID Ϊ 6~7�� CHDATA��ֵΪ 6~7
     */

    if (MODEM_ID_0 == enModemId)
    {
        if ((enDataChannelId < AT_CH_DATA_CHANNEL_ID_1)
         || (enDataChannelId > AT_CH_DATA_CHANNEL_ID_3))
        {
            return VOS_NULL_PTR;
        }
    }
    else if (MODEM_ID_1 == enModemId)
    {
        if ((enDataChannelId < AT_CH_DATA_CHANNEL_ID_4)
         || (enDataChannelId > AT_CH_DATA_CHANNEL_ID_5))
        {
            return VOS_NULL_PTR;
        }
    }
    else if (MODEM_ID_2 == enModemId)
    {
        if ((enDataChannelId < AT_CH_DATA_CHANNEL_ID_6)
         || (enDataChannelId > AT_CH_DATA_CHANNEL_ID_7))
        {
            return VOS_NULL_PTR;
        }
    }
    else
    {
        return VOS_NULL_PTR;
    }

    /*  �����ж����ܱ�֤enDataChannelId����Ч�ԣ�����RM NET IDһ�����ڱ����ҵ� */
    for (i = 0; i < AT_PS_GET_CHDATA_RNIC_RMNET_ID_TBL_SIZE() ; i++)
    {
        if (enDataChannelId == pstChdataRnicRmNetIdTab[i].enChdataValue)
        {
            pstChDataConfig = &pstChdataRnicRmNetIdTab[i];
            break;
        }
    }

    return pstChDataConfig;
}


FC_ID_ENUM_UINT8 AT_PS_GetFcIdByUdiDeviceId(
    UDI_DEVICE_ID_E                     enDataChannelId
)
{
    switch ( enDataChannelId )
    {
        case UDI_ACM_HSIC_ACM1_ID:
        case UDI_NCM_HSIC_NCM0_ID:
            return FC_ID_DIPC_1;

        case UDI_ACM_HSIC_ACM3_ID:
        case UDI_NCM_HSIC_NCM1_ID:
            return FC_ID_DIPC_2;

        case UDI_ACM_HSIC_ACM5_ID:
        case UDI_NCM_HSIC_NCM2_ID:
            return FC_ID_DIPC_3;

        default:
            AT_WARN_LOG("AT_PS_GetFcIdByUdiDeviceId: WARNING: data channel id is abnormal.");
            return FC_ID_BUTT;
    }
}


FC_ID_ENUM_UINT8 AT_PS_GetFcIdFromRnicByRmNetId(
    VOS_UINT32                          ulRmNetId
)
{
    switch (ulRmNetId)
    {
        case RNIC_DEV_ID_RMNET0:
            return FC_ID_NIC_1;

        case RNIC_DEV_ID_RMNET1:
            return FC_ID_NIC_2;

        case RNIC_DEV_ID_RMNET2:
            return FC_ID_NIC_3;
        case RNIC_DEV_ID_RMNET3:
            return FC_ID_NIC_4;

        case RNIC_DEV_ID_RMNET4:
            return FC_ID_NIC_5;
        case RNIC_DEV_ID_RMNET5:
            return FC_ID_NIC_6;

        case RNIC_DEV_ID_RMNET6:
            return FC_ID_NIC_7;
        default:
            AT_WARN_LOG("AT_PS_GetFcIdFromRnidRmNetId: WARNING: data channel id is abnormal.");
            return FC_ID_BUTT;
    }
}


AT_PS_CALL_ENTITY_STRU* AT_PS_GetCallEntity(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId
)
{
    AT_MODEM_PS_CTX_STRU                  *pstPsModemCtx = VOS_NULL_PTR;

    pstPsModemCtx = AT_GetModemPsCtxAddrFromClientId(usClientId);

    return &(pstPsModemCtx->astCallEntity[ucCallId]);
}


AT_PS_USER_INFO_STRU* AT_PS_GetUserInfo(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId
)
{
    AT_MODEM_PS_CTX_STRU                  *pstPsModemCtx = VOS_NULL_PTR;

    pstPsModemCtx = AT_GetModemPsCtxAddrFromClientId(usClientId);

    return &(pstPsModemCtx->astCallEntity[ucCallId].stUserInfo);
}


AT_PS_DATA_CHANL_CFG_STRU* AT_PS_GetDataChanlCfg(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCid
)
{
    AT_MODEM_PS_CTX_STRU                  *pstPsModemCtx = VOS_NULL_PTR;

    pstPsModemCtx = AT_GetModemPsCtxAddrFromClientId(usClientId);

    return &(pstPsModemCtx->astChannelCfg[ucCid]);
}


VOS_UINT8 AT_PS_TransCidToCallId(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCid
)
{
    AT_MODEM_PS_CTX_STRU               *pstPsModemCtx = VOS_NULL_PTR;

    /* cid ��Чֵ */
    if (ucCid > TAF_MAX_CID)
    {
        return AT_PS_CALL_INVALID_CALLID;
    }

    pstPsModemCtx = AT_GetModemPsCtxAddrFromClientId(usClientId);

    return pstPsModemCtx->aucCidToIndexTbl[ucCid];
}


TAF_PS_CALL_END_CAUSE_ENUM_UINT8 AT_PS_TransCallEndCause(
    VOS_UINT8                           ucConnectType
)
{
    TAF_PS_CALL_END_CAUSE_ENUM_UINT8    enCause;

    enCause = TAF_PS_CALL_END_CAUSE_NORMAL;

    if (TAF_PS_CALL_TYPE_DOWN_FORCE == ucConnectType)
    {
        enCause = TAF_PS_CALL_END_CAUSE_FORCE;
    }

    return enCause;
}


VOS_UINT32 AT_PS_IsCallIdValid(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId
)
{
    AT_MODEM_PS_CTX_STRU               *pstPsModemCtx = VOS_NULL_PTR;

    if (ucCallId >= AT_PS_MAX_CALL_NUM)
    {
        return VOS_FALSE;
    }

    pstPsModemCtx = AT_GetModemPsCtxAddrFromClientId(usClientId);

    if ((AT_PS_CALL_INVALID_CID == pstPsModemCtx->astCallEntity[ucCallId].ucIpv4Cid)
     && (AT_PS_CALL_INVALID_CID == pstPsModemCtx->astCallEntity[ucCallId].ucIpv6Cid))
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_VOID AT_PS_AssignCallIdToCid(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCid,
    VOS_UINT8                           ucCallId
)
{
    AT_MODEM_PS_CTX_STRU               *pstPsModemCtx = VOS_NULL_PTR;

    pstPsModemCtx = AT_GetModemPsCtxAddrFromClientId(usClientId);

    pstPsModemCtx->aucCidToIndexTbl[ucCid] = ucCallId;

    return;
}


VOS_VOID AT_PS_FreeCallIdToCid(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCid,
    VOS_UINT8                           ucCallId,
    TAF_PS_APN_DATA_SYS_ENUM_UINT8      enDataSys
)
{
    AT_MODEM_PS_CTX_STRU               *pstPsModemCtx = VOS_NULL_PTR;

    pstPsModemCtx = AT_GetModemPsCtxAddrFromClientId(usClientId);

    if (VOS_FALSE == AT_PS_IsNeedClearCurrCall(usClientId, ucCallId, enDataSys))
    {
        AT_NORM_LOG("AT_PS_FreeCallIdToCid: not need FreeCallIdToCid!");
        return;
    }

    pstPsModemCtx->aucCidToIndexTbl[ucCid] = AT_PS_CALL_INVALID_CALLID;

    return;
}


VOS_UINT32 AT_PS_IsNeedSndCallEnded(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    TAF_PS_APN_DATA_SYS_ENUM_UINT8      enDataSys
)
{
    /* ��CALLû�з��𼤻Ĭ�Ͽ����ϱ� */
    if (TAF_PS_APN_DATA_SYS_NONE == enDataSys)
    {
        AT_NORM_LOG("AT_PS_IsNeedSndCallEnded: this call is not dailup!");
        return VOS_TRUE;
    }

    /* ��CALL��APNû������DATA SYSTEM������Ϣ��Ĭ�Ͽ����ϱ� */
    if (TAF_PS_APN_CURRENT_DOMAIN_BUTT == AT_PS_GetPsCallCurrentDataSys(usClientId, ucCallId))
    {
        AT_NORM_LOG("AT_PS_IsNeedSndCallEnded: this call apn is not data sys config!");
        return VOS_TRUE;
    }

    /* ��CALL��APN����DATA SYSTEM������Ϣ�����ǵ�ǰ��ķ��ؽ��������Ҫ�ϱ� */
    if (enDataSys == AT_PS_GetPsCallCurrentDataSys(usClientId, ucCallId))
    {
        AT_NORM_LOG("AT_PS_IsNeedSndCallEnded: this call apn is data sys config, and need report!");
        return VOS_TRUE;
    }

    /* ��CALL��APN����DATA SYSTEM������Ϣ�����ǲ��ǵ�ǰ��ķ��ؽ��������Ҫ�ϱ� */
    AT_NORM_LOG("AT_PS_IsNeedSndCallEnded: this call apn is data sys config, and not need report!");
    return VOS_FALSE;
}


VOS_UINT32 AT_PS_IsPdpTypeStateAllIdle(
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType,
    AT_PDP_STATE_ENUM_U8                enIpv4State,
    AT_PDP_STATE_ENUM_U8                enIpv6State
)
{
    if ( ( (TAF_PDP_IPV4 == enPdpType)
        || (TAF_PDP_IPV4V6 == enPdpType))
      && (AT_PDP_STATE_IDLE != enIpv4State))
    {
        AT_NORM_LOG("AT_PS_IsPdpTypeStateAllIdle: IPV4 state is not idle!");
        return VOS_FALSE;
    }

    if ( ( (TAF_PDP_IPV6 == enPdpType)
        || (TAF_PDP_IPV4V6 == enPdpType))
      && (AT_PDP_STATE_IDLE != enIpv6State))
    {
        AT_NORM_LOG("AT_PS_IsPdpTypeStateAllIdle: IPV6 state is not idle!");
        return VOS_FALSE;
    }

    AT_NORM_LOG("AT_PS_IsPdpTypeStateAllIdle: IP state is all idle!");
    return VOS_TRUE;
}


VOS_UINT32 AT_PS_IsNeedSetInvalidCid(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType,
    TAF_PS_APN_DATA_SYS_ENUM_UINT8      enDataSys
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;
    VOS_UINT32                          ulRst;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);
    ulRst         = VOS_TRUE;

    /* ��CALLû�з��𼤻Ĭ�Ͽ�������Ϊ��ЧCID */
    if (TAF_PS_APN_DATA_SYS_NONE == enDataSys)
    {
        AT_NORM_LOG("AT_PS_IsNeedSetInvalidCid: this call is not dailup!");
        return VOS_TRUE;
    }

    /* ��CALL��APNû�ж��ƣ�Ĭ�Ͽ�������Ϊ��ЧCID */
    if (VOS_FALSE == pstCallEntity->stApnDataSysInfo.ucDataSysInfoFlg)
    {
        AT_NORM_LOG("AT_PS_IsNeedSetInvalidCid: this call apn is not data sys config, and need set invalid cid!");
        return VOS_TRUE;
    }

    if (TAF_PS_APN_DATA_SYS_CELLULAR == enDataSys)
    {
        ulRst = AT_PS_IsPdpTypeStateAllIdle(enPdpType, pstCallEntity->enWlanIpv4State, pstCallEntity->enWlanIpv6State);
    }
    else if (TAF_PS_APN_DATA_SYS_WLAN == enDataSys)
    {
       ulRst = AT_PS_IsPdpTypeStateAllIdle(enPdpType, pstCallEntity->enIpv4State, pstCallEntity->enIpv6State);
    }
    else
    {
        /* nothing to do */
        AT_ERR_LOG("AT_PS_IsNeedSetInvalidCid: enDataSys is error!");
    }

    return ulRst;
}


VOS_UINT32 AT_PS_IsNeedClearCurrCall(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    TAF_PS_APN_DATA_SYS_ENUM_UINT8      enDataSys
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);

    /* ��CALLû�з��𼤻Ĭ�Ͽ������ */
    if (TAF_PS_APN_DATA_SYS_NONE == enDataSys)
    {
        AT_NORM_LOG("AT_PS_IsNeedClearCurrCall: this call is not dailup!");
        return VOS_TRUE;
    }

    /* ��CALL��APNû�ж��ƣ�Ĭ�Ͽ������ */
    if (VOS_FALSE == pstCallEntity->stApnDataSysInfo.ucDataSysInfoFlg)
    {
        AT_NORM_LOG("AT_PS_IsNeedClearCurrCall: this call apn is not data sys config, and need clear!");
        return VOS_TRUE;
    }

    /* ��CALL WLAN�����̻�û�н�������������� */
    if ( (TAF_PS_APN_DATA_SYS_CELLULAR == enDataSys)
      && ( (AT_PDP_STATE_IDLE != pstCallEntity->enWlanIpv4State)
        || (AT_PDP_STATE_IDLE != pstCallEntity->enWlanIpv6State)))
    {
        AT_NORM_LOG("AT_PS_IsNeedClearCurrCall: this call has wlan state, and not need clear!");
        return VOS_FALSE;
    }

    /* ��CALL CELLULAR�����̻�û�н�������������� */
    if ( (TAF_PS_APN_DATA_SYS_WLAN == enDataSys)
      && ( (AT_PDP_STATE_IDLE != pstCallEntity->enIpv4State)
        || (AT_PDP_STATE_IDLE != pstCallEntity->enIpv6State)))
    {
        AT_NORM_LOG("AT_PS_IsNeedClearCurrCall: this call has cellular state, and not need clear!");
        return VOS_FALSE;
    }

    AT_NORM_LOG("AT_PS_IsNeedClearCurrCall: this call apn is data sys config, and need clear!");
    return VOS_TRUE;
}


VOS_VOID AT_PS_SetCid2CurrCall(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType,
    VOS_UINT8                           ucCid,
    TAF_PS_APN_DATA_SYS_ENUM_UINT8      enDataSys
)
{
    /* ����ʵ������(CallId)�ɵ����߱�֤����Ч�� */

    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);

    if ( (AT_PS_CALL_INVALID_CID == ucCid)
      && (VOS_FALSE == AT_PS_IsNeedSetInvalidCid(usClientId, ucCallId, enPdpType, enDataSys)))
    {
        AT_NORM_LOG("AT_PS_SetCid2CurrCall: not need Set Invalid Cid!");
        return;
    }

    switch (enPdpType)
    {
        case TAF_PDP_IPV4:
            pstCallEntity->ucIpv4Cid = ucCid;
            break;

        case TAF_PDP_IPV6:
            pstCallEntity->ucIpv6Cid = ucCid;
            break;

        case TAF_PDP_IPV4V6:
            pstCallEntity->ucIpv4Cid = ucCid;
            pstCallEntity->ucIpv6Cid = ucCid;
            break;

        default:
            AT_ERR_LOG("AT_PS_SetCid2CurrCall: PDP type is invalid!");
            break;
    }

    return;
}


VOS_UINT8 AT_PS_GetCidByCallType(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType
)
{
    /* ����ʵ������(CallId)�ɵ����߱�֤����Ч�� */

    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;
    VOS_UINT8                           ucCid;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);

    switch (enPdpType)
    {
        case TAF_PDP_IPV4:
            ucCid = pstCallEntity->ucIpv4Cid;
            break;

        case TAF_PDP_IPV6:
            ucCid = pstCallEntity->ucIpv6Cid;
            break;

        default:
            ucCid = AT_PS_CALL_INVALID_CID;
            AT_ERR_LOG("AT_PS_SetCid2CurrCall: PDP type is invalid!");
            break;
    }

    return ucCid;
}


VOS_VOID AT_PS_SetCallStateByType(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType,
    AT_PDP_STATE_ENUM_U8                enPdpState
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);

    switch (enPdpType)
    {
        case TAF_PDP_IPV4:
            pstCallEntity->enIpv4State = enPdpState;
            break;

        case TAF_PDP_IPV6:
            pstCallEntity->enIpv6State = enPdpState;
            break;

        case TAF_PDP_IPV4V6:
            pstCallEntity->enIpv4State = enPdpState;
            pstCallEntity->enIpv6State = enPdpState;
            break;

        default:
            AT_ERR_LOG("AT_PS_SetCallStateByType: PDP type is invalid!");
            break;
    }

    return;
}


AT_PDP_STATE_ENUM_U8 AT_PS_GetCallStateByType(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;
    AT_PDP_STATE_ENUM_U8                enPdpState;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);

    switch (enPdpType)
    {
        case TAF_PDP_IPV4:
            enPdpState = pstCallEntity->enIpv4State;
            break;

        case TAF_PDP_IPV6:
            enPdpState = pstCallEntity->enIpv6State;
            break;

        default:
            enPdpState = AT_PDP_STATE_IDLE;
            AT_ERR_LOG("AT_PS_GetCallStateByType: PDP type is invalid!");
            break;
    }

    return enPdpState;
}


AT_PDP_STATE_ENUM_U8 AT_PS_GetCallStateByCid(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    VOS_UINT8                           ucCid
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);

    if (pstCallEntity->ucIpv4Cid == ucCid)
    {
        return pstCallEntity->enIpv4State;
    }

    if (pstCallEntity->ucIpv6Cid == ucCid)
    {
        return pstCallEntity->enIpv6State;
    }

    return AT_PDP_STATE_IDLE;
}


TAF_PDP_TYPE_ENUM_UINT8 AT_PS_GenPdpTypeByCid(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    VOS_UINT8                           ucCid
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);

    if ((AT_PDP_STATE_ACTED == pstCallEntity->enIpv4State)
     && (AT_PDP_STATE_ACTED == pstCallEntity->enIpv6State)
     && (ucCid == pstCallEntity->ucIpv4Cid)
     && (ucCid == pstCallEntity->ucIpv6Cid))
    {
        return TAF_PDP_IPV4V6;
    }

    if ((AT_PDP_STATE_ACTED == pstCallEntity->enIpv4State)
     && (ucCid == pstCallEntity->ucIpv4Cid))
    {
        return TAF_PDP_IPV4;
    }

    if ((AT_PDP_STATE_ACTED == pstCallEntity->enIpv6State)
     && (ucCid == pstCallEntity->ucIpv6Cid))
    {
        return TAF_PDP_IPV6;
    }

    return TAF_PDP_TYPE_BUTT;
}


VOS_UINT32  AT_PS_IsUsrDialTypeDualStack(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);

    if (VOS_FALSE == AT_PS_GetPsCallHandOverFlg(usClientId, ucCallId))
    {
        if (TAF_PDP_IPV4V6 == pstCallEntity->stUsrDialParam.enPdpType)
        {
            return VOS_TRUE;
        }
    }
    else
    {
        if (TAF_PDP_IPV4V6 == pstCallEntity->enHoPdpType)
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_VOID AT_PS_ReportDCONN(
    VOS_UINT8                           ucCid,
    VOS_UINT8                           ucPortIndex,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType
)
{
    VOS_UINT16                          usLength;

    usLength = 0;

    switch (enPdpType)
    {
        case TAF_PDP_IPV4:
            usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                               (VOS_CHAR *)pgucAtSndCodeAddr,
                                               (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                               "%s^DCONN:%d,\"IPV4\"%s",
                                               gaucAtCrLf,
                                               ucCid,
                                               gaucAtCrLf);
            break;

        case TAF_PDP_IPV6:
            usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                               (VOS_CHAR *)pgucAtSndCodeAddr,
                                               (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                               "%s^DCONN:%d,\"IPV6\"%s",
                                               gaucAtCrLf,
                                               ucCid,
                                               gaucAtCrLf);
            break;

        case TAF_PDP_IPV4V6:
            usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                               (VOS_CHAR *)pgucAtSndCodeAddr,
                                               (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                               "%s^DCONN:%d,\"IPV4\"%s",
                                               gaucAtCrLf,
                                               ucCid,
                                               gaucAtCrLf);
            usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                               (VOS_CHAR *)pgucAtSndCodeAddr,
                                               (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                               "%s^DCONN:%d,\"IPV6\"%s",
                                               gaucAtCrLf,
                                               ucCid,
                                               gaucAtCrLf);
            break;

        default:
            AT_ERR_LOG("AT_PS_ReportDCONN: PDP type is invalid in ^DCONN.");
            return;
    }

    At_SendResultData(ucPortIndex, pgucAtSndCodeAddr, usLength);

    return;
}


VOS_VOID AT_PS_ReportDEND(
    VOS_UINT8                           ucCid,
    VOS_UINT8                           ucPortIndex,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType,
    TAF_PS_CAUSE_ENUM_UINT32            enCause
)
{
    VOS_UINT16                          usLength;

    usLength = 0;

    switch (enPdpType)
    {
        case TAF_PDP_IPV4:
            usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                               (VOS_CHAR *)pgucAtSndCodeAddr,
                                               (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                               "%s^DEND:%d,%d,\"IPV4\"%s",
                                               gaucAtCrLf,
                                               ucCid,
                                               enCause,
                                               gaucAtCrLf);

            break;
        case TAF_PDP_IPV6:
            usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                               (VOS_CHAR *)pgucAtSndCodeAddr,
                                               (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                               "%s^DEND:%d,%d,\"IPV6\"%s",
                                               gaucAtCrLf,
                                               ucCid,
                                               enCause,
                                               gaucAtCrLf);
            break;

        case TAF_PDP_IPV4V6:
            usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                               (VOS_CHAR *)pgucAtSndCodeAddr,
                                               (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                               "%s^DEND:%d,%d,\"IPV4\"%s",
                                               gaucAtCrLf,
                                               ucCid,
                                               enCause,
                                               gaucAtCrLf);
            usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                               (VOS_CHAR *)pgucAtSndCodeAddr,
                                               (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                               "%s^DEND:%d,%d,\"IPV6\"%s",
                                               gaucAtCrLf,
                                               ucCid,
                                               enCause,
                                               gaucAtCrLf);
            break;

        default:
            AT_ERR_LOG("AT_PS_ReportDEND: PDP type is invalid in ^DEND.");
            return;
    }

    At_SendResultData(ucPortIndex, pgucAtSndCodeAddr, usLength);

    return;
}


AT_PS_RPT_CONN_RSLT_FUNC AT_PS_GetRptConnResultFunc(AT_USER_TYPE ucUsrType)
{
    AT_PS_REPORT_CONN_RESULT_STRU      *pstRptConnRsltFuncTblPtr = VOS_NULL_PTR;
    AT_PS_RPT_CONN_RSLT_FUNC            pRptConnRsltFunc         = VOS_NULL_PTR;
    VOS_UINT32                          ulCnt;

    pstRptConnRsltFuncTblPtr = AT_PS_GET_RPT_CONN_RSLT_FUNC_TBL_PTR();

    /* �û�����ƥ�� */
    for (ulCnt = 0; ulCnt < AT_PS_GET_RPT_CONN_RSLT_FUNC_TBL_SIZE(); ulCnt++)
    {
        if (ucUsrType == pstRptConnRsltFuncTblPtr[ulCnt].ucUsrType)
        {
            pRptConnRsltFunc = pstRptConnRsltFuncTblPtr[ulCnt].pRptConnRsltFunc;
            break;
        }
    }

    return pRptConnRsltFunc;
}


AT_PS_RPT_END_RSLT_FUNC AT_PS_GetRptEndResultFunc(AT_USER_TYPE ucUsrType)
{
    AT_PS_REPORT_END_RESULT_STRU       *pstRptEndRsltFuncTblPtr = VOS_NULL_PTR;
    AT_PS_RPT_END_RSLT_FUNC             pRptEndRsltFunc         = VOS_NULL_PTR;
    VOS_UINT32                          ulCnt;

    pstRptEndRsltFuncTblPtr = AT_PS_GET_RPT_END_RSLT_FUNC_TBL_PTR();

    /* �û�����ƥ�� */
    for (ulCnt = 0; ulCnt < AT_PS_GET_RPT_END_RSLT_FUNC_TBL_SIZE(); ulCnt++)
    {
        if (ucUsrType == pstRptEndRsltFuncTblPtr[ulCnt].ucUsrType)
        {
            pRptEndRsltFunc = pstRptEndRsltFuncTblPtr[ulCnt].pRptEndRsltFunc;
            break;
        }
    }

    return pRptEndRsltFunc;
}


VOS_VOID AT_PS_PdpAddrProc(
    VOS_UINT8                           ucCallId,
    AT_DHCP_CONFIG_STRU                *pstDhcpConfig,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;
    AT_DIAL_PARAM_STRU                 *pstDialPara = VOS_NULL_PTR;
    VOS_UINT32                          ulIpAddr;
    VOS_UINT8                           ucIndex;

    pstCallEntity = AT_PS_GetCallEntity(pstEvent->stCtrl.usClientId, ucCallId);
    pstDialPara   = &pstCallEntity->stUsrDialParam;
    ulIpAddr      = 0;

    pstCallEntity->stIpv4DhcpInfo.ucRabId                 = pstEvent->ucRabId;
    pstCallEntity->stIpv4DhcpInfo.ucPduSessionId          = pstEvent->ucPduSessionId;
    pstCallEntity->stIpv4DhcpInfo.ulIpv4Addr              = pstDhcpConfig->ulIPAddr;
    pstCallEntity->stIpv4DhcpInfo.ulIpv4GateWay           = pstDhcpConfig->ulGateWay;
    pstCallEntity->stIpv4DhcpInfo.ulIpv4NetMask           = pstDhcpConfig->ulSubNetMask;

    /* ����û���������DNS����ʹ���û����õ�DNS�����緵�ص�DNS��ʹ�� */
    if (VOS_TRUE == pstDialPara->ulPrimIPv4DNSValidFlag)
    {
        if (AT_FAILURE == AT_LenStr2IpAddr(pstDialPara->aucPrimIPv4DNSAddr,
                                           (VOS_UINT8*)&ulIpAddr))
        {
            ulIpAddr = 0;
        }

        pstCallEntity->stIpv4DhcpInfo.bitOpIpv4PriDns     = VOS_TRUE;
        pstCallEntity->stIpv4DhcpInfo.ulIpv4PrimDNS       = VOS_NTOHL(ulIpAddr);
    }
    else
    {
        if (0 != pstDhcpConfig->ulPrimDNS)
        {
            pstCallEntity->stIpv4DhcpInfo.bitOpIpv4PriDns = VOS_TRUE;
            pstCallEntity->stIpv4DhcpInfo.ulIpv4PrimDNS   = pstDhcpConfig->ulPrimDNS;
        }
        else
        {
            pstCallEntity->stIpv4DhcpInfo.bitOpIpv4PriDns = VOS_FALSE;
        }
    }

    /* ����û������˸�DNS����ʹ���û����õ�DNS�����緵�ص�DNS��ʹ�� */
    if (VOS_TRUE == pstDialPara->ulSndIPv4DNSValidFlag)
    {
        if (AT_FAILURE == AT_LenStr2IpAddr(pstDialPara->aucSndIPv4DNSAddr,
                                           (VOS_UINT8*)&ulIpAddr))
        {
            ulIpAddr = 0;
        }

        pstCallEntity->stIpv4DhcpInfo.bitOpIpv4SecDns     = VOS_TRUE;
        pstCallEntity->stIpv4DhcpInfo.ulIpv4SecDNS        = VOS_NTOHL(ulIpAddr);
    }
    else
    {
        if (0 != pstDhcpConfig->ulSndDNS)
        {
            pstCallEntity->stIpv4DhcpInfo.bitOpIpv4SecDns = VOS_TRUE;
            pstCallEntity->stIpv4DhcpInfo.ulIpv4SecDNS    = pstDhcpConfig->ulSndDNS;
        }
        else
        {
            pstCallEntity->stIpv4DhcpInfo.bitOpIpv4SecDns = VOS_FALSE;
        }
    }

    pstCallEntity->stIpv4DhcpInfo.bitOpIpv4PriWINNS       = VOS_FALSE;
    pstCallEntity->stIpv4DhcpInfo.bitOpIpv4SecWINNS       = VOS_FALSE;

    /* ��ȡ��PCSCF��ַ */
    pstCallEntity->stIpv4DhcpInfo.stIpv4PcscfList.ucIpv4PcscfAddrNum = AT_MIN(TAF_PCSCF_ADDR_MAX_NUM,
                                                                              pstEvent->stIpv4PcscfList.ucIpv4PcscfAddrNum);
    for (ucIndex = 0; ucIndex < pstEvent->stIpv4PcscfList.ucIpv4PcscfAddrNum; ucIndex++)
    {
        pstCallEntity->stIpv4DhcpInfo.stIpv4PcscfList.aulIpv4PcscfAddrList[ucIndex] = AT_GetLanAddr32(pstEvent->stIpv4PcscfList.astIpv4PcscfAddrList[ucIndex].aucPcscfAddr);
    }

    return;
}


VOS_VOID AT_PS_ProcConnInd(
    VOS_UINT8                           ucCallId,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    AT_DHCP_SETUP_PARAM_ST              stParam;
    AT_DHCP_CONFIG_STRU                 stConfig;

    TAF_MEM_SET_S(&stParam, sizeof(stParam), 0x00, sizeof(AT_DHCP_SETUP_PARAM_ST));
    TAF_MEM_SET_S(&stConfig, sizeof(stConfig), 0x00, sizeof(AT_DHCP_CONFIG_STRU));

    /* ���PS�¼���DNS��ַ�����¼DNS��ַ*/
    if (pstEvent->stDns.bitOpPrimDnsAddr)
    {
        stParam.ulPrimDNS = AT_GetLanAddr32(pstEvent->stDns.aucPrimDnsAddr);
    }

    if (pstEvent->stDns.bitOpSecDnsAddr)
    {
        stParam.ulSndDNS = AT_GetLanAddr32(pstEvent->stDns.aucSecDnsAddr);
    }

    /* ��¼IP��ַ*/
    stParam.ulIPAddr = AT_GetLanAddr32(pstEvent->stPdpAddr.aucIpv4Addr);

    if (VOS_ERR == AT_DHCPServerSetUp(&stParam, &stConfig))
    {
        AT_ERR_LOG("AT_PS_ProcConnInd : ERROR AT_DHCPServerSetUp Error!" );
    }
    else
    {
        /* ����IPv4��ַ���� */
        AT_PS_PdpAddrProc(ucCallId, &stConfig, pstEvent);
    }

    return;
}


VOS_VOID AT_PS_SndCallConnectedResult(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType
)
{
    AT_PS_USER_INFO_STRU               *pstUsrInfo          = VOS_NULL_PTR;
    AT_PS_RPT_CONN_RSLT_FUNC            pRptConnRsltFunc    = VOS_NULL_PTR;

    /* ��ȡ��ǰ�����û���Ϣ */
    pstUsrInfo       = AT_PS_GetUserInfo(usClientId, ucCallId);

    /* ���¼�������в��Ҵ����� */
    pRptConnRsltFunc = AT_PS_GetRptConnResultFunc(pstUsrInfo->ucUsrType);

    /* ������������������ */
    if (VOS_NULL_PTR != pRptConnRsltFunc)
    {
        pRptConnRsltFunc(pstUsrInfo->ucUsrCid,
                         pstUsrInfo->enPortIndex,
                         enPdpType);
    }
    else
    {
        AT_ERR_LOG("AT_PS_SndCallConnectedResult:ERROR: User type is invalid!");
    }

    return;
}


VOS_UINT32 AT_PS_CallEndedReportAlready(
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity,
    TAF_PDP_TYPE_ENUM_UINT8            *penPdpType
)
{
    if (TAF_PDP_IPV4 == *penPdpType)
    {
        if (VOS_TRUE == pstCallEntity->ucIpv4DendRptFlg)
        {
            AT_NORM_LOG("AT_PS_CallEndedReportAlready: Has already report Ipv4 Dend!");
            return VOS_TRUE;
        }

        pstCallEntity->ucIpv4DendRptFlg = VOS_TRUE;
    }
    else if (TAF_PDP_IPV6 == *penPdpType)
    {
        if (VOS_TRUE == pstCallEntity->ucIpv6DendRptFlg)
        {
            AT_NORM_LOG("AT_PS_CallEndedReportAlready: Has already report Ipv6 Dend!");
            return VOS_TRUE;
        }

        pstCallEntity->ucIpv6DendRptFlg = VOS_TRUE;
    }
    else if (TAF_PDP_IPV4V6 == *penPdpType)
    {
        if ( (VOS_TRUE == pstCallEntity->ucIpv4DendRptFlg)
          && (VOS_TRUE == pstCallEntity->ucIpv6DendRptFlg))
        {
            AT_NORM_LOG("AT_PS_CallEndedReportAlready: Has already report IPV4 Ipv6 DCONN!");
            return VOS_TRUE;
        }

        if ( (VOS_TRUE == pstCallEntity->ucIpv4DendRptFlg)
          || (VOS_TRUE == pstCallEntity->ucIpv6DendRptFlg))
        {
            *penPdpType = (VOS_TRUE == pstCallEntity->ucIpv4DendRptFlg) ? TAF_PDP_IPV6 : TAF_PDP_IPV4;
        }

        pstCallEntity->ucIpv4DendRptFlg = VOS_TRUE;
        pstCallEntity->ucIpv6DendRptFlg = VOS_TRUE;
    }
    else
    {
        /* nothing to do */
        AT_ERR_LOG("AT_PS_CallEndedReportAlready: Pdp Type is error!");
    }

    return VOS_FALSE;
}


VOS_VOID AT_PS_ChgCallEndedCause(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    TAF_PS_CAUSE_ENUM_UINT32           *penCause
)
{
    TAF_PS_CAUSE_ENUM_UINT32            enHoFailCause;

    enHoFailCause   = TAF_PS_CAUSE_SUCCESS;

    enHoFailCause = AT_PS_GetCallHandOverFailCause(usClientId, ucCallId);

    if (TAF_PS_CAUSE_SUCCESS != enHoFailCause)
    {
        *penCause = enHoFailCause;
    }

    return;
}


VOS_VOID AT_PS_SndCallEndedResult(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType,
    TAF_PS_CAUSE_ENUM_UINT32            enCause,
    TAF_PS_APN_DATA_SYS_ENUM_UINT8      enDataSys
)
{
    AT_PS_USER_INFO_STRU               *pstUsrInfo      = VOS_NULL_PTR;
    AT_PS_RPT_END_RSLT_FUNC             pRptEndRsltFunc = VOS_NULL_PTR;
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity   = VOS_NULL_PTR;

    /* ��ȡ��ǰ�����û���Ϣ */
    pstUsrInfo      = AT_PS_GetUserInfo(usClientId, ucCallId);
    pstCallEntity   = AT_PS_GetCallEntity(usClientId, ucCallId);

    /* ���¼�������в��Ҵ����� */
    pRptEndRsltFunc = AT_PS_GetRptEndResultFunc(pstUsrInfo->ucUsrType);

    if (VOS_FALSE == AT_PS_IsNeedSndCallEnded(usClientId, ucCallId, enDataSys))
    {
        AT_NORM_LOG("AT_PS_SndCallEndedResult: not need report DEND!");
        return;
    }

    if (VOS_TRUE == AT_PS_CallEndedReportAlready(pstCallEntity, &enPdpType))
    {
        AT_NORM_LOG("AT_PS_SndCallEndedResult: has already report dend!");
        return;
    }

    AT_PS_ChgCallEndedCause(usClientId, ucCallId, &enCause);

    /* ������������������ */
    if (VOS_NULL_PTR != pRptEndRsltFunc)
    {
        pRptEndRsltFunc(pstUsrInfo->ucUsrCid,
                        pstUsrInfo->enPortIndex,
                        enPdpType,
                        enCause);
    }
    else
    {
        AT_ERR_LOG("AT_PS_SndCallEndedResult:ERROR: User type is invalid!");
    }

    return;
}


VOS_VOID AT_PS_ReportCustomPcoInfo(
    TAF_PS_CUSTOM_PCO_INFO_STRU        *pstPcoCustInfo,
    TAF_PS_PDN_OPERATE_TYPE_ENUM_UINT8  enOperateType,
    VOS_UINT8                           ucCid,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType,
    AT_CLIENT_TAB_INDEX_UINT8           enPortIndex
)
{
    VOS_UINT8                          *pstrIpType      = VOS_NULL_PTR;
    VOS_UINT8                           aucStrIpv4[]    = "IPV4";
    VOS_UINT8                           aucStrIpv6[]    = "IPV6";
    VOS_UINT8                           aucStrIpv4v6[]  = "IPV4V6";
    VOS_UINT32                          i = 0;
    VOS_UINT16                          usLength = 0;

    if (TAF_PDP_IPV4 == enPdpType)
    {
        pstrIpType = aucStrIpv4;
    }
    else if (TAF_PDP_IPV6 == enPdpType)
    {
        pstrIpType = aucStrIpv6;
    }
    else if (TAF_PDP_IPV4V6 == enPdpType)
    {
        pstrIpType = aucStrIpv4v6;
    }
    else
    {
        AT_ERR_LOG1("AT_PS_ReportCustomPcoInfo:ERROR: invalid pdp type ", enPdpType);
        return;
    }

    if (pstPcoCustInfo->ulContainerNum > TAF_PS_MAX_CUSTOM_PCO_CONTAINER_NUM)
    {
        AT_ERR_LOG1("AT_PS_ReportCustomPcoInfo:ERROR: ulContainerNum abnormal ", pstPcoCustInfo->ulContainerNum);
        return;
    }

    for ( i = 0; i < pstPcoCustInfo->ulContainerNum; i++ )
    {
        usLength = 0;

        /* �ϱ���ʽΪ:^CUSTPCOINFO:CID,OPERATETYPE,IPTYPE,CONTAINERID,CONTAINER CONTENT */
        usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN, (VOS_CHAR *)pgucAtSndCodeAddr, (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                           "%s%s %d,%d,\"%s\",%x,\"",
                                           gaucAtCrLf,
                                           gastAtStringTab[AT_STRING_CUST_PCO_INFO].pucText,
                                           ucCid,
                                           enOperateType,
                                           pstrIpType,
                                           pstPcoCustInfo->astContainerList[i].usContainerId);


        usLength += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN,
                                                        (VOS_CHAR *)pgucAtSndCodeAddr,
                                                        (VOS_UINT8 *)pgucAtSndCodeAddr + usLength,
                                                        pstPcoCustInfo->astContainerList[i].aucContents,
                                                        (VOS_UINT16)pstPcoCustInfo->astContainerList[i].ucContentLen);

        usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                           (VOS_CHAR *)pgucAtSndCodeAddr,
                                           (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                           "\"%s",
                                           gaucAtCrLf);

        /* ����At_SendResultData���������� */
        At_SendResultData(enPortIndex, pgucAtSndCodeAddr, usLength);
    }

    return;
}


VOS_VOID AT_PS_GenCallDialParam(
    AT_DIAL_PARAM_STRU                 *pstCallDialParam,
    AT_DIAL_PARAM_STRU                 *pstUsrDialParam,
    VOS_UINT8                           ucCid,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType
)
{
    /* ��ȡ�û����Ų��� */
    *pstCallDialParam           = *pstUsrDialParam;

    /* ˢ��CID��PDP���� */
    pstCallDialParam->ucCid     = ucCid;
    pstCallDialParam->enPdpType = enPdpType;

    return;
}


VOS_VOID AT_PS_RegFCPoint(
    VOS_UINT8                           ucCallId,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    AT_PS_USER_INFO_STRU               *pstUserInfo = VOS_NULL_PTR;
    AT_PS_REG_FC_POINT_STRU            *pstRegFcPointFuncTblPtr = VOS_NULL_PTR;
    AT_PS_REG_FC_POINT_FUNC             pRegFcPointFunc = VOS_NULL_PTR;
    VOS_UINT32                          ulCnt;

    pstUserInfo             = AT_PS_GetUserInfo(pstEvent->stCtrl.usClientId, ucCallId);
    pstRegFcPointFuncTblPtr = AT_PS_GET_REG_FC_POINT_FUNC_TBL_PTR();

    /* ���¼�������в��Ҵ����� */
    for (ulCnt = 0; ulCnt < AT_PS_GET_REG_FC_POINT_FUNC_TBL_SIZE(); ulCnt++)
    {
        if (pstUserInfo->ucUsrType == pstRegFcPointFuncTblPtr[ulCnt].ucUsrType)
        {
            /* ����ƥ�� */
            pRegFcPointFunc = pstRegFcPointFuncTblPtr[ulCnt].pRegFcPoint;
            break;
        }
    }

    /* ������������������ */
    if (VOS_NULL_PTR != pRegFcPointFunc)
    {
        pRegFcPointFunc(pstUserInfo->ucUsrCid, pstEvent);
    }
    else
    {
        AT_ERR_LOG("AT_PS_RegFCPoint:ERROR: User type is invalid!");
    }

    return;
}


VOS_VOID AT_PS_DeRegFCPoint(
    VOS_UINT8                           ucCallId,
    TAF_PS_CALL_PDP_DEACTIVATE_CNF_STRU *pstEvent
)
{
    AT_PS_USER_INFO_STRU               *pstUserInfo = VOS_NULL_PTR;
    AT_PS_DEREG_FC_POINT_STRU          *pstDeRegFcPointFuncTblPtr = VOS_NULL_PTR;
    AT_PS_DEREG_FC_POINT_FUNC           pDeRegFcPointFunc = VOS_NULL_PTR;
    VOS_UINT32                          ulCnt;

    pstUserInfo               = AT_PS_GetUserInfo(pstEvent->stCtrl.usClientId, ucCallId);
    pstDeRegFcPointFuncTblPtr = AT_PS_GET_DEREG_FC_POINT_FUNC_TBL_PTR();

    /* ���¼�������в��Ҵ����� */
    for (ulCnt = 0; ulCnt < AT_PS_GET_DEREG_FC_POINT_FUNC_TBL_SIZE(); ulCnt++)
    {
        if (pstUserInfo->ucUsrType == pstDeRegFcPointFuncTblPtr[ulCnt].ucUsrType)
        {
            /* ����ƥ�� */
            pDeRegFcPointFunc = pstDeRegFcPointFuncTblPtr[ulCnt].pDeRegFcPoint;
            break;
        }
    }

    /* ������������������ */
    if (VOS_NULL_PTR != pDeRegFcPointFunc)
    {
        pDeRegFcPointFunc(pstUserInfo->ucUsrCid, pstEvent);
    }
    else
    {
        AT_ERR_LOG("AT_PS_DeRegFCPoint:ERROR: User type is invalid!");
    }

    return;
}


VOS_VOID AT_PS_SndRnicIpv4PdpActInd(
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    AT_RNIC_PDN_INFO_CFG_IND_STRU      *pstMsg        = VOS_NULL_PTR;
    VOS_UINT32                          ulRslt;
    MODEM_ID_ENUM_UINT16                enModemId;

    /* ͨ��client���modemid */
    ulRslt = AT_GetModemIdFromClient(pstEvent->stCtrl.usClientId, &enModemId);
    if (VOS_OK != ulRslt)
    {
        AT_ERR_LOG("AT_PS_SndRnicIpv4PdpActInd: Get modem id fail.");
        return;
    }

    /* ����AT_RNIC_PDN_INFO_CFG_IND_STRU��Ϣ */
    /*lint -save -e516 */
    pstMsg = (AT_RNIC_PDN_INFO_CFG_IND_STRU *)AT_ALLOC_MSG_WITH_HDR(
                            sizeof(AT_RNIC_PDN_INFO_CFG_IND_STRU));
    /*lint -restore */
    if (VOS_NULL_PTR == pstMsg)
    {
        AT_WARN_LOG("AT_PS_SndRnicIpv4PdpActInd: alloc msg fail!");
        return;
    }

    /* ��ʼ����Ϣ */
    TAF_MEM_SET_S(AT_GET_MSG_ENTITY(pstMsg), AT_GET_MSG_LENGTH(pstMsg),
                  0x00, AT_GET_MSG_LENGTH(pstMsg));

    /* ��д��Ϣͷ */
    AT_CFG_RNIC_MSG_HDR(pstMsg, ID_AT_RNIC_PDN_INFO_CFG_IND);

    /* ��д��Ϣ���� */
    pstMsg->bitOpIpv4PdnInfo = VOS_TRUE;
    pstMsg->bitOpIpv6PdnInfo = VOS_FALSE;
    pstMsg->ucRmNetId        = pstCallEntity->ucRmNetId;
    pstMsg->ucIfaceId        = pstCallEntity->ucIfaceId;

    pstMsg->ucRabId          = pstCallEntity->stIpv4DhcpInfo.ucRabId;

    pstMsg->enModemId        = enModemId;


    if (VOS_OK != PS_SEND_MSG(WUEPS_PID_AT, pstMsg))
    {
        AT_WARN_LOG("AT_PS_SndRnicIpv4PdpActInd: Send msg fail!");
    }

    return;
}


VOS_VOID AT_PS_SndRnicIpv6PdpActInd(
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    AT_RNIC_PDN_INFO_CFG_IND_STRU      *pstMsg        = VOS_NULL_PTR;
    VOS_UINT32                          ulRslt;
    MODEM_ID_ENUM_UINT16                enModemId;

    /* ͨ��client���modemid */
    ulRslt = AT_GetModemIdFromClient(pstEvent->stCtrl.usClientId, &enModemId);
    if (VOS_OK != ulRslt)
    {
        AT_ERR_LOG("AT_PS_SndRnicIpv6PdpActInd: Get modem id fail.");
        return;
    }

    /* ����AT_RNIC_PDN_INFO_CFG_IND_STRU��Ϣ */
    /*lint -save -e516 */
    pstMsg = (AT_RNIC_PDN_INFO_CFG_IND_STRU *)AT_ALLOC_MSG_WITH_HDR(
                            sizeof(AT_RNIC_PDN_INFO_CFG_IND_STRU));
    /*lint -restore */
    if (VOS_NULL_PTR == pstMsg)
    {
        AT_WARN_LOG("AT_PS_SndRnicIpv6PdpActInd: alloc msg fail!");
        return;
    }

    /* ��ʼ����Ϣ */
    TAF_MEM_SET_S(AT_GET_MSG_ENTITY(pstMsg), AT_GET_MSG_LENGTH(pstMsg),
                  0x00, AT_GET_MSG_LENGTH(pstMsg));

    /* ��д��Ϣͷ */
    AT_CFG_RNIC_MSG_HDR(pstMsg, ID_AT_RNIC_PDN_INFO_CFG_IND);

    /* ��д��Ϣ���� */
    pstMsg->bitOpIpv4PdnInfo = VOS_FALSE;
    pstMsg->bitOpIpv6PdnInfo = VOS_TRUE;
    pstMsg->ucRmNetId        = pstCallEntity->ucRmNetId;
    pstMsg->ucIfaceId        = pstCallEntity->ucIfaceId;

    pstMsg->ucRabId          = pstCallEntity->stIpv6DhcpInfo.ucRabId;

    pstMsg->enModemId        = enModemId;


    if (VOS_OK != PS_SEND_MSG(WUEPS_PID_AT, pstMsg))
    {
        AT_WARN_LOG("AT_PS_SndRnicIpv6PdpActInd: Send msg fail!");
    }

    return;
}


VOS_VOID AT_PS_SndRnicPdpActInd(
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType
)
{
    /* ��DIPC�ϱ�PDP IPv4�Ѿ������¼� */
    if (TAF_PDP_IPV4 == (enPdpType & TAF_PDP_IPV4))
    {
        AT_PS_SndRnicIpv4PdpActInd(pstCallEntity, pstEvent);
    }

    /* ��DIPC�ϱ�PDP IPv6�Ѿ������¼� */
    if (TAF_PDP_IPV6 == (enPdpType & TAF_PDP_IPV6))
    {
        AT_PS_SndRnicIpv6PdpActInd(pstCallEntity, pstEvent);
    }

    return;

}


VOS_VOID AT_PS_SndRnicPdpDeactInd(
    AT_PS_CALL_ENTITY_STRU              *pstCallEntity,
    TAF_PS_CALL_PDP_DEACTIVATE_CNF_STRU *pstEvent,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType
)
{
    AT_RNIC_PDN_INFO_REL_IND_STRU      *pstMsg;

    /* ����AT_RNIC_PDN_INFO_REL_IND_STRU��Ϣ */
    /*lint -save -e516 */
    pstMsg = (AT_RNIC_PDN_INFO_REL_IND_STRU *)AT_ALLOC_MSG_WITH_HDR(
                            sizeof(AT_RNIC_PDN_INFO_REL_IND_STRU));
    /*lint -restore */
    if (VOS_NULL_PTR == pstMsg)
    {
        AT_WARN_LOG("AT_NotifyRnicPdpDeactInd: alloc msg fail!");
        return;
    }

    /* ��ʼ����Ϣ */
    TAF_MEM_SET_S(AT_GET_MSG_ENTITY(pstMsg), AT_GET_MSG_LENGTH(pstMsg),
                  0x00, AT_GET_MSG_LENGTH(pstMsg));

    /* ��д��Ϣͷ */
    AT_CFG_RNIC_MSG_HDR(pstMsg, ID_AT_RNIC_PDN_INFO_REL_IND);

    /* ��д��Ϣ���� */
    pstMsg->bitOpIpv4PdnInfo = ((TAF_PDP_IPV4 == (enPdpType & TAF_PDP_IPV4)) ? VOS_TRUE : VOS_FALSE);
    pstMsg->bitOpIpv6PdnInfo = ((TAF_PDP_IPV6 == (enPdpType & TAF_PDP_IPV6)) ? VOS_TRUE : VOS_FALSE);
    pstMsg->ucRmNetId        = pstCallEntity->ucRmNetId;
    pstMsg->ucIfaceId        = pstCallEntity->ucIfaceId;

    if (VOS_OK != PS_SEND_MSG(WUEPS_PID_AT, pstMsg))
    {
        AT_WARN_LOG("AT_NotifyRnicPdpDeactInd: Send msg fail!");
        return;
    }

    return;
}


VOS_VOID AT_PS_ActivateRmNet(
    VOS_UINT8                           ucCallId,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity             = VOS_NULL_PTR;
    AT_PS_SND_PDP_ACT_IND_STRU         *pstSndPdpActIndFuncTblPtr = VOS_NULL_PTR;
    AT_PS_SND_PDP_ACT_IND_FUNC          pSndPdpActIndFunc = VOS_NULL_PTR;
    VOS_UINT32                          ulCnt;

    pstCallEntity = AT_PS_GetCallEntity(pstEvent->stCtrl.usClientId, ucCallId);
    pstSndPdpActIndFuncTblPtr = AT_PS_GET_SND_PDP_ACT_IND_FUNC_TBL_PTR();

    /* ���¼�������в��Ҵ����� */
    for (ulCnt = 0; ulCnt < AT_PS_GET_SND_PDP_ACT_IND_FUNC_TBL_SIZE(); ulCnt++)
    {
        if (pstCallEntity->stUserInfo.ucUsrType == pstSndPdpActIndFuncTblPtr[ulCnt].ucUsrType)
        {
            /* ����ƥ�� */
            pSndPdpActIndFunc = pstSndPdpActIndFuncTblPtr[ulCnt].pSndPdpActInd;
            break;
        }
    }

    /* ������������������ */
    if (VOS_NULL_PTR != pSndPdpActIndFunc)
    {
        pSndPdpActIndFunc(pstCallEntity, pstEvent, enPdpType);
    }
    else
    {
        AT_ERR_LOG("AT_PS_ActivateRmNet:ERROR: User type is invalid!");
    }

    return;
}


VOS_VOID AT_PS_DeactivateRmNet(
    VOS_UINT8                           ucCallId,
    TAF_PS_CALL_PDP_DEACTIVATE_CNF_STRU *pstEvent,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType
)
{
    AT_PS_CALL_ENTITY_STRU          *pstCallEntity               = VOS_NULL_PTR;
    AT_PS_SND_PDP_DEACT_IND_STRU    *pstSndPdpDeActIndFuncTblPtr = VOS_NULL_PTR;
    AT_PS_SND_PDP_DEACT_IND_FUNC     pSndPdpDeActIndFunc         = VOS_NULL_PTR;
    VOS_UINT32                       ulCnt;

    pstCallEntity = AT_PS_GetCallEntity(pstEvent->stCtrl.usClientId, ucCallId);
    pstSndPdpDeActIndFuncTblPtr = AT_PS_GET_SND_PDP_DEACT_IND_FUNC_TBL_PTR();

    /* ���¼�������в��Ҵ����� */
    for (ulCnt = 0; ulCnt < AT_PS_GET_SND_PDP_DEACT_IND_FUNC_TBL_SIZE(); ulCnt++)
    {
        if (pstCallEntity->stUserInfo.ucUsrType == pstSndPdpDeActIndFuncTblPtr[ulCnt].ucUsrType)
        {
            /* ����ƥ�� */
            pSndPdpDeActIndFunc = pstSndPdpDeActIndFuncTblPtr[ulCnt].pSndPdpDeActInd;
            break;
        }
    }

    /* ������������������ */
    if (VOS_NULL_PTR != pSndPdpDeActIndFunc)
    {
        pSndPdpDeActIndFunc(pstCallEntity, pstEvent, enPdpType);
    }
    else
    {
        AT_ERR_LOG("AT_PS_DeactivateRmNet:ERROR: User type is invalid!");
    }

    return;
}


VOS_UINT32 AT_PS_IsHoEndCellularConnect(
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity,
    TAF_PDP_TYPE_ENUM_UINT8             enConnectPdpType
)
{
    /* ��IPV4����ж�HANDOVER�Ƿ���� */
    if ( (TAF_PDP_IPV4 == pstCallEntity->enHoPdpType)
      && (TAF_PDP_IPV4 == enConnectPdpType))
    {
        AT_NORM_LOG("AT_PS_IsHoEndCellularConnect: IPV4 is handover end!");
        return VOS_TRUE;
    }

    /* ��IPV6����ж�HANDOVER�Ƿ���� */
    if ( (TAF_PDP_IPV6 == pstCallEntity->enHoPdpType)
      && (TAF_PDP_IPV6 == enConnectPdpType))
    {
        AT_NORM_LOG("AT_PS_IsHoEndCellularConnect: IPV6 is handover end!");
        return VOS_TRUE;
    }

    /* ˫ջ����ж�HANDOVER�Ƿ���� */
    if (TAF_PDP_IPV4V6 == pstCallEntity->enHoPdpType)
    {
        if ( (TAF_PDP_IPV4 == enConnectPdpType)
          && (AT_PDP_STATE_ACTED == pstCallEntity->enIpv6State)
          && (VOS_TRUE == pstCallEntity->stIpv6RaInfo.bitOpPrefixAddr))
        {
            AT_NORM_LOG("AT_PS_IsHoEndCellularConnect: IPV4V6 is handover end 1!");
            return VOS_TRUE;
        }

        if ( (TAF_PDP_IPV6 == enConnectPdpType)
          && (AT_PDP_STATE_ACTED == pstCallEntity->enIpv4State))
        {
            AT_NORM_LOG("AT_PS_IsHoEndCellularConnect: IPV4V6 is handover end 2!");
            return VOS_TRUE;
        }
    }

    AT_NORM_LOG("AT_PS_IsHoEndCellularConnect: handover is not end!");
    return VOS_FALSE;
}


VOS_VOID AT_PS_ProcHoEndCellularConnect(
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity,
    VOS_UINT8                           ucCallId
)
{
    TAF_PS_APN_DATA_SYS_POLICY_INFO_STRU   *pstApnDataSysPolicyInfo = VOS_NULL_PTR;
    VOS_UINT32                              ulTmrName;
    VOS_UINT32                              ulTmrParam;

    /* ͨ����ѡ�����õ���ѡ���� */
    pstApnDataSysPolicyInfo = AT_PS_GetApnDataSysPolicyInfo(pstCallEntity->stUserInfo.enPortIndex,
                                                            pstCallEntity->stApnDataSysInfo.ucDataSysPolicyIndex);

    AT_PS_SetCallHandOverFlg(pstCallEntity->stUserInfo.enPortIndex, ucCallId, VOS_FALSE);

    /* ����handover�������� */
    AT_PS_SetHoCallType(pstCallEntity->stUserInfo.enPortIndex, ucCallId, TAF_PDP_TYPE_BUTT);

    /* ��WLAN�·��𱾵�ȥ���� */
    (VOS_VOID)AT_PS_HangupWlanCall(pstCallEntity->stUserInfo.enUserIndex, ucCallId, VOS_TRUE, VOS_TRUE);

    /* ������ƹ�Ҷ�ʱ�� */
    AT_SET_PROTECT_PDN_IN_DATA_SYS_TMR_NAME(ulTmrName);
    AT_SET_PROTECT_PDN_IN_DATA_SYS_TMR_PARAM(ulTmrParam, pstCallEntity->stUserInfo.enPortIndex, ucCallId);

    AT_StartRelTimer(&(pstCallEntity->stPsCallTimerInfo.hProtectInDataSysTmrHdl),
                     pstApnDataSysPolicyInfo->stDataSysPolicy.usCelluarOutProtectTimeLen * 1000,
                     ulTmrName,
                     ulTmrParam,
                     VOS_RELTIMER_NOLOOP);


    return;
}



VOS_UINT64 AT_PS_GenerateRandomIPv6IID(VOS_VOID)
{
    VOS_UINT32                         *pulAddr = VOS_NULL_PTR;
    VOS_UINT64                          ullIID;
    VOS_UINT32                          ulSlice;

    pulAddr = (VOS_UINT32 *)&ullIID;
    ulSlice = VOS_GetSlice();

    /* �ڽ���PC����ʱ����ʹ�����ֵ */
    VOS_SetSeed(ulSlice);
    pulAddr[0] = VOS_Rand(0xFFFFFFFFU);
    VOS_SetSeed(pulAddr[0]);
    pulAddr[1] = VOS_Rand(0xFFFFFFFFU);

    *(VOS_UINT8 *)(&pulAddr[0]) &= ~0x02;

    return ullIID;
}


VOS_UINT32 AT_PS_GetIPv6IIDMgrListSize(
    AT_PS_IPV6_IID_MGR_INFO_STRU       *pstMgrInfo
)
{
    HI_LIST_S                          *pstMe  = VOS_NULL_PTR;
    VOS_UINT32                          ulSize = 0;

    msp_list_for_each(pstMe, &(pstMgrInfo->stListHead))
    {
        ulSize++;
    }

    return ulSize;
}


AT_PS_IPV6_IID_ENTRY_STRU* AT_PS_LookupIPv6IIDMgrEntry(
    AT_PS_IPV6_IID_MGR_INFO_STRU       *pstMgrInfo,
    VOS_UINT8                          *pucApn
)
{
    AT_PS_IPV6_IID_ENTRY_STRU          *pstEntry = VOS_NULL_PTR;
    HI_LIST_S                          *pstMe    = VOS_NULL_PTR;
    VOS_UINT32                          ulFound  = VOS_FALSE;

    /* ��������, ƥ��APN */
    msp_list_for_each(pstMe, &(pstMgrInfo->stListHead))
    {
        pstEntry = msp_list_entry(pstMe, AT_PS_IPV6_IID_ENTRY_STRU, stList);
        if (!VOS_StrNiCmp((VOS_CHAR *)pstEntry->aucApn,
                          (VOS_CHAR *)pucApn,
                          (TAF_MAX_APN_LEN + 1)))
        {
            ulFound = VOS_TRUE;
            break;
        }
    }

    return ((VOS_TRUE == ulFound) ? pstEntry : VOS_NULL_PTR);
}


AT_PS_IPV6_IID_ENTRY_STRU* AT_PS_EvictIPv6IIDMgrEntry(
    AT_PS_IPV6_IID_MGR_INFO_STRU       *pstMgrInfo
)
{
    HI_LIST_S                          *pstListHead = VOS_NULL_PTR;
    AT_PS_IPV6_IID_ENTRY_STRU          *pstEntry    = VOS_NULL_PTR;

    pstListHead = &(pstMgrInfo->stListHead);
    if (VOS_TRUE == msp_list_empty(pstListHead))
    {
        AT_ERR_LOG("AT_PS_EvictIPv6IIDMgrEntry: list is empty.");
        return VOS_NULL_PTR;
    }

    pstEntry = msp_list_last_entry(pstListHead, AT_PS_IPV6_IID_ENTRY_STRU, stList);
    msp_list_del(&(pstEntry->stList));

    return pstEntry;
}


AT_PS_IPV6_IID_ENTRY_STRU* AT_PS_AllocIPv6IIDMgrEntry(
    AT_PS_IPV6_IID_MGR_INFO_STRU       *pstMgrInfo
)
{
    AT_PS_IPV6_IID_ENTRY_STRU          *pstEntry = VOS_NULL_PTR;

    pstEntry = (AT_PS_IPV6_IID_ENTRY_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT,
                    sizeof(AT_PS_IPV6_IID_ENTRY_STRU));//lint !e516

    return pstEntry;
}


AT_PS_IPV6_IID_ENTRY_STRU* AT_PS_CreateIPv6IIDMgrEntry(
    AT_PS_IPV6_IID_MGR_INFO_STRU       *pstMgrInfo
)
{
    AT_PS_IPV6_IID_ENTRY_STRU          *pstEntry = VOS_NULL_PTR;
    VOS_UINT32                          ulSize;

    ulSize = AT_PS_GetIPv6IIDMgrListSize(pstMgrInfo);
    if (ulSize >= AT_PS_IPV6_IID_MGR_TBL_SIZE)
    {
        /* ����ڵ�ﵽ������, ��β������һ���ڵ� */
        AT_NORM_LOG("AT_PS_CreateIPv6IIDMgrEntry: list is full, evict an entry.");
        pstEntry = AT_PS_EvictIPv6IIDMgrEntry(pstMgrInfo);
    }
    else
    {
        /* �����½ڵ�, �������ʧ��, ��β������һ���ڵ� */
        pstEntry = AT_PS_AllocIPv6IIDMgrEntry(pstMgrInfo);
        if (VOS_NULL_PTR == pstEntry)
        {
            AT_ERR_LOG("AT_PS_CreateIPv6IIDMgrEntry: alloc an entry failed, try to evict an entry.");
            pstEntry = AT_PS_EvictIPv6IIDMgrEntry(pstMgrInfo);
        }
    }

    if (VOS_NULL_PTR != pstEntry)
    {
        /* ��ʼ���ڵ� */
        AT_NORM_LOG("AT_PS_CreateIPv6IIDMgrEntry: entry is created.");
        TAF_MEM_SET_S(pstEntry, sizeof(AT_PS_IPV6_IID_ENTRY_STRU),
                      0x00, sizeof(AT_PS_IPV6_IID_ENTRY_STRU));
    }

    return pstEntry;
}


VOS_UINT64 AT_PS_GetIPv6IID(
    MODEM_ID_ENUM_UINT16                enModemId,
    VOS_UINT8                          *pucApn,
    VOS_UINT8                          *pucPrefix
)
{
    AT_PS_IPV6_IID_MGR_INFO_STRU       *pstMgrInfo = VOS_NULL_PTR;
    AT_PS_IPV6_IID_ENTRY_STRU          *pstEntry   = VOS_NULL_PTR;
    VOS_UINT64                          ullIID     = 0;

    /* ��ȡIPv6�ӿ�ID��¼���� */
    pstMgrInfo = AT_GetPsIPv6IIDMgrInfoByModemId(enModemId);
    if (VOS_NULL_PTR == pstMgrInfo)
    {
        AT_ERR_LOG1("AT_PS_GetIPv6IID: modem id is invalid.", enModemId);
        return 0;
    }

    /* ��֧�ֿ�APN */
    if (0 == VOS_StrLen((VOS_CHAR *)pucApn))
    {
        AT_ERR_LOG("AT_PS_GetIPv6IID: apn length is 0.");
        return 0;
    }

    /* ǰ׺��Ч�Լ�� */
    if (0 == *((VOS_UINT64 *)pucPrefix))
    {
        AT_ERR_LOG("AT_PS_GetIPv6IID: prefix is invalid.");
        return 0;
    }

    /* ����APN����IPv6�ӿ�ID��¼ */
    pstEntry = AT_PS_LookupIPv6IIDMgrEntry(pstMgrInfo, pucApn);
    if (VOS_NULL_PTR == pstEntry)
    {
        /* δ�ҵ���¼, ����һ���µļ�¼ */
        AT_NORM_LOG("AT_PS_GetIPv6IID: entry is not found.");

        pstEntry = AT_PS_CreateIPv6IIDMgrEntry(pstMgrInfo);
        if (VOS_NULL_PTR != pstEntry)
        {
            /* �����¼�¼�ɹ�, ���¼�¼��Ϣ */
            TAF_MEM_CPY_S(pstEntry->aucApn, sizeof(pstEntry->aucApn),
                          pucApn, VOS_StrLen((VOS_CHAR *)pucApn));
            TAF_MEM_CPY_S(pstEntry->aucPrefix, sizeof(pstEntry->aucPrefix),
                          pucPrefix, TAF_IPV6_PREFIX_LEN);
            pstEntry->ullIID = AT_PS_GenerateRandomIPv6IID();
            msp_list_add(&(pstEntry->stList), &(pstMgrInfo->stListHead));

            ullIID = pstEntry->ullIID;
        }
    }
    else
    {
        /*
         * �ҵ���¼, ƥ��IPv6��ַǰ׺
         * ���ƥ��, ��ʹ����һ�εĽӿ�ID, ����������ɲ����¼�¼
         */
        AT_NORM_LOG("AT_PS_GetIPv6IID: entry is found.");

        if (TAF_MEM_CMP(pstEntry->aucPrefix, pucPrefix, TAF_IPV6_PREFIX_LEN))
        {
            TAF_MEM_CPY_S(pstEntry->aucPrefix, sizeof(pstEntry->aucPrefix),
                          pucPrefix, TAF_IPV6_PREFIX_LEN);
            pstEntry->ullIID = AT_PS_GenerateRandomIPv6IID();
        }

        msp_list_del(&(pstEntry->stList));
        msp_list_add(&(pstEntry->stList), &(pstMgrInfo->stListHead));

        ullIID = pstEntry->ullIID;
    }

    return ullIID;
}


VOS_UINT32 AT_PS_GetPdpTypeChgMgrListSize(
    AT_PS_PDP_TYPE_CHG_MGR_INFO_STRU       *pstMgrInfo
)
{
    HI_LIST_S                          *pstMe  = VOS_NULL_PTR;
    VOS_UINT32                          ulSize = 0;

    msp_list_for_each(pstMe, &(pstMgrInfo->stListHead))
    {
        ulSize++;
    }

    return ulSize;
}


AT_PS_PDP_TYPE_CHG_ENTRY_STRU* AT_PS_LookupPdpTypeChgMgrEntry(
    AT_PS_PDP_TYPE_CHG_MGR_INFO_STRU   *pstMgrInfo,
    VOS_UINT8                          *pucApn
)
{
    AT_PS_PDP_TYPE_CHG_ENTRY_STRU      *pstEntry = VOS_NULL_PTR;
    HI_LIST_S                          *pstMe    = VOS_NULL_PTR;
    VOS_UINT32                          ulFound  = VOS_FALSE;

    if (VOS_NULL_PTR == pstMgrInfo)
    {
        AT_ERR_LOG("AT_PS_LookupPdpTypeChgMgrEntry :pstMgrInfo is NULL");
        return VOS_NULL_PTR;
    }

    /* ��������, ƥ��APN */
    msp_list_for_each(pstMe, &(pstMgrInfo->stListHead))
    {
        pstEntry = msp_list_entry(pstMe, AT_PS_PDP_TYPE_CHG_ENTRY_STRU, stList);
        if (!VOS_StrNiCmp((VOS_CHAR *)pstEntry->aucApn,
                          (VOS_CHAR *)pucApn,
                          (TAF_MAX_APN_LEN + 1)))
        {
            ulFound = VOS_TRUE;
            break;
        }
    }

    return ((VOS_TRUE == ulFound) ? pstEntry : VOS_NULL_PTR);
}


AT_PS_PDP_TYPE_CHG_ENTRY_STRU* AT_PS_EvictPdpTypeChgMgrEntry(
    AT_PS_PDP_TYPE_CHG_MGR_INFO_STRU       *pstMgrInfo
)
{
    HI_LIST_S                          *pstListHead = VOS_NULL_PTR;
    AT_PS_PDP_TYPE_CHG_ENTRY_STRU      *pstEntry    = VOS_NULL_PTR;

    pstListHead = &(pstMgrInfo->stListHead);
    if (VOS_TRUE == msp_list_empty(pstListHead))
    {
        AT_ERR_LOG("AT_PS_EvictPdpTypeChgMgrEntry: list is empty.");
        return VOS_NULL_PTR;
    }

    pstEntry = msp_list_last_entry(pstListHead, AT_PS_PDP_TYPE_CHG_ENTRY_STRU, stList);
    msp_list_del(&(pstEntry->stList));

    return pstEntry;
}


AT_PS_PDP_TYPE_CHG_ENTRY_STRU* AT_PS_AllocPdpTypeChgMgrEntry(
    AT_PS_PDP_TYPE_CHG_MGR_INFO_STRU       *pstMgrInfo
)
{
    AT_PS_PDP_TYPE_CHG_ENTRY_STRU          *pstEntry = VOS_NULL_PTR;

    pstEntry = (AT_PS_PDP_TYPE_CHG_ENTRY_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT,
                    sizeof(AT_PS_PDP_TYPE_CHG_ENTRY_STRU)); /*lint !e516*/

    return pstEntry;
}


VOS_VOID AT_PS_CreatePdpTypeChgMgrEntry(
    AT_PS_PDP_TYPE_CHG_MGR_INFO_STRU       *pstMgrInfo,
    VOS_UINT8                              *pucApn,
    TAF_PDP_TYPE_ENUM_UINT8                 enPdpType
)
{
    AT_PS_PDP_TYPE_CHG_ENTRY_STRU      *pstEntry = VOS_NULL_PTR;
    VOS_UINT32                          ulSize;

    ulSize = AT_PS_GetPdpTypeChgMgrListSize(pstMgrInfo);
    if (ulSize >= AT_PS_PDP_TYPE_CHG_MGR_TBL_SIZE)
    {
        /* ����ڵ�ﵽ������, ��β������һ���ڵ� */
        AT_NORM_LOG("AT_PS_CreatePdpTypeChgMgrEntry: list is full, evict an entry.");
        pstEntry = AT_PS_EvictPdpTypeChgMgrEntry(pstMgrInfo);
    }
    else
    {
        /* �����½ڵ�, �������ʧ��, ��β������һ���ڵ� */
        pstEntry = AT_PS_AllocPdpTypeChgMgrEntry(pstMgrInfo);
        if (VOS_NULL_PTR == pstEntry)
        {
            AT_ERR_LOG("AT_PS_CreatePdpTypeChgMgrEntry: alloc an entry failed, try to evict an entry.");
        }
    }

    if (VOS_NULL_PTR != pstEntry)
    {
        /* ��ʼ���ڵ� */
        AT_NORM_LOG("AT_PS_CreatePdpTypeChgMgrEntry: entry is created.");
        TAF_MEM_SET_S(pstEntry, sizeof(AT_PS_PDP_TYPE_CHG_ENTRY_STRU),
                      0x00, sizeof(AT_PS_PDP_TYPE_CHG_ENTRY_STRU));

        /* �����¼�¼�ɹ�, ���¼�¼��Ϣ */
        TAF_MEM_CPY_S(pstEntry->aucApn, sizeof(pstEntry->aucApn),
                      pucApn, VOS_StrLen((VOS_CHAR *)pucApn));

        pstEntry->enPdpType = enPdpType;

        msp_list_add(&(pstEntry->stList), &(pstMgrInfo->stListHead));

        return;
    }

    AT_ERR_LOG("AT_PS_CreatePdpTypeChgMgrEntry: pstEntry is null!");

    return;
}


VOS_VOID AT_PS_ProcPdpTypeChgMgrList(
    VOS_UINT16                              usClientId,
    VOS_UINT8                               ucAPNLen,
    VOS_UINT8                              *pucApn,
    TAF_PS_CAUSE_ENUM_UINT32                enCause
)
{
    AT_COMM_PS_CTX_STRU                *pstCommPsCtx    = VOS_NULL_PTR;
    AT_PS_PDP_TYPE_CHG_MGR_INFO_STRU   *pstMgrInfo      = VOS_NULL_PTR;
    AT_PS_PDP_TYPE_CHG_ENTRY_STRU      *pstEntry        = VOS_NULL_PTR;
    MODEM_ID_ENUM_UINT16                enModemId;
    VOS_UINT32                          i;
    VOS_UINT8                           ucFindNvCauseFlg;

    ucFindNvCauseFlg        = VOS_FALSE;
    enModemId               = MODEM_ID_0;
    i                       = 0;

    if (VOS_OK != AT_GetModemIdFromClient(usClientId, &enModemId))
    {
        AT_ERR_LOG1("AT_PS_ProcDialPdpTypeChg: modem id is invalid.", enModemId);
        return;
    }

    pstCommPsCtx    = AT_GetCommPsCtxAddr();
    pstMgrInfo      = AT_GetPsPdpTypeChgMgrInfoByModemId(enModemId);

    if (VOS_NULL_PTR == pstMgrInfo)
    {
        AT_ERR_LOG("AT_PS_ProcPdpTypeChgMgrList :pstMgrInfo is NULL");
        return;
    }

    if ( (VOS_TRUE == pstCommPsCtx->astPdpTypeChgPolicyCfg[enModemId].ucPdpTypeChgEnableFlg)
      && (0 != ucAPNLen))
    {
        for (i = 0; i < pstCommPsCtx->astPdpTypeChgPolicyCfg[enModemId].ucRejCauseNum; i++)
        {
            if (enCause == pstCommPsCtx->astPdpTypeChgPolicyCfg[enModemId].astPdpTypeChgInfo[i].ucCauseValue + TAF_PS_CAUSE_SM_NW_SECTION_BEGIN)
            {
                /* �ҵ���Ӧ���Ƶ��� */
                ucFindNvCauseFlg = VOS_TRUE;
                break;
            }
        }
    }

    if (VOS_TRUE == ucFindNvCauseFlg)
    {
        pstEntry = AT_PS_LookupPdpTypeChgMgrEntry(pstMgrInfo, pucApn);

        if (VOS_NULL_PTR != pstEntry)
        {
            /* �ҵ���Ӧ�ڵ㣬��Ҫ����PDP TYPE */
            pstEntry->enPdpType     = pstCommPsCtx->astPdpTypeChgPolicyCfg[enModemId].astPdpTypeChgInfo[i].enPdpType;

            msp_list_del(&(pstEntry->stList));
            msp_list_add(&(pstEntry->stList), &(pstMgrInfo->stListHead));

            AT_NORM_LOG1("AT_PS_ProcDialPdpTypeChg: find node and update pdp type is ", pstEntry->enPdpType);
        }
        else
        {
            /* û�ҵ���Ӧ�ڵ㣬��Ҫ�����ڵ� */
            AT_PS_CreatePdpTypeChgMgrEntry(pstMgrInfo,
                                           pucApn,
                                           pstCommPsCtx->astPdpTypeChgPolicyCfg[enModemId].astPdpTypeChgInfo[i].enPdpType);
        }
    }

    return;
}


AT_PS_PDP_TYPE_CHG_ENTRY_STRU* AT_PS_GetPdpTypeChgMgrNode(
    VOS_UINT16                              usClientId,
    VOS_UINT8                               ucCallId
)
{
    AT_COMM_PS_CTX_STRU                *pstCommPsCtx    = VOS_NULL_PTR;
    AT_PS_PDP_TYPE_CHG_MGR_INFO_STRU   *pstMgrInfo      = VOS_NULL_PTR;
    AT_PS_PDP_TYPE_CHG_ENTRY_STRU      *pstEntry        = VOS_NULL_PTR;
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity   = VOS_NULL_PTR;
    MODEM_ID_ENUM_UINT16                enModemId;

    enModemId               = MODEM_ID_0;
    pstCommPsCtx            = AT_GetCommPsCtxAddr();

    if (VOS_OK != AT_GetModemIdFromClient(usClientId, &enModemId))
    {
        AT_ERR_LOG1("AT_PS_GetPdpTypeChgMgrNode: modem id is invalid.", enModemId);
        return pstEntry;
    }

    if (VOS_FALSE == pstCommPsCtx->astPdpTypeChgPolicyCfg[enModemId].ucPdpTypeChgEnableFlg)
    {
        return pstEntry;
    }

    pstMgrInfo      = AT_GetPsPdpTypeChgMgrInfoByModemId(enModemId);
    pstCallEntity   = AT_PS_GetCallEntity(usClientId, ucCallId);

    pstEntry = AT_PS_LookupPdpTypeChgMgrEntry(pstMgrInfo, pstCallEntity->stUsrDialParam.aucAPN);

    return pstEntry;
}



VOS_UINT32 AT_PS_IsIpv6CapabilityValid(VOS_UINT8 ucCapability)
{
    if ((AT_IPV6_CAPABILITY_IPV4_ONLY == ucCapability)
     || (AT_IPV6_CAPABILITY_IPV6_ONLY == ucCapability)
     || (AT_IPV6_CAPABILITY_IPV4V6_OVER_ONE_PDP == ucCapability))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 AT_PS_IsIpv6Support(VOS_VOID)
{
    VOS_UINT8                           ucIpv6Capability;

    ucIpv6Capability = AT_GetCommPsCtxAddr()->ucIpv6Capability;

    if ((AT_IPV6_CAPABILITY_IPV6_ONLY == ucIpv6Capability)
     || (AT_IPV6_CAPABILITY_IPV4V6_OVER_ONE_PDP == ucIpv6Capability))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}

VOS_UINT32 AT_PS_GenIpv6LanAddrWithRadomIID(
    VOS_UINT8                          *pucPrefix,
    VOS_UINT32                          ulPrefixByteLen,
    VOS_UINT8                          *pucIpv6LanAddr
)
{
    VOS_UINT32                         *paulAddr = VOS_NULL_PTR;
    VOS_UINT32                          ulTick;

    paulAddr = (VOS_UINT32 *)pucIpv6LanAddr;

    if (ulPrefixByteLen > AT_IPV6_ADDR_PREFIX_BYTE_LEN)
    {
        return VOS_ERR;
    }

    /* ����IPv6��ַǰ׺ */
    TAF_MEM_CPY_S(pucIpv6LanAddr, TAF_IPV6_ADDR_LEN, pucPrefix, ulPrefixByteLen);

    ulTick = VOS_GetTick();

    /* �ڽ���PC����ʱ����ʹ�����ֵ */
    VOS_SetSeed(ulTick);
    paulAddr[2] = VOS_Rand(0xFFFFFFFFU);

    VOS_SetSeed(ulTick + 10);
    paulAddr[3] = VOS_Rand(0xFFFFFFFFU);


    *(VOS_UINT8 *)(&paulAddr[2]) &= ~0x02;

    return VOS_OK;
}


VOS_VOID  AT_PS_SaveIPv6Dns(
    VOS_UINT8                           ucCallId,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;

    pstCallEntity = AT_PS_GetCallEntity(pstEvent->stCtrl.usClientId, ucCallId);

    pstCallEntity->stIpv6DhcpInfo.bitOpIpv6PriDns    = VOS_FALSE;
    pstCallEntity->stIpv6DhcpInfo.bitOpIpv6SecDns    = VOS_FALSE;

    /* ��������DNS���յ�RA����ʱ��Ҫ����������ʱ��Ҫ֪ͨDRV */
    if (VOS_TRUE == pstEvent->stIpv6Dns.bitOpPrimDnsAddr)
    {
        TAF_MEM_CPY_S((VOS_VOID*)pstCallEntity->stIpv6DhcpInfo.aucIpv6PrimDNS,
                sizeof(pstCallEntity->stIpv6DhcpInfo.aucIpv6PrimDNS),
                pstEvent->stIpv6Dns.aucPrimDnsAddr,
                AT_MAX_IPV6_DNS_LEN);
        pstCallEntity->stIpv6DhcpInfo.bitOpIpv6PriDns = VOS_TRUE;
    }

    if (VOS_TRUE == pstEvent->stIpv6Dns.bitOpSecDnsAddr)
    {
        TAF_MEM_CPY_S((VOS_VOID*)pstCallEntity->stIpv6DhcpInfo.aucIpv6SecDNS,
                   sizeof(pstCallEntity->stIpv6DhcpInfo.aucIpv6SecDNS),
                   pstEvent->stIpv6Dns.aucSecDnsAddr,
                   AT_MAX_IPV6_DNS_LEN);
        pstCallEntity->stIpv6DhcpInfo.bitOpIpv6SecDns = VOS_TRUE;
    }

}


VOS_VOID  AT_PS_SaveIPv6Pcscf(
    VOS_UINT8                           ucCallId,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;
    VOS_UINT8                           ucIndex;

    pstCallEntity = AT_PS_GetCallEntity(pstEvent->stCtrl.usClientId, ucCallId);

    pstCallEntity->stIpv6DhcpInfo.stIpv6PcscfList.ucIpv6PcscfAddrNum = AT_MIN(TAF_PCSCF_ADDR_MAX_NUM,
                                                                              pstEvent->stIpv6PcscfList.ucIpv6PcscfAddrNum);

    for (ucIndex = 0; ucIndex < pstCallEntity->stIpv6DhcpInfo.stIpv6PcscfList.ucIpv6PcscfAddrNum; ucIndex++)
    {
        TAF_MEM_CPY_S(pstCallEntity->stIpv6DhcpInfo.stIpv6PcscfList.astIpv6PcscfAddrList[ucIndex].aucPcscfAddr,
                      TAF_IPV6_ADDR_LEN,
                      pstEvent->stIpv6PcscfList.astIpv6PcscfAddrList[ucIndex].aucPcscfAddr,
                      TAF_IPV6_ADDR_LEN);
    }
}


VOS_VOID AT_PS_ProcConnectedIpv6Addr(
    VOS_UINT8                           ucCallId,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;

    pstCallEntity = AT_PS_GetCallEntity(pstEvent->stCtrl.usClientId, ucCallId);

    pstCallEntity->stIpv6DhcpInfo.ucRabId         = pstEvent->ucRabId;
    pstCallEntity->stIpv6DhcpInfo.ucPduSessionId  = pstEvent->ucPduSessionId;

    /* ����IPv6��IP��ַ����ʽΪ������ */
    /* ��ѡҵ��ʹ����ʱ��ַ */
    if (VOS_FALSE == pstCallEntity->stApnDataSysInfo.ucDataSysInfoFlg)
    {
        TAF_MEM_CPY_S(pstCallEntity->stIpv6DhcpInfo.aucIpv6TmpAddr,
                      (VOS_UINT32)(sizeof(pstCallEntity->stIpv6DhcpInfo.aucIpv6TmpAddr)),
                      pstEvent->stPdpAddr.aucIpv6Addr,
                      TAF_IPV6_ADDR_LEN);
    }

    if (VOS_FALSE == AT_PS_GetPsCallHandOverFlg(pstEvent->stCtrl.usClientId, ucCallId))
    {
        TAF_MEM_CPY_S(pstCallEntity->stIpv6DhcpInfo.aucIpv6Addr,
                      (VOS_UINT32)(sizeof(pstCallEntity->stIpv6DhcpInfo.aucIpv6Addr)),
                      pstEvent->stPdpAddr.aucIpv6Addr,
                      TAF_IPV6_ADDR_LEN);
    }

    /* ����IPV6������DNS��ַ����ʽΪ������ */
    AT_PS_SaveIPv6Dns(ucCallId, pstEvent);

    /* ����IPV6������PCSCF��ַ����ʽΪ������ */
    AT_PS_SaveIPv6Pcscf(ucCallId, pstEvent);

    return;
}


TAF_PS_CALL_END_CAUSE_ENUM_UINT8 AT_PS_ChgIpv6RaFailCauseToCallEndCause(TAF_PS_IPV6_INFO_RESULT_ENUM_UINT8  enIpv6Rst)
{
    TAF_PS_CALL_END_CAUSE_ENUM_UINT8        enCallEndCause;

    enCallEndCause  = TAF_PS_CALL_END_CAUSE_BUTT;

    switch (enIpv6Rst)
    {
        case TAF_PS_IPV6_INFO_RESULT_ALLOC_FAIL:
            enCallEndCause = TAF_PS_CALL_END_CAUSE_IPV6_ADDR_ALLOC_FAIL;
            break;

        case TAF_PS_IPV6_INFO_RESULT_REFRESH_FAIL:
            enCallEndCause = TAF_PS_CALL_END_CAUSE_IPV6_ADDR_TIME_OUT;
            break;

        default:
            enCallEndCause = TAF_PS_CALL_END_CAUSE_NORMAL;
            break;
    }

    return enCallEndCause;
}


VOS_VOID AT_PS_ProcIpv6HoRaFail(
    TAF_PS_IPV6_INFO_IND_STRU          *pstRaInfoNotifyInd,
    VOS_UINT8                           ucCallId
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;

    pstCallEntity   = AT_PS_GetCallEntity(pstRaInfoNotifyInd->stCtrl.usClientId, ucCallId);

    /* ��¼IP��ַ�ı�Ĵ����� */
    AT_PS_SetPsCallErrCause(pstRaInfoNotifyInd->stCtrl.usClientId, TAF_PS_CAUSE_IP_ADDRESS_CHG_IN_HANDOVER);

    AT_PS_SetCallHandOverFailCause(pstCallEntity->stUserInfo.enPortIndex, ucCallId, TAF_PS_CAUSE_IP_ADDRESS_CHG_IN_HANDOVER);

    AT_PS_SetCallHandOverFlg(pstCallEntity->stUserInfo.enPortIndex, ucCallId, VOS_FALSE);

    /* ����handover�������� */
    AT_PS_SetHoCallType(pstCallEntity->stUserInfo.enPortIndex, ucCallId, TAF_PDP_TYPE_BUTT);

    /* ����WLAN����ȥ���� */
    (VOS_VOID)AT_PS_HangupWlanCall(pstCallEntity->stUserInfo.enUserIndex, ucCallId, VOS_TRUE, VOS_TRUE);

    /* IP��ַ�б仯����Ҫִ��CELLULAR��ȥ���� */
    (VOS_VOID)AT_PS_HangupCall(pstCallEntity->stUserInfo.enUserIndex, ucCallId, TAF_PS_CALL_END_CAUSE_NORMAL);

    return;
}


VOS_VOID AT_PS_ProcIpv6RaFail(
    TAF_PS_IPV6_INFO_IND_STRU          *pstRaInfoNotifyInd,
    VOS_UINT8                           ucCallId
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;

    pstCallEntity   = AT_PS_GetCallEntity(pstRaInfoNotifyInd->stCtrl.usClientId, ucCallId);

    if (VOS_FALSE == AT_PS_GetPsCallHandOverFlg(pstRaInfoNotifyInd->stCtrl.usClientId, ucCallId))
    {
        if ( (AT_PS_CALL_INVALID_CID == pstCallEntity->ucIpv4Cid)
          && (AT_PDP_STATE_IDLE == pstCallEntity->enIpv4State))
        {
            /* IPV4��ַ�����ڣ���Ҫȥ����ó��� */
            if (VOS_OK == TAF_PS_CallEndEx(WUEPS_PID_AT,
                                           AT_PS_BuildExClientId(pstRaInfoNotifyInd->stCtrl.usClientId),
                                           0,
                                           pstCallEntity->ucIpv6Cid,
                                           AT_PS_ChgIpv6RaFailCauseToCallEndCause(pstRaInfoNotifyInd->enIpv6Rst)))
            {
                AT_PS_SetCallStateByType(pstRaInfoNotifyInd->stCtrl.usClientId, ucCallId, TAF_PDP_IPV6, AT_PDP_STATE_DEACTING);
            }
            else
            {
                AT_ERR_LOG("AT_PS_ProcIpv6RaFail: Hangup call failed!");
            }
        }
        else
        {
            /* ����IPV4��ַ��ֱ���ϱ�AP IPV6 DEND��Ϣ */
            /* ��ʵIPV6�ĳ��ػ��Ǵ��ڵģ�ֻ��IPV6��ַ�����ã�֮������ȥ����IPV6�ĳ���ʱ���������ϱ�IPV6��DEND��Ϣ */
            AT_PS_SndCallEndedResult(pstRaInfoNotifyInd->stCtrl.usClientId,
                                     ucCallId,
                                     TAF_PDP_IPV6,
                                     TAF_PS_CAUSE_IPV6_ADDRESS_DISABLE,
                                     TAF_PS_APN_DATA_SYS_CELLULAR);
        }
    }
    else
    {
        AT_PS_ProcIpv6HoRaFail(pstRaInfoNotifyInd, ucCallId);
    }

    return;
}


VOS_VOID AT_PS_FillIpv6RaInfo(
    TAF_PS_IPV6_INFO_IND_STRU          *pstRaInfoNotifyInd,
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity
)
{
    AT_IPV6_RA_INFO_STRU               *pstIpv6RaInfo = VOS_NULL_PTR;
    MODEM_ID_ENUM_UINT16                enModemId;
    VOS_UINT64                          ullIID;

    enModemId = MODEM_ID_0;
    if (VOS_OK != AT_GetModemIdFromClient(pstRaInfoNotifyInd->stCtrl.usClientId, &enModemId))
    {
        AT_ERR_LOG("AT_PS_FillIpv6RaInfo: get modem id failed!");
        return;
    }

    pstIpv6RaInfo = &pstCallEntity->stIpv6RaInfo;

    /* ��¼IPv6ǰ׺ */
    /* û�л�ȡ��ǰ׺����ǰ׺�б仯��ʱ���ȥ���»�ȡ�ӿ�ID����������ǰ׺��Ч����������Ч��һ��֮����������Ч����Ҫ��AP�Խ� */
    if ( (VOS_FALSE == pstIpv6RaInfo->bitOpPrefixAddr)
      || (TAF_MEM_CMP(pstIpv6RaInfo->aucPrefixAddr, pstRaInfoNotifyInd->stIpv6RaInfo.astPrefixList[0].aucPrefix, TAF_IPV6_PREFIX_LEN)))
    {
        pstIpv6RaInfo->bitOpPrefixAddr    = VOS_TRUE;
        pstIpv6RaInfo->ulPrefixBitLen     = pstRaInfoNotifyInd->stIpv6RaInfo.astPrefixList[0].ulBitPrefixLen;
        TAF_MEM_CPY_S(pstIpv6RaInfo->aucPrefixAddr,
                      (VOS_UINT32)(sizeof(pstIpv6RaInfo->aucPrefixAddr)),
                      pstRaInfoNotifyInd->stIpv6RaInfo.astPrefixList[0].aucPrefix,
                      TAF_IPV6_ADDR_LEN);

        ullIID = AT_PS_GetIPv6IID(enModemId,
                                  pstCallEntity->stUsrDialParam.aucAPN,
                                  pstIpv6RaInfo->aucPrefixAddr);
        if (0 == ullIID)
        {
            ullIID = AT_PS_GenerateRandomIPv6IID();
        }

        /* ��¼IPv6ȫ�ֵ�ַ */
        pstIpv6RaInfo->bitOpLanAddr = VOS_TRUE;
        TAF_MEM_CPY_S(pstIpv6RaInfo->aucLanAddr,
                      TAF_IPV6_PREFIX_LEN,
                      pstIpv6RaInfo->aucPrefixAddr,
                      TAF_IPV6_PREFIX_LEN);

        TAF_MEM_CPY_S(pstIpv6RaInfo->aucLanAddr + TAF_IPV6_PREFIX_LEN,
                      TAF_IPV6_IID_LEN,
                      &ullIID,
                      (VOS_UINT32)(sizeof(ullIID)));

        /* ����DHCPV6��Ϣ�е�IPv6��ʱ��ַ ��ѡҵ��ʹ����ʱ��ַ*/
        if (VOS_FALSE == pstCallEntity->stApnDataSysInfo.ucDataSysInfoFlg)
        {
            TAF_MEM_CPY_S(pstCallEntity->stIpv6DhcpInfo.aucIpv6TmpAddr,
                          TAF_IPV6_PREFIX_LEN,
                          pstIpv6RaInfo->aucPrefixAddr,
                          TAF_IPV6_PREFIX_LEN);
        }

        /* ����DHCPV6��Ϣ�е�IPv6ȫ�ֵ�ַ */
        TAF_MEM_CPY_S(pstCallEntity->stIpv6DhcpInfo.aucIpv6Addr,
                      (VOS_UINT32)(sizeof(pstCallEntity->stIpv6DhcpInfo.aucIpv6Addr)),
                      pstIpv6RaInfo->aucLanAddr,
                      (VOS_UINT32)(sizeof(pstIpv6RaInfo->aucLanAddr)));
    }

    /* ��¼Preferred Lifetime */
    pstIpv6RaInfo->bitOpPreferredLifetime = VOS_TRUE;
    pstIpv6RaInfo->ulPreferredLifetime    = pstRaInfoNotifyInd->stIpv6RaInfo.astPrefixList[0].ulPreferredLifeTime;

    /* ��¼Valid Lifetime */
    pstIpv6RaInfo->bitOpValidLifetime     = VOS_TRUE;
    pstIpv6RaInfo->ulValidLifetime        = pstRaInfoNotifyInd->stIpv6RaInfo.astPrefixList[0].ulValidLifeTime;

    /* ��¼IPv6 MTU */
    if (VOS_TRUE == pstRaInfoNotifyInd->stIpv6RaInfo.bitOpMtu)
    {
        pstIpv6RaInfo->bitOpMtuSize       = VOS_TRUE;
        pstIpv6RaInfo->ulMtuSize          = pstRaInfoNotifyInd->stIpv6RaInfo.ulMtu;
    }

    return;
}


VOS_VOID AT_PS_ProcHoIpv6RaInfo(
    TAF_PS_IPV6_INFO_IND_STRU          *pstRaInfoNotifyInd,
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity,
    VOS_UINT8                           ucCallId
)
{
    /* IPV6��ַǰ׺����� */
    if ( (0 == pstRaInfoNotifyInd->stIpv6RaInfo.ulPrefixNum)
      || (VOS_FALSE == AT_PS_CmpIpv6PrefixAddr(pstCallEntity->stIpv6DhcpInfo.aucIpv6Addr, pstRaInfoNotifyInd->stIpv6RaInfo.astPrefixList[0].aucPrefix)))
    {
        /* ��¼IP��ַ�ı�Ĵ����� */
        AT_PS_SetPsCallErrCause(pstRaInfoNotifyInd->stCtrl.usClientId, TAF_PS_CAUSE_IP_ADDRESS_CHG_IN_HANDOVER);

        AT_PS_SetCallHandOverFailCause(pstRaInfoNotifyInd->stCtrl.usClientId, ucCallId, TAF_PS_CAUSE_IP_ADDRESS_CHG_IN_HANDOVER);

        AT_PS_SetCallHandOverFlg(pstCallEntity->stUserInfo.enPortIndex, ucCallId, VOS_FALSE);

        /* ����handover�������� */
        AT_PS_SetHoCallType(pstCallEntity->stUserInfo.enPortIndex, ucCallId, TAF_PDP_TYPE_BUTT);

        /* ����WLAN����ȥ���� */
        (VOS_VOID)AT_PS_HangupWlanCall(pstCallEntity->stUserInfo.enUserIndex, ucCallId, VOS_TRUE, VOS_TRUE);

        /* IP��ַ�б仯����Ҫִ��CELLULAR��ȥ���� */
        (VOS_VOID)AT_PS_HangupCall(pstCallEntity->stUserInfo.enUserIndex, ucCallId, TAF_PS_CALL_END_CAUSE_NORMAL);

        return;
    }

    AT_PS_FillIpv6RaInfo(pstRaInfoNotifyInd, pstCallEntity);

    if (VOS_TRUE == AT_PS_IsHoEndCellularConnect(pstCallEntity, TAF_PDP_IPV6))
    {
        AT_PS_ProcHoEndCellularConnect(pstCallEntity, ucCallId);
    }

    return;
}


VOS_VOID AT_PS_ProcIpv6RaInfo(TAF_PS_IPV6_INFO_IND_STRU *pstRaInfoNotifyInd)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;
    AT_IPV6_RA_INFO_STRU               *pstIpv6RaInfo = VOS_NULL_PTR;
    MODEM_ID_ENUM_UINT16                enModemId;
    VOS_UINT8                           ucCallId;

    enModemId = MODEM_ID_0;
    if (VOS_OK != AT_GetModemIdFromClient(pstRaInfoNotifyInd->stCtrl.usClientId, &enModemId))
    {
        AT_ERR_LOG("AT_PS_ProcIpv6RaInfo: get modem id failed!");
        return;
    }

    ucCallId = AT_PS_TransCidToCallId(pstRaInfoNotifyInd->stCtrl.usClientId, pstRaInfoNotifyInd->ucCid);
    if (VOS_FALSE == AT_PS_IsCallIdValid(pstRaInfoNotifyInd->stCtrl.usClientId, ucCallId))
    {
        AT_ERR_LOG("AT_PS_ProcIpv6RaInfo: CallId is invalid!");
        return;
    }

    pstCallEntity = AT_PS_GetCallEntity(pstRaInfoNotifyInd->stCtrl.usClientId, ucCallId);
    pstIpv6RaInfo = &pstCallEntity->stIpv6RaInfo;

    if (TAF_PS_IPV6_INFO_RESULT_SUCCESS != pstRaInfoNotifyInd->enIpv6Rst)
    {
        AT_PS_ProcIpv6RaFail(pstRaInfoNotifyInd, ucCallId);
        TAF_MEM_SET_S(pstIpv6RaInfo, sizeof(AT_IPV6_RA_INFO_STRU), 0x00, sizeof(AT_IPV6_RA_INFO_STRU));
        return;
    }

    if (VOS_TRUE == AT_PS_GetPsCallHandOverFlg(pstRaInfoNotifyInd->stCtrl.usClientId, ucCallId))
    {
        AT_PS_ProcHoIpv6RaInfo(pstRaInfoNotifyInd, pstCallEntity, ucCallId);

        return;
    }

    if (0 == pstRaInfoNotifyInd->stIpv6RaInfo.ulPrefixNum)
    {
        AT_NORM_LOG("AT_PS_ProcIpv6RaInfo: No IPv6 prefix address in RA.");
        return;
    }

    /* ��ȡ��IPv6��ַǰ׺��, �ϱ������ӽ��^DCONN */
    if (VOS_FALSE == pstIpv6RaInfo->bitOpPrefixAddr)
    {
        /* IPv6���ųɹ� */
        AT_PS_SndCallConnectedResult(pstRaInfoNotifyInd->stCtrl.usClientId, ucCallId, TAF_PDP_IPV6);

        if ( (AT_PDP_STATE_IDLE == pstCallEntity->enIpv4State)
          || (AT_PDP_STATE_ACTED == pstCallEntity->enIpv4State))
        {
            /* ע������ϵͳ��ı�֪ͨ */
            AT_PS_RegDataSysChgNtf(pstCallEntity);
        }
    }

    AT_PS_FillIpv6RaInfo(pstRaInfoNotifyInd, pstCallEntity);

    return;
}


VOS_UINT32 AT_PS_MatchIpv4v6ConnFailFallbackCause(
    TAF_PS_CAUSE_ENUM_UINT32            enCause
)
{
    AT_PS_IPV6_BACKPROC_EXT_CAUSE_STRU *pstBackProcExtCause = VOS_NULL_PTR;
    AT_COMM_PS_CTX_STRU                *pstCommPsCtx        = VOS_NULL_PTR;

    VOS_UINT32                          ulRslt;
    VOS_UINT32                          ulCnt;

    pstCommPsCtx    = AT_GetCommPsCtxAddr();
    ulRslt          = VOS_FALSE;

    /* ����Ƿ�ƥ��ܾ�ԭ��ֵΪ#28 */
    if (TAF_PS_CAUSE_SM_NW_UNKNOWN_PDP_ADDR_OR_TYPE == enCause)
    {
        if (VOS_FALSE == pstCommPsCtx->stRedialForNwCauseCfg.ucRedialForNoPdpTypeCausePolicy)
        {
            return VOS_FALSE;
        }

        return VOS_TRUE;
    }

    /* ����Ƿ�ƥ���û�����ԭ��ֵ */
    pstBackProcExtCause = &(AT_GetCommPsCtxAddr()->stIpv6BackProcExtCauseTbl);

    for (ulCnt = 0; ulCnt < pstBackProcExtCause->ulCauseNum; ulCnt++)
    {
        if (pstBackProcExtCause->aenPsCause[ulCnt] == enCause)
        {
            ulRslt = VOS_TRUE;
            break;
        }
    }

    return ulRslt;
}


VOS_UINT32 AT_PS_SetupSingleStackConn(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType
)
{
    AT_DIAL_PARAM_STRU                 *pstUsrDialParam = VOS_NULL_PTR;
    AT_DIAL_PARAM_STRU                  stCallDialParam;
    AT_PDP_STATE_ENUM_U8                enCallState;
    VOS_UINT8                           ucCid;
    VOS_UINT32                          ulRslt;

    TAF_MEM_SET_S(&stCallDialParam, sizeof(stCallDialParam), 0x00, sizeof(AT_DIAL_PARAM_STRU));

    ulRslt          = VOS_ERR;
    ucCid           = 0;
    pstUsrDialParam = &(AT_PS_GetCallEntity(usClientId, ucCallId)->stUsrDialParam);
    enCallState     = AT_PS_GetCallStateByType(usClientId, ucCallId, enPdpType);

    if (AT_PDP_STATE_IDLE == enCallState)
    {
        /* ����һ��δ�����CID���м��� */
        if (VOS_OK == TAF_AGENT_FindCidForDial(usClientId, &ucCid))
        {
            /* ��д���Ų��� */
            AT_PS_GenCallDialParam(&stCallDialParam,
                                   pstUsrDialParam,
                                   ucCid,
                                   enPdpType);

            /* ���𼤻����� */
            if (VOS_OK == AT_PS_SetupCall(usClientId, ucCallId, &stCallDialParam))
            {
                ulRslt = VOS_OK;
            }
        }
    }
    else
    {
        ulRslt = VOS_OK;
    }

    return ulRslt;
}


VOS_UINT32 AT_PS_HangupSingleStackConn(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType
)
{
    VOS_UINT32                          ulRslt;
    AT_PDP_STATE_ENUM_U8                enCallState;
    VOS_UINT8                           ucCid;

    ulRslt      = VOS_ERR;
    ucCid       = AT_PS_GetCidByCallType(usClientId, ucCallId, enPdpType);
    enCallState = AT_PS_GetCallStateByType(usClientId, ucCallId, enPdpType);

    switch (enCallState)
    {
        case AT_PDP_STATE_ACTED:
        case AT_PDP_STATE_ACTING:
            if (VOS_OK == TAF_PS_CallEnd(WUEPS_PID_AT,
                                         AT_PS_BuildExClientId(usClientId),
                                         0,
                                         ucCid))
            {
                ulRslt = VOS_OK;

                AT_PS_SetCallStateByType(usClientId, ucCallId, enPdpType, AT_PDP_STATE_DEACTING);
            }
            else
            {
                AT_ERR_LOG("AT_PS_HangupSingleStackConn: Hangup call failed!");
            }
            break;

        case AT_PDP_STATE_DEACTING:
            ulRslt = VOS_OK;
            break;

        case AT_PDP_STATE_IDLE:
        default:
            break;
    }

    return ulRslt;
}


VOS_VOID AT_PS_ProcDualStackCallConn(
    VOS_UINT8                           ucCallId,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    AT_COMM_PS_CTX_STRU                *pstCommPsCtx = VOS_NULL_PTR;

    pstCommPsCtx = AT_GetCommPsCtxAddr();

    /* ����ԭ��ֵ��#52ԭ��ֵ����Ҫ������һ��PDP���� */
    if ( (VOS_FALSE == pstEvent->bitOpCause)
      || ( (VOS_TRUE == pstEvent->bitOpCause)
        && (TAF_PS_CAUSE_SM_NW_SINGLE_ADDR_BEARERS_ONLY_ALLOWED == pstEvent->enCause) ) )
    {
        switch (pstEvent->stPdpAddr.enPdpType)
        {
            case TAF_PDP_IPV4:
                if ((VOS_FALSE == pstEvent->bitOpCause)
                 && (VOS_FALSE == pstCommPsCtx->stRedialForNwCauseCfg.ucRedialForNoCausePolicy))
                {
                    AT_PS_SndCallEndedResult(pstEvent->stCtrl.usClientId,
                                             ucCallId,
                                             TAF_PDP_IPV6,
                                             TAF_PS_CAUSE_SM_NW_PDP_TYPE_IPV4_ONLY_ALLOWED,
                                             TAF_PS_APN_DATA_SYS_CELLULAR);
                    break;
                }

                if (VOS_OK != AT_PS_SetupSingleStackConn(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV6))
                {
                    /* ��¼���д����� */
                    AT_PS_SetPsCallErrCause(pstEvent->stCtrl.usClientId, TAF_PS_CAUSE_UNKNOWN);

                    /* �ϱ�IPv6����ʧ�� */
                    AT_PS_SndCallEndedResult(pstEvent->stCtrl.usClientId,
                                             ucCallId,
                                             TAF_PDP_IPV6,
                                             AT_PS_GetPsCallErrCause(pstEvent->stCtrl.usClientId),
                                             TAF_PS_APN_DATA_SYS_CELLULAR);
                }
                break;

            case TAF_PDP_IPV6:
                if ((VOS_FALSE == pstEvent->bitOpCause)
                 && (VOS_FALSE == pstCommPsCtx->stRedialForNwCauseCfg.ucRedialForNoCausePolicy))
                {
                    AT_PS_SndCallEndedResult(pstEvent->stCtrl.usClientId,
                                             ucCallId,
                                             TAF_PDP_IPV4,
                                             TAF_PS_CAUSE_SM_NW_PDP_TYPE_IPV6_ONLY_ALLOWED,
                                             TAF_PS_APN_DATA_SYS_CELLULAR);
                    break;
                }

                if (VOS_OK != AT_PS_SetupSingleStackConn(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV4))
                {
                    /* ��¼���д����� */
                    AT_PS_SetPsCallErrCause(pstEvent->stCtrl.usClientId, TAF_PS_CAUSE_UNKNOWN);

                    /* �ϱ�IPv4����ʧ�� */
                    AT_PS_SndCallEndedResult(pstEvent->stCtrl.usClientId,
                                             ucCallId,
                                             TAF_PDP_IPV4,
                                             AT_PS_GetPsCallErrCause(pstEvent->stCtrl.usClientId),
                                             TAF_PS_APN_DATA_SYS_CELLULAR);
                }
                break;

            default:
                AT_WARN_LOG("AT_PS_ProcDualStackCallConn: PDP type is invalid!");
                break;
        }
    }
    else
    {
        /* ����ԭ��ֵ, ֪ͨAPP���Ž�� */
        switch (pstEvent->enCause)
        {
            /* ԭ��ֵ#50 (IPv4 ONLY), ֪ͨAPPģ��IPv6����ʧ�� */
            case TAF_PS_CAUSE_SM_NW_PDP_TYPE_IPV4_ONLY_ALLOWED:
            case TAF_PS_CAUSE_PDP_TYPE_IPV4_ONLY_ALLOWED:
                AT_PS_SndCallEndedResult(pstEvent->stCtrl.usClientId,
                                         ucCallId,
                                         TAF_PDP_IPV6,
                                         pstEvent->enCause,
                                         TAF_PS_APN_DATA_SYS_CELLULAR);

                break;

            /* ԭ��ֵ#51 (IPv6 ONLY), ֪ͨAPPģ��IPv4����ʧ�� */
            case TAF_PS_CAUSE_SM_NW_PDP_TYPE_IPV6_ONLY_ALLOWED:
            case TAF_PS_CAUSE_PDP_TYPE_IPV6_ONLY_ALLOWED:
                AT_PS_SndCallEndedResult(pstEvent->stCtrl.usClientId,
                                         ucCallId,
                                         TAF_PDP_IPV4,
                                         pstEvent->enCause,
                                         TAF_PS_APN_DATA_SYS_CELLULAR);

                break;

            /* ����ԭ��ֵ, ������ */
            default:
                AT_PS_SndCallEndedResult(pstEvent->stCtrl.usClientId,
                                         ucCallId,
                                         (TAF_PDP_IPV4 == pstEvent->stPdpAddr.enPdpType) ? TAF_PDP_IPV6 : TAF_PDP_IPV4,
                                         pstEvent->enCause,
                                         TAF_PS_APN_DATA_SYS_CELLULAR);
                break;
        }
    }

    return;
}


VOS_VOID AT_PS_ProcIpv4ConnSuccFallback(
    VOS_UINT8                           ucCallId,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    AT_PS_PDP_TYPE_CHG_ENTRY_STRU      *pstEntry        = VOS_NULL_PTR;

    switch (AT_PS_GetCurrCallType(pstEvent->stCtrl.usClientId, ucCallId))
    {
        /* �������������PDP���ˣ���ǰ��PDP����ܾ��󣬷ֱ���IPV4��
           IPV6��PDP������У�TAF_PDP_IPV6���쳣������û�����IPV6��PDP
           ���������ظ�IPV4�ļ����ʱҲ��Ҫ���·���IPV6��PDP���� */
        case TAF_PDP_IPV4:
            pstEntry = AT_PS_GetPdpTypeChgMgrNode(pstEvent->stCtrl.usClientId, ucCallId);
            if (VOS_NULL_PTR != pstEntry)
            {
                if (TAF_PDP_IPV4 == pstEntry->enPdpType)
                {
                    /* ����PDP�л���IDLE̬ */
                    AT_PS_SetCallStateByType(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV6, AT_PDP_STATE_IDLE);

                    /* �ϱ�IPv6����ʧ�� */
                    AT_PS_SndCallEndedResult(pstEvent->stCtrl.usClientId,
                                             ucCallId,
                                             TAF_PDP_IPV6,
                                             TAF_PS_CAUSE_SM_NW_PDP_TYPE_IPV4_ONLY_ALLOWED,
                                             TAF_PS_APN_DATA_SYS_CELLULAR);

                    break;
                }
            }

            if (VOS_OK != AT_PS_SetupSingleStackConn(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV6))
            {
                /* ��¼���д����� */
                AT_PS_SetPsCallErrCause(pstEvent->stCtrl.usClientId, TAF_PS_CAUSE_UNKNOWN);

                /* ����PDP�л���IDLE̬ */
                AT_PS_SetCallStateByType(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV6, AT_PDP_STATE_IDLE);

                /* �ϱ�IPv6����ʧ�� */
                AT_PS_SndCallEndedResult(pstEvent->stCtrl.usClientId,
                                         ucCallId,
                                         TAF_PDP_IPV6,
                                         AT_PS_GetPsCallErrCause(pstEvent->stCtrl.usClientId),
                                         TAF_PS_APN_DATA_SYS_CELLULAR);
            }
            break;

        /* ������PDP������˹���ʵ�� */
        case TAF_PDP_IPV4V6:

            /* ����IPv6��Ӧ��CIDΪ��Ч */
            AT_PS_SetCid2CurrCall(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV6, AT_PS_CALL_INVALID_CID, TAF_PS_APN_DATA_SYS_CELLULAR);

            /* ����PDP�л���IDLE̬ */
            AT_PS_SetCallStateByType(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV6, AT_PDP_STATE_IDLE);

            AT_PS_ProcDualStackCallConn(ucCallId, pstEvent);

            break;

        default:
            AT_ERR_LOG("AT_PS_ProcIpv4ConnSuccFallback: PDP type is invalid!");
            break;
    }

    return;
}


VOS_VOID AT_PS_ProcIpv6ConnSuccFallback(
    VOS_UINT8                           ucCallId,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    AT_PS_PDP_TYPE_CHG_ENTRY_STRU      *pstEntry        = VOS_NULL_PTR;

    switch (AT_PS_GetCurrCallType(pstEvent->stCtrl.usClientId, ucCallId))
    {
        case TAF_PDP_IPV6:
            /* �������������PDP���ˣ���ǰ��PDP����ܾ��󣬷ֱ���IPV4��
               IPV6��PDP����, IPV6������ٷ���IPV4��PDP���� */
            pstEntry = AT_PS_GetPdpTypeChgMgrNode(pstEvent->stCtrl.usClientId, ucCallId);
            if (VOS_NULL_PTR != pstEntry)
            {
                if (TAF_PDP_IPV6 == pstEntry->enPdpType)
                {
                    /* ����PDP�л���IDLE̬ */
                    AT_PS_SetCallStateByType(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV4, AT_PDP_STATE_IDLE);

                    /* �ϱ�IPv4����ʧ�� */
                    AT_PS_SndCallEndedResult(pstEvent->stCtrl.usClientId,
                                             ucCallId,
                                             TAF_PDP_IPV4,
                                             TAF_PS_CAUSE_SM_NW_PDP_TYPE_IPV6_ONLY_ALLOWED,
                                             TAF_PS_APN_DATA_SYS_CELLULAR);

                }
            }
            break;

        /* ������PDP������˹���ʵ�� */
        case TAF_PDP_IPV4V6:

            /* ����IPv4��Ӧ��CIDΪ��Ч */
            AT_PS_SetCid2CurrCall(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV4, AT_PS_CALL_INVALID_CID, TAF_PS_APN_DATA_SYS_CELLULAR);

            /* ����PDP�л���IDLE̬ */
            AT_PS_SetCallStateByType(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV4, AT_PDP_STATE_IDLE);

            AT_PS_ProcDualStackCallConn(ucCallId, pstEvent);

            break;

        default:
            AT_ERR_LOG("AT_PS_ProcIpv6ConnSuccFallback: PDP type is invalid!");
            break;
    }

    return;
}


VOS_UINT32 AT_PS_ProcIpv4ConnFailFallback(
    VOS_UINT8                           ucCallId,
    TAF_PS_CALL_PDP_ACTIVATE_REJ_STRU  *pstEvent
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity       = VOS_NULL_PTR;
    AT_DIAL_PARAM_STRU                  stCallDialParam;
    VOS_UINT32                          ulRslt;

    TAF_MEM_SET_S(&stCallDialParam, sizeof(stCallDialParam), 0x00, sizeof(AT_DIAL_PARAM_STRU));

    pstCallEntity = AT_PS_GetCallEntity(pstEvent->stCtrl.usClientId, ucCallId);
    ulRslt        = VOS_OK;

    if (AT_PDP_STATE_IDLE == pstCallEntity->enIpv6State)
    {
        /* ��д���Ų��� */
        AT_PS_GenCallDialParam(&stCallDialParam,
                               &pstCallEntity->stUsrDialParam,
                               pstCallEntity->stUserInfo.ucUsrCid,
                               TAF_PDP_IPV6);

        /* ����IPv4���͵�PDP�������� */
        if (VOS_OK != AT_PS_SetupCall(pstEvent->stCtrl.usClientId, ucCallId, &stCallDialParam))
        {
            ulRslt = VOS_ERR;
        }
    }

    return ulRslt;
}


VOS_UINT32 AT_PS_ProcIpv4v6ConnFailFallback(
    VOS_UINT8                           ucCallId,
    TAF_PS_CALL_PDP_ACTIVATE_REJ_STRU  *pstEvent
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity       = VOS_NULL_PTR;
    AT_DIAL_PARAM_STRU                  stCallDialParam;
    VOS_UINT32                          ulRslt;

    TAF_MEM_SET_S(&stCallDialParam, sizeof(stCallDialParam), 0x00, sizeof(AT_DIAL_PARAM_STRU));

    pstCallEntity = AT_PS_GetCallEntity(pstEvent->stCtrl.usClientId, ucCallId);
    ulRslt        = VOS_ERR;

    /* ��д���Ų��� */
    AT_PS_GenCallDialParam(&stCallDialParam,
                           &pstCallEntity->stUsrDialParam,
                           pstCallEntity->stUserInfo.ucUsrCid,
                           TAF_PDP_IPV4);

    /* ����IPv4���͵�PDP�������� */
    if (VOS_OK == AT_PS_SetupCall(pstEvent->stCtrl.usClientId, ucCallId, &stCallDialParam))
    {
        ulRslt = VOS_OK;
    }

    return ulRslt;
}


VOS_UINT32 AT_PS_CmpIpv4Addr(
    VOS_UINT32                          ulLocalIpv4Addr,
    VOS_UINT32                          ulNewIpv4Addr
)
{
    /* �Ƚ�ipv4��ַ*/
    if (ulLocalIpv4Addr != ulNewIpv4Addr)
    {
        AT_NORM_LOG("AT_PS_CmpIpv4Addr: Ipv4 Addr is diff");
        return VOS_FALSE;
    }

    AT_NORM_LOG("AT_PS_CmpIpv4Addr: Ipv4 Addr is same");
    return VOS_TRUE;
}


VOS_UINT32 AT_PS_CmpIpv4v6AddrHoWlanConn(
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity,
    WLAN_AT_PDN_ACTIVATE_CNF_STRU      *pstWlanPdnActivateCnf
)
{
    VOS_UINT32                          ulIpv4Rslt;
    VOS_UINT32                          ulIpv6Rslt;
    TAF_PDP_TYPE_ENUM_UINT8             enWlanPdpType;

    ulIpv4Rslt      = VOS_TRUE;
    ulIpv6Rslt      = VOS_TRUE;
    enWlanPdpType   = AT_PS_ConvertPdpType2Cellular(pstWlanPdnActivateCnf->enPdnType);

    /* CELLULAR2WLAN��˫ջ��Ϊ��ջ�ĳ���������ֱ����Ϊ�ǵ�ַ��ƥ�䣬�л�ʧ�ܴ��� */
    if (enWlanPdpType != pstCallEntity->enHoPdpType)
    {
        AT_WARN_LOG("AT_PS_CmpIpv4v6AddrHoWlanConn: pdp type is diff");
        return VOS_FALSE;
    }

    /* �Ƚ�ipv4��ַ */
    if ( (TAF_PDP_IPV4 == enWlanPdpType)
      || (TAF_PDP_IPV4V6 == enWlanPdpType) )
    {
        ulIpv4Rslt = AT_PS_CmpIpv4Addr(pstCallEntity->stIpv4DhcpInfo.ulIpv4Addr, AT_GetLanAddr32((VOS_UINT8 *)(pstWlanPdnActivateCnf->stPdpAddr.aucIpV4Addr)));
    }

    /* �Ƚ�ipv6��ַ */
    if ( (TAF_PDP_IPV6 == enWlanPdpType)
      || (TAF_PDP_IPV4V6 == enWlanPdpType) )
    {
        ulIpv6Rslt = AT_PS_CmpIpv6PrefixAddr(pstCallEntity->stIpv6DhcpInfo.aucIpv6Addr,
                                             (VOS_UINT8 *)(pstWlanPdnActivateCnf->stPdpAddr.aucIpV6Addr));
    }

    if ( (VOS_FALSE == ulIpv4Rslt)
      || (VOS_FALSE == ulIpv6Rslt) )
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_VOID AT_PS_ProcIpv4HoCallConnected(
    VOS_UINT8                           ucCallId,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    AT_PS_CALL_ENTITY_STRU                 *pstCallEntity          = VOS_NULL_PTR;

    pstCallEntity = AT_PS_GetCallEntity(pstEvent->stCtrl.usClientId, ucCallId);

    /* IPV4��ַ�ޱ仯 */
    if (VOS_TRUE == AT_PS_CmpIpv4Addr(pstCallEntity->stIpv4DhcpInfo.ulIpv4Addr, AT_GetLanAddr32(pstEvent->stPdpAddr.aucIpv4Addr)))
    {
        /* ����IPv4����PDP״̬�л�������״̬ */
        AT_PS_SetCallStateByType(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV4, AT_PDP_STATE_ACTED);

        /* ����IPv4���͵�DHCP */
        AT_PS_ProcConnInd(ucCallId, pstEvent);

        /* ����������PDP�����¼� */
        AT_PS_ActivateRmNet(ucCallId, pstEvent, TAF_PDP_IPV4);

        /* ��FCע�����ص� */
        AT_PS_RegFCPoint(ucCallId, pstEvent);

        AT_PS_ReportCustomPcoInfo(&pstEvent->stCustomPcoInfo,
                            TAF_PS_PDN_OPERATE_TYPE_ACTIVE,
                            pstCallEntity->stUserInfo.ucUsrCid,
                            pstEvent->stPdpAddr.enPdpType,
                            pstCallEntity->stUserInfo.enPortIndex);

        if (VOS_TRUE == pstEvent->bitOpIpv4Mtu)
        {
            pstCallEntity->usIpv4Mtu = pstEvent->usIpv4Mtu;
        }

        if (VOS_TRUE == AT_PS_IsHoEndCellularConnect(pstCallEntity, TAF_PDP_IPV4))
        {
            AT_PS_ProcHoEndCellularConnect(pstCallEntity, ucCallId);
            return;
        }
        else
        {
            /* handover�������IPV4V6���� */
            AT_PS_ProcIpv4ConnSuccFallback(ucCallId, pstEvent);

            if ( (AT_PDP_STATE_ACTING == pstCallEntity->enIpv6State)
              || ( (AT_PDP_STATE_ACTED == pstCallEntity->enIpv6State)
                && (VOS_FALSE == pstCallEntity->stIpv6RaInfo.bitOpPrefixAddr)))
            {
                /* handover���̼������У��ȴ�RA��Ϣ���ٴ��� */
                AT_NORM_LOG("AT_PS_ProcIpv4HoCallConnected: handover is not end.");
                return;
            }
        }
    }

    /* ��¼IP��ַ�ı�Ĵ����� */
    AT_PS_SetPsCallErrCause(pstEvent->stCtrl.usClientId, TAF_PS_CAUSE_IP_ADDRESS_CHG_IN_HANDOVER);

    AT_PS_SetCallHandOverFailCause(pstEvent->stCtrl.usClientId, ucCallId, TAF_PS_CAUSE_IP_ADDRESS_CHG_IN_HANDOVER);

    AT_PS_SetCallHandOverFlg(pstCallEntity->stUserInfo.enPortIndex, ucCallId, VOS_FALSE);

    /* ����handover�������� */
    AT_PS_SetHoCallType(pstCallEntity->stUserInfo.enPortIndex, ucCallId, TAF_PDP_TYPE_BUTT);

    /* ����WLAN����ȥ���� */
    (VOS_VOID)AT_PS_HangupWlanCall(pstCallEntity->stUserInfo.enUserIndex, ucCallId, VOS_TRUE, VOS_TRUE);

    /* IP��ַ�б仯����Ҫִ��CELLULAR��ȥ���� */
    (VOS_VOID)AT_PS_HangupCall(pstCallEntity->stUserInfo.enUserIndex, ucCallId, TAF_PS_CALL_END_CAUSE_NORMAL);

    return;
}


VOS_VOID AT_PS_ProcIpv4CallConnected(
    VOS_UINT8                           ucCallId,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity;

    pstCallEntity = AT_PS_GetCallEntity(pstEvent->stCtrl.usClientId, ucCallId);

    if (VOS_FALSE == AT_PS_GetPsCallHandOverFlg(pstEvent->stCtrl.usClientId, ucCallId))
    {
        /* ����IPv4����PDP״̬�л�������״̬ */
        AT_PS_SetCallStateByType(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV4, AT_PDP_STATE_ACTED);

        /* ����IPv4���͵�DHCP */
        AT_PS_ProcConnInd(ucCallId, pstEvent);

        /* �ϱ�IPv4����״̬ */
        AT_PS_SndCallConnectedResult(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV4);

        /* ����������PDP�����¼� */
        AT_PS_ActivateRmNet(ucCallId, pstEvent, TAF_PDP_IPV4);

        /* ��FCע�����ص� */
        AT_PS_RegFCPoint(ucCallId, pstEvent);

        /* ��ָ��CID��PDP�ļ���״̬����Ϊ����̬ */
        AT_SetAtChdataCidActStatus(pstEvent->stCtrl.usClientId, pstCallEntity->stUserInfo.ucUsrCid, VOS_TRUE);

        AT_PS_ReportCustomPcoInfo(&pstEvent->stCustomPcoInfo,
                            TAF_PS_PDN_OPERATE_TYPE_ACTIVE,
                            pstCallEntity->stUserInfo.ucUsrCid,
                            pstEvent->stPdpAddr.enPdpType,
                            pstCallEntity->stUserInfo.enPortIndex);


        if (VOS_TRUE == pstEvent->bitOpIpv4Mtu)
        {
            pstCallEntity->usIpv4Mtu = pstEvent->usIpv4Mtu;
        }

        /* ����û��������IPV4V6����Ҫ������һ��PDP */
        if (VOS_TRUE == AT_PS_IsUsrDialTypeDualStack(pstEvent->stCtrl.usClientId, ucCallId))
        {
            AT_PS_ProcIpv4ConnSuccFallback(ucCallId, pstEvent);
        }
        if ( (AT_PDP_STATE_IDLE == pstCallEntity->enIpv6State)
          || ( (AT_PDP_STATE_ACTED == pstCallEntity->enIpv6State)
            && ((VOS_TRUE == pstCallEntity->stIpv6RaInfo.bitOpPrefixAddr))))
        {
            /* ע������ϵͳ��ı�֪ͨ */
            AT_PS_RegDataSysChgNtf(pstCallEntity);
        }
    }
    else
    {
        AT_PS_ProcIpv4HoCallConnected(ucCallId, pstEvent);
    }

    return;
}


VOS_VOID AT_PS_ProcHoCellularCallReject(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId
)
{
    if (VOS_TRUE == AT_PS_GetPsCallHandOverFlg(usClientId, ucCallId))
    {
        AT_PS_SetCallHandOverFlg(usClientId, ucCallId, VOS_FALSE);

        AT_PS_SetCallHandOverFailCause(usClientId, ucCallId, AT_PS_GetPsCallErrCause(usClientId));

        /* ����handover�������� */
        AT_PS_SetHoCallType(usClientId, ucCallId, TAF_PDP_TYPE_BUTT);

        /* ����WLAN����ȥ���� */
        (VOS_VOID)AT_PS_HangupWlanCall(usClientId, ucCallId, VOS_TRUE, VOS_TRUE);

        /* ����CELLULAR��ȥ���� */
        (VOS_VOID)AT_PS_HangupCall(AT_PS_GetUserInfo(usClientId, ucCallId)->enUserIndex, ucCallId, TAF_PS_CALL_END_CAUSE_NORMAL);
    }

    return;
}


VOS_UINT8 AT_PS_ProcHoCallThrotRejectCause(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity   = VOS_NULL_PTR;

    pstCallEntity   = AT_PS_GetCallEntity(usClientId, ucCallId);

    if ( (VOS_TRUE == AT_PS_GetPsCallHandOverFlg(usClientId, ucCallId))
      && (TAF_PDP_TYPE_BUTT != pstCallEntity->enWlanPdpType))
    {
        /* Cellular�±���ԭ��ֵΪ�����㷨�ܾ����򱣳�WLAN��״̬���ȴ�THROTģ��Ķ���֪ͨ���ٷ���WLAN2CELLULAR������ */
        if ( (TAF_PS_CAUSE_SM_THROT_ALG_NOT_ALLOWED == AT_PS_GetPsCallErrCause(usClientId))
          || (TAF_PS_CAUSE_SM_THROT_T3396_IS_RUNNING == AT_PS_GetPsCallErrCause(usClientId))
          || (TAF_PS_CAUSE_SM_THROT_BACKOFF_IS_RUNNING == AT_PS_GetPsCallErrCause(usClientId)))
        {
            AT_NORM_LOG("AT_PS_ProcHoCallThrotRejectCause: handover is end because of throt in cellular!");

            /* ����Handover���� */
            AT_PS_SetCallHandOverFlg(usClientId, ucCallId, VOS_FALSE);

            /* ���õ�ǰ��ΪWLAN */
            AT_PS_SetPsCallCurrentDataSys(usClientId, ucCallId, TAF_PS_APN_DATA_SYS_WLAN);

            /* ����Ӧ���͵�PDP״̬�л���IDLE */
            AT_PS_SetCallStateByType(usClientId, ucCallId, pstCallEntity->enCurrPdpType, AT_PDP_STATE_IDLE);

            /* ��ԭ����CID */
            AT_PS_SetCid2CurrCall(usClientId, ucCallId, pstCallEntity->enHoPdpType, pstCallEntity->stUsrDialParam.ucCid, TAF_PS_APN_DATA_SYS_WLAN);

            /* ����handover�������� */
            AT_PS_SetHoCallType(usClientId, ucCallId, TAF_PDP_TYPE_BUTT);

            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}



VOS_VOID AT_PS_ProcIpv4CallReject(
    VOS_UINT8                           ucCallId,
    TAF_PS_CALL_PDP_ACTIVATE_REJ_STRU  *pstEvent
)
{
    /* �û�����IPv4v6���͵�PDP����, ���ұ�����ܾ�, ԭ��Ϊ28, Э��ջ��Ҫ
       �ֱ���IPv4/IPv6���͵�PDP����, Э��ջ���ȷ���IPv4, �ٷ���IPv6,
       ���IPv4���͵�PDP�����ٴα�����ܾ�, Э��ջ����Ҫ����IPV6���͵�
       PDP����Ϊ�˷�ֹPDP����Ƕ��, ���IPv6���͵�PDP����ʧ��, �����ٳ���
       IPv4���͵�PDP���� */

    AT_PDP_STATE_ENUM_U8                enPreCallState;
    TAF_PDP_TYPE_ENUM_UINT8             enPrePdpTypeChgValue;
    AT_PS_PDP_TYPE_CHG_ENTRY_STRU      *pstEntry        = VOS_NULL_PTR;
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity   = VOS_NULL_PTR;

    enPreCallState = AT_PS_GetCallStateByType(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV4);
    enPrePdpTypeChgValue    = TAF_PDP_TYPE_BUTT;

    if (VOS_TRUE == AT_PS_ProcHoCallThrotRejectCause(pstEvent->stCtrl.usClientId, ucCallId))
    {
        AT_NORM_LOG("AT_PS_ProcIpv4CallReject: handover is end because of throt in cellular!");
        return;
    }

    /* ����IPv4��Ӧ��CIDΪ��Ч */
    AT_PS_SetCid2CurrCall(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV4, AT_PS_CALL_INVALID_CID, TAF_PS_APN_DATA_SYS_CELLULAR);

    /* ��IPv4���͵�PDP״̬�л���IDLE */
    AT_PS_SetCallStateByType(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV4, AT_PDP_STATE_IDLE);

    /* �ϱ�IPv4����ʧ�� */
    AT_PS_SndCallEndedResult(pstEvent->stCtrl.usClientId,
                             ucCallId,
                             TAF_PDP_IPV4,
                             pstEvent->enCause,
                             TAF_PS_APN_DATA_SYS_CELLULAR);

    /* �ͷ�CALLID��CID��ӳ���ϵ */
    AT_PS_FreeCallIdToCid(pstEvent->stCtrl.usClientId, pstEvent->ucCid, ucCallId, TAF_PS_APN_DATA_SYS_CELLULAR);

    if ( (AT_PS_IsUsrDialTypeDualStack(pstEvent->stCtrl.usClientId, ucCallId))
      && (AT_PDP_STATE_ACTING == enPreCallState) )
    {
        pstEntry                = AT_PS_GetPdpTypeChgMgrNode(pstEvent->stCtrl.usClientId, ucCallId);

        if (VOS_NULL_PTR != pstEntry)
        {
            enPrePdpTypeChgValue    = pstEntry->enPdpType;
        }

        pstCallEntity   = AT_PS_GetCallEntity(pstEvent->stCtrl.usClientId, ucCallId);

        AT_PS_ProcPdpTypeChgMgrList(pstEvent->stCtrl.usClientId,
                                    pstCallEntity->stUsrDialParam.ucAPNLen,
                                    pstCallEntity->stUsrDialParam.aucAPN,
                                    pstEvent->enCause);

        if (TAF_PDP_IPV4 == enPrePdpTypeChgValue)
        {
            /* �ϱ�IPv6����ʧ�� */
            AT_PS_SndCallEndedResult(pstEvent->stCtrl.usClientId,
                                     ucCallId,
                                     TAF_PDP_IPV6,
                                     TAF_PS_CAUSE_SM_NW_PDP_TYPE_IPV4_ONLY_ALLOWED,
                                     TAF_PS_APN_DATA_SYS_CELLULAR);
        }
        else
        {
            if (VOS_OK == AT_PS_ProcIpv4ConnFailFallback(ucCallId, pstEvent))
            {
                AT_ERR_LOG("AT_PS_ProcIpv4CallReject: 14004AT_PS_ProcIpv4ConnFailFallback.");
                return;
            }
            else
            {
                /* ��¼���д����� */
                AT_PS_SetPsCallErrCause(pstEvent->stCtrl.usClientId, TAF_PS_CAUSE_UNKNOWN);

                /* �ϱ�IPv6����ʧ�� */
                AT_PS_SndCallEndedResult(pstEvent->stCtrl.usClientId,
                                         ucCallId,
                                         TAF_PDP_IPV6,
                                         AT_PS_GetPsCallErrCause(pstEvent->stCtrl.usClientId),
                                         TAF_PS_APN_DATA_SYS_CELLULAR);
            }
        }
    }

    AT_ERR_LOG("AT_PS_ProcIpv4CallReject: AT_PS_FreeCallEntity.");

    /* �ͷź���ʵ�� */
    AT_PS_FreeCallEntity(pstEvent->stCtrl.usClientId, ucCallId, TAF_PS_APN_DATA_SYS_CELLULAR);

    AT_PS_ProcHoCellularCallReject(pstEvent->stCtrl.usClientId, ucCallId);

    return;
}


VOS_VOID AT_PS_ProcIpv4CallEnded(
    VOS_UINT8                           ucCallId,
    TAF_PS_CALL_PDP_DEACTIVATE_CNF_STRU *pstEvent
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity;

    pstCallEntity = AT_PS_GetCallEntity(pstEvent->stCtrl.usClientId, ucCallId);

    AT_PS_ReportCustomPcoInfo(&pstEvent->stCustomPcoInfo,
                            TAF_PS_PDN_OPERATE_TYPE_DEACTIVE,
                            pstCallEntity->stUserInfo.ucUsrCid,
                            pstEvent->enPdpType,
                            pstCallEntity->stUserInfo.enPortIndex);

    /* ����IPv4��Ӧ��CIDΪ��Ч */
    AT_PS_SetCid2CurrCall(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV4, AT_PS_CALL_INVALID_CID, TAF_PS_APN_DATA_SYS_CELLULAR);

    /* ��IPv4���͵�PDP״̬�л���IDLE */
    AT_PS_SetCallStateByType(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV4, AT_PDP_STATE_IDLE);

    /* �ϱ�IPv4ȥ���� */
    AT_PS_SndCallEndedResult(pstEvent->stCtrl.usClientId,
                             ucCallId,
                             TAF_PDP_IPV4,
                             pstEvent->enCause,
                             TAF_PS_APN_DATA_SYS_CELLULAR);

    /* ����������PDPȥ�����¼� */
    AT_PS_DeactivateRmNet(ucCallId, pstEvent, TAF_PDP_IPV4);

    /* ��FCȥע�����ص� */
    AT_PS_DeRegFCPoint(ucCallId, pstEvent);

    /* ������CID������ͨ����ӳ���ϵ */
    AT_CleanAtChdataCfg(pstEvent->stCtrl.usClientId, pstCallEntity->stUserInfo.ucUsrCid, ucCallId, TAF_PS_APN_DATA_SYS_CELLULAR);

    /* �ͷ�CALLID��CID��ӳ���ϵ */
    AT_PS_FreeCallIdToCid(pstEvent->stCtrl.usClientId, pstEvent->ucCid, ucCallId, TAF_PS_APN_DATA_SYS_CELLULAR);

    /* ���IPv6���͵�PDP�����ڼ���״̬, ��Ҫ����ȥ���� */
    if (AT_PS_IsUsrDialTypeDualStack(pstEvent->stCtrl.usClientId, ucCallId))
    {
        if (VOS_OK == AT_PS_HangupSingleStackConn(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV6))
        {
             AT_ERR_LOG("AT_PS_ProcIpv4CallEnded: 14087AT_PS_HangupSingleStackConn.");
            return;
        }

        if (TAF_PDP_IPV4 == AT_PS_GetCurrCallType(pstEvent->stCtrl.usClientId, ucCallId))
        {
            /* �ϱ�IPv6���ŶϿ� */
            AT_PS_SndCallEndedResult(pstEvent->stCtrl.usClientId,
                                     ucCallId,
                                     TAF_PDP_IPV6,
                                     TAF_PS_CAUSE_SUCCESS,
                                     TAF_PS_APN_DATA_SYS_CELLULAR);
        }
    }

    AT_ERR_LOG("AT_PS_ProcIpv4CallEnded: AT_PS_FreeCallEntity.");

    /* ���PDPʵ�� */
    AT_PS_FreeCallEntity(pstEvent->stCtrl.usClientId, ucCallId, TAF_PS_APN_DATA_SYS_CELLULAR);

    return;
}


VOS_UINT32 AT_PS_CmpIpv6PrefixAddr(
    VOS_UINT8                          *pucLocIpv6PrefixAddr,
    VOS_UINT8                          *pucNewIpv6PrefixAddr
)
{
    /* �Ƚ�ipv6ǰ׺��ַ*/
    if (0 != VOS_StrNiCmp((VOS_CHAR *)pucLocIpv6PrefixAddr,
                          (VOS_CHAR *)pucNewIpv6PrefixAddr,
                          (TAF_IPV6_PREFIX_LEN)))
    {
        AT_NORM_LOG("AT_PS_CmpIpv6PrefixAddr: Ipv6 Prefix Addr is diff");
        return VOS_FALSE;
    }

    AT_NORM_LOG("AT_PS_CmpIpv6PrefixAddr: Ipv6 Prefix Addr is same");
    return VOS_TRUE;
}

VOS_VOID AT_PS_ProcIpv6HoCallConnected(
    VOS_UINT8                           ucCallId,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity;

    pstCallEntity = AT_PS_GetCallEntity(pstEvent->stCtrl.usClientId, ucCallId);

    /* ����IPv6����״̬�л�������״̬ */
    AT_PS_SetCallStateByType(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV6, AT_PDP_STATE_ACTED);

    /* ����IPV6��ַ */
    AT_PS_ProcConnectedIpv6Addr(ucCallId, pstEvent);

    /* ����������PDP�����¼� */
    AT_PS_ActivateRmNet(ucCallId, pstEvent, TAF_PDP_IPV6);

    /* ��FCע�����ص� */
    AT_PS_RegFCPoint(ucCallId, pstEvent);

    AT_PS_ReportCustomPcoInfo(&pstEvent->stCustomPcoInfo,
                        TAF_PS_PDN_OPERATE_TYPE_ACTIVE,
                        pstCallEntity->stUserInfo.ucUsrCid,
                        pstEvent->stPdpAddr.enPdpType,
                        pstCallEntity->stUserInfo.enPortIndex);

    if ( (TAF_PDP_IPV6 == pstCallEntity->enHoPdpType)
      || ( (TAF_PDP_IPV4V6 == pstCallEntity->enHoPdpType)
        && (AT_PDP_STATE_ACTED == pstCallEntity->enIpv4State)))
    {
        /* ��Ҫ�ȴ�IPV6��ַǰ׺���ٴ��� */
        AT_NORM_LOG("AT_PS_ProcIpv6HoCallConnected: handover is not end, wait ipv6 address.");

        return;
    }
    else
    {
        /* handover�������IPV4V6���� */
        AT_PS_ProcIpv6ConnSuccFallback(ucCallId, pstEvent);

        if (AT_PDP_STATE_ACTING == pstCallEntity->enIpv4State)
        {
            /* handover���̼������� */
            AT_NORM_LOG("AT_PS_ProcIpv6HoCallConnected: handover is not end, wait ipv4 result.");

            return;
        }
    }

    /* ��¼IP��ַ�ı�Ĵ����� */
    AT_PS_SetPsCallErrCause(pstEvent->stCtrl.usClientId, TAF_PS_CAUSE_IP_ADDRESS_CHG_IN_HANDOVER);

    AT_PS_SetCallHandOverFailCause(pstEvent->stCtrl.usClientId, ucCallId, TAF_PS_CAUSE_IP_ADDRESS_CHG_IN_HANDOVER);

    AT_PS_SetCallHandOverFlg(pstCallEntity->stUserInfo.enPortIndex, ucCallId, VOS_FALSE);

    /* ����handover�������� */
    AT_PS_SetHoCallType(pstCallEntity->stUserInfo.enPortIndex, ucCallId, TAF_PDP_TYPE_BUTT);

    /* ����WLAN����ȥ���� */
    (VOS_VOID)AT_PS_HangupWlanCall(pstCallEntity->stUserInfo.enUserIndex, ucCallId, VOS_TRUE, VOS_TRUE);

    /* IP��ַ�б仯����Ҫִ��CELLULAR��ȥ���� */
    (VOS_VOID)AT_PS_HangupCall(pstCallEntity->stUserInfo.enUserIndex, ucCallId, TAF_PS_CALL_END_CAUSE_NORMAL);

    return;
}


VOS_VOID AT_PS_ProcIpv6CallConnected(
    VOS_UINT8                           ucCallId,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity;

    pstCallEntity = AT_PS_GetCallEntity(pstEvent->stCtrl.usClientId, ucCallId);

    if (VOS_FALSE == AT_PS_GetPsCallHandOverFlg(pstEvent->stCtrl.usClientId, ucCallId))
    {
        /* ����IPv6����״̬�л�������״̬ */
        AT_PS_SetCallStateByType(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV6, AT_PDP_STATE_ACTED);

        /* ����IPV6��ַ */
        AT_PS_ProcConnectedIpv6Addr(ucCallId, pstEvent);

        /* ����������PDP�����¼� */
        AT_PS_ActivateRmNet(ucCallId, pstEvent, TAF_PDP_IPV6);

        /* ��FCע�����ص� */
        AT_PS_RegFCPoint(ucCallId, pstEvent);

        /* ��ָ��CID��PDP�ļ���״̬����Ϊ����̬ */
        AT_SetAtChdataCidActStatus(pstEvent->stCtrl.usClientId, pstCallEntity->stUserInfo.ucUsrCid, VOS_TRUE);

        AT_PS_ReportCustomPcoInfo(&pstEvent->stCustomPcoInfo,
                            TAF_PS_PDN_OPERATE_TYPE_ACTIVE,
                            pstCallEntity->stUserInfo.ucUsrCid,
                            pstEvent->stPdpAddr.enPdpType,
                            pstCallEntity->stUserInfo.enPortIndex);

        /* ����û��������IPV4V6����Ҫ������һ��PDP */
        if (AT_PS_IsUsrDialTypeDualStack(pstEvent->stCtrl.usClientId, ucCallId))
        {
            AT_PS_ProcIpv6ConnSuccFallback(ucCallId, pstEvent);
        }
    }
    else
    {
        AT_PS_ProcIpv6HoCallConnected(ucCallId, pstEvent);
    }

    return;
}


VOS_VOID AT_PS_ProcIpv4v6HoCallConnected(
    VOS_UINT8                           ucCallId,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity;

    pstCallEntity = AT_PS_GetCallEntity(pstEvent->stCtrl.usClientId, ucCallId);

    /* �Ƚ�IPV4��ַ�ޱ仯 */
    if (VOS_TRUE == AT_PS_CmpIpv4Addr(pstCallEntity->stIpv4DhcpInfo.ulIpv4Addr, AT_GetLanAddr32(pstEvent->stPdpAddr.aucIpv4Addr)))
    {
        /* IPv4v6����״̬�л�������״̬ */
        AT_PS_SetCallStateByType(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV4V6, AT_PDP_STATE_ACTED);

        /* ����IPv4���͵�DHCP */
        AT_PS_ProcConnInd(ucCallId, pstEvent);

        /* ����IPV6��ַ */
        AT_PS_ProcConnectedIpv6Addr(ucCallId, pstEvent);

        /* ����������PDP�����¼� */
        AT_PS_ActivateRmNet(ucCallId, pstEvent, TAF_PDP_IPV4V6);

        /* ��FCע�����ص� */
        AT_PS_RegFCPoint(ucCallId, pstEvent);

        if (VOS_TRUE == pstEvent->bitOpIpv4Mtu)
        {
            pstCallEntity->usIpv4Mtu = pstEvent->usIpv4Mtu;
        }

        AT_PS_ReportCustomPcoInfo(&pstEvent->stCustomPcoInfo,
                            TAF_PS_PDN_OPERATE_TYPE_ACTIVE,
                            pstCallEntity->stUserInfo.ucUsrCid,
                            pstEvent->stPdpAddr.enPdpType,
                            pstCallEntity->stUserInfo.enPortIndex);

        /* ��Ҫ�ȴ�IPV6��ַǰ׺���ٴ��� */
        AT_NORM_LOG("AT_PS_ProcIpv4v6HoCallConnected: handover is not end, wait ipv6 address.");
    }
    else
    {
        /* ��¼IP��ַ�ı�Ĵ����� */
        AT_PS_SetPsCallErrCause(pstEvent->stCtrl.usClientId, TAF_PS_CAUSE_IP_ADDRESS_CHG_IN_HANDOVER);

        AT_PS_SetCallHandOverFailCause(pstEvent->stCtrl.usClientId, ucCallId, TAF_PS_CAUSE_IP_ADDRESS_CHG_IN_HANDOVER);

        AT_PS_SetCallHandOverFlg(pstCallEntity->stUserInfo.enPortIndex, ucCallId, VOS_FALSE);

        /* ����handover�������� */
        AT_PS_SetHoCallType(pstCallEntity->stUserInfo.enPortIndex, ucCallId, TAF_PDP_TYPE_BUTT);

        /* ����WLAN����ȥ���� */
        (VOS_VOID)AT_PS_HangupWlanCall(pstCallEntity->stUserInfo.enUserIndex, ucCallId, VOS_TRUE, VOS_TRUE);

        /* IP��ַ�б仯����Ҫִ��CELLULAR��ȥ���� */
        (VOS_VOID)AT_PS_HangupCall(pstCallEntity->stUserInfo.enUserIndex, ucCallId, TAF_PS_CALL_END_CAUSE_NORMAL);
    }

    return;
}


VOS_VOID AT_PS_ProcIpv4v6CallConnected(
    VOS_UINT8                           ucCallId,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity;

    pstCallEntity = AT_PS_GetCallEntity(pstEvent->stCtrl.usClientId, ucCallId);

    if (VOS_FALSE == AT_PS_GetPsCallHandOverFlg(pstEvent->stCtrl.usClientId, ucCallId))
    {
        /* IPv4v6����״̬�л�������״̬ */
        AT_PS_SetCallStateByType(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV4V6, AT_PDP_STATE_ACTED);

        /* ����IPV4��ַ */
        AT_PS_ProcConnInd(ucCallId, pstEvent);

        /* ����IPV6��ַ */
        AT_PS_ProcConnectedIpv6Addr(ucCallId, pstEvent);

        /* �ϱ�IPv4����ָʾ, IPv6���͵ļ���ָʾ��Ҫ�ڻ�ȡRA��Ϣ���ϱ� */
        AT_PS_SndCallConnectedResult(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV4);

        /* ����������PDP�����¼� */
        AT_PS_ActivateRmNet(ucCallId, pstEvent, TAF_PDP_IPV4V6);

        /* ��FCע�����ص� */
        AT_PS_RegFCPoint(ucCallId, pstEvent);

        /* ��ָ��CID��PDP�ļ���״̬����Ϊ����̬ */
        AT_SetAtChdataCidActStatus(pstEvent->stCtrl.usClientId, pstCallEntity->stUserInfo.ucUsrCid, VOS_TRUE);

        if (VOS_TRUE == pstEvent->bitOpIpv4Mtu)
        {
            pstCallEntity->usIpv4Mtu = pstEvent->usIpv4Mtu;
        }

        AT_PS_ReportCustomPcoInfo(&pstEvent->stCustomPcoInfo,
                            TAF_PS_PDN_OPERATE_TYPE_ACTIVE,
                            pstCallEntity->stUserInfo.ucUsrCid,
                            pstEvent->stPdpAddr.enPdpType,
                            pstCallEntity->stUserInfo.enPortIndex);
    }
    else
    {
        AT_PS_ProcIpv4v6HoCallConnected(ucCallId, pstEvent);
    }

    return;
}


VOS_VOID AT_PS_ProcIpv6CallReject(
    VOS_UINT8                           ucCallId,
    TAF_PS_CALL_PDP_ACTIVATE_REJ_STRU  *pstEvent
)
{
    AT_PS_PDP_TYPE_CHG_ENTRY_STRU      *pstEntry        = VOS_NULL_PTR;
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity   = VOS_NULL_PTR;
    TAF_PDP_TYPE_ENUM_UINT8             enPrePdpTypeChgValue;

    enPrePdpTypeChgValue    = TAF_PDP_TYPE_BUTT;
    pstCallEntity           = AT_PS_GetCallEntity(pstEvent->stCtrl.usClientId, ucCallId);

    if (TAF_PDP_IPV6 == pstCallEntity->enHoPdpType)
    {
        if (VOS_TRUE == AT_PS_ProcHoCallThrotRejectCause(pstEvent->stCtrl.usClientId, ucCallId))
        {
            AT_NORM_LOG("AT_PS_ProcIpv6CallReject: handover is end because of throt in cellular!");
            return;
        }
    }

    if (AT_PS_IsUsrDialTypeDualStack(pstEvent->stCtrl.usClientId, ucCallId))
    {
        pstEntry = AT_PS_GetPdpTypeChgMgrNode(pstEvent->stCtrl.usClientId, ucCallId);

        if (VOS_NULL_PTR != pstEntry)
        {
            enPrePdpTypeChgValue    = pstEntry->enPdpType;
        }


        AT_PS_ProcPdpTypeChgMgrList(pstEvent->stCtrl.usClientId,
                                    pstCallEntity->stUsrDialParam.ucAPNLen,
                                    pstCallEntity->stUsrDialParam.aucAPN,
                                    pstEvent->enCause);

        if (TAF_PDP_IPV6 == enPrePdpTypeChgValue)
        {
            /* �ϱ�IPv4����ʧ�� */
            AT_PS_SndCallEndedResult(pstEvent->stCtrl.usClientId,
                                     ucCallId,
                                     TAF_PDP_IPV4,
                                     TAF_PS_CAUSE_SM_NW_PDP_TYPE_IPV6_ONLY_ALLOWED,
                                     TAF_PS_APN_DATA_SYS_CELLULAR);
        }
    }

    /* ���IPv6����, �Ͳ���Ҫ�ٳ���IPv4, ��Ϊǰ���Ѿ������IPv4���͵�PDP
       ����, ����ٷ���IPv4���͵�PDP����Ļ�, ���ܻᵼ��PDP����Ƕ�� */

    /* ����IPv6��Ӧ��CIDΪ��Ч */
    AT_PS_SetCid2CurrCall(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV6, AT_PS_CALL_INVALID_CID, TAF_PS_APN_DATA_SYS_CELLULAR);

    /* ��IPv6���͵�PDP״̬�л���IDLE */
    AT_PS_SetCallStateByType(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV6, AT_PDP_STATE_IDLE);

    /* �ϱ�IPv6ȥ���� */
    AT_PS_SndCallEndedResult(pstEvent->stCtrl.usClientId,
                             ucCallId,
                             TAF_PDP_IPV6,
                             pstEvent->enCause,
                             TAF_PS_APN_DATA_SYS_CELLULAR);

    /* �ͷ�CALLID��CID��ӳ���ϵ */
    AT_PS_FreeCallIdToCid(pstEvent->stCtrl.usClientId, pstEvent->ucCid, ucCallId, TAF_PS_APN_DATA_SYS_CELLULAR);

    /* ��ͬһ��ʵ���е�IPV4״̬ΪIDLE����Ҫ���ʵ�� */
    if (AT_PDP_STATE_IDLE == AT_PS_GetCallStateByType(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV4))
    {
        AT_ERR_LOG("AT_PS_ProcIpv6CallReject: AT_PS_FreeCallEntity.");

        /* �ͷź���ʵ�� */
        AT_PS_FreeCallEntity(pstEvent->stCtrl.usClientId, ucCallId, TAF_PS_APN_DATA_SYS_CELLULAR);
    }

    AT_PS_ProcHoCellularCallReject(pstEvent->stCtrl.usClientId, ucCallId);

    return;
}


VOS_VOID AT_PS_ProcIpv4v6CallReject(
    VOS_UINT8                           ucCallId,
    TAF_PS_CALL_PDP_ACTIVATE_REJ_STRU  *pstEvent
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity   = VOS_NULL_PTR;

    pstCallEntity   = AT_PS_GetCallEntity(pstEvent->stCtrl.usClientId, ucCallId);

    if (VOS_TRUE == AT_PS_ProcHoCallThrotRejectCause(pstEvent->stCtrl.usClientId, ucCallId))
    {
        AT_NORM_LOG("AT_PS_ProcIpv4v6CallReject: handover is end because of throt in cellular!");
        return;
    }

    /* ����IPv4v6��Ӧ��CIDΪ��Ч */
    AT_PS_SetCid2CurrCall(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV4V6, AT_PS_CALL_INVALID_CID, TAF_PS_APN_DATA_SYS_CELLULAR);

    /* ��IPv4v6���͵�PDP״̬�л���IDLE */
    AT_PS_SetCallStateByType(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV4V6, AT_PDP_STATE_IDLE);

    /* �ͷ�CALLID��CID��ӳ���ϵ */
    AT_PS_FreeCallIdToCid(pstEvent->stCtrl.usClientId, pstEvent->ucCid, ucCallId, TAF_PS_APN_DATA_SYS_CELLULAR);

    AT_PS_ProcPdpTypeChgMgrList(pstEvent->stCtrl.usClientId,
                                pstCallEntity->stUsrDialParam.ucAPNLen,
                                pstCallEntity->stUsrDialParam.aucAPN,
                                pstEvent->enCause);

    /* IPv4v6����ʧ�ܻ��˴��� */
    if (VOS_TRUE == AT_PS_MatchIpv4v6ConnFailFallbackCause(pstEvent->enCause))
    {
        if (VOS_OK == AT_PS_ProcIpv4v6ConnFailFallback(ucCallId, pstEvent))
        {
            return;
        }
        else
        {
            /* ��¼���д����� */
            AT_PS_SetPsCallErrCause(pstEvent->stCtrl.usClientId, TAF_PS_CAUSE_UNKNOWN);
        }
    }

    /* �ϱ�IPv4����ʧ�� */
    AT_PS_SndCallEndedResult(pstEvent->stCtrl.usClientId,
                             ucCallId,
                             TAF_PDP_IPV4,
                             AT_PS_GetPsCallErrCause(pstEvent->stCtrl.usClientId),
                             TAF_PS_APN_DATA_SYS_CELLULAR);
    /* �ϱ�IPv6����ʧ�� */
    AT_PS_SndCallEndedResult(pstEvent->stCtrl.usClientId,
                             ucCallId,
                             TAF_PDP_IPV6,
                             AT_PS_GetPsCallErrCause(pstEvent->stCtrl.usClientId),
                             TAF_PS_APN_DATA_SYS_CELLULAR);

    AT_ERR_LOG("AT_PS_ProcIpv4v6CallReject: AT_PS_FreeCallEntity.");

    /* �ͷź���ʵ�� */
    AT_PS_FreeCallEntity(pstEvent->stCtrl.usClientId, ucCallId, TAF_PS_APN_DATA_SYS_CELLULAR);

    AT_PS_ProcHoCellularCallReject(pstEvent->stCtrl.usClientId, ucCallId);

    return;
}


VOS_VOID AT_PS_ProcIpv6CallEnded(
    VOS_UINT8                           ucCallId,
    TAF_PS_CALL_PDP_DEACTIVATE_CNF_STRU *pstEvent
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity;

    pstCallEntity = AT_PS_GetCallEntity(pstEvent->stCtrl.usClientId, ucCallId);

    AT_PS_ReportCustomPcoInfo(&pstEvent->stCustomPcoInfo,
                            TAF_PS_PDN_OPERATE_TYPE_DEACTIVE,
                            pstCallEntity->stUserInfo.ucUsrCid,
                            pstEvent->enPdpType,
                            pstCallEntity->stUserInfo.enPortIndex);

    /* ����IPv6��Ӧ��CIDΪ��Ч */
    AT_PS_SetCid2CurrCall(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV6, AT_PS_CALL_INVALID_CID, TAF_PS_APN_DATA_SYS_CELLULAR);

    /* ��IPv6���͵�PDP״̬�л���IDLE */
    AT_PS_SetCallStateByType(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV6, AT_PDP_STATE_IDLE);

    /* �ϱ�IPv6ȥ���� */
    AT_PS_SndCallEndedResult(pstEvent->stCtrl.usClientId,
                             ucCallId,
                             TAF_PDP_IPV6,
                             pstEvent->enCause,
                             TAF_PS_APN_DATA_SYS_CELLULAR);

    /* ����������PDPȥ�����¼� */
    AT_PS_DeactivateRmNet(ucCallId, pstEvent, TAF_PDP_IPV6);

    /* ��FCȥע�����ص� */
    AT_PS_DeRegFCPoint(ucCallId, pstEvent);

    /* ������CID������ͨ����ӳ���ϵ */
    AT_CleanAtChdataCfg(pstEvent->stCtrl.usClientId, pstCallEntity->stUserInfo.ucUsrCid, ucCallId, TAF_PS_APN_DATA_SYS_CELLULAR);

    /* �ͷ�CALLID��CID��ӳ���ϵ */
    AT_PS_FreeCallIdToCid(pstEvent->stCtrl.usClientId, pstEvent->ucCid, ucCallId, TAF_PS_APN_DATA_SYS_CELLULAR);

    /* ���IPv4���͵�PDP�����ڼ���״̬, ��Ҫ����ȥ���� */
    if (VOS_OK == AT_PS_HangupSingleStackConn(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV4))
    {
        AT_ERR_LOG("AT_PS_ProcIpv6CallEnded: 14378AT_PS_HangupSingleStackConn.");
        return;
    }

    /* �ͷź���ʵ�� */
    AT_PS_FreeCallEntity(pstEvent->stCtrl.usClientId, ucCallId, TAF_PS_APN_DATA_SYS_CELLULAR);

    AT_ERR_LOG("AT_PS_ProcIpv6CallEnded: AT_PS_FreeCallEntity.");

    return;
}


VOS_VOID AT_PS_ProcIpv4v6CallEnded(
    VOS_UINT8                           ucCallId,
    TAF_PS_CALL_PDP_DEACTIVATE_CNF_STRU *pstEvent
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity;

    pstCallEntity = AT_PS_GetCallEntity(pstEvent->stCtrl.usClientId, ucCallId);

    AT_PS_ReportCustomPcoInfo(&pstEvent->stCustomPcoInfo,
                            TAF_PS_PDN_OPERATE_TYPE_DEACTIVE,
                            pstCallEntity->stUserInfo.ucUsrCid,
                            pstEvent->enPdpType,
                            pstCallEntity->stUserInfo.enPortIndex);

    /* ����IPv4v6��Ӧ��CIDΪ��Ч */
    AT_PS_SetCid2CurrCall(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV4V6, AT_PS_CALL_INVALID_CID, TAF_PS_APN_DATA_SYS_CELLULAR);

    /* ��IPv4v6���͵�PDP״̬�л���IDLE */
    AT_PS_SetCallStateByType(pstEvent->stCtrl.usClientId, ucCallId, TAF_PDP_IPV4V6, AT_PDP_STATE_IDLE);

    /* �ϱ�IPv4��IPv6���ӶϿ�ָʾ */
    AT_PS_SndCallEndedResult(pstEvent->stCtrl.usClientId,
                             ucCallId,
                             TAF_PDP_IPV4V6,
                             pstEvent->enCause,
                             TAF_PS_APN_DATA_SYS_CELLULAR);

    /* ����������PDPȥ�����¼� */
    AT_PS_DeactivateRmNet(ucCallId, pstEvent, TAF_PDP_IPV4V6);

    /* ��FCȥע�����ص� */
    AT_PS_DeRegFCPoint(ucCallId, pstEvent);

    /* ������CID������ͨ����ӳ���ϵ */
    AT_CleanAtChdataCfg(pstEvent->stCtrl.usClientId, pstCallEntity->stUserInfo.ucUsrCid, ucCallId, TAF_PS_APN_DATA_SYS_CELLULAR);

    /* �ͷ�CALLID��CID��ӳ���ϵ */
    AT_PS_FreeCallIdToCid(pstEvent->stCtrl.usClientId, pstEvent->ucCid, ucCallId, TAF_PS_APN_DATA_SYS_CELLULAR);

    AT_ERR_LOG("AT_PS_ProcIpv4v6CallEnded: AT_PS_FreeCallEntity.");

    /* ���PDPʵ�� */
    AT_PS_FreeCallEntity(pstEvent->stCtrl.usClientId, ucCallId, TAF_PS_APN_DATA_SYS_CELLULAR);

    return;
}


VOS_VOID AT_PS_ProcCallConnectedEvent(
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    VOS_UINT8                           ucCallId;

    ucCallId = AT_PS_TransCidToCallId(pstEvent->stCtrl.usClientId, pstEvent->ucCid);

    if (VOS_FALSE == AT_PS_IsCallIdValid(pstEvent->stCtrl.usClientId, ucCallId))
    {
        AT_ERR_LOG("AT_PS_ProcCallConnectedEvent:ERROR: CallId is invalid!");
        return;
    }

    /* ����PDP���ͷֱ���*/
    switch (pstEvent->stPdpAddr.enPdpType)
    {
        case TAF_PDP_IPV4:
            /* ����IPv4��PDP����ɹ��¼� */
            AT_PS_ProcIpv4CallConnected(ucCallId, pstEvent);
            break;

        case TAF_PDP_IPV6:
            /* ����IPv6��PDP����ɹ��¼� */
            AT_PS_ProcIpv6CallConnected(ucCallId, pstEvent);
            break;

        case TAF_PDP_IPV4V6:
            /* ����IPv4v6��PDP����ɹ��¼� */
            AT_PS_ProcIpv4v6CallConnected(ucCallId, pstEvent);
            break;

        default:
            AT_WARN_LOG("AT_PS_ProcCallConnectedEvent:WARNING: PDP type is invaild!");
            break;
    }

    return;
}


VOS_VOID AT_PS_ProcCallRejectEvent(
    TAF_PS_CALL_PDP_ACTIVATE_REJ_STRU   *pstEvent
)
{
    VOS_UINT8                           ucCallId;

    ucCallId = AT_PS_TransCidToCallId(pstEvent->stCtrl.usClientId, pstEvent->ucCid);

    if (VOS_FALSE == AT_PS_IsCallIdValid(pstEvent->stCtrl.usClientId, ucCallId))
    {
        AT_ERR_LOG("AT_PS_ProcCallRejectEvent:ERROR: CallId is invalid!");
        return;
    }

    /* ����PDP���ͷֱ���*/
    switch (AT_PS_GetCurrCallType(pstEvent->stCtrl.usClientId, ucCallId))
    {
        case TAF_PDP_IPV4:
            /* ����IPv4��PDP������¼� */
            AT_PS_ProcIpv4CallReject(ucCallId, pstEvent);
            break;

        case TAF_PDP_IPV6:
            /* ����IPv6��PDP������¼� */
            AT_PS_ProcIpv6CallReject(ucCallId, pstEvent);
            break;

        case TAF_PDP_IPV4V6:
            /* ����IPv4v6��PDP������¼� */
            AT_PS_ProcIpv4v6CallReject(ucCallId, pstEvent);
            break;

        default:
            AT_WARN_LOG("AT_PS_ProcCallRejectEvent:WARNING: PDP type is invaild!");
            break;
    }

    return;
}


VOS_VOID AT_PS_ProcCallEndedEvent(
    TAF_PS_CALL_PDP_DEACTIVATE_CNF_STRU *pstEvent
)
{
    VOS_UINT8                           ucCallId;

    ucCallId = AT_PS_TransCidToCallId(pstEvent->stCtrl.usClientId, pstEvent->ucCid);

    if (VOS_FALSE == AT_PS_IsCallIdValid(pstEvent->stCtrl.usClientId, ucCallId))
    {
        AT_ERR_LOG("AT_PS_ProcCallEndedEvent:ERROR: CallId is invalid!");
        return;
    }

    /* ����PDP���ͷֱ���*/
    switch ( pstEvent->enPdpType )
    {
        case TAF_PDP_IPV4:
            /* ����IPv4��PDPȥ�����¼� */
            AT_PS_ProcIpv4CallEnded(ucCallId, pstEvent);
            break;

        case TAF_PDP_IPV6:
            /* ����IPv6��PDPȥ�����¼� */
            AT_PS_ProcIpv6CallEnded(ucCallId, pstEvent);
            break;

        case TAF_PDP_IPV4V6:
            /* ����IPv4v6��PDPȥ�����¼� */
            AT_PS_ProcIpv4v6CallEnded(ucCallId, pstEvent);
            break;

        default:
            AT_WARN_LOG("AT_PS_ProcCallRejectEvent:WARNING: PDP type is invaild!");
            break;
    }

    return;
}


VOS_VOID AT_PS_ProcCallOrigCnfEvent(TAF_PS_CALL_ORIG_CNF_STRU *pstCallOrigCnf)
{
    /* (1) ��CID����ɺ���ʵ������
     * (2) ������ʵ��������Ч��
     * (3) �����д�����&��ǰ����״̬
     *     ����ɹ�
     *     ->�޴���
     *     ���ʧ��
     *     ->�ϱ�^DEND
     *     ->���CALLID��CID��ӳ��
     *     ->�ͷź���ʵ��
     */
    AT_PDP_STATE_ENUM_U8                enCallState;
    VOS_UINT8                           ucCallId;

    /* ��ȡCID�����ĺ���ʵ������(CallId) */
    ucCallId      = AT_PS_TransCidToCallId(pstCallOrigCnf->stCtrl.usClientId, pstCallOrigCnf->ucCid);

    if (VOS_FALSE == AT_PS_IsCallIdValid(pstCallOrigCnf->stCtrl.usClientId, ucCallId))
    {
        AT_ERR_LOG("AT_PS_ProcCallOrigCnfEvent: CallId is invalid!");
        return;
    }

    enCallState = AT_PS_GetCallStateByCid(pstCallOrigCnf->stCtrl.usClientId, ucCallId, pstCallOrigCnf->ucCid);

    if (TAF_PS_CAUSE_SUCCESS != pstCallOrigCnf->enCause)
    {
        switch (enCallState)
        {
            case AT_PDP_STATE_ACTING:
            case AT_PDP_STATE_DEACTING:

                /* ���ö�Ӧ��CIDΪ��Ч */
                AT_PS_SetCid2CurrCall(pstCallOrigCnf->stCtrl.usClientId,
                                      ucCallId,
                                      AT_PS_GetCurrCallType(pstCallOrigCnf->stCtrl.usClientId, ucCallId),
                                      AT_PS_CALL_INVALID_CID,
                                      TAF_PS_APN_DATA_SYS_CELLULAR);

                /* ���ö�Ӧ��PDP״̬�л���IDLE */
                AT_PS_SetCallStateByType(pstCallOrigCnf->stCtrl.usClientId,
                                         ucCallId,
                                         AT_PS_GetCurrCallType(pstCallOrigCnf->stCtrl.usClientId, ucCallId),
                                         AT_PDP_STATE_IDLE);

                /* �ϱ�����ʧ�� */
                AT_PS_SndCallEndedResult(pstCallOrigCnf->stCtrl.usClientId,
                                         ucCallId,
                                         AT_PS_GetCurrCallType(pstCallOrigCnf->stCtrl.usClientId, ucCallId),
                                         pstCallOrigCnf->enCause,
                                         TAF_PS_APN_DATA_SYS_CELLULAR);

                /* ���CALLID��CID��ӳ�� */
                AT_PS_FreeCallIdToCid(pstCallOrigCnf->stCtrl.usClientId, pstCallOrigCnf->ucCid, ucCallId, TAF_PS_APN_DATA_SYS_CELLULAR);

                break;

            default:
                AT_WARN_LOG1("AT_PS_ProcCallOrigCnfEvent: State is invalid! <state>", enCallState);
                break;
        }

        if (VOS_FALSE == AT_PS_GetPsCallHandOverFlg(pstCallOrigCnf->stCtrl.usClientId, ucCallId))
        {
            if (VOS_TRUE == AT_PS_IsLinkDown(pstCallOrigCnf->stCtrl.usClientId, ucCallId))
            {
                AT_ERR_LOG("AT_PS_ProcCallOrigCnfEvent: AT_PS_FreeCallEntity.");

                /* �ͷź���ʵ�� */
                AT_PS_FreeCallEntity(pstCallOrigCnf->stCtrl.usClientId, ucCallId, TAF_PS_APN_DATA_SYS_CELLULAR);
            }
        }
        else
        {
            /* �л����ܣ���Ҫ��WLAN�ϱ���ȥ������CELLULAR���г��أ���Ҫȥ���� */
            AT_PS_ProcHoCellularCallReject(pstCallOrigCnf->stCtrl.usClientId, ucCallId);

            if (VOS_FALSE == AT_PS_IsLinkDown(pstCallOrigCnf->stCtrl.usClientId, ucCallId))
            {
                (VOS_VOID)AT_PS_HangupCall(AT_PS_GetUserInfo(pstCallOrigCnf->stCtrl.usClientId, ucCallId)->enUserIndex,
                                                               ucCallId,
                                                               TAF_PS_CALL_END_CAUSE_NORMAL);
            }
        }
    }
    return;
}


VOS_VOID AT_PS_ProcCallEndCnfEvent(TAF_PS_CALL_END_CNF_STRU *pstCallEndCnf)
{
    /* (1) ��CID����ɺ���ʵ������
     * (2) ������ʵ��������Ч��
     * (3) �����д�����
     *     ����ɹ�
     *     ->�޴���
     *     ���ʧ��
     *     ->�쳣(��ӡ)
     */

    /* ��ȡCID�����ĺ���ʵ������(CallId)
     * (A) CallId��Ч
     *     -> ����
     * (B) CallId��Ч
     *     -> ����
     */
    VOS_UINT8                           ucCallId;

    ucCallId = AT_PS_TransCidToCallId(pstCallEndCnf->stCtrl.usClientId, pstCallEndCnf->ucCid);

    if (VOS_FALSE == AT_PS_IsCallIdValid(pstCallEndCnf->stCtrl.usClientId, ucCallId))
    {
        AT_ERR_LOG("AT_PS_ProcCallOrigCnfEvent: CallId is invalid!");
        return;
    }

    if (TAF_ERR_NO_ERROR != pstCallEndCnf->enCause)
    {
        AT_ERR_LOG1("AT_PS_ProcCallOrigCnfEvent: End call failed! <cause>", pstCallEndCnf->enCause);
    }

    return;
}


VOS_UINT32 AT_PS_ProcCallModifyEvent(
    VOS_UINT8                           ucIndex,
    TAF_PS_CALL_PDP_MODIFY_CNF_STRU    *pstEvent
)
{
    VOS_UINT8                           ucCallId;
    VOS_UINT8                           ucUserCid;
    AT_MODEM_PS_CTX_STRU               *pstModemPsCtx = VOS_NULL_PTR;
    VOS_UINT32                          ulRmNetId;
    FC_ID_ENUM_UINT8                    enDefaultFcId;
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType;

    ulRmNetId = 0;

    ucCallId = AT_PS_TransCidToCallId(ucIndex, pstEvent->ucCid);
    if (AT_PS_IsCallIdValid(ucIndex, ucCallId))
    {
        /* AT^NDISDUP���ţ��û�����CGCMODE�����෢��MODIFY IND */
        pstModemPsCtx = AT_GetModemPsCtxAddrFromClientId(pstEvent->stCtrl.usClientId);

        /* ��ȡ��Ӧ���û�CID */
        ucUserCid = pstModemPsCtx->astCallEntity[ucCallId].stUserInfo.ucUsrCid;

        /* ��ȡ�û�CID��Ӧ������ID */
        ulRmNetId = pstModemPsCtx->astChannelCfg[ucUserCid].ulRmNetId;

        /* ��ȡ����ID��Ӧ��FC ID */
        enDefaultFcId = AT_PS_GetFcIdFromRnicByRmNetId(ulRmNetId);

        if (enDefaultFcId >= FC_ID_BUTT)
        {
            return VOS_ERR;
        }

        AT_NotifyFcWhenPdpModify(pstEvent, enDefaultFcId);

        enPdpType = AT_PS_GenPdpTypeByCid(pstEvent->stCtrl.usClientId, ucCallId, pstEvent->ucCid);
        AT_PS_ReportCustomPcoInfo(&pstEvent->stCustomPcoInfo,
                            TAF_PS_PDN_OPERATE_TYPE_MODIFY,
                            ucUserCid,
                            enPdpType,
                            pstModemPsCtx->astCallEntity[ucCallId].stUserInfo.enPortIndex);
    }

    return VOS_OK;
}


VOS_VOID AT_PS_RegHsicFCPoint(
    VOS_UINT8                           ucCid,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    AT_FCID_MAP_STRU                    stFCPriOrg;
    FC_REG_POINT_STRU                   stRegFcPoint;
    FC_PRI_ENUM_UINT8                   enFCPri;
    UDI_DEVICE_ID_E                     enDataChannelId;
    FC_ID_ENUM_UINT8                    enFcId;
    VOS_UINT32                          ulRet;
    MODEM_ID_ENUM_UINT16                enModemId;
    AT_MODEM_PS_CTX_STRU               *pstPsModemCtx = VOS_NULL_PTR;

    TAF_MEM_SET_S(&stRegFcPoint, sizeof(stRegFcPoint), 0x00, sizeof(FC_REG_POINT_STRU));

    enModemId = MODEM_ID_0;

    ulRet = AT_GetModemIdFromClient((VOS_UINT8)pstEvent->stCtrl.usClientId, &enModemId);

    if (VOS_OK != ulRet)
    {
         return;
    }

    pstPsModemCtx = AT_GetModemPsCtxAddrFromModemId(enModemId);

    /* Ѱ�����׵�ͨ��ID */
    if ((VOS_TRUE         == pstPsModemCtx->astChannelCfg[ucCid].ulUsed)
     && (AT_PS_INVALID_RMNET_ID != pstPsModemCtx->astChannelCfg[ucCid].ulRmNetId))
    {
        enDataChannelId = (UDI_DEVICE_ID_E)pstPsModemCtx->astChannelCfg[ucCid].ulRmNetId;
    }
    else
    {
        AT_ERR_LOG("AT_PS_RegHsicFCPoint: data channel id is abnormal.\n");
        return;
    }

    enFcId = AT_PS_GetFcIdByUdiDeviceId(enDataChannelId);

    if (enFcId >= FC_ID_BUTT)
    {
        return;
    }

    /* ��ȡ��ǰ����QOS��Ӧ���������ȼ� */
    if (TAF_USED == pstEvent->bitOpUmtsQos)
    {
        enFCPri = AT_GetFCPriFromQos(&pstEvent->stUmtsQos);
    }
    else
    {
        enFCPri = FC_PRI_FOR_PDN_NONGBR;
    }
    /* ������ص��Ƿ��Ѿ�ע�� */
    ulRet = AT_GetFcPriFromMap(enFcId ,&stFCPriOrg);
    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_PS_RegHsicFCPoint: AT_GetFcPriFromMap Err.\n");
    }

    if (VOS_TRUE == stFCPriOrg.ulUsed)
    {
        /* �����ǰFC���ȼ���֮ǰ���ص�FC���ȼ���, ��ô�������ȼ� */
        if (enFCPri > stFCPriOrg.enFcPri)
        {
            AT_ChangeFCPoint(&pstEvent->stCtrl, enFCPri, enFcId);
        }

        /* ����ͨ����RABIDӳ���ϵ */
        FC_ChannelMapCreate(enFcId, pstEvent->ucRabId, enModemId);

        AT_INFO_LOG("AT_PS_RegHsicFCPoint: FC is already registered.");

        return;
    }

    /* ����ͨ����RABIDӳ���ϵ */
    FC_ChannelMapCreate(enFcId, pstEvent->ucRabId, enModemId);

    stRegFcPoint.enFcId             = enFcId;
    stRegFcPoint.enModemId          = enModemId;
    /* ����������������ȼ�RAB QoS���ȼ�������,���ȼ��ı�ʱ����Ҫ�ı����ȼ� */
    /*  FC_PRI_3        ��������ȼ��ĳ���
        FC_PRI_4        ��NONGBR����
        FC_PRI_5        ��GBR���� */
    stRegFcPoint.enFcPri            = enFCPri;
    stRegFcPoint.enPolicyId         = FC_POLICY_ID_MEM;
    stRegFcPoint.pClrFunc           = AT_DisableHsicFlowCtl;
    stRegFcPoint.pSetFunc           = AT_EnableHsicFlowCtl;
    stRegFcPoint.ulParam1           = (VOS_UINT32)DIPC_GetDevHandleByRabId(pstEvent->ucRabId);

    /* ע�����ص�,��Ҫ�ֱ�ע��MEM,CPU,CDS��GPRS�� */
    ulRet = FC_RegPoint(&stRegFcPoint);
    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_PS_RegHsicFCPoint: ERROR: reg mem point Failed.");
        return;
    }

    stRegFcPoint.enPolicyId         = FC_POLICY_ID_CPU_A;
    ulRet = FC_RegPoint(&stRegFcPoint);
    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_PS_RegHsicFCPoint: ERROR: reg a cpu point Failed.");
        return;
    }

    stRegFcPoint.enPolicyId         = FC_POLICY_ID_CDS;
    ulRet = FC_RegPoint(&stRegFcPoint);
    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_PS_RegHsicFCPoint: ERROR: reg cds point Failed.");
        return;
    }

    stRegFcPoint.enPolicyId         = FC_POLICY_ID_GPRS;
    ulRet = FC_RegPoint(&stRegFcPoint);
    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_PS_RegHsicFCPoint: ERROR: reg gprs point Failed.");
        return;
    }

    /* ����FCID��FC Pri��ӳ���ϵ */
    g_stFcIdMaptoFcPri[enFcId].ulUsed  = VOS_TRUE;
    g_stFcIdMaptoFcPri[enFcId].enFcPri = enFCPri;

    /* ��������Ϣ */
    AT_MNTN_TraceRegFcPoint((VOS_UINT8)pstEvent->stCtrl.usClientId, AT_FC_POINT_TYPE_HSIC);

    return;
}


VOS_VOID AT_PS_DeRegHsicFCPoint(
    VOS_UINT8                           ucCid,
    TAF_PS_CALL_PDP_DEACTIVATE_CNF_STRU *pstEvent
)
{
    FC_ID_ENUM_UINT8                    enFcId;
    VOS_UINT32                          ulRet;
    UDI_DEVICE_ID_E                     enDataChannelId;
    MODEM_ID_ENUM_UINT16                enModemId;
    AT_MODEM_PS_CTX_STRU               *pstPsModemCtx = VOS_NULL_PTR;

    enModemId = MODEM_ID_0;

    ulRet = AT_GetModemIdFromClient((AT_CLIENT_TAB_INDEX_UINT8)pstEvent->stCtrl.usClientId, &enModemId);
    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_DeRegHsicFCPoint: Get modem id fail.");
        return;
    }

    pstPsModemCtx = AT_GetModemPsCtxAddrFromModemId(enModemId);

    /* Ѱ�����׵�ͨ��ID */
    if ((VOS_TRUE == pstPsModemCtx->astChannelCfg[ucCid].ulUsed)
      &&(AT_PS_INVALID_RMNET_ID != pstPsModemCtx->astChannelCfg[ucCid].ulRmNetId))
    {
        enDataChannelId = (UDI_DEVICE_ID_E)pstPsModemCtx->astChannelCfg[ucCid].ulRmNetId;
    }
    else
    {
        AT_ERR_LOG("AT_PS_DeRegHsicFCPoint: ERROR: data channel id is abnormal.");
        return;
    }

    enFcId = AT_PS_GetFcIdByUdiDeviceId(enDataChannelId);

    if (enFcId >= FC_ID_BUTT)
    {
         return;
    }

    /* ɾ������ģ��ӳ���ϵ */
    FC_ChannelMapDelete(pstEvent->ucRabId, enModemId);

    ulRet = FC_DeRegPoint(enFcId, enModemId);
    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_PS_DeRegHsicFCPoint: ERROR: de reg point Failed.");
        return;
    }

    /* ���FCID��FC Pri��ӳ���ϵ */
    g_stFcIdMaptoFcPri[enFcId].ulUsed  = VOS_FALSE;
    g_stFcIdMaptoFcPri[enFcId].enFcPri = FC_PRI_BUTT;

    /* ��������Ϣ */
    AT_MNTN_TraceDeregFcPoint((VOS_UINT8)pstEvent->stCtrl.usClientId, AT_FC_POINT_TYPE_HSIC);

    return;
}


VOS_VOID AT_PS_RegAppFCPoint(
    VOS_UINT8                           ucCid,
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    VOS_UINT32                          ulRslt;
    AT_FCID_MAP_STRU                    stFCPriOrg;
    FC_ID_ENUM_UINT8                    enDefaultFcId;
    VOS_UINT32                          ulRmNetId;
    AT_MODEM_PS_CTX_STRU               *pstPsModemCtx = VOS_NULL_PTR;

    pstPsModemCtx = AT_GetModemPsCtxAddrFromClientId(pstEvent->stCtrl.usClientId);

    /* Ѱ�����׵�ͨ��ID */
    if ((VOS_TRUE == pstPsModemCtx->astChannelCfg[ucCid].ulUsed)
     && (pstPsModemCtx->astChannelCfg[ucCid].ulRmNetId < RNIC_DEV_ID_BUTT))
    {
        ulRmNetId = pstPsModemCtx->astChannelCfg[ucCid].ulRmNetId;
    }
    else
    {
        AT_ERR_LOG("AT_PS_RegAppFCPoint: data channel id is abnormal.\n");
        return;
    }

    /* ������֧���ܱ�֤��������Ч�� */
    enDefaultFcId = AT_PS_GetFcIdFromRnicByRmNetId(ulRmNetId);

    ulRslt = AT_GetFcPriFromMap(enDefaultFcId ,&stFCPriOrg);
    if (VOS_OK == ulRslt)
    {
        /* ���FC IDδע�ᣬ��ôע������ص㡣*/
        if (VOS_TRUE != stFCPriOrg.ulUsed)
        {
            /* ע��APP����ʹ�õ����ص� */
            AT_AppRegFCPoint(enDefaultFcId, pstEvent, (VOS_UINT8)ulRmNetId);
        }
        else
        {
            /* APP����ֻʹ����͵�����QOS���ȼ�FC_PRI_FOR_PDN_LOWEST */
            AT_NORM_LOG("AT_PS_RegAppFCPoint: No need to change the default QOS priority.");
        }
    }
    return;
}


VOS_VOID AT_PS_DeRegAppFCPoint(
    VOS_UINT8                           ucCid,
    TAF_PS_CALL_PDP_DEACTIVATE_CNF_STRU *pstEvent
)
{
    FC_ID_ENUM_UINT8                    enDefaultFcId;
    VOS_UINT32                          ulRmNetId;
    AT_MODEM_PS_CTX_STRU               *pstPsModemCtx = VOS_NULL_PTR;

    pstPsModemCtx = AT_GetModemPsCtxAddrFromClientId(pstEvent->stCtrl.usClientId);

    /* Ѱ�����׵�ͨ��ID */
    if ((VOS_TRUE == pstPsModemCtx->astChannelCfg[ucCid].ulUsed)
     && (pstPsModemCtx->astChannelCfg[ucCid].ulRmNetId < RNIC_DEV_ID_BUTT))
    {
        ulRmNetId = (VOS_UINT8)pstPsModemCtx->astChannelCfg[ucCid].ulRmNetId;
    }
    else
    {
        AT_ERR_LOG("AT_PS_DeRegAppFCPoint: data channel id is abnormal.\n");
        return;
    }

    /* ������֧���ܱ�֤��������Ч�� */
    enDefaultFcId = AT_PS_GetFcIdFromRnicByRmNetId(ulRmNetId);

    /* ȥע��APP����ʹ�õ����ص� */
    AT_AppDeRegFCPoint(enDefaultFcId, pstEvent);

    return;
}


VOS_VOID AT_PS_StopAllPsCallTimer(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;
    VOS_UINT32                          ulTmrName;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);

    /* WLAN�¼���PDP��ʱ����Ҫֹͣ */
    AT_SET_WLAN_ACT_PDN_CNF_TMR_NAME(ulTmrName);
    if (VOS_NULL_PTR != pstCallEntity->stPsCallTimerInfo.hWaitWlanActCnfTmrHdl)
    {
        AT_StopRelTimer(ulTmrName, &(pstCallEntity->stPsCallTimerInfo.hWaitWlanActCnfTmrHdl));
    }

    /* WLAN��ȥ����PDP��ʱ����Ҫֹͣ */
    AT_SET_WLAN_DEACT_PDN_CNF_TMR_NAME(ulTmrName);
    if (VOS_NULL_PTR != pstCallEntity->stPsCallTimerInfo.hWaitWlanDeActCnfTmrHdl)
    {
        AT_StopRelTimer(ulTmrName, &(pstCallEntity->stPsCallTimerInfo.hWaitWlanDeActCnfTmrHdl));
    }

    /* ��ƹ�Ҷ�ʱ����Ҫֹͣ */
    AT_SET_PROTECT_PDN_IN_DATA_SYS_TMR_NAME(ulTmrName);
    if (VOS_NULL_PTR != pstCallEntity->stPsCallTimerInfo.hProtectInDataSysTmrHdl)
    {
        AT_StopRelTimer(ulTmrName, &(pstCallEntity->stPsCallTimerInfo.hProtectInDataSysTmrHdl));
    }

    return;
}


VOS_UINT32 AT_PS_AllocCallEntity(
    VOS_UINT16                          usClientId,
    VOS_UINT8                          *pucCallId
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity   = VOS_NULL_PTR;
    AT_MNTN_PS_CALL_ENTITY_STRU         stMntnPsCallEntity;
    VOS_UINT8                           ucCallId;

    for (ucCallId = 0; ucCallId < AT_PS_MAX_CALL_NUM; ucCallId++)
    {
        pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);

        AT_LOG1("AT_PS_AllocCallEntity_normal: usClientId.", (VOS_INT32)usClientId);
        AT_LOG1("AT_PS_AllocCallEntity_normal: ucCallId.", (VOS_INT32)ucCallId);

        if (VOS_FALSE == pstCallEntity->ulUsedFlg)
        {
            break;
        }
    }

    if (ucCallId >= AT_PS_MAX_CALL_NUM)
    {
        for (ucCallId = 0; ucCallId < AT_PS_MAX_CALL_NUM; ucCallId++)
        {
            pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);

            TAF_MEM_SET_S(&stMntnPsCallEntity, sizeof(stMntnPsCallEntity), 0x00, sizeof(AT_MNTN_PS_CALL_ENTITY_STRU));

            /* ��д��Ϣͷ */
            AT_MNTN_CFG_MSG_HDR(&stMntnPsCallEntity, ID_AT_MNTN_PS_CALL_ENTITY_RPT, (sizeof(AT_MNTN_PS_CALL_ENTITY_STRU) - VOS_MSG_HEAD_LENGTH));

            /* ��д��Ϣ���� */
            stMntnPsCallEntity.ulUsedFlg                = pstCallEntity->ulUsedFlg;
            stMntnPsCallEntity.ucCurrPdpType            = pstCallEntity->enCurrPdpType;
            stMntnPsCallEntity.ucPortIndex              = pstCallEntity->stUserInfo.enPortIndex;
            stMntnPsCallEntity.ucUserIndex              = pstCallEntity->stUserInfo.enUserIndex;
            stMntnPsCallEntity.ucUsrType                = pstCallEntity->stUserInfo.ucUsrType;
            stMntnPsCallEntity.ucUsrCid                 = pstCallEntity->stUserInfo.ucUsrCid;
            stMntnPsCallEntity.ucUserPdpType            = pstCallEntity->stUsrDialParam.enPdpType;
            stMntnPsCallEntity.ucPdpTypeValidFlag       = pstCallEntity->stUsrDialParam.ucPdpTypeValidFlag;
            stMntnPsCallEntity.ucAPNLen                 = pstCallEntity->stUsrDialParam.ucAPNLen;
            stMntnPsCallEntity.usUsernameLen            = pstCallEntity->stUsrDialParam.usUsernameLen;
            stMntnPsCallEntity.usPasswordLen            = pstCallEntity->stUsrDialParam.usPasswordLen;
            stMntnPsCallEntity.usAuthType               = pstCallEntity->stUsrDialParam.usAuthType;
            stMntnPsCallEntity.ulIPv4ValidFlag          = pstCallEntity->stUsrDialParam.ulIPv4ValidFlag;
            stMntnPsCallEntity.ucIpv4Cid                = pstCallEntity->ucIpv4Cid;
            stMntnPsCallEntity.ucIpv4State              = pstCallEntity->enIpv4State;
            stMntnPsCallEntity.ucIpv4DendRptFlg         = pstCallEntity->ucIpv4DendRptFlg;
            stMntnPsCallEntity.ucIpv6Cid                = pstCallEntity->ucIpv6Cid;
            stMntnPsCallEntity.ucIpv6State              = pstCallEntity->enIpv6State;
            stMntnPsCallEntity.ucIpv6DendRptFlg         = pstCallEntity->ucIpv6DendRptFlg;
            /* ������Ϣ */
            AT_MNTN_TraceEvent(&stMntnPsCallEntity);
        }


        return VOS_ERR;
    }

    /* ���Ϸǿյ��ж� */
    if (VOS_NULL_PTR != pstCallEntity)
    {
        pstCallEntity->ulUsedFlg = VOS_TRUE;
    }

    *pucCallId = ucCallId;

    return VOS_OK;
}


VOS_VOID AT_PS_FreeCallEntity(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    TAF_PS_APN_DATA_SYS_ENUM_UINT8      enDataSys
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);

    AT_LOG1("AT_PS_FreeCallEntity_normal: usClientId.", (VOS_INT32)usClientId);
    AT_LOG1("AT_PS_FreeCallEntity_normal: ucCallId.", (VOS_INT32)ucCallId);

    if (enDataSys == AT_PS_GetPsCallCurrentDataSys(usClientId, ucCallId))
    {
        /* ȥע������ϵͳ��ı�֪ͨ,һ��Ҫ��������������Ϣ֮ǰ */
        AT_PS_DeRegDataSysChgNtf(usClientId, ucCallId);
    }

    if (VOS_FALSE == AT_PS_IsNeedClearCurrCall(usClientId, ucCallId, enDataSys))
    {
        AT_NORM_LOG("AT_PS_FreeCallEntity: not need FreeCallEntity!");
        return;
    }

    /* PS����ʵ����������ж�ʱ����Ҫֹͣ */
    AT_PS_StopAllPsCallTimer(usClientId, ucCallId);

    /* ���CID��ͨ���Ĺ�ϵ */
    AT_CleanAtChdataCfg(usClientId, pstCallEntity->stUserInfo.ucUsrCid, ucCallId, enDataSys);

    TAF_MEM_SET_S(&pstCallEntity->stUserInfo,
                  (VOS_UINT32)(sizeof(pstCallEntity->stUserInfo)),
                  0x00,
                  (VOS_UINT32)(sizeof(AT_PS_USER_INFO_STRU)));

    pstCallEntity->ulUsedFlg   = VOS_FALSE;
    pstCallEntity->ucIpv4Cid   = AT_PS_CALL_INVALID_CID;
    pstCallEntity->enIpv4State = AT_PDP_STATE_IDLE;
    pstCallEntity->enWlanPdpType    = TAF_PDP_TYPE_BUTT;
    pstCallEntity->enHoPdpType      = TAF_PDP_TYPE_BUTT;
    pstCallEntity->enWlanIpv4State  = AT_PDP_STATE_IDLE;
    pstCallEntity->ucIpv4DendRptFlg = VOS_FALSE;
    pstCallEntity->usIpv4Mtu   = 0;

    TAF_MEM_SET_S(&pstCallEntity->stIpv4DhcpInfo,
                  (VOS_UINT32)(sizeof(pstCallEntity->stIpv4DhcpInfo)),
                  0x00,
                  (VOS_UINT32)(sizeof(AT_IPV4_DHCP_PARAM_STRU)));

    pstCallEntity->ucIpv6Cid   = AT_PS_CALL_INVALID_CID;
    pstCallEntity->enIpv6State = AT_PDP_STATE_IDLE;
    pstCallEntity->enWlanIpv6State = AT_PDP_STATE_IDLE;
    pstCallEntity->ucIpv6DendRptFlg = VOS_FALSE;

    TAF_MEM_SET_S(&pstCallEntity->stIpv6RaInfo,
                  (VOS_UINT32)(sizeof(pstCallEntity->stIpv6RaInfo)),
                  0x00,
                  (VOS_UINT32)(sizeof(AT_IPV6_RA_INFO_STRU)));

    TAF_MEM_SET_S(&pstCallEntity->stIpv6DhcpInfo,
                  (VOS_UINT32)(sizeof(pstCallEntity->stIpv6DhcpInfo)),
                  0x00,
                  (VOS_UINT32)(sizeof(AT_IPV6_DHCP_PARAM_STRU)));

    TAF_MEM_SET_S(&pstCallEntity->stApnDataSysInfo,
                  (VOS_UINT32)(sizeof(pstCallEntity->stApnDataSysInfo)),
                  0x00,
                  (VOS_UINT32)(sizeof(AT_PS_APN_DATA_SYS_INFO_STRU)));

    TAF_MEM_SET_S(&pstCallEntity->stPsCallTimerInfo,
                  (VOS_UINT32)(sizeof(pstCallEntity->stPsCallTimerInfo)),
                  0x00,
                  (VOS_UINT32)(sizeof(AT_PS_CALL_TIMER_INFO_STRU)));

    return;
}


VOS_UINT32 AT_PS_IsWlanLinkGoingUp(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId
)
{
    /* ����ʵ������(CallId)�ɵ����߱�֤����Ч�� */

    /* (1) �������״̬�����ѽ���/���ڽ���: ����TRUE
     * (2) ��������: ����FALSE
     */

    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;
    VOS_UINT32                          ulLinkUpFlg;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);
    ulLinkUpFlg   = VOS_FALSE;

    if ((AT_PDP_STATE_ACTED == pstCallEntity->enWlanIpv4State)
     || (AT_PDP_STATE_ACTING == pstCallEntity->enWlanIpv4State))
    {
        ulLinkUpFlg = VOS_TRUE;
    }

    if ((AT_PDP_STATE_ACTED == pstCallEntity->enWlanIpv6State)
     || (AT_PDP_STATE_ACTING == pstCallEntity->enWlanIpv6State))
    {
        ulLinkUpFlg = VOS_TRUE;
    }

    return ulLinkUpFlg;
}


VOS_UINT32 AT_PS_IsLinkGoingUp(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId
)
{
    /* ����ʵ������(CallId)�ɵ����߱�֤����Ч�� */

    /* (1) �������״̬�����ѽ���/���ڽ���: ����TRUE
     * (2) ��������: ����FALSE
     */

    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;
    VOS_UINT32                          ulLinkUpFlg;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);
    ulLinkUpFlg   = VOS_FALSE;

    if ((AT_PDP_STATE_ACTED == pstCallEntity->enIpv4State)
     || (AT_PDP_STATE_ACTING == pstCallEntity->enIpv4State))
    {
        ulLinkUpFlg = VOS_TRUE;
    }

    if ((AT_PDP_STATE_ACTED == pstCallEntity->enIpv6State)
     || (AT_PDP_STATE_ACTING == pstCallEntity->enIpv6State))
    {
        ulLinkUpFlg = VOS_TRUE;
    }

    return ulLinkUpFlg;
}


VOS_UINT32 AT_PS_IsWlanLinkGoingDown(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId
)
{
    /* ����ʵ������(CallId)�ɵ����߱�֤����Ч�� */

    /* (1) ���Ⲧ�����Ӵ���DEACTING, ����TRUE
     * (2) ��������, ����FALSE
     */

    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;
    VOS_UINT32                          ulLinkDownFlg;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);
    ulLinkDownFlg = VOS_FALSE;

    if (AT_PDP_STATE_DEACTING == pstCallEntity->enWlanIpv4State)
    {
        ulLinkDownFlg = VOS_TRUE;
    }

    if (AT_PDP_STATE_DEACTING == pstCallEntity->enWlanIpv6State)
    {
        ulLinkDownFlg = VOS_TRUE;
    }

    return ulLinkDownFlg;
}


VOS_UINT32 AT_PS_IsLinkGoingDown(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId
)
{
    /* ����ʵ������(CallId)�ɵ����߱�֤����Ч�� */

    /* (1) ���Ⲧ�����Ӵ���DEACTING, ����TRUE
     * (2) ��������, ����FALSE
     */

    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;
    VOS_UINT32                          ulLinkDownFlg;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);
    ulLinkDownFlg = VOS_FALSE;

    if (AT_PDP_STATE_DEACTING == pstCallEntity->enIpv4State)
    {
        ulLinkDownFlg = VOS_TRUE;
    }

    if (AT_PDP_STATE_DEACTING == pstCallEntity->enIpv6State)
    {
        ulLinkDownFlg = VOS_TRUE;
    }

    return ulLinkDownFlg;
}


VOS_UINT32 AT_PS_IsLinkDown(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId
)
{
    /* ����ʵ������(CallId)�ɵ����߱�֤����Ч�� */

    /* (1) IPv4��IPv6���Ӷ�����IDLE, ����TRUE
     * (2) ��������, ����FALSE
     */

    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;
    VOS_UINT32                          ulLinkDownFlg;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);
    ulLinkDownFlg = VOS_TRUE;

    if (AT_PDP_STATE_IDLE != pstCallEntity->enIpv4State)
    {
        ulLinkDownFlg = VOS_FALSE;
    }

    if (AT_PDP_STATE_IDLE != pstCallEntity->enIpv6State)
    {
        ulLinkDownFlg = VOS_FALSE;
    }

    return ulLinkDownFlg;
}


VOS_VOID AT_PS_ReportCurrCallConnState(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId
)
{
    /* ����ʵ������(CallId)�ɵ����߱�֤����Ч�� */

    /* (1) ���ݺ���ʵ���е��û����Ͳ��, ��ȡ���ӽ���״̬�ϱ�����ָ��
     * (2) ��ȡ��������״̬, ��������״̬���ϱ�
     */

    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;
    AT_PS_RPT_CONN_RSLT_FUNC            pRptConnRsltFunc = VOS_NULL_PTR;

    pstCallEntity    = AT_PS_GetCallEntity(usClientId, ucCallId);

    pRptConnRsltFunc = AT_PS_GetRptConnResultFunc(pstCallEntity->stUserInfo.ucUsrType);

    if (VOS_NULL_PTR != pRptConnRsltFunc)
    {
        if ( (AT_PDP_STATE_ACTED == pstCallEntity->enIpv4State)
          || (AT_PDP_STATE_ACTED == pstCallEntity->enWlanIpv4State) )
        {
            pRptConnRsltFunc(pstCallEntity->stUserInfo.ucUsrCid,
                             pstCallEntity->stUserInfo.enPortIndex,
                             TAF_PDP_IPV4);
        }

        if ( ( (AT_PDP_STATE_ACTED == pstCallEntity->enIpv6State)
            && (VOS_TRUE == pstCallEntity->stIpv6RaInfo.bitOpPrefixAddr))
          || (AT_PDP_STATE_ACTED == pstCallEntity->enWlanIpv6State) )
        {
            pRptConnRsltFunc(pstCallEntity->stUserInfo.ucUsrCid,
                             pstCallEntity->stUserInfo.enPortIndex,
                             TAF_PDP_IPV6);
        }
    }
    else
    {
        AT_WARN_LOG("AT_PS_ReportCurrCallConnState: pRptConnRsltFunc is NULL.");
    }

    return;
}


VOS_VOID AT_PS_ReportCurrCallEndState(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId
)
{
    /* ����ʵ������(CallId)�ɵ����߱�֤����Ч�� */

    /* (1) ���ݺ���ʵ���е��û����Ͳ��, ��ȡ���ӽ���״̬�ϱ�����ָ��
     * (2) ��ȡ��������״̬, ���ڶϿ�״̬���ϱ�
     */

    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;
    AT_PS_RPT_END_RSLT_FUNC             pRptEndRsltFunc = VOS_NULL_PTR;

    pstCallEntity   = AT_PS_GetCallEntity(usClientId, ucCallId);
    pRptEndRsltFunc = AT_PS_GetRptEndResultFunc(pstCallEntity->stUserInfo.ucUsrType);

    if (VOS_NULL_PTR != pRptEndRsltFunc)
    {
        if ( (AT_PDP_STATE_IDLE == pstCallEntity->enIpv4State)
          && (AT_PDP_STATE_IDLE == pstCallEntity->enWlanIpv4State))
        {
            pRptEndRsltFunc(pstCallEntity->stUserInfo.ucUsrCid,
                            pstCallEntity->stUserInfo.enPortIndex,
                            TAF_PDP_IPV4,
                            TAF_PS_CAUSE_SUCCESS);
        }

        if (VOS_TRUE == AT_PS_IsIpv6Support())
        {
            if ( (AT_PDP_STATE_IDLE == pstCallEntity->enIpv6State)
              && (AT_PDP_STATE_IDLE == pstCallEntity->enWlanIpv6State))
            {
                pRptEndRsltFunc(pstCallEntity->stUserInfo.ucUsrCid,
                                pstCallEntity->stUserInfo.enPortIndex,
                                TAF_PDP_IPV6,
                                TAF_PS_CAUSE_SUCCESS);
            }
        }
    }
    else
    {
        AT_WARN_LOG("AT_PS_ReportCurrCallEndState: pRptConnRsltFunc is NULL.");
    }

    return;
}


VOS_VOID AT_PS_ReportAllCallEndState(VOS_UINT8 ucIndex)
{
    /* (1) �����û����Ͳ��, ��ȡ���ӶϿ�״̬�ϱ�����ָ��
     * (2) �ϱ��������ӶϿ�״̬
     */

    AT_PS_RPT_END_RSLT_FUNC             pRptEndRsltFunc = VOS_NULL_PTR;
    AT_PS_USER_INFO_STRU                stUsrInfo;

    TAF_MEM_SET_S(&stUsrInfo, sizeof(stUsrInfo), 0x00, sizeof(AT_PS_USER_INFO_STRU));

    AT_PS_ParseUsrInfo(ucIndex, &stUsrInfo);

    pRptEndRsltFunc = AT_PS_GetRptEndResultFunc(stUsrInfo.ucUsrType);

    if (VOS_NULL_PTR != pRptEndRsltFunc)
    {
        pRptEndRsltFunc(stUsrInfo.ucUsrCid,
                        stUsrInfo.enPortIndex,
                        TAF_PDP_IPV4,
                        TAF_PS_CAUSE_SUCCESS);

        if (VOS_TRUE == AT_PS_IsIpv6Support())
        {
            pRptEndRsltFunc(stUsrInfo.ucUsrCid,
                            stUsrInfo.enPortIndex,
                            TAF_PDP_IPV6,
                            TAF_PS_CAUSE_SUCCESS);
        }
    }
    else
    {
        AT_WARN_LOG("AT_PS_ReportAllCallEndState: pRptConnRsltFunc is NULL.");
    }

    return;
}


VOS_UINT32 AT_PS_ValidateDialParam(VOS_UINT8 ucIndex)
{
    AT_PS_DATA_CHANL_CFG_STRU          *pstChanCfg = VOS_NULL_PTR;

    /* ����������� */
    if (AT_CMD_OPT_SET_CMD_NO_PARA == g_stATParseCmd.ucCmdOptType)
    {
        AT_NORM_LOG("AT_PS_ValidateDialParam: No parameter input.");
        return AT_CME_INCORRECT_PARAMETERS;
    }


    /* ���������� */
    if (gucAtParaIndex > 8)
    {
        AT_NORM_LOG1("AT_PS_ValidateDialParam: Parameter number is .\n", gucAtParaIndex);
        return AT_TOO_MANY_PARA;
    }


    /* ��� CID */
    if (0 == gastAtParaList[0].usParaLen)
    {
        AT_NORM_LOG("AT_PS_ValidateDialParam: Missing CID.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ���� CONN: �ò�������ʡ��, 1��ʾ��������, 0��ʾ�Ͽ��Ͽ����� */
    if (0 == gastAtParaList[1].usParaLen)
    {
        AT_NORM_LOG("AT_PS_ValidateDialParam: Missing connect state.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ��� APN */
    if (0 != gastAtParaList[2].usParaLen)
    {
        /* APN���ȼ�� */
        if (gastAtParaList[2].usParaLen > TAF_MAX_APN_LEN)
        {
            AT_NORM_LOG("AT_PS_ValidateDialParam: APN is too long.");
            return AT_CME_INCORRECT_PARAMETERS;
        }

        /* APN��ʽ��� */
        if (VOS_OK != AT_CheckApnFormat(gastAtParaList[2].aucPara,
                                        gastAtParaList[2].usParaLen))
        {
            AT_NORM_LOG("AT_PS_ValidateDialParam: Format of APN is wrong.");
            return AT_CME_INCORRECT_PARAMETERS;
        }
    }

    /* ��� Username */
    if (gastAtParaList[3].usParaLen > TAF_MAX_AUTHDATA_USERNAME_LEN)
    {
        AT_NORM_LOG1("AT_PS_ValidateDialParam: Username length is.\n", gastAtParaList[3].usParaLen);
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ��� Password */
    if (gastAtParaList[4].usParaLen > TAF_MAX_AUTHDATA_PASSWORD_LEN)
    {
        AT_NORM_LOG1("AT_PS_ValidateDialParam: Password length is.\n", gastAtParaList[4].usParaLen);
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (gastAtParaList[7].usParaLen > 0)
    {
        if (VOS_TRUE != AT_PS_CheckDialRatType(ucIndex, (VOS_UINT8)gastAtParaList[7].ulParaValue))
        {
            AT_NORM_LOG1("AT_PS_ValidateDialParam: DialRatType is.\n", gastAtParaList[7].ulParaValue);
            return AT_CME_INCORRECT_PARAMETERS;
        }
    }

    /* ���ͨ��ӳ�� */
    pstChanCfg = AT_PS_GetDataChanlCfg(ucIndex, (VOS_UINT8)gastAtParaList[0].ulParaValue);

    if ( (VOS_FALSE == pstChanCfg->ulUsed)
      || (AT_PS_INVALID_RMNET_ID == pstChanCfg->ulRmNetId) )
    {
        AT_NORM_LOG1("AT_PS_ValidateDialParam: Used is .\n", pstChanCfg->ulUsed);
        AT_NORM_LOG1("AT_PS_ValidateDialParam: RmNetId is .\n", pstChanCfg->ulRmNetId);
        return AT_CME_INCORRECT_PARAMETERS;
    }

    return AT_SUCCESS;
}


VOS_VOID AT_PS_ParseUsrInfo(
    VOS_UINT8                           ucIndex,
    AT_PS_USER_INFO_STRU               *pstUserInfo
)
{
    AT_CLIENT_TAB_INDEX_UINT8           enUserIndex;

    enUserIndex = ucIndex;

    /* PCUI��������PCUI��ģ��NDISDUP���� */
    if (AT_USBCOM_USER == gastAtClientTab[ucIndex].UserType)
    {
        if (VOS_TRUE == AT_GetPcuiPsCallFlag())
        {
            enUserIndex = AT_GetPcuiUsertId();
        }
    }

    /* CTRL��������CTRL��ģ��NDISDUP���� */
    if (AT_CTR_USER == gastAtClientTab[ucIndex].UserType)
    {
        if (VOS_TRUE == AT_GetCtrlPsCallFlag())
        {
            enUserIndex = AT_GetCtrlUserId();
        }
    }

    /* PCUI2��������CTRL��ģ��NDISDUP���� */
    if (AT_PCUI2_USER == gastAtClientTab[ucIndex].UserType)
    {
        if (VOS_TRUE == AT_GetPcui2PsCallFlag())
        {
            enUserIndex = AT_GetPcui2UserId();
        }
    }

    pstUserInfo->enPortIndex = ucIndex;
    pstUserInfo->enUserIndex = enUserIndex;
    pstUserInfo->ucUsrType   = (AT_USER_TYPE)gastAtClientTab[enUserIndex].UserType;
    pstUserInfo->ucUsrCid    = (VOS_UINT8)gastAtParaList[0].ulParaValue;

    return;
}


VOS_UINT32 AT_PS_ParseUsrDialParam(
    VOS_UINT8                           ucIndex,
    AT_DIAL_PARAM_STRU                 *pstUsrDialParam
)
{
    /* �ɵ����߱�֤��κͳ�����Ч�� */

    TAF_PDP_PRIM_CONTEXT_STRU           stPdpCtxInfo;
    VOS_UINT32                          ulRslt;

    TAF_MEM_SET_S(&stPdpCtxInfo, sizeof(stPdpCtxInfo), 0x00, sizeof(TAF_PDP_PRIM_CONTEXT_STRU));

    /* CID */
    pstUsrDialParam->ucCid         = (VOS_UINT8)gastAtParaList[0].ulParaValue;

    /* APN */
    pstUsrDialParam->ucAPNLen      = (VOS_UINT8)gastAtParaList[2].usParaLen;
    TAF_MEM_CPY_S(pstUsrDialParam->aucAPN,
               sizeof(pstUsrDialParam->aucAPN),
               gastAtParaList[2].aucPara,
               gastAtParaList[2].usParaLen);

    /* Username */
    pstUsrDialParam->usUsernameLen = (VOS_UINT16)gastAtParaList[3].usParaLen;
    TAF_MEM_CPY_S(pstUsrDialParam->aucUsername,
               sizeof(pstUsrDialParam->aucUsername),
               gastAtParaList[3].aucPara,
               gastAtParaList[3].usParaLen);

    /* Password */
    pstUsrDialParam->usPasswordLen = (VOS_UINT16)gastAtParaList[4].usParaLen;
    TAF_MEM_CPY_S(pstUsrDialParam->aucPassword,
               sizeof(pstUsrDialParam->aucPassword),
               gastAtParaList[4].aucPara,
               gastAtParaList[4].usParaLen);

    /* AUTH TYPE */
    if (VOS_TRUE == At_CheckCurrRatModeIsCL(ucIndex))
    {
        pstUsrDialParam->usAuthType = AT_ClGetPdpAuthType(gastAtParaList[5].ulParaValue,
                                                               gastAtParaList[5].usParaLen);
    }
    else
    {
        if (gastAtParaList[5].usParaLen > 0)
        {
            pstUsrDialParam->usAuthType = AT_CtrlGetPDPAuthType(gastAtParaList[5].ulParaValue,
                                                                    gastAtParaList[5].usParaLen);
        }
        else
        {
            /* ����û��������볤�Ⱦ���Ϊ0, �Ҽ�Ȩ����δ����, ��Ĭ��ʹ��CHAP���� */
            if ( (0 != gastAtParaList[3].usParaLen)
              && (0 != gastAtParaList[4].usParaLen) )
            {
                pstUsrDialParam->usAuthType = TAF_PDP_AUTH_TYPE_CHAP;
            }
            else
            {
                pstUsrDialParam->usAuthType = TAF_PDP_AUTH_TYPE_NONE;
            }
        }
    }

    /* ADDR: �ݲ����� */

    /* PDN TYPE */
    ulRslt = TAF_AGENT_GetPdpCidPara(&stPdpCtxInfo, ucIndex, pstUsrDialParam->ucCid);

    if ( (VOS_OK == ulRslt)
      && (AT_PS_IS_PDP_TYPE_SUPPORT(stPdpCtxInfo.stPdpAddr.enPdpType)) )
    {
        pstUsrDialParam->enPdpType = stPdpCtxInfo.stPdpAddr.enPdpType;
    }
    else
    {
        pstUsrDialParam->enPdpType = TAF_PDP_IPV4;
    }


    if (gastAtParaList[7].usParaLen > 0)
    {
        pstUsrDialParam->ucBitRatType  = (VOS_UINT8)gastAtParaList[7].ulParaValue;
    }


    if (VOS_OK != AT_CheckIpv6Capability(pstUsrDialParam->enPdpType))
    {
        AT_INFO_LOG("AT_PS_ParseUsrDialParam: PDP type is not supported.");

        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_VOID AT_PS_SetCurrCallType(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);

    pstCallEntity->enCurrPdpType = enPdpType;

    return;
}


TAF_PDP_TYPE_ENUM_UINT8 AT_PS_GetCurrCallType(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);

    return pstCallEntity->enCurrPdpType;
}


VOS_VOID AT_PS_TransferQosPara(
    VOS_UINT8                           ucCid,
    TAF_PS_PDP_QOS_QUERY_PARA_STRU     *pstQosQueryPara,
    TAF_PS_PDP_QOS_SET_PARA_STRU       *pstQosSetPara
)
{
    if (VOS_TRUE == pstQosQueryPara->ucQosFlag)
    {
        pstQosSetPara->stQos.ucDefined = VOS_TRUE;
        pstQosSetPara->stQos.ucCid = ucCid;

        pstQosSetPara->stQos.bitOpTrafficClass = VOS_TRUE;
        pstQosSetPara->stQos.ucTrafficClass    = pstQosQueryPara->stQos.ucTrafficClass;

        pstQosSetPara->stQos.bitOpDeliverOrder = VOS_TRUE;
        pstQosSetPara->stQos.ucDeliverOrder    = pstQosQueryPara->stQos.ucDeliverOrder;

        pstQosSetPara->stQos.bitOpDeliverErrSdu = VOS_TRUE;
        pstQosSetPara->stQos.ucDeliverErrSdu    = pstQosQueryPara->stQos.ucDeliverErrSdu;

        pstQosSetPara->stQos.bitOpMaxSduSize = VOS_TRUE;
        pstQosSetPara->stQos.usMaxSduSize    = pstQosQueryPara->stQos.usMaxSduSize;

        pstQosSetPara->stQos.bitOpMaxBitUl = VOS_TRUE;
        pstQosSetPara->stQos.ulMaxBitUl    = pstQosQueryPara->stQos.ulMaxBitUl;

        pstQosSetPara->stQos.bitOpMaxBitDl = VOS_TRUE;
        pstQosSetPara->stQos.ulMaxBitDl    = pstQosQueryPara->stQos.ulMaxBitDl;

        pstQosSetPara->stQos.bitOpResidualBer = VOS_TRUE;
        pstQosSetPara->stQos.ucResidualBer    = pstQosQueryPara->stQos.ucResidualBer;

        pstQosSetPara->stQos.bitOpSduErrRatio = VOS_TRUE;
        pstQosSetPara->stQos.ucSduErrRatio    = pstQosQueryPara->stQos.ucSduErrRatio;

        pstQosSetPara->stQos.bitOpTransDelay = VOS_TRUE;
        pstQosSetPara->stQos.usTransDelay    = pstQosQueryPara->stQos.usTransDelay;

        pstQosSetPara->stQos.bitOpTraffHandlePrior = VOS_TRUE;
        pstQosSetPara->stQos.ucTraffHandlePrior    = pstQosQueryPara->stQos.ucTraffHandlePrior;

        pstQosSetPara->stQos.bitOpGtdBitUl = VOS_TRUE;
        pstQosSetPara->stQos.ulGtdBitUl    = pstQosQueryPara->stQos.ulGuarantBitUl;

        pstQosSetPara->stQos.bitOpGtdBitDl  = VOS_TRUE;
        pstQosSetPara->stQos.ulGtdBitDl     = pstQosQueryPara->stQos.ulGuarantBitDl;
    }
    else
    {
        pstQosSetPara->stQos.ucDefined = VOS_FALSE;
        pstQosSetPara->stQos.ucCid = ucCid;
    }

    if (VOS_TRUE == pstQosQueryPara->ucMinQosFlag)
    {
        pstQosSetPara->stMinQos.ucDefined = VOS_TRUE;
        pstQosSetPara->stMinQos.ucCid = ucCid;

        pstQosSetPara->stMinQos.bitOpTrafficClass = VOS_TRUE;
        pstQosSetPara->stMinQos.ucTrafficClass    = pstQosQueryPara->stMinQos.ucTrafficClass;

        pstQosSetPara->stMinQos.bitOpDeliverOrder = VOS_TRUE;
        pstQosSetPara->stMinQos.ucDeliverOrder    = pstQosQueryPara->stMinQos.ucDeliverOrder;

        pstQosSetPara->stMinQos.bitOpDeliverErrSdu = VOS_TRUE;
        pstQosSetPara->stMinQos.ucDeliverErrSdu    = pstQosQueryPara->stMinQos.ucDeliverErrSdu;

        pstQosSetPara->stMinQos.bitOpMaxSduSize = VOS_TRUE;
        pstQosSetPara->stMinQos.usMaxSduSize    = pstQosQueryPara->stMinQos.usMaxSduSize;

        pstQosSetPara->stMinQos.bitOpMaxBitUl = VOS_TRUE;
        pstQosSetPara->stMinQos.ulMaxBitUl    = pstQosQueryPara->stMinQos.ulMaxBitUl;

        pstQosSetPara->stMinQos.bitOpMaxBitDl = VOS_TRUE;
        pstQosSetPara->stMinQos.ulMaxBitDl    = pstQosQueryPara->stMinQos.ulMaxBitDl;

        pstQosSetPara->stMinQos.bitOpResidualBer = VOS_TRUE;
        pstQosSetPara->stMinQos.ucResidualBer    = pstQosQueryPara->stMinQos.ucResidualBer;

        pstQosSetPara->stMinQos.bitOpSduErrRatio = VOS_TRUE;
        pstQosSetPara->stMinQos.ucSduErrRatio    = pstQosQueryPara->stMinQos.ucSduErrRatio;

        pstQosSetPara->stMinQos.bitOpTransDelay = VOS_TRUE;
        pstQosSetPara->stMinQos.usTransDelay    = pstQosQueryPara->stMinQos.usTransDelay;

        pstQosSetPara->stMinQos.bitOpTraffHandlePrior = VOS_TRUE;
        pstQosSetPara->stMinQos.ucTraffHandlePrior    = pstQosQueryPara->stMinQos.ucTraffHandlePrior;

        pstQosSetPara->stMinQos.bitOpGtdBitUl = VOS_TRUE;
        pstQosSetPara->stMinQos.ulGtdBitUl    = pstQosQueryPara->stMinQos.ulGuarantBitUl;

        pstQosSetPara->stMinQos.bitOpGtdBitDl  = VOS_TRUE;
        pstQosSetPara->stMinQos.ulGtdBitDl     = pstQosQueryPara->stMinQos.ulGuarantBitDl;

    }
    else
    {
        pstQosSetPara->stMinQos.ucDefined = VOS_FALSE;
        pstQosSetPara->stMinQos.ucCid = ucCid;
    }

    return;
}


VOS_UINT32 AT_PS_SetQosPara(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucUserCid,
    VOS_UINT8                           ucActCid
)
{
    TAF_PS_PDP_QOS_QUERY_PARA_STRU      stQosQueryPara;
    TAF_PS_PDP_QOS_SET_PARA_STRU        stQosSetPara;

    TAF_MEM_SET_S(&stQosQueryPara, sizeof(stQosQueryPara), 0x00, sizeof(TAF_PS_PDP_QOS_QUERY_PARA_STRU));
    TAF_MEM_SET_S(&stQosSetPara, sizeof(stQosSetPara), 0x00, sizeof(TAF_PS_PDP_QOS_SET_PARA_STRU));

    /* �ж���Ҫ���õ�CID�Ƿ��û��·���CID��ͬ */
    if (ucActCid == ucUserCid)
    {
        return VOS_OK;
    }

    /* ��ȡ�û����õ�QOS���� */
    if (VOS_OK != TAF_AGENT_GetPdpCidQosPara(usClientId,
                                             ucUserCid,
                                             &stQosQueryPara))
    {
        AT_ERR_LOG("AT_PS_SetQosPara: ERROR: Get QOS Fail.");
        return VOS_ERR;
    }

    /* QOS����ת�� */
    AT_PS_TransferQosPara(ucActCid, &stQosQueryPara, &stQosSetPara);

    /* ��ȡ�û����õ�QOS���� */
    if (VOS_OK != TAF_AGENT_SetPdpCidQosPara(usClientId,
                                             &stQosSetPara))
    {
        AT_ERR_LOG("AT_PS_SetQosPara: ERROR: Set QOS Fail.");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_PS_SetupCall(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    AT_DIAL_PARAM_STRU                 *pstCallDialParam
)
{
    /* ����ʵ������(CallId)�ɵ����߱�֤����Ч�� */

    /* (1) ����CID�����Ĳ���
     *     -> ʧ��: ����ERROR
     *     -> �ɹ�: ����
     * (2) ����PS�����
     *     -> ʧ��: ����ERROR
     *     -> �ɹ�: ����
     * (3) ���CallIdӳ��
     * (4) ���ú���PDP����
     * (5) ���ú���CID
     * (6) ���ú���״̬
     */
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;
    AT_PS_DATA_CHANL_CFG_STRU          *pstChanCfg    = VOS_NULL_PTR;
    TAF_PS_DIAL_PARA_STRU               stPsDialParamInfo;
    VOS_UINT32                          ulRslt;

    TAF_MEM_SET_S(&stPsDialParamInfo, sizeof(stPsDialParamInfo),
                  0x00, sizeof(TAF_PS_DIAL_PARA_STRU));

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);
    pstChanCfg    = AT_PS_GetDataChanlCfg(usClientId,
                                          AT_PS_GetUserInfo(usClientId, ucCallId)->ucUsrCid);

    /* ����QOS���� */
    if (VOS_OK != AT_PS_SetQosPara(usClientId, AT_PS_GetUserInfo(usClientId, ucCallId)->ucUsrCid, pstCallDialParam->ucCid))
    {
        return VOS_ERR;
    }

    /* ��дPS���в��� */
    AT_GetPsDialParamFromAtDialParam(&stPsDialParamInfo, pstCallDialParam);

    stPsDialParamInfo.ucRmnetId = (VOS_UINT8)pstChanCfg->ulIfaceld;

    if (VOS_TRUE == AT_PS_GetPsCallHandOverFlg(usClientId, ucCallId))
    {
        stPsDialParamInfo.bitOpReqType      = VOS_TRUE;
        stPsDialParamInfo.enPdpRequestType  = TAF_PDP_REQUEST_TYPE_HANDOVER;

        if ( (TAF_PDP_IPV6 == stPsDialParamInfo.enPdpType)
          || (TAF_PDP_IPV4V6 == stPsDialParamInfo.enPdpType))
        {
            TAF_MEM_CPY_S(stPsDialParamInfo.stPdpAddr.aucIpv6Addr,
                          TAF_IPV6_ADDR_LEN,
                          pstCallEntity->stIpv6DhcpInfo.aucIpv6Addr,
                          TAF_IPV6_ADDR_LEN);
        }
    }

    /* ����PS����� */
    ulRslt = TAF_PS_CallOrig(WUEPS_PID_AT,
                             AT_PS_BuildExClientId(usClientId),
                             0, &stPsDialParamInfo);

    if (VOS_OK != ulRslt)
    {
        AT_ERR_LOG("AT_PS_SetupCall: Setup call failed.");
        return VOS_ERR;
    }

    /* ���CALLIDӳ�� */
    AT_PS_AssignCallIdToCid(usClientId, stPsDialParamInfo.ucCid, ucCallId);

    /* ���ú���PDP���� */
    AT_PS_SetCurrCallType(usClientId, ucCallId, stPsDialParamInfo.enPdpType);

    /* ���ú���CID */
    AT_PS_SetCid2CurrCall(usClientId, ucCallId, stPsDialParamInfo.enPdpType, stPsDialParamInfo.ucCid, TAF_PS_APN_DATA_SYS_CELLULAR);

    /* ���ú���״̬ */
    AT_PS_SetCallStateByType(usClientId, ucCallId, stPsDialParamInfo.enPdpType, AT_PDP_STATE_ACTING);

    /* ���õ�ǰ��ΪCELLULAR */
    AT_PS_SetPsCallCurrentDataSys(usClientId, ucCallId, TAF_PS_APN_DATA_SYS_CELLULAR);

    return VOS_OK;
}


VOS_UINT32 AT_PS_SetupWlanCall(
    AT_DIAL_PARAM_STRU                 *pstCallDialParam,
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;
    VOS_UINT32                          ulRslt;
    VOS_UINT32                          ulTmrName;
    VOS_UINT32                          ulTmrParam;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);

    /* Wlan�·���PS����� */
    ulRslt = AT_PS_SndWlanMsgPdnActivateReq((VOS_UINT8)usClientId,
                                            pstCallDialParam->enPdpType,
                                            (VOS_INT8)AT_PS_GetPsCallHandOverFlg(usClientId, ucCallId),
                                            ucCallId);

    if (VOS_OK != ulRslt)
    {
        AT_ERR_LOG("AT_PS_SetupWlanCall: Setup call in wlan failed.");
        return VOS_ERR;
    }

    AT_SET_WLAN_ACT_PDN_CNF_TMR_NAME(ulTmrName);
    AT_SET_WLAN_ACT_PDN_CNF_TMR_PARAM(ulTmrParam, usClientId, ucCallId);

    /* ������ʱ�� */
    AT_StartRelTimer(&(pstCallEntity->stPsCallTimerInfo.hWaitWlanActCnfTmrHdl),
                     AT_WAIT_WLAN_ACT_PDN_CNF_TIMER_LEN,
                     ulTmrName,
                     ulTmrParam,
                     VOS_RELTIMER_NOLOOP);

    if (VOS_FALSE == AT_PS_GetPsCallHandOverFlg(usClientId, ucCallId))
    {
        /* ���CALLIDӳ�� */
        AT_PS_AssignCallIdToCid(usClientId, pstCallDialParam->ucCid, ucCallId);

        /* ���ú���CID */
        AT_PS_SetCid2CurrCall(usClientId, ucCallId, pstCallDialParam->enPdpType, pstCallDialParam->ucCid, TAF_PS_APN_DATA_SYS_WLAN);
    }

    /* ����Wlan����PDP���� */
    AT_PS_SetWlanCurrCallType(usClientId, ucCallId, pstCallDialParam->enPdpType);

    /* ����Wlan����״̬ */
    AT_PS_SetWlanCallStateByType(usClientId, ucCallId, pstCallDialParam->enPdpType, AT_PDP_STATE_ACTING);

    /* ���õ�ǰ��ΪWLAN */
    AT_PS_SetPsCallCurrentDataSys(usClientId, ucCallId, TAF_PS_APN_DATA_SYS_WLAN);

    return VOS_OK;
}


VOS_UINT32 AT_PS_HangupWlanCall(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    VOS_INT8                            cIsLocal,
    VOS_INT8                            cIsHandover
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;
    VOS_UINT32                          ulTmrName;
    VOS_UINT32                          ulTmrParam;

    pstCallEntity   = AT_PS_GetCallEntity(usClientId, ucCallId);

    if ( (AT_PDP_STATE_ACTED == pstCallEntity->enWlanIpv4State)
      || (AT_PDP_STATE_ACTING == pstCallEntity->enWlanIpv4State)
      || (AT_PDP_STATE_ACTED == pstCallEntity->enWlanIpv6State)
      || (AT_PDP_STATE_ACTING == pstCallEntity->enWlanIpv6State))
    {
        if (VOS_OK == AT_PS_SndWlanMsgPdnDeActivateReq((VOS_UINT8)usClientId, cIsLocal, cIsHandover, ucCallId))
        {
            AT_PS_SetWlanCallStateByType(usClientId, ucCallId, pstCallEntity->enWlanPdpType, AT_PDP_STATE_DEACTING);

            /* ֹͣ����CNF��ʱ�� */
            AT_SET_WLAN_ACT_PDN_CNF_TMR_NAME(ulTmrName);

            if (VOS_NULL_PTR != pstCallEntity->stPsCallTimerInfo.hWaitWlanActCnfTmrHdl)
            {
                AT_StopRelTimer(ulTmrName, &(pstCallEntity->stPsCallTimerInfo.hWaitWlanActCnfTmrHdl));
            }

            AT_SET_WLAN_DEACT_PDN_CNF_TMR_NAME(ulTmrName);
            AT_SET_WLAN_DEACT_PDN_CNF_TMR_PARAM(ulTmrParam, usClientId, ucCallId);

            /* ����ȥ����CNF��ʱ�� */
            AT_StartRelTimer(&(pstCallEntity->stPsCallTimerInfo.hWaitWlanDeActCnfTmrHdl),
                             AT_WAIT_WLAN_DEACT_PDN_CNF_TIMER_LEN,
                             ulTmrName,
                             ulTmrParam,
                             VOS_RELTIMER_NOLOOP);
        }
        else
        {
            AT_ERR_LOG("AT_PS_HangupWlanCall: End call failed.");
            return VOS_ERR;
        }
    }

    return VOS_OK;
}


VOS_UINT32 AT_PS_HangupCall(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    TAF_PS_CALL_END_CAUSE_ENUM_UINT8    enCause
)
{
    /* ����ʵ������(CallId)�ɵ����߱�֤����Ч�� */

    /* (1) ���IPv4���ͺ����Ƿ��ѽ���/���ڽ���
     *     -> ��: �Ͽ��ú���, ���ú���״̬
     *     -> ��: ����
     * (2) ���IPv6���ͺ����Ƿ��ѽ���/���ڽ���
     *     -> ��: �Ͽ��ú���, ���ú���״̬
     *     -> ��: ����
     * (3) ����
     */
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;

    pstCallEntity   = AT_PS_GetCallEntity(usClientId, ucCallId);

    if ( (AT_PDP_STATE_ACTED == pstCallEntity->enIpv4State)
      || (AT_PDP_STATE_ACTING == pstCallEntity->enIpv4State) )
    {
        if (VOS_OK == TAF_PS_CallEndEx(WUEPS_PID_AT,
                                       AT_PS_BuildExClientId(usClientId),
                                       0,
                                       pstCallEntity->ucIpv4Cid,
                                       enCause))
        {
            AT_PS_SetCallStateByType(usClientId, ucCallId, TAF_PDP_IPV4, AT_PDP_STATE_DEACTING);

            if ( (pstCallEntity->ucIpv4Cid == pstCallEntity->ucIpv6Cid)
              && ( (AT_PDP_STATE_ACTED == pstCallEntity->enIpv6State)
                || (AT_PDP_STATE_ACTING == pstCallEntity->enIpv6State) ))
            {
                AT_PS_SetCallStateByType(usClientId, ucCallId, TAF_PDP_IPV6, AT_PDP_STATE_DEACTING);
            }
        }
        else
        {
            AT_ERR_LOG("AT_PS_HangupCall: STEP1->End call failed.");
            return VOS_ERR;
        }
    }

    if ( ( (pstCallEntity->ucIpv4Cid != pstCallEntity->ucIpv6Cid)
        || (AT_PDP_STATE_IDLE == pstCallEntity->enIpv4State))
      && ( (AT_PDP_STATE_ACTED == pstCallEntity->enIpv6State)
        || (AT_PDP_STATE_ACTING == pstCallEntity->enIpv6State) ) )
    {
        if (VOS_OK == TAF_PS_CallEndEx(WUEPS_PID_AT,
                                       AT_PS_BuildExClientId(usClientId),
                                       0,
                                       pstCallEntity->ucIpv6Cid,
                                       enCause))
        {
            AT_PS_SetCallStateByType(usClientId, ucCallId, TAF_PDP_IPV6, AT_PDP_STATE_DEACTING);
        }
        else
        {
            AT_ERR_LOG("AT_PS_HangupCall: STEP2->End call step2 failed.");
            return VOS_ERR;
        }
    }

    return VOS_OK;
}


VOS_UINT32 AT_PS_ProcConflictDialUpWithCurrCall(
    VOS_UINT8                           ucIndex,
    VOS_UINT8                           ucCallId
)
{
    /* ����ʵ������(CallId)�ɵ����߱�֤����Ч�� */

    /* ����������ڽ���, ����OK, ͬʱ�ϱ��ѽ���������״̬ */
    if ( (VOS_TRUE == AT_PS_IsLinkGoingUp(ucIndex, ucCallId))
      || (VOS_TRUE == AT_PS_IsWlanLinkGoingUp(ucIndex, ucCallId)))
    {
        AT_INFO_LOG("AT_PS_ProcConflictDialUpWithCurrCall: Call is going up.");

        At_FormatResultData(ucIndex, AT_OK);

        AT_PS_ReportCurrCallConnState(ucIndex, ucCallId);

        return VOS_OK;
    }

    /* ����������ڶϿ�, ����ERROR */
    if (VOS_TRUE == AT_PS_IsLinkGoingDown(ucIndex, ucCallId))
    {
        AT_INFO_LOG("AT_PS_ProcConflictDialUpWithCurrCall: Call is going down.");

        AT_PS_SetPsCallErrCause(ucIndex, TAF_PS_CAUSE_UNKNOWN);

        At_FormatResultData(ucIndex, AT_ERROR);

        return VOS_OK;
    }

    if (VOS_TRUE == AT_PS_IsWlanLinkGoingDown(ucIndex, ucCallId))
    {
        AT_INFO_LOG("AT_PS_ProcConflictDialUpWithCurrCall: wlan Call is going down.");

        AT_PS_SetPsCallErrCause(ucIndex, TAF_PS_CAUSE_UNKNOWN);

        At_FormatResultData(ucIndex, AT_ERROR);

        return VOS_OK;
    }

    return VOS_ERR;
}


VOS_VOID AT_PS_ProcDialPdpTypeChg(
    VOS_UINT16                          usClientId,
    AT_DIAL_PARAM_STRU                 *pstUsrDialParam
)
{
    AT_COMM_PS_CTX_STRU                *pstCommPsCtx    = VOS_NULL_PTR;
    AT_PS_PDP_TYPE_CHG_MGR_INFO_STRU   *pstMgrInfo      = VOS_NULL_PTR;
    AT_PS_PDP_TYPE_CHG_ENTRY_STRU      *pstEntry        = VOS_NULL_PTR;
    MODEM_ID_ENUM_UINT16                enModemId;

    enModemId = MODEM_ID_0;

    if (VOS_OK != AT_GetModemIdFromClient(usClientId, &enModemId))
    {
        AT_ERR_LOG1("AT_PS_ProcDialPdpTypeChg: modem id is invalid.", enModemId);
        return;
    }

    pstCommPsCtx    = AT_GetCommPsCtxAddr();
    pstMgrInfo      = AT_GetPsPdpTypeChgMgrInfoByModemId(enModemId);

    /* ����NV�򿪵�����������˫ջ����ʱ����Ҫ�滻PDP TYPE */
    if ( (VOS_TRUE == pstCommPsCtx->astPdpTypeChgPolicyCfg[enModemId].ucPdpTypeChgEnableFlg)
      && (TAF_PDP_IPV4V6 == pstUsrDialParam->enPdpType))
    {
        pstEntry = AT_PS_LookupPdpTypeChgMgrEntry(pstMgrInfo, pstUsrDialParam->aucAPN);

        if (VOS_NULL_PTR != pstEntry)
        {
            pstUsrDialParam->enPdpType = pstEntry->enPdpType;
        }
    }

    return;
}


VOS_UINT32 AT_PS_ProcDialUp(VOS_UINT8 ucIndex)
{
    /* (1) ��CID����ɺ���ʵ������(CallId)
     * (2) ��ȡ���Ų���
     * (3) ������ʵ��������Ч��
     *     -> ��Ч: ��������״̬�ϱ�������
     *     -> ��Ч: ��������
     * (4) ��������ʵ��
     * (5) �������
     */
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;
    AT_PS_DATA_CHANL_CFG_STRU          *pstChanCfg    = VOS_NULL_PTR;
    AT_PS_USER_INFO_STRU                stUserInfo;
    AT_DIAL_PARAM_STRU                  stUsrDialParam;
    VOS_UINT32                          ulRslt;
    AT_PS_DATA_SYS_ENUM_UINT32          enDataSystem;
    VOS_UINT8                           ucCallId;

    TAF_MEM_SET_S(&stUserInfo, sizeof(stUserInfo), 0x00, sizeof(AT_PS_USER_INFO_STRU));
    TAF_MEM_SET_S(&stUsrDialParam, sizeof(stUsrDialParam), 0x00, sizeof(AT_DIAL_PARAM_STRU));

    /* ��ȡ�û���Ϣ */
    AT_PS_ParseUsrInfo(ucIndex, &stUserInfo);

    /* ��ȡ���Ų��� */
    ulRslt = AT_PS_ParseUsrDialParam(ucIndex, &stUsrDialParam);

    if (VOS_OK != ulRslt)
    {
        AT_ERR_LOG("AT_PS_ProcDialUp: Get dial parameter failed.");

        AT_PS_SetPsCallErrCause(ucIndex, TAF_PS_CAUSE_UNKNOWN);

        return AT_ERROR;
    }

    /* ��ȡCID�����ĺ���ʵ������ */
    ucCallId = AT_PS_TransCidToCallId(ucIndex, stUsrDialParam.ucCid);

    if (AT_PS_IsCallIdValid(ucIndex, ucCallId))
    {
        ulRslt = AT_PS_ProcConflictDialUpWithCurrCall(ucIndex, ucCallId);

        if (VOS_OK != ulRslt)
        {
            AT_PS_SetPsCallErrCause(ucIndex, TAF_PS_CAUSE_UNKNOWN);

             AT_ERR_LOG("AT_PS_ProcDialUp: AT_PS_FreeCallEntity 16376.");
            AT_PS_FreeCallEntity(ucIndex, ucCallId, TAF_PS_APN_DATA_SYS_NONE);

            AT_ERR_LOG("AT_PS_ProcCallDialUp: Call entity status error.");
            return AT_ERROR;
        }

        return AT_SUCCESS;
    }

    /* ��������ʵ�� */
    ulRslt = AT_PS_AllocCallEntity(ucIndex, &ucCallId);

    if (VOS_OK != ulRslt)
    {
        AT_NORM_LOG("AT_PS_ProcDialUp: Alloc call entity failed.");

        AT_PS_SetPsCallErrCause(ucIndex, TAF_PS_CAUSE_UNKNOWN);

        return AT_ERROR;
    }

    pstChanCfg    = AT_PS_GetDataChanlCfg(ucIndex, stUserInfo.ucUsrCid);

    pstCallEntity = AT_PS_GetCallEntity(ucIndex, ucCallId);

    /* ��������ID*/
    pstCallEntity->ucIfaceId      = (VOS_UINT8)pstChanCfg->ulIfaceld;
    pstCallEntity->ucRmNetId      = (VOS_UINT8)pstChanCfg->ulRmNetId;

    /* �����û���Ϣ */
    pstCallEntity->stUserInfo     = stUserInfo;

    /* ���沦�Ų��� */
    pstCallEntity->stUsrDialParam = stUsrDialParam;

    /* ������ѡ���� */
    AT_PS_SelectApnDataSysConfig(ucIndex, pstCallEntity);

    enDataSystem = AT_PS_GetPreDataSystem(pstCallEntity, ucIndex);

    /* ������ѡ��� ѡ����з����� */
    ulRslt          = AT_PS_DialUpDataSys(pstCallEntity, enDataSystem, ucIndex, ucCallId);

    if (VOS_OK != ulRslt)
    {
        /* �ͷź���ʵ�� */
        AT_PS_FreeCallEntity(ucIndex, ucCallId, TAF_PS_APN_DATA_SYS_NONE);

        return AT_ERROR;
    }

    return AT_OK;
}


VOS_UINT32 AT_PS_DialUpDataSys(
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity,
    AT_PS_DATA_SYS_ENUM_UINT32          enDataSysType,
    VOS_UINT8                           ucIndex,
    VOS_UINT8                           ucCallId
)
{
    AT_DIAL_PARAM_STRU                  stUsrDialParam;
    VOS_UINT32                          ulRslt;

    TAF_MEM_SET_S(&stUsrDialParam, (VOS_UINT32)(sizeof(stUsrDialParam)), 0x00, (VOS_UINT32)(sizeof(AT_DIAL_PARAM_STRU)));

    TAF_MEM_CPY_S(&stUsrDialParam,
                  (VOS_UINT32)(sizeof(stUsrDialParam)),
                  &(pstCallEntity->stUsrDialParam),
                  (VOS_UINT32)(sizeof(pstCallEntity->stUsrDialParam)));

    ulRslt                 = VOS_ERR;

    if (AT_PS_DATA_SYS_CELLULAR == enDataSysType)
    {
        /* ��Cellular�·������ */
        /* ����PDP TYPE CHG */
        AT_PS_ProcDialPdpTypeChg(pstCallEntity->stUserInfo.enUserIndex, &stUsrDialParam);

        /* �������� */
        ulRslt = AT_PS_SetupCall(pstCallEntity->stUserInfo.enUserIndex, ucCallId, &stUsrDialParam);
    }
    else if (AT_PS_DATA_SYS_WLAN == enDataSysType)
    {
        /* ��Wlan�·������ */
        /* APN������ */
        if (VOS_FALSE == AT_PS_IsWlanThrotAllowed(pstCallEntity))
        {
            /* �����ڴ����뵥����¼ */
            AT_PS_SetPsCallErrCause(ucIndex, TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_THROT_NOT_ALLOWED);

            return VOS_ERR;
        }

        /* �������� */
        ulRslt = AT_PS_SetupWlanCall(&stUsrDialParam, ucIndex, ucCallId);
    }
    else
    {
        AT_ERR_LOG("AT_PS_DialUpDataSys: DIALUP DATA SYS NOT ALLOWED.");
    }

    if (VOS_OK != ulRslt)
    {
        /* �������� */
        AT_ERR_LOG("AT_PS_DialUpDataSys: not allow dial up.");

        /* ��¼���д����� */
        AT_PS_SetPsCallErrCause(ucIndex, TAF_PS_CAUSE_UNKNOWN);

        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_PS_ProcDialDownOneDataSys(
    VOS_UINT8                           ucIndex,
    VOS_UINT8                           ucCallId,
    TAF_PS_CALL_END_CAUSE_ENUM_UINT8    enCause
)
{
    AT_PS_USER_INFO_STRU               *pstUserInfo = VOS_NULL_PTR;
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;

    pstUserInfo = AT_PS_GetUserInfo(ucIndex, ucCallId);
    pstCallEntity = AT_PS_GetCallEntity(ucIndex, ucCallId);

    if ( (VOS_TRUE == pstCallEntity->stApnDataSysInfo.ucDataSysInfoFlg)
      && (TAF_PS_APN_DATA_SYS_WLAN == pstCallEntity->stApnDataSysInfo.enCurrentDataSys))
    {
        /* ��Ҫ��WLAN�Ϸ���ȥ�������� */
        if (VOS_TRUE == AT_PS_IsWlanLinkGoingDown(ucIndex, ucCallId))
        {
            At_FormatResultData(ucIndex, AT_OK);

            AT_PS_ReportCurrCallEndState(ucIndex, ucCallId);

            return AT_SUCCESS;
        }

        if (VOS_OK != AT_PS_HangupWlanCall(pstUserInfo->enUserIndex, ucCallId, VOS_FALSE, VOS_FALSE))
        {
            AT_ERR_LOG("AT_PS_ProcDialDown: Hangup wlan call failed.");
            return AT_ERROR;
        }

        return AT_OK;
    }

    /* �������״̬�Ƿ�Ϊ���ڶϿ�
     * (A) ����������ڶϿ�
     *���� -> �ϱ�OK
     *     -> �ϱ��ѶϿ�����^DEND(or NDISSTAT)
     *     -> ����SUCCESS
     * (B) ����״̬
     *     -> ��������
     */
    if (VOS_TRUE == AT_PS_IsLinkGoingDown(ucIndex, ucCallId))
    {
        At_FormatResultData(ucIndex, AT_OK);

        AT_PS_ReportCurrCallEndState(ucIndex, ucCallId);

        return AT_SUCCESS;
    }

    /* �ҶϺ��� */
    if (VOS_OK != AT_PS_HangupCall(pstUserInfo->enUserIndex, ucCallId, enCause))
    {
        AT_ERR_LOG("AT_PS_ProcDialDown: Hangup call failed.");
        return AT_ERROR;
    }

    return AT_OK;
}


VOS_UINT32 AT_PS_ProcDialDownMultiDataSys(
    VOS_UINT8                           ucIndex,
    VOS_UINT8                           ucCallId,
    TAF_PS_CALL_END_CAUSE_ENUM_UINT8    enCause
)
{
    AT_PS_USER_INFO_STRU               *pstUserInfo = VOS_NULL_PTR;
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;

    pstUserInfo = AT_PS_GetUserInfo(ucIndex, ucCallId);
    pstCallEntity = AT_PS_GetCallEntity(ucIndex, ucCallId);

    if (TAF_PS_APN_DATA_SYS_CELLULAR == pstCallEntity->stApnDataSysInfo.enCurrentDataSys)
    {
        /* ��Ҫ��CELLULAR�Ϸ���ȥ����������WLAN�Ϸ��𱾵�ȥ�������� */
        if (VOS_OK != AT_PS_HangupCall(pstUserInfo->enUserIndex, ucCallId, enCause))
        {
            AT_ERR_LOG("AT_PS_ProcDialDownMultiDataSys: Hangup call failed.");
            return AT_ERROR;
        }

        (VOS_VOID)AT_PS_HangupWlanCall(pstUserInfo->enUserIndex, ucCallId, VOS_TRUE, VOS_TRUE);

        AT_PS_SetCallHandOverFlg(pstUserInfo->enUserIndex, ucCallId, VOS_FALSE);

        /* ����handover�������� */
        AT_PS_SetHoCallType(pstUserInfo->enUserIndex, ucCallId, TAF_PDP_TYPE_BUTT);

        return AT_OK;
    }
    else if (TAF_PS_APN_DATA_SYS_WLAN == pstCallEntity->stApnDataSysInfo.enCurrentDataSys)
    {
        /* ��Ҫ��WLAN�Ϸ���ȥ����������CELLULAR�Ϸ��𱾵�ȥ�������� */
        if (VOS_OK != AT_PS_HangupWlanCall(pstUserInfo->enUserIndex, ucCallId, VOS_FALSE, VOS_FALSE))
        {
            AT_ERR_LOG("AT_PS_ProcDialDownMultiDataSys: Hangup wlan call failed.");
            return AT_ERROR;
        }

        (VOS_VOID)AT_PS_HangupCall(pstCallEntity->stUserInfo.enUserIndex, ucCallId, TAF_PS_CALL_END_CAUSE_LOCAL);

        AT_PS_SetCallHandOverFlg(pstUserInfo->enUserIndex, ucCallId, VOS_FALSE);

        /* ����handover�������� */
        AT_PS_SetHoCallType(pstUserInfo->enUserIndex, ucCallId, TAF_PDP_TYPE_BUTT);

        return AT_OK;
    }
    else
    {
        AT_ERR_LOG("AT_PS_ProcDialDownMultiDataSys: enCurrentDataSys is failed.");
        return AT_ERROR;
    }
}


VOS_UINT32 AT_PS_ProcDialDown(VOS_UINT8 ucIndex)
{
    /* (1) ��CID����ɺ���ʵ������(CallId)
     * (2) ������ʵ��������Ч��
     *     -> ��Ч: ֱ���ϱ����ӶϿ�
     *     -> ��Ч: ����
     * (3) �������״̬
     * (4) �Ͽ�����
     */

    /* ��ȡCID�����ĺ���ʵ������(CallId)
     * (A) CallId��Ч
     *     -> �ϱ�OK
     *     -> �ϱ���������^DEND(or NDISSTAT)
     *     -> ����
     * (B) CallId��Ч
     *     -> ��������
     */
    VOS_UINT8                           ucCallId;
    TAF_PS_CALL_END_CAUSE_ENUM_UINT8    enCause;

    ucCallId = AT_PS_TransCidToCallId(ucIndex, (VOS_UINT8)gastAtParaList[0].ulParaValue);

    enCause = AT_PS_TransCallEndCause((VOS_UINT8)gastAtParaList[1].ulParaValue);

    if (VOS_FALSE ==  AT_PS_IsCallIdValid(ucIndex, ucCallId))
    {
        At_FormatResultData(ucIndex, AT_OK);

        AT_PS_ReportAllCallEndState(ucIndex);

        return AT_SUCCESS;
    }

    if (VOS_FALSE == AT_PS_GetPsCallHandOverFlg(ucIndex, ucCallId))
    {
        /* ֻ����һ��������Ҫȥ���� */
        return AT_PS_ProcDialDownOneDataSys(ucIndex, ucCallId, enCause);
    }
    else
    {
        /* CELLULAR��WLAN�϶���Ҫȥ���� */
        return AT_PS_ProcDialDownMultiDataSys(ucIndex, ucCallId, enCause);
    }
}


VOS_UINT32 AT_PS_ProcDialCmd(VOS_UINT8 ucIndex)
{
    VOS_UINT32                          ulRslt;

    /* ��������Ч�� */
    ulRslt = AT_PS_ValidateDialParam(ucIndex);

    if (AT_SUCCESS != ulRslt)
    {
        /* ��¼PS����д����� */
        AT_PS_SetPsCallErrCause(ucIndex, TAF_PS_CAUSE_INVALID_PARAMETER);

        return ulRslt;
    }

    if (TAF_PS_CALL_TYPE_UP == gastAtParaList[1].ulParaValue)
    {
        ulRslt = AT_PS_ProcDialUp(ucIndex);
    }
    else
    {
        ulRslt = AT_PS_ProcDialDown(ucIndex);
    }

    return ulRslt;
}


VOS_UINT16 AT_PS_BuildExClientId(VOS_UINT16 usClientId)
{
    MODEM_ID_ENUM_UINT16                enModemId;

    enModemId = MODEM_ID_0;

    if (VOS_OK != AT_GetModemIdFromClient(usClientId, &enModemId))
    {
        enModemId =  MODEM_ID_BUTT;
    }

    return TAF_PS_BUILD_EXCLIENTID(enModemId, usClientId);
}


VOS_UINT16 AT_PS_BuildNdisExClientId(
    VOS_UINT16                          usPortIndex,
    VOS_UINT16                          usClientId
)
{
    MODEM_ID_ENUM_UINT16                enModemId;

    enModemId = MODEM_ID_0;

    if (VOS_OK != AT_GetModemIdFromClient(usPortIndex, &enModemId))
    {
        enModemId =  MODEM_ID_BUTT;
    }

    return TAF_PS_BUILD_EXCLIENTID(enModemId, usClientId);
}





VOS_UINT32 AT_PS_CheckDialRatType(
    VOS_UINT8                           ucIndex,
    VOS_UINT8                           ucBitRatType
)
{
    if (VOS_TRUE == At_CheckCurrRatModeIsCL(ucIndex))
    {
        switch (ucBitRatType)
        {
            case AT_PS_DIAL_RAT_TYPE_NO_ASTRICT:
            case AT_PS_DIAL_RAT_TYPE_1X_OR_HRPD:
            case AT_PS_DIAL_RAT_TYPE_LTE_OR_EHRPD:
                return VOS_TRUE;

            default:
                AT_NORM_LOG1("AT_PS_CheckDialRatType: Rat Type Error.\n", ucBitRatType);
                return VOS_FALSE;
        }
    }
    else
    {
        AT_NORM_LOG("AT_PS_CheckDialRatType: Not CL mode.\n");
        return VOS_FALSE;
    }

}


VOS_VOID AT_PS_ProcRabidChangedEvent(
    TAF_PS_CALL_PDP_RABID_CHANGE_IND_STRU  *pstEvent
)
{
    VOS_UINT8                               ucCallId;
    AT_PS_CALL_ENTITY_STRU                 *pstCallEntity   = VOS_NULL_PTR;
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU      *pstActEvent     = VOS_NULL_PTR;
    TAF_PS_CALL_PDP_DEACTIVATE_CNF_STRU     stDeactEvent;

    ucCallId = AT_PS_TransCidToCallId(pstEvent->stCtrl.usClientId, pstEvent->ucCid);

    if (VOS_FALSE == AT_PS_IsCallIdValid(pstEvent->stCtrl.usClientId, ucCallId))
    {
        AT_ERR_LOG("AT_PS_ProcRabidChangedEvent:ERROR: CallId is invalid!");
        return;
    }

    /*--------------------------1.֪ͨ����ȥ���ȥע�����ص�----------------------------*/

    TAF_MEM_SET_S(&stDeactEvent,
                  sizeof(stDeactEvent),
                  0,
                  sizeof(TAF_PS_CALL_PDP_DEACTIVATE_CNF_STRU));

    TAF_MEM_CPY_S(&stDeactEvent.stCtrl,
                  sizeof(TAF_CTRL_STRU),
                  &pstEvent->stCtrl,
                  sizeof(TAF_CTRL_STRU));

    stDeactEvent.ucRabId                = pstEvent->ucOldRabId;
    stDeactEvent.enPdpType              = pstEvent->enPdpType;

    /* ����������PDPȥ�����¼� */
    AT_PS_DeactivateRmNet(ucCallId, &stDeactEvent, pstEvent->enPdpType);

    /* ��FCȥע�����ص� */
    AT_PS_DeRegFCPoint(ucCallId, &stDeactEvent);


    /*--------------------------2.֪ͨ�������ע�����ص�----------------------------*/
    /*lint -save -e516 */
    pstActEvent = (TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU *)PS_MEM_ALLOC(WUEPS_PID_TAF,
                      sizeof(TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU));
    /*lint -restore */
    /* ������Ϣʧ�� */
    if (VOS_NULL_PTR == pstActEvent)
    {
        /* ϵͳ�쳣��ӡ, ֱ�ӷ��� */
        AT_ERR_LOG("AT_PS_ProcRabidChangedEvent: PS_MEM_ALLOC Error!");
        return;
    }

    TAF_MEM_SET_S(pstActEvent,
                  sizeof(TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU),
                  0,
                  sizeof(TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU));

    TAF_MEM_CPY_S(&pstActEvent->stCtrl,
                  sizeof(TAF_CTRL_STRU),
                  &pstEvent->stCtrl,
                  sizeof(TAF_CTRL_STRU));

    pstActEvent->ucRabId                = pstEvent->ucNewRabId;

    /*--------------------------3.ˢ��AT������Ϣ----------------------------*/
    pstCallEntity = AT_PS_GetCallEntity(pstEvent->stCtrl.usClientId, ucCallId);

    if (TAF_PDP_IPV4 == (pstEvent->enPdpType & TAF_PDP_IPV4))
    {
        pstCallEntity->stIpv4DhcpInfo.ucRabId   = pstEvent->ucNewRabId;
    }

    if (TAF_PDP_IPV6 == (pstEvent->enPdpType & TAF_PDP_IPV6))
    {
        pstCallEntity->stIpv6DhcpInfo.ucRabId   = pstEvent->ucNewRabId;
    }

    /* ����������PDP�����¼� */
    AT_PS_ActivateRmNet(ucCallId, pstActEvent, pstEvent->enPdpType);

    /* ��FCע�����ص� */
    AT_PS_RegFCPoint(ucCallId, pstActEvent);
    /*lint -save -e516 */
    PS_MEM_FREE(WUEPS_PID_TAF, pstActEvent);
    /*lint -restore */

    return;
}


TAF_PS_CAUSE_ENUM_UINT32 AT_PS_MapWlanActCnfCause(
    WIFI_IMSA_PDN_ACT_RESULT_ENUM_INT8      ucErrCause
)
{
    AT_PS_WLAN_PDN_ACT_ERR_CODE_MAP_STRU   *pstErrMapTblPtr = VOS_NULL_PTR;
    VOS_UINT32                              ulErrMapTblSize;
    VOS_UINT32                              ulCnt;

    pstErrMapTblPtr     = g_astTafPsWlanPdnActErrCodeMapTbl;
    ulErrMapTblSize     = sizeof(g_astTafPsWlanPdnActErrCodeMapTbl) / sizeof(AT_PS_WLAN_PDN_ACT_ERR_CODE_MAP_STRU);

    for (ulCnt = 0; ulCnt < ulErrMapTblSize; ulCnt++)
    {
        if (pstErrMapTblPtr[ulCnt].ulWlanCause == ucErrCause)
        {
            return pstErrMapTblPtr[ulCnt].enPsCause;
        }
    }

    return TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_UNKNOWN;
}


TAF_PS_CAUSE_ENUM_UINT32 AT_PS_MapWlanDeActIndCause(
    WIFI_IMSA_PDN_DEACT_CAUSE_ENUM_INT8     ucErrCause
)
{
    AT_PS_WLAN_PDN_DEACT_ERR_CODE_MAP_STRU *pstErrMapTblPtr = VOS_NULL_PTR;
    VOS_UINT32                              ulErrMapTblSize;
    VOS_UINT32                              ulCnt;

    pstErrMapTblPtr     = g_astTafPsWlanPdnDeActErrCodeMapTbl;
    ulErrMapTblSize     = sizeof(g_astTafPsWlanPdnDeActErrCodeMapTbl) / sizeof(AT_PS_WLAN_PDN_DEACT_ERR_CODE_MAP_STRU);

    for (ulCnt = 0; ulCnt < ulErrMapTblSize; ulCnt++)
    {
        if (pstErrMapTblPtr[ulCnt].ulWlanCause == ucErrCause)
        {
            return pstErrMapTblPtr[ulCnt].enPsCause;
        }
    }

    return TAF_PS_CAUSE_WLAN_PDN_DEACT_RESULT_UNKNOWN;
}


VOS_VOID AT_PS_SetWlanCallStateByType(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType,
    AT_PDP_STATE_ENUM_U8                enPdpState
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);

    switch (enPdpType)
    {
        case TAF_PDP_IPV4:
            pstCallEntity->enWlanIpv4State = enPdpState;
            break;

        case TAF_PDP_IPV6:
            pstCallEntity->enWlanIpv6State = enPdpState;
            break;

        case TAF_PDP_IPV4V6:
            pstCallEntity->enWlanIpv4State = enPdpState;
            pstCallEntity->enWlanIpv6State = enPdpState;
            break;

        default:
            AT_ERR_LOG("AT_PS_SetWlanCallStateByType: PDP type is invalid!");
            break;
    }

    return;
}

VOS_VOID AT_PS_SetWlanCurrCallType(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);

    pstCallEntity->enWlanPdpType = enPdpType;

    return;
}

VOS_VOID AT_PS_SetHoCallType(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);

    pstCallEntity->enHoPdpType = enPdpType;

    return;
}


VOS_VOID AT_PS_SetPsCallCurrentDataSys(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    TAF_PS_APN_DATA_SYS_ENUM_UINT8      enCurrentDataSys
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);

    if (VOS_TRUE == pstCallEntity->stApnDataSysInfo.ucDataSysInfoFlg)
    {
        pstCallEntity->stApnDataSysInfo.enCurrentDataSys = enCurrentDataSys;
    }

    return;
}


TAF_PS_APN_DATA_SYS_ENUM_UINT8 AT_PS_GetPsCallCurrentDataSys(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);

    if (VOS_TRUE == pstCallEntity->stApnDataSysInfo.ucDataSysInfoFlg)
    {
        return pstCallEntity->stApnDataSysInfo.enCurrentDataSys;
    }

    return TAF_PS_APN_CURRENT_DOMAIN_BUTT;
}


VOS_VOID AT_PS_SetCallHandOverFlg(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    VOS_UINT8                           ucIsHandOver
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);

    if (VOS_TRUE == pstCallEntity->stApnDataSysInfo.ucDataSysInfoFlg)
    {
        AT_NORM_LOG1("AT_PS_SetCallHandOverFlg: handover is ", ucIsHandOver);
        pstCallEntity->stApnDataSysInfo.ucIsHandOver = ucIsHandOver;
    }

    return;
}


VOS_UINT8 AT_PS_GetPsCallHandOverFlg(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);

    if (VOS_TRUE == pstCallEntity->stApnDataSysInfo.ucDataSysInfoFlg)
    {
        AT_NORM_LOG1("AT_PS_GetPsCallHandOverFlg: handover is ", pstCallEntity->stApnDataSysInfo.ucIsHandOver);
        return pstCallEntity->stApnDataSysInfo.ucIsHandOver;
    }

    AT_NORM_LOG("AT_PS_GetPsCallHandOverFlg: not data sys apn, handover is false");
    return VOS_FALSE;
}


VOS_VOID AT_PS_SetCallHandOverFailCause(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    TAF_PS_CAUSE_ENUM_UINT32            enHoFailCause
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);

    if (VOS_TRUE == pstCallEntity->stApnDataSysInfo.ucDataSysInfoFlg)
    {
        AT_NORM_LOG1("AT_PS_SetCallHandOverFailCause: enHoFailCause is ", enHoFailCause);
        pstCallEntity->stApnDataSysInfo.enHoFailCause = enHoFailCause;
    }

    return;
}


TAF_PS_CAUSE_ENUM_UINT32 AT_PS_GetCallHandOverFailCause(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);

    if (VOS_TRUE == pstCallEntity->stApnDataSysInfo.ucDataSysInfoFlg)
    {
        AT_NORM_LOG1("AT_PS_GetCallHandOverFailCause: enHoFailCause is ", pstCallEntity->stApnDataSysInfo.enHoFailCause);
        return pstCallEntity->stApnDataSysInfo.enHoFailCause;
    }

    return TAF_PS_CAUSE_SUCCESS;
}


TAF_PDP_TYPE_ENUM_UINT8 AT_PS_GetHoPdpIpType(
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity
)
{
    if (TAF_PS_APN_DATA_SYS_CELLULAR == pstCallEntity->stApnDataSysInfo.enCurrentDataSys)
    {
        /* CELLULAR2WLAN */
        if ( (AT_PDP_STATE_ACTED == pstCallEntity->enIpv4State)
          && (AT_PDP_STATE_ACTED == pstCallEntity->enIpv6State))
        {
            return TAF_PDP_IPV4V6;
        }
        else if (AT_PDP_STATE_ACTED == pstCallEntity->enIpv4State)
        {
            return TAF_PDP_IPV4;
        }
        else
        {
            return TAF_PDP_IPV6;
        }
    }
    else if (TAF_PS_APN_DATA_SYS_WLAN == pstCallEntity->stApnDataSysInfo.enCurrentDataSys)
    {
        /* WLAN2CELLULAR */
        if ( (AT_PDP_STATE_ACTED == pstCallEntity->enWlanIpv4State)
          && (AT_PDP_STATE_ACTED == pstCallEntity->enWlanIpv6State))
        {
            return TAF_PDP_IPV4V6;
        }
        else if (AT_PDP_STATE_ACTED == pstCallEntity->enWlanIpv4State)
        {
            return TAF_PDP_IPV4;
        }
        else
        {
            return TAF_PDP_IPV6;
        }
    }
    else
    {
        AT_ERR_LOG("AT_PS_GetHoPdpIpType: enCurrentDataSys is error.");

        return pstCallEntity->stUsrDialParam.enPdpType;
    }
}


VOS_UINT32 AT_PS_GetDestDataSysWhenSupMultiDataSys(
    TAF_PS_DATA_SYSTEM_STATUS_STRU          stDataSystem,
    AT_PS_CALL_ENTITY_STRU                 *pstCallEntity,
    TAF_PS_APN_DATA_SYS_ENUM_UINT8         *penDestDataSys
)
{
    TAF_PS_APN_DATA_SYS_POLICY_INFO_STRU    *pstApnDataSysPolicyInfo = VOS_NULL_PTR;

    /* ͨ����ѡ�����õ���ѡ���� */
    pstApnDataSysPolicyInfo = AT_PS_GetApnDataSysPolicyInfo(pstCallEntity->stUserInfo.enPortIndex,
                                                            pstCallEntity->stApnDataSysInfo.ucDataSysPolicyIndex);

    if ( (VOS_FALSE == pstCallEntity->stApnDataSysInfo.ucDataSysInfoFlg)
      || (TAF_PS_APN_DATA_SYS_POLICY_CELLULAR_PREFER != pstApnDataSysPolicyInfo->stDataSysPolicy.enDataSysPolicy))
    {
        AT_WARN_LOG("AT_PS_GetDestDataSysWhenSupMultiDataSys: not support multi data sys");
        return VOS_FALSE;
    }

    if ( (VOS_FALSE == stDataSystem.bCellularPowerState)
      && (VOS_FALSE == stDataSystem.bWlanPowerState))
    {
        AT_WARN_LOG("AT_PS_GetDestDataSysWhenSupMultiDataSys: DATA SYS IS  NONE");
        return VOS_FALSE;
    }

    if (TAF_PS_APN_DATA_SYS_POLICY_CELLULAR_PREFER == pstApnDataSysPolicyInfo->stDataSysPolicy.enDataSysPolicy)
    {
        /* CELLULAR������ */
        if ( (VOS_TRUE == stDataSystem.bCellularPowerState)
          && (TAF_PS_CELLULAR_SERVICE_STATUS_NORMAL_SERVICE == stDataSystem.enPsServiceStatus))
        {
            AT_NORM_LOG("AT_PS_GetDestDataSysWhenSupMultiDataSys: Dest Data Sys is CELLULAR.");
            *penDestDataSys = TAF_PS_APN_DATA_SYS_CELLULAR;
            return VOS_TRUE;
        }

        if ( (VOS_TRUE == stDataSystem.bWlanPowerState)
          && (TAF_PS_WLAN_SERVICE_STATUS_NORMAL_SERVICE == stDataSystem.enWiFiServiceStatus))
        {
            AT_NORM_LOG("AT_PS_GetDestDataSysWhenSupMultiDataSys: Dest Data Sys is WLAN .");
            *penDestDataSys = TAF_PS_APN_DATA_SYS_WLAN;
            return VOS_TRUE;
        }
    }

    AT_WARN_LOG("AT_PS_GetDestDataSysWhenSupMultiDataSys: data sys unknown!");
    return VOS_FALSE;
}

TAF_PS_APN_DATA_SYS_POLICY_INFO_STRU* AT_PS_GetApnDataSysPolicyInfo(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucIndex
)
{
    AT_COMM_PS_CTX_STRU                *pstCommPsCtx          = VOS_NULL_PTR;
    VOS_UINT32                          ulRslt;
    MODEM_ID_ENUM_UINT16                usModemId;

    usModemId = MODEM_ID_0;

    ulRslt = AT_GetModemIdFromClient((VOS_UINT8)usClientId, &usModemId);

    if (VOS_ERR == ulRslt)
    {
        AT_ERR_LOG("AT_PS_GetApnDataSysPolicyInfo: Get ModemId Fail!");
    }

    pstCommPsCtx    = AT_GetCommPsCtxAddr();

    return &(pstCommPsCtx->astApnDataSysCfgList[usModemId].astApnDataSysInfo[ucIndex]);
}

VOS_VOID AT_PS_SelectApnDataSysConfig(
    VOS_UINT8                           ucIndex,
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity
)
{
    TAF_PS_APN_DATA_SYS_POLICY_CONFIG_STRU     *pstApnDataSysCfgList  = VOS_NULL_PTR;
    VOS_UINT32                                  i;
    MODEM_ID_ENUM_UINT16                        usModemId;

    usModemId = MODEM_ID_0;

    if (VOS_OK != AT_GetModemIdFromClient(ucIndex, &usModemId))
    {
        AT_ERR_LOG("AT_PS_SelectApnDataSysConfig: Get modem id fail.");

        return;
    }

    pstApnDataSysCfgList    = &(AT_GetCommPsCtxAddr()->astApnDataSysCfgList[usModemId]);

    /* ������ѡ����APN */
    for (i = 0; i < pstApnDataSysCfgList->ucProfileNum; i++)
    {
        /* �û�����APN����ѡAPNƥ�� �������õ�����ʵ�� */
        if (0 == VOS_StrNiCmp((VOS_CHAR *)(pstApnDataSysCfgList->astApnDataSysInfo[i].aucApn),
                                (VOS_CHAR *)(pstCallEntity->stUsrDialParam.aucAPN),
                                TAF_MAX_APN_LEN + 1))
        {
            AT_NORM_LOG1("AT_PS_SelectApnDataSysConfig: this apn is data sys config. data system policy index is ", i);

            pstCallEntity->stApnDataSysInfo.ucDataSysInfoFlg     = VOS_TRUE;
            pstCallEntity->stApnDataSysInfo.ucDataSysPolicyIndex = (VOS_UINT8)i;
            break;
        }
    }

    return;
}


AT_PS_DATA_SYS_ENUM_UINT32 AT_PS_GetPreDataSystem(
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity,
    VOS_UINT8                           ucIndex
)
{
    TAF_PS_APN_DATA_SYS_POLICY_INFO_STRU   *pstApnDataSysPolicyInfo = VOS_NULL_PTR;
    VOS_UINT32                              ulRslt;
    VOS_UINT8                               ucBitDataSystem;

    /* ͨ����ѡ�����õ���ѡ���� */
    pstApnDataSysPolicyInfo = AT_PS_GetApnDataSysPolicyInfo(ucIndex, pstCallEntity->stApnDataSysInfo.ucDataSysPolicyIndex);

    ulRslt                  = VOS_ERR;
    ucBitDataSystem         = 0;

    /* ��ѡ���ò�ʹ�� */
    if (VOS_FALSE == pstCallEntity->stApnDataSysInfo.ucDataSysInfoFlg)
    {
        return AT_PS_DATA_SYS_CELLULAR;
    }

    /* ��ȡϵͳ״̬ʧ�� �������� */
    ulRslt = TAF_AGENT_GetDataSystem(ucIndex, &ucBitDataSystem);

    if (VOS_OK != ulRslt)
    {
        AT_NORM_LOG("AT_PS_GetPreDataSystem: Can't Get Data System ");
        return AT_PS_DATA_SYS_NONE;
    }

    if (0 == ucBitDataSystem)
    {
        AT_NORM_LOG("AT_PS_GetPreDataSystem: ucBitDataSystem is NONE .");
        return AT_PS_DATA_SYS_NONE;
    }

    /* ��Cellular�·��� */
    if (VOS_TRUE == AT_PS_DialUpInCellularAllowed(ucBitDataSystem, pstApnDataSysPolicyInfo->stDataSysPolicy.enDataSysPolicy))
    {
        AT_NORM_LOG("AT_PS_GetPreDataSystem: Cellular is satisfy .");
        return AT_PS_DATA_SYS_CELLULAR;
    }

    /* ��Wlan�·��� */
    if (VOS_TRUE == AT_PS_DialUpInWlanAllowed(ucBitDataSystem, pstApnDataSysPolicyInfo->stDataSysPolicy.enDataSysPolicy))
    {
        AT_NORM_LOG("AT_PS_GetPreDataSystem: Wlan is satisfy .");
        return AT_PS_DATA_SYS_WLAN;
    }

    /* �������� */
    AT_NORM_LOG("AT_PS_GetPreDataSystem: Data System is not satisfy .");
    return AT_PS_DATA_SYS_NONE;
}

VOS_UINT32 AT_PS_IsPsCallInSteadyState(
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity
)
{
    /* �����л������� */
    if ( ( (AT_PDP_STATE_IDLE != pstCallEntity->enIpv4State) || (AT_PDP_STATE_IDLE != pstCallEntity->enIpv6State))
      && ( (AT_PDP_STATE_IDLE != pstCallEntity->enWlanIpv4State) || (AT_PDP_STATE_IDLE != pstCallEntity->enWlanIpv6State)))
    {
        AT_NORM_LOG("AT_PS_IsPsCallInSteadyState: data sys switching .");
        return VOS_FALSE;
    }

    /* ԭ��(CELLULAR)�����ڷ���ȥ������� */
    if (TAF_PS_APN_DATA_SYS_CELLULAR == pstCallEntity->stApnDataSysInfo.enCurrentDataSys)
    {
        if ( (AT_PDP_STATE_DEACTING == pstCallEntity->enIpv4State)
          || (AT_PDP_STATE_DEACTING == pstCallEntity->enIpv6State))
        {
            AT_NORM_LOG("AT_PS_IsPsCallInSteadyState: cellular is deacting .");
            return VOS_FALSE;
        }
    }

    /* ԭ��(WLAN)�����ڷ���ȥ������� */
    if (TAF_PS_APN_DATA_SYS_WLAN == pstCallEntity->stApnDataSysInfo.enCurrentDataSys)
    {
        if ( (AT_PDP_STATE_DEACTING == pstCallEntity->enWlanIpv4State)
          || (AT_PDP_STATE_DEACTING == pstCallEntity->enWlanIpv6State))
        {
            AT_NORM_LOG("AT_PS_IsPsCallInSteadyState: wlan is deacting .");
            return VOS_FALSE;
        }
    }

    return VOS_TRUE;
}


VOS_UINT32 AT_PS_IsNeedPsCallDataSysSwitch(
    VOS_UINT16                          usClientId,
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity,
    TAF_PS_APN_DATA_SYS_ENUM_UINT8     *penDestDataSys
)
{
    AT_COMM_PS_CTX_STRU                    *pstCommPsCtx            = VOS_NULL_PTR;
    TAF_PS_APN_DATA_SYS_POLICY_INFO_STRU   *pstApnDataSysPolicyInfo = VOS_NULL_PTR;
    VOS_UINT32                              ulRst;

    MODEM_ID_ENUM_UINT16                    usModemId;

    pstCommPsCtx    = AT_GetCommPsCtxAddr();

    if (VOS_OK != AT_GetModemIdFromClient(usClientId, &usModemId))
    {
        AT_ERR_LOG("AT_PS_IsNeedPsCallDataSysSwitch: Get modem id fail.");

        return VOS_FALSE;
    }

    /* ͨ����ѡ�����õ���ѡ���� */
    pstApnDataSysPolicyInfo = AT_PS_GetApnDataSysPolicyInfo(usClientId, pstCallEntity->stApnDataSysInfo.ucDataSysPolicyIndex);

    /* ��ѡ���Բ���CELLULAR��WLAN���������л� */
    if (TAF_PS_APN_DATA_SYS_POLICY_CELLULAR_PREFER != pstApnDataSysPolicyInfo->stDataSysPolicy.enDataSysPolicy)
    {
        AT_NORM_LOG("AT_PS_IsNeedPsCallDataSysSwitch: not TAF_PS_APN_DATA_SYS_POLICY_CELLULAR_PREFER.");

        return VOS_FALSE;
    }

    ulRst = AT_PS_GetDestDataSysWhenSupMultiDataSys(pstCommPsCtx->astDataSystemStatus[usModemId], pstCallEntity, penDestDataSys);

    /* �õ�Ŀ����ʧ�� */
    if (VOS_FALSE == ulRst)
    {
        AT_NORM_LOG("AT_PS_IsNeedPsCallDataSysSwitch: AT_PS_GetDestDataSysWhenSupMultiDataSys is false.");

        return VOS_FALSE;
    }

    /* ��ǰ���Ѿ����� */
    if ((*penDestDataSys) == pstCallEntity->stApnDataSysInfo.enCurrentDataSys)
    {
        AT_NORM_LOG("AT_PS_IsNeedPsCallDataSysSwitch: CurrentDataSys is ok.");

        return VOS_FALSE;
    }

    /* ��ƹ�Ҷ�ʱ���������У��������л� */
    if (VOS_NULL_PTR != pstCallEntity->stPsCallTimerInfo.hProtectInDataSysTmrHdl)
    {
        AT_NORM_LOG("AT_PS_IsNeedPsCallDataSysSwitch: hProtectInDataSysTmrHdl is not null.");

        return VOS_FALSE;
    }

    /* WLAN2CELLULAR����������²�֧���л����������л� */
    if ( (TAF_PS_APN_DATA_SYS_CELLULAR == (*penDestDataSys))
      && (VOS_TRUE == pstCommPsCtx->astDataSystemStatus[usModemId].bCellularRoamingStatus)
      && (VOS_FALSE == pstApnDataSysPolicyInfo->stDataSysPolicy.ucRoamHoSupportFlg))
    {
        AT_NORM_LOG("AT_PS_IsNeedPsCallDataSysSwitch: not support Roam data Sys Switch.");

        return VOS_FALSE;
    }

    /* WLAN2CELLULAR�����뼼����֧���л����������л� */
    if ( (TAF_PS_APN_DATA_SYS_CELLULAR == (*penDestDataSys))
      && (0 == (pstApnDataSysPolicyInfo->stDataSysPolicy.ucDataSysSwitchRatPolicy & (0x01 << pstCommPsCtx->astDataSystemStatus[usModemId].enRatType))))
    {
        AT_NORM_LOG("AT_PS_IsNeedPsCallDataSysSwitch: not support Rat data Sys Switch.");

        return VOS_FALSE;
    }

    /* ����̬���������л� */
    if (VOS_FALSE == AT_PS_IsPsCallInSteadyState(pstCallEntity))
    {
        AT_NORM_LOG("AT_PS_IsNeedPsCallDataSysSwitch: doing data Sys switch.");

        return VOS_FALSE;
    }

    /* CELLULAR2WLAN������ģ�鲻�������������л����ȴ�����֪ͨ���� */
    if ( (TAF_PS_APN_DATA_SYS_WLAN == (*penDestDataSys))
      && (VOS_FALSE == AT_PS_IsWlanThrotAllowed(pstCallEntity)))
    {
        AT_NORM_LOG("AT_PS_IsNeedPsCallDataSysSwitch: do not dailup in wlan because of throt.");

        return VOS_FALSE;
    }

    return VOS_TRUE;
}

VOS_UINT32 AT_PS_DialUpInWlanAllowed(
    VOS_UINT8                               ucBitDataSystem,
    TAF_PS_APN_DATA_SYS_POLICY_ENUM_UINT8   enDataSysPolicy
)
{
    VOS_UINT8                               ucWlanAvailableFlg;

    ucWlanAvailableFlg      = VOS_FALSE;

    ucWlanAvailableFlg      = (0x01 & (ucBitDataSystem >> TAFAGENT_DATA_SYSTEM_WLAN));

    /* Wlan������ */
    if (VOS_FALSE == ucWlanAvailableFlg)
    {
        return VOS_FALSE;
    }

    /* ��ѡ���Բ�������Wlan�·��� */
    if ( (TAF_PS_APN_DATA_SYS_POLICY_ONLY_WLAN != enDataSysPolicy)
      && (TAF_PS_APN_DATA_SYS_POLICY_CELLULAR_PREFER != enDataSysPolicy))
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}

VOS_UINT32 AT_PS_DialUpInCellularAllowed(
    VOS_UINT8                               ucBitDataSystem,
    TAF_PS_APN_DATA_SYS_POLICY_ENUM_UINT8   enDataSysPolicy
)
{
    VOS_UINT8                               ucCellularAvailableFlg;

    ucCellularAvailableFlg  = VOS_FALSE;

    ucCellularAvailableFlg  = (0x01 & (ucBitDataSystem >> TAFAGENT_DATA_SYSTEM_CELLULAR));

    /* CELLULAR������ */
    if (VOS_FALSE == ucCellularAvailableFlg)
    {
        return VOS_FALSE;
    }

    /* ��ѡ���Բ�������CELLULAR�·��� */
    if ( (TAF_PS_APN_DATA_SYS_POLICY_CELLULAR_PREFER != enDataSysPolicy)
      && (TAF_PS_APN_DATA_SYS_POLICY_ONLY_CELLULAR != enDataSysPolicy))
    {
        return VOS_FALSE;
    }

    /* ��CELLULAR WLAN�����ã���ѡ����ΪWLAN PREFERʱҲ��������CELLULAR�·��� */

    return VOS_TRUE;
}


VOS_UINT32 AT_PS_IsWlanThrotAllowed(
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity
)
{
    VOS_UINT32                          ulSetupAllowedFlg;

    ulSetupAllowedFlg = VOS_FALSE;

    if (VOS_OK != TAF_AGENT_GetWlanApnThrotAllowed(pstCallEntity->stUserInfo.enUserIndex,
                                                   pstCallEntity->stUsrDialParam.ucAPNLen,
                                                   pstCallEntity->stUsrDialParam.aucAPN,
                                                   &ulSetupAllowedFlg))
    {
        /* ��ѯ���ڲ���ʧ�� */
        AT_ERR_LOG("AT_PS_IsWlanThrotAllowed: Get wlan throt fail.");
        return VOS_FALSE;
    }

    if (VOS_TRUE != ulSetupAllowedFlg)
    {
       /* �������� */
       AT_ERR_LOG("AT_PS_IsWlanThrotAllowed: wlan is throttling.");
       return VOS_FALSE;
    }

    /* ������ */
    return VOS_TRUE;
}


TAF_PDP_TYPE_ENUM_UINT8 AT_PS_ConvertPdpType2Cellular(IMSA_WIFI_IP_TYPE_ENUM_INT8 enWlanPdpType)
{
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType;

    switch (enWlanPdpType)
    {
        case IMSA_WIFI_IP_IPV4:
            enPdpType = TAF_PDP_IPV4;
            break;

        case IMSA_WIFI_IP_IPV6:
            enPdpType = TAF_PDP_IPV6;
            break;

        case IMSA_WIFI_IP_IPV4V6:
            enPdpType = TAF_PDP_IPV4V6;
            break;

        default:
            enPdpType = TAF_PDP_TYPE_BUTT;
            break;
    }

    return enPdpType;
}

IMSA_WIFI_IP_TYPE_ENUM_INT8 AT_PS_ConvertPdpType2Wlan( TAF_PDP_TYPE_ENUM_UINT8 eCellularPdpType)
{
    IMSA_WIFI_IP_TYPE_ENUM_INT8             enPdpType;

    switch (eCellularPdpType)
    {
        case TAF_PDP_IPV4 :
            enPdpType = IMSA_WIFI_IP_IPV4;
            break;

        case TAF_PDP_IPV6 :
            enPdpType = IMSA_WIFI_IP_IPV6;
            break;

        case TAF_PDP_IPV4V6:
            enPdpType = IMSA_WIFI_IP_IPV4V6;
            break;

        default:
            enPdpType = IMSA_WIFI_IP_TYPE_BUTT;
            break;
    }

    return enPdpType;
}


VOS_VOID AT_PS_ReportImsCtrlMsgu(
    VOS_UINT8                                       ucIndex,
    AT_IMS_CTRL_MSG_RECEIVE_MODULE_ENUM_UINT8       enModule,
    VOS_UINT32                                      ulMsgLen,
    VOS_UINT8                                      *pucDst
)
{
    /* ����ֲ����� */
    VOS_UINT16                          usLength;

    usLength = 0;

    usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                       (VOS_CHAR *)pgucAtSndCodeAddr,
                                       (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                       "%s^IMSCTRLMSGU: %d,%d,\"",
                                       gaucAtCrLf,
                                       enModule,
                                       ulMsgLen);

    usLength += (VOS_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN,
                                                    (VOS_CHAR *)pgucAtSndCodeAddr,
                                                    (VOS_UINT8 *)pgucAtSndCodeAddr + usLength,
                                                    pucDst,
                                                    (VOS_UINT16)ulMsgLen);

    usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                       (VOS_CHAR *)pgucAtSndCodeAddr,
                                       (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                       "\"%s",
                                       gaucAtCrLf);

    /* ����At_SendResultData���������� */
    At_SendResultData(ucIndex, pgucAtSndCodeAddr, usLength);

    return;
}


VOS_VOID AT_PS_RegDataSysChgNtf(
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity
)
{
    TAF_PS_APN_DATA_SYS_POLICY_INFO_STRU    *pstApnDataSysPolicyInfo = VOS_NULL_PTR;

    /* ͨ����ѡ�����õ���ѡ���� */
    pstApnDataSysPolicyInfo = AT_PS_GetApnDataSysPolicyInfo(pstCallEntity->stUserInfo.enPortIndex, pstCallEntity->stApnDataSysInfo.ucDataSysPolicyIndex);

    /* ����ֻ�д����л�ҵ���APN��ע�ṳ�� */
    if ( (VOS_TRUE == pstCallEntity->stApnDataSysInfo.ucDataSysInfoFlg)
      && (TAF_PS_APN_DATA_SYS_POLICY_CELLULAR_PREFER == pstApnDataSysPolicyInfo->stDataSysPolicy.enDataSysPolicy))
    {
        (VOS_VOID)TAF_AGENT_RegDataSysChgNtf(pstCallEntity->stUserInfo.enPortIndex, pstCallEntity->stUserInfo.ucUsrCid);
    }

    return;
}


VOS_VOID AT_PS_DeRegDataSysChgNtf(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId
)
{
    AT_PS_CALL_ENTITY_STRU                 *pstCallEntity           = VOS_NULL_PTR;
    TAF_PS_APN_DATA_SYS_POLICY_INFO_STRU   *pstApnDataSysPolicyInfo = VOS_NULL_PTR;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);

    /* ͨ����ѡ�����õ���ѡ���� */
    pstApnDataSysPolicyInfo = AT_PS_GetApnDataSysPolicyInfo(usClientId, pstCallEntity->stApnDataSysInfo.ucDataSysPolicyIndex);

    /* ����ֻ�д����л�ҵ���APN��ע�ṳ�ӣ�����ȥע��Ҳֻ�д����л�ҵ���APN */
    if ( (VOS_TRUE == pstCallEntity->stApnDataSysInfo.ucDataSysInfoFlg)
      && (TAF_PS_APN_DATA_SYS_POLICY_CELLULAR_PREFER == pstApnDataSysPolicyInfo->stDataSysPolicy.enDataSysPolicy))
    {
        (VOS_VOID)TAF_AGENT_DeRegDataSysChgNtf(usClientId, pstCallEntity->stUserInfo.ucUsrCid);
    }

    return;
}


VOS_VOID AT_PS_WlanPdpAddrProc(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    AT_DHCP_CONFIG_STRU                *pstDhcpConfig
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity   = VOS_NULL_PTR;
    AT_DIAL_PARAM_STRU                 *pstDialPara     = VOS_NULL_PTR;
    VOS_UINT32                          ulIpAddr;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);
    pstDialPara   = &pstCallEntity->stUsrDialParam;
    ulIpAddr      = 0;

    pstCallEntity->stIpv4DhcpInfo.ulIpv4Addr              = pstDhcpConfig->ulIPAddr;
    pstCallEntity->stIpv4DhcpInfo.ulIpv4GateWay           = pstDhcpConfig->ulGateWay;
    pstCallEntity->stIpv4DhcpInfo.ulIpv4NetMask           = pstDhcpConfig->ulSubNetMask;

    /* ����û���������DNS����ʹ���û����õ�DNS�����緵�ص�DNS��ʹ�� */
    if (VOS_TRUE == pstDialPara->ulPrimIPv4DNSValidFlag)
    {
        if (AT_FAILURE == AT_LenStr2IpAddr(pstDialPara->aucPrimIPv4DNSAddr,
                                           (VOS_UINT8*)&ulIpAddr))
        {
            ulIpAddr = 0;
        }

        pstCallEntity->stIpv4DhcpInfo.bitOpIpv4PriDns     = VOS_TRUE;
        pstCallEntity->stIpv4DhcpInfo.ulIpv4PrimDNS       = VOS_NTOHL(ulIpAddr);
    }
    else
    {
        if (0 != pstDhcpConfig->ulPrimDNS)
        {
            pstCallEntity->stIpv4DhcpInfo.bitOpIpv4PriDns = VOS_TRUE;
            pstCallEntity->stIpv4DhcpInfo.ulIpv4PrimDNS   = pstDhcpConfig->ulPrimDNS;
        }
        else
        {
            pstCallEntity->stIpv4DhcpInfo.bitOpIpv4PriDns = VOS_FALSE;
        }
    }

    /* ����û������˸�DNS����ʹ���û����õ�DNS�����緵�ص�DNS��ʹ�� */
    if (VOS_TRUE == pstDialPara->ulSndIPv4DNSValidFlag)
    {
        if (AT_FAILURE == AT_LenStr2IpAddr(pstDialPara->aucSndIPv4DNSAddr,
                                           (VOS_UINT8*)&ulIpAddr))
        {
            ulIpAddr = 0;
        }

        pstCallEntity->stIpv4DhcpInfo.bitOpIpv4SecDns     = VOS_TRUE;
        pstCallEntity->stIpv4DhcpInfo.ulIpv4SecDNS        = VOS_NTOHL(ulIpAddr);
    }
    else
    {
        if (0 != pstDhcpConfig->ulSndDNS)
        {
            pstCallEntity->stIpv4DhcpInfo.bitOpIpv4SecDns = VOS_TRUE;
            pstCallEntity->stIpv4DhcpInfo.ulIpv4SecDNS    = pstDhcpConfig->ulSndDNS;
        }
        else
        {
            pstCallEntity->stIpv4DhcpInfo.bitOpIpv4SecDns = VOS_FALSE;
        }
    }

    pstCallEntity->stIpv4DhcpInfo.bitOpIpv4PriWINNS       = VOS_FALSE;
    pstCallEntity->stIpv4DhcpInfo.bitOpIpv4SecWINNS       = VOS_FALSE;

    /* ��ȡ��PCSCF��ַ */
    /* WLAN�µ�PCSCF��ַ�Ȳ����ǣ��ʲ����� */
    pstCallEntity->stIpv4DhcpInfo.stIpv4PcscfList.ucIpv4PcscfAddrNum = 0;

    /* WLAN��usIpv4Mtu��� */
    pstCallEntity->usIpv4Mtu   = 0;

    return;
}


VOS_VOID AT_PS_ProcWlanConnectedIpv4Addr(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    WLAN_AT_PDN_ACTIVATE_CNF_STRU      *pstWlanPdnActivateCnf
)
{
    AT_DHCP_SETUP_PARAM_ST              stParam;
    AT_DHCP_CONFIG_STRU                 stConfig;

    TAF_MEM_SET_S(&stParam, (VOS_UINT32)(sizeof(stParam)), 0x00, (VOS_UINT32)(sizeof(AT_DHCP_SETUP_PARAM_ST)));
    TAF_MEM_SET_S(&stConfig, (VOS_UINT32)(sizeof(stConfig)), 0x00, (VOS_UINT32)(sizeof(AT_DHCP_CONFIG_STRU)));

    /* ���PS�¼���DNS��ַ�����¼DNS��ַ*/
    if (pstWlanPdnActivateCnf->stDns.cOpPriDns)
    {
        stParam.ulPrimDNS = AT_GetLanAddr32(pstWlanPdnActivateCnf->stDns.aucPriDns);
    }

    if (pstWlanPdnActivateCnf->stDns.cOpSecDns)
    {
        stParam.ulSndDNS = AT_GetLanAddr32(pstWlanPdnActivateCnf->stDns.aucSecDns);
    }

    /* ��¼IP��ַ*/
    stParam.ulIPAddr = AT_GetLanAddr32((VOS_UINT8 *)(pstWlanPdnActivateCnf->stPdpAddr.aucIpV4Addr));

    if (VOS_ERR == AT_DHCPServerSetUp(&stParam, &stConfig))
    {
        AT_ERR_LOG("AT_PS_ProcWlanConnectedIpv4Addr : AT_DHCPServerSetUp Error!" );

        return;
    }
    else
    {
        /* ����IPv4��ַ���� */
        AT_PS_WlanPdpAddrProc(usClientId, ucCallId, &stConfig);
    }

    return;
}


VOS_VOID AT_PS_ProcWlanConnectedIpv6Addr(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    WLAN_AT_PDN_ACTIVATE_CNF_STRU      *pstWlanPdnActivateCnf
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);

    /* ����IPv6��IP��ַ����ʽΪ������ */
    TAF_MEM_CPY_S(pstCallEntity->stIpv6DhcpInfo.aucIpv6Addr,
                  (VOS_UINT32)(sizeof(pstCallEntity->stIpv6DhcpInfo.aucIpv6Addr)),
                  pstWlanPdnActivateCnf->stPdpAddr.aucIpV6Addr,
                  TAF_IPV6_ADDR_LEN);

    /* ����IPV6������DNS��ַ����ʽΪ������ */
    pstCallEntity->stIpv6DhcpInfo.bitOpIpv6PriDns    = VOS_FALSE;
    pstCallEntity->stIpv6DhcpInfo.bitOpIpv6SecDns    = VOS_FALSE;

    if (VOS_TRUE == pstWlanPdnActivateCnf->stIPv6Dns.cOpPriDns)
    {
        TAF_MEM_CPY_S((VOS_VOID*)pstCallEntity->stIpv6DhcpInfo.aucIpv6PrimDNS,
                      (VOS_UINT32)(sizeof(pstCallEntity->stIpv6DhcpInfo.aucIpv6PrimDNS)),
                      pstWlanPdnActivateCnf->stIPv6Dns.aucPriDns,
                      IMSA_WIFI_IPV6_ADDR_LEN);
        pstCallEntity->stIpv6DhcpInfo.bitOpIpv6PriDns = VOS_TRUE;
    }

    if (VOS_TRUE == pstWlanPdnActivateCnf->stIPv6Dns.cOpSecDns)
    {
        TAF_MEM_CPY_S((VOS_VOID*)pstCallEntity->stIpv6DhcpInfo.aucIpv6SecDNS,
                      (VOS_UINT32)(sizeof(pstCallEntity->stIpv6DhcpInfo.aucIpv6SecDNS)),
                      pstWlanPdnActivateCnf->stIPv6Dns.aucSecDns,
                      IMSA_WIFI_IPV6_ADDR_LEN);
        pstCallEntity->stIpv6DhcpInfo.bitOpIpv6SecDns = VOS_TRUE;
    }

    /* ����IPV6������PCSCF��ַ����ʽΪ������ */
    /* WLAN�µ�PCSCF��ַ�Ȳ����ǣ��ʲ����� */
    pstCallEntity->stIpv6DhcpInfo.stIpv6PcscfList.ucIpv6PcscfAddrNum = 0;

    /* WLAN��û��RA��Ϣ */
    TAF_MEM_SET_S(&pstCallEntity->stIpv6RaInfo,
                  (VOS_UINT32)(sizeof(pstCallEntity->stIpv6RaInfo)),
                  0x00,
                  (VOS_UINT32)(sizeof(AT_IPV6_RA_INFO_STRU)));

    return;
}


VOS_VOID AT_PS_ProcWlanConnectedIpAddr(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    WLAN_AT_PDN_ACTIVATE_CNF_STRU      *pstWlanPdnActivateCnf
)
{
    /* ����IP���͵�DHCP */
    if ( (TAF_PDP_IPV4 == AT_PS_ConvertPdpType2Cellular(pstWlanPdnActivateCnf->enPdnType))
      || (TAF_PDP_IPV4V6 == AT_PS_ConvertPdpType2Cellular(pstWlanPdnActivateCnf->enPdnType)))
    {
        AT_PS_ProcWlanConnectedIpv4Addr(usClientId, ucCallId, pstWlanPdnActivateCnf);
    }

    if ( (TAF_PDP_IPV6 == AT_PS_ConvertPdpType2Cellular(pstWlanPdnActivateCnf->enPdnType))
      || (TAF_PDP_IPV4V6 == AT_PS_ConvertPdpType2Cellular(pstWlanPdnActivateCnf->enPdnType)))
    {
        AT_PS_ProcWlanConnectedIpv6Addr(usClientId, ucCallId, pstWlanPdnActivateCnf);
    }

    return;
}


VOS_VOID AT_PS_FillHoWlanPdnActivateIpv4Info(
    AT_WLAN_PDN_ACTIVATE_REQ_STRU           *pstPdnActReq,
    AT_PS_CALL_ENTITY_STRU                  *pstCallEntity

)
{
    AT_GetIpv4AddrFromLanAddr32(pstCallEntity->stIpv4DhcpInfo.ulIpv4Addr, (VOS_UINT8 *)(pstPdnActReq->stPdpAddr.aucIpV4Addr));

    if (VOS_TRUE == pstCallEntity->stIpv4DhcpInfo.bitOpIpv4PriDns)
    {
        pstPdnActReq->stDns.cOpPriDns = VOS_TRUE;
        AT_GetIpv4AddrFromLanAddr32(pstCallEntity->stIpv4DhcpInfo.ulIpv4PrimDNS, pstPdnActReq->stDns.aucPriDns);
    }

    if (VOS_TRUE == pstCallEntity->stIpv4DhcpInfo.bitOpIpv4SecDns)
    {
        pstPdnActReq->stDns.cOpSecDns = VOS_TRUE;
        AT_GetIpv4AddrFromLanAddr32(pstCallEntity->stIpv4DhcpInfo.ulIpv4SecDNS, pstPdnActReq->stDns.aucSecDns);
    }

    return;
}


VOS_VOID AT_PS_FillHoWlanPdnActivateIpv6Info(
    AT_WLAN_PDN_ACTIVATE_REQ_STRU           *pstPdnActReq,
    AT_PS_CALL_ENTITY_STRU                  *pstCallEntity

)
{
    TAF_MEM_CPY_S(pstPdnActReq->stPdpAddr.aucIpV6Addr,
                  TAF_IPV6_ADDR_LEN,
                  pstCallEntity->stIpv6DhcpInfo.aucIpv6Addr,
                  TAF_IPV6_ADDR_LEN);

    if (VOS_TRUE == pstCallEntity->stIpv6DhcpInfo.bitOpIpv6PriDns)
    {
        pstPdnActReq->stIPv6Dns.cOpPriDns = VOS_TRUE;

        TAF_MEM_CPY_S(pstPdnActReq->stIPv6Dns.aucPriDns,
                  TAF_IPV6_ADDR_LEN,
                  pstCallEntity->stIpv6DhcpInfo.aucIpv6PrimDNS,
                  TAF_IPV6_ADDR_LEN);
    }

    if (VOS_TRUE == pstCallEntity->stIpv6DhcpInfo.bitOpIpv6SecDns)
    {
        pstPdnActReq->stIPv6Dns.cOpSecDns = VOS_TRUE;

        TAF_MEM_CPY_S(pstPdnActReq->stIPv6Dns.aucSecDns,
                  TAF_IPV6_ADDR_LEN,
                  pstCallEntity->stIpv6DhcpInfo.aucIpv6SecDNS,
                  TAF_IPV6_ADDR_LEN);
    }

    return;
}


VOS_UINT32 AT_PS_SndWlanMsgPdnActivateReq(
    VOS_UINT8                           ucIndex,
    TAF_PDP_TYPE_ENUM_UINT8             enIpType,
    VOS_INT8                            cIsHandover,
    VOS_UINT8                           ucCallId
)
{
    AT_WLAN_PDN_ACTIVATE_REQ_STRU      *pstPdnActReq  = VOS_NULL_PTR;
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity = VOS_NULL_PTR;
    VOS_UINT8                           ucBroadCastIndex;

    pstPdnActReq    = (AT_WLAN_PDN_ACTIVATE_REQ_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(AT_WLAN_PDN_ACTIVATE_REQ_STRU)); //lint !e516

    if (VOS_NULL_PTR == pstPdnActReq)
    {
        AT_ERR_LOG("AT_PS_SndWlanMsgPdnActivateReq: Memory malloc failed!");
        return VOS_ERR;
    }

    pstCallEntity   = AT_PS_GetCallEntity(ucIndex, ucCallId);
    TAF_MEM_SET_S(pstPdnActReq, (VOS_UINT32)(sizeof(AT_WLAN_PDN_ACTIVATE_REQ_STRU)), 0x00, (VOS_UINT32)(sizeof(AT_WLAN_PDN_ACTIVATE_REQ_STRU)));

    pstPdnActReq->lMsgId        = ID_IMSA_WIFI_IMS_PDN_ACTIVATE_REQ;
    pstPdnActReq->enIpType      = AT_PS_ConvertPdpType2Wlan(enIpType);
    pstPdnActReq->cIsHandover   = cIsHandover;
    pstPdnActReq->cServiceType  = AT_PS_CALL_GET_SERVICE_TYPE_FROM_CID(pstCallEntity->stUsrDialParam.ucCid);

    pstPdnActReq->stApn.ucLength    = TAF_MIN(pstCallEntity->stUsrDialParam.ucAPNLen, TAF_MAX_APN_LEN);
    TAF_MEM_CPY_S(pstPdnActReq->stApn.aucValue,
                  (VOS_UINT32)(sizeof(pstPdnActReq->stApn.aucValue)),
                  pstCallEntity->stUsrDialParam.aucAPN,
                  pstPdnActReq->stApn.ucLength);

    if (VOS_TRUE == cIsHandover)
    {
        pstPdnActReq->stPdpAddr.enIpType = AT_PS_ConvertPdpType2Wlan(enIpType);

        if ( (TAF_PDP_IPV4 == enIpType)
          || (TAF_PDP_IPV4V6 == enIpType))
        {
            AT_PS_FillHoWlanPdnActivateIpv4Info(pstPdnActReq, pstCallEntity);
        }

        if ( (TAF_PDP_IPV6 == enIpType)
          || (TAF_PDP_IPV4V6 == enIpType))
        {
            AT_PS_FillHoWlanPdnActivateIpv6Info(pstPdnActReq, pstCallEntity);
        }
    }

    if (AT_SUCCESS == At_ClientIdToUserBroadCastId(ucIndex, &ucBroadCastIndex))
    {
        AT_PS_ReportImsCtrlMsgu(ucBroadCastIndex,
                                AT_IMS_CTRL_MSG_RECEIVE_MODULE_NON_IMSA,
                                (VOS_UINT32)(sizeof(AT_WLAN_PDN_ACTIVATE_REQ_STRU)),
                                (VOS_UINT8 *)pstPdnActReq);
    }

    PS_MEM_FREE(WUEPS_PID_AT, pstPdnActReq); //lint !e516

    return VOS_OK;
}


VOS_UINT32 AT_PS_SndWlanMsgPdnDeActivateReq(
    VOS_UINT8                           ucIndex,
    VOS_INT8                            cIsLocal,
    VOS_INT8                            cIsHandover,
    VOS_UINT8                           ucCallId
)
{
    AT_WLAN_PDN_DEACTIVATE_REQ_STRU    *pstPdnDeActReq     = VOS_NULL_PTR;
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity      = VOS_NULL_PTR;
    VOS_UINT8                           ucBroadCastIndex;

    pstPdnDeActReq  = (AT_WLAN_PDN_DEACTIVATE_REQ_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(AT_WLAN_PDN_DEACTIVATE_REQ_STRU)); //lint !e516
    pstCallEntity   = AT_PS_GetCallEntity(ucIndex, ucCallId);

    if (VOS_NULL_PTR == pstPdnDeActReq)
    {
        AT_ERR_LOG("AT_PS_SndWlanMsgPdnDeActivateReq: Memory malloc failed!");
        return VOS_ERR;
    }

    TAF_MEM_SET_S(pstPdnDeActReq, (VOS_UINT32)(sizeof(AT_WLAN_PDN_DEACTIVATE_REQ_STRU)), 0x00, (VOS_UINT32)(sizeof(AT_WLAN_PDN_DEACTIVATE_REQ_STRU)));

    pstPdnDeActReq->lMsgId        = ID_IMSA_WIFI_IMS_PDN_DEACTIVATE_REQ;
    pstPdnDeActReq->cIsLocal      = cIsLocal;
    pstPdnDeActReq->cIsHandover   = cIsHandover;
    pstPdnDeActReq->cServiceType  = AT_PS_CALL_GET_SERVICE_TYPE_FROM_CID(pstCallEntity->stUsrDialParam.ucCid);

    pstPdnDeActReq->stApn.ucLength    = TAF_MIN(pstCallEntity->stUsrDialParam.ucAPNLen, TAF_MAX_APN_LEN);
    TAF_MEM_SET_S(pstPdnDeActReq->stApn.aucValue,
                  (VOS_UINT32)(sizeof(pstPdnDeActReq->stApn.aucValue)),
                  0x00,
                  (VOS_UINT32)(sizeof(pstPdnDeActReq->stApn.aucValue)));

    TAF_MEM_CPY_S(pstPdnDeActReq->stApn.aucValue,
                  (VOS_UINT32)(sizeof(pstPdnDeActReq->stApn.aucValue)),
                  pstCallEntity->stUsrDialParam.aucAPN,
                  pstPdnDeActReq->stApn.ucLength);

    if (AT_SUCCESS == At_ClientIdToUserBroadCastId(pstCallEntity->stUserInfo.enPortIndex, &ucBroadCastIndex))
    {
        AT_PS_ReportImsCtrlMsgu(ucBroadCastIndex,
                                AT_IMS_CTRL_MSG_RECEIVE_MODULE_NON_IMSA,
                                (VOS_UINT32)(sizeof(AT_WLAN_PDN_DEACTIVATE_REQ_STRU)),
                                (VOS_UINT8 *)pstPdnDeActReq);
    }

    PS_MEM_FREE(WUEPS_PID_AT, pstPdnDeActReq); //lint !e516

    return VOS_OK;
}


VOS_VOID AT_PS_CleanPsCallEntityWhenProcWlanMsg(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    TAF_PS_CAUSE_ENUM_UINT32            enCause
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity;
    TAF_PDP_TYPE_ENUM_UINT8             enWlanPdpType;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);
    enWlanPdpType = pstCallEntity->enWlanPdpType;

    /* ��IP���͵�PDP״̬�л���IDLE */
    AT_PS_SetWlanCallStateByType(usClientId, ucCallId, enWlanPdpType, AT_PDP_STATE_IDLE);

    /* ����Wlan����PDP���� */
    AT_PS_SetWlanCurrCallType(usClientId, ucCallId, TAF_PDP_TYPE_BUTT);

    /* ����IP��Ӧ��CIDΪ��Ч */
    AT_PS_SetCid2CurrCall(usClientId, ucCallId, enWlanPdpType, AT_PS_CALL_INVALID_CID, TAF_PS_APN_DATA_SYS_WLAN);

    /* ������CID������ͨ����ӳ���ϵ */
    AT_CleanAtChdataCfg(usClientId, pstCallEntity->stUserInfo.ucUsrCid, ucCallId, TAF_PS_APN_DATA_SYS_WLAN);

    /* �ͷ�CALLID��CID��ӳ���ϵ */
    AT_PS_FreeCallIdToCid(usClientId, pstCallEntity->stUsrDialParam.ucCid, ucCallId, TAF_PS_APN_DATA_SYS_WLAN);

    /* �ϱ�IP���ӶϿ�ָʾ */
    AT_PS_SndCallEndedResult(usClientId,
                             ucCallId,
                             enWlanPdpType,
                             enCause,
                             TAF_PS_APN_DATA_SYS_WLAN);

    /* �ͷź���ʵ�� */
    AT_PS_FreeCallEntity(usClientId, ucCallId, TAF_PS_APN_DATA_SYS_WLAN);

    return;
}


VOS_VOID AT_PS_ProcWlanDiffPdpTypeConn(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    WLAN_AT_PDN_ACTIVATE_CNF_STRU      *pstWlanPdnActivateCnf
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity;
    TAF_PDP_TYPE_ENUM_UINT8             enWlanPdpType;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);
    enWlanPdpType = AT_PS_ConvertPdpType2Cellular(pstWlanPdnActivateCnf->enPdnType);

    AT_NORM_LOG2("AT_PS_ProcWlanDiffPdpTypeConn: enWlanPdpType and pstCallEntity->enWlanPdpType is ", enWlanPdpType, pstCallEntity->enWlanPdpType);

    /* ����ֻ����������IPV4V6���ظ��ɹ������ǵ�ջ�����������������ΪIPV4���ظ�IPV6�ɹ���������IPV6���ظ�IPV4�ɹ������ */
    if ( (enWlanPdpType != pstCallEntity->enWlanPdpType)
      && (TAF_PDP_IPV4V6 == pstCallEntity->enWlanPdpType))
    {
        if (TAF_PDP_IPV4 == enWlanPdpType)
        {
            /* ����IP��Ӧ��CIDΪ��Ч */
            AT_PS_SetCid2CurrCall(usClientId, ucCallId, TAF_PDP_IPV6, AT_PS_CALL_INVALID_CID, TAF_PS_APN_DATA_SYS_WLAN);

            /* ��IPV6���͵�PDP״̬�л���IDLE */
            AT_PS_SetWlanCallStateByType(usClientId, ucCallId, TAF_PDP_IPV6, AT_PDP_STATE_IDLE);

            /* ��¼PS����д����� */
            AT_PS_SetPsCallErrCause(usClientId, TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_IPV6_NOT_ALLOWED);

            /* �ϱ�IPV6����ʧ�� */
            AT_PS_SndCallEndedResult(usClientId,
                                     ucCallId,
                                     TAF_PDP_IPV6,
                                     TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_IPV6_NOT_ALLOWED,
                                     TAF_PS_APN_DATA_SYS_WLAN);
        }

        if (TAF_PDP_IPV6 == enWlanPdpType)
        {
            /* ����IP��Ӧ��CIDΪ��Ч */
            AT_PS_SetCid2CurrCall(usClientId, ucCallId, TAF_PDP_IPV4, AT_PS_CALL_INVALID_CID, TAF_PS_APN_DATA_SYS_WLAN);

            /* ��IPV4���͵�PDP״̬�л���IDLE */
            AT_PS_SetWlanCallStateByType(usClientId, ucCallId, TAF_PDP_IPV4, AT_PDP_STATE_IDLE);

            /* ��¼PS����д����� */
            AT_PS_SetPsCallErrCause(usClientId, TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_IPV4_NOT_ALLOWED);

            /* �ϱ�IPV4����ʧ�� */
            AT_PS_SndCallEndedResult(usClientId,
                                     ucCallId,
                                     TAF_PDP_IPV4,
                                     TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_IPV4_NOT_ALLOWED,
                                     TAF_PS_APN_DATA_SYS_WLAN);
        }

        /* ����WLAN�µ�PDN IP���� */
        AT_PS_SetWlanCurrCallType(usClientId, ucCallId, enWlanPdpType);
    }

    return;
}



VOS_VOID AT_PS_ProcWlanCallConnected(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    WLAN_AT_PDN_ACTIVATE_CNF_STRU      *pstWlanPdnActivateCnf
)
{
    AT_PS_CALL_ENTITY_STRU                 *pstCallEntity           = VOS_NULL_PTR;
    TAF_PS_APN_DATA_SYS_POLICY_INFO_STRU   *pstApnDataSysPolicyInfo = VOS_NULL_PTR;
    VOS_UINT32                              ulTmrName;
    VOS_UINT32                              ulTmrParam;

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);

    /* ͨ����ѡ�����õ���ѡ���� */
    pstApnDataSysPolicyInfo = AT_PS_GetApnDataSysPolicyInfo(usClientId, pstCallEntity->stApnDataSysInfo.ucDataSysPolicyIndex);

    if (VOS_FALSE == AT_PS_GetPsCallHandOverFlg(usClientId, ucCallId))
    {
        /* ����IP����PDP״̬�л�������״̬ */
        AT_PS_SetWlanCallStateByType(usClientId, ucCallId, AT_PS_ConvertPdpType2Cellular(pstWlanPdnActivateCnf->enPdnType), AT_PDP_STATE_ACTED);

        /* ����IP���͵�DHCP */
        AT_PS_ProcWlanConnectedIpAddr(usClientId, ucCallId, pstWlanPdnActivateCnf);

        /* �ϱ�IP����״̬ */
        AT_PS_SndCallConnectedResult(usClientId, ucCallId, AT_PS_ConvertPdpType2Cellular(pstWlanPdnActivateCnf->enPdnType));

        /* ����ֻ����������IPV4V6���ظ��ɹ������ǵ�ջ�����������������ΪIPV4���ظ�IPV6�ɹ���������IPV6���ظ�IPV4�ɹ������ */
        AT_PS_ProcWlanDiffPdpTypeConn(usClientId, ucCallId, pstWlanPdnActivateCnf);

        /* ��ָ��CID��PDP�ļ���״̬����Ϊ����̬ */
        AT_SetAtChdataCidActStatus(usClientId, pstCallEntity->stUserInfo.ucUsrCid, VOS_TRUE);

        /* ע������ϵͳ��ı�֪ͨ */
        AT_PS_RegDataSysChgNtf(pstCallEntity);
    }
    else
    {
        /* ���IP��ַ�ޱ仯 */
        if (VOS_TRUE == AT_PS_CmpIpv4v6AddrHoWlanConn(pstCallEntity, pstWlanPdnActivateCnf))
        {
            AT_PS_SetCallHandOverFlg(usClientId, ucCallId, VOS_FALSE);

            /* ����handover�������� */
            AT_PS_SetHoCallType(usClientId, ucCallId, TAF_PDP_TYPE_BUTT);

            /* ����IP���͵�DHCP */
            AT_PS_ProcWlanConnectedIpAddr(usClientId, ucCallId, pstWlanPdnActivateCnf);

            /* ����IP����PDP״̬�л�������״̬ */
            AT_PS_SetWlanCallStateByType(usClientId, ucCallId, AT_PS_ConvertPdpType2Cellular(pstWlanPdnActivateCnf->enPdnType), AT_PDP_STATE_ACTED);

            /* ��CELLULAR�·��𱾵�ȥ���� */
            (VOS_VOID)AT_PS_HangupCall(AT_PS_GetUserInfo(usClientId, ucCallId)->enUserIndex, ucCallId, TAF_PS_CALL_END_CAUSE_LOCAL);

            /* ������ƹ�Ҷ�ʱ�� */
            AT_SET_PROTECT_PDN_IN_DATA_SYS_TMR_NAME(ulTmrName);
            AT_SET_PROTECT_PDN_IN_DATA_SYS_TMR_PARAM(ulTmrParam, usClientId, ucCallId);

            AT_StartRelTimer(&(pstCallEntity->stPsCallTimerInfo.hProtectInDataSysTmrHdl),
                             pstApnDataSysPolicyInfo->stDataSysPolicy.usWlanOutProtectTimeLen * 1000,
                             ulTmrName,
                             ulTmrParam,
                             VOS_RELTIMER_NOLOOP);
        }
        else
        {
            /* ��¼IP��ַ�ı�Ĵ����� */
            AT_PS_SetPsCallErrCause(usClientId, TAF_PS_CAUSE_WLAN_PDN_ACT_RESULT_IP_ADDRESS_CHG_IN_HANDOVER);

            AT_PS_SetCallHandOverFlg(usClientId, ucCallId, VOS_FALSE);

            /* ����handover�������� */
            AT_PS_SetHoCallType(usClientId, ucCallId, TAF_PDP_TYPE_BUTT);

            /* ����CELLULAR����ȥ���� */
            (VOS_VOID)AT_PS_HangupCall(AT_PS_GetUserInfo(usClientId, ucCallId)->enUserIndex, ucCallId, TAF_PS_CALL_END_CAUSE_LOCAL);

            /* ����ֻ����������IPV4V6���ظ��ɹ������ǵ�ջ�����������������ΪIPV4���ظ�IPV6�ɹ���������IPV6���ظ�IPV4�ɹ������ */
            /* �ò����������CELLULARȥ���������WLANȥ�������ǰ */
            AT_PS_ProcWlanDiffPdpTypeConn(usClientId, ucCallId, pstWlanPdnActivateCnf);

            AT_PS_SetCallHandOverFailCause(usClientId, ucCallId, TAF_PS_CAUSE_IP_ADDRESS_CHG_IN_HANDOVER);

            /* IP��ַ�б仯����Ҫִ��WLAN��ȥ���� */
            (VOS_VOID)AT_PS_HangupWlanCall(usClientId, ucCallId, VOS_FALSE, VOS_FALSE);
        }
    }

    return;
}


VOS_VOID AT_PS_ProcWlanCallReject(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCallId,
    WLAN_AT_PDN_ACTIVATE_CNF_STRU      *pstWlanPdnActivateCnf
)
{
    /* ��¼PS����д����� */
    AT_PS_SetPsCallErrCause(usClientId, AT_PS_MapWlanActCnfCause(pstWlanPdnActivateCnf->enCause));

    if (VOS_TRUE == AT_PS_GetPsCallHandOverFlg(usClientId, ucCallId))
    {
        AT_PS_SetCallHandOverFailCause(usClientId, ucCallId, AT_PS_GetPsCallErrCause(usClientId));

        AT_PS_SetCallHandOverFlg(usClientId, ucCallId, VOS_FALSE);

        /* ����handover�������� */
        AT_PS_SetHoCallType(usClientId, ucCallId, TAF_PDP_TYPE_BUTT);

        /* ����CELLULAR����ȥ���� */
        (VOS_VOID)AT_PS_HangupCall(AT_PS_GetUserInfo(usClientId, ucCallId)->enUserIndex, ucCallId, TAF_PS_CALL_END_CAUSE_LOCAL);
    }

    /* CELLULAR�·��𱾵�ȥ�����������ȫ������ */
    AT_PS_CleanPsCallEntityWhenProcWlanMsg(usClientId, ucCallId, AT_PS_GetPsCallErrCause(usClientId));

    return;
}


VOS_VOID AT_PS_ProcWlanMsgPdnActivateCnf (
    VOS_UINT16                          usClientId,
    WLAN_AT_PDN_ACTIVATE_CNF_STRU      *pstWlanPdnActivateCnf
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity;
    VOS_UINT32                          ulTmrName;
    VOS_UINT8                           ucCallId;


    if (!AT_PS_IS_SERVICE_TYPE_VALID(pstWlanPdnActivateCnf->cServiceType))
    {
        AT_ERR_LOG("AT_PS_ProcWlanMsgPdnActivateCnf: cServiceType is invalid!");
        return;
    }

    ucCallId = AT_PS_TransCidToCallId(usClientId, AT_PS_CALL_GET_CID_FROM_SERVICE_TYPE(pstWlanPdnActivateCnf->cServiceType));

    if (VOS_FALSE == AT_PS_IsCallIdValid(usClientId, ucCallId))
    {
        AT_ERR_LOG("AT_PS_ProcWlanMsgPdnActivateCnf: CallId is invalid!");
        return;
    }

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);
    AT_SET_WLAN_ACT_PDN_CNF_TMR_NAME(ulTmrName);

    if (VOS_NULL_PTR != pstCallEntity->stPsCallTimerInfo.hWaitWlanActCnfTmrHdl)
    {
        AT_StopRelTimer(ulTmrName, &(pstCallEntity->stPsCallTimerInfo.hWaitWlanActCnfTmrHdl));
    }

    AT_NORM_LOG2("AT_PS_ProcWlanMsgPdnActivateCnf: enCause and lNetworkReason is ", pstWlanPdnActivateCnf->enCause, pstWlanPdnActivateCnf->lNetworkReason);

    if (WIFI_IMSA_PDN_ACT_RESULT_SUCCESS == pstWlanPdnActivateCnf->enCause)
    {
        /* ����WLAN��PDP����ɹ��¼� */
        AT_PS_ProcWlanCallConnected(usClientId, ucCallId, pstWlanPdnActivateCnf);
    }
    else
    {
        /* ����WLAN��PDP������¼� */
        AT_PS_ProcWlanCallReject(usClientId, ucCallId, pstWlanPdnActivateCnf);
    }

    TAF_AGENT_FeedbackWlanApnThrotResult(usClientId,
                                         pstCallEntity->stUsrDialParam.ucAPNLen,
                                         pstCallEntity->stUsrDialParam.aucAPN,
                                         (VOS_UINT32)(pstWlanPdnActivateCnf->enCause), /*lint !e571*/
                                         pstWlanPdnActivateCnf->lNetworkReason);

    return;
}


VOS_VOID AT_PS_ProcWlanMsgPdnDeactiveCnf (
    VOS_UINT16                          usClientId,
    WLAN_AT_PDN_DEACTIVATE_CNF_STRU    *pstWlanPdnDeActivateCnf
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity;
    VOS_UINT32                          ulTmrName;
    VOS_UINT8                           ucCallId;

    if (!AT_PS_IS_SERVICE_TYPE_VALID(pstWlanPdnDeActivateCnf->cServiceType))
    {
        AT_ERR_LOG("AT_PS_ProcWlanMsgPdnDeactiveCnf: cServiceType is invalid!");
        return;
    }

    ucCallId = AT_PS_TransCidToCallId(usClientId, AT_PS_CALL_GET_CID_FROM_SERVICE_TYPE(pstWlanPdnDeActivateCnf->cServiceType));

    if (VOS_FALSE == AT_PS_IsCallIdValid(usClientId, ucCallId))
    {
        AT_ERR_LOG("AT_PS_ProcWlanMsgPdnDeactiveCnf: CallId is invalid!");
        return;
    }

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);
    AT_SET_WLAN_DEACT_PDN_CNF_TMR_NAME(ulTmrName);

    if (VOS_NULL_PTR != pstCallEntity->stPsCallTimerInfo.hWaitWlanDeActCnfTmrHdl)
    {
        AT_StopRelTimer(ulTmrName, &(pstCallEntity->stPsCallTimerInfo.hWaitWlanDeActCnfTmrHdl));
    }

    AT_PS_CleanPsCallEntityWhenProcWlanMsg(usClientId, ucCallId, TAF_PS_CAUSE_SUCCESS);

    return;
}


VOS_VOID AT_PS_ProcWlanMsgPdnDeactiveInd (
    VOS_UINT16                          usClientId,
    WLAN_AT_PDN_DEACTIVATE_IND_STRU    *pstWlanPdnDeActivateInd
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity;
    VOS_UINT32                          ulTmrName;
    VOS_UINT8                           ucCallId;

    if (!AT_PS_IS_SERVICE_TYPE_VALID(pstWlanPdnDeActivateInd->cServiceType))
    {
        AT_ERR_LOG("AT_PS_ProcWlanMsgPdnDeactiveInd: cServiceType is invalid!");
        return;
    }

    ucCallId = AT_PS_TransCidToCallId(usClientId, AT_PS_CALL_GET_CID_FROM_SERVICE_TYPE(pstWlanPdnDeActivateInd->cServiceType));

    if (VOS_FALSE == AT_PS_IsCallIdValid(usClientId, ucCallId))
    {
        AT_ERR_LOG("AT_PS_ProcWlanMsgPdnDeactiveInd: CallId is invalid!");
        return;
    }

    pstCallEntity = AT_PS_GetCallEntity(usClientId, ucCallId);

    /* ֹͣ����CNF��ʱ�� */
    AT_SET_WLAN_ACT_PDN_CNF_TMR_NAME(ulTmrName);

    if (VOS_NULL_PTR != pstCallEntity->stPsCallTimerInfo.hWaitWlanActCnfTmrHdl)
    {
        AT_StopRelTimer(ulTmrName, &(pstCallEntity->stPsCallTimerInfo.hWaitWlanActCnfTmrHdl));
    }

    /* ֹͣȥ����CNF��ʱ�� */
    AT_SET_WLAN_DEACT_PDN_CNF_TMR_NAME(ulTmrName);

    if (VOS_NULL_PTR != pstCallEntity->stPsCallTimerInfo.hWaitWlanDeActCnfTmrHdl)
    {
        AT_StopRelTimer(ulTmrName, &(pstCallEntity->stPsCallTimerInfo.hWaitWlanDeActCnfTmrHdl));
    }

    if (VOS_FALSE == AT_PS_GetPsCallHandOverFlg(usClientId, ucCallId))
    {
        /* ��¼PS����д����� */
        AT_PS_SetPsCallErrCause(usClientId, AT_PS_MapWlanDeActIndCause(pstWlanPdnDeActivateInd->enCause));
    }

    AT_PS_CleanPsCallEntityWhenProcWlanMsg(usClientId, ucCallId, AT_PS_GetPsCallErrCause(usClientId));

    return;
}


VOS_UINT32 AT_ProcMapconMsg (
    VOS_UINT16                          usClientId,
    AT_MAPCON_CTRL_MSG_STRU            *pstAtMapConMsgPara
)
{
    VOS_UINT32                         *pulMsgType;
    VOS_UINT32                          ulResult;

    pulMsgType  = (VOS_UINT32 *)(pstAtMapConMsgPara->ucMsgContext);
    ulResult    = VOS_OK;

    AT_NORM_LOG1("AT_ProcMapConMsg: Msg Type is ", (*pulMsgType));

    switch (*pulMsgType)
    {
        case ID_WIFI_IMSA_IMS_PDN_ACTIVATE_CNF:
            AT_PS_ProcWlanMsgPdnActivateCnf(usClientId, (WLAN_AT_PDN_ACTIVATE_CNF_STRU*)&pstAtMapConMsgPara->ucMsgContext[0]);
            break;

        case ID_WIFI_IMSA_IMS_PDN_DEACTIVATE_CNF:
            AT_PS_ProcWlanMsgPdnDeactiveCnf(usClientId, (WLAN_AT_PDN_DEACTIVATE_CNF_STRU*)&pstAtMapConMsgPara->ucMsgContext[0]);
            break;

        case ID_WIFI_IMSA_IMS_PDN_DEACTIVATE_IND:
            AT_PS_ProcWlanMsgPdnDeactiveInd(usClientId, (WLAN_AT_PDN_DEACTIVATE_IND_STRU*)&pstAtMapConMsgPara->ucMsgContext[0]);
            break;

        default:
            AT_ERR_LOG("AT_ProcMapConMsg: Msg Type is error!");
            ulResult = VOS_ERR;
            break;
    }

    return ulResult;
}


VOS_VOID AT_PS_HoWlanCall(
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity,
    VOS_UINT8                           ucCallId
)
{
    AT_DIAL_PARAM_STRU                  stUsrDialParam;
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType;

    TAF_MEM_CPY_S(&stUsrDialParam, (VOS_UINT32)(sizeof(stUsrDialParam)), &(pstCallEntity->stUsrDialParam), (VOS_UINT32)(sizeof(pstCallEntity->stUsrDialParam)));
    enPdpType = AT_PS_GetHoPdpIpType(pstCallEntity);

    /* ������wlan�½�������(handover) */
    AT_PS_SetCallHandOverFlg(pstCallEntity->stUserInfo.enPortIndex, ucCallId, VOS_TRUE);

    /* ����handover�������� */
    AT_PS_SetHoCallType(pstCallEntity->stUserInfo.enPortIndex, ucCallId, enPdpType);

    stUsrDialParam.enPdpType = enPdpType;

    AT_NORM_LOG1("AT_PS_HoWlanCall: act pdp in wlan Pdp Type is ", enPdpType);

    if (VOS_OK != AT_PS_SetupWlanCall(&stUsrDialParam, pstCallEntity->stUserInfo.enPortIndex, ucCallId))
    {
        /* �л�ʧ��, ��Ҫ��CELLULAR��ִ�б���ȥ������� */
        AT_PS_SetCallHandOverFlg(pstCallEntity->stUserInfo.enPortIndex, ucCallId, VOS_FALSE);

        AT_PS_SetCallHandOverFailCause(pstCallEntity->stUserInfo.enPortIndex, ucCallId, TAF_PS_CAUSE_UNKNOWN);

        /* ����handover�������� */
        AT_PS_SetHoCallType(pstCallEntity->stUserInfo.enPortIndex, ucCallId, TAF_PDP_TYPE_BUTT);

        AT_PS_SetPsCallErrCause(pstCallEntity->stUserInfo.enPortIndex, TAF_PS_CAUSE_UNKNOWN);

        (VOS_VOID)AT_PS_HangupCall(pstCallEntity->stUserInfo.enUserIndex, ucCallId, TAF_PS_CALL_END_CAUSE_LOCAL);
    }

    return;
}


VOS_VOID AT_PS_HoCellularCall(
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity,
    VOS_UINT8                           ucCallId
)
{
    AT_DIAL_PARAM_STRU                  stUsrDialParam;
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType;

    TAF_MEM_CPY_S(&stUsrDialParam,
                  (VOS_UINT32)(sizeof(stUsrDialParam)),
                  &pstCallEntity->stUsrDialParam,
                  (VOS_UINT32)(sizeof(pstCallEntity->stUsrDialParam)));

    enPdpType = AT_PS_GetHoPdpIpType(pstCallEntity);

    /* ������cellular�½�������(handover) */
    AT_PS_SetCallHandOverFlg(pstCallEntity->stUserInfo.enPortIndex, ucCallId, VOS_TRUE);

    /* ����handover�������� */
    AT_PS_SetHoCallType(pstCallEntity->stUserInfo.enPortIndex, ucCallId, enPdpType);

    stUsrDialParam.enPdpType = enPdpType;

    AT_NORM_LOG1("AT_PS_HoCellularCall: act pdp in cellular Pdp Type is ", enPdpType);

    if (VOS_OK != AT_PS_SetupCall(pstCallEntity->stUserInfo.enUserIndex, ucCallId, &stUsrDialParam))
    {
        /* �л�ʧ��, ��Ҫ��WLAN��ִ�б���ȥ������� */
        AT_PS_SetCallHandOverFlg(pstCallEntity->stUserInfo.enPortIndex, ucCallId, VOS_FALSE);

        AT_PS_SetCallHandOverFailCause(pstCallEntity->stUserInfo.enPortIndex, ucCallId, TAF_PS_CAUSE_UNKNOWN);

        /* ����handover�������� */
        AT_PS_SetHoCallType(pstCallEntity->stUserInfo.enPortIndex, ucCallId, TAF_PDP_TYPE_BUTT);

        AT_PS_SetPsCallErrCause(pstCallEntity->stUserInfo.enPortIndex, TAF_PS_CAUSE_UNKNOWN);

        (VOS_VOID)AT_PS_HangupWlanCall(pstCallEntity->stUserInfo.enUserIndex, ucCallId, VOS_TRUE, VOS_TRUE);
    }

    return;
}


VOS_VOID AT_PS_HoCall(
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity,
    VOS_UINT8                           ucCallId,
    TAF_PS_APN_DATA_SYS_ENUM_UINT8      enDestDataSys
)
{
    switch(enDestDataSys)
    {
        case TAF_PS_APN_DATA_SYS_WLAN:
            AT_PS_HoWlanCall(pstCallEntity, ucCallId);
            break;

        case TAF_PS_APN_DATA_SYS_CELLULAR:
            AT_PS_HoCellularCall(pstCallEntity, ucCallId);
            break;

        default:
            /* nothing to do */
            AT_ERR_LOG1("AT_PS_HoCall: enDestDataSys is error, is !", enDestDataSys);
            break;
    }

    return;
}


VOS_VOID AT_PS_ProcWlanPowerOff (
    VOS_UINT16                          usClientId
)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity;
    WLAN_AT_PDN_DEACTIVATE_IND_STRU     stWlanPdnDeActivateInd;
    VOS_UINT32                          i;

    for (i = 0; i < AT_PS_MAX_CALL_NUM; i++)
    {
        pstCallEntity = AT_PS_GetCallEntity(usClientId, (VOS_UINT8)i);

        TAF_MEM_SET_S(&stWlanPdnDeActivateInd,
                      (VOS_UINT32)(sizeof(stWlanPdnDeActivateInd)),
                      0x00,
                      (VOS_UINT32)(sizeof(WLAN_AT_PDN_DEACTIVATE_IND_STRU)));

        if ( (VOS_TRUE == pstCallEntity->ulUsedFlg)
          && ( (AT_PDP_STATE_IDLE != pstCallEntity->enWlanIpv4State)
            || (AT_PDP_STATE_IDLE != pstCallEntity->enWlanIpv6State)))
        {
            /* ��wlan�ϱ�DeactiveInd���� */
            stWlanPdnDeActivateInd.cServiceType     = AT_PS_CALL_GET_SERVICE_TYPE_FROM_CID(pstCallEntity->stUsrDialParam.ucCid);
            stWlanPdnDeActivateInd.enCause          = WIFI_IMSA_PDN_DEACT_CAUSE_MAPCON_TEAR;
            stWlanPdnDeActivateInd.stApn.ucLength   = TAF_MIN(pstCallEntity->stUsrDialParam.ucAPNLen, TAF_MAX_APN_LEN);

            TAF_MEM_CPY_S(stWlanPdnDeActivateInd.stApn.aucValue,
                          (VOS_UINT32)(sizeof(stWlanPdnDeActivateInd.stApn.aucValue)),
                          pstCallEntity->stUsrDialParam.aucAPN,
                          (VOS_UINT32)(stWlanPdnDeActivateInd.stApn.ucLength));

            AT_PS_ProcWlanMsgPdnDeactiveInd(usClientId, &stWlanPdnDeActivateInd);
        }
    }

    return;
}


VOS_VOID AT_PS_PreProcPsCallDataSysChgNtf(
    VOS_UINT16                   usClientId,
    VOS_UINT32                   ulBitDataSysSwitchCid
)
{
    AT_MODEM_PS_CTX_STRU               *pstPsModemCtx = VOS_NULL_PTR;
    VOS_UINT32                          i;
    TAF_PS_APN_DATA_SYS_ENUM_UINT8      enDestDataSys;

    pstPsModemCtx   = AT_GetModemPsCtxAddrFromClientId(usClientId);
    enDestDataSys   = TAF_PS_APN_DATA_SYS_NONE;

    for (i = 0; i < AT_PS_MAX_CALL_NUM; i++)
    {
        if ( (0 != ((0x01 << pstPsModemCtx->astCallEntity[i].stUsrDialParam.ucCid) & ulBitDataSysSwitchCid))
          && (VOS_TRUE == pstPsModemCtx->astCallEntity[i].ulUsedFlg)
          && (VOS_TRUE == pstPsModemCtx->astCallEntity[i].stApnDataSysInfo.ucDataSysInfoFlg))
        {
            if (VOS_TRUE == AT_PS_IsNeedPsCallDataSysSwitch(usClientId, &pstPsModemCtx->astCallEntity[i], &enDestDataSys))
            {
                /* �����л�������ִ���л� */
                AT_PS_HoCall(&pstPsModemCtx->astCallEntity[i], (VOS_UINT8)i, enDestDataSys);
            }
            else
            {
                /* �������л����������κδ��� */
                /* nothing to do */
            }
        }
    }

    return;
}

VOS_VOID AT_RcvTiWlanActPdnCnfExpired(REL_TIMER_MSG *pstTmrMsg)
{
    WLAN_AT_PDN_ACTIVATE_CNF_STRU       stWlanPdnActivateCnf;
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity   = VOS_NULL_PTR;
    VOS_UINT8                           ucCallId;
    VOS_UINT8                           ucIndex;

    ucCallId       = AT_GET_WLAN_ACT_PDN_CNF_CALLID_FROM_TMR_PARAM(pstTmrMsg->ulPara);
    ucIndex        = AT_GET_WLAN_ACT_PDN_CNF_CLIENTID_FROM_TMR_PARAM(pstTmrMsg->ulPara);

    if (VOS_FALSE == AT_PS_IsCallIdValid(ucIndex, ucCallId))
    {
        AT_ERR_LOG("AT_RcvTiWlanActPdnCnfExpired: CallId is invalid!");
        return;
    }

    pstCallEntity   = AT_PS_GetCallEntity(ucIndex, ucCallId);

    TAF_MEM_SET_S(&stWlanPdnActivateCnf, (VOS_UINT32)(sizeof(stWlanPdnActivateCnf)), 0x00, (VOS_UINT32)(sizeof(WLAN_AT_PDN_ACTIVATE_CNF_STRU)));

    stWlanPdnActivateCnf.enCause    = WIFI_IMSA_PDN_ACT_RESULT_TIMEOUT;
    stWlanPdnActivateCnf.enPdnType  = AT_PS_ConvertPdpType2Wlan(pstCallEntity->enWlanPdpType);

    AT_PS_ProcWlanCallReject(ucIndex, ucCallId, &stWlanPdnActivateCnf);

    TAF_AGENT_FeedbackWlanApnThrotResult(ucIndex,
                                         pstCallEntity->stUsrDialParam.ucAPNLen,
                                         pstCallEntity->stUsrDialParam.aucAPN,
                                         WIFI_IMSA_PDN_ACT_RESULT_TIMEOUT,
                                         0);

    return;
}


VOS_VOID AT_RcvTiWlanDeActPdnCnfExpired(REL_TIMER_MSG *pstTmrMsg)
{
    WLAN_AT_PDN_DEACTIVATE_CNF_STRU     stWlanPdnDeActivateCnf;
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity   = VOS_NULL_PTR;
    VOS_UINT8                           ucCallId;
    VOS_UINT8                           ucIndex;

    ucCallId       = AT_GET_WLAN_DEACT_PDN_CNF_CALLID_FROM_TMR_PARAM(pstTmrMsg->ulPara);
    ucIndex        = AT_GET_WLAN_DEACT_PDN_CNF_CLIENTID_FROM_TMR_PARAM(pstTmrMsg->ulPara);

    if (VOS_FALSE == AT_PS_IsCallIdValid(ucIndex, ucCallId))
    {
        AT_ERR_LOG("AT_RcvTiWlanDeActPdnCnfExpired: CallId is invalid!");
        return;
    }

    pstCallEntity   = AT_PS_GetCallEntity(ucIndex, ucCallId);

    TAF_MEM_SET_S(&stWlanPdnDeActivateCnf, (VOS_UINT32)(sizeof(stWlanPdnDeActivateCnf)), 0x00, (VOS_UINT32)(sizeof(WLAN_AT_PDN_DEACTIVATE_CNF_STRU)));

    stWlanPdnDeActivateCnf.cServiceType    = AT_PS_CALL_GET_SERVICE_TYPE_FROM_CID(pstCallEntity->stUsrDialParam.ucCid);

    AT_PS_ProcWlanMsgPdnDeactiveCnf(ucIndex, &stWlanPdnDeActivateCnf);

    return;
}


VOS_VOID AT_RcvTiProtectPdnInDataSysExpired(REL_TIMER_MSG *pstTmrMsg)
{
    AT_PS_CALL_ENTITY_STRU             *pstCallEntity   = VOS_NULL_PTR;
    VOS_UINT32                          ulBitDataSysSwitchCid;
    VOS_UINT8                           ucCallId;
    VOS_UINT8                           ucIndex;

    ucCallId       = AT_GET_PROTECT_PDN_IN_DATA_SYS_CALLID_FROM_TMR_PARAM(pstTmrMsg->ulPara);
    ucIndex        = AT_GET_PROTECT_PDN_IN_DATA_SYS_CLIENTID_FROM_TMR_PARAM(pstTmrMsg->ulPara);

    if (VOS_FALSE == AT_PS_IsCallIdValid(ucIndex, ucCallId))
    {
        AT_ERR_LOG("AT_RcvTiProtectPdnInDataSysExpired: CallId is invalid!");
        return;
    }

    pstCallEntity   = AT_PS_GetCallEntity(ucIndex, ucCallId);

    /* ִ���л��߼� */
    ulBitDataSysSwitchCid = 0x01 << pstCallEntity->stUsrDialParam.ucCid;
    AT_PS_PreProcPsCallDataSysChgNtf(ucIndex, ulBitDataSysSwitchCid);

    return;
}




