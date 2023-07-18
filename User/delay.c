#include "delay.h"

/*
*********************************************************************************************************
*   函 数 名: bsp_DelayMS
*   功能说明: ms级延迟，延迟精度为正负1ms
*   形    参：n : 延迟长度，单位1 ms。 n 应大于2
*   返 回 值: 无
*********************************************************************************************************
*/
void Delayms(uint32_t n)
{
    uint32_t ticks;
    uint32_t told,tnow,tcnt=0;
    uint32_t reload = SysTick->CMP; /* LOAD的值 */

    //ticks = n * 168000;                /* 需要的节拍数 */
    /*  the systick has been set to be 8-div by RT  */
    ticks = n * reload;                  /* 需要的节拍数 */
    tcnt = 0;
    told = SysTick->CNT;             /* 刚进入时的计数器值 */


    while(1)
    {
        tnow = SysTick->CNT;
        if(tnow != told)
        {
            /* 这里注意一下SYSTICK是一个递减的计数器 */
            if(tnow < told)
            {
                tcnt += told - tnow;
            }
            /* 重新装载递减 */
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;

            /*时间超过/等于要延迟的时间,则退出 */
            if(tcnt >= ticks)break;
        }
    }

}

/*
*********************************************************************************************************
*   函 数 名: bsp_DelayUS
*   功能说明: us级延迟，延迟精度为正负1ms
*   形    参：n : 延迟长度，单位1 us
*   返 回 值: 无
*********************************************************************************************************
*/
void Delayus(uint32_t n)
{
    uint32_t ticks;
    uint32_t told,tnow,tcnt=0;
    uint32_t reload = SysTick->CMP; /* LOAD的值 */

    //ticks = n * 168;               /* 需要的节拍数 */
    ticks = n * reload/1000;                 /* 需要的节拍数 */
    told = SysTick->CNT;             /* 刚进入时的计数器值 */

    while(1)
    {
        tnow = SysTick->CNT;
        if(tnow != told)
        {
            /* 这里注意一下SYSTICK是一个递减的计数器 */
            if(tnow < told)
            {
                tcnt += told - tnow;
            }
            /* 重新装载递减 */
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;

            /*时间超过/等于要延迟的时间,则退出 */
            if(tcnt >= ticks)break;
        }
    }
}
