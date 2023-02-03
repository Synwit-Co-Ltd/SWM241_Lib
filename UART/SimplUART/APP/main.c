#include "SWM241.h"


void SerialInit(void);

int main(void)
{
	uint32_t chr;
	
	SystemInit();
	
	SerialInit();
   	
	printf("Hello World from %c%c%c%c%c%c%c%c%c%c%c%c%c!\n",83,121,110,119,105,116,46,99,111,109,46,99,110);
	while(1==1)
	{
		if(UART_IsRXFIFOEmpty(UART0) == 0)
		{
			if(UART_ReadByte(UART0, &chr) == 0)
				UART_WriteByte(UART0, chr);
		}
		
		if(UART_IsRXFIFOEmpty(UART1) == 0)
		{
			if(UART_ReadByte(UART1, &chr) == 0)
				UART_WriteByte(UART1, chr);
		}
		
		if(UART_IsRXFIFOEmpty(UART2) == 0)
		{
			if(UART_ReadByte(UART2, &chr) == 0)
				UART_WriteByte(UART2, chr);
		}
		
		if(UART_IsRXFIFOEmpty(UART3) == 0)
		{
			if(UART_ReadByte(UART3, &chr) == 0)
				UART_WriteByte(UART3, chr);
		}
	}
}

void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTD, PIN13, PORTD_PIN13_UART0_RX, 1);	//GPIOD.13配置为UART0输入引脚
	PORT_Init(PORTD, PIN14, PORTD_PIN14_UART0_TX, 0);	//GPIOD.14配置为UART0输出引脚
	PORT_Init(PORTB, PIN9,  PORTB_PIN9_UART1_RX, 1);	//GPIOB.9 配置为UART1输入引脚
	PORT_Init(PORTB, PIN8,  PORTB_PIN8_UART1_TX, 0);	//GPIOB.8 配置为UART1输出引脚
	PORT_Init(PORTB, PIN3,  PORTB_PIN3_UART2_RX, 1);	//GPIOB.3 配置为UART2输入引脚
	PORT_Init(PORTB, PIN4,  PORTB_PIN4_UART2_TX, 0);	//GPIOB.4 配置为UART2输出引脚
	PORT_Init(PORTB, PIN1,  PORTB_PIN1_UART3_RX, 1);	//GPIOB.1 配置为UART3输入引脚
	PORT_Init(PORTB, PIN0,  PORTB_PIN0_UART3_TX, 0);	//GPIOB.0 配置为UART3输出引脚
 	
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
	UART_Init(UART1, &UART_initStruct);
	UART_Open(UART1);
	UART_Init(UART2, &UART_initStruct);
	UART_Open(UART2);
	UART_Init(UART3, &UART_initStruct);
	UART_Open(UART3);
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
	UART_WriteByte(UART1, ch);
	UART_WriteByte(UART2, ch);
	UART_WriteByte(UART3, ch);
	
	while(UART_IsTXBusy(UART0));
	while(UART_IsTXBusy(UART1));
	while(UART_IsTXBusy(UART2));
	while(UART_IsTXBusy(UART3));
 	
	return ch;
}
