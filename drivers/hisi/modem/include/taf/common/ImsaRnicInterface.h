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
#ifndef __IMSA_RNIC_INTERFACE_H__
#define __IMSA_RNIC_INTERFACE_H__


/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/

#include "vos.h"


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

#define   IMSA_RNIC_IPV4_ADDR_LENGTH                (4)     /* IPV4��ַ���ȣ���λ�ֽ� */
#define   IMSA_RNIC_IPV6_ADDR_LENGTH                (16)    /* IPV6��ַ����, ��λ�ֽ� */
#define   IMSA_RNIC_MAX_DNS_SERVER_NUM              (4)     /* ��������4��DNS��������һ������ */
#define   IMSA_RNIC_IMS_PORT_RANGE_GROUP_MAX_NUM    (32)    /* IMS�˿ں������ֵ */
#define   IMSA_RNIC_SIP_PORT_RANGE_GROUP_MAX_NUM    (4)     /* IMS SIP�˿ں������ֵ */


/*******************************************************************************
  3 ö�ٶ���
*******************************************************************************/

/*****************************************************************************
 ö������: IMSA_RNIC_MSG_ID_ENUM
 ö��˵��: IMSA��RNIC����Ϣ����
*****************************************************************************/
enum IMSA_RNIC_MSG_ID_ENUM
{
    /* IMSA����RNIC����Ϣö�� */
    ID_IMSA_RNIC_PDN_ACT_IND                = 0x0001,                           /* PDN����ָʾ */
    ID_IMSA_RNIC_PDN_DEACT_IND              = 0x0002,                           /* PDNȥ����ָʾ */
    ID_IMSA_RNIC_PDN_MODIFY_IND             = 0x0003,                           /* PDN�޸�ָʾ */
    ID_IMSA_RNIC_RESERVED_PORTS_CONFIG_IND  = 0x0004,                           /* IMS�˿ں�����ָʾ */
    ID_IMSA_RNIC_SOCKET_EXCEPTION_IND       = 0x0005,                           /* SOCKET�쳣ָʾ */
    ID_IMSA_RNIC_SIP_PORTS_RANGE_IND        = 0x0006,                           /* SIP�˿ںŷ�Χָʾ */

    /* RNIC����IMSA����Ϣö�� */

    ID_IMSA_RNIC_MSG_ID_ENUM_BUTT
};
typedef VOS_UINT32 IMSA_RNIC_MSG_ID_ENUM_UINT32;

/*****************************************************************************
 ö������: IMSA_RNIC_IMS_RAT_TYPE_ENUM
 ö��˵��: IMSע��������
*****************************************************************************/
enum IMSA_RNIC_IMS_RAT_TYPE_ENUM
{
    IMSA_RNIC_IMS_RAT_TYPE_LTE          = 0x00,
    IMSA_RNIC_IMS_RAT_TYPE_WIFI         = 0x01,
    IMSA_RNIC_IMS_RAT_TYPE_BUTT
};
typedef VOS_UINT8 IMSA_RNIC_IMS_RAT_TYPE_ENUM_UINT8;

/*****************************************************************************
 ö������: IMSA_RNIC_PDN_EMC_IND_ENUM
 ö��˵��: IMS PDN �Ƿ�����emergency service
*****************************************************************************/
enum IMSA_RNIC_PDN_EMC_IND_ENUM
{
    IMSA_RNIC_PDN_NOT_FOR_EMC           = 0x00,
    IMSA_RNIC_PDN_FOR_EMC               = 0x01,

    IMSA_RNIC_PDN_EMC_IND_BUTT
};
typedef VOS_UINT8 IMSA_RNIC_PDN_EMC_IND_ENUM_UINT8;


/*****************************************************************************
  4 STRUCT&UNION����
*****************************************************************************/

