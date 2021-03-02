/*
 * Copyright (C) 2018 Shanghai Eastsoft Microelectronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-15     liuhy       first implementation.
 */
/*
 * �����嵥������һ�� pm˯�߻��ѵ�ʹ������
 * ���̵����� pm_sample ��������ն�
 * ������ø�ʽ��pm_sample
 * ������ͣ����벻ͬ��˯��ģʽ��Ȼ���ð�������
 * �����ܣ�ͨ����������ַ�������֪����˯�ߺͻ���˯�ߵ������
*/

#include <rtthread.h>
#include <rtdevice.h>
#include "drv_pm.h"

#define PM_NAME       "pm"      /* �豸���� */
#define WAKE_UP_PIN     51      /* ����Դ */
#define SLEEP_TIMES     12      /* ����˯�ߴ������������벻ͬ��˯��ģʽ��������˯��ģʽ */


struct pm_callback_t
{
     volatile int in_fun_times;   /*���뺯���Ĵ���*/
     volatile char flag;          /*��־*/
     volatile int mode;           /*��Ҫ��ӡ��ģʽ*/
};

volatile struct pm_callback_t g_pm_data;

uint32_t save_load_mem[1024] __attribute__ ((aligned(4)));  /*���ݵĿռ�*/

/*����˯��ǰ��˯�߻��Ѻ󣬶�����롣*/
/*������ӡ˯����ص���Ϣ*/
void sleep_in_out_callback(rt_uint8_t event, rt_uint8_t mode, void *data)
{
    /*û�б�־��������*/
    if(!(g_pm_data.flag))
    {
        return;
    }

    /*��־����������ձ�־*/
    if((g_pm_data.flag) > 2)
    {
        (g_pm_data.flag) = 0;
        return;
    }

    /*ģʽ��ƥ��*/
    if(g_pm_data.mode != mode )
    {
        return;
    }

    /*������¼�*/
    switch(event)
    {
        /*����˯��ǰ*/
        case RT_PM_ENTER_SLEEP: g_pm_data.flag = 1;
                                rt_kprintf("\n\r##%d :  ENTER  ",g_pm_data.in_fun_times);
                                save_register(UART0,sizeof(UART_TypeDef),save_load_mem);    /*���ݼĴ�����ֵ*/
                                g_pm_data.in_fun_times++;     /*����˯�ߴ���+1*/
                                break;
        /*˯�߻��Ѻ�*/
        case RT_PM_EXIT_SLEEP:  g_pm_data.flag = 0;  /*˯�߻��Ѻ�*/
                                load_register(UART0,sizeof(UART_TypeDef),save_load_mem);    /*��ԭ�Ĵ�����ֵ*/
                                rt_kprintf("\n\rEXIT\n\r");
                                rt_pm_release(mode);   /*�ͷ�����ģʽ*/
                                return;

        default: break;

    };

    /*��ǰ��˯��ģʽ*/
    switch(mode)
    {
        case PM_SLEEP_MODE_NONE: rt_kprintf("PM_SLEEP_MODE_NONE\n\r");
                                break;
        case PM_SLEEP_MODE_IDLE:  rt_kprintf("PM_SLEEP_MODE_IDLE\n\r");
                                break;
        case PM_SLEEP_MODE_LIGHT: rt_kprintf("PM_SLEEP_MODE_LIGHT\n\r");
                                break;
        case PM_SLEEP_MODE_DEEP:  rt_kprintf("PM_SLEEP_MODE_DEEP\n\r");
                                break;
        case PM_SLEEP_MODE_STANDBY: rt_kprintf("PM_SLEEP_MODE_STANDBY\n\r");
                                break;
        case PM_SLEEP_MODE_SHUTDOWN:  rt_kprintf("PM_SLEEP_MODE_SHUTDOWN\n\r");
                                break;
        case PM_SLEEP_MODE_MAX:  rt_kprintf("PM_SLEEP_MODE_MAX\n\r");
                                break;
        default: break;
    }

}

/* pm���Ժ��� */
static void pm_test(void *parameter)
{
    int in_mode[7],i = 0;

        g_pm_data.in_fun_times = 0;
        g_pm_data.flag = 0;

        in_mode[0] = PM_SLEEP_MODE_NONE;
        in_mode[1] = PM_SLEEP_MODE_IDLE;
        in_mode[2] = PM_SLEEP_MODE_LIGHT;
        in_mode[3] = PM_SLEEP_MODE_DEEP;
        in_mode[4] = PM_SLEEP_MODE_STANDBY;
        in_mode[5] = PM_SLEEP_MODE_SHUTDOWN;
        in_mode[6] = PM_SLEEP_MODE_MAX;

    /*���ûص�������˽������*/
    rt_pm_notify_set(sleep_in_out_callback,RT_NULL);

    while(i < SLEEP_TIMES)
   {

       g_pm_data.mode = in_mode[i%6];

       /*������ģʽ���������־*/
       if(g_pm_data.mode != PM_SLEEP_MODE_NONE)
       {
            g_pm_data.flag = 2;

       }

       /*����ѡ�������ģʽ*/
       rt_pm_request(in_mode[i%6]);

       rt_thread_mdelay(500);

       /*������ģʽ������Ҫ����ĵȴ�*/
       while(( g_pm_data.flag != 0 )&&(g_pm_data.mode != PM_SLEEP_MODE_NONE))
       {
           rt_thread_mdelay(500);
       }

       /*�ͷ�ѡ�������ģʽ*/
       rt_pm_release(in_mode[i%6]);

       i++;

   }
      /*����ص�������˽������*/
    rt_pm_notify_set(RT_NULL,RT_NULL);
    rt_kprintf("thread pm_test close\n\r");

}

/*�������ѵĻص�����*/
void wake_by_pin(void *args)
{

}

static int pm_sample(int argc, char *argv[])
{
    rt_thread_t thread;

    /* ��������Ϊ����ģʽ */
    rt_pin_mode(WAKE_UP_PIN, PIN_MODE_INPUT_PULLUP);

    /* ���жϣ��½���ģʽ���ص�������Ϊwake_by_pin */
    rt_pin_attach_irq(WAKE_UP_PIN, PIN_IRQ_MODE_RISING, wake_by_pin, RT_NULL);
    /* ʹ���ж� */
    rt_pin_irq_enable(WAKE_UP_PIN, PIN_IRQ_ENABLE);

    thread = rt_thread_create("pm_test", pm_test, RT_NULL, 1024, 25, 10);

    if (thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }
    else
    {
        rt_kprintf("create pm_test thread failed!\n\r");
    }

    return RT_EOK;
}
/* ������ msh �����б��� */
MSH_CMD_EXPORT(pm_sample, pm sample);
