/*
 * �����嵥����̬��ʱ������
 *
 * ������̻ᴴ��1����̬�����Ͷ�ʱ������Ȼ����������ж�ʱʱ�䳤�ȵĸ��ġ�
 */
#include <rtthread.h>
#include "tc_comm.h"

/* ��ʱ���Ŀ��ƿ� */
static rt_timer_t timer1;
static rt_uint8_t count;

/* ��ʱ����ʱ���� */
static void timeout1(void* parameter)
{
    rt_tick_t timeout = 50;

    rt_kprintf("periodic timer is timeout\n");

    count ++;
    /* ֹͣ��ʱ������ */
    if (count >= 8)
    {
        /* ���ƶ�ʱ��Ȼ����ĳ�ʱʱ�䳤�� */
        rt_timer_control(timer1, RT_TIMER_CTRL_SET_TIME, (void *)&timeout);
        count = 0;
    }
}

void timer_control_init()
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
}

#ifdef RT_USING_TC
static void _tc_cleanup()
{
    /* �����������������󣬽������л��������̣߳�����Ӧ�ж� */
    rt_enter_critical();

    /* ɾ����ʱ������ */
    rt_timer_delete(timer1);
    timer1 = RT_NULL;

    /* ���������� */
    rt_exit_critical();

    /* ����TestCase״̬ */
    tc_done(TC_STAT_PASSED);
}

int _tc_timer_control()
{
    /* ����TestCase����ص����� */
    tc_cleanup(_tc_cleanup);

    /* ִ�ж�ʱ������ */
    count = 0;
    timer_control_init();

    /* ����TestCase���е��ʱ�� */
    return 100;
}
/* ����������finsh shell�� */
FINSH_FUNCTION_EXPORT(_tc_timer_control, a timer control example);
#else
/* �û�Ӧ����� */
int rt_application_init()
{
    timer_control_init();

    return 0;
}
#endif
