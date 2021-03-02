/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:  (Pelican Mode)
 * Date           Author       Notes
 * 2018-01-06     sundm75       first version
 */

#ifndef __OPENLOONGSON_CAN_H
#define __OPENLOONGSON_CAN_H

#define CAN0               ( (CAN_TypeDef* )LS1C_REG_BASE_CAN0)
#define CAN1               ( (CAN_TypeDef* )LS1C_REG_BASE_CAN1)

#define CAN_InitStatus_Failed              ((unsigned char)0x00) /*!< CAN initialization failed */
#define CAN_InitStatus_Success             ((unsigned char)0x01) /*!< CAN initialization OK */

#define CAN_SJW_1tq                 ((unsigned char)0x00)  /*!< 1 time quantum */
#define CAN_SJW_2tq                 ((unsigned char)0x01)  /*!< 2 time quantum */
#define CAN_SJW_3tq                 ((unsigned char)0x02)  /*!< 3 time quantum */
#define CAN_SJW_4tq                 ((unsigned char)0x03)  /*!< 4 time quantum */

#define CAN_BS1_1tq                 ((unsigned char)0x00)  /*!< 1 time quantum */
#define CAN_BS1_2tq                 ((unsigned char)0x01)  /*!< 2 time quantum */
#define CAN_BS1_3tq                 ((unsigned char)0x02)  /*!< 3 time quantum */
#define CAN_BS1_4tq                 ((unsigned char)0x03)  /*!< 4 time quantum */
#define CAN_BS1_5tq                 ((unsigned char)0x04)  /*!< 5 time quantum */
#define CAN_BS1_6tq                 ((unsigned char)0x05)  /*!< 6 time quantum */
#define CAN_BS1_7tq                 ((unsigned char)0x06)  /*!< 7 time quantum */
#define CAN_BS1_8tq                 ((unsigned char)0x07)  /*!< 8 time quantum */
#define CAN_BS1_9tq                 ((unsigned char)0x08)  /*!< 9 time quantum */
#define CAN_BS1_10tq                ((unsigned char)0x09)  /*!< 10 time quantum */
#define CAN_BS1_11tq                ((unsigned char)0x0A)  /*!< 11 time quantum */
#define CAN_BS1_12tq                ((unsigned char)0x0B)  /*!< 12 time quantum */
#define CAN_BS1_13tq                ((unsigned char)0x0C)  /*!< 13 time quantum */
#define CAN_BS1_14tq                ((unsigned char)0x0D)  /*!< 14 time quantum */
#define CAN_BS1_15tq                ((unsigned char)0x0E)  /*!< 15 time quantum */
#define CAN_BS1_16tq                ((unsigned char)0x0F)  /*!< 16 time quantum */

#define CAN_BS2_1tq                 ((unsigned char)0x00)  /*!< 1 time quantum */
#define CAN_BS2_2tq                 ((unsigned char)0x01)  /*!< 2 time quantum */
#define CAN_BS2_3tq                 ((unsigned char)0x02)  /*!< 3 time quantum */
#define CAN_BS2_4tq                 ((unsigned char)0x03)  /*!< 4 time quantum */
#define CAN_BS2_5tq                 ((unsigned char)0x04)  /*!< 5 time quantum */
#define CAN_BS2_6tq                 ((unsigned char)0x05)  /*!< 6 time quantum */
#define CAN_BS2_7tq                 ((unsigned char)0x06)  /*!< 7 time quantum */
#define CAN_BS2_8tq                 ((unsigned char)0x07)  /*!< 8 time quantum */

#define CAN_Id_Standard           0
#define CAN_Id_Extended           1
#define CAN_RTR_DATA               0
#define CAN_RTR_Remote            1

#define CAN_MODE_NORMAL              0
#define CAN_MODE_LISEN               1
#define CAN_MODE_LOOPBACK            2
#define CAN_MODE_LOOPBACKANLISEN     3

/*!< CAN ����״̬�Ĵ��� */
/************************** CAN_MOD �Ĵ���λ����*******************************/
#define CAN_Mode_RM                 ((unsigned char)0x01)  /*!< ��λģʽ */
#define CAN_Mode_LOM                ((unsigned char)0x02)  /*!< ֻ��ģʽ 1:ֻ��  0:����  */
#define CAN_Mode_STM                ((unsigned char)0x04)  /*!< ��������ģʽ1:�Լ��  0:����  */
#define CAN_Mode_AFM                ((unsigned char)0x08)  /*!< ��/˫�˲�ģʽ 1:�� 0: ˫*/
#define CAN_Mode_SM                 ((unsigned char)0x10)  /*!< ˯��ģʽ1: ˯�� 0: ���� */

