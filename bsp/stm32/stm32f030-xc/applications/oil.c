

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "oil.h"
#include "appconfig.h"


#define SAMPLE_UART_NAME       "uart4"

static rt_device_t oilserial;
/* ���ڽ�����Ϣ���ź��� */
static struct rt_semaphore rx_sem;
static char uart_name[RT_NAME_MAX];
static RxControl_Type oilRx;
static TxControl_Type oilTx;
static char oilRXBuf[1024];
static char oilTXBuf[1024];
/* �������ݻص����� */
static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    /* ���ڽ��յ����ݺ�����жϣ����ô˻ص�������Ȼ���ͽ����ź��� */
    rt_sem_release(&rx_sem);

    return RT_EOK;
}

void  oil_thread_entry(void*param)
{
    rt_uint16_t len = 0;
    while (1)
    {

        while (rt_sem_take(&rx_sem, 10)==RT_EOK)/* �����ȴ������ź������ȵ��ź������ٴζ�ȡ���� */
        {
            /* �Ӵ��ڶ�ȡһ���ֽڵ����ݣ�û�ж�ȡ����ȴ������ź��� */
            len += rt_device_read(oilserial, -1, &oilRx.recBuf[len], 40);
            if(len>1024)
                len = 0;
        }
        oilRx.recsize = len;
        len = 0;
    }
}

//��������
static void tranData(unsigned char*data)
{
    if(data == 0)
        return;
    rt_device_write(oilserial, 0, (const char*)data, strlen((const char*)data));
}

//���ڳ�ʼ��
static int initoil(void)
{
    rt_memset(oilRXBuf,0,sizeof(oilRXBuf));
    rt_memset(oilTXBuf,0,sizeof(oilTXBuf));
    memset(&oilRx,0,sizeof(oilRx));
    memset(&oilTx,0,sizeof(oilTx));
    oilRx.recBuf = oilRXBuf;
    oilTx.trnBuf = oilTXBuf;
    /* ����ϵͳ�еĴ����豸 */
    oilserial = rt_device_find(SAMPLE_UART_NAME);

    if (oilserial != RT_NULL)
    {
        rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
        /* �Զ�д���жϽ��շ�ʽ�򿪴����豸 */
        rt_device_open(oilserial, RT_DEVICE_OFLAG_RDWR|RT_DEVICE_FLAG_INT_RX);
        /* ���ý��ջص����� */
        rt_device_set_rx_indicate(oilserial, uart_input);


    }
    else
    {
        rt_kprintf("uart sample run failed! can't find %s device!\n",uart_name);
        return RT_ERROR;
    }

    rt_thread_t tid;
    tid = rt_thread_create("oil",oil_thread_entry,RT_NULL,1024,8,20);
    if(tid)
        rt_thread_startup(tid);
    else
        return RT_ERROR;
    return RT_EOK;
}

INIT_APP_EXPORT(initoil);




