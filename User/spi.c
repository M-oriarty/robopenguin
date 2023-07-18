#include "spi.h"

SPI_InitTypeDef  SPI_InitStructure;

/*---------SPI������ų�ʼ��---------*/
//��ʼ��SPI���ݴ����������
void SPI_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE);
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//���ù���
  //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

//    GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2); //PB13����ΪSPI2,PA5����Ϊ SPI1
//    GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2); //PB14����ΪSPI2,PA6����Ϊ SPI1
//    GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2); //PB15����ΪSPI2,PA7����Ϊ SPI1

    GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;       //����SPI����ģʽ:����Ϊ��SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;       //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;      //ѡ���˴���ʱ�ӵ���̬:ʱ�����յ�
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;    //���ݲ����ڵڶ���ʱ����
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;       //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;      //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI_InitStructure.SPI_CRCPolynomial = 7;    //CRCֵ����Ķ���ʽ
    SPI_Init(SPI2, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���

    SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����

//  SPI_ReadWriteByte(0xff);//��������
}

//��ʼ��SPIƬѡ�ź�CS,����CS��Ĭ��״̬����Ϊ�ߵ�ƽ
void SPI_CS_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  //PB12 �˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//���
    //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);                   //�����趨������ʼ��GPIOB12
    GPIO_SetBits(GPIOB,GPIO_Pin_12);                         //PB12 �����
}

/*-----------SPI������غ���-----------*/

//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8��Ƶ   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16��Ƶ  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256��Ƶ (SPI 281.25K@sys 72M)
void SPI_SetSpeed(u8 SpeedSet)
{
    SPI_InitStructure.SPI_BaudRatePrescaler = SpeedSet ;
  SPI_Init(SPI2, &SPI_InitStructure);
    SPI_Cmd(SPI2,ENABLE);
}

//�������ܣ���ʼ��SPI���ߣ�����SPI����Ϊ0ģʽ
void SPI_BUS_Init(void)
{
    SPI_GPIO_Init();           //��ʼ��SPI
    SPI_SetSpeed(SPI_BaudRatePrescaler_8);  //����Ϊ10Mʱ��,����ģʽ
}

//�������ܣ���ʼ��SPI�ӿ�
void rt_hw_spi_init(void)
{
    SPI_CS_Init();
    SPI_BUS_Init();
}

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI_ReadWriteByte(u8 TxData)
{
    int retry=0;
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
    {
        retry++;
        if(retry>100)return 0;
    }
    SPI_I2S_SendData(SPI2, TxData); //ͨ������SPIx����һ������
    retry=0;

    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)//���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
    {
        retry++;
    if(retry>100)return 0;
    }
    return SPI_I2S_ReceiveData(SPI2); //����ͨ��SPIx������յ�����
}

//�������ܣ�����CS�ź�Ϊ�ߵ�ƽ
void SPI_CS_H(void)
{
    GPIO_SetBits(GPIOB,GPIO_Pin_12);
}

//�������ܣ�����CS�ź�Ϊ�͵�ƽ
void SPI_CS_L(void)
{
    GPIO_ResetBits(GPIOB,GPIO_Pin_12);
}



