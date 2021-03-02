/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-05-06     sundm75       first version
 */

#ifndef _LS1C_WDOG_H_
#define _LS1C_WDOG_H_

#ifdef __cplusplus
extern "C" {
#endif

unsigned int Wdog_Init(void);                   // ��ʱΪ��
unsigned int Wdog_Enable(void);                 // ���Ź�ʹ�ܼĴ�����WDT_EN��
unsigned int Wdog_Disable(void);                // ���Ź�ʧ�ܼĴ�����WDT_EN��
unsigned int Wdog_Set(void);                    // ���Ź����üĴ��� (WDT_SET)
unsigned int Wdog_Reset(void);                  // ���Ź����üĴ��� (WDT_SET)
unsigned int Wdog_GetValue(void);               // ��ÿ��Ź���������WDT_timer��
unsigned int Wdog_LoadValue(unsigned int cnt);  // ���ÿ��Ź���������WDT_timer��
unsigned int Wdog_GetPreValue(void);            // ��ÿ��Ź��������趨ֵ

#ifdef __cplusplus
}
#endif

#endif /* _LS1C_WDOG_H_ */

