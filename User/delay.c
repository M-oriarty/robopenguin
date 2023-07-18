#include "delay.h"

/*
*********************************************************************************************************
*   �� �� ��: bsp_DelayMS
*   ����˵��: ms���ӳ٣��ӳپ���Ϊ����1ms
*   ��    �Σ�n : �ӳٳ��ȣ���λ1 ms�� n Ӧ����2
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void Delayms(uint32_t n)
{
    uint32_t ticks;
    uint32_t told,tnow,tcnt=0;
    uint32_t reload = SysTick->CMP; /* LOAD��ֵ */

    //ticks = n * 168000;                /* ��Ҫ�Ľ����� */
    /*  the systick has been set to be 8-div by RT  */
    ticks = n * reload;                  /* ��Ҫ�Ľ����� */
    tcnt = 0;
    told = SysTick->CNT;             /* �ս���ʱ�ļ�����ֵ */


    while(1)
    {
        tnow = SysTick->CNT;
        if(tnow != told)
        {
            /* ����ע��һ��SYSTICK��һ���ݼ��ļ����� */
            if(tnow < told)
            {
                tcnt += told - tnow;
            }
            /* ����װ�صݼ� */
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;

            /*ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳� */
            if(tcnt >= ticks)break;
        }
    }

}

/*
*********************************************************************************************************
*   �� �� ��: bsp_DelayUS
*   ����˵��: us���ӳ٣��ӳپ���Ϊ����1ms
*   ��    �Σ�n : �ӳٳ��ȣ���λ1 us
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void Delayus(uint32_t n)
{
    uint32_t ticks;
    uint32_t told,tnow,tcnt=0;
    uint32_t reload = SysTick->CMP; /* LOAD��ֵ */

    //ticks = n * 168;               /* ��Ҫ�Ľ����� */
    ticks = n * reload/1000;                 /* ��Ҫ�Ľ����� */
    told = SysTick->CNT;             /* �ս���ʱ�ļ�����ֵ */

    while(1)
    {
        tnow = SysTick->CNT;
        if(tnow != told)
        {
            /* ����ע��һ��SYSTICK��һ���ݼ��ļ����� */
            if(tnow < told)
            {
                tcnt += told - tnow;
            }
            /* ����װ�صݼ� */
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;

            /*ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳� */
            if(tcnt >= ticks)break;
        }
    }
}
