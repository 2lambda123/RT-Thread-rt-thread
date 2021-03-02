/*
 * �����嵥����ʱ������
 *
 * ���������ʼ��2����̬��ʱ����һ���ǵ��ζ�ʱ��һ���������ԵĶ�ʱ
 */
#include <rtthread.h>
#include "tc_comm.h"

/* ��ʱ���Ŀ��ƿ� */
static struct rt_timer timer1;
static struct rt_timer timer2;

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

void timer_static_init()
{
    /* ��ʼ����ʱ�� */
    rt_timer_init(&timer1, "timer1",  /* ��ʱ�������� timer1 */
        timeout1, /* ��ʱʱ�ص��Ĵ����� */
        RT_NULL, /* ��ʱ��������ڲ��� */
        10, /* ��ʱ���ȣ���OS TickΪ��λ����10��OS Tick */
        RT_TIMER_FLAG_PERIODIC); /* �����Զ�ʱ�� */
    rt_timer_init(&timer2, "timer2",   /* ��ʱ�������� timer2 */
        timeout2, /* ��ʱʱ�ص��Ĵ����� */
        RT_NULL, /* ��ʱ��������ڲ��� */
        30, /* ��ʱ����Ϊ30��OS Tick */
        RT_TIMER_FLAG_ONE_SHOT); /* ���ζ�ʱ�� */

    /* ������ʱ�� */
    rt_timer_start(&timer1);
    rt_timer_start(&timer2);
}

#ifdef RT_USING_TC
static void _tc_cleanup()
{
    /* �����������������󣬽������л��������̣߳�����Ӧ�ж� */
    rt_enter_critical();

    /* ִ�ж�ʱ������ */
    rt_timer_detach(&timer1);
    rt_timer_detach(&timer2);

    /* ���������� */
    rt_exit_critical();

    /* ����TestCase״̬ */
    tc_done(TC_STAT_PASSED);
}

int _tc_timer_static()
{
    /* ����TestCase����ص����� */
    tc_cleanup(_tc_cleanup);

    /* ִ�ж�ʱ������ */
    timer_static_init();

    /* ����TestCase���е��ʱ�� */
    return 100;
}
/* ����������finsh shell�� */
FINSH_FUNCTION_EXPORT(_tc_timer_static, a static timer example);
#else
/* �û�Ӧ����� */
int rt_application_init()
{
    timer_static_init();

    return 0;
}
#endif
