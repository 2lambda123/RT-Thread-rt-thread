/*
 * �����嵥�������߳�
 *
 * ��������н�����������̬�̣߳������ȼ��߳̽���������Ȼ��
 * �����ȼ��߳̽���һ��ʱ�̺��ѵ����ȼ��̡߳�
 */
#include <rtthread.h>
#include "tc_comm.h"

/* ָ���߳̿��ƿ��ָ�� */
static rt_thread_t tid1 = RT_NULL;
static rt_thread_t tid2 = RT_NULL;
/* �߳�1��� */
static void thread1_entry(void* parameter)
{
    /* �����ȼ��߳�1��ʼ���� */
    rt_kprintf("thread1 startup%d\n");

    /* �������� */
    rt_kprintf("suspend thread self\n");
    rt_thread_suspend(tid1);
    /* ����ִ���̵߳��� */
    rt_schedule();

    /* ���߳�1������ʱ */
    rt_kprintf("thread1 resumed\n");
}
static void thread_cleanup(rt_thread_t tid)
{
    if (tid == tid1)
    {
        tid1 = RT_NULL;
    }
    if (tid == tid2)
    {
        tid = RT_NULL;
    }
}

/* �߳�2��� */
static void thread2_entry(void* parameter)
{
    /* ��ʱ10��OS Tick */
    rt_thread_delay(10);

    /* �����߳�1 */
    rt_thread_resume(tid1);
    rt_kprintf("thread2: to resume thread1\n");

    /* ��ʱ10��OS Tick */
    rt_thread_delay(10);

    /* �߳�2�Զ��˳� */
}

int thread_resume_init()
{
    /* �����߳�1 */
    tid1 = rt_thread_create("thread",
        thread1_entry, RT_NULL, /* �߳������thread1_entry, ��ڲ�����RT_NULL */
        THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid1 != RT_NULL)
    {
        tid1->cleanup = thread_cleanup;
        rt_thread_startup(tid1);
    }
    else
        tc_stat(TC_STAT_END | TC_STAT_FAILED);

    /* �����߳�2 */
    tid2 = rt_thread_create("thread",
        thread2_entry, RT_NULL, /* �߳������thread2_entry, ��ڲ�����RT_NULL */
        THREAD_STACK_SIZE, THREAD_PRIORITY - 1, THREAD_TIMESLICE);
    if (tid2 != RT_NULL)
    {
        tid2->cleanup = thread_cleanup;
        rt_thread_startup(tid2);
    }
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

int _tc_thread_resume()
{
    /* ����TestCase����ص����� */
    tc_cleanup(_tc_cleanup);
    thread_resume_init();

    /* ����TestCase���е��ʱ�� */
    return 25;
}
/* ����������finsh shell�� */
FINSH_FUNCTION_EXPORT(_tc_thread_resume, a thread resume example);
#else
/* �û�Ӧ����� */
int rt_application_init()
{
    thread_resume_init();

    return 0;
}
#endif
