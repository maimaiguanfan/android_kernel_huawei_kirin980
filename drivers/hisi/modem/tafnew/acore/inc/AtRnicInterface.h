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

#ifndef __AT_RNIC_INTERFACE_H__
#define __AT_RNIC_INTERFACE_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/

#include "vos.h"
#include "rnic_dev_def.h"


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

#if (VOS_OS_VER == VOS_WIN32)
/* ����ST���������IP���ݰ����ȣ���λ�ֽ� */
#define RNIC_IP_DATA_LEN                (12)
#endif

/* APP���ź�Ĭ�Ͽ���ʱ�� */
#define AT_APP_IDLE_TIME                (600)

/* ���IPV6��ַ���ȣ�������":" */
#define RNICITF_MAX_IPV6_ADDR_LEN       (16)

#define RNIC_RMNET_NAME_MAX_LEN         (16)


/*******************************************************************************
  3 ö�ٶ���
*******************************************************************************/

/*****************************************************************************
 ö������: AT_RNIC_USB_TETHER_CONN_ENUM
 �ṹ˵��: USB Tethering����״̬
*****************************************************************************/
enum AT_RNIC_USB_TETHER_CONN_ENUM
{
    AT_RNIC_USB_TETHER_DISCONNECT       = 0x00,                                 /* USB tetheringδ���� */
    AT_RNIC_USB_TETHER_CONNECTED        = 0x01,                                 /* USB tethering������ */
    AT_RNIC_USB_TETHER_CONN_BUTT
};
typedef VOS_UINT8 AT_RNIC_USB_TETHER_CONN_ENUM_UINT8;

/*****************************************************************************
 ö������: AT_RNIC_MSG_ID_ENUM
 ö��˵��: AT��RNIC����Ϣ����
*****************************************************************************/
enum AT_RNIC_MSG_ID_ENUM
{
    /* AT����RNIC����Ϣö�� */
    /* 0x0001, 0x0003 ~ 0x0006, 0x0008 ɾ�� */
    ID_AT_RNIC_DIAL_MODE_REQ            = 0x0002,   /* ����ģʽ��ѯ */
    ID_AT_RNIC_DSFLOW_IND               = 0x0007,   /* �����ϱ�ָʾ */
    ID_AT_RNIC_PDN_INFO_CFG_IND         = 0x0009,
    ID_AT_RNIC_PDN_INFO_REL_IND         = 0x000A,
    ID_AT_RNIC_USB_TETHER_INFO_IND      = 0x000B,   /* USB Tethering��Ϣָʾ */

    /* RNIC����AT����Ϣö�� */
    /* 0x1001 ɾ�� */
    ID_RNIC_AT_DSFLOW_RSP               = 0x1002,   /* �����ظ� */
    ID_RNIC_AT_DIAL_MODE_CNF            = 0x1003,   /* ����ģʽ��ѯ�ظ� */
    ID_RNIC_AT_MSG_ID_ENUM_BUTT
};
typedef VOS_UINT32 AT_RNIC_MSG_ID_ENUM_UINT32;

/*****************************************************************************
  4 STRUCT&UNION����
*****************************************************************************/

/*****************************************************************************
 �ṹ����: AT_RNIC_DSFLOW_IND_STRU
 �ṹ˵��: AT��RNIC����������ѯ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgId;

    VOS_UINT16                          usClientId;                   /* Client ID */
    VOS_UINT8                           aucRsv[2];                  /* ���� */
} AT_RNIC_DIAL_MODE_REQ_STRU;

/*****************************************************************************
 �ṹ����: RNIC_AT_DIAL_MODE_CNF_STRU
 �ṹ˵��: RNIC��AT����ģʽ��ѯ�ظ�
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgId;

    VOS_UINT16                          usClientId;                   /* Client ID */
    VOS_UINT8                           aucRsv[2];                  /* ���� */
    VOS_UINT32                          ulDialMode;                 /* Dialģʽ */
    VOS_UINT32                          ulIdleTime;                 /* ��ʱ������ */
    VOS_UINT32                          ulEventReportFlag;          /* �Ƿ��Ӧ���ϱ���ʶ */
} RNIC_AT_DIAL_MODE_CNF_STRU;

