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
 * �����嵥������һ�� I2C �豸ʹ������
 * ���̵����� i2c_io_sample ��������ն�
 * ������ø�ʽ��i2c_io_sample
 * ������ͣ�ʹ��Ĭ�ϵ�I2C�����豸
 * �����ܣ�ͨ�� I2C �豸�������ݲ���ӡ��Ȼ�󽫽��յ��ַ���1�����
*/

#include <rtthread.h>
#include <rtdevice.h>

#define I2C_BUS_NAME          "i2c1"  /* I2C�����豸���� */
#define SLAVE_ADDR                  0x2D    /* �ӻ���ַ */
#define STR_LEN                       16    /* ���շ��͵����ݳ��� */

static void i2c_io_sample(int argc, char *argv[])
{

    struct rt_i2c_bus_device *i2c_bus = RT_NULL;     /* I2C�����豸��� */
    struct rt_i2c_msg temp_msg;                             /* I2C��Ϣ */
    rt_uint8_t buffer[STR_LEN] = { 0U };
    rt_uint32_t i,num_msg;
    rt_size_t s_stat;

    i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(I2C_BUS_NAME);    /* ͨ�����ֻ�ȡI2C�����豸�ľ�� */

    if( i2c_bus == RT_NULL)
    {
        rt_kprintf("can't find i2c device :%s !\n",I2C_BUS_NAME);

        return;
    }

        /*��ʼ����Ϣ*/
        temp_msg.addr = SLAVE_ADDR;  /* �ӻ���ַ */
        temp_msg.len  = STR_LEN;     /* ��������ݳ��� */
        temp_msg.buf = buffer;       /* ��д������ */

        num_msg = 1;             /* ����һ����Ϣ */

        temp_msg.flags = RT_I2C_RD;             /* I2C�� */
        s_stat = rt_i2c_transfer(i2c_bus,&temp_msg,num_msg);   /* ������Ϣ */

        rt_thread_mdelay(400);

        if( s_stat == num_msg )
        {
            rt_kprintf("receive successful. \n receive messege : %s \n:",buffer);

            for( i = 0 ; i < STR_LEN ; i++)
            rt_kprintf(" %x",(unsigned int)buffer[i]);

            rt_kprintf("\n");
        }
        else
        {
            rt_kprintf("device s% recieve fail \n buffer : s%\n",I2C_BUS_NAME,buffer);
            return;
        }

        for( i = 0 ; i < STR_LEN ; i++)
        buffer[i]++;

        temp_msg.flags = RT_I2C_WR;             /* I2Cд */
        s_stat = rt_i2c_transfer(i2c_bus,&temp_msg,num_msg);   /* ����һ�� */

        rt_thread_mdelay(400);

        if( s_stat == num_msg )
        {
            rt_kprintf(" send successful \n messege : %s \n:",buffer);

            for( i = 0 ; i < STR_LEN ; i++)
            rt_kprintf(" %x",(unsigned int)buffer[i]);

            rt_kprintf("\n");
        }
        else
        {
            rt_kprintf("device s% send fail \n",I2C_BUS_NAME);
            return;
        }

        return;

}
/* ������ msh �����б��� */
MSH_CMD_EXPORT(i2c_io_sample, i2c io sample);
