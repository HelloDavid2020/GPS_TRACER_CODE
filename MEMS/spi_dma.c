#include "spi_dma.h"


uint8_t spi1_tx_buf[6]={0xA8,0xA9,0xAA,0xAB,0xAC,0xAD};
uint8_t dma_rx_buf[12]={0x00};
uint8_t spi1_rx_buf[12]={0x00};

uint8_t spi1_tx_index=0;
uint8_t spi1_rx_index=0;
int16_t gZ= 0;
float f_gZ= 0;

void get_lms330_data(void)
{
	static uint8_t dma_tx_index  = 0;
	static uint8_t dma_rx_index = 0;
	
//	GPIO_SetBits(GPIOB, GPIO_Pin_8);
//	GPIO_ResetBits(GPIOA, GPIO_Pin_4); 
//	
//	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
//	GPIO_SetBits(GPIOA, GPIO_Pin_4); 

	SPI_SendByte(spi1_tx_buf[dma_tx_index]);
	
			while (DMA_GetFlagStatus(DMA1_FLAG_TC2) == RESET);
			
				spi1_rx_buf[dma_tx_index] = dma_rx_buf[0];
			
	dma_tx_index++;
	
	
	
	if(dma_tx_index>5) dma_tx_index = 0;
//	
	  gZ = (int16_t)( (int16_t)spi1_rx_buf[1]<<8 | spi1_rx_buf[0] );
		f_gZ = gZ* GYRORATE;

}

		

void SPI1_DMA_Configuration( void )
{
  DMA_InitTypeDef DMA_InitStructure;
    /* DMA1 Channel2 (triggered by SPI1 Rx event) Config */
  DMA_DeInit(DMA1_Channel2);  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;                          //���� SPI1 ��������(0x4001300C) ��ַ(Ŀ�ĵ�ַ)
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)dma_rx_buf;                    //���� SRAM �洢��ַ(Ŀ�ĵ�ַ)
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                                //���䷽�� ����-�ڴ�
  DMA_InitStructure.DMA_BufferSize = 1;                         //���� SPI1 ���ͳ���
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; // DMA_Mode_Circular   DMA_Mode_Normal
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel2, &DMA_InitStructure);
  
  DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
  /* Enable SPI1 DMA RX request */
  SPI1->CR2 |= 1<<0;                                                                 //���ջ�����DMAʹ��
  DMA_Cmd(DMA1_Channel2, ENABLE);
    
    
    /* DMA1 Channel3 (triggered by SPI1 Tx event) Config */
  DMA_DeInit(DMA1_Channel3);  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;                          //����  ��������(0x4001300C) ��ַ(Դ��ַ)
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)spi1_tx_buf;                    //���� SRAM �洢��ַ(Դ��ַ)
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                                //���䷽�� �ڴ�-����
  DMA_InitStructure.DMA_BufferSize = 1;                           //���� SPI1 ���ճ���
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                  //�����ַ����(����)
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                           //�ڴ��ַ����(�仯)
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;           //���贫����(�ֽ�)
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;                   //�ڴ洫����(�ֽ�)
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                                     //���䷽ʽ,һ�δ�����ֹͣ,�����¼���
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                           //�жϷ�ʽ-��(����)
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                      //�ڴ浽�ڴ淽ʽ��ֹ
  DMA_Init(DMA1_Channel3, &DMA_InitStructure);
  
  DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, DISABLE);                                   //���� DMA1_Channel3 ��������ж�
  DMA_ITConfig(DMA1_Channel3, DMA_IT_TE, ENABLE);                                   //���� DMA1_Channel3 ��������ж�
  /* Enable SPI1 DMA TX request */
  SPI1->CR2 |= 1<<1;                                                                //���ͻ�����DMAʹ��
  DMA_Cmd(DMA1_Channel3, DISABLE);                                                  //���� DMA ͨ�� DMA1_Channel3
	




	
//    RCC->AHBENR |= 1<<0 ;                     //DMA1ʱ��ʹ��

//	/*------------------����SPI1_RX_DMAͨ��Channel2---------------------*/

//    DMA1_Channel2->CCR &= ~( 1<<14 ) ;        //�Ǵ洢�����洢��ģʽ
//	DMA1_Channel2->CCR |=    2<<12   ;        //ͨ�����ȼ���
//	DMA1_Channel2->CCR &= ~( 3<<10 ) ;        //�洢�����ݿ��8bit
//	DMA1_Channel2->CCR &= ~( 3<<8  ) ;        //�������ݿ��8bit
//	DMA1_Channel2->CCR |=    1<<7    ;        //�洢����ַ����ģʽ
//	DMA1_Channel2->CCR &= ~( 1<<6  ) ;        //��ִ�������ַ����ģʽ
//	DMA1_Channel2->CCR &= ~( 1<<5  ) ;        //ִ��ѭ������
//	DMA1_Channel2->CCR &= ~( 1<<4  ) ;        //�������

