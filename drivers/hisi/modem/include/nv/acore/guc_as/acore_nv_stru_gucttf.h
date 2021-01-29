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


#ifndef __ACORE_NV_STRU_GUCTTF_H__
#define __ACORE_NV_STRU_GUCTTF_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  1 Include Headfile
*****************************************************************************/
#include "vos.h"
#include "acore_nv_id_gucttf.h"

#define FC_ACPU_DRV_ASSEM_NV_LEV            (4)


enum FC_ACPU_DRV_ASSEM_LEV_ENUM
{
    FC_ACPU_DRV_ASSEM_LEV_1             = 0,
    FC_ACPU_DRV_ASSEM_LEV_2             = 1,
    FC_ACPU_DRV_ASSEM_LEV_3             = 2,
    FC_ACPU_DRV_ASSEM_LEV_4             = 3,
    FC_ACPU_DRV_ASSEM_LEV_5             = 4,
    FC_ACPU_DRV_ASSEM_LEV_BUTT          = 5
};
typedef VOS_UINT32  FC_ACPU_DRV_ASSEM_LEV_ENUM_UINT32;


enum FC_MEM_THRESHOLD_LEV_ENUM
{
    FC_MEM_THRESHOLD_LEV_1              = 0,
    FC_MEM_THRESHOLD_LEV_2              = 1,
    FC_MEM_THRESHOLD_LEV_3              = 2,
    FC_MEM_THRESHOLD_LEV_4              = 3,
    FC_MEM_THRESHOLD_LEV_5              = 4,
    FC_MEM_THRESHOLD_LEV_6              = 5,
    FC_MEM_THRESHOLD_LEV_7              = 6,
    FC_MEM_THRESHOLD_LEV_8              = 7,
    FC_MEM_THRESHOLD_LEV_BUTT           = 8
};
typedef VOS_UINT32  FC_MEM_THRESHOLD_LEV_ENUM_UINT32;

/*****************************************************************************
 ö����    : TTF_BOOL_ENUM
 Э����  :
 ASN.1���� :
 ö��˵��  : TTFͳһ��������ö�ٶ���
*****************************************************************************/
enum TTF_ACORE_BOOL_ENUM
{
    TTF_ACORE_FALSE                     = 0,
    TTF_ACORE_TRUE                      = 1,

    TTF_ACORE_BOOL_BUTT
};
typedef VOS_UINT8   TTF_ACORE_BOOL_ENUM_UINT8;

/*****************************************************************************
 �ṹ��    : FC_CPU_DRV_ASSEM_PARA_STRU
 DESCRIPTION: ����CPU LOAD��̬���������������
*****************************************************************************/
typedef struct
{
    VOS_UINT8                          ucHostOutTimeout;    /* PC�������ʱ�� */
    VOS_UINT8                          ucEthTxMinNum;       /* UE��������������� */
    VOS_UINT8                          ucEthTxTimeout;      /* UE�����������ʱ�� */
    VOS_UINT8                          ucEthRxMinNum;       /* UE��������������� */
    VOS_UINT8                          ucEthRxTimeout;      /* UE�����������ʱ�� */
    VOS_UINT8                          ucCdsGuDlThres;      /* �ѷ��� */
    VOS_UINT8                          aucRsv[2];
}FC_DRV_ASSEM_PARA_STRU;

/*****************************************************************************
 �ṹ��    : FC_CPU_DRV_ASSEM_PARA_STRU
 DESCRIPTION: ����CPU LOAD��̬���������������
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulCpuLoad;                              /* CPU����,Range:[0,100] */
    FC_DRV_ASSEM_PARA_STRU              stDrvAssemPara;
}FC_CPU_DRV_ASSEM_PARA_STRU;

