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
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <asm/string.h>
#include <linux/kthread.h>
#include <linux/timer.h>
#include <linux/timex.h>
#include <linux/rtc.h>
#include <linux/sched.h>
#include "product_config.h"
#include "osl_types.h"
#include "bsp_sysctrl.h"
#include "bsp_slice.h"
#include "bsp_wdt.h"
#include "bsp_ipc.h"
#include "bsp_fiq.h"
#include "bsp_coresight.h"
#include "bsp_dump.h"
#include "bsp_adump.h"
#include "bsp_ddr.h"
#include "bsp_slice.h"
#include "bsp_om.h"
#include "bsp_noc.h"
#include "dump_area.h"
#include "dump_cp_agent.h"
#include "dump_cp_wdt.h"
#include "dump_config.h"
#include "dump_baseinfo.h"
#include "dump_lphy_tcm.h"
#include "dump_cphy_tcm.h"
#include "dump_file.h"
#include "dump_exc_type.h"
#include "dump_sec_mem.h"
#undef	THIS_MODU
#define THIS_MODU mod_dump
struct semaphore g_cp_agent_sem;
u32 g_rdr_mod_id = 0;



modem_cp_exc_desc  g_dump_cp_desc[]= {
    {RDR_MODEM_CP_DRV_MOD_ID,"modem cp drv err"},
    {RDR_MODEM_CP_OSA_MOD_ID,"modem cp osa err"},
    {RDR_MODEM_CP_OAM_MOD_ID,"modem cp oam err"},
    {RDR_MODEM_CP_GUL2_MOD_ID,"modem cp gul2 err"},
    {RDR_MODEM_CP_CTTF_MOD_ID,"modem cp cttf err"},
    {RDR_MODEM_CP_GUWAS_MOD_ID,"modem cp guwas err"},
    {RDR_MODEM_CP_CAS_MOD_ID,"modem cp cas err"},
    {RDR_MODEM_CP_CPROC_MOD_ID,"modem cp cproc err"},
    {RDR_MODEM_CP_GUGAS_MOD_ID,"modem cp gas err"},
    {RDR_MODEM_CP_GUCNAS_MOD_ID,"modem cp gucnas err"},
    {RDR_MODEM_CP_GUDSP_MOD_ID,"modem cp gudsp err"},
    {RDR_MODEM_CP_LPS_MOD_ID,"modem cp tlps err"},
    {RDR_MODEM_CP_LMSP_MOD_ID,"modem cp tlmsp err"},
    {RDR_MODEM_CP_TLDSP_MOD_ID,"modem cp tldsp err"},
    {RDR_MODEM_CP_CPHY_MOD_ID,"modem cp cphy err"},
    {RDR_MODEM_CP_IMS_MOD_ID,"modem cp ims err"},};


/*****************************************************************************
* �� �� ��  : dump_get_mdmcp_save_done
* ��������  : �ж�c��log�Ƿ񱣴����
*
* �������  :
* �������  :

* �� �� ֵ  :

*
* �޸ļ�¼  : 2016��1��4��17:05:33   lixiaofan  creat
*
*****************************************************************************/
s32 dump_get_mdmcp_save_done(void)
{
    s32  flag = 0;
    struct dump_area_mntn_addr_info_s area_info= {NULL,};

    flag = dump_get_level1_area_info(DUMP_AREA_LR,&area_info);
    if(flag == BSP_ERROR)
    {
        dump_error("fail to find cp area head\n");
        return BSP_ERROR;
    }

    if(area_info.vaddr == NULL)
    {
        return BSP_ERROR;
    }
    /*coverity[secure_coding]*/
    memcpy_s(&flag,sizeof(flag),(u32*)(((dump_area_head_t*)(area_info.vaddr))->version),sizeof(flag));

    /*lint -e650 -esym(650,*)*/
    if((flag == DUMP_SAVE_SUCCESS ))
    {
        return BSP_OK;
    }
    /*lint -e650 +esym(650,*)*/

    return BSP_ERROR;
}

