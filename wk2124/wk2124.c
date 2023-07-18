#include "wk2124.h"



#define WK2124_RST1_GPIO_Port GPIOC
#define WK2124_RST1_Pin GPIO_Pin_0

#define WK2124_CS1_GPIO_Port GPIOB
#define WK2124_CS1_Pin GPIO_Pin_12

/// HAL库SPI句柄

// 拉低RST引脚
#define WK_GPIO_RST_L()    GPIO_ResetBits(WK2124_RST1_GPIO_Port, WK2124_RST1_Pin)

// 拉高RST引脚
#define WK_GPIO_RST_H()    GPIO_SetBits(WK2124_RST1_GPIO_Port, WK2124_RST1_Pin)

// 片选引脚拉低
#define WK_GPIO_SEL_L()    GPIO_ResetBits(WK2124_CS1_GPIO_Port, WK2124_CS1_Pin)

// 片选引脚拉高
#define WK_GPIO_SEL_H()    GPIO_SetBits(WK2124_CS1_GPIO_Port, WK2124_CS1_Pin)


static void wk_delay_ms(uint32_t nms)
{
        HAL_Delay(nms);
}

static uint8_t WK_SPI_ReadWriteByte(uint8_t TxData)
{
        uint8_t RxData;
        RxData = SPI_ReadWriteByte(TxData);
        return RxData;
}




//
///
///                            驱动程序代码
///
//



///// 硬件复位（拉低复位引脚最低10毫秒进行复位）
//void WK_Rst(void)
//{
//        WK_GPIO_RST_L();
//        wk_delay_ms(50);
//        WK_GPIO_RST_H();
//        wk_delay_ms(50);
//}

void WK_WriteGReg(uint8_t reg, uint8_t value)
{
        WK_GPIO_SEL_L();
        WK_SPI_ReadWriteByte(reg);
        WK_SPI_ReadWriteByte(value);
        WK_GPIO_SEL_H();
}

uint8_t WK_ReadGReg(uint8_t reg)
{
        uint8_t value = 0X00;
        WK_GPIO_SEL_L();
        WK_SPI_ReadWriteByte(0X40 | reg);
        value = WK_SPI_ReadWriteByte(0X00);
        WK_GPIO_SEL_H();
        return value;
}

void WK_WriteSReg(uint8_t port, uint8_t reg, uint8_t value)
{
        WK_GPIO_SEL_L();
        WK_SPI_ReadWriteByte(((port - 1) << 4) | reg);
        WK_SPI_ReadWriteByte(value);
        WK_GPIO_SEL_H();
}

uint8_t WK_ReadSReg(uint8_t port, uint8_t reg)
{
        uint8_t value = 0X00;
        WK_GPIO_SEL_L();
        int state = WK_SPI_ReadWriteByte(0X40 | ((port - 1) << 4) | reg);
                printf("reg %d state %d\n",reg,state);
        value = WK_SPI_ReadWriteByte(0X00);
                printf("reg %d value %d\n",reg,value);
        WK_GPIO_SEL_H();
        return value;
}

void WK_WriteFIFO(uint8_t port, const void* buf, uint16_t len)
{
        const uint8_t* p = (const uint8_t *)buf;
        WK_GPIO_SEL_L();
        int state = WK_SPI_ReadWriteByte(0X80 | ((port - 1) << 4));
        printf("write fifo state %d\n",state);
                while (len--)
        {
                WK_SPI_ReadWriteByte(*p++);
        }
        WK_GPIO_SEL_H();
}

void WK_ReadFIFO(uint8_t port, void* buf, uint16_t len)
{
        uint8_t* p = (uint8_t *)buf;
        WK_GPIO_SEL_L();
        WK_SPI_ReadWriteByte(0XC0 | ((port - 1) << 4));
        while (len--)
        {
                *p++ = WK_SPI_ReadWriteByte(0X00);
        }
        WK_GPIO_SEL_H();
}

//void WK_PortCalcBaudrate(uint32_t fosc, uint32_t baudrate, uint8_t* BAUD0, uint8_t* BAUD1, uint8_t* PRES)
//{
//        float value = (float)fosc / (float)(baudrate << 4);
//        *BAUD0 = (((uint32_t)value) - 1) & 0XFF;
//        *BAUD1 = ((((uint32_t)value) - 1) >> 8) & 0XFF;
//        *PRES  = ((uint32_t)(value * 10)) % 10;
//}

