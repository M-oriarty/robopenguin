/*
 * JY_901.c
 *
 *  Created on: 2023年5月7日
 *      Author: 27220
 */

#include "JY_901.h"

#define JY_901_THREAD_PRIORITY         25
#define JY_901_THREAD_STACK_SIZE       512

/*　使用静态线程时，线程的栈需要设置字节对齐　*/
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t jy_901_thread_stack[JY_901_THREAD_STACK_SIZE];
static struct rt_thread jy_901_thread;

static void jy_901_entry(void* parameter)
{
    while(1)
    {
            printf("angle:%f %f %f\n",JY901_data.angle.angle[0],JY901_data.angle.angle[1],JY901_data.angle.angle[2]);

            rt_thread_mdelay(500);
    }

}

void jy_901_thread_create(void)
{

    JY_901_Buffer_Init(&JY901_data);

    rt_thread_init(&jy_901_thread,
                                    "_jy_901",
                                    jy_901_entry,
                                    RT_NULL,
                                    jy_901_thread_stack,
                                    JY_901_THREAD_STACK_SIZE,
                                    JY_901_THREAD_PRIORITY,30);

    rt_thread_startup(&jy_901_thread);

}
