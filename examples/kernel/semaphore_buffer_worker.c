/*
 * �����嵥���ź���ʵ�������������߼�Ļ���
 *
 * ����������У��ᴴ�������̣߳�һ�����������߳�workerһ�����������߳�thread
 *
 * ��������Ϣ���������ѵĹ����У�worker��������ݽ�д�뵽����buffer�У���thread
 * ��ӻ���buffer�ж�����
 */
#include <rtthread.h>
#include "tc_comm.h"

/* һ������buffer��ʵ�� */
struct rb
{
    rt_uint16_t read_index, write_index;
    rt_uint8_t *buffer_ptr;
    rt_uint16_t buffer_size;
};

/* ָ���ź������ƿ��ָ�� */
static rt_sem_t sem = RT_NULL;
/* ָ���߳̿��ƿ��ָ�� */
static rt_thread_t tid = RT_NULL, worker = RT_NULL;

/* ����buffer���ڴ�飨���������ֳ����� */
#define BUFFER_SIZE        256
#define BUFFER_ITEM        32
static rt_uint8_t working_buffer[BUFFER_SIZE];
struct rb working_rb;

/* ��ʼ������buffer��sizeָ����buffer�Ĵ�С��ע�����ﲢû�����ݵ�ַ���������� */
static void rb_init(struct rb* rb, rt_uint8_t *pool, rt_uint16_t size)
{
    RT_ASSERT(rb != RT_NULL);

    /* �Զ�дָ������*/
    rb->read_index = rb->write_index = 0;

    /* ���û���buffer���ڴ����ݿ� */
    rb->buffer_ptr = pool;
    rb->buffer_size = size;
}

/* ����buffer��д������ */
static rt_bool_t rb_put(struct rb* rb, const rt_uint8_t *ptr, rt_uint16_t length)
{
    rt_size_t size;

    /* �ж��Ƿ����㹻��ʣ��ռ� */
    if (rb->read_index > rb->write_index)
        size = rb->read_index - rb->write_index;
    else
        size = rb->buffer_size - rb->write_index + rb->read_index;

    /* û�ж���Ŀռ� */
    if (size < length) return RT_FALSE;

    if (rb->read_index > rb->write_index)
    {
        /* read_index - write_index ��Ϊ�ܵĿ���ռ� */
        memcpy(&rb->buffer_ptr[rb->write_index], ptr, length);
        rb->write_index += length;
    }
    else
    {
        if (rb->buffer_size - rb->write_index > length)
        {
            /* write_index ����ʣ��Ŀռ����㹻�ĳ��� */
            memcpy(&rb->buffer_ptr[rb->write_index], ptr, length);
            rb->write_index += length;
        }
        else
        {
            /*
             * write_index ����ʣ��Ŀռ䲻�����㹻�ĳ��ȣ���Ҫ�Ѳ������ݸ��Ƶ�
             * ǰ���ʣ��ռ���
             */
            memcpy(&rb->buffer_ptr[rb->write_index], ptr,
                   rb->buffer_size - rb->write_index);
            memcpy(&rb->buffer_ptr[0], &ptr[rb->buffer_size - rb->write_index],
                   length - (rb->buffer_size - rb->write_index));
            rb->write_index = length - (rb->buffer_size - rb->write_index);
        }
    }

    return RT_TRUE;
}

/* �ӻ���buffer�ж������� */
static rt_bool_t rb_get(struct rb* rb, rt_uint8_t *ptr, rt_uint16_t length)
{
    rt_size_t size;

    /* �ж��Ƿ����㹻������ */
    if (rb->read_index > rb->write_index)
        size = rb->buffer_size - rb->read_index + rb->write_index;
    else
        size = rb->write_index - rb->read_index;

    /* û���㹻������ */
    if (size < length) return RT_FALSE;

    if (rb->read_index > rb->write_index)
    {
        if (rb->buffer_size - rb->read_index > length)
        {
            /* read_index�������㹻�ֱ࣬�Ӹ��� */
            memcpy(ptr, &rb->buffer_ptr[rb->read_index], length);
            rb->read_index += length;
        }
        else
        {
            /* read_index�����ݲ�������Ҫ�ֶθ��� */
            memcpy(ptr, &rb->buffer_ptr[rb->read_index],
                   rb->buffer_size - rb->read_index);
            memcpy(&ptr[rb->buffer_size - rb->read_index], &rb->buffer_ptr[0],
                   length - rb->buffer_size + rb->read_index);
            rb->read_index = length - rb->buffer_size + rb->read_index;
        }
    }
    else
    {
        /*
         * read_indexҪ��write_indexС���ܵ�����������ǰ���Ѿ���������������
         * �ϣ���ֱ�Ӹ��Ƴ����ݡ�
         */
        memcpy(ptr, &rb->buffer_ptr[rb->read_index], length);
        rb->read_index += length;
    }

    return RT_TRUE;
}

