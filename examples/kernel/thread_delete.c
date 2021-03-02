/*
 * �����嵥��ɾ���߳�
 *
 * ������ӻᴴ�������̣߳���һ���߳���ɾ������һ���̡߳�
 */
#include <rtthread.h>
#include "tc_comm.h"

/*
 * �߳�ɾ��(rt_thread_delete)�������ʺ��ڶ�̬�̣߳�Ϊ����һ���߳�
 * �з�����һ���̵߳Ŀ��ƿ飬���԰��߳̿�ָ��������ȫ�������Թ�ȫ
 * �ַ���
 */
static rt_thread_t tid1 = RT_NULL, tid2 = RT_NULL;
/* �߳�1����ں��� */
static void thread1_entry(void* parameter)
{
    rt_uint32_t count = 0;

    while (1)
    {
        /* �߳�1���õ����ȼ����У�һֱ��ӡ����ֵ */
        // rt_kprintf("thread count: %d\n", count ++);
        count ++;
    }
}
static void thread1_cleanup(struct rt_thread *tid)
{
    if (tid != tid1)
    {
        tc_stat(TC_STAT_END | TC_STAT_FAILED);
        return ;
    }
    rt_kprintf("thread1 end\n");
    tid1 = RT_NULL;
}

/* �߳�2����ں��� */
static void thread2_entry(void* parameter)
{
    /* �߳�2ӵ�нϸߵ����ȼ�������ռ�߳�1�����ִ�� */

    /* �߳�2��������˯��10��OS Tick */
    rt_thread_delay(RT_TICK_PER_SECOND);

    /*
     * �߳�2���Ѻ�ֱ��ɾ���߳�1��ɾ���߳�1���߳�1�Զ���������߳�
     * ����
     */
    rt_thread_delete(tid1);

    /*
     * �߳�2��������10��OS TickȻ���˳����߳�2���ߺ�Ӧ�л���idle�߳�
     * idle�߳̽�ִ���������߳�1���ƿ���߳�ջ��ɾ��
     */
    rt_thread_delay(RT_TICK_PER_SECOND);
}

static void thread2_cleanup(struct rt_thread *tid)
{
    /*
     * �߳�2���н�����Ҳ���Զ���ɾ��(�߳̿��ƿ���߳�ջ��idle��
     * �����ͷ�)
     */

    if (tid != tid2)
    {
        tc_stat(TC_STAT_END | TC_STAT_FAILED);
        return ;
    }
    rt_kprintf("thread2 end\n");
    tid2 = RT_NULL;
    tc_done(TC_STAT_PASSED);
}

/* �߳�ɾ��ʾ���ĳ�ʼ�� */
int thread_delete_init()
{
    /* �����߳�1 */
    tid1 = rt_thread_create("t1", /* �߳�1��������t1 */
        thread1_entry, RT_NULL,   /* �����thread1_entry��������RT_NULL */
        THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid1 != RT_NULL) /* �������߳̿��ƿ飬��������߳� */
    {
        tid1->cleanup = thread1_cleanup;
        rt_thread_startup(tid1);
    }
    else
        tc_stat(TC_STAT_END | TC_STAT_FAILED);

    /* �����߳�1 */
    tid2 = rt_thread_create("t2", /* �߳�1��������t2 */
        thread2_entry, RT_NULL,   /* �����thread2_entry��������RT_NULL */
        THREAD_STACK_SIZE, THREAD_PRIORITY - 1, THREAD_TIMESLICE);
    if (tid2 != RT_NULL) /* �������߳̿��ƿ飬��������߳� */
    {
        tid2->cleanup = thread2_cleanup;
        rt_thread_startup(tid2);
    }
    else
        tc_stat(TC_STAT_END | TC_STAT_FAILED);

    return 10 * RT_TICK_PER_SECOND;
}

#ifdef RT_USING_TC
static void _tc_cleanup()
{
    /* lock scheduler */
    rt_enter_critical();

    /* delete thread */
    if (tid1 != RT_NULL)
    {
        rt_kprintf("tid1 is %p, should be NULL\n", tid1);
        tc_stat(TC_STAT_FAILED);
    }
    if (tid2 != RT_NULL)
    {
        rt_kprintf("tid2 is %p, should be NULL\n", tid2);
        tc_stat(TC_STAT_FAILED);
    }

    /* unlock scheduler */
    rt_exit_critical();
}

int _tc_thread_delete()
{
    /* set tc cleanup */
    tc_cleanup(_tc_cleanup);
    return thread_delete_init();
}
FINSH_FUNCTION_EXPORT(_tc_thread_delete, a thread delete example);
#else
int rt_application_init()
{
    thread_delete_init();

    return 0;
}
#endif
