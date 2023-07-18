#include "mass_angle.h"
/**************************************磁编码器**************************************************************/
//float get_MASS_data(void){
//  uint32_t angle[2];
//  float a_angle = 0;
//
//  MASS_IIC_Start();
//  MASS_IIC_Send_Byte(MASS_SLAVE_ADDR); //发送从机地址
//  MASS_IIC_Wait_Ack();
//  MASS_IIC_Send_Byte(MASS_MEM1_ADDR); //发送寄存器地址
//  MASS_IIC_Wait_Ack();
//  MASS_IIC_Stop();
//
//  MASS_IIC_Start();
//  MASS_IIC_Send_Byte(MASS_SLAVE_ADDR+1); //切换到读取模式
//  MASS_IIC_Wait_Ack();
//  angle[0] = MASS_IIC_Read_Byte(1); //NAK的读取
//  //printf("angle0 %d\n",angle[0]);
//  MASS_IIC_Stop();
//
//  MASS_IIC_Start();
//  MASS_IIC_Send_Byte(MASS_SLAVE_ADDR); //发送从机地址
//  MASS_IIC_Wait_Ack();
//  MASS_IIC_Send_Byte(MASS_MEM2_ADDR); //发送寄存器地址
//  MASS_IIC_Wait_Ack();
//  MASS_IIC_Stop();
//
//  MASS_IIC_Start();
//  MASS_IIC_Send_Byte(MASS_SLAVE_ADDR+1); //切换到读取模式
//  MASS_IIC_Wait_Ack();
//  angle[1] = MASS_IIC_Read_Byte(1); //NAK的读取
//  //printf("angle1 %d\n",angle[1]);
//  MASS_IIC_Stop();
//
//  a_angle = (angle[0] << 6) | (angle[1] >> 2); //14Bit角度信息，存储在0x03[13:6]、0x04[5:0]两个寄存器中，高位在前，原始读数0~16383
//  a_angle = a_angle*360/16384; //转换单位到°
//
//  return a_angle;
//}


//static rt_tick_t now_os_tick;//当前获取的系统tick
//static rt_tick_t last_os_tick = 0;

//float mass_ori_angle;
//float _mass_last_angle;
//float mass_final_angle;
//float mass_count = 0;

//void get_mass_angle(void){
//  mass_ori_angle = get_MASS_data();
//          while(mass_ori_angle == 0){
//              mass_ori_angle = get_MASS_data();
//          }
//          _mass_last_angle = mass_ori_angle;
//          //printf("ori_angle %f\n",mass_ori_angle);
//}


//void get_mass_final_angle(int command){
//  float mass_now_angle;
//  mass_now_angle = get_MASS_data();
//  if(mass_now_angle == 0){
//      mass_now_angle = _mass_last_angle;
//  }
//  switch(command){
//      case clockwise:
////            if(last_angle-now_angle>=150 && last_angle-now_angle<=300){
////                now_angle = last_angle;
////            }
//          if(mass_now_angle+300 < _mass_last_angle){
//                  now_os_tick = rt_tick_get();
//                  if(now_os_tick - last_os_tick >0.5 && _mass_last_angle>350 && mass_now_angle>0 && mass_now_angle<5){
//                      mass_count++;
//                  }
//          }
//          if(mass_now_angle > _mass_last_angle+300){
//              now_os_tick = rt_tick_get();
//              if(now_os_tick - last_os_tick >0.5 && mass_now_angle>350 && _mass_last_angle>0 && _mass_last_angle<5){
//                      mass_count--;
//                  }
//          }
//          //printf("now_angle %f\n",now_angle);
//          mass_final_angle = (360-mass_ori_angle) + 360*(mass_count-1) +mass_now_angle;
//          _mass_last_angle = mass_now_angle;
//          break;
//      case counterclockwise:
//          if(mass_now_angle-_mass_last_angle>=100){
//              mass_now_angle = _mass_last_angle;
//          }
//          if(mass_now_angle+300 > _mass_last_angle){
//                  mass_count++;
//          }
//          mass_final_angle = mass_ori_angle + 360*(mass_count-1) +(360-mass_now_angle);
//          _mass_last_angle = mass_now_angle;
//          break;
//      default:
//          break;
//  }
//}
/*********************************************************************************************************/



