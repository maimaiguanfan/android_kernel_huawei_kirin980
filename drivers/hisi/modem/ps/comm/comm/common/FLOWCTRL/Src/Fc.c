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

/******************************************************************************
   1 ͷ�ļ�����
******************************************************************************/
#include "Fc.h"
#include "FcInterface.h"
#include "PsTypeDef.h"
#include "PsCommonDef.h"
#include "NVIM_Interface.h"
#include "TTFComm.h"
#include "TTFUtil.h"



/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
/*lint -e534*/
#define    THIS_FILE_ID        PS_FILE_ID_FLOW_CTRL_C


/******************************************************************************
   2 �궨��
******************************************************************************/

/*****************************************************************************
   ��������
*****************************************************************************/
STATIC FC_POINT_STRU *FC_POINT_Get(FC_ID_ENUM_UINT8 enFcId);
STATIC FC_PRI_ENUM_UINT8  FC_POLICY_GetPriWithFcId
(
    FC_PRIVATE_POLICY_ID_ENUM_UINT8     enPolicyId,
    FC_ID_ENUM_UINT8                   enFcId
);
STATIC VOS_UINT32 FC_SndRegPointMsg
(
    FC_REG_POINT_STRU                  *pstFcRegPoint
);
STATIC VOS_UINT32 FC_SndChangePointMsg
(
    FC_ID_ENUM_UINT8                   enFcId,
    FC_POLICY_ID_ENUM_UINT8             enPolicyId,
    FC_PRI_ENUM_UINT8                  enPri,
    MODEM_ID_ENUM_UINT16                enModemId
);
STATIC VOS_UINT32  FC_POLICY_AddPoint
(
    FC_PRIVATE_POLICY_ID_ENUM_UINT8     enPolicyId,
    FC_ID_ENUM_UINT8                   enFcId,
    FC_PRI_ENUM_UINT8                  enPointPri
);
STATIC VOS_UINT32  FC_POLICY_DelPoint
(
    FC_PRIVATE_POLICY_ID_ENUM_UINT8     enPolicyId,
    FC_ID_ENUM_UINT8                   enFcId
);
STATIC VOS_UINT32  FC_POLICY_ChangePoint
(
    FC_PRIVATE_POLICY_ID_ENUM_UINT8     enPolicyId,
    FC_ID_ENUM_UINT8                   enFcId,
    FC_PRI_ENUM_UINT8                  enPointOldPri,
    FC_PRI_ENUM_UINT8                  enPointNewPri
);
STATIC VOS_UINT32 FC_SndDeRegPointMsg
(
    FC_ID_ENUM_UINT8                   enFcId,
    MODEM_ID_ENUM_UINT16                enModemId
);

/*****************************************************************************
   ȫ�ֱ�������
*****************************************************************************/

/* ���ز���ʵ�� */
FC_POLICY_STRU                          g_astFcPolicy[FC_PRIVATE_POLICY_ID_BUTT];

/* ���ص����ʵ�� */
FC_POINT_MGR_STRU                       g_stFcPointMgr;

/* �����ڲ����Ա� */
FC_PRIVATE_POLICY_ID_ENUM_UINT8         g_aenPrivatePolicyTbl[FC_MODEM_ID_NUM][FC_POLICY_ID_BUTT] =
{
    {
        FC_PRIVATE_POLICY_ID_MEM_MODEM_0,
        FC_PRIVATE_POLICY_ID_CPU_A_MODEM_0,
        FC_PRIVATE_POLICY_ID_CDS_MODEM_0,
        FC_PRIVATE_POLICY_ID_CST_MODEM_0,
        FC_PRIVATE_POLICY_ID_GPRS_MODEM_0,
        FC_PRIVATE_POLICY_ID_TMP_MODEM_0,
        FC_PRIVATE_POLICY_ID_CPU_C_MODEM_0,
        FC_PRIVATE_POLICY_ID_CDMA_MODEM_0
    },
    {
        FC_PRIVATE_POLICY_ID_MEM_MODEM_1,
        FC_PRIVATE_POLICY_ID_CPU_A_MODEM_1,
        FC_PRIVATE_POLICY_ID_CDS_MODEM_1,
        FC_PRIVATE_POLICY_ID_CST_MODEM_1,
        FC_PRIVATE_POLICY_ID_GPRS_MODEM_1,
        FC_PRIVATE_POLICY_ID_TMP_MODEM_1,
        FC_PRIVATE_POLICY_ID_CPU_C_MODEM_1,
        FC_PRIVATE_POLICY_ID_CDMA_MODEM_1,
    }
};

VOS_UINT32                              g_ulFcEnableMask = 0;

VOS_UINT32                              g_ulFcDebugLevel = (VOS_UINT32)PS_PRINT_WARNING;

/******************************************************************************
   6 ����ʵ��
******************************************************************************/

/*lint -save -e958 */


VOS_VOID FC_MNTN_TraceEvent(VOS_VOID *pMsg)
{
   DIAG_TraceReport(pMsg);

   return;
}


VOS_VOID FC_MNTN_TracePointFcEvent
(
    FC_MNTN_EVENT_TYPE_ENUM_UINT16      enMsgName,
    FC_POINT_STRU                      *pstFcPoint,
    VOS_UINT32                          ulIsFuncInvoked,
    VOS_UINT32                          ulResult
)
{
    FC_MNTN_POINT_FC_STRU               stMntnPointFc;
    VOS_UINT_PTR                        PtrAddr;
    VOS_UINT32                          *pulOctet;


    stMntnPointFc.ulSenderCpuId         = VOS_LOCAL_CPUID;
    stMntnPointFc.ulSenderPid           = UEPS_PID_FLOWCTRL;
    stMntnPointFc.ulReceiverCpuId       = VOS_LOCAL_CPUID;
    stMntnPointFc.ulReceiverPid         = UEPS_PID_FLOWCTRL;
    stMntnPointFc.ulLength              = (sizeof(FC_MNTN_POINT_FC_STRU) - VOS_MSG_HEAD_LENGTH);
    stMntnPointFc.enMsgName             = enMsgName;
    stMntnPointFc.ulIsFuncInvoked       = ulIsFuncInvoked;
    stMntnPointFc.ulResult              = ulResult;

    /* fetch FcPoint info */
    stMntnPointFc.stFcPoint.enFcId          = pstFcPoint->enFcId;
    stMntnPointFc.stFcPoint.enModemId       = pstFcPoint->enModemId;
    stMntnPointFc.stFcPoint.aucRsv[0]       = 0x0U;
    stMntnPointFc.stFcPoint.aucRsv[1]       = 0x0U;
    stMntnPointFc.stFcPoint.aucRsv[2]       = 0x0U;
    stMntnPointFc.stFcPoint.aucRsv[3]       = 0x0U;
    stMntnPointFc.stFcPoint.aucRsv[4]       = 0x0U;
    stMntnPointFc.stFcPoint.ulPolicyMask    = pstFcPoint->ulPolicyMask;
    stMntnPointFc.stFcPoint.ulFcMask        =   pstFcPoint->ulFcMask;
    stMntnPointFc.stFcPoint.ulParam1        =   pstFcPoint->ulParam1;
    stMntnPointFc.stFcPoint.ulParam2        =   pstFcPoint->ulParam2;

    PtrAddr                                     = (VOS_UINT_PTR)(pstFcPoint->pSetFunc);
    stMntnPointFc.stFcPoint.aulPointSetAddr[0]  = (VOS_UINT32)(PtrAddr & (~0U));   /* Low */
    pulOctet                                    = (VOS_UINT32 *)(VOS_UINT_PTR)pstFcPoint->pSetFunc;
    stMntnPointFc.stFcPoint.aulPointSetAddr[1]  = (VOS_UINT32)(VOS_UINT_PTR)(pulOctet + 1);    /* High */

    PtrAddr                                     = (VOS_UINT_PTR)(pstFcPoint->pClrFunc);
    stMntnPointFc.stFcPoint.aulPointClrAddr[0]  = (VOS_UINT32)(PtrAddr & (~0U));   /* Low */
    pulOctet                                    = (VOS_UINT32 *)(VOS_UINT_PTR)pstFcPoint->pClrFunc;
    stMntnPointFc.stFcPoint.aulPointClrAddr[1]  = (VOS_UINT32)(VOS_UINT_PTR)(pulOctet + 1);    /* High */

    PtrAddr                                     = (VOS_UINT_PTR)(pstFcPoint->pRstFunc);
    stMntnPointFc.stFcPoint.aulPointRstAddr[0]  = (VOS_UINT32)(PtrAddr & (~0U));   /* Low */
    pulOctet                                    = (VOS_UINT32 *)(VOS_UINT_PTR)pstFcPoint->pRstFunc;
    stMntnPointFc.stFcPoint.aulPointRstAddr[1]  = (VOS_UINT32)(VOS_UINT_PTR)(pulOctet + 1);    /* High */

    FC_MNTN_TraceEvent((VOS_VOID *)&stMntnPointFc);

    return;
}



