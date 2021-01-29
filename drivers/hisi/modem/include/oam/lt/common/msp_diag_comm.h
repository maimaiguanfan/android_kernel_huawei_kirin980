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


#ifndef __MSP_DIAG_COMM_H__
#define __MSP_DIAG_COMM_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <product_config.h>
#include <vos.h>
#include <mdrv.h>

/**************************************************************************
  �궨��
**************************************************************************/

/*���PS��MODID ��DIAG_AIR_MSG_LOG_ID�ĵط����滻��DIAG_ID*/
#define DIAG_ID(module_id, log_type)   (VOS_UINT32)(module_id | (log_type << 12))

#define DIAG_AIR_MSG_LOG_ID(module_id, is_up_link)  DIAG_ID(module_id, is_up_link) /*module_id��ӦPID*/

#define DIAG_GEN_LOG_MODULE(modemid, modetype, level)  \
               (((VOS_UINT32)(modemid & 0xff) << 24)  \
              | ((VOS_UINT32)(modetype & 0xf) << 16)  \
              | ((VOS_UINT32)(level    & 0xf ) << 12))

#define DIAG_GEN_MODULE(modemid, modetype)  \
               (((VOS_UINT32)(modemid & 0xff) << 24)  \
              | ((VOS_UINT32)(modetype & 0xf) << 16))


#define DIAG_GEN_MODULE_EX(modemid, modetype, groupid)  \
                   (((VOS_UINT32)(modemid & 0xff) << 24)  \
                  | ((VOS_UINT32)(modetype & 0xf) << 16) \
                  | ((VOS_UINT32)(groupid  & 0xf)  << 8))

#define DIAG_GEN_LOG_ID(filenum, linenum)   \
                ((((VOS_UINT32)(filenum & 0XFFFF)) << 16)   \
                | ((VOS_UINT32)(linenum & 0XFFFF)))

/*is_up_linkȡֵ*/
#define OS_MSG_UL                (0x01)/* ��ʾ������Ϣ*/
#define OS_MSG_DL                (0x02)/* ��ʾ������Ϣ*/

#define DIAG_SIDE_UE             (0x1)  /* ��ʾUE���յĿտ���Ϣ��NET-->UE*/
#define DIAG_SIDE_NET            (0x2)  /* ��ʾNET���յĿտ���Ϣ��UE-->NET*/

/* �¼���Ϣ��ӡ������*/
#define DIAG_LOG_TYPE_INFO            0x00000008UL
#define DIAG_LOG_TYPE_AUDIT_FAILURE   0x00000004UL
#define DIAG_LOG_TYPE_AUDIT_SUCCESS   0x00000002UL
#define DIAG_LOG_TYPE_ERROR           0x00000001UL
#define DIAG_LOG_TYPE_WARNING         0x00000010UL

/* MSP�ڲ�ʹ��*/
#define ID_MSG_DIAG_HSO_DISCONN_IND                 (0x00010004)

/*diag AGENT����PSģ���HSO�ط�����*/
#define ID_MSG_DIAG_CMD_REPLAY_TO_PS    			(0x00010100)

/* diag ���͸�������ϵͳ�Ľ������� */
#define ID_MSG_DIAG_CMD_CONNECT_REQ    		        (0x00010200)
#define ID_MSG_DIAG_CMD_CONNECT_CNF    		        (ID_MSG_DIAG_CMD_CONNECT_REQ)
/* diag ���͸�������ϵͳ�Ķ������� */
#define ID_MSG_DIAG_CMD_DISCONNECT_REQ    		    (0x00010202)
#define ID_MSG_DIAG_CMD_DISCONNECT_CNF    		    (ID_MSG_DIAG_CMD_DISCONNECT_REQ)

/* diag ���͸�TL-PHY����Ϣ�������� */
#define ID_MSG_DIAG_DSP_CONNECT_REQ    		        (0x30004903)
#define ID_MSG_DIAG_DSP_CONNECT_CNF    		        (ID_MSG_DIAG_DSP_CONNECT_REQ)
/* diag ���͸�TL-PHY����Ϣ�������� */
#define ID_MSG_DIAG_DSP_DISCONNECT_REQ    		    (0x30004904)
#define ID_MSG_DIAG_DSP_DISCONNECT_CNF    		    (ID_MSG_DIAG_DSP_DISCONNECT_REQ)

