/*
 * �����嵥���߳�Ƕ�׹���
 *
 * ����rtconfig.h�ж���RT_USING_SUSPEND_NESTING�Կ����ù���
 *
 * 2020-10-26    Meco Man    First Version
 */

#include <rtthread.h>
#include "tc_comm.h"

#ifdef RT_USING_SUSPEND_NESTING

/* ָ���߳̿��ƿ��ָ�� */
static rt_thread_t tid1 = RT_NULL;
static rt_thread_t tid2 = RT_NULL;
/* �߳�1��� */
static void thread1_entry(void* parameter)
{
    rt_uint32_t count = 0;
    
    while (1)
    {
        /* �߳�1���õ����ȼ����У�һֱ��ӡ����ֵ */
        rt_kprintf("thread1 count: %d\n", count ++);
        rt_thread_delay(200);
    }
}

/* �߳�2��� */
static void thread2_entry(void* parameter)
{
    while(1)
    {
        /* ��������5���߳�1,����Ƕ�׹��� */
        rt_thread_suspend(tid1);
        rt_thread_suspend(tid1);
        rt_thread_suspend(tid1);
        rt_thread_suspend(tid1);
        rt_thread_suspend(tid1);
        
        /* ��ӡ��ʱthread1�̵߳�Ƕ�׹������ */
        rt_kprintf("thread1 suspend_ctr: %d\n", tid1->suspend_ctr);
        
        /* ��ʱ2000��OS Tick */
        rt_thread_delay(1000);
        
        /* �������5���߳�1,����Ƕ�׽�� */
        rt_thread_resume(tid1);
        rt_thread_resume(tid1);
        rt_thread_resume(tid1);
        rt_thread_resume(tid1);
        rt_thread_resume(tid1);
        
        /* ��ӡ��ʱthread1�̵߳�Ƕ�׹������ */
        rt_kprintf("thread1 suspend_ctr: %d\n", tid1->suspend_ctr);
        
        /* ��ʱ2000��OS Tick */
        rt_thread_delay(1000);        
    }
}

int thread_suspend_init(void)
{
    /* �����߳�1 */
    tid1 = rt_thread_create("thread1",
        thread1_entry, RT_NULL, /* �߳������thread1_entry, ��ڲ�����RT_NULL */
        THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);
    else
        tc_stat(TC_STAT_END | TC_STAT_FAILED);

    /* �����߳�2 */
    tid2 = rt_thread_create("thread2",
        thread2_entry, RT_NULL, /* �߳������thread2_entry, ��ڲ�����RT_NULL */
        THREAD_STACK_SIZE, THREAD_PRIORITY - 1, THREAD_TIMESLICE);
    if (tid2 != RT_NULL)
        rt_thread_startup(tid2);
    else
        tc_stat(TC_STAT_END | TC_STAT_FAILED);

    return 0;
}

#else
    #warning "����rtconfig.h�ж���RT_USING_SUSPEND_NESTING�Կ����ù���"
#endif