/*****************************************************************************
* �� �� ��  : dump_clear_cpboot_area
* ��������  : ���c�������׶ε�����
*
* �������  :
* �������  :

* �� �� ֵ  :

*
* �޸ļ�¼  : 2016��1��4��17:05:33   lixiaofan  creat
*
*****************************************************************************/
void dump_clear_cpboot_area(void)
{
    unsigned long offset,base;
    offset = MNTN_AREA_CBOOT_ADDR - MNTN_BASE_ADDR;
    base = (unsigned long)dump_get_mntn_base_addr();
    if(base == 0)
    {
        return ;
    }
    /* coverity[secure_coding] */
    (void)memset_s( (void*)(base+offset),MNTN_AREA_CBOOT_SIZE,0, MNTN_AREA_CBOOT_SIZE);
}


/*****************************************************************************
* �� �� ��  : dump_save_cp_sysctrl
* ��������  : ����modem��ϵͳ���ƼĴ���
*
* �������  :
* �������  :

* �� �� ֵ  :

*
* �޸ļ�¼  : 2016��1��4��17:05:33   lixiaofan  creat
*
*****************************************************************************/
void dump_save_cp_sysctrl(void)
{
    u32 * field_addr = NULL;
    u32 * reg_addr = NULL;
    u32 reg_size = 0;

    field_addr = (u32 *)bsp_dump_get_field_addr(DUMP_CP_SYSCTRL);
    if(field_addr == NULL)
    {
        dump_error("fail to read  DUMP_CP_SYSCTRL field\n");
        return;
    }

    /* sysctrl mdm */
    reg_addr = (u32 *)bsp_sysctrl_addr_byindex(sysctrl_mdm);
    reg_size = 0x1000;

    /* coverity[dereference] */
    dump_memcpy(field_addr, reg_addr, reg_size >> 2);
    *(field_addr + (reg_size >> 2) - 4) = (u32)OM_SYSCTRL_MAGIC;
    *(field_addr + (reg_size >> 2) - 3) = (uintptr_t)bsp_sysctrl_addr_phy_byindex(sysctrl_mdm);
    *(field_addr + (reg_size >> 2) - 2) = reg_size;
    *(field_addr + (reg_size >> 2) - 1) = 0;
    //field_addr = field_addr + (reg_size >> 2);

    dump_ok("save modem sysctrl ok\n");
}





/*****************************************************************************
* �� �� ��  : dump_get_cp_task_name_by_id
* ��������  : ͨ������id����������
*
* �������  :task_id
* �������  :task_name

* �� �� ֵ  :

*
* �޸ļ�¼  : 2016��1��4��17:05:33   lixiaofan  creat
*
*****************************************************************************/
void dump_get_cp_task_name_by_id(u32 task_id, char* task_name,u32 task_name_len)
{
    dump_task_info_s * temp_task_name = NULL;
    dump_queue_t* task_name_table = NULL;
    u32 task_cnt = 0;
    u32 task_index = 0;


    /*CP�洢������������*/
    task_name_table = (dump_queue_t*)bsp_dump_get_field_addr(DUMP_CP_ALLTASK_NAME);
    if(NULL == task_name_table)
    {
        dump_error("fail to get cp task name field\n");
        return;
    }

    task_cnt = task_name_table->num / 4;

    /* ƫ���ֽڣ�ȥ������ͷ */
    task_name_table += 1;
    temp_task_name = (dump_task_info_s *)task_name_table;

    /*����������*/
    for(task_index = 0;task_index < task_cnt; task_index++)
    {
        if(temp_task_name->task_id == task_id)
        {
            dump_error("reboot task:%s\n", temp_task_name->task_name);
            /*coverity[secure_coding]*/
            memcpy_s(task_name,task_name_len,temp_task_name->task_name,12);
            break;
        }
        temp_task_name++;
    }

}

