
#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <rtthread.h>

typedef struct RxCon
{
    rt_uint32_t recsize;
    char* recBuf;

} RxControl_Type;

typedef struct TxCon
{
    rt_uint32_t recsize;
    char* trnBuf;

} TxControl_Type;

typedef struct sysPram
{
    rt_uint8_t data[4];
    rt_uint8_t times[3];
    float lat;//γ��
    float lon;//����
    float speed;//�ٶ�
} sysPram_Type;

extern sysPram_Type sysPrams;
#endif