/* diag ���͸�HL1C����Ϣ�������� */
#define ID_MSG_DIAG_HL1C_CONNECT_REQ    		    (0x3f000007)
#define ID_MSG_DIAG_HL1C_CONNECT_CNF    		    (ID_MSG_DIAG_HL1C_CONNECT_REQ)
/* diag ���͸�HL1C����Ϣ�������� */
#define ID_MSG_DIAG_HL1C_DISCONNECT_REQ    		    (0x3f000006)
#define ID_MSG_DIAG_HL1C_DISCONNECT_CNF    		    (ID_MSG_DIAG_HL1C_DISCONNECT_REQ)
/* �˿���Ϣ */
#define ID_MSG_DIAG_NRPHY_PORT_INFO                 (0x3f00000a)

/* ������Ϣ�ķ�Χ�� DIAG_MESSAGE_TYPE_U32 ��ͨ */

/* 2000 - 2fff����PS����Ϣ��Χ */

/* c000 - cfff����HIFI����Ϣ��Χ */
#define     DIAG_HIFI_RELEASE_REQ                   (0x0000c001)

/*****************************************************************************
  4 Enum
*****************************************************************************/

/* ==============��Ϣ��ͷ�ṹö��ֵ����==================================== */

/* MSP_DIAG_STID_STRU:pri4b */
#ifdef DIAG_SYSTEM_5G
enum DIAG_MESSAGE_TYPE
{
    DIAG_MSG_TYPE_RSV       = 0x0,
    DIAG_MSG_TYPE_MSP       = 0x1,
    DIAG_MSG_TYPE_PS        = 0x2,
    DIAG_MSG_TYPE_PHY       = 0x3,
    DIAG_MSG_TYPE_BBP       = 0x4,
    DIAG_MSG_TYPE_HSO       = 0x5,
    DIAG_MSG_TYPE_BSP       = 0x9, /*MODEM BSP*/
    DIAG_MSG_TYPE_EASYRF    = 0xa,
    DIAG_MSG_TYPE_AP_BSP    = 0xb, /*AP BSP*/
    DIAG_MSG_TYPE_AUDIO     = 0xc,
    DIAG_MSG_TYPE_APP       = 0xe,
    DIAG_MSG_TYPE_BUTT
};
#else
enum DIAG_MESSAGE_TYPE
{
    DIAG_MSG_TYPE_RSV   = 0x0,
    DIAG_MSG_TYPE_MSP   = 0x1,
    DIAG_MSG_TYPE_PS    = 0x2,
    DIAG_MSG_TYPE_PHY   = 0x3,
    DIAG_MSG_TYPE_BBP   = 0x4,
    DIAG_MSG_TYPE_HSO   = 0x5,
    DIAG_MSG_TYPE_BSP   = 0x9,
    DIAG_MSG_TYPE_ISP   = 0xa,
    DIAG_MSG_TYPE_AUDIO = 0xc,
    DIAG_MSG_TYPE_APP   = 0xe,
    DIAG_MSG_TYPE_BUTT
};
#endif
typedef VOS_UINT32 DIAG_MESSAGE_TYPE_U32;

/* MSP_DIAG_STID_STRU:mode4b */
enum DIAG_MODE_TYPE
{
    DIAG_MODE_LTE  = 0x0,
    DIAG_MODE_TDS  = 0x1,
    DIAG_MODE_GSM  = 0x2,
    DIAG_MODE_UMTS = 0x3,
    DIAG_MODE_1X   = 0x4,
    DIAG_MODE_HRPD = 0x5,
    DIAG_MODE_NR   = 0x6,
    DIAG_MODE_COMM = 0xf
};
typedef VOS_UINT32 DIAG_MODE_TYPE_U32;


enum DIAG_MODEM_ID
{
    DIAG_MODEM_0 = 0x0,
    DIAG_MODEM_1 = 0x1,
    DIAG_MODEM_2 = 0x2
};
typedef unsigned int DIAG_MODEM_ID_U32;

typedef enum
{
    PS_LOG_LEVEL_OFF  = 0,
    PS_LOG_LEVEL_ERROR,
    PS_LOG_LEVEL_WARNING,
    PS_LOG_LEVEL_NORMAL,
    PS_LOG_LEVEL_INFO,
    PS_LOG_LEVEL_BUTT
}PS_LOG_LEVEL_EN;

/**************************************************************************
  5 �ṹ����
**************************************************************************/

