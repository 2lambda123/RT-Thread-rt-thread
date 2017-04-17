/*********************************************************************************************************
**
**                                ����ŵ����ͨ��Ϣ�Ƽ����޹�˾
**
**                               Copyright All Rights Reserved
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**
** �� �� ��: cfs-coffee_arch.c
**
** �� �� ��: Urey(��Ʈ����)
**
** ��������: 2015��11��12��
**
** ��    ��: TODO
*********************************************************************************************************/


/*********************************************************************************************************
**   ͷ�ļ�
*********************************************************************************************************/
#include <rtthread.h>
#include <rtdevice.h>

#include <cfs.h>


/*********************************************************************************************************
**   �궨��
*********************************************************************************************************/
#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) rt_kprintf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif



/*********************************************************************************************************
**   ȫ�ֱ���
*********************************************************************************************************/
static rt_device_t              xmem_device = RT_NULL;
struct rt_device_blk_geometry   xmem_geometry = {0};

int xmem_init(rt_device_t device)
{
    int result ;

    RT_ASSERT(device != RT_NULL);

    xmem_device = device;

    //��ȡ mem �豸������Ϣ
    result = rt_device_control(xmem_device,RT_DEVICE_CTRL_BLK_GETGEOME,&xmem_geometry);
    RT_ASSERT(result == RT_EOK);

    return result;
}


int xmem_erase(long size, unsigned long addr)
{
    unsigned long end = addr + size;

    RT_ASSERT(xmem_device != RT_NULL);

    if(size % XMEM_ERASE_UNIT_SIZE != 0) {
      PRINTF("xmem_erase: bad size\n");
      return -1;
    }

    if(addr % XMEM_ERASE_UNIT_SIZE != 0) {
      PRINTF("xmem_erase: bad offset\n");
      return -1;
    }

    for (; addr < end; addr += XMEM_ERASE_UNIT_SIZE)
    {
        rt_device_control(xmem_device, RT_DEVICE_CTRL_BLK_ERASE, (void *) addr);
    }

    return size;
}

int xmem_pwrite(const void *_buf, int size, unsigned long addr)
{
    const unsigned char *p = _buf;
    const unsigned long end = addr + size;
    unsigned long i, next_page;

    RT_ASSERT(xmem_device != RT_NULL);

    for (i = addr; i < end;)
    {
        next_page = (i | 0xff) + 1;
        if (next_page > end)
        {
            next_page = end;
        }
        p += rt_device_write(xmem_device, i, p, next_page - i);
        i = next_page;
    }

    return size;
}


int xmem_pread(void *_p, int size, unsigned long offset)
{
    int retSize;

    RT_ASSERT(xmem_device != RT_NULL);

    retSize = rt_device_read(xmem_device,offset,_p,size);

    return retSize;
}


