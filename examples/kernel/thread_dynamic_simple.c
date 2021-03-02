/*
 * �����嵥����̬�߳�
 *
 * ���������ʼ��2����̬�̣߳�����ӵ�й�ͬ����ں���������������ͬ
 */
#include <rtthread.h>
#include "tc_comm.h"

/* ָ���߳̿��ƿ��ָ�� */
static rt_thread_t tid1 = RT_NULL;
static rt_thread_t tid2 = RT_NULL;
/* �߳���� */
static void thread_entry(void* parameter)
{
    rt_uint32_t count = 0;
    rt_uint32_t no = (rt_uint32_t) parameter; /* �����ȷ����ڲ��� */

    while (1)
    {
        /* ��ӡ�̼߳���ֵ��� */
        rt_kprintf("thread%d count: %d\n", no, count ++);

        /* ����10��OS Tick */
        rt_thread_delay(10);
    }
}

int thread_dynamic_simple_init()
{
    /* �����߳�1 */
    tid1 = rt_thread_create("t1",
        thread_entry, (void*)1, /* �߳������thread_entry, ��ڲ�����1 */
        THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);
    else
        tc_stat(TC_STAT_END | TC_STAT_FAILED);

    /* �����߳�2 */
    tid2 = rt_thread_create("t2",
        thread_entry, (void*)2, /* �߳������thread_entry, ��ڲ�����2 */
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

int _tc_thread_dynamic_simple()
{
    /* ����TestCase����ص����� */
    tc_cleanup(_tc_cleanup);
    thread_dynamic_simple_init();

    /* ����TestCase���е��ʱ�� */
    return 100;
}
/* ����������finsh shell�� */
FINSH_FUNCTION_EXPORT(_tc_thread_dynamic_simple, a dynamic thread example);
#else
/* �û�Ӧ����� */
int rt_application_init()
{
    thread_dynamic_simple_init();

    return 0;
}
#endif