///*******************************************************************************
//* Function Name  :SensorWritenByte
//* Description    :  Sensor read api.
//* Input          : devid-设备i2c地址
//*                : *TXBuff-the address of the buffer which to write.
//*                : SubAdd-the address read to
//*                : Size-the buffer size
//* Output         : None
//* Return         : None
//*******************************************************************************/
//unsigned char SensorWritenByte(unsigned char Devid, unsigned char *TXBuff, unsigned char SubAdd, unsigned char Size)
//{
//    unsigned char i = 0;
//
//    MASS_IIC_Start();
//
//    MASS_IIC_Send_Byte( Devid | 0x00 );
//
//    if( 0 == MASS_IIC_Wait_Ack() )
//    {
//        MASS_IIC_Stop();
//        return 0;
//    }
//
//    MASS_IIC_Send_Byte( SubAdd & 0xff );
//
//    if( 0 == MASS_IIC_Wait_Ack() )
//    {
//        MASS_IIC_Stop();
//        return 0;
//    }
//
//    for ( i = 0; i < Size; i++)
//    {
//        MASS_IIC_Send_Byte( TXBuff[Size - i - 1] );
//
//        if( 0 == MASS_IIC_Wait_Ack() )
//        {
//            MASS_IIC_Stop();
//            return 0;
//        }
//    }
//
//    MASS_IIC_Stop();
//
//    return 1;
//}

/*
 Pin Connect:
 GP2Y0E03            MSP430
 1.PIN--VCC          3V3
 2.PIN--Vout(A)      Not Connect
 3.PIN--GND          GND
 4.PIN--VIN(IO)      3V3
 5.PIN--GPIO1        3V3
 6.PIN--SCL          P1.4
 7.PIN--SDA          P1.5
 */


int shift_mass;

unsigned char SensorReadByte_mass(unsigned char Devid, unsigned char *RXBuff, unsigned char SubAdd, unsigned char Size){
        MASS_IIC_Start();

        MASS_IIC_Send_Byte( Devid );
    MASS_IIC_Wait_Ack();
    MASS_IIC_Send_Byte( SubAdd);
    MASS_IIC_Wait_Ack();
    MASS_IIC_Stop();

        MASS_IIC_Start();
        MASS_IIC_Send_Byte( Devid+0x01 );   //滤波距离地址
    MASS_IIC_Wait_Ack();
        shift_mass = MASS_IIC_Read_Byte(1);
        //printf("shift %d ",shift);
    MASS_IIC_Stop();
}

unsigned char R_mass[2];

/*******************************************************************************
* Function Name  : SensorReadnByte
* Description    :  Sensor read api.
* Input          : devid-设备i2c地址
*                : *RXBuff-the address of the buffer which stores read content.
*                : SubAdd-the address read from
*                : Size-the buffer size
* Output         : None
* Return         : None
*******************************************************************************/
unsigned char SensorReadnByte_mass(unsigned char Devid, unsigned char *RXBuff, unsigned char SubAdd, unsigned char Size)
{
    unsigned char i = 0;

    MASS_IIC_Start();

        MASS_IIC_Send_Byte( Devid );
    MASS_IIC_Wait_Ack();
    MASS_IIC_Send_Byte( SubAdd);
    MASS_IIC_Wait_Ack();
    MASS_IIC_Stop();

    MASS_IIC_Start();
        MASS_IIC_Send_Byte( Devid+0x01 );   //滤波距离地址
    MASS_IIC_Wait_Ack();
        R_mass[0] = MASS_IIC_Read_Byte(1);
        R_mass[1] = MASS_IIC_Read_Byte(0);
        //printf("[0] %d [1] %d \n",R[0],R[1]);
    MASS_IIC_Stop();

    return 1;
}


//extern unsigned char devid;
//extern unsigned char dirt_send_flag;
//extern unsigned char dirt_detection_flag;


//void UserI2c_Init(void)
//{
///************************/
///**配置TOF内部寄存器*****/
///**代码改于2022/6/1_dpt**/
///************************/
//  devid=0xA4;       //设置I2C从机地址  默认164 0xA4
//
//  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET);
//  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_SET);
//
//  dirt_send_flag=1;            //=0串口主动发送 =1串口或者I2C被动读取
//  SensorWritenByte(devid,(unsigned char *)&dirt_send_flag, 0x09, 1);      //0x09设置距离发送方式  1 主机去读取  0 模块发送
//  Delayms(50);
//
//  dirt_detection_flag=0; //=0滤波值 =1实时值
//  SensorWritenByte(devid,(unsigned char *)&dirt_detection_flag, 0x08, 1);  //0x08设置 距离数据模式   1 实时值 0滤波值
//  Delayms(50);
//
//}

void ScanI2CDevices_mass()
{
  printf("Scanning I2C devices...\n");
  for (uint8_t address = 0; address < 128; address++)
  {
    MASS_IIC_Start();
    MASS_IIC_Send_Byte((address << 1) | 0);
    uint8_t ack = MASS_IIC_Wait_Ack();
    MASS_IIC_Stop();
    if (ack)
    {
      printf("Device found at address: 0x%02X\n", address);
    }
  }
}
