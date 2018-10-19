/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2006-08-31     Bernard      first implementation
 * 2018-03-01     flyingcys    add realtek ameba
 */

#include <rthw.h>
#include <rtthread.h>

/**
 * @addtogroup ameba
 */

/*@{*/

int main(void)
{
    rt_kprintf("build time: %s %s\n", __DATE__, __TIME__);
    rt_kprintf("Hello RT-Thread!\n");
    
    return 0;
}

/*@}*/
