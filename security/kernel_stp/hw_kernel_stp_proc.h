/*
 * hw_kernel_stp_proc.h
 *
 * the hw_kernel_stp_proc.h for proc file create and destroy
 *
 * sunhongqing <sunhongqing@huawei.com>
 *
 * Copyright (c) 2001-2021, Huawei Tech. Co., Ltd. All rights reserved.
 */

#ifndef _HW_KERNEL_STP_PROC_H_
#define _HW_KERNEL_STP_PROC_H_

#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/uidgid.h>
#include "hw_kernel_stp_utils.h"
#include "hw_kernel_stp_scanner.h"

#define KERNEL_STP_TRIGGER_MARK "ffffffff"
#define KERNEL_STP_TRIGGER_MARK_LEN 8

int kernel_stp_proc_init(void);
void kernel_stp_proc_exit(void);

#endif