/*****************************************************************************
* �� �� ��  : dump_save_cp_base_info
* ��������  : ����modem ap�Ļ�����Ϣ
*
* �������  :
* �������  :

* �� �� ֵ  :

*
* �޸ļ�¼  : 2016��1��4��17:05:33   lixiaofan  creat
*
*****************************************************************************/
/*���Ź��쳣Ҫ���¿�����ô����*/
void dump_save_cp_base_info(u32 mod_id, u32 arg1, u32 arg2, char *data, u32 length)
{

    dump_base_info_t* modem_cp_base_info = NULL;
    dump_cpu_info_t* modem_cp_cpuinfo = NULL;
    u8* addr = NULL;
    u32 i = 0;
    dump_cp_reboot_contex* reboot_contex= NULL;
    addr = bsp_dump_get_field_addr(DUMP_CP_BASE_INFO_SMP);
    if(addr == NULL)
    {
        dump_error("fail to get cp base info\n");
        return;
    }
    modem_cp_base_info = (dump_base_info_t*)addr;

    modem_cp_base_info->modId = mod_id;
    modem_cp_base_info->arg1 = arg1;
    modem_cp_base_info->arg2 = arg2;
    modem_cp_base_info->arg3 = (u32)(uintptr_t)data;
    modem_cp_base_info->arg3_length = length;
    modem_cp_base_info->reboot_time = bsp_get_slice_value();

    if(modem_cp_base_info->cpu_max_num == 1)
    {
        dump_ok("modem cp is single core \n");
        modem_cp_base_info->reboot_cpu = 0;

        modem_cp_cpuinfo = (dump_cpu_info_t*)bsp_dump_get_field_addr(DUMP_CP_CPUINFO);
        if(modem_cp_cpuinfo == NULL)
        {
            dump_error("get modem_cp_cpuinfo fail\n");
            return;
        }

        if(modem_cp_cpuinfo->current_ctx == DUMP_CTX_TASK)
        {
            modem_cp_base_info->reboot_task = modem_cp_cpuinfo->current_task;
            dump_get_cp_task_name_by_id( modem_cp_base_info->reboot_task,(char*)(modem_cp_base_info->taskName),sizeof(modem_cp_base_info->taskName));
            modem_cp_base_info->reboot_int = (u32)(-1);
            modem_cp_base_info->reboot_context = DUMP_CTX_TASK;
        }
        else
        {
            modem_cp_base_info->reboot_task = (u32)(-1);
            modem_cp_base_info->reboot_int = modem_cp_cpuinfo->current_int;
            modem_cp_base_info->reboot_context = DUMP_CTX_INT;

        }

    }
    else
    {
        dump_ok("modem has %d core \n",modem_cp_base_info->cpu_max_num);
        addr = (u8*)bsp_dump_get_field_addr(DUMP_CP_REBOOTCONTEX);
        if(addr == NULL)
        {
            dump_error("fail to get cp reboot field\n");
            return;
        }
        /*coverity[tainted_data]*/
        for(i = 0;i<modem_cp_base_info->cpu_max_num;i++ )
        {
            modem_cp_cpuinfo = (dump_cpu_info_t*)bsp_dump_get_field_addr(DUMP_CP_CPUINFO +i);
            if(modem_cp_cpuinfo == NULL)
            {
                dump_error("fail to get modem_cp_cpuinfo field\n");
                return;
            }
            reboot_contex = (dump_cp_reboot_contex*)((uintptr_t)(addr) + i *sizeof(dump_cp_reboot_contex));
            if(modem_cp_cpuinfo->current_ctx == DUMP_CTX_TASK)
            {
                reboot_contex->reboot_context = DUMP_CTX_TASK;
                reboot_contex->reboot_task = modem_cp_cpuinfo->current_task;
                dump_get_cp_task_name_by_id( reboot_contex->reboot_task,(char*)(reboot_contex->taskName),sizeof(modem_cp_base_info->taskName));
                reboot_contex->reboot_int = (u32)(-1);
            }
            else
            {
                reboot_contex->reboot_context = (u32)(-1);
                reboot_contex->reboot_int = modem_cp_cpuinfo->current_int;
                reboot_contex->reboot_context = DUMP_CTX_INT;
            }
            dump_ok("reboot_context = 0x%x",reboot_contex->reboot_context);
            dump_ok("reboot_int = 0x%x",reboot_contex->reboot_int);
            dump_ok("reboot_task = 0x%x",reboot_contex->reboot_task);
            reboot_contex->taskName[15] = '\0';
            dump_ok("taskname is %s",reboot_contex->taskName);

        }
    }


    dump_ok("save cp base info ok\n");
    return;
}