/**************************************************************************
  ��������
**************************************************************************/

/* ==============�����ϱ��ӿڲ���========================================== */

typedef struct
{
    VOS_UINT32        ulModule;		/* DIAG_GEN_MODULE*/
    VOS_UINT32        ulPid;
    VOS_UINT32        ulEventId;		/* �¼�ID */
    VOS_UINT32        ulLength;
    VOS_VOID          *pData;
}DIAG_EVENT_IND_STRU;

typedef struct
{
    VOS_UINT32        ulModule;       /* DIAG_GEN_MODULE*/
    VOS_UINT32        ulPid;
    VOS_UINT32        ulMsgId;
    VOS_UINT32        ulDirection;
    VOS_UINT32        ulLength;
    VOS_VOID          *pData;
}DIAG_AIR_IND_STRU;

typedef struct
{
    VOS_UINT32        ulModule;
    VOS_UINT32        ulPid;
    VOS_UINT32        ulMsgId;
    VOS_UINT32        ulReserve;
    VOS_UINT32        ulLength;
    VOS_VOID          *pData;
} DIAG_TRANS_IND_STRU;

/*********************************���ӹ������********************************************/
/* OSA��Ϣͷ */
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32              ulMsgId;           /* ��ϢID */
    VOS_UINT32              ulLen;             /* ��Ϣ���� */
    VOS_UINT8               pContext[0];       /* ������ʵ��ַ*/ /*lint !e43 */
}DIAG_OSA_MSG_HEAD;

typedef DIAG_OSA_MSG_HEAD DIAG_CFG_MSG_HEAD;

/* �˼���Ϣͷ */
typedef struct{
	VOS_UINT32		        ulMsgId;            /* ��ϢID */
	VOS_UINT32		        ulLen;              /* ��Ϣ���� */
	VOS_UINT8		        *pContext[0];       /* ������ʵ��ַ*/ /*lint !e43 */
}DIAG_CORE_MSG_HEAD;

/* ��TLPHY��Ϣͷ */
typedef struct
{
	VOS_UINT32              ulMsgId;            /* ��ϢID */
	VOS_UINT8	            *pContext[0];        /*lint !e43 */
}DIAG_PHY_MSG_HEAD;

typedef struct{
    VOS_UINT32			    ulChannelId;	    /* ͨ��ID */
    VOS_UINT32			    ulResult;		    /*  ������ 0�ɹ�, 0x5C5C5C5Cͨ��δ����, 0x5A5A5A5Aͨ��δʹ��, ����ֵʧ��*/
}DIAG_CONNECT_RESULT;

/* ������Ϣ�ظ��ṹ��,  �����-> DIAG */
typedef struct{
    VOS_UINT32				ulSn;
    DIAG_CONNECT_RESULT     pstResult[0];    /*lint !e43 */
}DIAG_CONN_CNF_MSG_STRU;

/* ������Ϣ�ַ��ṹ��,  DIAG -> ����� */
typedef struct{
    VOS_UINT32				ulSn;
}DIAG_CONN_REQ_MSG_STRU;


/* ��ϢID :ID_MSG_DIAG_CMD_REPLAY_TO_PS */
typedef struct
{
    VOS_UINT32 ulReserved;/*����*/
} DIAG_CMD_REPLAY_SET_REQ_STRU;

/* ��ϢID: DIAG_HIFI_RELEASE_REQ*/
typedef struct
{
    VOS_UINT32                  ulRsv;
} DIAG_MSG_TO_OTHERCPU_DISCONN_STRU;


/* �����Ϣƥ��ӿڣ���κͷ���ֵΪ��׼��OSA��Ϣ��ʽ */
typedef VOS_VOID* (*DIAG_LayerMsgMatchFunc)(VOS_VOID *pMsg);
/*****************************************************************************
 Function Name   : DIAG_TraceMatchFuncReg
 Description        : �����Ϣ����ע��ӿ�(�˽ӿڲ�֧���ظ�ע�ᣬ���ע�᷵��ʧ��)
 Input                : ���˴�����
 Output              : None
 Return              : ����ֵΪע����: 0-ע��ɹ�������-ע��ʧ��
*****************************************************************************/
VOS_UINT32 DIAG_LayerMsgMatchFuncReg(DIAG_LayerMsgMatchFunc pFun);