STATIC VOS_UINT32  FC_MNTN_TracePolicy(FC_MNTN_EVENT_TYPE_ENUM_UINT16 enMsgName, FC_POLICY_STRU *pPolicy )
{
    FC_MNTN_POLICY_STRU                 stFcMntnPolicy;
    VOS_UINT_PTR                        PtrAddr;
    VOS_UINT32                          *pulOctet;

    PSACORE_MEM_SET(&stFcMntnPolicy, sizeof(FC_MNTN_POLICY_STRU), 0, sizeof(FC_MNTN_POLICY_STRU));

    stFcMntnPolicy.ulSenderCpuId    = VOS_LOCAL_CPUID;
    stFcMntnPolicy.ulSenderPid      = UEPS_PID_FLOWCTRL;
    stFcMntnPolicy.ulReceiverCpuId  = VOS_LOCAL_CPUID;
    stFcMntnPolicy.ulReceiverPid    = UEPS_PID_FLOWCTRL;
    stFcMntnPolicy.ulLength         = (sizeof(FC_MNTN_POLICY_STRU) - VOS_MSG_HEAD_LENGTH);
    stFcMntnPolicy.enMsgName        = enMsgName;

    stFcMntnPolicy.stPolicy.enPolicyId      =   pPolicy->enPolicyId;
    stFcMntnPolicy.stPolicy.ucPriCnt        =   pPolicy->ucPriCnt;
    stFcMntnPolicy.stPolicy.aucRsv[0]       =   0x0U;
    stFcMntnPolicy.stPolicy.aucRsv[1]       =   0x0U;
    stFcMntnPolicy.stPolicy.aucRsv[2]       =   0x0U;
    stFcMntnPolicy.stPolicy.enHighestPri    =   pPolicy->enHighestPri;
    stFcMntnPolicy.stPolicy.enDonePri       =   pPolicy->enDonePri;
    stFcMntnPolicy.stPolicy.enToPri         =   pPolicy->enToPri;
    PSACORE_MEM_CPY(stFcMntnPolicy.stPolicy.astFcPri, sizeof(pPolicy->astFcPri), pPolicy->astFcPri, sizeof(pPolicy->astFcPri));

    PtrAddr                                     =   (VOS_UINT_PTR)(pPolicy->pAdjustForUpFunc);
    stFcMntnPolicy.stPolicy.aulPolicyUpAddr[0]  = (VOS_UINT32)(PtrAddr & (~0U));   /* Low */
    pulOctet                                    = (VOS_UINT32 *)(VOS_UINT_PTR)(pPolicy->pAdjustForUpFunc);
    stFcMntnPolicy.stPolicy.aulPolicyUpAddr[1]  = (VOS_UINT32)(VOS_UINT_PTR)(pulOctet + 1);    /* High */

    PtrAddr                                     =   (VOS_UINT_PTR)(pPolicy->pAdjustForDownFunc);
    stFcMntnPolicy.stPolicy.aulPolicyDownAddr[0]  = (VOS_UINT32)(PtrAddr & (~0U));   /* Low */
    pulOctet                                    = (VOS_UINT32 *)(VOS_UINT_PTR)(pPolicy->pAdjustForDownFunc);
    stFcMntnPolicy.stPolicy.aulPolicyDownAddr[1]  = (VOS_UINT32)(VOS_UINT_PTR)(pulOctet + 1);    /* High */

    PtrAddr                                     =   (VOS_UINT_PTR)(pPolicy->pPostFunc);
    stFcMntnPolicy.stPolicy.aulPolicyPostAddr[0]  = (VOS_UINT32)(PtrAddr & (~0U));   /* Low */
    pulOctet                                    = (VOS_UINT32 *)(VOS_UINT_PTR)(pPolicy->pAdjustForDownFunc);
    stFcMntnPolicy.stPolicy.aulPolicyPostAddr[1]  = (VOS_UINT32)(VOS_UINT_PTR)(pulOctet + 1);    /* High */

    FC_MNTN_TraceEvent(&stFcMntnPolicy);

    return VOS_OK;
}


VOS_UINT32  FC_MNTN_TraceCpuLoad(FC_MNTN_EVENT_TYPE_ENUM_UINT16 enMsgName, VOS_UINT32  ulCpuLoad )
{
    FC_MNTN_CPULOAD_STRU                stFcMntnCpuLoad;

    PSACORE_MEM_SET(&stFcMntnCpuLoad, sizeof(FC_MNTN_CPULOAD_STRU), 0, sizeof(FC_MNTN_CPULOAD_STRU));

    stFcMntnCpuLoad.ulSenderCpuId   = VOS_LOCAL_CPUID;
    stFcMntnCpuLoad.ulSenderPid     = UEPS_PID_FLOWCTRL;
    stFcMntnCpuLoad.ulReceiverCpuId = VOS_LOCAL_CPUID;
    stFcMntnCpuLoad.ulReceiverPid   = UEPS_PID_FLOWCTRL;
    stFcMntnCpuLoad.ulLength        = (sizeof(FC_MNTN_CPULOAD_STRU) - VOS_MSG_HEAD_LENGTH);
    stFcMntnCpuLoad.enMsgName       = enMsgName;
    stFcMntnCpuLoad.ulCpuLoad       = ulCpuLoad;

    FC_MNTN_TraceEvent(&stFcMntnCpuLoad);

    return VOS_OK;
}


VOS_VOID  FC_SetFcEnableMask( VOS_UINT32 ulEnableMask )
{
    g_ulFcEnableMask    = ulEnableMask;

    return;
}


/* add by wangrong */
STATIC VOS_UINT32 FC_IsPolicyEnable(VOS_UINT32 ulPointPolicyMask, MODEM_ID_ENUM_UINT16 enModemId)
{
    VOS_UINT32                          ulPolicyMask;

    /* ����Modem1,�ڲ��������ж�ʹ��ʱ����Ҫ����FC_POLICY_ID_BUTT������NV����ʹ��λ�Ƚ� */
    if ( MODEM_ID_0 ==  enModemId )
    {
        ulPolicyMask = ulPointPolicyMask;
    }
    else
    {
        ulPolicyMask = (ulPointPolicyMask >> FC_POLICY_ID_BUTT);
    }

    return (ulPolicyMask & g_ulFcEnableMask);
}
/* add by wangrong */