/*****************************************************************************
* �� �� ��  : dump_wait_cp_save_done
* ��������  : ����modem cp������
*
* �������  : u32 ms  �ȴ��ĺ�����
              bool wait �Ƿ�ѭ���ȴ�
* �������  :

* �� �� ֵ  :

*
* �޸ļ�¼  : 2016��1��4��17:05:33   lixiaofan  creat
*
*****************************************************************************/
s32 dump_wait_cp_save_done(u32 ms,bool wait)
{
    u32 time_start = 0;

    time_start = bsp_get_elapse_ms();

    do{

        if( BSP_OK == dump_get_mdmcp_save_done())
        {
            dump_ok("cp save done\n");
            return BSP_OK;
        }

        if(ms <= (bsp_get_elapse_ms()-time_start))
        {
            dump_error("dump wait cp done time out\n");
            return BSP_ERROR;
        }

        if(wait)
        {
            msleep(5);
        }

    }while(1);
    /*lint -e527 -esym(527,*)*/
    return BSP_ERROR;
    /*lint -e527 +esym(527,*)*/
}
/*****************************************************************************
* �� �� ��  : dump_print_mdmcp_error
* ��������  : ��ӡCP���쳣����
*
* �������  :
* �������  :

* �� �� ֵ  :

*
* �޸ļ�¼  : 2016��1��4��17:05:33   lixiaofan  creat
*
*****************************************************************************/

void dump_print_mdmcp_error(u32 rdr_id)
{
    u32 i = 0;
    for(i = 0; i < sizeof(g_dump_cp_desc)/sizeof(g_dump_cp_desc[0]);i++)
    {
        if(rdr_id == g_dump_cp_desc[i].modem_cp_modid)
        {
            dump_ok("%s\n",g_dump_cp_desc[i].desc);
            return;
        }
    }
    dump_ok("modem cp drv err\n");
}

/*****************************************************************************
* �� �� ��  : dump_noc_modid_match
* ��������  : ƥ��NOC���쳣����
*
* �������  :
* �������  :

* �� �� ֵ  :

*
* �޸ļ�¼  : 2016��1��4��17:05:33   lixiaofan  creat
*
*****************************************************************************/

int dump_noc_modid_match(u32 modid, u32 arg)
{
    if((modid == DRV_ERRNO_MODEM_NOC) || (modid == NOC_RESET_GUC_MODID) || (modid == NOC_RESET_NXP_MODID)
         || (modid == NOC_RESET_BBP_DMA0_MODID) || (modid == NOC_RESET_BBP_DMA1_MODID) || (modid == NOC_RESET_HARQ_MODID)
          || (modid == NOC_RESET_CPHY_MODID) || (modid == NOC_RESET_GUL2_MODID))
    {
        if(arg == NOC_AP_RESET)
        {
            return NOC_AP_RESET;
        }
        else if(arg == NOC_CP_RESET)
        {
            return NOC_CP_RESET;
        }
    }
    return BSP_ERROR;
}