/************************** CAN_CMR �Ĵ���λ����*******************************/
 #define  CAN_CMR_TR                         ((unsigned char)0x01)         /*!< �������� 1: ��ǰ��Ϣ������  0: �� */
 #define  CAN_CMR_AT                         ((unsigned char)0x02)         /*!< ��ֹ���� 1: �ȴ����͵���Ϣȡ��  0: ��ȱ  */
 #define  CAN_CMR_RRB                        ((unsigned char)0x04)         /*!< �ͷŽ��ջ�����  1:�ͷ�  0: �޶��� */
 #define  CAN_CMR_CDO                        ((unsigned char)0x08)         /*!< ����������  1:���  0: �޶���    */
//#define  CAN_CMR_GTS                        ((unsigned char)0x10)        /*!< STDģʽ< ˯��: 1:����˯��  0: ����  */
 #define  CAN_CMR_SRR                        ((unsigned char)0x10)         /*!< �Խ�������  1:  0:   */
 #define  CAN_CMR_EFF                        ((unsigned char)0x80)         /*!< ��չģʽ 1:��չ֡ 0: ��׼֡  */

/************************** CAN_SR �Ĵ���λ����********************************/
 #define  CAN_SR_BBS                         ((unsigned char)0x01)         /*!< ���ջ�����״̬1: ��  0: �� */
 #define  CAN_SR_DOS                         ((unsigned char)0x02)         /*!< �������״̬ 1: ���  0: ��ȱ  */
 #define  CAN_SR_TBS                         ((unsigned char)0x04)         /*!< ���ͻ�����״̬1: �ͷ�  0: ���� */
 #define  CAN_SR_TCS                         ((unsigned char)0x08)         /*!< �������״̬1: ���  0: δ���    */
 #define  CAN_SR_RS                          ((unsigned char)0x10)         /*!< ����״̬1: ����  0: ����  */
 #define  CAN_SR_TS                          ((unsigned char)0x20)         /*!< ����״̬1:  ���� 0:  ����*/
 #define  CAN_SR_ES                          ((unsigned char)0x40)         /*!< ����״̬1:���� 0: ���� */
 #define  CAN_SR_BS                          ((unsigned char)0x80)         /*!< ����״̬1: �ر�  0: ����  */

/************************** CAN_IR �жϼĴ���λ����****************************/
 #define  CAN_IR_RI                          ((unsigned char)0x01)         /*!< �����ж� */
 #define  CAN_IR_TI                          ((unsigned char)0x02)         /*!< �����ж� */
 #define  CAN_IR_EI                          ((unsigned char)0x04)         /*!< �����ж� */
 #define  CAN_IR_DOI                         ((unsigned char)0x08)         /*!< ��������ж�  */
 #define  CAN_IR_WUI                         ((unsigned char)0x10)         /*!< �����ж� */
 #define  CAN_IR_EPI                         ((unsigned char)0x20)         /*!< ���������ж� */
 #define  CAN_IR_ALI                         ((unsigned char)0x40)         /*!< �ٲö�ʧ�ж� */
 #define  CAN_IR_BEI                         ((unsigned char)0x80)         /*!< ���ߴ����ж�  */

/************************* CAN_IER �ж�ʹ�ܼĴ���λ����************************/
 #define  CAN_IER_RIE                         ((unsigned char)0x01)        /*!< �����ж�ʹ�� */
 #define  CAN_IER_TIE                         ((unsigned char)0x02)        /*!< �����ж�ʹ�� */
 #define  CAN_IER_EIE                         ((unsigned char)0x04)        /*!< �����ж�ʹ�� */
 #define  CAN_IER_DOIE                        ((unsigned char)0x08)        /*!< ��������ж�ʹ��  */
 #define  CAN_IER_WUIE                        ((unsigned char)0x10)        /*!< �����ж�ʹ�� */
 #define  CAN_IER_EPIE                        ((unsigned char)0x20)        /*!< ���������ж�ʹ�� */
 #define  CAN_IER_ALIE                        ((unsigned char)0x40)        /*!< �ٲö�ʧ�ж�ʹ�� */
 #define  CAN_IER_BEIE                        ((unsigned char)0x80)        /*!< ���ߴ����ж�ʹ��  */

typedef enum
{
    LS1C_CAN1MBaud=0,    // 1 MBit/sec
    LS1C_CAN800kBaud,    // 800 kBit/sec
    LS1C_CAN500kBaud,    // 500 kBit/sec
    LS1C_CAN250kBaud,    // 250 kBit/sec
    LS1C_CAN125kBaud,    // 125 kBit/sec
    LS1C_CAN100kBaud,    // 100 kBit/sec
    LS1C_CAN50kBaud,     // 50 kBit/sec
    LS1C_CAN40kBaud,     // 40 kBit/sec
}Ls1c_CanBPS_t;

