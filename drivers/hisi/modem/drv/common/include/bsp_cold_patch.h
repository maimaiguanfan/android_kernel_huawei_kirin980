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

#ifndef __BSP_COLD_PATCH_H__
#define __BSP_COLD_PATCH_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <osl_types.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>

enum modem_patch_status
{
    PUT_PATCH_SUCESS,
    NOT_LOAD_PATCH,
    LOAD_PATCH_FAIL,
    PUT_PATCH,
    PUT_PATCH_FAIL,
};

#define NVME_COLD_PATCH_ID 260

#define SECBOOT_RET_COLD_PATCH_SPLICING_FAIL (0xFFFFFF22)

enum modem_patch_type
{
    CCORE_PATCH,
    NV_PATCH,
    DSP_PATCH,
    RESERVED1,
    RESERVED2,
    RESERVED3,
    RESERVED4,
    MAX_PATCH,
};

struct modem_patch_info_s
{
    char patch_exist;  //�����Ƿ����
    char patch_status;  //�Ƿ��Ѿ��򲹶�
    char reserved1;
    char reserved2;
};

struct cold_patch_info_s
{
   char cold_patch_status;  //�����Ƿ��Ѿ����ϣ�ֻҪ����һ������������ء�ƴ�ӳɹ����ó�Ա�ͻᱻ��1;
                           //modem���쳣��λʱ������ó�Ա����Ϊ1�����³�Աmodem_update_fail_count
   char modem_patch_update_count; //�䲹�����´��� 
   char modem_update_fail_count;  //modem����ʧ�ܴ��������ó�Ա����3�κ��ٴ�����ʱ�Ͳ�����ز�������
   struct modem_patch_info_s modem_patch_info[MAX_PATCH]; //�ֱ��¼nv��dsp��ccore�Ĳ�����Ϣ,ʣ�µ�Ԥ��
};

int bsp_nvem_cold_patch_read(struct cold_patch_info_s *p_cold_patch_info);
int bsp_nvem_cold_patch_write(struct cold_patch_info_s *p_cold_patch_info);
bool bsp_modem_cold_patch_is_exist(void);
ssize_t modem_imag_patch_status_store(struct device *dev, struct device_attribute *attr,const char *buf, size_t count);
ssize_t modem_imag_patch_status_show(struct device *dev,struct device_attribute *attr, char *buf);
int bsp_modem_cold_patch_init(void);

#ifdef __cplusplus
}
#endif

#endif
