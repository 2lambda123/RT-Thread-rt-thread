/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 */
#ifndef _SDMMC_SDCARD_H
#define _SDMMC_SDCARD_H
#include <rtthread.h>
#include <board.h>
#include "stm32f4xx.h"
#include <rtdevice.h>


#define SD_TIMEOUT ((uint32_t)100000000)            //��ʱʱ��

#define SD_DMA_MODE    		0	//1��DMAģʽ��0����ѯģʽ   

extern SD_HandleTypeDef        SDCARD_Handler;     //SD�����
extern HAL_SD_CardInfoTypedef  SDCardInfo;         //SD����Ϣ�ṹ��

rt_uint8_t SD_Init(void);
rt_uint8_t SD_GetCardInfo(HAL_SD_CardInfoTypedef *cardinfo);

#endif