VOS_UINT32  FC_RegPoint
(
    FC_REG_POINT_STRU                  *pstFcRegPoint
)
{
    VOS_UINT32                          ulResult;

    /*�������*/
    if ( VOS_NULL_PTR == pstFcRegPoint )
    {
        FC_LOG(PS_PRINT_ERROR, "FC_RegPoint, ERROR, pstFcRegPoint is NULL!\n");
        return VOS_ERR;
    }

    /* Add by wangrong */
    if ( MODEM_ID_BUTT <= pstFcRegPoint->enModemId )
    {
        FC_LOG1(PS_PRINT_ERROR, "FC_RegPoint, ERROR, enModemId err %d!\n",
            (VOS_INT32)pstFcRegPoint->enModemId);
        return VOS_ERR;
    }
    /* Add by wangrong */

    if ( FC_POLICY_ID_BUTT <= pstFcRegPoint->enPolicyId )
    {
        FC_LOG1(PS_PRINT_ERROR, "FC_RegPoint, ERROR, enPolicyId err %d!\n",
            (VOS_INT32)pstFcRegPoint->enPolicyId);
        return VOS_ERR;
    }

    if ( FC_ID_BUTT <= pstFcRegPoint->enFcId )
    {
        FC_LOG1(PS_PRINT_ERROR, "FC_RegPoint, ERROR, enFcId err %d\n",
            (VOS_INT32)pstFcRegPoint->enFcId);
        return VOS_ERR;
    }

    if ( (FC_PRI_LOWEST > pstFcRegPoint->enFcPri)
         || (FC_PRI_HIGHEST < pstFcRegPoint->enFcPri) )
    {
        FC_LOG1(PS_PRINT_ERROR, "FC_RegPoint, ERROR, Invalid enPri %d\n",
                (VOS_INT32)pstFcRegPoint->enFcPri);
        return VOS_ERR;
    }

    /* ʹ�ܼ�� */
    if ( FC_POLICY_MASK(pstFcRegPoint->enPolicyId) != (FC_POLICY_MASK(pstFcRegPoint->enPolicyId) & g_ulFcEnableMask) )
    {
        /* �ڴ�����δʹ�� */
        FC_LOG1(PS_PRINT_INFO, "FC_RegPoint, INFO, MEM FlowCtrl is disabled %d \n",
                (VOS_INT32)g_ulFcEnableMask);
        return VOS_OK;
    }

    /* ע���ڴ���ԣ����ȼ�ֻ�����ڴ�����Լ���ķ�Χ֮�� */
    if (FC_POLICY_ID_MEM == pstFcRegPoint->enPolicyId)
    {
        if ( (FC_PRI_FOR_MEM_LEV_1 > pstFcRegPoint->enFcPri)
             || (FC_PRI_FOR_MEM_LEV_4 < pstFcRegPoint->enFcPri) )
        {
            FC_LOG1(PS_PRINT_ERROR, "FC_RegPoint, ERROR, Invalid enPri for MEM %d\n",
                    (VOS_INT32)pstFcRegPoint->enFcPri);
            return VOS_ERR;
        }
    }

    if (VOS_NULL_PTR == pstFcRegPoint->pSetFunc)
    {
        FC_LOG(PS_PRINT_ERROR, "FC_RegPoint, ERROR, Invalid Fun pSetFunc\n");
        return VOS_ERR;
    }

    if (VOS_NULL_PTR == pstFcRegPoint->pClrFunc)
    {
        FC_LOG(PS_PRINT_ERROR, "FC_RegPoint, ERROR, Invalid Fun pClrFunc\n");
        return VOS_ERR;
    }

    ulResult = FC_SndRegPointMsg(pstFcRegPoint);

    return ulResult;
}


VOS_UINT32  FC_DeRegPoint
(
    FC_ID_ENUM_UINT8                   enFcId,
    MODEM_ID_ENUM_UINT16                enModemId
)
{
    VOS_UINT32                          ulResult;
    FC_POINT_STRU                      *pstFcPoint;
    VOS_UINT32                          ulPointPolicyMask;


    if (FC_ID_BUTT <=  enFcId)
    {
         FC_LOG1(PS_PRINT_ERROR, "FC_DeRegPoint, ERROR, Invalid enFcId %d\n",
                (VOS_INT32)enFcId);
         return VOS_ERR;
    }

    /* add by wangrong */
    if ( MODEM_ID_BUTT <= enModemId )
    {
        FC_LOG1(PS_PRINT_ERROR, "FC_DeRegPoint, ERROR, enModemId err %d!\n",
            (VOS_INT32)enModemId);
        return VOS_ERR;
    }
    /* add by wangrong */


    /* ͨ�����ص�ID�ҵ����ص�ṹ */
    pstFcPoint    = FC_POINT_Get(enFcId);
    if (VOS_NULL_PTR == pstFcPoint)
    {
        /* �����ز���δʹ�ܣ��򷵻�VOS_OK */
        FC_LOG1(PS_PRINT_WARNING, "FC_DeRegPoint, WARNING, This FCPonit = %d didn't Reg!\n",
                (VOS_INT32)enFcId);
        return VOS_OK;
    }


    /* ���ص�ɾ������Ҫ����Ӧ�����ز��Խ��д��� */
    ulPointPolicyMask   = pstFcPoint->ulPolicyMask;

    /* add by wangrong */
    if (0 != FC_IsPolicyEnable(ulPointPolicyMask, enModemId))
    {
         ulResult = FC_SndDeRegPointMsg(enFcId,enModemId);

         return ulResult;
    }
    /* add by wangrong */

    return VOS_OK;

}



VOS_UINT32  FC_ChangePoint
(
    FC_ID_ENUM_UINT8                   enFcId,
    FC_POLICY_ID_ENUM_UINT8             enPolicyId,
    FC_PRI_ENUM_UINT8                  enPri,
    MODEM_ID_ENUM_UINT16                enModemId
)
{
    VOS_UINT32                          ulResult;

    if ( FC_ID_BUTT <=  enFcId )
    {
        FC_LOG1(PS_PRINT_ERROR, "FC_ChangePoint, ERROR, Invalid enFcId %d\n",
                (VOS_INT32)enFcId);
        return VOS_ERR;
    }


    if ( (FC_PRI_LOWEST > enPri)
         || (FC_PRI_HIGHEST < enPri) )
    {
        FC_LOG1(PS_PRINT_ERROR, "FC_ChangePoint, ERROR, Invalid enPri %d\n",
                (VOS_INT32)enPri);
        return VOS_ERR;
    }


    if ( FC_POLICY_ID_BUTT <= enPolicyId )
    {
        FC_LOG1(PS_PRINT_ERROR, "FC_ChangePoint, ERROR, enPolicyId err %d!\n",
            (VOS_INT32)enPolicyId);
        return VOS_ERR;
    }

     /* add by wangrong */
    if ( MODEM_ID_BUTT <= enModemId )
    {
        FC_LOG1(PS_PRINT_ERROR, "FC_ChangePoint, ERROR, enModemId err %d!\n",
            (VOS_INT32)enModemId);
        return VOS_ERR;
    }
     /* add by wangrong */

    /* ʹ�ܼ�� */
    if ( FC_POLICY_MASK(enPolicyId) != (FC_POLICY_MASK(enPolicyId) & g_ulFcEnableMask) )
    {
        /* �ڴ�����δʹ�� */
        FC_LOG1(PS_PRINT_INFO, "FC_ChangePoint, INFO, MEM FlowCtrl is disabled %d \n",
                (VOS_INT32)g_ulFcEnableMask);
        return VOS_OK;
    }

    ulResult = FC_SndChangePointMsg(enFcId, enPolicyId, enPri, enModemId);

    return ulResult;
}



STATIC FC_POINT_STRU *FC_POINT_Get(FC_ID_ENUM_UINT8 enFcId)
{
    VOS_UINT32                          ulFcIdIdxLoop;


    if ( FC_MAX_POINT_NUM < g_stFcPointMgr.ulPointNum )
    {
        FC_LOG(PS_PRINT_WARNING, "FC_POINT_Get, ERROR, g_stFcPointMgr.ulPointNum overflow!\n");

        return VOS_NULL_PTR;
    }

    for (ulFcIdIdxLoop = 0; ulFcIdIdxLoop < g_stFcPointMgr.ulPointNum; ulFcIdIdxLoop++)
    {
        if (g_stFcPointMgr.astFcPoint[ulFcIdIdxLoop].enFcId == enFcId)
        {
            return (&(g_stFcPointMgr.astFcPoint[ulFcIdIdxLoop]));
        }
    }

    return VOS_NULL_PTR;
}



