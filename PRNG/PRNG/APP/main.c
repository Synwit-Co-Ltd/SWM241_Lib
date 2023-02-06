#include "SWM241.h"
#include <string.h>

void SerialInit(void);

int main(void)
{		
	uint32_t i;
	
	SystemInit();
	
	SerialInit();
	
	SYS->HRCCR |= (1 << SYS_HRCCR_ON_Pos);
	SYS->LRCCR |= (1 << SYS_LRCCR_ON_Pos);
	SYS->PRNGCR = (0 << 0) | ( 3 << 1) ;

	while(1==1)
	{
		while((SYS->PRNGCR & (1<<8)) == 0) __NOP();
		printf("PRNGDL: %08X, PRNGDH: %08X\n", SYS->PRNGDL, SYS->PRNGDH);
		
		for(i = 0; i < SystemCoreClock/8; i++) __NOP();
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
	UART_initStruct.RXThreshold = 3;
	UART_initStruct.RXThresholdIEn = 0;
	UART_initStruct.TXThreshold = 3;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutTime = 10;
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