uint16_t WK_PortSendData(uint8_t port, const void* buf, uint16_t len)
{
        uint8_t  state = 0;
        uint16_t nsend = 0;
        state = WK_ReadSReg(port, WK2XXX_FSR);
                //printf("tx FSR %d\n",state);
        if (state & WK2XXX_TFULL)                    // 发送FIFO满
        {
                nsend = 0;
        } else
        {
                state = WK_ReadSReg(port, WK2XXX_TFCNT); // 读取发送FIFO已用空间
                                //printf("tx TFCNT %d\n",state);
                nsend = 256 - state;
                nsend = nsend >= len ? len : nsend;
                                //printf("nsend %d\n",nsend);
                WK_WriteFIFO(port, buf, nsend);
        }
                while(WK_ReadSReg(port, WK2XXX_TFCNT)!=0){

                }
        return nsend;                                // 返回实际发送成功的数据量
}

uint16_t WK_PortRecvData(uint8_t port, void* buf, uint16_t len)
{
        uint8_t state = 0;
        uint8_t nread = 0;
                int sifr = WK_ReadSReg(port,WK2XXX_SIFR);
                printf("sifr %d\n",sifr);
//              int count=0;
//        while(WK_ReadSReg(port, WK2XXX_FSR)==0){
                    state = WK_ReadSReg(port, WK2XXX_FSR);
//                  count++;
//              }
//              printf("count %d\n",count);
//              count = 0;
                printf("rx FSR %d\n",state);
//        if (state & WK2XXX_RDAT)                      // 接收FIFO非空
//        {
                nread = WK_ReadSReg(port, WK2XXX_RFCNT);  // 查询FIFO中的数据量
                                printf("rx RFCNT %d\n",nread);
                nread = len >= nread ? nread : len;
                WK_ReadFIFO(port, buf, nread);            // 读取FIFO中的数据
//        }
        return nread;                                 // 返回实际读取到的数据量
}

void WK_setBaud(uint8_t port, uint32_t baudrate){
    unsigned char baud1,baud0,pres;
    if(baudrate == 4800){
        baud1=0x00;
        baud0=0x8f;
        pres=0;
    }else if(baudrate == 9600){
        baud1=0x00;
        baud0=0x47;
        pres=0;
    }else if(baudrate == 19200){
        baud1=0x00;
        baud0=0x23;
        pres=0x00;
    }else if(baudrate == 115200){
        baud1=0x00;
        baud0=0x05;
        pres=0;
    }

    WK_WriteSReg(port, WK2XXX_BAUD1, baud1);
    WK_WriteSReg(port, WK2XXX_BAUD0, baud0);
    WK_WriteSReg(port, WK2XXX_PRES, pres);
}

