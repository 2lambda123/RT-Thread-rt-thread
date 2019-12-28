#include <modbus.h>

#include "app_modbus_rtu_slave.h"

#define DBG_SECTION_NAME    "app_modbus_rtu_slave"
#define DBG_LEVEL           DBG_LOG
#include <rtdbg.h>

static void __modbus_rtu_slave_init(void);
static void __modbus_rtu_slave_thread(void *para);

static void __modbus_rtu_slave_init(void) {
    rt_thread_t tid;
    
    //����modbus_rtu_slave�߳�
    tid = rt_thread_create("modbus_rtu_slace",
                           __modbus_rtu_slave_thread,
                           RT_NULL,
                           2048,
                           1,
                           10);
    /* �����ɹ��������߳� */
    if (tid != RT_NULL)
        rt_thread_startup(tid);
    else
        LOG_E("create modbus_rtu_slace thread failed.");
}

static void __modbus_rtu_slave_thread(void *para) {
    modbus_t *ctx = NULL;
    modbus_mapping_t *mb_mapping = NULL;
    int rc;
    int use_backend;
    //��ʼ��modbus rtu
    ctx = modbus_new_rtu("/dev/uart3", 115200, 'N', 8, 1);
    
    //�趨���豸��ַ
    modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS232);
    modbus_set_debug(ctx, 1);
    modbus_set_slave(ctx, 1);
    //modbus����
    modbus_connect(ctx);
    //�Ĵ���map��ʼ��
    mb_mapping = modbus_mapping_new(0, 0,
                                    MODBUS_MAX_READ_REGISTERS, MODBUS_MAX_WRITE_REGISTERS);
    if (mb_mapping == NULL) {
        rt_kprintf("Failed to allocate the mapping: %s\n",
                   modbus_strerror(errno));
        modbus_free(ctx);
        return;
    }
 
   //��ʼ�����Ĵ���
   p_data_rtu = (struct data_rtu *)mb_mapping->tab_registers;
//   mb_mapping->tab_registers[0] = 1;
//   mb_mapping->tab_registers[1] = 2;
//   mb_mapping->tab_registers[2] = 3;
//   mb_mapping->tab_registers[3] = 4;
//   mb_mapping->tab_registers[4] = 5;
//   mb_mapping->tab_registers[5] = 6;
//   mb_mapping->tab_registers[6] = 7;
//   mb_mapping->tab_registers[7] = 8;
//   mb_mapping->tab_registers[8] = 9;
//   mb_mapping->tab_registers[9] = 10;
//    mb_mapping->tab_input_registers[0] = 0x33;
   //ѭ��
   while( 1 ){
        uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
        //��ѯ�������ݣ�������Ӧ����
        rc = modbus_receive(ctx, query);
        if (rc > 0) {
            modbus_reply(ctx, query, rc, mb_mapping);
        } else if (rc  == -1) {
            /* Connection closed by the client or error */
            rt_kprintf("Quit the loop: %s\n", modbus_strerror(errno));
        }
    }
}
INIT_APP_EXPORT(__modbus_rtu_slave_thread);
