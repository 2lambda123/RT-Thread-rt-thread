/*
 * �����嵥����̬�ź���
 *
 * ��������н�����һ����̬�ź�������ʼֵΪ0 ����һ����̬�̣߳��������̬�߳���
 * ����ͼ���ó�ʱ��ʽȥ�����ź�����Ӧ�ó�ʱ���ء�Ȼ������߳��ͷ�һ���ź�������
 * �ں�������������õȴ���ʽȥ�����ź����� �ɹ�����ź����󷵻ء�
 */
#include <rtthread.h>
#include "tc_comm.h"

/* ָ���߳̿��ƿ��ָ�� */
static rt_thread_t tid = RT_NULL;
/* ָ���ź�����ָ�� */
static rt_sem_t sem = RT_NULL;
/* �߳���� */
static void thread_entry(void* parameter)
{
    rt_err_t result;
    rt_tick_t tick;

    /* ��õ�ǰ��OS Tick */
    tick = rt_tick_get();

    /* ��ͼ����һ���ź��������10��OS Tick��Ȼû�õ�����ʱ���� */
    result = rt_sem_take(sem, 10);
    if (result == -RT_ETIMEOUT)
    {
        rt_tick_t new_tick = rt_tick_get();
        /* ���������� tick ����� */
        if (new_tick - tick >= 12)
        {
            rt_kprintf("tick error to large: expect: 10, get %d\n",
                       new_tick - tick);

            /* ���ʧ�ܣ������ʧ�� */
            tc_done(TC_STAT_FAILED);
            rt_sem_delete(sem);
            return;
        }
        rt_kprintf("take semaphore timeout\n");
    }
    else
    {
        /* ��Ϊ��û�ͷ��ź�����Ӧ���ǳ�ʱ���أ��������ʧ�� */
        tc_done(TC_STAT_FAILED);
        rt_sem_delete(sem);
        return;
    }

    /* �ͷ�һ���ź��� */
    rt_sem_release(sem);

    /* ���������ź���������Զ�ȴ�ֱ�����е��ź��� */
    result = rt_sem_take(sem, RT_WAITING_FOREVER);
    if (result != RT_EOK)
    {
        /* ���ز���ȷ������ʧ�� */
        tc_done(TC_STAT_FAILED);
        rt_sem_delete(sem);
        return;
    }

    /* ���Գɹ� */
    tc_done(TC_STAT_PASSED);
    /* ɾ���ź��� */
    rt_sem_delete(sem);
}

int semaphore_dynamic_init()
{
    /* ����һ���ź�������ʼֵ��0 */
    sem = rt_sem_create("sem", 0, RT_IPC_FLAG_FIFO);
    if (sem == RT_NULL)
    {
        tc_stat(TC_STAT_END | TC_STAT_FAILED);
        return 0;
    }

    /* �����߳� */
    tid = rt_thread_create("thread",
                           thread_entry, RT_NULL, /* �߳������thread_entry, ��ڲ�����RT_NULL */
                           THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid != RT_NULL)
        rt_thread_startup(tid);
    else
        tc_stat(TC_STAT_END | TC_STAT_FAILED);

    return 0;
}

#ifdef RT_USING_TC
static void _tc_cleanup()
{
    /* �����������������󣬽������л��������̣߳�����Ӧ�ж� */
    rt_enter_critical();

    if (sem)
    {
        rt_sem_delete(sem);
        sem = RT_NULL;
    }

    /* ɾ���߳� */
    if (tid != RT_NULL && tid->stat != RT_THREAD_CLOSE)
    {
        rt_thread_delete(tid);
    }

    /* ���������� */
    rt_exit_critical();

    /* ����TestCase״̬ */
    tc_done(TC_STAT_PASSED);
}

int _tc_semaphore_dynamic()
{
    /* ����TestCase����ص����� */
    tc_cleanup(_tc_cleanup);
    semaphore_dynamic_init();

    /* ����TestCase���е��ʱ�� */
    return 100;
}
/* ����������finsh shell�� */
FINSH_FUNCTION_EXPORT(_tc_semaphore_dynamic, a dynamic semaphore example);
#else
/* �û�Ӧ����� */
int rt_application_init()
{
    semaphore_dynamic_init();

    return 0;
}
#endif