STATIC VOS_UINT32  FC_POINT_Add
(
    FC_REG_POINT_STRU                  *pstFcRegPoint
)
{
    FC_POINT_STRU                      *pPoint;
    VOS_UINT32                          ulFcPointLoop;
    /* add by wangrong */
    FC_PRIVATE_POLICY_ID_ENUM_UINT8     enFcPrivatePolicyId;

    enFcPrivatePolicyId = g_aenPrivatePolicyTbl[pstFcRegPoint->enModemId][pstFcRegPoint->enPolicyId];
    /* add by wangrong */

    if ( FC_MAX_POINT_NUM < g_stFcPointMgr.ulPointNum )
    {
        FC_LOG(PS_PRINT_WARNING, "FC_POINT_Add, WARNING, Reach maximun point number!\n");
        return VOS_ERR;
    }

    for (ulFcPointLoop = 0; ulFcPointLoop < g_stFcPointMgr.ulPointNum; ulFcPointLoop++)
    {
        if (g_stFcPointMgr.astFcPoint[ulFcPointLoop].enFcId == pstFcRegPoint->enFcId)
        {
            /*���õ��µĲ��Լ��뵽ԭ���õ�Ĳ�����*/
            pPoint  = &(g_stFcPointMgr.astFcPoint[ulFcPointLoop]);
            pPoint->ulPolicyMask |=  FC_POLICY_MASK(enFcPrivatePolicyId);
            pPoint->enModemId     =  pstFcRegPoint->enModemId;

            /*��ӡһ����ʾ�������õ㱻����������*/
            FC_LOG1(PS_PRINT_NORMAL, "FC_POINT_Add, NORMAL, The same point has added %d!\n",
                (VOS_INT32)pstFcRegPoint->enFcId);

            return VOS_OK;
        }
    }

    if ( FC_MAX_POINT_NUM == g_stFcPointMgr.ulPointNum )
    {
        FC_LOG(PS_PRINT_WARNING, "FC_POINT_Add, WARNING, g_stFcPointMgr.ulPointNum reaches the MAXNUM!\n");
        return VOS_ERR;
    }

    /*���û���ҵ�����ô����һ���µ㣬��Ҫ���뵽����ṹ��*/
    pPoint  = &(g_stFcPointMgr.astFcPoint[g_stFcPointMgr.ulPointNum]);

    pPoint->enFcId          = pstFcRegPoint->enFcId;
    pPoint->ulPolicyMask    = FC_POLICY_MASK(enFcPrivatePolicyId);
    pPoint->ulFcMask        = 0;
    pPoint->ulParam1        = pstFcRegPoint->ulParam1;
    pPoint->ulParam2        = pstFcRegPoint->ulParam2;
    pPoint->pSetFunc        = pstFcRegPoint->pSetFunc;
    pPoint->pClrFunc        = pstFcRegPoint->pClrFunc;
    pPoint->pRstFunc        = pstFcRegPoint->pRstFunc;

    g_stFcPointMgr.ulPointNum++;

    return VOS_OK;
}



VOS_UINT32  FC_POINT_Del(FC_ID_ENUM_UINT8 enFcId )
{
    VOS_UINT32                          ulFcIdIdxLoop;
    VOS_UINT32                          ulFcShiftId;
    FC_POINT_STRU                      *pFcPoint;


    /* ��ֹȫ�ֱ������ȣ����º����������Խ�� */
    if ( FC_MAX_POINT_NUM < g_stFcPointMgr.ulPointNum)
    {
        FC_LOG(PS_PRINT_WARNING, "FC_POINT_Del, WARNING, g_stFcPointMgr.ulPointNum overflow!\n");

        return VOS_ERR;
    }

    for (ulFcIdIdxLoop = 0; ulFcIdIdxLoop < g_stFcPointMgr.ulPointNum; ulFcIdIdxLoop++)
    {
        if (g_stFcPointMgr.astFcPoint[ulFcIdIdxLoop].enFcId == enFcId)
        {
            for (ulFcShiftId = ulFcIdIdxLoop + 1; ulFcShiftId < g_stFcPointMgr.ulPointNum; ulFcShiftId++)
            {
                pFcPoint    = &(g_stFcPointMgr.astFcPoint[ulFcShiftId]);
                PSACORE_MEM_CPY((pFcPoint - 1), sizeof(FC_POINT_STRU), pFcPoint, sizeof(FC_POINT_STRU));
            }

            /* ���һ����Ҫ�ֶ���� */
            PSACORE_MEM_SET(&(g_stFcPointMgr.astFcPoint[ulFcShiftId-1]), sizeof(FC_POINT_STRU), 0, sizeof(FC_POINT_STRU));

            g_stFcPointMgr.ulPointNum--;

            return VOS_OK;
        }
    }

    return VOS_OK;
}



FC_PRI_OPER_ENUM_UINT32 FC_POINT_SetFc
(
    VOS_UINT32                          ulPolicyMask,
    FC_ID_ENUM_UINT8                   enFcId
)
{
    FC_POINT_STRU                      *pFcPoint;
    VOS_UINT32                          ulNeedSet;
    VOS_UINT32                          ulResult ;


    /* �������ص�,�о��Ƿ���Ҫ�������ز��� */
    ulNeedSet   = PS_FALSE;
    ulResult    = VOS_OK;

    pFcPoint    = FC_POINT_Get(enFcId);
    if (VOS_NULL_PTR == pFcPoint)
    {
        return FC_PRI_CHANGE_AND_CONTINUE;
    }

    /*============================*/
    /* �����0����0��������ת����Ҫִ�����ز���������˵��֮ǰ�����ص��Ѿ���������״̬ */
    if (0 == pFcPoint->ulFcMask)
    {
        /* ִ�����غ��� */
        ulNeedSet   = VOS_TRUE;
    }

    pFcPoint->ulFcMask  |= ulPolicyMask;

    /* ִ������ */
    if (PS_FALSE == ulNeedSet)
    {
        FC_MNTN_TracePointFcEvent(ID_FC_MNTN_POINT_SET_FC, pFcPoint, ulNeedSet, ulResult);
        return FC_PRI_CHANGE_AND_CONTINUE;
    }

    if (VOS_NULL_PTR != pFcPoint->pSetFunc)
    {
        ulResult = pFcPoint->pSetFunc(pFcPoint->ulParam1, pFcPoint->ulParam2);
    }

    FC_MNTN_TracePointFcEvent(ID_FC_MNTN_POINT_SET_FC, pFcPoint, ulNeedSet, ulResult);

    return FC_PRI_CHANGE_AND_BREAK;
}



FC_PRI_OPER_ENUM_UINT32 FC_POINT_ClrFc(VOS_UINT32 ulPolicyMask, FC_ID_ENUM_UINT8 enFcId)
{
    /*  ����ID�ҵ����ص�ʵ����ִ��ʵ�������ؽ������ */
    FC_POINT_STRU                      *pFcPoint;
    VOS_UINT32                          ulNeedSet;
    VOS_UINT32                          ulResult;


    /* �������ص�,�о��Ƿ���Ҫ���н�����ز��� */
    ulNeedSet   = PS_FALSE;
    ulResult    = VOS_OK;

    pFcPoint    = FC_POINT_Get(enFcId);
    if (VOS_NULL_PTR == pFcPoint)
    {
        return FC_PRI_CHANGE_AND_CONTINUE;
    }

    /* ��û����Ϊ�������͵������أ����������� */
    if (0 == (pFcPoint->ulFcMask & ulPolicyMask))
    {
        return FC_PRI_CHANGE_AND_CONTINUE;
    }

    pFcPoint->ulFcMask &= ~ulPolicyMask;

    if ( 0 == pFcPoint->ulFcMask )
    {
        /* ֻ���������������ˣ����ܽ������ */
        ulNeedSet = VOS_TRUE;
    }

    /* ִ�н�����صĲ��� */
    if (PS_FALSE == ulNeedSet)
    {
        FC_MNTN_TracePointFcEvent(ID_FC_MNTN_POINT_CLR_FC, pFcPoint, ulNeedSet, ulResult);
        return FC_PRI_CHANGE_AND_CONTINUE;
    }

    if (VOS_NULL_PTR != pFcPoint->pClrFunc)
    {
        ulResult = pFcPoint->pClrFunc(pFcPoint->ulParam1, pFcPoint->ulParam2);
    }

    FC_MNTN_TracePointFcEvent(ID_FC_MNTN_POINT_CLR_FC, pFcPoint, ulNeedSet, ulResult);

    return FC_PRI_CHANGE_AND_BREAK;
}



