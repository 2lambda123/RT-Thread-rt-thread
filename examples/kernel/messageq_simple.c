/*
 * �����嵥����Ϣ��������
 *
 * �������ᴴ��3����̬�̣߳�һ���̻߳����Ϣ��������ȡ��Ϣ��һ���̻߳ᶨʱ����
 * Ϣ���з�����Ϣ��һ���̻߳ᶨʱ����Ϣ���з��ͽ�����Ϣ��
 */
#include <rtthread.h>
#include "tc_comm.h"

/* ָ���߳̿��ƿ��ָ�� */
static rt_thread_t tid1 = RT_NULL;
static rt_thread_t tid2 = RT_NULL;
static rt_thread_t tid3 = RT_NULL;

/* ��Ϣ���п��ƿ� */
static struct rt_messagequeue mq;
/* ��Ϣ�������õ��ķ�����Ϣ���ڴ�� */
static char msg_pool[2048];

/* �߳�1��ں��� */
static void thread1_entry(void* parameter)
{
    char buf[128];

    while (1)
    {
        rt_memset(&buf[0], 0, sizeof(buf));

        /* ����Ϣ�����н�����Ϣ */
        if (rt_mq_recv(&mq, &buf[0], sizeof(buf), RT_WAITING_FOREVER) == RT_EOK)
        {
            rt_kprintf("thread1: recv msg from message queue, the content:%s\n", buf);
        }

        /* �ӳ�10��OS Tick */
        rt_thread_delay(10);
    }
}

/* �߳�2��ں��� */
static void thread2_entry(void* parameter)
{
    int i, result;
    char buf[] = "this is message No.x";

    while (1)
    {
        for (i = 0; i < 10; i++)
        {
            buf[sizeof(buf) - 2] = '0' + i;

            rt_kprintf("thread2: send message - %s\n", buf);
            /* ������Ϣ����Ϣ������ */
            result = rt_mq_send(&mq, &buf[0], sizeof(buf));
            if ( result == -RT_EFULL)
            {
                /* ��Ϣ�������� �ӳ�1sʱ�� */
                rt_kprintf("message queue full, delay 1s\n");
                rt_thread_delay(100);
            }
        }

        /* ��ʱ10��OS Tick */
        rt_thread_delay(10);
    }
}

/* �߳�3��ں��� */
static void thread3_entry(void* parameter)
{
    char buf[] = "this is an urgent message!";

    while (1)
    {
        rt_kprintf("thread3: send an urgent message\n");

        /* ���ͽ�����Ϣ����Ϣ������ */
        rt_mq_urgent(&mq, &buf[0], sizeof(buf));

        /* ��ʱ25��OS Tick */
        rt_thread_delay(25);
    }
}

int messageq_simple_init()
{
    /* ��ʼ����Ϣ���� */
    rt_mq_init(&mq, "mqt",
               &msg_pool[0],        /* �ڴ��ָ��msg_pool */
               128 - sizeof(void*), /* ÿ����Ϣ�Ĵ�С�� 128 - void* */
               sizeof(msg_pool),    /* �ڴ�صĴ�С��msg_pool�Ĵ�С */
               RT_IPC_FLAG_FIFO);   /* ����ж���̵߳ȴ������������ȵõ��ķ���������Ϣ */

    /* �����߳�1 */
    tid1 = rt_thread_create("t1",
                            thread1_entry, RT_NULL, /* �߳������thread1_entry, ��ڲ�����RT_NULL */
                            THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);
    else
        tc_stat(TC_STAT_END | TC_STAT_FAILED);

    /* �����߳�2 */
    tid2 = rt_thread_create("t2",
                            thread2_entry, RT_NULL, /* �߳������thread2_entry, ��ڲ�����RT_NULL */
                            THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid2 != RT_NULL)
        rt_thread_startup(tid2);
    else
        tc_stat(TC_STAT_END | TC_STAT_FAILED);

    /* �����߳�3 */
    tid3 = rt_thread_create("t3",
                            thread3_entry, RT_NULL, /* �߳������thread2_entry, ��ڲ�����RT_NULL */
                            THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid3 != RT_NULL)
        rt_thread_startup(tid3);
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
    if (tid3 != RT_NULL && tid3->stat != RT_THREAD_CLOSE)
        rt_thread_delete(tid3);

    /* ִ����Ϣ���ж������� */
    rt_mq_detach(&mq);

    /* ���������� */
    rt_exit_critical();

    /* ����TestCase״̬ */
    tc_done(TC_STAT_PASSED);
}

int _tc_messageq_simple()
{
    /* ����TestCase����ص����� */
    tc_cleanup(_tc_cleanup);
    messageq_simple_init();

    /* ����TestCase���е��ʱ�� */
    return 100;
}
/* ����������finsh shell�� */
FINSH_FUNCTION_EXPORT(_tc_messageq_simple, a simple message queue example);
#else
/* �û�Ӧ����� */
int rt_application_init()
{
    messageq_simple_init();

    return 0;
}
#endif