/*****************************************************************************
* �� �� ��  : dump_int_handle
* ��������  : ����modem cp���͹������ж�
*
* �������  :
* �������  :

* �� �� ֵ  :

*
* �޸ļ�¼  : 2016��1��4��17:05:33   lixiaofan  creat
*
*****************************************************************************/
void dump_cp_agent_handle(s32 param)
{
    dump_base_info_t* modem_cp_base_info = NULL;
    u32 rdr_mod_id = 0;
    dump_reboot_reason_t reboot_reason ;

    dump_ok("modem ccore enter system error! timestamp:0x%x\n", bsp_get_slice_value());

    if(true == dump_check_has_error())
    {
        return;
    }

    bsp_wdt_irq_disable(WDT_CCORE_ID);
    dump_ok("stop cp wdt\n");

    bsp_coresight_disable();

    modem_cp_base_info = (dump_base_info_t*)bsp_dump_get_field_addr(DUMP_CP_BASE_INFO_SMP);

    if(modem_cp_base_info == NULL)
    {
       dump_error("modem_cp_base_info is NULL\n");
       return;
    }

    reboot_reason = modem_cp_base_info->modId != DRV_ERRNO_DUMP_ARM_EXC ? DUMP_REASON_NORMAL : DUMP_REASON_ARM;

    dump_set_reboot_contex(DUMP_CPU_COMM, reboot_reason);

    dump_ok("exc core is = 0x%x,exc reason is 0x%x,modid = 0x%x\n",DUMP_CPU_COMM,reboot_reason,modem_cp_base_info->modId);

    if(DUMP_MBB == dump_get_product_type())
    {

    }
    if(NOC_AP_RESET == dump_noc_modid_match(modem_cp_base_info->modId, modem_cp_base_info->arg2))
    {
        dump_error("noc reset ap,arg = 0x%x!\n",modem_cp_base_info->arg2);
        rdr_mod_id = RDR_MODEM_AP_NOC_MOD_ID;
    }
    else if(NOC_CP_RESET == dump_noc_modid_match(modem_cp_base_info->modId, modem_cp_base_info->arg2))
    {
        dump_error("noc reset cp,arg = 0x%x!n",modem_cp_base_info->arg2);
        rdr_mod_id = RDR_MODEM_CP_NOC_MOD_ID;
    }
    else
    {
        rdr_mod_id = dump_convert_id_mdmcp2rdr(modem_cp_base_info->modId);
        dump_print_mdmcp_error(rdr_mod_id);
    }


    dump_save_base_info(BSP_MODU_OTHER_CORE,0,0,0,0);

    if(DUMP_PHONE == dump_get_product_type())
    {
        dump_save_cp_sysctrl();

        dump_save_balong_rdr_info(rdr_mod_id);
    }

    g_rdr_mod_id = rdr_mod_id;

    up(&g_cp_agent_sem);

    return;
}

/*****************************************************************************
* �� �� ��  : dump_notify_cp
* ��������  : ֪ͨmodem cp
*
* �������  :
* �������  :

* �� �� ֵ  :

*
* �޸ļ�¼  : 2016��1��4��17:05:33   lixiaofan  creat
*
*****************************************************************************/
void dump_notify_cp(u32 mod_id)
{
    s32 ret ;
    dump_reboot_cpu_t core =  DUMP_CPU_BUTTON;
    dump_reboot_reason_t reason = DUMP_REASON_UNDEF;

    dump_get_reboot_contex((u32*)&core, (u32*)&reason);

    if(core == DUMP_CPU_COMM && (reason != DUMP_REASON_WDT && reason != DUMP_REASON_DLOCK))
    {
        dump_ok("CP exception ,no need to notify C core 0x%x\n", mod_id);
    }
    else
    {
        ret = bsp_ipc_int_send(IPC_CORE_CCORE, IPC_CCPU_SRC_ACPU_DUMP);
        if(ret == BSP_OK)
        {
            dump_ok("notify modem ccore success \n");
        }
        else
        {
            dump_error("notify modem ccore fail,please let ipc check \n");
        }
    }
}