VOS_UINT32  FC_POINT_Reg
(
    FC_REG_POINT_STRU                  *pstFcRegPoint
)
{
    FC_PRI_ENUM_UINT8                   enPri;
    FC_PRIVATE_POLICY_ID_ENUM_UINT8     enFcPrivatePolicyId;

    enFcPrivatePolicyId = g_aenPrivatePolicyTbl[pstFcRegPoint->enModemId][pstFcRegPoint->enPolicyId];

    /* ������ص� */
    if ( VOS_OK != FC_POINT_Add(pstFcRegPoint) )
    {
        FC_LOG(PS_PRINT_ERROR, "FC_POINT_Reg, ERROR, FC_POINT_Add error!\n");
        return VOS_ERR;
    }

    /* ��������ز������Ѿ����ڸ�����ID��ֱ�ӷ��� */
    /* add by wangrong */
    enPri   = FC_POLICY_GetPriWithFcId(enFcPrivatePolicyId, pstFcRegPoint->enFcId);
    /* add by wangrong */

    if (FC_PRI_BUTT != enPri)
    {
        FC_LOG(PS_PRINT_ERROR, "FC_POINT_Reg, ERROR, Policy has same FC Id\n");
        return VOS_ERR;
    }

    /* add by wangrong */
    (VOS_VOID)FC_POLICY_AddPoint(enFcPrivatePolicyId,
        pstFcRegPoint->enFcId,
        pstFcRegPoint->enFcPri);
    /* add by wangrong */

    return VOS_OK;
}



VOS_UINT32  FC_POINT_DeReg(FC_ID_ENUM_UINT8 enFcId, MODEM_ID_ENUM_UINT16  enModemId)
{
    FC_POINT_STRU                      *pFcPoint;
    /* add by wangrong */
    FC_PRIVATE_POLICY_ID_ENUM_UINT8     enPolicyId;
    /* add by wangrong */
    VOS_UINT32                          ulPointPolicyMask;

    /* ͨ�����ص�ID�ҵ����ص�ṹ */
    pFcPoint    = FC_POINT_Get(enFcId);
    if (VOS_NULL_PTR == pFcPoint)
    {
        return VOS_ERR;
    }
    /* ��ֹ��һ�ſ���� */
    if ( enModemId != pFcPoint->enModemId )
    {
        return VOS_ERR;
    }

    /* ���ص�ɾ������Ҫ����Ӧ�����ز��Խ��д��� */
    ulPointPolicyMask   = pFcPoint->ulPolicyMask;

    /* add by wangrong */
    for (enPolicyId = FC_PRIVATE_POLICY_ID_MEM_MODEM_0; enPolicyId < FC_PRIVATE_POLICY_ID_BUTT; enPolicyId++)
    {
        if ( 0 != (FC_POLICY_MASK(enPolicyId) & ulPointPolicyMask) )
        {
            /* ʹ�ܼ�� */
            if ( 0 != FC_IsPolicyEnable(FC_POLICY_MASK(enPolicyId), enModemId) )
            {
                (VOS_VOID)FC_POLICY_DelPoint(enPolicyId, enFcId);
            }
        }
    }
    /* add by wangrong */

    /* �����ص����ɾ�������ص� */
    (VOS_VOID)FC_POINT_Del(enFcId);

    return VOS_OK;
}



VOS_UINT32  FC_POINT_Change
(
    FC_ID_ENUM_UINT8                   enFcId,
    FC_POLICY_ID_ENUM_UINT8             enPolicyId,
    FC_PRI_ENUM_UINT8                  enNewPri,
    MODEM_ID_ENUM_UINT16                enModemId
)
{
    FC_POINT_STRU                      *pFcPoint;
    FC_PRI_ENUM_UINT8                  enOldPri;
    FC_PRIVATE_POLICY_ID_ENUM_UINT8     enFcPrivatePolicyId;


    pFcPoint    = FC_POINT_Get(enFcId);
    if (VOS_NULL_PTR == pFcPoint)
    {
        return VOS_ERR;
    }

    enFcPrivatePolicyId = g_aenPrivatePolicyTbl[enModemId][enPolicyId];
    enOldPri            = FC_POLICY_GetPriWithFcId(enFcPrivatePolicyId, enFcId);

    if (FC_PRI_BUTT <= enOldPri)
    {

        FC_LOG3(PS_PRINT_WARNING,
            "FC, FC_POINT_Change, WARNING, oldPri %d err with PolicyId %d, FcId %d\n",
            (VOS_INT32)enOldPri, (VOS_INT32)enFcPrivatePolicyId, (VOS_INT32)enFcId);
        return VOS_ERR;
    }

    if (enNewPri == enOldPri)
    {

        FC_LOG3(PS_PRINT_WARNING,
            "FC, FC_POINT_Change, WARNING, NewPri %d eqaul old with PolicyId %d, FcId %d\n",
            (VOS_INT32)enNewPri, (VOS_INT32)enFcPrivatePolicyId, (VOS_INT32)enFcId);
        return VOS_ERR;
    }

    (VOS_VOID)FC_POLICY_ChangePoint(enFcPrivatePolicyId, enFcId, enOldPri, enNewPri);

    return VOS_OK;
}



STATIC VOS_VOID  FC_POINT_Init( VOS_VOID )
{
    PSACORE_MEM_SET(&g_stFcPointMgr, sizeof(FC_POINT_MGR_STRU), 0, sizeof(FC_POINT_MGR_STRU));
}


STATIC VOS_VOID  FC_POLICY_TrimInvalidPri( FC_POLICY_STRU *pFcPolicy )
{
    FC_PRI_STRU                        *pstPri;
    FC_PRI_ENUM_UINT8                  enPri;

    /* �����ǰ���ȼ���Ч����ֱ�ӷ��أ����轵�� */
    if (FC_PRI_NULL == pFcPolicy->enDonePri)
    {
        return;
    }

    /* �ҵ�һ����Ч�����ȼ���������ѭ�������ı䵱ǰ�������ȼ� */
    for (enPri = pFcPolicy->enDonePri; enPri > FC_PRI_NULL; enPri--)
    {
        pstPri   = &(pFcPolicy->astFcPri[enPri]);
        if (VOS_TRUE == pstPri->ucValid)
        {
            break;
        }
    }

    /* �����ǰ���ȼ��Լ����͵��������ȼ���Ϊ��Ч���򽫵�ǰ���ȼ�ֱ����Ϊ������ȼ� */
    pFcPolicy->enDonePri    = enPri;

    return;
}



STATIC FC_PRI_OPER_ENUM_UINT32 FC_POLICY_UpWithOnePri
(
    FC_POLICY_STRU                     *pFcPolicy,
    FC_PRI_STRU                        *pstPri
)
{
    VOS_UINT32                          ulLoop;

    if (FC_MAX_POINT_NUM < pstPri->ucFcIdCnt)
    {
        FC_LOG(PS_PRINT_ERROR, "FC_POLICY_UpWithOnePri, ERROR, FcIdCnt overflow!\n");

        return FC_PRI_CHANGE_AND_CONTINUE;
    }

    for (ulLoop = 0; ulLoop < pstPri->ucFcIdCnt; ulLoop++)
    {
        (VOS_VOID)FC_POINT_SetFc(FC_POLICY_MASK(pFcPolicy->enPolicyId), pstPri->aenFcId[ulLoop]);
    }

    return FC_PRI_CHANGE_AND_BREAK;
}



STATIC FC_PRI_OPER_ENUM_UINT32  FC_POLICY_DownWithOnePri
(
    FC_POLICY_STRU                     *pPolicy,
    FC_PRI_STRU                        *pstPri
)
{
    VOS_UINT32                          ulLoop;

    if (FC_MAX_POINT_NUM < pstPri->ucFcIdCnt)
    {
        FC_LOG(PS_PRINT_ERROR, "FC_POLICY_DownWithOnePri, ERROR, FcIdCnt overflow!\n");

        return FC_PRI_CHANGE_AND_CONTINUE;
    }

    for (ulLoop = 0; ulLoop < pstPri->ucFcIdCnt; ulLoop++)
    {
        (VOS_VOID)FC_POINT_ClrFc(FC_POLICY_MASK(pPolicy->enPolicyId), pstPri->aenFcId[ulLoop]);
    }

    return FC_PRI_CHANGE_AND_BREAK;
}