/*****************************************************************************
 �ṹ����: IMSA_RNIC_IPV4_PDN_INFO_STRU
 �ṹ˵��: IPV4 ������Ϣ�ṹ
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          bitOpDnsPrim         :1;                    /* �ⲿģ����д��ָʾaucDnsPrimAddr�Ƿ���Ҫ���� */
    VOS_UINT32                          bitOpDnsSec          :1;                    /* �ⲿģ����д��ָʾaucDnsSecAddr�Ƿ���Ҫ���� */
    VOS_UINT32                          bitOpSpare           :30;

    VOS_UINT8                           aucIpV4Addr[IMSA_RNIC_IPV4_ADDR_LENGTH];    /* IP��ַ */
    VOS_UINT8                           aucDnsPrimAddr[IMSA_RNIC_IPV4_ADDR_LENGTH]; /* ��DNS������IP */
    VOS_UINT8                           aucDnsSecAddr[IMSA_RNIC_IPV4_ADDR_LENGTH];  /* ��DNS������IP */
} IMSA_RNIC_IPV4_PDN_INFO_STRU;

/*****************************************************************************
 �ṹ����: IMSA_RNIC_IPV6_PDN_INFO_STRU
 �ṹ˵��: IPV6 ������Ϣ�ṹ
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          bitOpDnsPrim         :1;                    /* �ⲿģ����д��ָʾaucDnsPrimAddr�Ƿ���Ҫ���� */
    VOS_UINT32                          bitOpDnsSec          :1;                    /* �ⲿģ����д��ָʾaucDnsSecAddr�Ƿ���Ҫ���� */
    VOS_UINT32                          bitOpSpare           :30;

    VOS_UINT32                          ulBitPrefixLen;                             /* IPV6ǰ׺����,��λΪbit */
    VOS_UINT8                           aucIpV6Addr[IMSA_RNIC_IPV6_ADDR_LENGTH];    /* IP��ַ */
    VOS_UINT8                           aucDnsPrimAddr[IMSA_RNIC_IPV6_ADDR_LENGTH]; /* ��DNS������IP */
    VOS_UINT8                           aucDnsSecAddr[IMSA_RNIC_IPV6_ADDR_LENGTH];  /* ��DNS������IP */
} IMSA_RNIC_IPV6_PDN_INFO_STRU;

/*****************************************************************************
 �ṹ����: IMSA_RNIC_PDN_INFO_CONFIG_STRU
 �ṹ˵��: IMSA��RNIC����������PDN������Ϣ
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          bitOpIpv4PdnInfo : 1;
    VOS_UINT32                          bitOpIpv6PdnInfo : 1;
    VOS_UINT32                          bitOpMtuInfo     : 1;
    VOS_UINT32                          bitOpSpare       : 29;

    MODEM_ID_ENUM_UINT16                enModemId;
    union
    {
        VOS_UINT8                       ucRabId;
        VOS_UINT8                       ucPduSessionId;
    };
    IMSA_RNIC_IMS_RAT_TYPE_ENUM_UINT8   enRatType;
    IMSA_RNIC_PDN_EMC_IND_ENUM_UINT8    enEmcInd;
    VOS_UINT8                           aucReserved[3];

    IMSA_RNIC_IPV4_PDN_INFO_STRU        stIpv4PdnInfo;
    IMSA_RNIC_IPV6_PDN_INFO_STRU        stIpv6PdnInfo;

    VOS_UINT32                          ulMtuSize;
} IMSA_RNIC_PDN_INFO_CONFIG_STRU;

/*****************************************************************************
 �ṹ����: IMSA_RNIC_PDN_ACT_IND_STRU
 �ṹ˵��: IMSA��RNIC��PDN����ָʾ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /* ��Ϣͷ */     /* _H2ASN_Skip */
    VOS_UINT32                          ulMsgId;                                /* ��Ϣ���� */  /* _H2ASN_Skip */
    IMSA_RNIC_PDN_INFO_CONFIG_STRU      stPdnInfo;
} IMSA_RNIC_PDN_ACT_IND_STRU;

/*****************************************************************************
 �ṹ����: IMSA_RNIC_PDN_DEACT_IND_STRU
 �ṹ˵��: IMSA��RNIC��PDNȥ����ָʾ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /* ��Ϣͷ */     /* _H2ASN_Skip */
    VOS_UINT32                          ulMsgId;                                /* ��Ϣ���� */  /* _H2ASN_Skip */

    MODEM_ID_ENUM_UINT16                enModemId;
    union
    {
        VOS_UINT8                       ucRabId;
        VOS_UINT8                       ucPduSessionId;
    };
    IMSA_RNIC_IMS_RAT_TYPE_ENUM_UINT8   enRatType;
    IMSA_RNIC_PDN_EMC_IND_ENUM_UINT8    enEmcInd;
    VOS_UINT8                           aucReserved[3];
} IMSA_RNIC_PDN_DEACT_IND_STRU;

