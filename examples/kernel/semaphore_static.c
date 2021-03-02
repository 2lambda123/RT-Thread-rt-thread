/*
 * �����嵥����̬�ź���
 *
 * ��������н�����һ����̬�ź�������ʼֵΪ0 ����һ����̬�̣߳��������̬�߳���
 * ����ͼ���ó�ʱ��ʽȥ�����ź�����Ӧ�ó�ʱ���ء�Ȼ������߳��ͷ�һ���ź�������
 * �ں�������������õȴ���ʽȥ�����ź����� �ɹ�����ź����󷵻ء�
 */
#include <rtthread.h>
#include "tc_comm.h"

/* �߳̿��ƿ鼰ջ */
static struct rt_thread thread;
static rt_uint8_t thread_stack[THREAD_STACK_SIZE];
/* �ź������ƿ� */
static struct rt_semaphore sem;

/* �߳���� */
static void thread_entry(void* parameter)
{
    rt_err_t result;
    rt_tick_t tick;

    /* ��õ�ǰ��OS Tick */
    tick = rt_tick_get();

    /* ��ͼ�����ź��������ȴ�10��OS Tick�󷵻� */
    result = rt_sem_take(&sem, 10);
    if (result == -RT_ETIMEOUT)
    {
        rt_tick_t new_tick = rt_tick_get();
        /* ���������� tick ����� */
        if (new_tick - tick >= 12)
        {
            rt_kprintf("tick error to large: expect: 10, get %d\n",
                       new_tick - tick);

            tc_done(TC_STAT_FAILED);
            rt_sem_detach(&sem);
            return;
        }
        rt_kprintf("take semaphore timeout\n");
    }
    else
    {
        /* ��Ϊû�������ط��Ƿ��ź��������Բ�Ӧ�óɹ������ź������������ʧ�� */
        tc_done(TC_STAT_FAILED);
        rt_sem_detach(&sem);
        return;
    }

    /* �ͷ�һ���ź��� */
    rt_sem_release(&sem);

    /* ���õȴ���ʽ�����ź��� */
    result = rt_sem_take(&sem, RT_WAITING_FOREVER);
    if (result != RT_EOK)
    {
        /* ���ɹ������ʧ�� */
        tc_done(TC_STAT_FAILED);
        rt_sem_detach(&sem);
        return;
    }

    /* ����ͨ�� */
    tc_done(TC_STAT_PASSED);
    /* �����ź������� */
    rt_sem_detach(&sem);
}

int semaphore_static_init(void)
{
    rt_err_t result;

    /* ��ʼ���ź�������ʼֵ��0 */
    result = rt_sem_init(&sem, "sem", 0, RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        tc_stat(TC_STAT_END | TC_STAT_FAILED);
        return 0;
    }

    /* ��ʼ���߳�1 */
    result = rt_thread_init(&thread, "thread", /* �߳�����thread */
                            thread_entry, RT_NULL, /* �̵߳������thread_entry����ڲ�����RT_NULL*/
                            &thread_stack[0], sizeof(thread_stack), /* �߳�ջ��thread_stack */
                            THREAD_PRIORITY, 10);
    if (result == RT_EOK) /* ���������ȷ�������߳�1 */
        rt_thread_startup(&thread);
    else
        tc_stat(TC_STAT_END | TC_STAT_FAILED);

    return 0;
}

#ifdef RT_USING_TC
static void _tc_cleanup(void)
{
    /* �����������������󣬽������л��������̣߳�����Ӧ�ж� */
    rt_enter_critical();

    /* ִ���߳����� */
    if (thread.stat != RT_THREAD_CLOSE)
    {
        rt_thread_detach(&thread);

        /* ִ���ź����������� */
        rt_sem_detach(&sem);
    }

    /* ���������� */
    rt_exit_critical();

    /* ����TestCase״̬ */
    tc_done(TC_STAT_PASSED);
}

int _tc_semaphore_static(void)
{
    /* ����TestCase����ص����� */
    tc_cleanup(_tc_cleanup);
    semaphore_static_init();

    /* ����TestCase���е��ʱ�� */
    return 100;
}
/* ����������finsh shell�� */
FINSH_FUNCTION_EXPORT(_tc_semaphore_static, a static semaphore example);
#else
/* �û�Ӧ����� */
int rt_application_init(void)
{
    semaphore_static_init();

    return 0;
}
#endif