/* �����Ϣƥ����ɺ��notify�ӿ� */
typedef VOS_UINT32 (*DIAG_LayerMsgMatchNotifyFunc)(VOS_UINT32 sendPid, VOS_VOID *pMsg);
/*****************************************************************************
 Function Name   : DIAG_TraceMatchFuncReg
 Description        : �����Ϣ���˵�notify�ӿ�ע��(�˽ӿڲ�֧���ظ�ע�ᣬ���ע�᷵��ʧ��)
 Input                : ���˴�����
 Output              : None
 Return              : ����ֵΪע����: 0-ע��ɹ�������-ע��ʧ��
*****************************************************************************/
VOS_UINT32 DIAG_LayerMsgMatchNotifyFuncReg(DIAG_LayerMsgMatchNotifyFunc pFun);

/*****************************************************************************
 ���� ��      : DIAG_TransReport
 ��������   : �ṹ�������ϱ��ӿ�(�滻ԭ����DIAG_ReportCommand)
 �������   : DIAG_TRANS_IND_STRU->ulModule( 31-24:modemid,19-16:modeid)
 �������    : DIAG_TRANS_IND_STRU->ulPid(ģ���PID)
                          DIAG_TRANS_IND_STRU->ulMsgId(͸������ID)
                          DIAG_TRANS_IND_STRU->ulLength(͸����Ϣ�ĳ���)
                          DIAG_TRANS_IND_STRU->pData(͸����Ϣ)
*****************************************************************************/
VOS_UINT32 DIAG_TransReport(DIAG_TRANS_IND_STRU *pstData);

/*****************************************************************************
 ���� ��       : DIAG_TransReport_Ex
 ��������    : �ṹ�������ϱ�����չ�ӿ�(���DIAG_TransReport��������11-8bits �����Ϣ)
 �������    : DIAG_TRANS_IND_STRU->ulModule( 31-24:modemid,19-16:modeid, 11-8:groupid )
 �������    : DIAG_TRANS_IND_STRU->ulPid(ģ���PID)
                            DIAG_TRANS_IND_STRU->ulMsgId(͸������ID)
                            DIAG_TRANS_IND_STRU->ulLength(͸����Ϣ�ĳ���)
                            DIAG_TRANS_IND_STRU->pData(͸����Ϣ)
*****************************************************************************/
VOS_UINT32 DIAG_TransReport_Ex(DIAG_TRANS_IND_STRU *pstData);

/*****************************************************************************
 ���� ��      : DIAG_LogReport
 ��������  : ��ӡ�ϱ��ӿ�
 �������  : ulModuleId( 31-24:modemid,19-16:modeid,15-12:level )
                          ulPid( PID )
                          pcFileName(�ϱ�ʱ����ļ�·��ɾ����ֻ�����ļ���)
                          ulLineNum(�к�)
                          pszFmt(�ɱ����)
ע������  : ���ڴ˽ӿڻᱻЭ��ջƵ�����ã�Ϊ��ߴ���Ч�ʣ����ӿ��ڲ���ʹ��1K�ľֲ����������ϱ����ַ�����Ϣ��
                         �Ӷ��˽ӿڶ�Э��ջ���������ƣ�
                         һ�ǵ��ô˽ӿڵ�����ջ�е��ڴ�Ҫ����Ϊ�˽ӿ�Ԥ��1K�ռ䣻
                         ���ǵ��ô˽ӿ������log��Ҫ����1K���������ֻ��Զ�������
*****************************************************************************/
VOS_UINT32 DIAG_LogReport(VOS_UINT32 ulModuleId, VOS_UINT32 ulPid, VOS_CHAR *cFileName, VOS_UINT32 ulLineNum, VOS_CHAR* pszFmt, ...);
/******************************************************************************
��������: DIAG_LogIdReport
��������: ��ӡ�����͵Ĵ�ӡ�ӿں���
����˵��:ulModuleId[in]  : ( 31-24:modemid,23-16:modeid,15-12:level )
                      ulPid[in]           : PID
                      ulLogId[in]       : ���ļ��ź��кŸ���DIAG_LOG_ID����
                      amount[in]       : �ɱ����������������ulModuleId/ulLevel/ulLogId/amout��
                      ...                   : �ɱ����
����Լ��:
            1. ���Խ�ֹ�Դ˺������ж��η�װ��ֻ��ת����
            2. ֧�ֿɱ�Ĳ����������������ڵ���ʱ�ɲ���amountָ����������
            3. �ɱ����ֻ֧��int����
            4. Ŀǰ�汾��֧�ֵ�������������6���������Ĳ���Ĭ�϶���
******************************************************************************/
VOS_INT32 DIAG_LogIdReport(VOS_UINT32 ulModuleId, VOS_UINT32 ulPid,
                        VOS_UINT32 ulLogId, VOS_UINT32 amount, ...);

