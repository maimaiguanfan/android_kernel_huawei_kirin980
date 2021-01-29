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

#ifndef __DIAG_API_COMM_H__
#define __DIAG_API_COMM_H__

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push)
#pragma pack(4)

/*****************************************************************************
  1 Include Headfile
*****************************************************************************/
#include <vos.h>
#include <mdrv.h>
#include "diag_service_comm.h"
#include "diag_cfg_comm.h"
/*****************************************************************************
  2 macro
*****************************************************************************/
#define DIAG_GET_MODEM_ID(id)               (id >> 24)
#define DIAG_GET_MODE_ID(id)                ((id & 0x000F0000)>>16)
#define DIAG_GET_PRINTF_LEVEL(id)           ((id & 0x0000F000)>>12)
#define DIAG_GET_GROUP_ID(id)               ((id & 0x00000F00)>>8)
#define DIAG_GET_MODULE_ID(id)              ( id & 0x00000FFF)

#define LTE_DIAG_PRINTF_PARAM_MAX_NUM       (6)

#ifdef DIAG_SYSTEM_5G
#define VOS_TRANSID_LEN  VOS_UINT16
#else
#define VOS_TRANSID_LEN  VOS_UINT32
#endif

/*****************************************************************************
  3 Enum
*****************************************************************************/

/*****************************************************************************
  4 UNION
*****************************************************************************/
/*****************************************************************************
  5 struct
*****************************************************************************/
/* ==============��ϢӦ���ϱ��ӿڲ���====================================== */

/*diag printV��������ֽ���,����ǰ���ļ������кų��ȣ�-1��Ԥ��\0������*/
#define DIAG_PRINTF_MAX_LEN 	(1000-1)

typedef struct
{
    VOS_UINT32 ulModule;                        /* ��ӡ��Ϣ���ڵ�ģ��ID */
    VOS_UINT32 ulLevel;                         /* ������� */
    VOS_UINT32 ulNo;                            /* IND��� */
    VOS_CHAR   szText[DIAG_PRINTF_MAX_LEN+1];   /* ���д�ӡ�ı����ݣ����ܰ����ļ����к�,��'\0'��β */
} DIAG_CMD_LOG_PRINT_RAW_TXT_IND_STRU;


/* diag event report�ϱ���Ϣ�Ľṹ�� */
typedef struct
{
    VOS_UINT32 ulNo;      /* ���*/
    VOS_UINT32 ulId;      /* ��Ϣ�����¼�ID,��Ҫ�����Ϣ,�տ�,�¼�,��ͨ��ӡ���ʱ�ó�ԱΪ��*/
    VOS_UINT32 ulModule;  /* ��ӡ��Ϣ���ڵ�ģ��ID */
    VOS_INT8   aucDta[0]; /* �û����ݻ�����*/    /*lint !e43 */
} DIAG_CMD_LOG_EVENT_IND_STRU;

/* diag air report�ϱ���Ϣ�Ľṹ�� */
typedef struct
{
    VOS_UINT32 ulModule;     /* Դģ��ID*/
    VOS_UINT32 ulSide;       /* 1: NET-->UE, 2: UE-->NET*/
    VOS_UINT32 ulNo;         /* ���*/
    VOS_UINT32 ulId;         /* ID*/
    VOS_INT8 aucDta[0];     /* �û����ݻ�����*/  /*lint !e43 */
} DIAG_CMD_LOG_AIR_IND_STRU;

/* diag volte report�ϱ���Ϣ�Ľṹ�� */
typedef struct
{
    VOS_UINT32 ulModule;     /* Դģ��ID*/
    VOS_UINT32 ulSide;       /* 1: NET-->UE, 2: UE-->NET*/
    VOS_UINT32 ulNo;         /* ���*/
    VOS_UINT32 ulId;         /* ID*/
    VOS_INT8   aucDta[0];    /* �û����ݻ�����*/     /*lint !e43 */
} DIAG_CMD_LOG_VOLTE_IND_STRU;


/* diag userplane report�ϱ���Ϣ�Ľṹ�� */
typedef struct
{
    VOS_UINT32 ulModule;     /* Դģ��ID*/
    VOS_UINT32 ulNo;         /* ���*/
    VOS_UINT32 ulId;         /* ID*/
    VOS_INT8 aucDta[0];     /* �û����ݻ�����*/  /*lint !e43 */
} DIAG_CMD_LOG_USERPLANE_IND_STRU;


