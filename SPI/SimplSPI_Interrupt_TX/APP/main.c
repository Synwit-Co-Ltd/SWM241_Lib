#include "SWM241.h"
#include <string.h>

uint16_t *SPI_TXBuff = 0; 
uint32_t SPI_TXCount = 0;
uint32_t SPI_TXIndex = 0;

void SerialInit(void);
void SPIMstInit(void);
void SPIMstSend(uint16_t buff[], uint32_t cnt);

int main(void)
{	
	uint32_t i;
	uint16_t buff[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
	
	SystemInit();
	
	SerialInit();	
	
	SPIMstInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);		//�����жϽ���ָʾ�ź�
	
	while(1==1)
	{
		SPIMstSend(buff, 16);
		
		for(i = 0; i < SystemCoreClock/10; i++) __NOP();
	}
}

void SPIMstInit(void)
{
	SPI_InitStructure SPI_initStruct;
	
	GPIO_Init(GPIOA, PIN0, 1, 0, 0, 0);				//�������Ƭѡ
#define SPI_CS_Low()	GPIO_ClrBit(GPIOA, PIN0)
#define SPI_CS_High()	GPIO_SetBit(GPIOA, PIN0)
	SPI_CS_High();
	
	PORT_Init(PORTB, PIN2, PORTB_PIN2_SPI0_SCLK, 0);
	PORT_Init(PORTB, PIN3, PORTB_PIN3_SPI0_MISO, 1);	//�� MISO �� MOSI ��������
	PORT_Init(PORTB, PIN4, PORTB_PIN4_SPI0_MOSI, 0);

	
	SPI_initStruct.clkDiv = SPI_CLKDIV_16;
	SPI_initStruct.FrameFormat = SPI_FORMAT_SPI;
	SPI_initStruct.SampleEdge = SPI_SECOND_EDGE;
	SPI_initStruct.IdleLevel = SPI_HIGH_LEVEL;
	SPI_initStruct.WordSize = 8;
	SPI_initStruct.Master = 1;
	SPI_initStruct.RXThreshold    = 0;
	SPI_initStruct.RXThresholdIEn = 0;
	SPI_initStruct.TXThreshold    = 4;	// ������FIFO����������4��ʱ�����ж�
	SPI_initStruct.TXThresholdIEn = 0;
	SPI_initStruct.TXCompleteIEn  = 0;
	SPI_Init(SPI0, &SPI_initStruct);
	
	NVIC_EnableIRQ(SPI0_IRQn);
	
	SPI_Open(SPI0);
}

void SPIMstSend(uint16_t buff[], uint32_t cnt)
{	
	SPI_TXBuff = buff;
	SPI_TXCount = cnt;
	SPI_TXIndex = 0;
	
	SPI_CS_Low();
	
	SPI_INTEn(SPI0, SPI_IT_TX_DONE);
 	SPI_INTEn(SPI0, SPI_IT_TX_THRES);
}

void SPI0_Handler(void)
{
	if(SPI_INTStat(SPI0, SPI_IT_TX_THRES))
	{
		while((SPI_IsTXFull(SPI0) == 0) && (SPI_TXIndex < SPI_TXCount))
		{
			SPI_Write(SPI0, SPI_TXBuff[SPI_TXIndex++]);
		}
		
		if(SPI_TXIndex == SPI_TXCount)
		{
			SPI_INTDis(SPI0, SPI_IT_TX_THRES);
		}
		
		SPI_INTClr(SPI0, SPI_IT_TX_THRES);	//����жϱ�־�����������TX FIFO�����жϱ�־
	}
	
	if(SPI_INTStat(SPI0, SPI_IT_TX_DONE))
	{
		SPI_INTClr(SPI0, SPI_IT_TX_DONE);
		
		if(SPI_TXIndex == SPI_TXCount)	// Ҫ���͵�������ȫ������SPI TX FIFO
		{
			SPI_CS_High();
			SPI_INTDis(SPI0, SPI_IT_TX_DONE);
		}
	}
	
	GPIO_InvBit(GPIOA, PIN5);
}

void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTD, PIN13, PORTD_PIN13_UART0_RX, 1);	//GPIOD.13����ΪUART0��������
	PORT_Init(PORTD, PIN14, PORTD_PIN14_UART0_TX, 0);	//GPIOD.14����ΪUART0�������
 	
	UART_initStruct.Baudrate = 57600;
	UART_initStruct.DataBits = UART_DATA_8BIT;
	UART_initStruct.Parity = UART_PARITY_NONE;
	UART_initStruct.StopBits = UART_STOP_1BIT;
	UART_initStruct.RXThresholdIEn = 0;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutIEn = 0;
 	UART_Init(UART0, &UART_initStruct);
	UART_Open(UART0);
}

/****************************************************************************************************************************************** 
* ��������: fputc()
* ����˵��: printf()ʹ�ô˺������ʵ�ʵĴ��ڴ�ӡ����
* ��    ��: int ch		Ҫ��ӡ���ַ�
*			FILE *f		�ļ����
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
int fputc(int ch, FILE *f)
{
	UART_WriteByte(UART0, ch);
	
	while(UART_IsTXBusy(UART0));
 	
	return ch;
}