void WK_PortInit(uint8_t port, uint32_t baudrate)
{
        uint8_t  value = 0, ret = 0;

        //
        // 使能子串口时钟
        //
        value = WK_ReadGReg(WK2XXX_GENA);
        value |= (1 << (port - 1));
        WK_WriteGReg(WK2XXX_GENA, value);
        ret = WK_ReadGReg(WK2XXX_GENA);

        //
        // 软件复位子串口
        //
        value = WK_ReadGReg(WK2XXX_GRST);
        value |= (1 << (port - 1));
        WK_WriteGReg(WK2XXX_GRST, value);
        ret = WK_ReadGReg(WK2XXX_GRST);

        //
        // 使能子串口总中断
        //
        value = WK_ReadGReg(WK2XXX_GIER);
        value |= (1 << (port - 1));
        WK_WriteGReg(WK2XXX_GIER, value);
        ret = WK_ReadGReg(WK2XXX_GIER);

        //
        // 使能子串口FIFO相关中断
        //
        value = WK_ReadSReg(port, WK2XXX_SIER);
        value |= WK2XXX_RFTRIG_IEN;                  // 接收FIFO触点中断
        //value |= WK2XXX_TFTRIG_IEN;                  // 发送FIFO触点中断
        value |= WK2XXX_RXOUT_IEN;                   // 接收FIFO超时中断
        WK_WriteSReg(port, WK2XXX_SIER, value);
        ret = WK_ReadSReg(port, WK2XXX_SIER);

        //
        // 设置FCR寄存器
        //
        value = 0;
        value |= (1 << 0);  // 复位接收FIFO
        value |= (1 << 1);  // 复位发送FIFO
        value |= (1 << 2);  // 使能接收FIFO(这一步必须)
        value |= (1 << 3);  // 使能发送FIFO(这一步必须)
        value |= (0 << 4);  // 设置接收FIFO触点固定为8字节
        value |= (0 << 6);  // 设置发送FIFO触点固定为8字节
        WK_WriteSReg(port, WK2XXX_FCR, value);
        ret = WK_ReadSReg(port, WK2XXX_FCR);

        //
        // 切换到page1设置中断触点和波特率
        //
        WK_WriteSReg(port, WK2XXX_SPAGE, 1);
        ret = WK_ReadSReg(port, WK2XXX_SPAGE);

        WK_WriteSReg(port, WK2XXX_RFTL, 0x40);      // 设置接收触点为64个字节
        ret = WK_ReadSReg(port, WK2XXX_RFTL);

        WK_WriteSReg(port, WK2XXX_TFTL, 0x10);      // 设置发送触点为16个字节
        ret = WK_ReadSReg(port, WK2XXX_TFTL);

                WK_setBaud(port,baudrate);

        //
        // 切换回page0
        //
        WK_WriteSReg(port, WK2XXX_SPAGE, 0);
        ret = WK_ReadSReg(port, WK2XXX_SPAGE);

        //
        // 使能子串口收发
        //
        value = WK_ReadSReg(port, WK2XXX_SCR);
        value |= WK2XXX_TXEN;
        value |= WK2XXX_RXEN;
        WK_WriteSReg(port, WK2XXX_SCR, value);
        ret = WK_ReadSReg(port, WK2XXX_SCR);
}



/*

int main()
{
        int nrecv = 0;
        uint8_t buffer[256];

        // 硬件复位一下
        WK_Rst();

        // 初始化四个端口
        WK_PortInit(1, 11059200, 9600);  // WK2124晶振我用的是11.0592MHz的，这个值根据实际进行修改
        WK_PortInit(2, 11059200, 9600);
        WK_PortInit(3, 11059200, 9600);
        WK_PortInit(4, 11059200, 9600);

        // 发送数据
        WK_PortSendData(1, "helloworld\r\n", 12);
        WK_PortSendData(2, "helloworld\r\n", 12);
        WK_PortSendData(3, "helloworld\r\n", 12);
        WK_PortSendData(4, "helloworld\r\n", 12);

        // 接收数据，轮训方式
        nrecv = WK_PortRecvData(1, buffer, sizeof(buffer));

        if(nrecv != 0)
        {
                // 处理数据
        }

}

*/

int WK_RxChars(uint8_t port,uint8_t *recbuf)
{
    uint8_t  fsr=0,rfcnt=0,rfcnt2=0,sifr=0;
  int len=0;
    sifr=WK_ReadSReg(port,WK2XXX_SIFR);
    printf("sifr %d\n",sifr);

    if((sifr&WK2XXX_RFTRIG_INT)||(sifr&WK2XXX_RXOVT_INT))//有接收中断和接收超时中断
    {
            fsr  =WK_ReadSReg(port,WK2XXX_FSR);
            rfcnt=WK_ReadSReg(port,WK2XXX_RFCNT);
            rfcnt2=WK_ReadSReg(port,WK2XXX_RFCNT);
            printf("fsr %d\n",fsr);
            printf("rfcnt %d\n",rfcnt);
            printf("rfcnt2 %d\n",rfcnt2);
            //printf("rfcnt=0x%x.\n",rfcnt);
            /*判断fifo中数据个数*/
            if(fsr& WK2XXX_RDAT)
            {
                if(!(rfcnt2>=rfcnt))
                {
                    rfcnt=rfcnt2;
                }
                len=(rfcnt==0)?256:rfcnt;
            }
        #if 1
            WK_ReadFIFO(port,recbuf,len);
        #else
            for(n=0;n<len;n++)
             *(recbuf+n)=WkReadSReg(port,WK2XXX_FDAT);
        #endif
            return len;
  }
    else
    {
        len=0;
        return len;
    }
}





