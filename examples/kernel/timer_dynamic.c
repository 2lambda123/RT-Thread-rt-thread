/*
 * �����嵥����̬��ʱ������
 *
 * ������̻ᴴ��������̬��ʱ������һ���ǵ��ζ�ʱ��һ���������ԵĶ�ʱ
 */
#include <rtthread.h>
#include "tc_comm.h"

/* ��ʱ���Ŀ��ƿ� */
static rt_timer_t timer1;
static rt_timer_t timer2;

/* ��ʱ��1��ʱ���� */
static void timeout1(void* parameter)
{
    rt_kprintf("periodic timer is timeout\n");
}

/* ��ʱ��2��ʱ���� */
static void timeout2(void* parameter)
{
    rt_kprintf("one shot timer is timeout\n");
}

void timer_create_init()
{
    /* ������ʱ��1 */
    timer1 = rt_timer_create("timer1",  /* ��ʱ�������� timer1 */
        timeout1, /* ��ʱʱ�ص��Ĵ����� */
        RT_NULL, /* ��ʱ��������ڲ��� */
        10, /* ��ʱ���ȣ���OS TickΪ��λ����10��OS Tick */
        RT_TIMER_FLAG_PERIODIC); /* �����Զ�ʱ�� */
    /* ������ʱ�� */
    if (timer1 != RT_NULL)
        rt_timer_start(timer1);
    else
        tc_stat(TC_STAT_END | TC_STAT_FAILED);

    /* ������ʱ��2 */
    timer2 = rt_timer_create("timer2",   /* ��ʱ�������� timer2 */
        timeout2, /* ��ʱʱ�ص��Ĵ����� */
        RT_NULL, /* ��ʱ��������ڲ��� */
        30, /* ��ʱ����Ϊ30��OS Tick */
        RT_TIMER_FLAG_ONE_SHOT); /* ���ζ�ʱ�� */

    /* ������ʱ�� */
    if (timer2 != RT_NULL)
        rt_timer_start(timer2);
    else
        tc_stat(TC_STAT_END | TC_STAT_FAILED);
}

#ifdef RT_USING_TC
static void _tc_cleanup()
{
    /* �����������������󣬽������л��������̣߳�����Ӧ�ж� */
    rt_enter_critical();

    /* ɾ����ʱ������ */
    rt_timer_delete(timer1);
    rt_timer_delete(timer2);

    /* ���������� */
    rt_exit_critical();

    /* ����TestCase״̬ */
    tc_done(TC_STAT_PASSED);
}

int _tc_timer_create()
{
    /* ����TestCase����ص����� */
    tc_cleanup(_tc_cleanup);

    /* ִ�ж�ʱ������ */
    timer_create_init();

    /* ����TestCase���е��ʱ�� */
    return 100;
}
/* ����������finsh shell�� */
FINSH_FUNCTION_EXPORT(_tc_timer_create, a dynamic timer example);
#else
/* �û�Ӧ����� */
int rt_application_init()
{
    timer_create_init();

    return 0;
}
#endif
