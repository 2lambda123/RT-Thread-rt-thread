/*
 * �����嵥���߳�����
 *
 * ������ӻᴴ�������̣߳�������һ���߳���ִ�ж���һ���̵߳����롣
 */
#include <rtthread.h>
#include "tc_comm.h"

/* �߳�1���ƿ� */
static struct rt_thread thread1;
/* �߳�1ջ */
static rt_uint8_t thread1_stack[THREAD_STACK_SIZE];
/* �߳�2���ƿ� */
static struct rt_thread thread2;
/* �߳�2ջ */
static rt_uint8_t thread2_stack[THREAD_STACK_SIZE];

/* �߳�1��� */
static void thread1_entry(void* parameter)
{
    rt_uint32_t count = 0;

    while (1)
    {
        /* �߳�1���õ����ȼ����У�һֱ��ӡ����ֵ */
        rt_kprintf("thread count: %d\n", count ++);
    }
}

/* �߳�2��� */
static void thread2_entry(void* parameter)
{
    /* �߳�2ӵ�нϸߵ����ȼ�������ռ�߳�1�����ִ�� */

    /* �߳�2��������˯��10��OS Tick */
    rt_thread_delay(10);

    /*
     * �߳�2���Ѻ�ֱ��ִ���߳�1���룬�߳�1���Ӿ����̶߳�����ɾ��
     */
    rt_thread_detach(&thread1);

    /*
     * �߳�2��������10��OS TickȻ���˳�
     */
    rt_thread_delay(10);

    /*
     * �߳�2���н�����Ҳ���Զ����Ӿ���������ɾ�����������̶߳���
     */
}

int thread_detach_init()
{
    rt_err_t result;

    /* ��ʼ���߳�1 */
    result = rt_thread_init(&thread1, "t1", /* �߳�����t1 */
        thread1_entry, RT_NULL, /* �̵߳������thread1_entry����ڲ�����RT_NULL*/
        &thread1_stack[0], sizeof(thread1_stack), /* �߳�ջ��thread1_stack */
        THREAD_PRIORITY, 10);
    if (result == RT_EOK) /* ���������ȷ�������߳�1 */
        rt_thread_startup(&thread1);
    else
        tc_stat(TC_STAT_END | TC_STAT_FAILED);

    /* ��ʼ���߳�2 */
    result = rt_thread_init(&thread2, "t2", /* �߳�����t2 */
        thread2_entry, RT_NULL, /* �̵߳������thread2_entry����ڲ�����RT_NULL*/
        &thread2_stack[0], sizeof(thread2_stack), /* �߳�ջ��thread2_stack */
        THREAD_PRIORITY - 1, 10);
    if (result == RT_EOK) /* ���������ȷ�������߳�2 */
        rt_thread_startup(&thread2);
    else
        tc_stat(TC_STAT_END | TC_STAT_FAILED);

    return 0;
}

#ifdef RT_USING_TC
static void _tc_cleanup()
{
    /* �����������������󣬽������л��������̣߳�����Ӧ�ж� */
    rt_enter_critical();

    /* ִ���߳����� */
    if (thread1.stat != RT_THREAD_CLOSE)
        rt_thread_detach(&thread1);
    if (thread2.stat != RT_THREAD_CLOSE)
        rt_thread_detach(&thread2);

    /* ���������� */
    rt_exit_critical();

    /* ����TestCase״̬ */
    tc_done(TC_STAT_PASSED);
}

int _tc_thread_detach()
{
    /* ����TestCase����ص����� */
    tc_cleanup(_tc_cleanup);
    thread_detach_init();

    /* ����TestCase���е��ʱ�� */
    return 25;
}
/* ����������finsh shell�� */
FINSH_FUNCTION_EXPORT(_tc_thread_detach, a static thread example);
#else
/* �û�Ӧ����� */
int rt_application_init()
{
    thread_detach_init();

    return 0;
}
#endif