typedef struct
{
   unsigned char MOD;
   unsigned char CMR;
   unsigned char SR;
   unsigned char IR;
   unsigned char IER;
   unsigned char reserved0;
   unsigned char BTR0;
   unsigned char BTR1;
   unsigned char OCR;
   unsigned char reserved[2];
   unsigned char ALC;
   unsigned char ECC ;
   unsigned char EMLR;
   unsigned char RXERR;
   unsigned char TXERR;
   unsigned char IDE_RTR_DLC;
   unsigned char ID[4];
   unsigned char BUF[8];
   unsigned char RMCR;
   unsigned char CDR;
} CAN_TypeDef;

typedef struct
{
  unsigned char  CAN_Prescaler;    /* �����ʷ�Ƶϵ��1 to 31. */
  unsigned char  CAN_Mode;         /*0x10:˯��0x08:��,˫�˲� 0x40:��������0x20:ֻ�� 0x01:��λ*/
  unsigned char  CAN_SJW;          /*ͬ����ת��� */
  unsigned char  CAN_BS1;          /*ʱ���1����ֵ*/
  unsigned char  CAN_BS2;          /*ʱ���2����ֵ*/

} CAN_InitTypeDef;

typedef struct
{
  unsigned char  IDE;        /*0: ʹ�ñ�׼��ʶ��1: ʹ����չ��ʶ��*/
  unsigned char  RTR;    /*0: ����֡     1: Զ��֡*/
  unsigned char  MODE;        /* 0- ˫�˲���ģʽ;1-���˲���ģʽ*/
  unsigned long  First_Data;    /*˫�˲���ģʽ����Ϣ��һ�������ֽ�*/
  unsigned long  Data_Mask;    /*˫�˲���ģʽ����Ϣ��һ�������ֽ�����*/
  unsigned long  ID;        /*���մ���*/
  /*
 ˫�˲���-  ��չ֡: 2���˲�����ǰ16λ,�ֱ����ID ��ǰ16λ�� ID�ĺ�16λ.
 ˫�˲���-  ��׼֡: 2���˲�����11λ,�ֱ����ID ��ǰ16λ�� ID�ĺ�16λ,��1���˲���ͬʱʹ��First_Data��Data_Mask
 ���˲���-  ��չ֡: ʹ��29λ, ����ID �ĺ�29λ.
 ���˲���-  ��׼֡: ʹ��11λ, ����ID �ĺ�11λ.
  */
  unsigned long  IDMASK;    /*��������*/
} CAN_FilterInitTypeDef;

typedef struct
{
  unsigned long StdId;  /* 11λID*/
  unsigned long ExtId;  /*29λID**/
  unsigned char IDE;    /*IDE: ��ʶ��ѡ��
                                     ��λ�������������б���ʹ�õı�ʶ������
                                     0: ʹ�ñ�׼��ʶ��
                                     1: ʹ����չ��ʶ��*/
  unsigned char RTR;     /*Զ�̷�������
                                       0: ����֡
                                       1: Զ��֡*/
  unsigned char DLC;     /*����֡����*/
  unsigned char Data[8]; /*8�ֽ�����*/
} CanRxMsg;

typedef struct
{
  unsigned long StdId;  /* 11λID*/
  unsigned long ExtId;  /*29λID**/
  unsigned char IDE;    /*IDE: ��ʶ��ѡ��
                                     ��λ�������������б���ʹ�õı�ʶ������
                                     0: ʹ�ñ�׼��ʶ��
                                     1: ʹ����չ��ʶ��*/
  unsigned char RTR;     /*Զ�̷�������
                                       0: ����֡
                                       1: Զ��֡*/
  unsigned char DLC;     /*����֡����*/
  unsigned char Data[8]; /*8�ֽ�����*/
} CanTxMsg;

unsigned char CAN_Init(CAN_TypeDef* CANx, CAN_InitTypeDef* CAN_InitStruct);
unsigned char CAN_SetBps(CAN_TypeDef* CANx, Ls1c_CanBPS_t  Bps);
unsigned char CAN_SetMode(CAN_TypeDef* CANx, unsigned char  mode);
void CAN_FilterInit(CAN_TypeDef* CANx,  CAN_FilterInitTypeDef* CAN_FilterInitStruct);
unsigned char CAN_Transmit(CAN_TypeDef* CANx, CanTxMsg* TxMessage);
void CAN_Receive(CAN_TypeDef* CANx,  CanRxMsg* RxMessage);

unsigned char  set_reset_mode(CAN_TypeDef* CANx);
unsigned char  set_start(CAN_TypeDef* CANx);

#endif

