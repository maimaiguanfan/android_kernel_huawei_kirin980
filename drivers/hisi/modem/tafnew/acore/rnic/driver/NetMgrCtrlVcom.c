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

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "NetMgrCtrlVcom.h"
#include "NetMgrCtrlInterface.h"
#include "mdrv.h"

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_NET_MGR_CTRL_VCOM_C

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
NM_CTRL_CTX_STRU    g_stNmCtrlCtx = {0};
STATIC unsigned int nm_bind_pid = 0;

STATIC const struct file_operations g_stNmCtrlCdevFops =
{
    .owner   = THIS_MODULE,
    .read    = NM_CTRL_Read,
    .open    = NM_CTRL_Open,
    .release = NM_CTRL_Release,
    .poll    = NM_CTRL_Poll,
};

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

void NM_CTRL_SendMsg(void* pDataBuffer, unsigned int len)
{
    /*lint --e{429}*/
    /* ����error 429(����pstListEntry�ڴ�û���ͷţ��˴�pstListEntry�ڴ���read�������ͷţ����Ըø澯����) */
    NM_CTRL_CDEV_DATA_STRU             *pstListEntry    = VOS_NULL_PTR;
    NM_CTRL_CDEV_DATA_STRU             *pstCurEntry     = VOS_NULL_PTR;
    NM_MSG_STRU                        *pstRnicNmMsg    = VOS_NULL_PTR;
    unsigned long                       flags           = 0;

    pstRnicNmMsg = (NM_MSG_STRU *)pDataBuffer;

    NM_CTRL_LOGI("Enter MsgId(%d), len(%d).", pstRnicNmMsg->enMsgId, len);

    if (len > NM_MSG_BUFFER_SIZE)
    {
       NM_CTRL_LOGE("the length(%d) received exceed the MAX NM_MSG_BUFFER_SIZE.", len);
       return;
    }

    /* ���������ڴ�kmalloc�����ڴ洢���� */
    pstListEntry = (NM_CTRL_CDEV_DATA_STRU *)kmalloc(sizeof(NM_CTRL_CDEV_DATA_STRU) + len, GFP_KERNEL);
    if (VOS_NULL_PTR == pstListEntry)
    {
        NM_CTRL_LOGE("kmalloc data failed.");
        return;
    }

    /* ���θ澯 */
    memset(pstListEntry, 0, sizeof(NM_CTRL_CDEV_DATA_STRU) + len); /* unsafe function ignore: memset */

    pstListEntry->ulLen = len;
    memcpy(pstListEntry->aucData, pDataBuffer, len); /* unsafe function ignore: memcpy */


    /* ��ȡ�ź��� */
    spin_lock_irqsave(&(g_stNmCtrlCtx.stListLock), flags);

    if ( ID_NM_BIND_PID_CONFIG_IND != pstRnicNmMsg->enMsgId)
    {
        /* �ҽӵ�����ĩβ */
        list_add_tail(&(pstListEntry->stMsgList), &(g_stNmCtrlCtx.stListHead));/*lint !e64 */
    }
    else
    {
        /* HIFI Agent Pid��Ϣ�ҽӵ������ȼ�����ĩβ��ֻ��һ���ڵ� */
        if (list_empty(&(g_stNmCtrlCtx.stLowPriListHead)))
        {
            list_add_tail(&(pstListEntry->stMsgList), &(g_stNmCtrlCtx.stLowPriListHead));/*lint !e64 */
        }
        else
        {
            /* stLowPriListHead��ɾ��һ���ڵ㣬Ȼ������β���һ���ڵ� */
            pstCurEntry = list_first_entry(&(g_stNmCtrlCtx.stLowPriListHead), NM_CTRL_CDEV_DATA_STRU, stMsgList);
            list_del((LIST_HEAD_STRU *)pstCurEntry);/*lint !e64 */
            kfree(pstCurEntry);

            list_add_tail(&(pstListEntry->stMsgList), &(g_stNmCtrlCtx.stLowPriListHead));/*lint !e64 */
        }
    }

    g_stNmCtrlCtx.ulDataFlg = true;

    /* �ͷ��ź��� */
    spin_unlock_irqrestore(&(g_stNmCtrlCtx.stListLock), flags);

    wake_up_interruptible(&(g_stNmCtrlCtx.stReadInq));

    return;
}


