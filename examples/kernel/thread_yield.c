/*
 * �����嵥��
 */
#include <rtthread.h>
#include "tc_comm.h"

/* ָ���߳̿��ƿ��ָ�� */
static rt_thread_t tid1 = RT_NULL;
static rt_thread_t tid2 = RT_NULL;
/* �߳�1��� */
static void thread1_entry(void* parameter)
{
    rt_uint32_t count = 0;

    while (1)
    {
        /* ��ӡ�߳�1����� */
        rt_kprintf("thread1: count = %d\n", count ++);

        /* ִ��yield��Ӧ���л���thread2ִ�� */
        rt_thread_yield();
    }
}

/* �߳�2��� */
static void thread2_entry(void* parameter)
{
    rt_uint32_t count = 0;

    while (1)
    {
        /* ��ӡ�߳�2����� */
        rt_kprintf("thread2: count = %d\n", count ++);

        /* ִ��yield��Ӧ���л���thread1ִ�� */
        rt_thread_yield();
    }
}

int thread_yield_init()
{
    /* �����߳�1 */
    tid1 = rt_thread_create("thread",
        thread1_entry, RT_NULL, /* �߳������thread1_entry, ��ڲ�����RT_NULL */
        THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);
    else
        tc_stat(TC_STAT_END | TC_STAT_FAILED);

    /* �����߳�2 */
    tid2 = rt_thread_create("thread",
        thread2_entry, RT_NULL, /* �߳������thread2_entry, ��ڲ�����RT_NULL */
        THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid2 != RT_NULL)
        rt_thread_startup(tid2);
    else
        tc_stat(TC_STAT_END | TC_STAT_FAILED);

    return 0;
}

#ifdef RT_USING_TC
static void _tc_cleanup()
{
    /* �����������������󣬽������л��������̣߳�����Ӧ�ж� */
    rt_enter_critical();

    /* ɾ���߳� */
    if (tid1 != RT_NULL && tid1->stat != RT_THREAD_CLOSE)
        rt_thread_delete(tid1);
    if (tid2 != RT_NULL && tid2->stat != RT_THREAD_CLOSE)
        rt_thread_delete(tid2);

    /* ���������� */
    rt_exit_critical();

    /* ����TestCase״̬ */
    tc_done(TC_STAT_PASSED);
}

int _tc_thread_yield()
{
    /* ����TestCase����ص����� */
    tc_cleanup(_tc_cleanup);
    thread_yield_init();

    /* ����TestCase���е��ʱ�� */
    return 30;
}
/* ����������finsh shell�� */
FINSH_FUNCTION_EXPORT(_tc_thread_yield, a thread yield example);
#else
/* �û�Ӧ����� */
int rt_application_init()
{
    thread_yield_init();

    return 0;
}
#endif
