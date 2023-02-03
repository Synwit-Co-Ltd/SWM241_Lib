#include "SWM241.h"

void SerialInit(void);

int main(void)
{	
	uint32_t i;
	uint32_t rxdata, txdata = 0x23;
	SPI_InitStructure SPI_initStruct;
	
	SystemInit();
	
	SerialInit();	
	
	PORT_Init(PORTB, PIN2, PORTB_PIN2_SPI0_SCLK, 0);
	PORT_Init(PORTB, PIN3, PORTB_PIN3_SPI0_MISO, 1);
	PORT_Init(PORTB, PIN4, PORTB_PIN4_SPI0_MOSI, 0);
	
	SPI_initStruct.clkDiv = SPI_CLKDIV_16;
	SPI_initStruct.FrameFormat = SPI_FORMAT_SPI;
	SPI_initStruct.SampleEdge = SPI_SECOND_EDGE;
	SPI_initStruct.IdleLevel = SPI_HIGH_LEVEL;
	SPI_initStruct.WordSize = 8;
	SPI_initStruct.Master = 1;
	SPI_initStruct.RXThreshold = 4;
	SPI_initStruct.RXThresholdIEn = 0;
	SPI_initStruct.TXThreshold = 4;
	SPI_initStruct.TXThresholdIEn = 0;
	SPI_Init(SPI0, &SPI_initStruct);
	
	SPI_Open(SPI0);
	
	while(1==1)
	{
		rxdata = SPI_ReadWrite(SPI0, txdata);	// �� MISO �� MOSI ��������
		txdata = rxdata + 1;
		
		printf("rxdata: %d\r\n", rxdata);
		
		for(i = 0; i < SystemCoreClock/16; i++);
	}
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