VOS_VOID NM_CTRL_SendBindPidCfgInd(unsigned int ulBindPid)
{
    NM_MSG_STRU                         stRnicNmMsg;

    TAF_MEM_SET_S(&stRnicNmMsg, sizeof(NM_MSG_STRU), 0x00, sizeof(NM_MSG_STRU));

    stRnicNmMsg.enMsgId             = ID_NM_BIND_PID_CONFIG_IND;
    stRnicNmMsg.ulMsgLen            = sizeof(unsigned int);
    stRnicNmMsg.unMsgInfo.ulBindPid = ulBindPid;

    /* ���������豸�ṩ�ķ��ͽӿڷ�����Ϣ */
    NM_CTRL_SendMsg(&stRnicNmMsg, sizeof(NM_MSG_STRU));
    return;
}


int NM_CTRL_Open(struct inode *node, struct file *filp)
{
    int                                 ret = 0;

    filp->private_data = g_stNmCtrlCtx.pstNmCtrlDev;

    NM_CTRL_LOGI("Enter.");

    return ret;
}


int NM_CTRL_Release(struct inode *node, struct file *filp)
{
    LIST_HEAD_STRU                     *pstNextPtr  = VOS_NULL_PTR;
    LIST_HEAD_STRU                     *pstCurPtr   = VOS_NULL_PTR;
    NM_CTRL_CDEV_DATA_STRU             *pstCurEntry = VOS_NULL_PTR;
    unsigned long                       flags       = 0;
    int                                 ret         = 0;

    /* ��ȡ�ź��� */
    spin_lock_irqsave(&(g_stNmCtrlCtx.stListLock), flags);

    list_for_each_safe(pstCurPtr, pstNextPtr, &(g_stNmCtrlCtx.stListHead))
    {
        pstCurEntry = list_entry(pstCurPtr, NM_CTRL_CDEV_DATA_STRU, stMsgList);
        list_del(&(pstCurEntry->stMsgList));/*lint !e64 */
        kfree(pstCurEntry);
    }

    list_for_each_safe(pstCurPtr, pstNextPtr, &(g_stNmCtrlCtx.stLowPriListHead))
    {
        pstCurEntry = list_entry(pstCurPtr, NM_CTRL_CDEV_DATA_STRU, stMsgList);
        list_del(&(pstCurEntry->stMsgList));/*lint !e64 */
        kfree(pstCurEntry);
    }

    g_stNmCtrlCtx.ulDataFlg = false;

    /* �ͷ��ź��� */
    spin_unlock_irqrestore(&(g_stNmCtrlCtx.stListLock), flags);

    NM_CTRL_LOGI("Enter.");

    return ret;
}


int NM_CTRL_Read_List_Entry(LIST_HEAD_STRU *pstCurPtr, char __user *buf, size_t size)
{
    NM_CTRL_CDEV_DATA_STRU             *pstCurEntry = VOS_NULL_PTR;
    int                                 ret         = 0;

    pstCurEntry = list_first_entry(pstCurPtr, NM_CTRL_CDEV_DATA_STRU, stMsgList);

    if (size < pstCurEntry->ulLen)
    {
        NM_CTRL_LOGE("size is less than the send len");
    }

    if (size > pstCurEntry->ulLen)
    {
        NM_CTRL_LOGI("size is more than the send len");
        size = pstCurEntry->ulLen;
    }


    /* read data to user space */
    if (copy_to_user(buf, (void*)(pstCurEntry->aucData), (unsigned long)size))
    {
        ret = -EFAULT;
        NM_CTRL_LOGE("copy_to_user err");
    }
    else
    {
       ret = (int)pstCurEntry->ulLen;
       list_del((LIST_HEAD_STRU *)pstCurEntry);/*lint !e64 */
       kfree(pstCurEntry);
       NM_CTRL_LOGI("has read %d bytes.", ret);
    }

    return ret;

}