VOS_UINT32 FC_POLICY_Up(FC_POLICY_STRU *pPolicy)
{
    FC_PRI_STRU                        *pstPri;
    FC_PRI_ENUM_UINT8                  enPri;

    if (pPolicy->enDonePri >= pPolicy->enHighestPri)
    {
        return VOS_OK;
    }

    while (pPolicy->enDonePri < pPolicy->enHighestPri)
    {
        enPri = (FC_PRI_ENUM_UINT8)(pPolicy->enDonePri + 1);
        pstPri   = &(pPolicy->astFcPri[enPri]);

        /* ��ǰ���ȼ����ڣ�ִ�е�ǰ���ز��� */
        if (VOS_TRUE == pstPri->ucValid)
        {
            (VOS_VOID)FC_POLICY_UpWithOnePri(pPolicy, pstPri);
            pPolicy->enDonePri++;
            break;
        }

        pPolicy->enDonePri++;
    }

    (VOS_VOID)FC_MNTN_TracePolicy(ID_FC_MNTN_POLICY_UP, pPolicy);

    return VOS_OK;
}



VOS_UINT32  FC_POLICY_UpToTargetPri(FC_POLICY_STRU *pFcPolicy, FC_PRI_ENUM_UINT8 enTargetPri )
{
    FC_PRI_STRU                    *pstPri;
    FC_PRI_ENUM_UINT8              enPri;

    if (enTargetPri >= FC_PRI_BUTT)
    {
        return VOS_ERR;
    }

    if (pFcPolicy->enDonePri >= enTargetPri)
    {
        return VOS_OK;
    }

    while (pFcPolicy->enDonePri < enTargetPri)
    {
        enPri   = (FC_PRI_ENUM_UINT8)(pFcPolicy->enDonePri + 1);
        pstPri  = &(pFcPolicy->astFcPri[enPri]);

        /* ��ǰ���ȼ����ڣ�ִ�е�ǰ���ز��� */
        if (VOS_TRUE == pstPri->ucValid)
        {
            (VOS_VOID)FC_POLICY_UpWithOnePri(pFcPolicy, pstPri);
        }

        pFcPolicy->enDonePri++;
    }

    (VOS_VOID)FC_MNTN_TracePolicy(ID_FC_MNTN_POLICY_UP, pFcPolicy);

    return VOS_OK;
}



VOS_UINT32 FC_POLICY_Down(FC_POLICY_STRU *pFcPolicy)
{
    FC_PRI_STRU                        *pstPri;

    if (FC_PRI_NULL == pFcPolicy->enDonePri)
    {
        return VOS_OK;
    }

    while (pFcPolicy->enDonePri > FC_PRI_NULL)
    {
        pstPri   = &(pFcPolicy->astFcPri[pFcPolicy->enDonePri]);

        /* ��ǰ���ȼ����ڣ�ִ�е�ǰ���ȼ��Ľ����ز��� */
        if (VOS_TRUE == pstPri->ucValid)
        {
            (VOS_VOID)FC_POLICY_DownWithOnePri(pFcPolicy, pstPri);
            pFcPolicy->enDonePri--;
            break;
        }

        pFcPolicy->enDonePri--;
    }

    /* ȥ���������Ч���ȼ� */
    FC_POLICY_TrimInvalidPri(pFcPolicy);

    (VOS_VOID)FC_MNTN_TracePolicy(ID_FC_MNTN_POLICY_DOWN, pFcPolicy);

    return VOS_OK;
}



VOS_UINT32  FC_POLICY_DownToTargetPri(FC_POLICY_STRU *pFcPolicy, FC_PRI_ENUM_UINT8 enTargetPri )
{
    FC_PRI_STRU                        *pstPri;

    if (pFcPolicy->enDonePri <= enTargetPri)
    {
        return VOS_OK;
    }

    while (pFcPolicy->enDonePri > enTargetPri)
    {
        pstPri   = &(pFcPolicy->astFcPri[pFcPolicy->enDonePri]);

        /* ��ǰ���ȼ����ڣ�ִ�е�ǰ���ȼ��Ľ����ز��� */
        if (VOS_TRUE == pstPri->ucValid)
        {
            (VOS_VOID)FC_POLICY_DownWithOnePri(pFcPolicy, pstPri);
        }

        pFcPolicy->enDonePri--;
    }

    (VOS_VOID)FC_MNTN_TracePolicy(ID_FC_MNTN_POLICY_DOWN, pFcPolicy);

    return VOS_OK;
}



STATIC VOS_VOID  FC_POLICY_UpdateHighestPri( FC_POLICY_STRU *pPolicy )
{
    FC_PRI_ENUM_UINT8                  enPri;

    pPolicy->enHighestPri   = FC_PRI_NULL;

    for (enPri = FC_PRI_1; enPri < FC_PRI_BUTT; enPri++)
    {
        if (VOS_TRUE == pPolicy->astFcPri[enPri].ucValid)
        {
            pPolicy->enHighestPri   = enPri;
        }
    }

    return;
}



STATIC VOS_UINT32  FC_POLICY_AddPointForPri
(
    FC_POLICY_STRU                     *pPolicy,
    FC_ID_ENUM_UINT8                   enFcId,
    FC_PRI_ENUM_UINT8                  enPointPri
)
{
    FC_PRI_STRU                        *pFcPri;
    VOS_UINT32                          ulFcIdxLoop;

    if (FC_PRI_BUTT <= enPointPri)
    {
        FC_LOG(PS_PRINT_ERROR, "FC_POLICY_AddPointForPri, ERROR, enPointPri overflow!\n");

        return VOS_ERR;
    }

    pFcPri  = &(pPolicy->astFcPri[enPointPri]);

    if ( pFcPri->ucFcIdCnt >= FC_MAX_POINT_NUM  )
    {
        FC_LOG1(PS_PRINT_WARNING, "FC_POLICY_AddPointForPri, WARNING, added FcId overflow\n",
                (VOS_INT32)enFcId);
        return VOS_ERR;
    }

    /* �����ǰ���ȼ��Ѵ��ڣ�����Ӹ����ص� */
    if (VOS_TRUE == pFcPri->ucValid)
    {
        for (ulFcIdxLoop = 0; ulFcIdxLoop < pFcPri->ucFcIdCnt; ulFcIdxLoop++)
        {
            if (enFcId == pFcPri->aenFcId[ulFcIdxLoop])
            {
                /* �Ѿ�ע�����ͬID��ֱ�ӷ��� */
                FC_LOG1(PS_PRINT_WARNING, "FC_POLICY_AddPointForPri, WARNING, add added FcId %d\n",
                    (VOS_INT32)enFcId);
                return VOS_ERR;
            }
        }

        pFcPri->aenFcId[pFcPri->ucFcIdCnt]  = enFcId;
        pFcPri->ucFcIdCnt++;
    }
    /* �����ǰ���ȼ������ڣ�������ȼ���Ϊ��Ч */
    else
    {
        pFcPri->aenFcId[0]  = enFcId;
        pFcPri->ucFcIdCnt   = 1;
        pFcPri->ucValid     = VOS_TRUE;
        pPolicy->ucPriCnt++;
    }

    return VOS_OK;
}



