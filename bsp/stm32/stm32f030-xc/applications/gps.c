

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "gps.h"
#include "appconfig.h"


#define SAMPLE_UART_NAME       "uart2"

static rt_device_t gpsserial;
/* ���ڽ�����Ϣ���ź��� */
static struct rt_semaphore rx_sem;
static char uart_name[RT_NAME_MAX];
static RxControl_Type gpsRx;
static TxControl_Type gpsTx;
static char gpsRXBuf[1024];
static char gpsTXBuf[1024];
/* �������ݻص����� */
static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    /* ���ڽ��յ����ݺ�����жϣ����ô˻ص�������Ȼ���ͽ����ź��� */
    rt_sem_release(&rx_sem);

    return RT_EOK;
}

void  gps_thread_entry(void*param)
{
    rt_uint16_t len = 0;
    while (1)
    {

        while (rt_sem_take(&rx_sem, 10)==RT_EOK)/* �����ȴ������ź������ȵ��ź������ٴζ�ȡ���� */
        {
            /* �Ӵ��ڶ�ȡһ���ֽڵ����ݣ�û�ж�ȡ����ȴ������ź��� */
            len += rt_device_read(gpsserial, -1, &gpsRx.recBuf[len], 40);
            if(len>1024)
                len = 0;
        }
        gpsRx.recsize = len;
        len = 0;
    }
}

//��������
static void tranData(unsigned char*data)
{
    if(data == 0)
        return;
    rt_device_write(gpsserial, 0, (const char*)data, strlen((const char*)data));
}

//���ڳ�ʼ��
static int initGps(void)
{
    rt_memset(gpsRXBuf,0,sizeof(gpsRXBuf));
    rt_memset(gpsTXBuf,0,sizeof(gpsTXBuf));
    memset(&gpsRx,0,sizeof(gpsRx));
    memset(&gpsTx,0,sizeof(gpsTx));
    gpsRx.recBuf = gpsRXBuf;
    gpsTx.trnBuf = gpsTXBuf;
    /* ����ϵͳ�еĴ����豸 */
    gpsserial = rt_device_find(SAMPLE_UART_NAME);

    if (gpsserial != RT_NULL)
    {
        rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
        /* �Զ�д���жϽ��շ�ʽ�򿪴����豸 */
        rt_device_open(gpsserial, RT_DEVICE_OFLAG_RDWR|RT_DEVICE_FLAG_INT_RX);
        /* ���ý��ջص����� */
        rt_device_set_rx_indicate(gpsserial, uart_input);


    }
    else
    {
        rt_kprintf("uart sample run failed! can't find %s device!\n",uart_name);
        return RT_ERROR;
    }

    rt_thread_t tid;
    tid = rt_thread_create("gps",gps_thread_entry,RT_NULL,1024,10,20);
    if(tid)
        rt_thread_startup(tid);
    else
        return RT_ERROR;
    return RT_EOK;
}

INIT_APP_EXPORT(initGps);