/* diag �ṹ�������ϱ���Ϣ�Ľṹ�� */
typedef struct
{
    VOS_UINT32 ulModule;     /* Դģ��ID*/
    VOS_UINT32 ulMsgId;      /* ID*/
    VOS_UINT32 ulNo;         /* ���*/
    VOS_INT8   aucDta[0];    /* �û����ݻ�����*/     /*lint !e43 */
} DIAG_CMD_TRANS_IND_STRU;


/* diag trace report�ϱ���Ϣ�Ľṹ�� */
typedef struct
{
    VOS_UINT32 ulModule;     /* Դģ��ID*/
    VOS_UINT32 ulDestMod;    /* Ŀ��ģ��ID*/
    VOS_UINT32 ulNo;         /* ���*/
    VOS_UINT32 ulId;         /* ID*/
    VOS_INT8 aucDta[0];     /* �û����ݻ�����*/  /*lint !e43 */
} DIAG_CMD_LOG_LAYER_IND_STRU;

/* ======================================================================== */

/* CNF������Ϣͷ */
typedef struct
{
    DIAG_SRV_SOCP_HEADER_STRU       socp_header;
    DIAG_FRAME_INFO_STRU            frame_header;
}DIAG_API_CNF_HEADER_STRU;

/* ���������Ϣͷ */
typedef struct
{
    DIAG_SRV_SOCP_HEADER_STRU       socp_header;
    DIAG_FRAME_INFO_STRU            frame_header;
}DIAG_SRV_LOG_ID_HEADER_STRU;

/* ��ӡ������Ϣͷ */
typedef struct
{
    DIAG_SRV_SOCP_HEADER_STRU       socp_header;
    DIAG_FRAME_INFO_STRU            frame_header;
}DIAG_SRV_LOG_HEADER_STRU;

/* trans ������Ϣͷ */
typedef struct
{
    DIAG_SRV_SOCP_HEADER_STRU       socp_header;
    DIAG_FRAME_INFO_STRU            frame_header;
    DIAG_CMD_TRANS_IND_STRU         trans_header;
}DIAG_SRV_TRANS_HEADER_STRU;

/* event ������Ϣͷ */
typedef struct
{
    DIAG_SRV_SOCP_HEADER_STRU       socp_header;
    DIAG_FRAME_INFO_STRU            frame_header;
    DIAG_CMD_LOG_EVENT_IND_STRU     event_header;
}DIAG_SRV_EVENT_HEADER_STRU;

/* air ������Ϣͷ */
typedef struct
{
    DIAG_SRV_SOCP_HEADER_STRU       socp_header;
    DIAG_FRAME_INFO_STRU            frame_header;
    DIAG_CMD_LOG_AIR_IND_STRU       air_header;
}DIAG_SRV_AIR_HEADER_STRU;

/* volte ������Ϣͷ */
typedef struct
{
    DIAG_SRV_SOCP_HEADER_STRU       socp_header;
    DIAG_FRAME_INFO_STRU            frame_header;
    DIAG_CMD_LOG_VOLTE_IND_STRU     volte_header;
}DIAG_SRV_VOLTE_HEADER_STRU;

/* trace ������Ϣͷ */
typedef struct
{
    DIAG_SRV_SOCP_HEADER_STRU       socp_header;
    DIAG_FRAME_INFO_STRU            frame_header;
    DIAG_CMD_LOG_LAYER_IND_STRU     trace_header;
}DIAG_SRV_TRACE_HEADER_STRU;

/* layer ������Ϣͷ */
typedef struct
{
    DIAG_SRV_SOCP_HEADER_STRU       socp_header;
    DIAG_FRAME_INFO_STRU            frame_header;
    DIAG_CMD_LOG_LAYER_IND_STRU     layer_header;
}DIAG_SRV_LAYER_HEADER_STRU;

/* user ������Ϣͷ */
typedef struct
{
    DIAG_SRV_SOCP_HEADER_STRU       socp_header;
    DIAG_FRAME_INFO_STRU            frame_header;
    DIAG_CMD_LOG_USERPLANE_IND_STRU user_header;
}DIAG_SRV_USER_HEADER_STRU;


/*****************************************************************************
  6 Extern Global Variable
*****************************************************************************/

/*****************************************************************************
  7 Fuction Extern
*****************************************************************************/


#pragma pack(pop)

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif

