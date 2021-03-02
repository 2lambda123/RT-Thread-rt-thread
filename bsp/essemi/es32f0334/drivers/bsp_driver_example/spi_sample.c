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
 * �����嵥������һ�� SPI �豸ʹ������
 * ���̵����� spi_io_sample ��������ն�
 * ������ø�ʽ��spi_io_sample
 * �����ܣ�ͨ��SPI�豸�ȶ�ȡ���ݣ�Ȼ��ÿ���ַ���1�������
*/

#include <rtthread.h>
#include <rtdevice.h>

#define SPI_DEVICE_NAME     "spi00"
#define BUF_LEN             16

static void spi_io_sample(int argc, char *argv[])
{
    struct rt_spi_device * spi_dev;          /* spi�豸�ľ�� */
    struct rt_spi_configuration spi_config;
    rt_uint8_t i,buffer[BUF_LEN] = { 0U };
    rt_err_t s_stat;
    rt_err_t result;

     /* ���� spi�豸 ��ȡspi�豸��� */
    spi_dev = (struct rt_spi_device *)rt_device_find(SPI_DEVICE_NAME);

    if (spi_dev == RT_NULL)
    {
        rt_kprintf("spi sample run failed! can't find %s device!\n", SPI_DEVICE_NAME);
        return;
    }


    /* ������ýṹ�� */
    rt_memset(&spi_config,0,sizeof(struct rt_spi_configuration));

    spi_config.mode &= ~RT_SPI_SLAVE; /* ����ģʽ */
    spi_config.mode &= ~RT_SPI_3WIRE; /* 4�ߣ�˫���� */
    spi_config.mode |= RT_SPI_CPHA;   /* �ڶ����ز��� */
    spi_config.mode |= RT_SPI_CPOL;   /* ���иߵ�ƽ */
    spi_config.mode |= RT_SPI_NO_CS;  /* ��������ӻ�ѡ����� */
    spi_config.mode |= RT_SPI_MSB;    /* ��λ��ǰ */

    spi_config.data_width = 8;        /* ���ݳ��ȣ�8 */

    spi_config.max_hz = 2000000;      /* ���ʱ��Ƶ�� */

    /* ����SPI�豸 */
    s_stat = rt_spi_configure(spi_dev,&spi_config);

    if(s_stat != RT_EOK)
    {
        rt_kprintf(" spi config fail !\n ");
        return;
    }


    /* ��ȡ���� ����ֹ���߱�����߳�ͬʱʹ�� */
    result = rt_spi_take_bus(spi_dev);

    if (result != RT_EOK)
    {
        rt_kprintf(" %s take spi bus  failed! \n", SPI_DEVICE_NAME);
        return;
    }

    /* ѡ��Ƭѡ */
    result = rt_spi_take(spi_dev);

    if (result != RT_EOK)
    {
        rt_kprintf(" %s take spi cs  failed! \n", SPI_DEVICE_NAME);
        return;
    }


    /*����һ������*/
    result = rt_spi_recv(spi_dev,buffer,BUF_LEN);

    if(result != BUF_LEN)
    {
        rt_kprintf("receive fail. \n buffer is : %s \n:",buffer);

        for( i = 0 ; i < BUF_LEN ; i++)
         rt_kprintf(" %x",(unsigned int)buffer[i]);

        rt_kprintf("\n");

        return;
    }

    rt_kprintf("receive successful. \n buffer is : %s \n:",buffer);

    for( i = 0 ; i < BUF_LEN ; i++)
    rt_kprintf(" %x",(unsigned int)buffer[i]);

    rt_kprintf("\n");

    /* �����յ������ݼ�1 */
    for( i = 0 ; i < BUF_LEN ; i++)
      buffer[i]++;

    /*��������*/
    result = rt_spi_send(spi_dev,buffer,BUF_LEN);

    if(result != BUF_LEN)
    {
        rt_kprintf("send fail. \n buffer is : %s \n:",buffer);

        for( i = 0 ; i < BUF_LEN ; i++)
         rt_kprintf(" %x",(unsigned int)buffer[i]);

        rt_kprintf("\n");

        return;
    }

    rt_kprintf("send successful. \n buffer is : %s \n:",buffer);

    for( i = 0 ; i < BUF_LEN ; i++)
    rt_kprintf(" %x",(unsigned int)buffer[i]);

    rt_kprintf("\n");

    /* �ͷ�Ƭѡ */
    result = rt_spi_release(spi_dev);

    if (result != RT_EOK)
    {
        rt_kprintf(" %s release spi cs failed! \n", SPI_DEVICE_NAME);
        return;
    }

    /* �ͷ����� */
    result = rt_spi_release_bus(spi_dev);

    if (result != RT_EOK)
    {
        rt_kprintf(" %s release spi bus  failed! \n", SPI_DEVICE_NAME);
        return;
    }

}
/* ������ msh �����б��� */
MSH_CMD_EXPORT(spi_io_sample, spi  sample);
