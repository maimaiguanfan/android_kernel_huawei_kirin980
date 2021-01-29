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

#ifndef __MDRV_EICC_COMMON_H__
#define __MDRV_EICC_COMMON_H__
#ifdef __cplusplus
extern "C"
{
#endif

#define EICC_CHANNEL_ID_MAKEUP(channel_id, func_id) ((channel_id << 16) | (func_id))

#ifndef BSP_ERR_EICC_BASE
#define BSP_ERR_EICC_BASE               (int)(0x80000000 | 0x10180000)
#endif

/* C�˷�����λ */
#ifndef BSP_ERR_EICC_CCORE_RESETTING
#define BSP_ERR_EICC_CCORE_RESETTING    (BSP_ERR_EICC_BASE + 0x12)
#endif

/*�Ƿ�����ж�*/
#define EICC_ARV_INT_OPEN      (0x1)
#define EICC_ARV_INT_CLOSED    (0)

/* CPU ID ���� */
enum EICC_CPU_ID
{
    EICC_CPU_MIN = 0,

    EICC_CPU_APP = 0,
    EICC_CPU_LPM3 = 1,
    EICC_CPU_NRCCPU = 2,
    EICC_CPU_R8HL1C0 = 3,
    EICC_CPU_R8HL1C1 = 4,
    EICC_CPU_L2HAC0 = 5,
    EICC_CPU_L2HAC1 = 6,
    EICC_CPU_LL1C0 = 7,
    EICC_CPU_LL1C1 = 8,
    EICC_CPU_LL1C2 = 9,
    EICC_CPU_LL1C3 = 10,
    EICC_CPU_LL1C4 = 11,
    EICC_CPU_LL1C5 = 12,
    EICC_CPU_LL1C6 = 13,
    EICC_CPU_LL1C7 = 14,
    EICC_CPU_LRMCCPU = 15,

    EICC_CPU_MAX
};

/* �����붨�� */
enum EICC_ERR_NO
{
    EICC_CHN_INIT_FAIL = (0x80000000 + (0 << 16)),
    EICC_MALLOC_CHANNEL_FAIL,
    EICC_MALLOC_VECTOR_FAIL,
    EICC_CREATE_TASK_FAIL,
    EICC_DEBUG_INIT_FAIL,
    EICC_CREATE_SEM_FAIL,
    EICC_REGISTER_INT_FAIL,
    EICC_INVALID_PARA,
    EICC_WAIT_SEM_TIMEOUT,
    EICC_SEND_ERR,
    EICC_RECV_ERR,
    EICC_REGISTER_CB_FAIL,
    EICC_REGISTER_DPM_FAIL,
    EICC_MALLOC_MEM_FAIL,
    EICC_NULL_PTR,
    EICC_INIT_ADDR_TOO_BIG,
    EICC_INIT_SKIP,
    EICC_CHANNEL_OPEN_FAIL,
    EICC_CHANNEL_CLOSE_FAIL,
    EICC_REGISTER_DYNAMIC_CB_FAIL,
    EICC_DYNAMIC_SEND_FAIL,
    EICC_INVALID_NO_FIFO_SPACE,  /* ͨ���� */
    EICC_INVALID_DMA_MODIFY_TYPE,  /* DMA����MODIFY����*/
    EICC_DMA_MALLOC_DESC_FAIL,  /* DMA����CD�ڴ�ʧ��*/
    EICC_DMA_FREE_DESC_FAIL  /* DMA�ͷ�CD�ڴ�ʧ��*/
};

typedef int (*eicc_msg_cb)(unsigned int channel_id, unsigned int len); /*len:������Ϣ����,д�ص�ʱlen������*/

enum EICC_READ_INT_LEV
{
    EICC_RD_FLDN_INT = 0x1,
    EICC_RD_FLUP_INT = 0x2,
    EICC_RD_DATA_ARV_INT = 0x4,
    EICC_RD_DATA_ARV_TIMEOUT_INT = 0x8,
    EICC_RD_PIPE_WAKEUP_INT = 0x10
};

enum EICC_WRITE_INT_LEV
{
    EICC_WR_FLDN_INT = 0x1,
    EICC_WR_FLUP_INT = 0x2,
    EICC_WR_DATA_DONE_INT = 0x4,
    EICC_WR_PIPE_ERR_INT = 0x8,
    EICC_WR_TRANS_CNT_OVF_INT = 0x10
};

enum CHANL_TYPE
{
    CHANNEL_OUTSIDE_SEND = 0x10, /*�ⲿ����ͨ��*/
    CHANNEL_OUTSIDE_RECV = 0x11, /*�ⲿ����ͨ��*/
    CHANNEL_INSIDE_SEND = 0x12,  /*�ڲ�����ͨ��*/
    CHANNEL_INSIDE_RECV = 0x13,  /*�ڲ�����ͨ��*/  
    CHANNEL_OUTDMA_SEND = 0x14,  /*�ⲿDMA����ͨ��*/
    CHANNEL_OUTDMA_RECV = 0x15,  /*�ⲿDMA����ͨ��*/
    CHANNEL_INSIDE_DMA = 0x16    /*�ڲ�DMAͨ��*/
};

#ifdef CONFIG_EICC_PHY_EMU
/* ͨ��id���� */
/* nx EICC DEV ö�� DEV0~DEV3*/
enum EICC_DEV0_CHN_ID
{
    EICC_SDR_CHN_MIN = 0,            /* EICC_SDR��Сͨ���� */

    EICC_SDR_SEND_LL1C_DL_0 = 0,     /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_SDR_SEND_LL1C_DL_1,         /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_SDR_SEND_LL1C_DL_2,         /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_SDR_SEND_LL1C_DL_3,         /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_SDR_SEND_HL1C,              /* HL1C��SDR��ԭ�ｻ�� */

    EICC_SDR_RECV_LL1C_DL_0,         /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_SDR_RECV_LL1C_DL_1,         /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_SDR_RECV_LL1C_DL_2,         /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_SDR_RECV_LL1C_DL_3,         /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_SDR_RECV_HL1C,              /* HL1C��SDR��ԭ�ｻ�� */

    EICC_SDR_RECV_HL1C_OM,           /* HL1C��SDR��OM��Ϣ*/

    EICC_SDR_SEND_LL1C_DL_4,         /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_SDR_SEND_LL1C_DL_5,         /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_SDR_SEND_LL1C_DL_6,         /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_SDR_SEND_LL1C_DL_7,         /* LL1C DL��SDR��ԭ�ｻ�� */
    
    EICC_LL1C_DL_RECV_LL1C_DL_0,         /* LL1C DL��LL1C DL��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_LL1C_DL_1,         /* LL1C DL��LL1C DL��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_LL1C_DL_2,         /* LL1C DL��LL1C DL��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_LL1C_DL_3,         /* LL1C DL��LL1C DL��ԭ�ｻ�� */

    EICC_SDR_SEND_HL1C_1,            /* HL1C��SDR��ԭ�ｻ�� */
    EICC_SDR_RECV_HL1C_1,            /* HL1C��SDR��ԭ�ｻ�� */

    EICC_SDR_CHN_ID_MAX
};

enum EICC_DEV1_CHN_ID
{
    EICC_HL1C_CHN_MIN = 0x1000,  /* EICC_HL1C_DL��Сͨ���� */

    EICC_HL1C_SEND_LL1C_DL_0 = 0x1000,/* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_HL1C_SEND_LL1C_DL_1,        /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_HL1C_SEND_LL1C_DL_2,        /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_HL1C_SEND_LL1C_DL_3,        /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_HL1C_SEND_SDR,              /* HL1C��SDR��ԭ�ｻ�� */

    EICC_HL1C_RECV_LL1C_DL_0,        /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_HL1C_RECV_LL1C_DL_1,        /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_HL1C_RECV_LL1C_DL_2,        /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_HL1C_RECV_LL1C_DL_3,        /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_HL1C_RECV_SDR,              /* HL1C��SDR��ԭ�ｻ�� */

    EICC_HL1C_SEND_NRCCPU_ULMBX,
    EICC_HL1C_RECV_NRCCPU_DLMBX,
    EICC_HL1C_RECV_NRCCPU_OM,

    EICC_HL1C_SEND_SDR_OM,           /* HL1C��SDR��OM��Ϣ*/
    EICC_HL1C_SEND_LL1C_DL_OM_0,     /* HL1C��LL1C DL��OM��Ϣ*/
    EICC_HL1C_SEND_LL1C_DL_OM_1,     /* HL1C��LL1C DL��OM��Ϣ*/
    EICC_HL1C_SEND_LL1C_DL_OM_2,     /* HL1C��LL1C DL��OM��Ϣ*/
    EICC_HL1C_SEND_LL1C_DL_OM_3,     /* HL1C��LL1C DL��OM��Ϣ*/
    EICC_HL1C_SEND_LL1C_UL_OM_0,     /* HL1C��LL1C UL��OM��Ϣ*/
    EICC_HL1C_SEND_LL1C_UL_OM_1,     /* HL1C��LL1C UL��OM��Ϣ*/

    EICC_HL1C_SEND_LL1C_UL_0,        /* HL1C����Ϣ������ULL1C�ˣ���2*2=4��*/
    EICC_HL1C_SEND_LL1C_UL_1,        /* HL1C����Ϣ������ULL1C�ˣ���2*2=4��*/
    EICC_HL1C_SEND_LL1C_UL_2,        /* HL1C����Ϣ������ULL1C�ˣ���2*2=4��*/
    EICC_HL1C_SEND_LL1C_UL_3,        /* HL1C����Ϣ������ULL1C�ˣ���2*2=4��*/
    
    EICC_HL1C_RECV_LL1C_UL_0,        /* HL1C������Ϣ������ULL1C�ˣ���2*2=4��(��/��ʱ��) */
    EICC_HL1C_RECV_LL1C_UL_1,        /* HL1C������Ϣ������ULL1C�ˣ���2*2=4��(��/��ʱ��) */
    EICC_HL1C_RECV_LL1C_UL_2,        /* HL1C������Ϣ������ULL1C�ˣ���2*2=4��(��/��ʱ��) */
    EICC_HL1C_RECV_LL1C_UL_3,        /* HL1C������Ϣ������ULL1C�ˣ���2*2=4��(��/��ʱ��) */

    EICC_HL1C_SEND_LL1C_UL_SP_0,     /* ר��������Beam Pool��������ULL1C�ˣ���2�� */
    EICC_HL1C_SEND_LL1C_UL_SP_1,     /* ר��������Beam Pool��������ULL1C�ˣ���2�� */

    EICC_HL1C_SEND_BBIC_SEQ_DMA,     /* HL1C��дBBIC SEQ, EICC DMA��ʽ��ÿ������������1*2�� */
    EICC_HL1C_RECV_BBIC_SEQ_DMA,     /* HL1C��дBBIC SEQ, EICC DMA��ʽ��ÿ������������1*2�� */

    EICC_HL1C_SEND_LL1C_DL_4,        /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_HL1C_SEND_LL1C_DL_5,        /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_HL1C_SEND_LL1C_DL_6,        /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_HL1C_SEND_LL1C_DL_7,        /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_HL1C_RECV_LL1C_DL_4,        /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_HL1C_RECV_LL1C_DL_5,        /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_HL1C_RECV_LL1C_DL_6,        /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_HL1C_RECV_LL1C_DL_7,        /* LL1C DL��HL1C��ԭ�ｻ�� */

    EICC_HL1C_SEND_SDR_1,            /* HL1C��SDR��ԭ�ｻ�� */
    EICC_HL1C_RECV_SDR_1,            /* HL1C��SDR��ԭ�ｻ�� */
    
    EICC_HL1C_CHN_ID_MAX
};

enum EICC_DEV2_CHN_ID
{
    EICC_LL1C_DL_CHN_MIN = 0x2000,   /* EICC_LL1C��Сͨ���� */

    EICC_LL1C_DL_SEND_SDR_0 = 0x2000,/* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_LL1C_DL_SEND_SDR_1,        /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_LL1C_DL_SEND_SDR_2,        /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_LL1C_DL_SEND_SDR_3,        /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_LL1C_DL_SEND_HL1C_0,       /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_LL1C_DL_SEND_HL1C_1,       /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_LL1C_DL_SEND_HL1C_2,       /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_LL1C_DL_SEND_HL1C_3,       /* LL1C DL��HL1C��ԭ�ｻ�� */

    EICC_LL1C_DL_RECV_SDR_0,        /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_SDR_1,        /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_SDR_2,        /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_SDR_3,        /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_HL1C_0,       /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_HL1C_1,       /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_HL1C_2,       /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_HL1C_3,       /* LL1C DL��HL1C��ԭ�ｻ�� */

    EICC_LL1C_DL_RECV_HL1C_OM_0,    /* HL1C��LL1C DL��OM��Ϣ*/
    EICC_LL1C_DL_RECV_HL1C_OM_1,    /* HL1C��LL1C DL��OM��Ϣ*/
    EICC_LL1C_DL_RECV_HL1C_OM_2,    /* HL1C��LL1C DL��OM��Ϣ*/
    EICC_LL1C_DL_RECV_HL1C_OM_3,    /* HL1C��LL1C DL��OM��Ϣ*/

    EICC_LL1C_DL_0_SEND_LL1C_UL_0,  /* 4��DL1C����Ϣ������ULL1C�ˣ���8�� */
    EICC_LL1C_DL_1_SEND_LL1C_UL_0,  /* 4��DL1C����Ϣ������ULL1C�ˣ���8�� */
    EICC_LL1C_DL_2_SEND_LL1C_UL_0,  /* 4��DL1C����Ϣ������ULL1C�ˣ���8�� */
    EICC_LL1C_DL_3_SEND_LL1C_UL_0,  /* 4��DL1C����Ϣ������ULL1C�ˣ���8�� */
    EICC_LL1C_DL_0_SEND_LL1C_UL_1,  /* 4��DL1C����Ϣ������ULL1C�ˣ���8�� */
    EICC_LL1C_DL_1_SEND_LL1C_UL_1,  /* 4��DL1C����Ϣ������ULL1C�ˣ���8�� */
    EICC_LL1C_DL_2_SEND_LL1C_UL_1,  /* 4��DL1C����Ϣ������ULL1C�ˣ���8�� */
    EICC_LL1C_DL_3_SEND_LL1C_UL_1,  /* 4��DL1C����Ϣ������ULL1C�ˣ���8�� */
    EICC_LL1C_DL_0_RECV_LL1C_UL_0,  /* 4��DL1C������Ϣ������ULL1C�ˣ���8�� */
    EICC_LL1C_DL_1_RECV_LL1C_UL_0,  /* 4��DL1C������Ϣ������ULL1C�ˣ���8�� */
    EICC_LL1C_DL_2_RECV_LL1C_UL_0,  /* 4��DL1C������Ϣ������ULL1C�ˣ���8�� */
    EICC_LL1C_DL_3_RECV_LL1C_UL_0,  /* 4��DL1C������Ϣ������ULL1C�ˣ���8�� */
    EICC_LL1C_DL_0_RECV_LL1C_UL_1,  /* 4��DL1C������Ϣ������ULL1C�ˣ���8�� */
    EICC_LL1C_DL_1_RECV_LL1C_UL_1,  /* 4��DL1C������Ϣ������ULL1C�ˣ���8�� */
    EICC_LL1C_DL_2_RECV_LL1C_UL_1,  /* 4��DL1C������Ϣ������ULL1C�ˣ���8�� */
    EICC_LL1C_DL_3_RECV_LL1C_UL_1,  /* 4��DL1C������Ϣ������ULL1C�ˣ���8�� */

    EICC_LL1C_DL_SEND_LL1C_DL_0,        /* LL1C DL��LL1C_DL��ԭ�ｻ�� */
    EICC_LL1C_DL_SEND_LL1C_DL_1,        /* LL1C DL��LL1C_DL��ԭ�ｻ�� */
    EICC_LL1C_DL_SEND_LL1C_DL_2,        /* LL1C DL��LL1C_DL��ԭ�ｻ�� */
    EICC_LL1C_DL_SEND_LL1C_DL_3,        /* LL1C DL��LL1C_DL��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_SDR_4,        /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_SDR_5,        /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_SDR_6,        /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_SDR_7,        /* LL1C DL��SDR��ԭ�ｻ�� */

    EICC_LL1C_DL_SEND_HL1C_4,       /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_LL1C_DL_SEND_HL1C_5,       /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_LL1C_DL_SEND_HL1C_6,       /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_LL1C_DL_SEND_HL1C_7,       /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_HL1C_4,       /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_HL1C_5,       /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_HL1C_6,       /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_HL1C_7,       /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_LL1C_UL_1_RECV_LL1C_UL_0,  /* ����ULL1C���໥������Ϣ����2�� */
    EICC_LL1C_UL_0_RECV_LL1C_UL_1,  /* ����ULL1C���໥������Ϣ����2�� */

    EICC_LL1C_DL_CHN_ID_MAX
};

enum EICC_DEV3_CHN_ID
{
    EICC_LL1C_UL_CHN_MIN = 0x3000,        /* EICC_LL1C_UL��Сͨ���� */

    EICC_NRCCPU_RECV_HL1C_ULMBX = 0x3000, /* PCģ����ʱ���� */
    EICC_NRCCPU_SEND_HL1C_DLMBX,      /* PCģ����ʱ���� */
    EICC_NRCCPU_SEND_HL1C_OM,         /* PCģ����ʱ���� */

    EICC_LL1C_UL_RECV_HL1C_OM_0,      /* LL1C UL��HL1C��OM��Ϣ*/
    EICC_LL1C_UL_RECV_HL1C_OM_1,      /* LL1C UL��HL1C��OM��Ϣ*/

    EICC_LL1C_UL_SEND_HL1C_0,         /* ����ULL1C�˷���Ϣ��HL1C����2*2=4��*/
    EICC_LL1C_UL_SEND_HL1C_1,         /* ����ULL1C�˷���Ϣ��HL1C����2*2=4��*/
    EICC_LL1C_UL_SEND_HL1C_2,         /* ����ULL1C�˷���Ϣ��HL1C����2*2=4��*/
    EICC_LL1C_UL_SEND_HL1C_3,         /* ����ULL1C�˷���Ϣ��HL1C����2*2=4��*/

    EICC_LL1C_UL_RECV_HL1C_0,         /* ����ULL1C�˽�����Ϣ��HL1C����2*2=4��*/
    EICC_LL1C_UL_RECV_HL1C_1,         /* ����ULL1C�˽�����Ϣ��HL1C����2*2=4��*/
    EICC_LL1C_UL_RECV_HL1C_2,         /* ����ULL1C�˽�����Ϣ��HL1C����2*2=4��*/
    EICC_LL1C_UL_RECV_HL1C_3,         /* ����ULL1C�˽�����Ϣ��HL1C����2*2=4��*/

    EICC_LL1C_UL_0_SEND_LL1C_DL_0,    /* ����ULL1C�˷���Ϣ��4��DL1C����8�� */
    EICC_LL1C_UL_0_SEND_LL1C_DL_1,    /* ����ULL1C�˷���Ϣ��4��DL1C����8�� */
    EICC_LL1C_UL_0_SEND_LL1C_DL_2,    /* ����ULL1C�˷���Ϣ��4��DL1C����8�� */
    EICC_LL1C_UL_0_SEND_LL1C_DL_3,    /* ����ULL1C�˷���Ϣ��4��DL1C����8�� */
    EICC_LL1C_UL_1_SEND_LL1C_DL_0,    /* ����ULL1C�˷���Ϣ��4��DL1C����8�� */
    EICC_LL1C_UL_1_SEND_LL1C_DL_1,    /* ����ULL1C�˷���Ϣ��4��DL1C����8�� */
    EICC_LL1C_UL_1_SEND_LL1C_DL_2,    /* ����ULL1C�˷���Ϣ��4��DL1C����8�� */
    EICC_LL1C_UL_1_SEND_LL1C_DL_3,    /* ����ULL1C�˷���Ϣ��4��DL1C����8�� */
    EICC_LL1C_UL_0_RECV_LL1C_DL_0,    /* ����ULL1C�˽�����Ϣ��4��DL1C����8�� */
    EICC_LL1C_UL_0_RECV_LL1C_DL_1,    /* ����ULL1C�˽�����Ϣ��4��DL1C����8�� */
    EICC_LL1C_UL_0_RECV_LL1C_DL_2,    /* ����ULL1C�˽�����Ϣ��4��DL1C����8�� */
    EICC_LL1C_UL_0_RECV_LL1C_DL_3,    /* ����ULL1C�˽�����Ϣ��4��DL1C����8�� */
    EICC_LL1C_UL_1_RECV_LL1C_DL_0,    /* ����ULL1C�˽�����Ϣ��4��DL1C����8�� */
    EICC_LL1C_UL_1_RECV_LL1C_DL_1,    /* ����ULL1C�˽�����Ϣ��4��DL1C����8�� */
    EICC_LL1C_UL_1_RECV_LL1C_DL_2,    /* ����ULL1C�˽�����Ϣ��4��DL1C����8�� */
    EICC_LL1C_UL_1_RECV_LL1C_DL_3,    /* ����ULL1C�˽�����Ϣ��4��DL1C����8�� */

    EICC_LL1C_UL_0_SEND_LL1C_UL_1,    /* ����ULL1C���໥������Ϣ����2�� */
    EICC_LL1C_UL_1_SEND_LL1C_UL_0,    /* ����ULL1C���໥������Ϣ����2�� */

    EICC_LL1C_UL_0_RECV_HL1C_SP,      /* ר��������Beam Pool��������ULL1C�ˣ���2��*/
    EICC_LL1C_UL_1_RECV_HL1C_SP,      /* ר��������Beam Pool��������ULL1C�ˣ���2��*/

    EICC_LL1C_UL_0_SEND_BBIC_SEQ_DMA, /* LL1C��дBBIC SEQ, EICC DMA��ʽ��ÿ������������2*2�� */
    EICC_LL1C_UL_1_SEND_BBIC_SEQ_DMA, /* LL1C��дBBIC SEQ, EICC DMA��ʽ��ÿ������������2*2�� */
    EICC_LL1C_UL_0_RECV_BBIC_SEQ_DMA, /* LL1C��дBBIC SEQ, EICC DMA��ʽ��ÿ������������2*2�� */
    EICC_LL1C_UL_1_RECV_BBIC_SEQ_DMA, /* LL1C��дBBIC SEQ, EICC DMA��ʽ��ÿ������������2*2�� */
    
    EICC_LL1C_UL_CHN_ID_MAX
};

/* NRCCPU EICC DEV DEV0~DEV1*/
enum EICC_NRCCPU_DEV0_CHN_ID
{
    EICC_NRCCPU_DEV0_CHN_ID_MIN = 0x4000,

    EICC_NRCCPU_SEND_AP_IFC_0 = 0x4000,  /* nrccpu��ap֮��Ĺ���IFC�ķ���ͨ�� */   
    EICC_NRCCPU_SEND_AP_IFC_1,           /* nrccpu��ap֮��Ĺ���IFC�ķ���ͨ�� */    
    EICC_NRCCPU_SEND_AP_IFC_2,           /* nrccpu��ap֮��Ĺ���IFC�ķ���ͨ�� */   
    EICC_NRCCPU_SEND_AP_IFC_3,           /* nrccpu��ap֮��Ĺ���IFC�ķ���ͨ�� */   
    EICC_NRCCPU_RECV_AP_IFC_0,           /* nrccpu��ap֮��Ĺ���IFC�Ľ���ͨ�� */
    EICC_NRCCPU_RECV_AP_IFC_1,           /* nrccpu��ap֮��Ĺ���IFC�Ľ���ͨ�� */
    EICC_NRCCPU_RECV_AP_IFC_2,           /* nrccpu��ap֮��Ĺ���IFC�Ľ���ͨ�� */
    EICC_NRCCPU_RECV_AP_IFC_3,           /* nrccpu��ap֮��Ĺ���IFC�Ľ���ͨ�� */
    EICC_NRCCPU_SEND_LRCCPU_0,           /* nrccpu��lrccpu֮���ר�з���ͨ�� */
    EICC_NRCCPU_RECV_LRCCPU_0,           /* nrccpu��lrccpu֮���ר�н���ͨ�� */
    EICC_NRCCPU_SEND_LRCCPU_IFC_0,       /* nrccpu��lrccpu֮��Ĺ�����ͨ�� */
    EICC_NRCCPU_RECV_LRCCPU_IFC_0,       /* nrccpu��lrccpu֮��Ĺ������ͨ�� */

    EICC_NRCCPU_DEV0_CHN_ID_MAX,
    EICC_NRCCPU_SEND_LRCCPU_DTRANS_0,
    EICC_NRCCPU_SEND_LRCCPU_DTRANS_1,
    EICC_NRCCPU_SEND_LRCCPU_DTRANS_2,
    EICC_NRCCPU_SEND_LRCCPU_DTRANS_3,
    EICC_NRCCPU_SEND_LRCCPU_DTRANS_4,
    EICC_NRCCPU_SEND_LRCCPU_DTRANS_5,
    EICC_NRCCPU_SEND_LRCCPU_DTRANS_6,
    EICC_NRCCPU_SEND_LRCCPU_DTRANS_7,
    EICC_NRCCPU_SEND_LRCCPU_NPDCP_BSR,
    EICC_NRCCPU_SEND_LRCCPU_NPDCP_COUNT,
    EICC_NRCCPU_RECV_LRCCPU_NPDCP_DATA_CNF,
    EICC_NRCCPU_RECV_LRCCPU_NPDCP_REL_PDU
};

enum EICC_NRCCPU_DEV1_CHN_ID
{
    EICC_NRCCPU_DEV1_CHN_ID_MIN     = 0x5000,

    EICC_NRCCPU_SEND_L2HAC_DTRANS_0 = 0x5000,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_1,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_2,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_3,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_4,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_5,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_6,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_7,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_8,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_9,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_10,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_11,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_12,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_13,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_14,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_15,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_16,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_17,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_18,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_19,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_20,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_21,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_22,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_23,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_24,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_25,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_26,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_27,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_28,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_29,
    EICC_NRCCPU_SEND_L2HAC_NRLC_BSR,
    EICC_NRCCPU_RECV_L2HAC_NRLC_REL_PDU,
    EICC_NRCCPU_SEND_L2HAC_NPDCP_COUNT,

    EICC_NRCCPU_SEND_L2HAC_IFC_0,           /* 5Gʵʱ����->��ʵʱ������ͨ�� */
    EICC_NRCCPU_RECV_L2HAC_IFC_0,           /* 5G��ʵʱ����->ʵʱ������ͨ�� */

    EICC_NRCCPU_SEND_HL1C_OSA_0,
    EICC_NRCCPU_RECV_HL1C_OSA_0,
    EICC_NRCCPU_SEND_L2HAC_OSA_0,
    EICC_NRCCPU_RECV_L2HAC_OSA_0,

    EICC_NRCCPU_SEND_AP_OSA_0,
    EICC_NRCCPU_RECV_AP_OSA_0,
    EICC_NRCCPU_SEND_LRCCPU_OSA_0,
    EICC_NRCCPU_RECV_LRCCPU_OSA_0,

    EICC_NRCCPU_DEV1_CHN_ID_MAX
};

/* LRCCPU EICC DEV */
enum EICC_LRCCPU_DEV_CHN_ID
{
    EICC_LRCCPU_CHN_ID_MIN = 0x6000,        /* EICC LRCCPU��Сͨ���� */

    EICC_LRCCPU_SEND_NRCCPU_IFC_0 = 0x6000, /* lrccpu��nrccpu֮��Ĺ�����ͨ�� */ 
    EICC_LRCCPU_RECV_NRCCPU_IFC_0,          /* lrccpu��nrccpu֮��Ĺ�����ͨ�� */ 
    EICC_LRCCPU_SEND_NRCCPU_0,                /* lrccpu��nrccpu֮���ר�з���ͨ�� */
    EICC_LRCCPU_RECV_NRCCPU_0,                /* lrccpu��nrccpu֮���ר�н���ͨ�� */
    EICC_LRNX_SEND_L2HAC_IFC_0,             /* LTE<->NR������Ϣ��������ͨ�� */
    EICC_LRNX_RECV_L2HAC_IFC_0,             /* LTE<->NR������Ϣ��������ͨ�� */
    EICC_LRCCPU_SEND_NRCCPU_OSA_0,
    EICC_LRCCPU_RECV_NRCCPU_OSA_0,

    EICC_LRCCPU_CHN_ID_MAX,
    EICC_LRCCPU_RECV_NRCCPU_DTRANS_0,
    EICC_LRCCPU_RECV_NRCCPU_DTRANS_1,
    EICC_LRCCPU_RECV_NRCCPU_DTRANS_2,
    EICC_LRCCPU_RECV_NRCCPU_DTRANS_3,
    EICC_LRCCPU_RECV_NRCCPU_DTRANS_4,
    EICC_LRCCPU_RECV_NRCCPU_DTRANS_5,
    EICC_LRCCPU_RECV_NRCCPU_DTRANS_6,
    EICC_LRCCPU_RECV_NRCCPU_DTRANS_7,
    EICC_LRCCPU_RECV_NRCCPU_NPDCP_COUNT,
    EICC_LRCCPU_RECV_NRCCPU_NPDCP_BSR,
    EICC_LRCCPU_SEND_NRCCPU_REL_PDU,
    EICC_LRCCPU_SEND_NRCCPU_DATA_CNF
};

/* HAC EICC DEV DEV */
enum EICC_L2HAC_DEV0_ID_ENUM
{
    EICC_L2HAC_DEV0_CHN_ID_MIN      = 0x7000,

    EICC_L2HAC_RECV_NRCCPU_DTRANS_0 = 0x7000,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_1,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_2,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_3,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_4,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_5,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_6,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_7,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_8,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_9,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_10,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_11,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_12,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_13,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_14,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_15,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_16,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_17,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_18,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_19,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_20,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_21,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_22,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_23,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_24,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_25,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_26,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_27,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_28,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_29,
    EICC_L2HAC_RECV_NRCCPU_NRLC_BSR,
    EICC_L2HAC_SEND_NRCCPU_NRLC_REL_PDU,
    EICC_L2HAC_RECV_NRCCPU_NPDCP_COUNT,
    
    EICC_L2HAC_SEND_LRNX_IFC_0,       /* LTE<->NR������Ϣ��������ͨ�� */
    EICC_L2HAC_RECV_LRNX_IFC_0,       /* LTE<->NR������Ϣ��������ͨ�� */
    EICC_L2HAC_SEND_NRCCPU_IFC_0,     /* 5Gʵʱ����->��ʵʱ���񹫹���Ϣ */
    EICC_L2HAC_RECV_NRCCPU_IFC_0,     /* 5G��ʵʱ����->ʵʱ���񹫹���Ϣ */

    EICC_L2HAC_SEND_NRCCPU_OSA_0,
    EICC_L2HAC_RECV_NRCCPU_OSA_0,

    EICC_L2HAC_DEV0_CHN_ID_MAX
};

enum EICC_L2HAC_DEV1_ID_ENUM
{
    EICC_L2HAC_DEV1_CHN_ID_MIN  = 0x8000,

    EICC_L2HAC_SEND_HL1C_URGENT = 0x8000,
    EICC_L2HAC_SEND_HL1C_NORMAL,
    EICC_L2HAC_RECV_HL1C_URGENT,
    EICC_L2HAC_RECV_HL1C_NORMAL,
    EICC_L2HAC_SEND_LL1C_UL_0,
    EICC_L2HAC_SEND_LL1C_UL_1,
    EICC_L2HAC_RECV_LL1C_UL_0_URGENT,
    EICC_L2HAC_RECV_LL1C_UL_0_NORMAL,
    EICC_L2HAC_RECV_LL1C_UL_1_URGENT,
    EICC_L2HAC_RECV_LL1C_UL_1_NORMAL,
    EICC_L2HAC_RECV_LL1C_DL_0,
    EICC_L2HAC_RECV_LL1C_DL_1,
    EICC_L2HAC_RECV_LL1C_DL_2,
    EICC_L2HAC_RECV_LL1C_DL_3,

    EICC_L2HAC_DEV1_CHN_ID_MAX
};

/* AP EICC DEV DEV */
enum EICC_AP_DEV_CHN_ID
{
    EICC_AP_CHN_ID_MIN = 0x9000,
    
    EICC_AP_SEND_NRCCPU_IFC_0 = 0x9000, /* nrccpu��ap֮��Ĺ���IFC�ķ���ͨ�� */   
    EICC_AP_SEND_NRCCPU_IFC_1,          /* nrccpu��ap֮��Ĺ���IFC�ķ���ͨ�� */    
    EICC_AP_SEND_NRCCPU_IFC_2,          /* nrccpu��ap֮��Ĺ���IFC�ķ���ͨ�� */   
    EICC_AP_SEND_NRCCPU_IFC_3,          /* nrccpu��ap֮��Ĺ���IFC�ķ���ͨ�� */   

    EICC_AP_RECV_NRCCPU_IFC_0,          /* nrccpu��ap֮��Ĺ���IFC�Ľ���ͨ�� */
    EICC_AP_RECV_NRCCPU_IFC_1,          /* nrccpu��ap֮��Ĺ���IFC�Ľ���ͨ�� */
    EICC_AP_RECV_NRCCPU_IFC_2,          /* nrccpu��ap֮��Ĺ���IFC�Ľ���ͨ�� */
    EICC_AP_RECV_NRCCPU_IFC_3,          /* nrccpu��ap֮��Ĺ���IFC�Ľ���ͨ�� */

    EICC_AP_SEND_NRCCPU_OSA_0,
    EICC_AP_RECV_NRCCPU_OSA_0,

    EICC_AP_CHN_ID_MAX
};

/* HAC EICC DEV */
enum EICC_HL1C_1_DEV_CHN_ID
{
    EICC_HL1C_1_CHN_ID_MIN = 0xa000,
    
    EICC_HL1C_SEND_NRCCPU_OSA_0 = 0xa000,
    EICC_HL1C_RECV_NRCCPU_OSA_0, 

    EICC_HL1C_1_CHN_ID_MAX
};

enum EICC_FUNC_SUB_CHN_ID
{
    EICC_IFC_FUNC_MIN = 0,
    EICC_IFC_FUNC_PANRPC = 0,
    EICC_IFC_FUNC_MAX ,
};
#else
/* ͨ��id���� */
/* nx EICC DEV ö�� DEV0~DEV3*/
enum EICC_DEV0_CHN_ID
{
    EICC_SDR_CHN_MIN = 0,            /* EICC_SDR��Сͨ���� */

    EICC_SDR_SEND_LL1C_DL_0 = 0,     /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_SDR_SEND_LL1C_DL_1,         /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_SDR_SEND_LL1C_DL_2,         /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_SDR_SEND_LL1C_DL_3,         /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_SDR_SEND_HL1C,              /* HL1C��SDR��ԭ�ｻ�� */

    EICC_SDR_RECV_LL1C_DL_0,         /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_SDR_RECV_LL1C_DL_1,         /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_SDR_RECV_LL1C_DL_2,         /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_SDR_RECV_LL1C_DL_3,         /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_SDR_RECV_HL1C,              /* HL1C��SDR��ԭ�ｻ�� */

    EICC_SDR_RECV_HL1C_OM,           /* HL1C��SDR��OM��Ϣ*/

    EICC_SDR_SEND_LL1C_DL_4,         /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_SDR_SEND_LL1C_DL_5,         /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_SDR_SEND_LL1C_DL_6,         /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_SDR_SEND_LL1C_DL_7,         /* LL1C DL��SDR��ԭ�ｻ�� */
    
    EICC_LL1C_DL_RECV_LL1C_DL_0,         /* LL1C DL��LL1C DL��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_LL1C_DL_1,         /* LL1C DL��LL1C DL��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_LL1C_DL_2,         /* LL1C DL��LL1C DL��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_LL1C_DL_3,         /* LL1C DL��LL1C DL��ԭ�ｻ�� */

    EICC_SDR_SEND_HL1C_1,            /* HL1C��SDR��ԭ�ｻ�� */
    EICC_SDR_RECV_HL1C_1,            /* HL1C��SDR��ԭ�ｻ�� */

    EICC_HL1C_SEND_BBIC_SEQ_DMA,     /* HL1C��дBBIC SEQ, EICC DMA��ʽ��ÿ������������1*2�� */
    EICC_HL1C_RECV_BBIC_SEQ_DMA,     /* HL1C��дBBIC SEQ, EICC DMA��ʽ��ÿ������������1*2�� */
    EICC_SDR_CHN_ID_MAX
};

enum EICC_DEV1_CHN_ID
{
    EICC_HL1C_CHN_MIN = 0x1000,  /* EICC_HL1C_DL��Сͨ���� */

    EICC_HL1C_SEND_LL1C_DL_0 = 0x1000,/* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_HL1C_SEND_LL1C_DL_1,        /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_HL1C_SEND_LL1C_DL_2,        /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_HL1C_SEND_LL1C_DL_3,        /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_HL1C_SEND_SDR,              /* HL1C��SDR��ԭ�ｻ�� */

    EICC_HL1C_RECV_LL1C_DL_0,        /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_HL1C_RECV_LL1C_DL_1,        /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_HL1C_RECV_LL1C_DL_2,        /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_HL1C_RECV_LL1C_DL_3,        /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_HL1C_RECV_SDR,              /* HL1C��SDR��ԭ�ｻ�� */

    EICC_HL1C_SEND_NRCCPU_H,         /* HL1C��NRCCPU�ĸ����ȼ���Ϣ*/
    EICC_HL1C_SEND_NRCCPU_L,         /* HL1C��NRCCPU�ĵ����ȼ���Ϣ*/
    EICC_HL1C_SEND_NRCCPU_OM,        /* HL1C��NRCCPU��OM��Ϣ*/
    EICC_HL1C_RECV_NRCCPU_H,         /* HL1C��NRCCPU�ĸ����ȼ���Ϣ*/
    EICC_HL1C_RECV_NRCCPU_L,         /* HL1C��NRCCPU�ĵ����ȼ���Ϣ*/
    EICC_HL1C_RECV_NRCCPU_OM,        /* HL1C��NRCCPU��OM��Ϣ*/

    EICC_HL1C_SEND_SDR_OM,           /* HL1C��SDR��OM��Ϣ*/
    EICC_HL1C_SEND_LL1C_DL_OM_0,     /* HL1C��LL1C DL��OM��Ϣ*/
    EICC_HL1C_SEND_LL1C_DL_OM_1,     /* HL1C��LL1C DL��OM��Ϣ*/
    EICC_HL1C_SEND_LL1C_DL_OM_2,     /* HL1C��LL1C DL��OM��Ϣ*/
    EICC_HL1C_SEND_LL1C_DL_OM_3,     /* HL1C��LL1C DL��OM��Ϣ*/
    EICC_HL1C_SEND_LL1C_UL_OM_0,     /* HL1C��LL1C UL��OM��Ϣ*/
    EICC_HL1C_SEND_LL1C_UL_OM_1,     /* HL1C��LL1C UL��OM��Ϣ*/

    EICC_HL1C_SEND_LL1C_UL_0,        /* HL1C����Ϣ������ULL1C�ˣ���2*2=4��*/
    EICC_HL1C_SEND_LL1C_UL_1,        /* HL1C����Ϣ������ULL1C�ˣ���2*2=4��*/
    EICC_HL1C_SEND_LL1C_UL_2,        /* HL1C����Ϣ������ULL1C�ˣ���2*2=4��*/
    EICC_HL1C_SEND_LL1C_UL_3,        /* HL1C����Ϣ������ULL1C�ˣ���2*2=4��*/
    
    EICC_HL1C_RECV_LL1C_UL_0,        /* HL1C������Ϣ������ULL1C�ˣ���2*2=4��(��/��ʱ��) */
    EICC_HL1C_RECV_LL1C_UL_1,        /* HL1C������Ϣ������ULL1C�ˣ���2*2=4��(��/��ʱ��) */
    EICC_HL1C_RECV_LL1C_UL_2,        /* HL1C������Ϣ������ULL1C�ˣ���2*2=4��(��/��ʱ��) */
    EICC_HL1C_RECV_LL1C_UL_3,        /* HL1C������Ϣ������ULL1C�ˣ���2*2=4��(��/��ʱ��) */

    EICC_HL1C_SEND_LL1C_UL_SP_0,     /* ר��������Beam Pool��������ULL1C�ˣ���2�� */
    EICC_HL1C_SEND_LL1C_UL_SP_1,     /* ר��������Beam Pool��������ULL1C�ˣ���2�� */

    EICC_HL1C_SEND_LL1C_DL_4,        /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_HL1C_SEND_LL1C_DL_5,        /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_HL1C_SEND_LL1C_DL_6,        /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_HL1C_SEND_LL1C_DL_7,        /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_HL1C_RECV_LL1C_DL_4,        /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_HL1C_RECV_LL1C_DL_5,        /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_HL1C_RECV_LL1C_DL_6,        /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_HL1C_RECV_LL1C_DL_7,        /* LL1C DL��HL1C��ԭ�ｻ�� */

    EICC_HL1C_SEND_SDR_1,            /* HL1C��SDR��ԭ�ｻ�� */
    EICC_HL1C_RECV_SDR_1,            /* HL1C��SDR��ԭ�ｻ�� */
    
    EICC_HL1C_RECV_L2HAC_URGENT,
    EICC_HL1C_RECV_L2HAC_NORMAL,
    EICC_HL1C_SEND_L2HAC_URGENT,
    EICC_HL1C_SEND_L2HAC_NORMAL,
    
    EICC_HL1C_CHN_ID_MAX
};

enum EICC_DEV2_CHN_ID
{
    EICC_LL1C_DL_CHN_MIN = 0x2000,   /* EICC_LL1C��Сͨ���� */

    EICC_LL1C_DL_SEND_SDR_0 = 0x2000,/* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_LL1C_DL_SEND_SDR_1,        /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_LL1C_DL_SEND_SDR_2,        /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_LL1C_DL_SEND_SDR_3,        /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_LL1C_DL_SEND_HL1C_0,       /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_LL1C_DL_SEND_HL1C_1,       /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_LL1C_DL_SEND_HL1C_2,       /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_LL1C_DL_SEND_HL1C_3,       /* LL1C DL��HL1C��ԭ�ｻ�� */

    EICC_LL1C_DL_RECV_SDR_0,        /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_SDR_1,        /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_SDR_2,        /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_SDR_3,        /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_HL1C_0,       /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_HL1C_1,       /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_HL1C_2,       /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_HL1C_3,       /* LL1C DL��HL1C��ԭ�ｻ�� */

    EICC_LL1C_DL_RECV_HL1C_OM_0,    /* HL1C��LL1C DL��OM��Ϣ*/
    EICC_LL1C_DL_RECV_HL1C_OM_1,    /* HL1C��LL1C DL��OM��Ϣ*/
    EICC_LL1C_DL_RECV_HL1C_OM_2,    /* HL1C��LL1C DL��OM��Ϣ*/
    EICC_LL1C_DL_RECV_HL1C_OM_3,    /* HL1C��LL1C DL��OM��Ϣ*/

    EICC_LL1C_DL_0_SEND_LL1C_UL_0,  /* 4��DL1C����Ϣ������ULL1C�ˣ���8�� */
    EICC_LL1C_DL_1_SEND_LL1C_UL_0,  /* 4��DL1C����Ϣ������ULL1C�ˣ���8�� */
    EICC_LL1C_DL_2_SEND_LL1C_UL_0,  /* 4��DL1C����Ϣ������ULL1C�ˣ���8�� */
    EICC_LL1C_DL_3_SEND_LL1C_UL_0,  /* 4��DL1C����Ϣ������ULL1C�ˣ���8�� */
    EICC_LL1C_DL_0_SEND_LL1C_UL_1,  /* 4��DL1C����Ϣ������ULL1C�ˣ���8�� */
    EICC_LL1C_DL_1_SEND_LL1C_UL_1,  /* 4��DL1C����Ϣ������ULL1C�ˣ���8�� */
    EICC_LL1C_DL_2_SEND_LL1C_UL_1,  /* 4��DL1C����Ϣ������ULL1C�ˣ���8�� */
    EICC_LL1C_DL_3_SEND_LL1C_UL_1,  /* 4��DL1C����Ϣ������ULL1C�ˣ���8�� */
    EICC_LL1C_DL_0_RECV_LL1C_UL_0,  /* 4��DL1C������Ϣ������ULL1C�ˣ���8�� */
    EICC_LL1C_DL_1_RECV_LL1C_UL_0,  /* 4��DL1C������Ϣ������ULL1C�ˣ���8�� */
    EICC_LL1C_DL_2_RECV_LL1C_UL_0,  /* 4��DL1C������Ϣ������ULL1C�ˣ���8�� */
    EICC_LL1C_DL_3_RECV_LL1C_UL_0,  /* 4��DL1C������Ϣ������ULL1C�ˣ���8�� */
    EICC_LL1C_DL_0_RECV_LL1C_UL_1,  /* 4��DL1C������Ϣ������ULL1C�ˣ���8�� */
    EICC_LL1C_DL_1_RECV_LL1C_UL_1,  /* 4��DL1C������Ϣ������ULL1C�ˣ���8�� */
    EICC_LL1C_DL_2_RECV_LL1C_UL_1,  /* 4��DL1C������Ϣ������ULL1C�ˣ���8�� */
    EICC_LL1C_DL_3_RECV_LL1C_UL_1,  /* 4��DL1C������Ϣ������ULL1C�ˣ���8�� */

    EICC_LL1C_DL_SEND_LL1C_DL_0,        /* LL1C DL��LL1C_DL��ԭ�ｻ�� */
    EICC_LL1C_DL_SEND_LL1C_DL_1,        /* LL1C DL��LL1C_DL��ԭ�ｻ�� */
    EICC_LL1C_DL_SEND_LL1C_DL_2,        /* LL1C DL��LL1C_DL��ԭ�ｻ�� */
    EICC_LL1C_DL_SEND_LL1C_DL_3,        /* LL1C DL��LL1C_DL��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_SDR_4,        /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_SDR_5,        /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_SDR_6,        /* LL1C DL��SDR��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_SDR_7,        /* LL1C DL��SDR��ԭ�ｻ�� */

    EICC_LL1C_DL_SEND_HL1C_4,       /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_LL1C_DL_SEND_HL1C_5,       /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_LL1C_DL_SEND_HL1C_6,       /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_LL1C_DL_SEND_HL1C_7,       /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_HL1C_4,       /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_HL1C_5,       /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_HL1C_6,       /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_LL1C_DL_RECV_HL1C_7,       /* LL1C DL��HL1C��ԭ�ｻ�� */
    EICC_LL1C_UL_1_RECV_LL1C_UL_0,  /* ����ULL1C���໥������Ϣ����2�� */
    EICC_LL1C_UL_0_RECV_LL1C_UL_1,  /* ����ULL1C���໥������Ϣ����2�� */

    EICC_LL1C_DL_0_SEND_L2HAC,
    EICC_LL1C_DL_1_SEND_L2HAC,
    EICC_LL1C_DL_2_SEND_L2HAC,
    EICC_LL1C_DL_3_SEND_L2HAC,

    EICC_LL1C_DL_CHN_ID_MAX
};

enum EICC_DEV3_CHN_ID
{
    EICC_LL1C_UL_CHN_MIN = 0x3000,        /* EICC_LL1C_UL��Сͨ���� */

    EICC_LL1C_UL_RECV_HL1C_OM_0 = 0x3000,      /* LL1C UL��HL1C��OM��Ϣ*/
    EICC_LL1C_UL_RECV_HL1C_OM_1,      /* LL1C UL��HL1C��OM��Ϣ*/

    EICC_LL1C_UL_SEND_HL1C_0,         /* ����ULL1C�˷���Ϣ��HL1C����2*2=4��*/
    EICC_LL1C_UL_SEND_HL1C_1,         /* ����ULL1C�˷���Ϣ��HL1C����2*2=4��*/
    EICC_LL1C_UL_SEND_HL1C_2,         /* ����ULL1C�˷���Ϣ��HL1C����2*2=4��*/
    EICC_LL1C_UL_SEND_HL1C_3,         /* ����ULL1C�˷���Ϣ��HL1C����2*2=4��*/

    EICC_LL1C_UL_RECV_HL1C_0,         /* ����ULL1C�˽�����Ϣ��HL1C����2*2=4��*/
    EICC_LL1C_UL_RECV_HL1C_1,         /* ����ULL1C�˽�����Ϣ��HL1C����2*2=4��*/
    EICC_LL1C_UL_RECV_HL1C_2,         /* ����ULL1C�˽�����Ϣ��HL1C����2*2=4��*/
    EICC_LL1C_UL_RECV_HL1C_3,         /* ����ULL1C�˽�����Ϣ��HL1C����2*2=4��*/

    EICC_LL1C_UL_0_SEND_LL1C_DL_0,    /* ����ULL1C�˷���Ϣ��4��DL1C����8�� */
    EICC_LL1C_UL_0_SEND_LL1C_DL_1,    /* ����ULL1C�˷���Ϣ��4��DL1C����8�� */
    EICC_LL1C_UL_0_SEND_LL1C_DL_2,    /* ����ULL1C�˷���Ϣ��4��DL1C����8�� */
    EICC_LL1C_UL_0_SEND_LL1C_DL_3,    /* ����ULL1C�˷���Ϣ��4��DL1C����8�� */
    EICC_LL1C_UL_1_SEND_LL1C_DL_0,    /* ����ULL1C�˷���Ϣ��4��DL1C����8�� */
    EICC_LL1C_UL_1_SEND_LL1C_DL_1,    /* ����ULL1C�˷���Ϣ��4��DL1C����8�� */
    EICC_LL1C_UL_1_SEND_LL1C_DL_2,    /* ����ULL1C�˷���Ϣ��4��DL1C����8�� */
    EICC_LL1C_UL_1_SEND_LL1C_DL_3,    /* ����ULL1C�˷���Ϣ��4��DL1C����8�� */
    EICC_LL1C_UL_0_RECV_LL1C_DL_0,    /* ����ULL1C�˽�����Ϣ��4��DL1C����8�� */
    EICC_LL1C_UL_0_RECV_LL1C_DL_1,    /* ����ULL1C�˽�����Ϣ��4��DL1C����8�� */
    EICC_LL1C_UL_0_RECV_LL1C_DL_2,    /* ����ULL1C�˽�����Ϣ��4��DL1C����8�� */
    EICC_LL1C_UL_0_RECV_LL1C_DL_3,    /* ����ULL1C�˽�����Ϣ��4��DL1C����8�� */
    EICC_LL1C_UL_1_RECV_LL1C_DL_0,    /* ����ULL1C�˽�����Ϣ��4��DL1C����8�� */
    EICC_LL1C_UL_1_RECV_LL1C_DL_1,    /* ����ULL1C�˽�����Ϣ��4��DL1C����8�� */
    EICC_LL1C_UL_1_RECV_LL1C_DL_2,    /* ����ULL1C�˽�����Ϣ��4��DL1C����8�� */
    EICC_LL1C_UL_1_RECV_LL1C_DL_3,    /* ����ULL1C�˽�����Ϣ��4��DL1C����8�� */

    EICC_LL1C_UL_0_SEND_LL1C_UL_1,    /* ����ULL1C���໥������Ϣ����2�� */
    EICC_LL1C_UL_1_SEND_LL1C_UL_0,    /* ����ULL1C���໥������Ϣ����2�� */

    EICC_LL1C_UL_0_RECV_HL1C_SP,      /* ר��������Beam Pool��������ULL1C�ˣ���2��*/
    EICC_LL1C_UL_1_RECV_HL1C_SP,      /* ר��������Beam Pool��������ULL1C�ˣ���2��*/

    EICC_LL1C_UL_0_SEND_BBIC_SEQ_DMA, /* LL1C��дBBIC SEQ, EICC DMA��ʽ��ÿ������������2*2�� */
    EICC_LL1C_UL_1_SEND_BBIC_SEQ_DMA, /* LL1C��дBBIC SEQ, EICC DMA��ʽ��ÿ������������2*2�� */
    EICC_LL1C_UL_0_RECV_BBIC_SEQ_DMA, /* LL1C��дBBIC SEQ, EICC DMA��ʽ��ÿ������������2*2�� */
    EICC_LL1C_UL_1_RECV_BBIC_SEQ_DMA, /* LL1C��дBBIC SEQ, EICC DMA��ʽ��ÿ������������2*2�� */
    
    EICC_LL1C_UL_0_RECV_L2HAC,
    EICC_LL1C_UL_1_RECV_L2HAC,
    EICC_LL1C_UL_0_SEND_L2HAC_URGENT,
    EICC_LL1C_UL_0_SEND_L2HAC_NORMAL,
    EICC_LL1C_UL_1_SEND_L2HAC_URGENT,
    EICC_LL1C_UL_1_SEND_L2HAC_NORMAL,
    
    EICC_LL1C_UL_CHN_ID_MAX
};

/* NRCCPU EICC DEV DEV0~DEV1*/
enum EICC_NRCCPU_DEV0_CHN_ID
{
    EICC_NRCCPU_DEV0_CHN_ID_MIN = 0x4000,

    EICC_NRCCPU_SEND_AP_IFC_0 = 0x4000,  /* nrccpu��ap֮��Ĺ���IFC�ķ���ͨ�� */   
    EICC_NRCCPU_SEND_AP_IFC_1,           /* nrccpu��ap֮��Ĺ���IFC�ķ���ͨ�� */    
    EICC_NRCCPU_SEND_AP_IFC_2,           /* nrccpu��ap֮��Ĺ���IFC�ķ���ͨ�� */   
    EICC_NRCCPU_SEND_AP_IFC_3,           /* nrccpu��ap֮��Ĺ���IFC�ķ���ͨ�� */   
    EICC_NRCCPU_RECV_AP_IFC_0,           /* nrccpu��ap֮��Ĺ���IFC�Ľ���ͨ�� */
    EICC_NRCCPU_RECV_AP_IFC_1,           /* nrccpu��ap֮��Ĺ���IFC�Ľ���ͨ�� */
    EICC_NRCCPU_RECV_AP_IFC_2,           /* nrccpu��ap֮��Ĺ���IFC�Ľ���ͨ�� */
    EICC_NRCCPU_RECV_AP_IFC_3,           /* nrccpu��ap֮��Ĺ���IFC�Ľ���ͨ�� */
    EICC_NRCCPU_SEND_LRCCPU_0,           /* nrccpu��lrccpu֮���ר�з���ͨ�� */
    EICC_NRCCPU_RECV_LRCCPU_0,           /* nrccpu��lrccpu֮���ר�н���ͨ�� */
    EICC_NRCCPU_SEND_LRCCPU_IFC_0,       /* nrccpu��lrccpu֮��Ĺ�����ͨ�� */
    EICC_NRCCPU_RECV_LRCCPU_IFC_0,       /* nrccpu��lrccpu֮��Ĺ������ͨ�� */

    EICC_NRCCPU_DEV0_CHN_ID_MAX,
    EICC_NRCCPU_SEND_LRCCPU_DTRANS_0,
    EICC_NRCCPU_SEND_LRCCPU_DTRANS_1,
    EICC_NRCCPU_SEND_LRCCPU_DTRANS_2,
    EICC_NRCCPU_SEND_LRCCPU_DTRANS_3,
    EICC_NRCCPU_SEND_LRCCPU_DTRANS_4,
    EICC_NRCCPU_SEND_LRCCPU_DTRANS_5,
    EICC_NRCCPU_SEND_LRCCPU_DTRANS_6,
    EICC_NRCCPU_SEND_LRCCPU_DTRANS_7,
    EICC_NRCCPU_SEND_LRCCPU_NPDCP_BSR,
    EICC_NRCCPU_SEND_LRCCPU_NPDCP_COUNT,
    EICC_NRCCPU_RECV_LRCCPU_NPDCP_DATA_CNF,
    EICC_NRCCPU_RECV_LRCCPU_NPDCP_REL_PDU
};

enum EICC_NRCCPU_DEV1_CHN_ID
{
    EICC_NRCCPU_DEV1_CHN_ID_MIN     = 0x5000,

    EICC_NRCCPU_SEND_L2HAC_DTRANS_0 = 0x5000,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_1,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_2,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_3,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_4,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_5,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_6,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_7,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_8,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_9,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_10,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_11,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_12,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_13,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_14,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_15,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_16,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_17,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_18,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_19,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_20,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_21,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_22,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_23,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_24,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_25,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_26,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_27,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_28,
    EICC_NRCCPU_SEND_L2HAC_DTRANS_29,
    EICC_NRCCPU_SEND_L2HAC_NRLC_BSR,
    EICC_NRCCPU_RECV_L2HAC_NRLC_REL_PDU,
    EICC_NRCCPU_SEND_L2HAC_NPDCP_COUNT,

    EICC_NRCCPU_SEND_L2HAC_IFC_0,           /* 5Gʵʱ����->��ʵʱ������ͨ�� */
    EICC_NRCCPU_RECV_L2HAC_IFC_0,           /* 5G��ʵʱ����->ʵʱ������ͨ�� */

    EICC_NRCCPU_SEND_HL1C_H,
    EICC_NRCCPU_SEND_HL1C_L,
    EICC_NRCCPU_SEND_HL1C_OM,
    EICC_NRCCPU_RECV_HL1C_H,
    EICC_NRCCPU_RECV_HL1C_L,
    EICC_NRCCPU_RECV_HL1C_OM,
 
    EICC_NRCCPU_SEND_L2HAC_OSA_0,
    EICC_NRCCPU_RECV_L2HAC_OSA_0,

    EICC_NRCCPU_SEND_AP_OSA_0,
    EICC_NRCCPU_RECV_AP_OSA_0,
    EICC_NRCCPU_SEND_LRCCPU_OSA_0,
    EICC_NRCCPU_RECV_LRCCPU_OSA_0,

    EICC_NRCCPU_DEV1_CHN_ID_MAX
};

/* LRCCPU EICC DEV */
enum EICC_LRCCPU_DEV_CHN_ID
{
    EICC_LRCCPU_CHN_ID_MIN = 0x6000,        /* EICC LRCCPU��Сͨ���� */

    EICC_LRCCPU_SEND_NRCCPU_IFC_0 = 0x6000, /* lrccpu��nrccpu֮��Ĺ�����ͨ�� */ 
    EICC_LRCCPU_RECV_NRCCPU_IFC_0,          /* lrccpu��nrccpu֮��Ĺ�����ͨ�� */ 
    EICC_LRCCPU_SEND_NRCCPU_0,                /* lrccpu��nrccpu֮���ר�з���ͨ�� */
    EICC_LRCCPU_RECV_NRCCPU_0,                /* lrccpu��nrccpu֮���ר�н���ͨ�� */
    EICC_LRNX_SEND_L2HAC_IFC_0,             /* LTE<->NR������Ϣ��������ͨ�� */
    EICC_LRNX_RECV_L2HAC_IFC_0,             /* LTE<->NR������Ϣ��������ͨ�� */
    EICC_LRCCPU_SEND_NRCCPU_OSA_0,
    EICC_LRCCPU_RECV_NRCCPU_OSA_0,

    EICC_LRCCPU_CHN_ID_MAX,
    EICC_LRCCPU_RECV_NRCCPU_DTRANS_0,
    EICC_LRCCPU_RECV_NRCCPU_DTRANS_1,
    EICC_LRCCPU_RECV_NRCCPU_DTRANS_2,
    EICC_LRCCPU_RECV_NRCCPU_DTRANS_3,
    EICC_LRCCPU_RECV_NRCCPU_DTRANS_4,
    EICC_LRCCPU_RECV_NRCCPU_DTRANS_5,
    EICC_LRCCPU_RECV_NRCCPU_DTRANS_6,
    EICC_LRCCPU_RECV_NRCCPU_DTRANS_7,
    EICC_LRCCPU_RECV_NRCCPU_NPDCP_COUNT,
    EICC_LRCCPU_RECV_NRCCPU_NPDCP_BSR,
    EICC_LRCCPU_SEND_NRCCPU_REL_PDU,
    EICC_LRCCPU_SEND_NRCCPU_DATA_CNF
};

/* HAC EICC DEV DEV */
enum EICC_L2HAC_DEV0_ID_ENUM
{
    EICC_L2HAC_DEV0_CHN_ID_MIN      = 0x7000,

    EICC_L2HAC_RECV_NRCCPU_DTRANS_0 = 0x7000,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_1,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_2,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_3,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_4,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_5,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_6,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_7,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_8,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_9,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_10,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_11,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_12,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_13,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_14,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_15,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_16,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_17,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_18,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_19,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_20,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_21,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_22,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_23,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_24,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_25,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_26,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_27,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_28,
    EICC_L2HAC_RECV_NRCCPU_DTRANS_29,
    EICC_L2HAC_RECV_NRCCPU_NRLC_BSR,
    EICC_L2HAC_SEND_NRCCPU_NRLC_REL_PDU,
    EICC_L2HAC_RECV_NRCCPU_NPDCP_COUNT,
    
    EICC_L2HAC_SEND_LRNX_IFC_0,       /* LTE<->NR������Ϣ��������ͨ�� */
    EICC_L2HAC_RECV_LRNX_IFC_0,       /* LTE<->NR������Ϣ��������ͨ�� */
    EICC_L2HAC_SEND_NRCCPU_IFC_0,     /* 5Gʵʱ����->��ʵʱ���񹫹���Ϣ */
    EICC_L2HAC_RECV_NRCCPU_IFC_0,     /* 5G��ʵʱ����->ʵʱ���񹫹���Ϣ */

    EICC_L2HAC_SEND_NRCCPU_OSA_0,
    EICC_L2HAC_RECV_NRCCPU_OSA_0,

    EICC_L2HAC_DEV0_CHN_ID_MAX
};

enum EICC_L2HAC_DEV1_ID_ENUM
{
    EICC_L2HAC_DEV1_CHN_ID_MIN  = 0x8000,

    EICC_L2HAC_SEND_HL1C_URGENT = 0x8000,
    EICC_L2HAC_SEND_HL1C_NORMAL,
    EICC_L2HAC_RECV_HL1C_URGENT,
    EICC_L2HAC_RECV_HL1C_NORMAL,
    EICC_L2HAC_SEND_LL1C_UL_0,
    EICC_L2HAC_SEND_LL1C_UL_1,
    EICC_L2HAC_RECV_LL1C_UL_0_URGENT,
    EICC_L2HAC_RECV_LL1C_UL_0_NORMAL,
    EICC_L2HAC_RECV_LL1C_UL_1_URGENT,
    EICC_L2HAC_RECV_LL1C_UL_1_NORMAL,
    EICC_L2HAC_RECV_LL1C_DL_0,
    EICC_L2HAC_RECV_LL1C_DL_1,
    EICC_L2HAC_RECV_LL1C_DL_2,
    EICC_L2HAC_RECV_LL1C_DL_3,

    EICC_L2HAC_DEV1_CHN_ID_MAX
};

/* AP EICC DEV DEV */
enum EICC_AP_DEV_CHN_ID
{
    EICC_AP_CHN_ID_MIN = 0x9000,
    
    EICC_AP_SEND_NRCCPU_IFC_0 = 0x9000, /* nrccpu��ap֮��Ĺ���IFC�ķ���ͨ�� */   
    EICC_AP_SEND_NRCCPU_IFC_1,          /* nrccpu��ap֮��Ĺ���IFC�ķ���ͨ�� */    
    EICC_AP_SEND_NRCCPU_IFC_2,          /* nrccpu��ap֮��Ĺ���IFC�ķ���ͨ�� */   
    EICC_AP_SEND_NRCCPU_IFC_3,          /* nrccpu��ap֮��Ĺ���IFC�ķ���ͨ�� */   

    EICC_AP_RECV_NRCCPU_IFC_0,          /* nrccpu��ap֮��Ĺ���IFC�Ľ���ͨ�� */
    EICC_AP_RECV_NRCCPU_IFC_1,          /* nrccpu��ap֮��Ĺ���IFC�Ľ���ͨ�� */
    EICC_AP_RECV_NRCCPU_IFC_2,          /* nrccpu��ap֮��Ĺ���IFC�Ľ���ͨ�� */
    EICC_AP_RECV_NRCCPU_IFC_3,          /* nrccpu��ap֮��Ĺ���IFC�Ľ���ͨ�� */

    EICC_AP_SEND_NRCCPU_OSA_0,
    EICC_AP_RECV_NRCCPU_OSA_0,

    EICC_AP_CHN_ID_MAX
};

/* HAC EICC DEV */
enum EICC_HL1C_1_DEV_CHN_ID
{
    EICC_HL1C_1_CHN_ID_MIN = 0xa000,
    
    EICC_HL1C_1_CHN_ID_MAX
};

enum EICC_FUNC_SUB_CHN_ID
{
    EICC_IFC_FUNC_MIN = 0,
    EICC_IFC_FUNC_PANRPC = 0,
    EICC_IFC_FUNC_MAX ,
};
#endif
/* ˵��: ���ջص�����ID,��ͨ�����������, "ͨ������_xxx=0��ͨ������_RECV_FUNC_ID_MAX֮��
 */
enum EICC_LL1C_UL_SEND_LL1C_UL_FUNC_ID
{
    LL1C_UL_SEND_LL1C_UL_FUNC_TEST1 = 0,


    /* ��Ҫ��EICC_CHN_IFC����ͨ���϶�����ͨ��,���ڸ�ע����֮ǰ���� */
    LL1C_UL_SEND_LL1C_UL_FUNC_ID_MAX,
};

enum FLOWLINE_UP
{
    FLUP_ONE_ARV = 0x0,        /*һ�����ݿ����һ����Ϣ*/
    FLUP_HALF = 0x1,           /*1/2���*/
    FLUP_THREE_FOURTHS = 0x2,  /*3/4���*/
    FLUP_SEVEN_EIGHTHS = 0x3,  /*7/8���*/  
};

enum FLOWLINE_DN
{
    FLDN_EMPTY = 0x0,        /*������ˮ��*/
    FLDN_ONE_EIGHTHS = 0x1,  /*1/8���*/
    FLDN_ONE_FOURTHS = 0x2,  /*1/4���*/
    FLDN_HALF = 0x3,         /*1/2���*/  
};

enum ALIGN_MODE
{
    ALIGN_4BYTES = 0x0,   /*4�ֽڶ���*/
    ALIGN_16BYTES = 0x1,  /*16�ֽڶ���*/
    ALIGN_32BYTES = 0x2,  /*32�ֽڶ���*/
};

enum MSG_DISTRIBUTE
{
    IS_DISTRIBUTE = 0x0,   /*������ע�����ͨ���ص��ַ���Ϣ*/
    NOT_DISTRIBUTE = 0x1,  /*ͨ����������Ϣ������ͨ��0�ص���*/
};

enum EICC_PIPE_STATUS
{
    EICC_PIPE_BUSY = 0x0,
    EICC_PIPE_IDLE = 0x1,
};

enum EICC_DMA_TYPE
{
    SEPARATE_DST_ADDR = 0x0,   /*���Դ��ַ���Ŀ�ĵ�ַ��ʽDMA����*/
    SAME_DST_ADDR = 0x1,          /*���Դ��ַͬһĿ�ĵ�ַ��ʽDMA����*/
};

struct eicc_chan_attr
{
    enum CHANL_TYPE     type;           /*ͨ������*/
    enum MSG_DISTRIBUTE distribute;     /*�Ƿ���ͨ���ַ���Ϣ*/
    enum ALIGN_MODE     align_mode;     /*��Ϣͷ����Ϣ����뷽ʽ��Ĭ��оƬԼ��4�ֽڶ��룬��Ĭ������ʱram_addr��ram_size���뷽ʽ���뱣��һ��*/
    eicc_msg_cb         eicc_cb;        /*������ɻ���Ϣ���ջص�*/
    void *              ram_addr;       /*ע�᱾��pipe�ռ��ַ��Ĭ������8�ֽڶ��룬align_mode��Ĭ������ʱ�����䱣��һ��*/
    unsigned int        ram_size;       /*ע�᱾��pipe�ռ��С��Ĭ������8�ֽڶ��룬align_mode��Ĭ������ʱ�����䱣��һ��*/
    unsigned int        packet_len_max; /*���հ�����󳤶�(����ͨ������0)��Ĭ������8�ֽڶ��룬align_mode��Ĭ������ʱ�����䱣��һ��*/
};

struct eicc_currmsg_info
{
    unsigned int channel_id;            /* ��ǰ���ݰ���¼��ͨ��id������ͨ������ͨ����ɣ�*/
    unsigned int msg_len;               /* ��ǰ���ݰ����� */
    unsigned int time_stamp;            /* ��ǰ���ݰ����͵�ʱ��� */
    void *   read_ptr;                  /* ��ǰ���ݰ���Ϣ��ָ����Ե�ַ*/
    unsigned int is_continuous;         /* ��Ϣ�Ƿ�������1:������0:������ */
};

struct eicc_dma_node_info
{
    void *           src_addr; /* ����Դ��ַ */
    void *           des_addr; /* Ŀ�ĵ�ַ */
    unsigned int     msg_len;/* ���ݳ��� */
};

struct eicc_lli_attr
{
    enum EICC_DMA_TYPE type;
    struct
    {
        unsigned int is_int:1;/*bit0:�Ƿ��ϱ�д����жϱ��*/
        unsigned int is_malloc:1; /*bit1:�Ƿ�malloc��������ַ���*/
        unsigned int reserved:30; /*����λ*/
    }flag;
    void *cd_desc_addr;
    struct eicc_dma_node_info *node;
    unsigned int node_num; 
}; /*dma ��ʽ���ݰ��ƣ�����ṹ�嶨��*/

/************************************************************************
 * �� �� ��  : mdrv_eicc_open
 * ��������  :
 * �������  :
 *            channel_id: EICC �߼�ͨ����
 *            p_chan_attr: EICC ͨ�����ԣ�������ֻ����ֵ
 * �������  : ��
 * �� �� ֵ  :  0          �����ɹ���
 *             ����        ����ʧ�ܡ�
 **************************************************************************/
int mdrv_eicc_open(unsigned int channel_id, struct eicc_chan_attr *p_chan_attr);

/************************************************************************
 * �� �� ��  : mdrv_eicc_close
 * ��������  :
 * �������  :
 *            channel_id: EICC �߼�ͨ����
 * �������  : ��
 * �� �� ֵ  :  0          �����ɹ���
 *             ����        ����ʧ�ܡ�
 **************************************************************************/
int mdrv_eicc_close(unsigned int channel_id);

/************************************************************************
 * �� �� ��  : mdrv_eicc_enable_all_local_pipe
 * ��������  :
 * �������  :

 * �������  : ��
 * �� �� ֵ  :  0          �����ɹ���
 *             ����        ����ʧ�ܡ�
 **************************************************************************/
int mdrv_eicc_enable_all_local_pipe(void);

/************************************************************************
 * �� �� ��  : mdrv_eicc_disable_all_local_pipe
 * ��������  :
 * �������  :
 
 * �������  : ��
 * �� �� ֵ  :  0          �����ɹ���
 *             ����        ����ʧ�ܡ�
 **************************************************************************/
int mdrv_eicc_disable_all_local_pipe(void);

/************************************************************************
 * �� �� ��  : mdrv_eicc_read
 * ��������  :
 * �������  :
 *            u32ChanId: ICC �߼�ͨ����
 *            pData:     ���ݴ�ŵ�ַ
 *            s32Size:   �ص��������ص����ݳ���
 * �������  : pData       ����
 * �� �� ֵ  : ��ֵ        ���ݳ��ȡ�
 *             ����        ����ʧ�ܡ�
 **************************************************************************/
int mdrv_eicc_read(unsigned int channel_id, unsigned char *msg_ptr, unsigned int msg_len);

/************************************************************************
 * �� �� ��  : mdrv_eicc_write
 * ��������  :
 * �������  :
 *            u32ChanId: ICC �߼�ͨ����
 *            pData:     ���ݴ�ŵ�ַ
 *            s32Size:   ���ݳ���
 * �������  : ��
 * �� �� ֵ  : ��s32Size���  �����ɹ���
 *             ����           ����ʧ�ܡ�
 **************************************************************************/
int mdrv_eicc_write(unsigned int channel_id, unsigned char *msg_ptr, unsigned int msg_len, unsigned int is_arv_int);

/************************************************************************
 * �� �� ��  : mdrv_eicc_read_int_enable
 * ��������  :
 * �������  :
 *            channel_id: ICC �߼�ͨ����
 * �������  : ��
 * �� �� ֵ  :  0          �����ɹ���
 *             ����        ����ʧ�ܡ�
 **************************************************************************/
int mdrv_eicc_read_int_enable(unsigned int channel_id, enum EICC_READ_INT_LEV level);

/************************************************************************
 * �� �� ��  : mdrv_eicc_read_int_disable
 * ��������  :
 * �������  :
 *            channel_id: ICC �߼�ͨ����
 * �������  : ��
 * �� �� ֵ  :  0          �����ɹ���
 *             ����        ����ʧ�ܡ�
 **************************************************************************/
int mdrv_eicc_read_int_disable(unsigned int channel_id, enum EICC_READ_INT_LEV level);

/************************************************************************
 * �� �� ��  : mdrv_eicc_write_int_enable
 * ��������  :
 * �������  :
 *                          channel_id: ICC �߼�ͨ����
 *                          EICC_WRITE_INT_LEV д�ж�����
 * �������  : ��
 * �� �� ֵ  :  0          �����ɹ���
 *             ����        ����ʧ�ܡ�
 **************************************************************************/
int mdrv_eicc_write_int_enable(unsigned int channel_id, enum EICC_WRITE_INT_LEV level);

/************************************************************************
 * �� �� ��  : mdrv_eicc_write_int_disable
 * ��������  :
 * �������  :
 *                          channel_id: ICC �߼�ͨ����
 *                          EICC_WRITE_INT_LEV д�ж�����
 * �������  : ��
 * �� �� ֵ  :  0          �����ɹ���
 *             ����        ����ʧ�ܡ�
 **************************************************************************/
int mdrv_eicc_write_int_disable(unsigned int channel_id, enum EICC_WRITE_INT_LEV level);

int mdrv_eicc_currmsg_header_get(unsigned int phy_channel_id, struct eicc_currmsg_info *currmsg_info);

int mdrv_eicc_rptr_get(unsigned int channel_id, void **rptr_addr);

int mdrv_eicc_read_done(unsigned int channel_id, void *rptr_addr);

int mdrv_eicc_dma_block_start(unsigned int channel_id, struct eicc_dma_node_info *node, unsigned int is_arv_int);

int mdrv_eicc_dma_lli_start(unsigned int channel_id, struct eicc_lli_attr *lli);

int mdrv_eicc_dma_2vec_start(unsigned int channel_id, struct eicc_lli_attr *lli);

enum EICC_PIPE_STATUS mdrv_eicc_local_pipe_status_get(unsigned int channel_id);

int mdrv_eicc_is_local_pipe_empty(unsigned int channel_id);

/* Э��ջeicc�ӿڴ�׮ */
struct eicc_dtrans_bd
{
    unsigned int  msg_len     :16;/*[0-15]*/
    unsigned int  reserved1   :6; /*[16-21]*/
    unsigned int  I_pos       :1; /*[22]I*/
    unsigned int  reserved2   :9; /*[23-31]*/
};

enum ETRANS_INT_LEV
{
    ETRANS_RD_FLDN_INT = 0x1,
    ETRANS_RD_FLUP_INT = 0x2,
    ETRANS_RD_DATA_ARV_INT = 0x4,
    ETRANS_RD_DATA_ARV_TIMEOUT_INT = 0x8,
    ETRANS_RD_PIPE_WAKEUP_INT = 0x10,
    ETRANS_WR_FLDN_INT = 0x100,
    ETRANS_WR_FLUP_INT = 0x200,
    ETRANS_WR_DATA_DONE_INT = 0x400,
    ETRANS_WR_PIPE_ERR_INT = 0x800,
    ETRANS_WR_TRANS_CNT_OVF_INT = 0x1000
};

enum ETRANS_PIPE_STATUS
{
    ETRANS_PIPE_STATUS_BUSY = 0x0,
    ETRANS_PIPE_STATUS_IDLE = 0x1
};

struct eicc_dtrans_chan_attr
{
    enum    CHANL_TYPE      type;            /*ͨ������*/
    enum    FLOWLINE_UP     flup;            /*��ˮ������*/
    enum    FLOWLINE_DN     fldn;            /*��ˮ������*/
    enum    ETRANS_INT_LEV  etans_int;       /*�ж�����*/
    eicc_msg_cb             eicc_cb;         /*������ɻ���Ϣ���ջص�*/
    void *                  ram_addr;        /*ע�᱾��pipe�ռ�*/
    unsigned int            ram_size;
    unsigned int            packet_len_max;
};

int mdrv_eicc_dtrans_open(unsigned int channel_id, struct eicc_dtrans_chan_attr *p_chan_attr);

int mdrv_eicc_dtrans_close(unsigned int channel_id);

void mdrv_eicc_dtrans_optr_get(unsigned int channel_id, unsigned int *wptr_offset, unsigned int *rptr_offset);

void mdrv_eicc_dtrans_wptr_set(unsigned int channel_id, unsigned int wptr_offset);

void mdrv_eicc_dtrans_iptr_get(unsigned int channel_id, unsigned int *wptr_offset, unsigned int  *rptr_offset);

void mdrv_eicc_dtrans_rptr_set(unsigned int channel_id, unsigned int rptr_offset);

enum ETRANS_PIPE_STATUS mdrv_eicc_dtrans_opipe_status_get(unsigned int channel_id);

enum ETRANS_PIPE_STATUS mdrv_eicc_dtrans_ipipe_status_get(unsigned int channel_id);

enum ETRANS_INT_LEV mdrv_eicc_dtrans_pipe_intstat_get(unsigned int channel_id);

#ifdef __cplusplus
}
#endif
#endif