/*****************************************************************************
 �ṹ����: IMSA_RNIC_PDN_MODIFY_IND_STRU
 �ṹ˵��: IMSA��RNIC��PDN�޸�ָʾ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /* ��Ϣͷ */     /* _H2ASN_Skip */
    VOS_UINT32                          ulMsgId;                                /* ��Ϣ���� */  /* _H2ASN_Skip */
    IMSA_RNIC_PDN_INFO_CONFIG_STRU      stPdnInfo;
} IMSA_RNIC_PDN_MODIFY_IND_STRU;

/*****************************************************************************
 �ṹ����: IMSA_RNIC_IMS_PORT_RANGE_STRU
 �ṹ˵��: IMSA��RNIC��IMS�˿ں��鷶Χ�ṹ
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usMinPort;
    VOS_UINT16                          usMaxPort;
} IMSA_RNIC_IMS_PORT_RANGE_STRU;

/*****************************************************************************
 �ṹ����: IMSA_RNIC_IMS_PORT_INFO_STRU
 �ṹ˵��: IMSA��RNIC��IMS�˿ں����ýṹ
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulImsPortRangeNum;                      /* IMS�˿������ */
    MODEM_ID_ENUM_UINT16                enModemId;                              /* ��ǰModem�� */
    VOS_UINT16                          usReserved;
    IMSA_RNIC_IMS_PORT_RANGE_STRU       astImsPortRange[IMSA_RNIC_IMS_PORT_RANGE_GROUP_MAX_NUM];  /* IMS�˿����� */
} IMSA_RNIC_IMS_PORT_INFO_STRU;

/*****************************************************************************
 �ṹ����: IMSA_RNIC_RESERVED_PORTS_CONFIG_IND_STRU
 �ṹ˵��: IMSA��RNIC��IMS�˿ں�����ָʾ��Ϣ�ṹ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /* ��Ϣͷ */    /* _H2ASN_Skip */
    VOS_UINT32                          ulMsgId;                                /* ��Ϣ���� */  /* _H2ASN_Skip */
    IMSA_RNIC_IMS_PORT_INFO_STRU        stImsPortInfo;                          /* IMS�˿�������Ϣ */
} IMSA_RNIC_RESERVED_PORTS_CONFIG_IND_STRU;

/*****************************************************************************
 �ṹ����: IMSA_RNIC_SOCKET_EXCEPTION_IND_STRU
 �ṹ˵��: IMSA��RNIC��SOCKET�쳣ָʾ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /* ��Ϣͷ */    /* _H2ASN_Skip */
    VOS_UINT32                          ulMsgId;                                /* ��Ϣ���� */  /* _H2ASN_Skip */
    VOS_INT16                           sSocketErrorNo;
    VOS_UINT16                          usRsv;
    IMSA_RNIC_PDN_INFO_CONFIG_STRU      stPdnInfo;
} IMSA_RNIC_SOCKET_EXCEPTION_IND_STRU;

/*****************************************************************************
 �ṹ����: IMSA_RNIC_IMS_SIP_PORT_RANGE_STRU
 �ṹ˵��: IMSA��RNIC��IMS�˿ں�����ָʾ��Ϣ�ṹ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /* ��Ϣͷ */    /* _H2ASN_Skip */
    VOS_UINT32                          ulMsgId;                                /* ��Ϣ���� */  /* _H2ASN_Skip */
    MODEM_ID_ENUM_UINT16                enModemId;
    VOS_UINT16                          usSipPortRangeNum;
    IMSA_RNIC_IMS_PORT_RANGE_STRU       astSipPortRange[IMSA_RNIC_SIP_PORT_RANGE_GROUP_MAX_NUM];
} IMSA_RNIC_SIP_PORT_RANGE_IND_STRU;


/*****************************************************************************
  5 OTHERS����
*****************************************************************************/

/*****************************************************************************
  6 ��������
*****************************************************************************/



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

#endif /* __IMSA_RNIC_INTERFACE_H__ */

