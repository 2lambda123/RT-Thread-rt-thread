/*
 * �����嵥������������������
 *
 * ��������н����������߳�����ʵ������������������
 */
#include <rtthread.h>
#include "tc_comm.h"

/* �������5��Ԫ���ܹ������� */
#define MAXSEM 5

/* ���ڷ����������������� */
rt_uint32_t array[MAXSEM];
/* ָ�������ߡ���������array�����еĶ�дλ�� */
static rt_uint32_t set, get;

/* ָ���߳̿��ƿ��ָ�� */
static rt_thread_t producer_tid = RT_NULL;
static rt_thread_t consumer_tid = RT_NULL;

struct rt_semaphore sem_lock;
struct rt_semaphore sem_empty, sem_full;

/* �������߳���� */
void producer_thread_entry(void* parameter)
{
    int cnt = 0;

    /* ����100�� */
    while( cnt < 100)
    {
        /* ��ȡһ����λ */
        rt_sem_take(&sem_empty, RT_WAITING_FOREVER);

        /* �޸�array���ݣ����� */
        rt_sem_take(&sem_lock, RT_WAITING_FOREVER);
        array[set%MAXSEM] = cnt + 1;
        rt_kprintf("the producer generates a number: %d\n", array[set%MAXSEM]);
        set++;
        rt_sem_release(&sem_lock);

        /* ����һ����λ */
        rt_sem_release(&sem_full);
        cnt++;

        /* ��ͣһ��ʱ�� */
        rt_thread_delay(50);
    }

    rt_kprintf("the producer exit!\n");
}

/* �������߳���� */
void consumer_thread_entry(void* parameter)
{
    rt_uint32_t no;
    rt_uint32_t sum;

    /* ��n���̣߳�����ڲ��������� */
    no = (rt_uint32_t)parameter;

    sum = 0;
    while(1)
    {
        /* ��ȡһ����λ */
        rt_sem_take(&sem_full, RT_WAITING_FOREVER);

        /* �ٽ������������в��� */
        rt_sem_take(&sem_lock, RT_WAITING_FOREVER);
        sum += array[get%MAXSEM];
        rt_kprintf("the consumer[%d] get a number: %d\n", no, array[get%MAXSEM] );
        get++;
        rt_sem_release(&sem_lock);

        /* �ͷ�һ����λ */
        rt_sem_release(&sem_empty);

        /* ������������100����Ŀ��ֹͣ���������߳���Ӧֹͣ */
        if (get == 100) break;

        /* ��ͣһС��ʱ�� */
        rt_thread_delay(10);
    }

    rt_kprintf("the consumer[%d] sum is %d \n ", no, sum);
    rt_kprintf("the consumer[%d] exit!\n");
}

int semaphore_producer_consumer_init()
{
    /* ��ʼ��3���ź��� */
    rt_sem_init(&sem_lock , "lock",     1,      RT_IPC_FLAG_FIFO);
    rt_sem_init(&sem_empty, "empty",    MAXSEM, RT_IPC_FLAG_FIFO);
    rt_sem_init(&sem_full , "full",     0,      RT_IPC_FLAG_FIFO);

    /* �����߳�1 */
    producer_tid = rt_thread_create("producer",
                                    producer_thread_entry, RT_NULL, /* �߳������producer_thread_entry, ��ڲ�����RT_NULL */
                                    THREAD_STACK_SIZE, THREAD_PRIORITY - 1, THREAD_TIMESLICE);
    if (producer_tid != RT_NULL)
        rt_thread_startup(producer_tid);
    else
        tc_stat(TC_STAT_END | TC_STAT_FAILED);

    /* �����߳�2 */
    consumer_tid = rt_thread_create("consumer",
                                    consumer_thread_entry, RT_NULL, /* �߳������consumer_thread_entry, ��ڲ�����RT_NULL */
                                    THREAD_STACK_SIZE, THREAD_PRIORITY + 1, THREAD_TIMESLICE);
    if (consumer_tid != RT_NULL)
        rt_thread_startup(consumer_tid);
    else
        tc_stat(TC_STAT_END | TC_STAT_FAILED);

    return 0;
}

#ifdef RT_USING_TC
static void _tc_cleanup()
{
    /* �����������������󣬽������л��������̣߳�����Ӧ�ж� */
    rt_enter_critical();

    rt_sem_detach(&sem_lock);
    rt_sem_detach(&sem_empty);
    rt_sem_detach(&sem_full);

    /* ɾ���߳� */
    if (producer_tid != RT_NULL && producer_tid->stat != RT_THREAD_CLOSE)
        rt_thread_delete(producer_tid);
    if (consumer_tid != RT_NULL && consumer_tid->stat != RT_THREAD_CLOSE)
        rt_thread_delete(consumer_tid);

    /* ���������� */
    rt_exit_critical();

    /* ����TestCase״̬ */
    tc_done(TC_STAT_PASSED);
}

int _tc_semaphore_producer_consumer()
{
    /* ����TestCase����ص����� */
    tc_cleanup(_tc_cleanup);
    semaphore_producer_consumer_init();

    /* ����TestCase���е��ʱ�� */
    return 100;
}
/* ����������finsh shell�� */
FINSH_FUNCTION_EXPORT(_tc_semaphore_producer_consumer, producer and consumer example);
#else
/* �û�Ӧ����� */
int rt_application_init()
{
    semaphore_producer_consumer_init();

    return 0;
}
#endif