/*****************************************************************************
 �ṹ����: AT_RNIC_DSFLOW_IND_STRU
 �ṹ˵��: AT��RNIC���������ϱ�ָʾ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgId;

    VOS_UINT16                          usClientId;                   /* Client ID */
    RNIC_DEV_ID_ENUM_UINT8              enRnicRmNetId;              /* RNIC����ID */
    VOS_UINT8                           aucRsv[1];                  /* ���� */
} AT_RNIC_DSFLOW_IND_STRU;

/*****************************************************************************
 �ṹ����: RNIC_DATA_RATE_STRU
 �ṹ˵��: RNICͳ�Ƶ���������
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulDLDataRate;                           /* ��ǰ�������� */
    VOS_UINT32                          ulULDataRate;                           /* ��ǰ�������� */
} RNIC_DATA_RATE_STRU;

/*****************************************************************************
 �ṹ����: RNIC_AT_DSFLOW_RSP_STRU
 �ṹ˵��: RNIC��AT�����������ݽṹ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgId;

    VOS_UINT16                          usClientId;                   /* Client ID */
    VOS_UINT8                           aucRsv[2];                  /* ���� */
    RNIC_DATA_RATE_STRU                 stRnicDataRate;             /* ��ǰ�������� */
} RNIC_AT_DSFLOW_RSP_STRU;

/*****************************************************************************
 �ṹ����: AT_RNIC_PDN_INFO_CFG_IND_STRU
 �ṹ˵��: PDN����������Ϣ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgId;

    VOS_UINT32                          bitOpIpv4PdnInfo    : 1;
    VOS_UINT32                          bitOpIpv6PdnInfo    : 1;
    VOS_UINT32                          bitOpSpare          : 30;

    VOS_UINT8                           ucRmNetId;                              /* ����ID */
    VOS_UINT8                           ucIfaceId;                              /* ȫ������ID */
    VOS_UINT8                           aucRsv[3];
    union
    {
        VOS_UINT8                       ucRabId;                                /* ���غ�, ��Χ[5, 15] */
        VOS_UINT8                       ucPduSessionId;                         /* PduSession ID */
    };
    MODEM_ID_ENUM_UINT16                enModemId;                              /* Modem ID */

    VOS_UINT32                          ulIpv4Addr;                             /* IPV4��IP��ַ�������� */
    VOS_UINT8                           aucIpv6Addr[RNICITF_MAX_IPV6_ADDR_LEN]; /* �� PDP�����Ĵ�����IPV6��ַ���ȣ�������":" */
} AT_RNIC_PDN_INFO_CFG_IND_STRU;

/*****************************************************************************
 �ṹ����: AT_RNIC_PDN_INFO_REL_IND_STRU
 �ṹ˵��: PDN�ͷ�������Ϣ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgId;

    VOS_UINT32                          bitOpIpv4PdnInfo    : 1;
    VOS_UINT32                          bitOpIpv6PdnInfo    : 1;
    VOS_UINT32                          bitOpSpare          : 30;

    VOS_UINT8                           ucRmNetId;                              /* ����ID */
    VOS_UINT8                           ucIfaceId;                              /* ȫ������ID */
    VOS_UINT8                           aucRsv[2];

} AT_RNIC_PDN_INFO_REL_IND_STRU;

/*****************************************************************************
 �ṹ����: AT_RNIC_USB_TETHER_INFO_IND_STRU
 �ṹ˵��: USB������Ϣָʾ��Ϣ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgId;

    VOS_UINT8                           aucRmnetName[RNIC_RMNET_NAME_MAX_LEN];  /* Rmnet������ */
    AT_RNIC_USB_TETHER_CONN_ENUM_UINT8  enTetherConnStat;                       /* USB Tethering����״̬ */
    VOS_UINT8                           aucRsv[3];

} AT_RNIC_USB_TETHER_INFO_IND_STRU;


/*****************************************************************************
  5 OTHERS����
*****************************************************************************/

/*****************************************************************************
  6 ��������
*****************************************************************************/

extern VOS_UINT32 RNIC_StartFlowCtrl(RNIC_DEV_ID_ENUM_UINT8 enRmNetId);
extern VOS_UINT32 RNIC_StopFlowCtrl(RNIC_DEV_ID_ENUM_UINT8 enRmNetId);



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

#endif /* __AT_RNIC_INTERFACE_H__ */