ssize_t NM_CTRL_Read(struct file *filp, char __user *buf, size_t size, loff_t *ppos)
{
    unsigned long                       flags       = 0;
    int                                 ret         = 0;

    NM_CTRL_LOGI("Enter size_t(%lu).", (unsigned long)size);

    if (filp->f_flags & O_NONBLOCK)
    {
        NM_CTRL_LOGI("NONBLOCK");
        return -EAGAIN;
    }

    if (wait_event_interruptible(g_stNmCtrlCtx.stReadInq, g_stNmCtrlCtx.ulDataFlg))
    {
        NM_CTRL_LOGE("wait_event_interruptible error");
        return -ERESTARTSYS;
    }

    /* ��ȡ�ź��� */
    spin_lock_irqsave(&(g_stNmCtrlCtx.stListLock), flags);

    /* ��ȡ������������stListHead֮���ٶ�stLowPriListHead */
    if (!list_empty(&(g_stNmCtrlCtx.stListHead)))/*lint !e64 */
    {
        ret = NM_CTRL_Read_List_Entry(&(g_stNmCtrlCtx.stListHead), buf, size);
    }
    else
    {
        if(!list_empty(&(g_stNmCtrlCtx.stLowPriListHead)))/*lint !e64 */
        {
            ret = NM_CTRL_Read_List_Entry(&(g_stNmCtrlCtx.stLowPriListHead), buf, size);
        }
    }

    /* �ж������Ƿ�Ϊ�� list_empty(g_stNmCtrlCdevp->data)������ǿգ�false������ǿգ�true; */
    if (list_empty(&(g_stNmCtrlCtx.stListHead)) && list_empty(&(g_stNmCtrlCtx.stLowPriListHead)))/*lint !e64 */
    {
        g_stNmCtrlCtx.ulDataFlg = false;
    }
    else
    {
        g_stNmCtrlCtx.ulDataFlg = true;
    }

    /* �ͷ��ź��� */
    spin_unlock_irqrestore(&(g_stNmCtrlCtx.stListLock), flags);

    return ret;
}


unsigned int NM_CTRL_Poll(struct file* filp, poll_table *wait)
{
    unsigned int                        mask = 0;

    NM_CTRL_LOGI("Enter.");

    /*put the queue into poll_table */
    poll_wait(filp, &(g_stNmCtrlCtx.stReadInq), wait);

    if (true == g_stNmCtrlCtx.ulDataFlg)
    {
       mask |= POLLIN | POLLRDNORM;
       NM_CTRL_LOGI("notify read  process.");
    }

    return mask;
}


void NM_CTRL_Setup(struct cdev * dev)
{
    int                                 err = 0;
    dev_t                               devno;

    NM_CTRL_LOGI("Enter.");

    devno = MKDEV(g_stNmCtrlCtx.ulMajorNum, 0);

    /* Init the device */
    cdev_init( dev, &g_stNmCtrlCdevFops );
    dev->owner = THIS_MODULE;
    dev->ops = &g_stNmCtrlCdevFops;

    /* add the device into devices list */
    err = cdev_add( dev, devno, 1 );

    if (err)
    {
        NM_CTRL_LOGE("Error %d adding ACT Device Failed", err);
        return;
    }
}


int __init NM_CTRL_Init(VOS_VOID)
{
    static struct class                *pstNmCtrlClass;/*lint !e565*/
    int                                 ret = 0;
    dev_t                               devno;

    NM_CTRL_LOGI("Enter.");

    /* ���θ澯 */
    memset(&g_stNmCtrlCtx, 0, sizeof(NM_CTRL_CTX_STRU)); /* unsafe function ignore: memset */

    ret = alloc_chrdev_region(&devno, 0, 1, NM_CTRL_DEVICE_NAME);

    NM_CTRL_LOGI("alloc_chrdev_region ret (%d) devno (%d).", ret, devno);

    if (ret < 0)
    {
        return ret;
    }

    g_stNmCtrlCtx.ulMajorNum = MAJOR(devno);

    /* alloc the reource */
    g_stNmCtrlCtx.pstNmCtrlDev = (struct cdev *)kmalloc(sizeof(struct cdev), GFP_KERNEL);

    if (!g_stNmCtrlCtx.pstNmCtrlDev)
    {
        ret = -ENOMEM;
        NM_CTRL_LOGE("kmalloc failed.");
        unregister_chrdev_region(devno, 1);
        return ret;
    }

    NM_CTRL_Setup(g_stNmCtrlCtx.pstNmCtrlDev);

    /* create the device node */
    pstNmCtrlClass = class_create(THIS_MODULE, NM_CTRL_DEVICE_NAME);

    if (IS_ERR(pstNmCtrlClass))
    {
       NM_CTRL_LOGE("failed to create class.");
       ret = -EFAULT;
    }
    else
    {
       device_create(pstNmCtrlClass, NULL, MKDEV(g_stNmCtrlCtx.ulMajorNum, 0), NULL, NM_CTRL_DEVICE_NAME);
    }

    /* ��ʼ�� */
    INIT_LIST_HEAD(&(g_stNmCtrlCtx.stListHead));/*lint !e64 */
    INIT_LIST_HEAD(&(g_stNmCtrlCtx.stLowPriListHead));/*lint !e64 */

    spin_lock_init(&(g_stNmCtrlCtx.stListLock));

    /* init wait queue */
    init_waitqueue_head(&(g_stNmCtrlCtx.stReadInq));

    NM_CTRL_LOGI("Exit.");

    return ret;
}

