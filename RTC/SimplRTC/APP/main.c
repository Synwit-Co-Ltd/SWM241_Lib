#include "SWM241.h"


void XTAL32K_Enable(void);
void SerialInit(void);

int main(void)
{
	RTC_InitStructure RTC_initStruct;
	
	SystemInit();
	
  SerialInit();
	
	XTAL32K_Enable();
  
	RTC_initStruct.clksrc = RTC_CLKSRC_XTAL32K;
	RTC_initStruct.Year = 2016;
	RTC_initStruct.Month = 5;
	RTC_initStruct.Date = 5;
	RTC_initStruct.Hour = 15;
	RTC_initStruct.Minute = 5;
	RTC_initStruct.Second = 5;
	RTC_initStruct.SecondIEn = 1;
	RTC_initStruct.MinuteIEn = 1;
	RTC_Init(RTC, &RTC_initStruct);
	
	RTC_Start(RTC);
	
	while(1)
	{
	}
}

void XTAL32K_Enable(void)
{
	uint32_t i;
	
  GPIO_Init(GPIOD, PIN0, 0, 0, 0, 0);
  GPIO_Init(GPIOA, PIN6, 0, 0, 0, 0);
  
	SYS->XTALCR |= (1 << SYS_XTALCR_32KON_Pos) | (7 << SYS_XTALCR_32KDRV_Pos);
	for(i = 0; i < 1000; i++) __NOP();
}

void RTC_GPIOD_Handler(void)
{
	RTC_DateTime dateTime;
	
	if(RTC_INTStat(RTC, RTC_IT_SECOND))
	{
		RTC_INTClr(RTC, RTC_IT_SECOND);
		
		RTC_GetDateTime(RTC, &dateTime);
		printf("Second ISR: %d Minute %d Second\r\n", dateTime.Minute, dateTime.Second);
	}
	
	if(RTC_INTStat(RTC, RTC_IT_MINUTE))
	{
		RTC_INTClr(RTC, RTC_IT_MINUTE);
		
		RTC_GetDateTime(RTC, &dateTime);
		printf("Minute ISR: %d Minute %d Second\r\n", dateTime.Minute, dateTime.Second);
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