/*****************************************************************************
* �� �� ��  : dump_cp_wdt_proc
* ��������  : ���Ź��쳣����
*
* �������  :
* �������  :

* �� �� ֵ  :

*
* �޸ļ�¼  : 2016��1��4��17:05:33   lixiaofan  creat
*
*****************************************************************************/
void dump_cp_wdt_proc(void)
{
    bsp_coresight_stop_cp();
    dump_save_cp_base_info(DRV_ERRNO_DUMP_CP_WDT, DUMP_REASON_WDT, 0, NULL, 0);
    dump_set_reboot_contex(DUMP_CPU_COMM,DUMP_REASON_WDT);
    dump_ok("cp wdt exception handle ok \n");
}
/*****************************************************************************
* �� �� ��  : dump_cp_wdt_proc
* ��������  : ���Ź��쳣����
*
* �������  :
* �������  :

* �� �� ֵ  :

*
* �޸ļ�¼  : 2016��1��4��17:05:33   lixiaofan  creat
*
*****************************************************************************/
void dump_cp_dlock_proc(void)
{
    bsp_coresight_stop_cp();
    dump_save_cp_base_info(DRV_ERRNO_DLOCK, DUMP_REASON_DLOCK, 0, NULL, 0);
    dump_set_reboot_contex(DUMP_CPU_COMM,DUMP_REASON_DLOCK);
    dump_ok("cp dlock exception handle ok \n");
}

/*****************************************************************************
* �� �� ��  : dump_cp_timeout_proc
* ��������  : IPC�жϳ�ʱ�Ĵ���
*
* �������  :
* �������  :

* �� �� ֵ  :

*
* �޸ļ�¼  : 2016��1��4��17:05:33   lixiaofan  creat
*
*****************************************************************************/
void dump_cp_timeout_proc(void)
{
    s32 ret ;
    dump_reboot_cpu_t core =  DUMP_CPU_BUTTON;
    dump_reboot_reason_t reason = DUMP_REASON_UNDEF;

    dump_get_reboot_contex((u32*)&core, (u32*)&reason);

    if(core == DUMP_CPU_COMM && (reason != DUMP_REASON_WDT && reason != DUMP_REASON_DLOCK))
    {
        dump_ok("CP exception ,no need send fiq 0x%x\n", reason);
        return;
    }

    ret = bsp_send_cp_fiq(FIQ_DUMP);
    if(ret == BSP_ERROR)
    {
       /*�е���ȫosʹ�����ݴ���*/
       dump_sec_enable_trans();
       dump_error("fail to send fiq\n");
       return ;
    }
    else
    {
        dump_ok("trig fiq process success\n");
    }
    ret = dump_wait_cp_save_done(15000, true);
    if(ret == BSP_ERROR)
    {
        /*�е���ȫosʹ�����ݴ���*/
        dump_sec_enable_trans();
        dump_error("ipc fiq save log both fail\n");
    }
    else
    {
        dump_ok("fiq save log success\n");
    }
}

/*****************************************************************************
* �� �� ��  : dump_wait_mdmcp_done
* ��������  : �ȴ�mdmcp log�������
*
* �������  :
* �������  :

* �� �� ֵ  :

*
* �޸ļ�¼  : 2016��1��4��17:05:33   lixiaofan  creat
*
*****************************************************************************/

s32 dump_wait_mdmcp_done(void)
{
    s32 ret;
    dump_ok("begin to wait cp log save done\n");

    ret = dump_wait_cp_save_done(5000, (bool)true);
    if(ret == BSP_ERROR)
    {
        dump_cp_timeout_proc();
    }
    return BSP_OK;

}