/*****************************************************************************
 ���� ��     : DIAG_EventReport
 ��������  : �¼��ϱ��ӿڣ���PSʹ��(�滻ԭ����DIAG_ReportEventLog)
 �������  : DIAG_EVENT_IND_STRU->ulModule( 31-24:modemid,19-16:modeid )
                          DIAG_EVENT_IND_STRU->ulEventId(event ID)
                          DIAG_EVENT_IND_STRU->ulLength(event�ĳ���)
                          DIAG_EVENT_IND_STRU->pData(event��Ϣ)
*****************************************************************************/
VOS_UINT32 DIAG_EventReport(DIAG_EVENT_IND_STRU *pstEvent);

/*****************************************************************************
 ���� ��       : DIAG_AirMsgReport
 ��������  : �տ���Ϣ�ϱ��ӿڣ���PSʹ��(�滻ԭ����DIAG_ReportAirMessageLog)
 �������  : DIAG_AIR_IND_STRU->ulModule( 31-24:modemid,23-16:modeid)
                          DIAG_AIR_IND_STRU->ulMsgId(�տ���ϢID)
                          DIAG_AIR_IND_STRU->ulDirection(�տ���Ϣ�ķ���)
                          DIAG_AIR_IND_STRU->ulLength(�տ���Ϣ�ĳ���)
                          DIAG_AIR_IND_STRU->pData(�տ���Ϣ��Ϣ)
*****************************************************************************/
VOS_UINT32 DIAG_AirMsgReport(DIAG_AIR_IND_STRU *pstAir);

/*****************************************************************************
 �� �� ��     : DIAG_TraceReport
 ��������  : �����Ϣ�ϱ��ӿڣ���PSʹ��(�滻ԭ����DIAG_ReportLayerMessageLog)
 �������  : pMsg(��׼��VOS��Ϣ�壬Դģ�顢Ŀ��ģ����Ϣ����Ϣ���л�ȡ)
*****************************************************************************/
VOS_VOID DIAG_TraceReport(VOS_VOID *pMsg);

/*****************************************************************************
 Function Name   : DIAG_GetConnState
 Description        : ��ȡ��ǰ��������״̬
 Return              : 1:connect; 0:disconnect
*****************************************************************************/
VOS_UINT32 DIAG_GetConnState(VOS_VOID);

/*****************************************************************************
 �� �� ��     : Log_GetPrintLevel
 ��������  : �õ�ģ��Id����ģ��Id�ڴ�ӡ�����¼���е�������
 return               : PS_LOG_LEVEL_EN
*****************************************************************************/
VOS_UINT32 Diag_GetLogLevel(VOS_UINT32 ulPid);
/*****************************************************************************
 �� �� ��  : DIAG_RttReport	1344
 ��������  : 1. �ϱ�TL PHY���ݣ�NR/LR Modem�ṩ,����TLPHY RTTʹ��
             2. ���ָ��ΪTLPHYҪ�ϱ������ݣ�����TLPHY���������ݸ�ʽ����serviceͷ��ʼ������(��sid��ʼ)
             3. Rtt Agent֡�ṹ����mspͷ�ļ�msp_diag_comm.h(ʹ��msp.h)
             4. �ӿ�ͷ�ļ���������msp_diag_comm.h(ʹ��msp.h)
             5. pData����Ϊ�� ulDataLen����Ϊ0
             6. ����Ҫ��֤ulDataLen��ȷ�ԣ����ܳ���pData����Խ��
             7. ulDataLen���ܴ���64K
 ����ֵ   :  ERR_MSP_SUCCESS  �ɹ�
             ERR_MSP_DIAG_SRC_BUFF_ALLOC_FAIL Դ���ڴ治��
             ����ֵ          ʧ��
*****************************************************************************/
VOS_UINT32 DIAG_RttReport(VOS_VOID *pData, VOS_UINT32 ulDatalen);
#ifdef __cplusplus
    }
#endif
#endif

