#ifndef SRC_WK2XXX_H_
#define SRC_WK2XXX_H_


#include "spi.h"
#include "stdio.h"
#include "ch32v30x.h"


#define         WK2XXX_GENA     0X00
#define         WK2XXX_GRST     0X01
#define                WK2XXX_GMUT     0X02
#define         WK2XXX_GIER     0X10
#define         WK2XXX_GIFR     0X11
#define         WK2XXX_GPDIR    0X21
#define         WK2XXX_GPDAT    0X31
//wkxxxx  slave uarts  rigister address defines

#define         WK2XXX_SPAGE    0X03
//PAGE0
#define         WK2XXX_SCR      0X04
#define         WK2XXX_LCR      0X05
#define         WK2XXX_FCR      0X06
#define         WK2XXX_SIER     0X07
#define         WK2XXX_SIFR     0X08
#define         WK2XXX_TFCNT    0X09
#define         WK2XXX_RFCNT    0X0A
#define         WK2XXX_FSR      0X0B
#define         WK2XXX_LSR      0X0C
#define         WK2XXX_FDAT     0X0D
#define         WK2XXX_FWCR     0X0E
#define         WK2XXX_RS485    0X0F
//PAGE1
#define         WK2XXX_BAUD1    0X04
#define         WK2XXX_BAUD0    0X05
#define         WK2XXX_PRES     0X06
#define         WK2XXX_RFTL     0X07
#define         WK2XXX_TFTL     0X08
#define         WK2XXX_FWTH     0X09
#define         WK2XXX_FWTL     0X0A
#define         WK2XXX_XON1     0X0B
#define         WK2XXX_XOFF1    0X0C
#define         WK2XXX_SADR     0X0D
#define         WK2XXX_SAEN     0X0E
#define         WK2XXX_RTSDLY   0X0F

//WK������չоƬ�ļĴ�����λ����
//wkxxx register bit defines
// GENA
#define         WK2XXX_UT4EN        0x08
#define         WK2XXX_UT3EN        0x04
#define         WK2XXX_UT2EN        0x02
#define         WK2XXX_UT1EN        0x01
//GRST
#define         WK2XXX_UT4SLEEP        0x80
#define         WK2XXX_UT3SLEEP        0x40
#define         WK2XXX_UT2SLEEP        0x20
#define         WK2XXX_UT1SLEEP        0x10
#define         WK2XXX_UT4RST        0x08
#define         WK2XXX_UT3RST        0x04
#define         WK2XXX_UT2RST        0x02
#define         WK2XXX_UT1RST        0x01
//GIER
#define         WK2XXX_UT4IE        0x08
#define         WK2XXX_UT3IE        0x04
#define         WK2XXX_UT2IE        0x02
#define         WK2XXX_UT1IE        0x01
//GIFR
#define         WK2XXX_UT4INT        0x08
#define         WK2XXX_UT3INT        0x04
#define         WK2XXX_UT2INT        0x02
#define         WK2XXX_UT1INT        0x01
//SPAGE
#define         WK2XXX_SPAGE0          0x00
#define         WK2XXX_SPAGE1   0x01
//SCR
#define         WK2XXX_SLEEPEN        0x04
#define         WK2XXX_TXEN     0x02
#define         WK2XXX_RXEN     0x01
//LCR
#define         WK2XXX_BREAK          0x20
#define         WK2XXX_IREN     0x10
#define         WK2XXX_PAEN     0x08
#define         WK2XXX_PAM1     0x04
#define         WK2XXX_PAM0     0x02
#define         WK2XXX_STPL     0x01
//FCR
//SIER
#define         WK2XXX_FERR_IEN      0x80
#define         WK2XXX_CTS_IEN       0x40
#define         WK2XXX_RTS_IEN       0x20
#define         WK2XXX_XOFF_IEN      0x10
#define         WK2XXX_TFEMPTY_IEN   0x08
#define         WK2XXX_TFTRIG_IEN    0x04
#define         WK2XXX_RXOUT_IEN     0x02
#define         WK2XXX_RFTRIG_IEN    0x01
//SIFR
#define         WK2XXX_FERR_INT      0x80
#define         WK2XXX_CTS_INT       0x40
#define         WK2XXX_RTS_INT       0x20
#define         WK2XXX_XOFF_INT      0x10
#define         WK2XXX_TFEMPTY_INT   0x08
#define         WK2XXX_TFTRIG_INT    0x04
#define         WK2XXX_RXOVT_INT     0x02
#define         WK2XXX_RFTRIG_INT    0x01


//TFCNT
//RFCNT
//FSR
#define         WK2XXX_RFOE     0x80
#define         WK2XXX_RFBI     0x40
#define         WK2XXX_RFFE     0x20
#define         WK2XXX_RFPE     0x10
#define         WK2XXX_RDAT     0x08
#define         WK2XXX_TDAT     0x04
#define         WK2XXX_TFULL    0x02
#define         WK2XXX_TBUSY    0x01
//LSR
#define         WK2XXX_OE       0x08
#define         WK2XXX_BI       0x04
#define         WK2XXX_FE       0x02
#define         WK2XXX_PE       0x01
//FWCR

uint16_t WK_PortSendData(uint8_t port, const void* buf, uint16_t len);
uint16_t WK_PortRecvData(uint8_t port, void* buf, uint16_t len);
void WK_PortInit(uint8_t port,uint32_t baudrate);
void WK_IRQHandler(void);
void WK_Rst(void);

uint8_t WK_ReadSReg(uint8_t port, uint8_t reg);

void WK_WriteFIFO(uint8_t port, const void* buf, uint16_t len);
void WK_ReadFIFO(uint8_t port, void* buf, uint16_t len);

int WK_RxChars(uint8_t port,uint8_t *recbuf);

#endif /* SRC_WK2XXX_H_ */
