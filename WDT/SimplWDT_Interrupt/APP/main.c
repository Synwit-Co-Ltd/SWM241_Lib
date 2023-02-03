#include "SWM241.h"

void SerialInit(void);

int main(void)
{
	WDT_InitStructure WDT_initStruct;
	
	SystemInit();
	
	SerialInit();
		
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);			//输出，接LED
	
	WDT_initStruct.clksrc = WDT_CLKSRC_HSI;
	WDT_initStruct.clkdiv = WDT_CLKDIV_32768;
	WDT_initStruct.int_period = 100;			//约100ms
	WDT_initStruct.int_en = 1;
	WDT_initStruct.rst_period = 500;			//约500ms
	WDT_initStruct.rst_en = 0;
	WDT_initStruct.win_en = 0;
	WDT_Init(WDT, &WDT_initStruct);
		
	WDT_Start(WDT);								//启动WDT
	
	while(1==1)
	{
		
	}
}

void WDT_Handler(void)
{
	WDT_INTClr(WDT);
	
	GPIO_InvBit(GPIOA, PIN5);
	
	printf("WDT Handler\r\n");
}

void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTD, PIN13, PORTD_PIN13_UART0_RX, 1);	//GPIOD.13配置为UART0输入引脚
	PORT_Init(PORTD, PIN14, PORTD_PIN14_UART0_TX, 0);	//GPIOD.14配置为UART0输出引脚
 	
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
* 函数名称: fputc()
* 功能说明: printf()使用此函数完成实际的串口打印动作
* 输    入: int ch		要打印的字符
*			FILE *f		文件句柄
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
int fputc(int ch, FILE *f)
{
	UART_WriteByte(UART0, ch);
	
	while(UART_IsTXBusy(UART0));
 	
	return ch;
}
