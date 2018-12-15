

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "sim800c.h"
#include "appconfig.h"

#define SAMPLE_UART_NAME       "uart6"

static rt_device_t sim800serial;
/* ���ڽ�����Ϣ���ź��� */
static struct rt_semaphore rx_sem;
/* ���ڽ�����Ϣ���ź��� */
static struct rt_semaphore simApp_sem;
static char uart_name[RT_NAME_MAX];
static RxControl_Type simRx;
static TxControl_Type simTx;
static char sim800RXBuf[1024];
static char sim800TXBuf[1024];
static void sim800cApp_thread_entry(void* param);
/* �������ݻص����� */
static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    /* ���ڽ��յ����ݺ�����жϣ����ô˻ص�������Ȼ���ͽ����ź��� */
    rt_sem_release(&rx_sem);

    return RT_EOK;
}

void  sim800_thread_entry(void*param)
{
    rt_uint16_t len = 0;
    while (1)
    {

        while (rt_sem_take(&rx_sem, 10)==RT_EOK)/* �����ȴ������ź������ȵ��ź������ٴζ�ȡ���� */
        {
            /* �Ӵ��ڶ�ȡһ���ֽڵ����ݣ�û�ж�ȡ����ȴ������ź��� */
            len += rt_device_read(sim800serial, -1, &simRx.recBuf[len], 40);
            if(len>1024)
                len = 0;
        }
		simRx.recBuf[len+1] = '0';
        simRx.recsize = len;
		if(len>1)
			rt_sem_release(&simApp_sem);
        len = 0;
    }
}

//��������
static void tranData(unsigned char*data)
{
    if(data == 0)
        return;
    rt_device_write(sim800serial, 0, (const char*)data, strlen((const char*)data));
}

//���ڳ�ʼ��
static int initSim800c(void)
{
    rt_memset(sim800RXBuf,0,sizeof(sim800RXBuf));
    rt_memset(sim800TXBuf,0,sizeof(sim800TXBuf));
    memset(&simRx,0,sizeof(simRx));
    memset(&simTx,0,sizeof(simTx));
    simRx.recBuf = sim800RXBuf;
    simTx.trnBuf = sim800TXBuf;
    /* ����ϵͳ�еĴ����豸 */
    sim800serial = rt_device_find(SAMPLE_UART_NAME);

    if (sim800serial != RT_NULL)
    {
        rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
        rt_sem_init(&simApp_sem, "simApp", 0, RT_IPC_FLAG_FIFO);
        /* �Զ�д���жϽ��շ�ʽ�򿪴����豸 */
        rt_device_open(sim800serial, RT_DEVICE_OFLAG_RDWR|RT_DEVICE_FLAG_INT_RX);
        /* ���ý��ջص����� */
        rt_device_set_rx_indicate(sim800serial, uart_input);


    }
    else
    {
        rt_kprintf("uart sample run failed! can't find %s device!\n",uart_name);
        return RT_ERROR;
    }

    rt_thread_t tid;
    tid = rt_thread_create("sim800",sim800_thread_entry,RT_NULL,1024,9,20);
    if(tid)
        rt_thread_startup(tid);
    else
        return -RT_ERROR;

    tid = rt_thread_create("simApp",sim800cApp_thread_entry,RT_NULL,1024,11,20);
    if(tid)
        rt_thread_startup(tid);
    else
        return -RT_ERROR;
	
    return RT_EOK;

}

INIT_APP_EXPORT(initSim800c);

char* cpin = "AT+CPIN?\r\n";//��ѯ��״̬
char* creg = "AT+CREG?\r\n";//ע��

//ע��sim800c
void regSim800c(void)
{
    while(1)
    {
        tranData((rt_uint8_t*)cpin);
		rt_sem_take(&simApp_sem,500);
        if(simRx.recsize<2)
        {
            rt_thread_delay(500);
            continue;
        }
		rt_kprintf("\n%s\n",sim800RXBuf);
        simRx.recsize = 0;
        tranData((rt_uint8_t*)creg);
		rt_sem_take(&simApp_sem,500);
        if(simRx.recsize<2)
        {
            rt_thread_delay(500);
            continue;
        }
		rt_kprintf("\n%s\n",sim800RXBuf);
        simRx.recsize = 0;
//		return;
    }

}
static void sim800cApp_thread_entry(void* param)
{
    regSim800c();
    rt_kprintf("sim800c reg success!\n");
    while(1)
    {
            rt_thread_delay(500);
    }
}