//	DMA1_Channel2->CNDTR &= 0x0000   ;        //���������Ĵ�������
//	DMA1_Channel2->CNDTR = 1 ;       //������������Ϊbuffersize��

//	DMA1_Channel2->CPAR = (uint32_t)&SPI1->DR ;      //���������ַ��ע��PSIZE
//	DMA1_Channel2->CMAR = (uint32_t)spi1_rx_buf; //����DMA�洢����ַ��ע��MSIZE

//	/*------------------����SPI1_TX_DMAͨ��Channel3---------------------*/

//	DMA1_Channel3->CCR &= ~( 1<<14 ) ;        //�Ǵ洢�����洢��ģʽ
//	DMA1_Channel3->CCR |=    0<<12   ;        //ͨ�����ȼ����
//	DMA1_Channel3->CCR &= ~( 3<<10 ) ;        //�洢�����ݿ��8bit
//	DMA1_Channel3->CCR &= ~( 3<<8 )  ;        //�������ݿ��8bit
//	DMA1_Channel3->CCR |=    1<<7    ;        //�洢����ַ����ģʽ
//	DMA1_Channel3->CCR &= ~( 1<<6 )  ;        //��ִ�������ַ����ģʽ
//	DMA1_Channel3->CCR &= ~( 1<<5 ) ;         //��ִ��ѭ������
//	DMA1_Channel3->CCR |=    1<<4    ;        //�Ӵ洢����

//	DMA1_Channel3->CNDTR &= 0x0000   ;        //���������Ĵ�������
//	DMA1_Channel3->CNDTR = 1 ;       //������������Ϊbuffersize��
//	
//	DMA1_Channel3->CPAR = (uint32_t)&SPI1->DR ;      //���������ַ��ע��PSIZE
//	DMA1_Channel3->CMAR = (uint32_t)spi1_tx_buf ; //����DMA�洢����ַ��ע��MSIZE				 
}




void SPI1_ReceiveSendByte( u16 num )
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	GPIO_SetBits(GPIOA, GPIO_Pin_4); 

	
	DMA1_Channel2->CNDTR = 0x0000   ;           //���������Ĵ�������
	DMA1_Channel2->CNDTR = num ;         //������������Ϊbuffersize��

	DMA1_Channel3->CNDTR = 0x0000   ;           //���������Ĵ�������
	DMA1_Channel3->CNDTR = num ;         //������������Ϊbuffersize��

	DMA1->IFCR = 0xF0 ;                         //���ͨ��2�ı�־λ
	DMA1->IFCR = 0xF00 ;                        //���ͨ��3�ı�־λ

	SPI1->DR ;									//����ǰ��һ��SPI1->DR����֤���ջ�����Ϊ��

	while( ( SPI1->SR & 0x02 ) == 0 );
	
	DMA1_Channel3->CCR |= 1 << 0 ;              //����DMAͨ��3
	DMA1_Channel2->CCR |= 1 << 0 ;              //����DMAͨ��2	

	while( ( DMA1->ISR & 0x20 ) == 0 );

	DMA1_Channel3->CCR &= ~( 1 << 0 ) ;         //�ر�DMAͨ��3
	DMA1_Channel2->CCR &= ~( 1 << 0 ) ;         //�ر�DMAͨ��2
	
	
  /* Set chip select High at the end of the transmission */ 
	GPIO_SetBits(GPIOB, GPIO_Pin_8);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);	

}

//SPI����һ���ֽ�
uint8 SPI_SendByte(uint8 Data)
{
	spi1_tx_buf[0] = Data;
	SPI1_ReceiveSendByte(1);

	return (spi1_rx_buf[0]);
}

////SPI��ȡLen�ֽ�
//void SPI_ReadBytes( uint8 Addr, NtrxBufferPtr Buffer, uint8 Len )
//{
//	uint8 i = 0;

//	if( ( Len > 0x80 ) || ( Len == 0 ) )
//		Len = 1 ;

//	SPI1_TX_Buff[0] = Len;
//	SPI1_TX_Buff[1] = Addr;

//	SPI1_ReceiveSendByte(Len + 2);

//	for (i = 0;i < Len;i++)
//	{
//	 	*Buffer++ = SPI1_RX_Buff[i + 2];	
//	}
//}

unsigned char SPI_Read(unsigned char* data,unsigned char bytesNumber)
{
    unsigned char byte = 0;
    for(byte = 0; byte < bytesNumber; byte++)
    {
         spi1_tx_buf[byte]=0;
    }
    SPI1_ReceiveSendByte(bytesNumber);
    for(byte = 0; byte < bytesNumber; byte++)
    {
         data[byte]=spi1_rx_buf[byte];
    }
    return bytesNumber;
}