/*****************************************************************************
 �ṹ��    : FC_ACORE_CFG_CPU_STRU
 DESCRIPTION: FC_ACORE_CFG_CPU�ṹ,CPU���ص����޺�����ֵ
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulCpuOverLoadVal;                       /* CPU������ˮ�ߣ�ȡֵ��Χ[1,100]����ʾCPUռ���ʴ�С��*/
    VOS_UINT32                          ulCpuUnderLoadVal;                      /* CPUͣ����ˮ�ߣ�ȡֵ��Χ[1,ulCpuOverLoadVal]����ʾCPUռ���ʴ�С��*/
    VOS_UINT32                          ulSmoothTimerLen;                       /* CPU��������ƽ��������ȡֵ��Χ[2,1000]����λ��CPU������ڡ�*/
    VOS_UINT32                          ulStopAttemptTimerLen;                  /* CPU���شﵽ��߼���������ʱ�����Խ����أ��ö�ʱ����ʱ������λ���룬ȡֵ��Χ���ڵ���0��0��ʾ��ʹ�á�*/
    VOS_UINT32                          ulUmUlRateThreshold;                    /* ��������ˮ�ߣ��������ʳ���ˮ�߲ſ�����CPU���أ���λbps��ȡֵ��Χ���ڵ���0��*/
    VOS_UINT32                          ulUmDlRateThreshold;                    /* ��������ˮ�ߣ��������ʳ���ˮ�߲ſ�����CPU���أ���λbps��ȡֵ��Χ���ڵ���0��*/
    VOS_UINT32                          ulRmRateThreshold;                      /* ��������ˮ�ߣ��������ʳ���ˮ�߲ſ�����CPU���أ���λbps��ȡֵ��Χ���ڵ���0��*/
} FC_ACORE_CFG_CPU_STRU;

/*****************************************************************************
 �ṹ��    : FC_ACORE_CFG_MEM_THRESHOLD_STRU
 DESCRIPTION: FC_ACORE_CFG_MEM_THRESHOLD�ṹ,MEM���ص����޺�����ֵ
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulSetThreshold;                         /* ������������ ��λ�ֽ� */
    VOS_UINT32                          ulStopThreshold;                        /* ֹͣ�������� ��λ�ֽ� */
} FC_ACORE_CFG_MEM_THRESHOLD_STRU;

/*****************************************************************************
 �ṹ��    : FC_CFG_MEM_THRESHOLD_CST_STRU
 DESCRIPTION: FC_CFG_MEM_THRESHOLD_CST�ṹ,MEM���ص����޺�����ֵ
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulSetThreshold;                         /*Range:[0,4096]*//* ������������ ��λ�ֽ� */
    VOS_UINT32                          ulStopThreshold;                        /*Range:[0,4096]*//* ֹͣ�������� ��λ�ֽ� */
} FC_CFG_MEM_THRESHOLD_CST_STRU;

/*****************************************************************************
 �ṹ��    : NV_TTF_PPP_CONFIG_STRU
 DESCRIPTION: ��Ӧen_NV_Item_PPP_CONFIG��Ľṹ
*****************************************************************************/
typedef struct
{
    TTF_ACORE_BOOL_ENUM_UINT8           enChapEnable;           /* �Ƿ�ʹ��Chap��Ȩ */
    TTF_ACORE_BOOL_ENUM_UINT8           enPapEnable;            /* �Ƿ�ʹ��Pap��Ȩ */
    VOS_UINT16                          usLcpEchoMaxLostCnt;    /* ����LcpEchoRequest�������������� */

    VOS_UINT16                          usQueneMaxCnt;          /* �������������� */
    VOS_UINT8                           aucRsv[2];
}NV_TTF_PPP_CONFIG_STRU;

/*****************************************************************************
 �ṹ��    : CPULOAD_CFG_STRU
 DESCRIPTION: ��Ӧen_NV_Item_Linux_CPU_Moniter_Timer_Len��Ľṹ,A��CPUռ���ʼ������ʱ���ʱ��
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulMonitorTimerLen;
} CPULOAD_CFG_STRU;

/*****************************************************************************
 �ṹ��    : FC_CPU_DRV_ASSEM_PARA_NV_STRU
 DESCRIPTION: ��Ӧen_NV_Item_FC_ACPU_DRV_ASSEMBLE_PARA��Ľṹ������CPU LOAD��̬���������������
*****************************************************************************/
typedef struct
{
    VOS_UINT8                              ucEnableMask;                        /* ʹ�ܿ��ƣ�0x0:��ʹ�ܣ�0x1:ʹ�� */
    VOS_UINT8                              ucSmoothCntUpLev;                    /* ���ϵ���ƽ��ϵ�� */
    VOS_UINT8                              ucSmoothCntDownLev;                  /* ���µ���ƽ��ϵ�� */
    VOS_UINT8                              ucRsv;
    FC_CPU_DRV_ASSEM_PARA_STRU             stCpuDrvAssemPara[FC_ACPU_DRV_ASSEM_NV_LEV]; /* CPU���ò��� */
}FC_CPU_DRV_ASSEM_PARA_NV_STRU;