STATIC VOS_UINT32  FC_POLICY_DelPointForPri
(
    FC_POLICY_STRU                     *pPolicy,
    FC_ID_ENUM_UINT8                   enFcId,
    FC_PRI_ENUM_UINT8                  enPointPri
)
{
    FC_PRI_STRU                        *pFcPri;
    VOS_UINT32                          ulFcIdIdxLoop;
    VOS_UINT32                          ulFcShiftId;

    if (FC_PRI_BUTT <= enPointPri)
    {
        FC_LOG(PS_PRINT_ERROR, "FC_POLICY_DelPointForPri, ERROR, enPointPri overflow!\n");
        return VOS_ERR;
    }

    pFcPri  = &(pPolicy->astFcPri[enPointPri]);

    for (ulFcIdIdxLoop = 0; ulFcIdIdxLoop < pFcPri->ucFcIdCnt; ulFcIdIdxLoop++)
    {
        if (enFcId == pFcPri->aenFcId[ulFcIdIdxLoop])
        {
            /* ɾ�����ص㣬����������λ */
            pFcPri->aenFcId[ulFcIdIdxLoop]  = 0;
            for (ulFcShiftId = (ulFcIdIdxLoop + 1); ulFcShiftId < pFcPri->ucFcIdCnt; ulFcShiftId++)
            {
                pFcPri->aenFcId[ulFcShiftId - 1]    = pFcPri->aenFcId[ulFcShiftId];
                pFcPri->aenFcId[ulFcShiftId]        = 0;
            }

            pFcPri->ucFcIdCnt--;

            /* �����ȼ�ɾ�������е����ص㣬��ɾ�������ȼ� */
            if (0 == pFcPri->ucFcIdCnt)
            {
                pFcPri->ucValid     = PS_FALSE;
                pPolicy->ucPriCnt--;
            }

            return VOS_OK;
        }
    }

    return VOS_OK;
}



STATIC VOS_UINT32  FC_POLICY_AddPoint
(
    FC_PRIVATE_POLICY_ID_ENUM_UINT8     enPolicyId,
    FC_ID_ENUM_UINT8                   enFcId,
    FC_PRI_ENUM_UINT8                  enPointPri
)
{
    FC_POLICY_STRU                     *pPolicy;

    pPolicy     = FC_POLICY_Get(enPolicyId);

    /* �����ز���������ص� */
    FC_POLICY_AddPointForPri(pPolicy, enFcId, enPointPri);

    /* ���¸����ز���������ȼ� */
    FC_POLICY_UpdateHighestPri(pPolicy);

    /* �������ز��ԣ��������ص㣬�����������޽������µ��� */
    if (VOS_NULL_PTR != pPolicy->pAdjustForUpFunc)
    {
        (VOS_VOID)pPolicy->pAdjustForUpFunc(enPointPri, enFcId);
    }

    /* �����ص�������о����� */
    /* ���ע����������ȼ����ڵ�ǰ���ȼ�����ִ�и����ص������ */
    if (enPointPri <= pPolicy->enDonePri)
    {
        (VOS_VOID)FC_POINT_SetFc( FC_POLICY_MASK(enPolicyId), enFcId );
    }

    /* �����ά�ɲ� */
    (VOS_VOID)FC_MNTN_TracePolicy(ID_FC_MNTN_POLICY_ADD_POINT, pPolicy);

    return VOS_OK;
}



STATIC FC_PRI_ENUM_UINT8  FC_POLICY_GetPriWithFcId
(
    FC_PRIVATE_POLICY_ID_ENUM_UINT8     enPolicyId,
    FC_ID_ENUM_UINT8                   enFcId
)
{
    FC_POLICY_STRU                     *pPolicy;
    FC_PRI_STRU                        *pPri;
    FC_PRI_ENUM_UINT8                   enPriLoop;
    VOS_UINT32                          ulFcIdIndexLoop;
    FC_PRI_ENUM_UINT8                   enPointPri;

    pPolicy     = FC_POLICY_Get(enPolicyId);

    for (enPriLoop = FC_PRI_1; enPriLoop < FC_PRI_BUTT; enPriLoop++)
    {
        pPri    = &(pPolicy->astFcPri[enPriLoop]);

        if (FC_MAX_POINT_NUM < pPri->ucFcIdCnt)
        {
            FC_LOG(PS_PRINT_ERROR, "FC_POLICY_GetPriWithFcId, ERROR, ucFcIdCnt overflow!\n");

            continue;
        }

        for (ulFcIdIndexLoop = 0; ulFcIdIndexLoop < pPri->ucFcIdCnt; ulFcIdIndexLoop++)
        {
            if (pPri->aenFcId[ulFcIdIndexLoop] == enFcId)
            {
                /* �Ӹ����ز�����ɾ�������ص� */
                enPointPri  = enPriLoop;
                return enPointPri;
            }
        }
    }

    return FC_PRI_BUTT;
}



STATIC VOS_UINT32  FC_POLICY_DelPoint
(
    FC_PRIVATE_POLICY_ID_ENUM_UINT8     enPolicyId,
    FC_ID_ENUM_UINT8                   enFcId
)
{
    FC_PRI_ENUM_UINT8                  enPointPri;
    FC_POLICY_STRU                     *pPolicy;

    /*================================*//* ����Ҫɾ�������ص���н�����ش��� */
    (VOS_VOID)FC_POINT_ClrFc(FC_POLICY_MASK(enPolicyId), enFcId);

    /*================================*//* ������������ز��Խ��е��� */
    enPointPri  = FC_POLICY_GetPriWithFcId(enPolicyId, enFcId);
    if (FC_PRI_BUTT == enPointPri)
    {
        return VOS_OK;
    }

    pPolicy = FC_POLICY_Get(enPolicyId);
    (VOS_VOID)FC_POLICY_DelPointForPri(pPolicy, enFcId, enPointPri);

    /* ���¸����ز���������ȼ� */
    FC_POLICY_UpdateHighestPri(pPolicy);

    /* ���ز��Ե��� */
    if (VOS_NULL_PTR != pPolicy->pAdjustForDownFunc)
    {
        (VOS_VOID)pPolicy->pAdjustForDownFunc(enPointPri, enFcId);
    }

    /* �����ά�ɲ� */
    (VOS_VOID)FC_MNTN_TracePolicy(ID_FC_MNTN_POLICY_DEL_POINT, pPolicy);

    return VOS_OK;
}



STATIC VOS_UINT32  FC_POLICY_ChangePoint
(
    FC_PRIVATE_POLICY_ID_ENUM_UINT8     enPolicyId,
    FC_ID_ENUM_UINT8                   enFcId,
    FC_PRI_ENUM_UINT8                  enPointOldPri,
    FC_PRI_ENUM_UINT8                  enPointNewPri
)
{
    FC_POLICY_STRU                     *pPolicy;

    pPolicy = FC_POLICY_Get(enPolicyId);

    /* ˢ�¸����ص������ز��Ե����ص�ά�� */
    (VOS_VOID)FC_POLICY_DelPointForPri(pPolicy, enFcId, enPointOldPri);
    FC_POLICY_AddPointForPri(pPolicy, enFcId, enPointNewPri);


    /* ���¸����ز���������ȼ� */
    FC_POLICY_UpdateHighestPri(pPolicy);

    /* �������ز��ԣ��������ص㣬�����������޽������µ��� */
    if (VOS_NULL_PTR != pPolicy->pAdjustForUpFunc)
    {
        (VOS_VOID)pPolicy->pAdjustForUpFunc(enPointOldPri, enFcId);
    }

    /* ������ȼ��ĸ��Ľ������ص�״̬�ĵ��� */
    /* ����ϵ����ȼ����ڵ�ǰ���ȼ����µ����ȼ����ڵ�ǰ���ȼ������������ص������ */
    if ( (enPointOldPri < pPolicy->enDonePri) && (enPointNewPri > pPolicy->enDonePri) )
    {
        (VOS_VOID)FC_POINT_ClrFc(FC_POLICY_MASK(pPolicy->enPolicyId), enFcId);
    }

    /* ����ϵ����ȼ����ڵ�ǰ���ȼ����µ����ȼ����ڵ�ǰ���ȼ�����Ը����ص�������� */
    if ( (enPointOldPri > pPolicy->enDonePri) && (enPointNewPri < pPolicy->enDonePri) )
    {
        (VOS_VOID)FC_POINT_SetFc(FC_POLICY_MASK(pPolicy->enPolicyId), enFcId);
    }

    /* �����ά�ɲ� */
    (VOS_VOID)FC_MNTN_TracePolicy(ID_FC_MNTN_POLICY_CHANGE_POINT, pPolicy);

    return VOS_OK;
}