/*****************************************************************************
* �� �� ��  : dump_cp_wdt_hook
* ��������  : cp ���Ź��ص�����
*
* �������  :
* �������  :

* �� �� ֵ  :

*
* �޸ļ�¼  : 2016��1��4��17:05:33   lixiaofan  creat
*
*****************************************************************************/
void dump_cp_wdt_hook(void)
{
    system_error(DRV_ERRNO_DUMP_CP_WDT, DUMP_REASON_WDT, 0, 0, 0);
}

/*****************************************************************************
* �� �� ��  : dump_cp_exc_proc_task
* ��������  : ����modem log����ں���
*
* �������  :
* �������  :

* �� �� ֵ  :

*
* �޸ļ�¼  : 2016��1��4��17:05:33   lixiaofan  creat
*
*****************************************************************************/
int dump_cp_exc_proc_task(void *data)
{
    (void)data;
    /* coverity[no_escape] */
    while(1)
    {
        down(&g_cp_agent_sem);
        dump_ok("down g_cp_agent_sem success,g_rdr_mod_id = 0x%x\n",g_rdr_mod_id);

        /*��modem ap�����noc�쳣��ͳ�Ƹ�λ������modem ap noc�쳣ֱ�Ӹ�λϵͳ*/
        if(RDR_MODEM_AP_NOC_MOD_ID !=g_rdr_mod_id)
        {
            if(BSP_OK == dump_check_reset_timestamp(g_rdr_mod_id))
            {
                rdr_system_error(g_rdr_mod_id, 0, 0);
            }
            else
            {
                dump_error("stop modem single reset");
            }
        }
        else
        {
            rdr_system_error(g_rdr_mod_id, 0, 0);
        }

    }
    /*lint -save -e527*/
    return BSP_OK;
    /*lint -restore +e527*/
}

/*****************************************************************************
* �� �� ��  : dump_save_task_init
* ��������  : ����modem ap ����log ��������
*
* �������  :
* �������  :

* �� �� ֵ  :

*
* �޸ļ�¼  : 2016��1��4��17:05:33   lixiaofan  creat
*
*****************************************************************************/
s32 dump_cp_exc_proc_task_init(void)
{
    struct task_struct * pid;
    struct sched_param   param = {0,};

    sema_init(&g_cp_agent_sem, 0);

    pid = (struct task_struct *)kthread_run(dump_cp_exc_proc_task, 0, "dump_cp_exc_proc_task");
    if (IS_ERR((void*)pid))
    {
        dump_error("fail to create kthread task! ret=%pK\n", __LINE__, pid);
        return BSP_ERROR;
    }
    param.sched_priority = 97;
    if (BSP_OK != sched_setscheduler(pid, SCHED_FIFO, &param))
    {
        dump_error("fail to setscheduler!\n", __LINE__);
        return BSP_ERROR;
    }

    dump_ok("init cp exception task ok\n");

    return BSP_OK;
}

/*****************************************************************************
* �� �� ��  : dump_cp_wdt_proc
* ��������  : ���Ź��쳣����
*
* �������  :
* �������  :

* �� �� ֵ  :

*
* �޸ļ�¼  : 2016��1��4��17:05:33   lixiaofan  creat
*
*****************************************************************************/
s32 dump_cp_agent_init(void)
{
    int ret ;

    dump_cp_exc_proc_task_init();

    ret = bsp_ipc_int_connect(IPC_ACPU_SRC_CCPU_DUMP, (voidfuncptr)dump_cp_agent_handle, 0);
    if(BSP_OK != ret)
    {
        dump_error("fail to connect ipc int\n");
        return BSP_ERROR;
    }

    ret = bsp_ipc_int_enable(IPC_ACPU_SRC_CCPU_DUMP);
    if(BSP_OK != ret)
    {
        dump_error("fail to enbale ipc int\n");
        return BSP_ERROR;
    }

    ret = bsp_wdt_register_hook(WDT_CCORE_ID,dump_cp_wdt_hook);

    if(BSP_OK != ret)
    {
        dump_error("fail to register wdt hook\n");
        return BSP_ERROR;
    }

    return BSP_OK;

}