/*****************************************************************************
 �ṹ��    : NF_EXT_NV_STRU
 DESCRIPTION: ��Ӧen_NV_Item_NETFILTER_HOOK_MASK��Ľṹ,���ù������NV�Ԥ��5���������:
             ulNvValue1~ulNvValue5, ÿ������ȡֵ��ΧΪ0-FFFFFFFF,
             ��������λΪ1����������λ��Ӧ�Ĺ��Ӻ������ܻᱻע�ᵽ�ں���
*****************************************************************************/
typedef struct
{
    VOS_UINT32          ulNetfilterPara1;                                       /* ���Ӻ����������1 */
    VOS_UINT32          ulNetfilterPara2;                                       /* ���Ӻ����������2 */
    VOS_UINT32          ulNetfilterPara3;                                       /* ���Ӻ����������3 */
    VOS_UINT32          ulNetfilterPara4;                                       /* ���Ӻ����������4 */
    VOS_UINT32          ulNetfilterPara5;                                       /* ���Ӻ����������5 */
}NF_EXT_NV_STRU;

/*****************************************************************************
�ṹ��    : PPP_CONFIG_MRU_TYPE_NV_STRU
DESCRIPTION: ��Ӧen_NV_Item_PPP_CONFIG_MRU_Type��Ľṹ,Ĭ��Э��MTU����������PPP LCPЭ��
*****************************************************************************/
typedef struct
{
    VOS_UINT16                           usPppConfigType;   /* Ĭ��MRU��С,Range:[296,1500]*/
    VOS_UINT8                            aucReserve[2];
}PPP_CONFIG_MRU_TYPE_NV_STRU;

/*****************************************************************************
 �ṹ��    : FC_CFG_CST_STRU
 Э����  : ��
 ASN.1���� : ��
 �ṹ˵��  : CST��������
*****************************************************************************/
typedef struct
{
    FC_ACORE_CFG_MEM_THRESHOLD_STRU     stThreshold;                            /* CST���λ����������ˮ�ߣ���λ�ֽڣ��ܳ�4096��*/
} FC_CFG_CST_STRU;

/*****************************************************************************
 �ṹ��    : FC_CFG_MEM_STRU
 Э����  : ��
 ASN.1���� : ��
 �ṹ˵��  : MEM���ص����޺�����ֵ
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulThresholdCnt;                         /* A���ڴ����ص�λ������ȡֵ��Χ[0,8]��*/
    FC_ACORE_CFG_MEM_THRESHOLD_STRU     astThreshold[FC_MEM_THRESHOLD_LEV_BUTT];/* A���ڴ�����ˮ�����ã�ȡA���ڴ��ʣ����п������*/
} FC_CFG_MEM_STRU;

/*****************************************************************************
 �ṹ��    : ACORE_FC_CFG_NV_STRU
 DESCRIPTION: ��Ӧen_NV_Item_Acore_Flow_Ctrl_Config��Ľṹ
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulFcEnableMask;                         /* A������ʹ�ܿ��ء�ÿ������λ��0��ʾ��������ȥʹ�ܣ�1��ʾʹ�ܡ�Bit0��MEM���أ�Bit1��A��CPU���أ�Bit2��CDS���أ�Bit3��CST���أ�*/
    FC_ACORE_CFG_CPU_STRU               stFcCfgCpuA;                            /* A��CPU����ˮ�����á�*/
    FC_CFG_MEM_STRU                     stFcCfgMem;                             /* A���ڴ�����ˮ�����á�*/
    FC_CFG_CST_STRU                     stFcCfgCst;                             /* CSTҵ������ˮ�����á�*/
} ACORE_FC_CFG_NV_STRU;

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* __ACORE_NV_ID_GUCTTF_H__ */