STATIC VOS_UINT32  FC_POLICY_Init( VOS_VOID )
{
    FC_POLICY_ID_ENUM_UINT8             enPolicyId;
    VOS_UINT32                          ulFcPrivatePolicyId;

    ulFcPrivatePolicyId                 = sizeof(FC_POLICY_STRU) * FC_PRIVATE_POLICY_ID_BUTT;

    /*ͨ�����ز��Գ�ʼ�� */
    PSACORE_MEM_SET(&g_astFcPolicy[0], ulFcPrivatePolicyId, 0x00, ulFcPrivatePolicyId);

    for (enPolicyId = 0; enPolicyId < FC_PRIVATE_POLICY_ID_BUTT; enPolicyId++)
    {
        g_astFcPolicy[enPolicyId].enPolicyId    = (VOS_UINT8)enPolicyId;
    }

    return VOS_OK;
}


VOS_UINT32  FC_CFG_CheckCpuParam( FC_CFG_CPU_STRU *pstFcCfgCpu )
{
    if ( pstFcCfgCpu->ulCpuOverLoadVal < pstFcCfgCpu->ulCpuUnderLoadVal)
    {
        FC_LOG2(PS_PRINT_WARNING,
            "FC, FC_CFG_CheckCpuParam, WARNING, overLoad %d less than underLoad %d\n",
            (VOS_INT32)pstFcCfgCpu->ulCpuOverLoadVal, (VOS_INT32)pstFcCfgCpu->ulCpuUnderLoadVal);
        return VOS_ERR;
    }

    if ( (pstFcCfgCpu->ulCpuOverLoadVal == 0) || (pstFcCfgCpu->ulCpuOverLoadVal > 100))
    {
        FC_LOG2(PS_PRINT_WARNING,
            "FC, FC_CFG_CheckCpuParam, WARNING, invalid value overLoad %d, underLoad %d\n",
            (VOS_INT32)pstFcCfgCpu->ulCpuOverLoadVal, (VOS_INT32)pstFcCfgCpu->ulCpuUnderLoadVal);
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 FC_SndCpuMsg
(
    FC_MSG_TYPE_ENUM_UINT16 usMsgName,
    VOS_UINT32              ulCpuLoad,
    VOS_UINT32              ulUlRate,
    VOS_UINT32              ulDlRate
)
{
    VOS_UINT32                          ulResult;
    FC_CPULOAD_IND_STRU                *pstMsg;

    pstMsg = (FC_CPULOAD_IND_STRU *)VOS_AllocMsg(UEPS_PID_FLOWCTRL, \
                                    sizeof(FC_CPULOAD_IND_STRU) - VOS_MSG_HEAD_LENGTH);

    if(VOS_NULL_PTR == pstMsg)
    {
        /* �ú��������ж��б����ã���ʹ��LogPrint */
        FC_LOG(PS_PRINT_ERROR, "FC_SndIntraCpuMsg, ERROR, Alloc Msg Fail\n");
        return VOS_ERR;
    }

    pstMsg->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMsg->ulReceiverPid   = UEPS_PID_FLOWCTRL;
    pstMsg->usMsgName       = usMsgName;
    pstMsg->ulCpuLoad       = ulCpuLoad;
    pstMsg->ulUlRate        = ulUlRate;
    pstMsg->ulDlRate        = ulDlRate;

    ulResult = VOS_SendMsg(UEPS_PID_FLOWCTRL, pstMsg);

    if (VOS_OK != ulResult)
    {
        /* �ú��������ж��б����ã���ʹ��LogPrint */
        FC_LOG(PS_PRINT_ERROR, "FC_SndIntraCpuMsg, ERROR, Send Msg Fail\n");
        return VOS_ERR;
    }

    return VOS_OK;
}


STATIC VOS_UINT32 FC_SndRegPointMsg
(
    FC_REG_POINT_STRU                  *pstFcRegPoint
)
{
    VOS_UINT32                          ulResult;
    FC_REG_POINT_IND_STRU              *pstMsg;

    pstMsg = (FC_REG_POINT_IND_STRU *)VOS_AllocMsg(UEPS_PID_FLOWCTRL, \
                                      sizeof(FC_REG_POINT_IND_STRU) - VOS_MSG_HEAD_LENGTH);

    if(VOS_NULL_PTR == pstMsg)
    {
        FC_LOG(PS_PRINT_ERROR,"FC_SndRegPointMsg, Alloc Msg Fail\n");
        return VOS_ERR;
    }

    pstMsg->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMsg->ulReceiverPid   = UEPS_PID_FLOWCTRL;
    pstMsg->usMsgName       = ID_FC_REG_POINT_IND;

    PSACORE_MEM_CPY(&(pstMsg->stFcPoint), sizeof(FC_REG_POINT_STRU), pstFcRegPoint, sizeof(FC_REG_POINT_STRU));

    ulResult = VOS_SendMsg(UEPS_PID_FLOWCTRL, pstMsg);

    if (VOS_OK != ulResult)
    {
        FC_LOG(PS_PRINT_ERROR,"FC_SndRegPointMsg, Send Msg Fail\n");
        return VOS_ERR;
    }

    return VOS_OK;
}



STATIC VOS_UINT32 FC_SndDeRegPointMsg
(
    FC_ID_ENUM_UINT8                   enFcId,
    MODEM_ID_ENUM_UINT16                enModemId
)
{
    VOS_UINT32                          ulResult;
    FC_DEREG_POINT_IND_STRU            *pstMsg;

    pstMsg = (FC_DEREG_POINT_IND_STRU *)VOS_AllocMsg(UEPS_PID_FLOWCTRL, \
                                      sizeof(FC_DEREG_POINT_IND_STRU) - VOS_MSG_HEAD_LENGTH);

    if(VOS_NULL_PTR == pstMsg)
    {
        FC_LOG(PS_PRINT_ERROR,"FC_SndDeRegPointMsg, Alloc Msg Fail\n");
        return VOS_ERR;
    }

    pstMsg->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMsg->ulReceiverPid   = UEPS_PID_FLOWCTRL;
    pstMsg->usMsgName       = ID_FC_DEREG_POINT_IND;
    pstMsg->enFcId          = enFcId;
    pstMsg->enModemId       = enModemId;

    ulResult = VOS_SendMsg(UEPS_PID_FLOWCTRL, pstMsg);

    if (VOS_OK != ulResult)
    {
        FC_LOG(PS_PRINT_ERROR,"FC_SndDeRegPointMsg, Send Msg Fail\n");
        return VOS_ERR;
    }

    return VOS_OK;
}


STATIC VOS_UINT32 FC_SndChangePointMsg
(
    FC_ID_ENUM_UINT8                   enFcId,
    FC_POLICY_ID_ENUM_UINT8             enPolicyId,
    FC_PRI_ENUM_UINT8                  enPri,
    MODEM_ID_ENUM_UINT16                enModemId
)
{
    VOS_UINT32                          ulResult;
    FC_CHANGE_POINT_IND_STRU           *pstMsg;

    pstMsg = (FC_CHANGE_POINT_IND_STRU *)VOS_AllocMsg(UEPS_PID_FLOWCTRL, \
                                         sizeof(FC_CHANGE_POINT_IND_STRU) - VOS_MSG_HEAD_LENGTH);

    if(VOS_NULL_PTR == pstMsg)
    {
        FC_LOG(PS_PRINT_ERROR,"FC_SndChangePointMsg, Alloc Msg Fail\n");
        return VOS_ERR;
    }

    pstMsg->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMsg->ulReceiverPid   = UEPS_PID_FLOWCTRL;
    pstMsg->usMsgName       = ID_FC_CHANGE_POINT_IND;
    pstMsg->enFcId          = enFcId;
    pstMsg->enPolicyId      = enPolicyId;
    pstMsg->enPri           = enPri;
    pstMsg->enModemId       = enModemId;

    ulResult = VOS_SendMsg(UEPS_PID_FLOWCTRL, pstMsg);

    if (VOS_OK != ulResult)
    {
        FC_LOG(PS_PRINT_ERROR,"FC_SndChangePointMsg, Send Msg Fail\n");
        return VOS_ERR;
    }

    return VOS_OK;
}



VOS_UINT32  FC_CommInit( VOS_VOID )
{
    /* ���ز��Գ�ʼ�� */
    FC_POLICY_Init();

    /* ���ص��ʼ�� */
    FC_POINT_Init();

    return VOS_OK;
}

/*lint -restore */
/*lint +e534*/