/*****************************************************************************
 Prototype    : nm_bind_pid_read
 Description  : Read function of bind pid proc file.
 Input        : file: file handle
                user_buf: the user space buffer to write to
                count: the size of the user space buffer
                ppos : the current position in the buffer
 Output       : None
 Return Value : On success, the number of bytes read is returned, or
                negative value is returned on error.
*****************************************************************************/
STATIC ssize_t nm_bind_pid_read(struct file *file, char __user *user_buf,
			size_t count, loff_t *ppos)
{
	char buf[NM_BIND_PID_LEN];
	ssize_t bytes_read;

	if (*ppos > 0) {
		NM_CTRL_LOGE("only allow read from start.");
		return 0;
	}

	memset(buf, 0, sizeof(buf));
	scnprintf(buf, sizeof(buf), "%u", nm_bind_pid);

	bytes_read = simple_read_from_buffer(user_buf, count, ppos, buf, strlen(buf));
	if (bytes_read < 0)
		NM_CTRL_LOGE("input buf read error, return: %zd.", bytes_read);

	return bytes_read;
}

/*****************************************************************************
 Prototype    : nm_bind_pid_write
 Description  : Write function of bind pid proc file.
 Input        : file: file handle
                user_buf: the user space buffer to read from
                count: the size of the user space buffer
                ppos : the current position in the buffer
 Output       : None
 Return Value : On success, the number of bytes written is returned, or
                negative value is returned on error.
*****************************************************************************/
STATIC ssize_t nm_bind_pid_write(struct file *file, const char __user *user_buf,
			size_t count, loff_t *ppos)
{
	char buf[NM_BIND_PID_LEN];
	size_t buf_size;
	ssize_t bytes_written;
	int pid_value = 0;

	if (*ppos > 0) {
		NM_CTRL_LOGE("only allow write from start.");
		return 0;
	}

	memset(buf, 0, sizeof(buf));
	buf_size = count < (sizeof(buf) - 1) ? count : (sizeof(buf) - 1);

	bytes_written = simple_write_to_buffer(buf, buf_size, ppos, user_buf, count);
	if (bytes_written <= 0) {
		NM_CTRL_LOGE("input buf read error: %d.", (int)bytes_written);
		return bytes_written;
	}

	if ((size_t)bytes_written != count) {
		NM_CTRL_LOGE("input buf too long: %s.", buf);
		return -EINVAL;
	}

	if(!NM_ISDIGIT(buf[0])) {
		NM_CTRL_LOGE("input buf is error: %s.", buf);
		return -EINVAL;
	}

	if (kstrtoint(&buf[0], 10, &pid_value)) {
		NM_CTRL_LOGE("convert to int fail: %s.", buf);
		return -EINVAL;
	}

	nm_bind_pid = (unsigned int)pid_value;
	NM_CTRL_SendBindPidCfgInd(nm_bind_pid);

	return bytes_written;
}

STATIC const struct file_operations nm_bind_pid_ops = {
	.owner = THIS_MODULE,
	.read  = nm_bind_pid_read,
	.write = nm_bind_pid_write,
};

/*****************************************************************************
 Prototype    : nm_bind_pid_init
 Description  : Init function of bind pid proc file.
 Input        : void
 Output       : None
 Return Value : Return 0 on success, negative on failure.
*****************************************************************************/
int __init nm_bind_pid_init(void)
{
	struct proc_dir_entry *proc_entry;

	proc_entry = proc_create(NM_PROC_FILE_BIND_PID, 0660, NULL, &nm_bind_pid_ops);
	if (!proc_entry) {
		NM_CTRL_LOGE("fail.");
		return -ENOMEM;
	}

	NM_CTRL_LOGI("succ.");

	return 0;
}

/*****************************************************************************
 Prototype    : nm_bind_pid_exit
 Description  : Exit function of bind pid proc file.
 Input        : void
 Output       : None
 Return Value : None
*****************************************************************************/
STATIC void __exit nm_bind_pid_exit(void)
{
	remove_proc_entry(NM_PROC_FILE_BIND_PID, NULL);
	NM_CTRL_LOGI("succ.");
}