/* �������߳���� */
static void thread_entry(void* parameter)
{
    rt_bool_t result;
    rt_uint8_t data_buffer[BUFFER_ITEM + 1];

    while (1)
    {
        /* �����ź��� */
        rt_sem_take(sem, RT_WAITING_FOREVER);
        /* �ӻ�buffer�л������ */
        result = rb_get(&working_rb, &data_buffer[0], BUFFER_ITEM);
        /* �ͷ��ź��� */
        rt_sem_release(sem);
        data_buffer[BUFFER_ITEM] = '\0';

        if (result == RT_TRUE)
        {
            /* ��ȡ���ݳɹ�����ӡ���� */
            rt_kprintf("%s\n", data_buffer);
        }

        /* ��һ��5 OS Tick������ */
        rt_thread_delay(5);
    }
}

/* worker�߳���� */
static void worker_entry(void* parameter)
{
    rt_bool_t result;
    rt_uint32_t index, setchar;
    rt_uint8_t  data_buffer[BUFFER_ITEM];

    setchar = 0x21;
    while (1)
    {
        /* �������� */
        for(index = 0; index < BUFFER_ITEM; index++)
        {
            data_buffer[index] = setchar;
            if (++setchar == 0x7f)
                setchar = 0x21;
        }

        /* �����ź��� */
        rt_sem_take(sem, RT_WAITING_FOREVER);

        /* �����ݷŵ�����buffer�� */
        result = rb_put(&working_rb, &data_buffer[0], BUFFER_ITEM);
        if (result == RT_FALSE)
        {
            rt_kprintf("put error\n");
        }

        /* �ͷ��ź��� */
        rt_sem_release(sem);

        /* ����ɹ�����һ��10 OS Tick������ */
        rt_thread_delay(10);
    }
}

int semaphore_buffer_worker_init()
{
    /* ��ʼ��ring buffer */
    rb_init(&working_rb, working_buffer, BUFFER_SIZE);

    /* �����ź��� */
    sem = rt_sem_create("sem", 1, RT_IPC_FLAG_FIFO);
    if (sem == RT_NULL)
    {
        tc_stat(TC_STAT_END | TC_STAT_FAILED);
        return 0;
    }

    /* �����߳�1 */
    tid = rt_thread_create("thread",
                           thread_entry, RT_NULL, /* �߳������thread_entry, ��ڲ�����RT_NULL */
                           THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid != RT_NULL)
        rt_thread_startup(tid);
    else
        tc_stat(TC_STAT_END | TC_STAT_FAILED);

    /* �����߳�2 */
    worker = rt_thread_create("worker",
                              worker_entry, RT_NULL, /* �߳������worker_entry, ��ڲ�����RT_NULL */
                              THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if (worker != RT_NULL)
        rt_thread_startup(worker);
    else
        tc_stat(TC_STAT_END | TC_STAT_FAILED);

    return 0;
}

#ifdef RT_USING_TC
static void _tc_cleanup()
{
    /* �����������������󣬽������л��������̣߳�����Ӧ�ж� */
    rt_enter_critical();

    /* ɾ���ź��� */
    if (sem != RT_NULL)
        rt_sem_delete(sem);

    /* ɾ���߳� */
    if (tid != RT_NULL && tid->stat != RT_THREAD_CLOSE)
        rt_thread_delete(tid);
    if (worker != RT_NULL && worker->stat != RT_THREAD_CLOSE)
        rt_thread_delete(worker);

    /* ���������� */
    rt_exit_critical();

    /* ����TestCase״̬ */
    tc_done(TC_STAT_PASSED);
}

int _tc_semaphore_buffer_worker()
{
    /* ����TestCase����ص����� */
    tc_cleanup(_tc_cleanup);
    semaphore_buffer_worker_init();

    /* ����TestCase���е��ʱ�� */
    return 100;
}
/* ����������finsh shell�� */
FINSH_FUNCTION_EXPORT(_tc_semaphore_buffer_worker, a buffer worker with semaphore example);
#else
/* �û�Ӧ����� */
int rt_application_init()
{
    semaphore_buffer_worker_init();

    return 0;
}
#endif
