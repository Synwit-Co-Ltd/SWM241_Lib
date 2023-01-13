#include "SWM241.h"


void SerialInit(void);

int main(void)
{
	uint32_t i, j, k;
	SLED_InitStructure SLED_initStruct;
	
	SystemInit();
	
	SerialInit();
	
	PORT_Init(PORTD, PIN6,  PORTD_PIN6_SLED_COM0,   0);
	PORT_Init(PORTD, PIN7,  PORTD_PIN7_SLED_COM1,   0);
	PORT_Init(PORTD, PIN8,  PORTD_PIN8_SLED_COM2,   0);
	PORT_Init(PORTD, PIN9,  PORTD_PIN9_SLED_COM3,   0);
	PORT_Init(PORTA, PIN0,  PORTA_PIN0_SLED_COM4,   0);
	PORT_Init(PORTA, PIN1,  PORTA_PIN1_SLED_COM5,   0);
	PORT_Init(PORTA, PIN2,  PORTA_PIN2_SLED_COM6,   0);
	PORT_Init(PORTC, PIN0,  PORTC_PIN0_SLED_COM7,   0);
	
	PORT_Init(PORTB, PIN1,  PORTB_PIN1_SLED_SEG0,   0);
	PORT_Init(PORTB, PIN2,  PORTB_PIN2_SLED_SEG1,   0);
	PORT_Init(PORTB, PIN3,  PORTB_PIN3_SLED_SEG2,   0);
	PORT_Init(PORTB, PIN4,  PORTB_PIN4_SLED_SEG3,   0);
 	PORT_Init(PORTB, PIN5,  PORTB_PIN5_SLED_SEG4,   0);
 	PORT_Init(PORTB, PIN6,  PORTB_PIN6_SLED_SEG5,   0);
	PORT_Init(PORTB, PIN7,  PORTB_PIN7_SLED_SEG6,   0);
	PORT_Init(PORTB, PIN9,  PORTB_PIN9_SLED_SEG7,   0);
	PORT_Init(PORTD, PIN13, PORTD_PIN13_SLED_SEG8,  0);
	PORT_Init(PORTD, PIN14, PORTD_PIN14_SLED_SEG9,  0);
	PORT_Init(PORTD, PIN15, PORTD_PIN15_SLED_SEG10, 0);
	PORT_Init(PORTA, PIN3,  PORTA_PIN3_SLED_SEG11,  0);
	PORT_Init(PORTA, PIN7,  PORTA_PIN7_SLED_SEG12,  0);
	PORT_Init(PORTA, PIN8,  PORTA_PIN8_SLED_SEG13,  0);
	PORT_Init(PORTA, PIN9,  PORTA_PIN9_SLED_SEG14,  0);
//	PORT_Init(PORTA, PIN10, PORTA_PIN10_SLED_SEG15, 0);		// SWCLK
//	PORT_Init(PORTA, PIN11, PORTA_PIN11_SLED_SEG16, 0);		// SWDIO
	PORT_Init(PORTC, PIN1,  PORTC_PIN1_SLED_SEG17,  0);
	PORT_Init(PORTC, PIN2,  PORTC_PIN2_SLED_SEG18,  0);
	PORT_Init(PORTC, PIN3,  PORTC_PIN3_SLED_SEG19,  0);
	
	SLED_initStruct.duty = SLED_DUTY_8;
	SLED_initStruct.clkdiv = SLED_CLKDIV_1024;
	SLED_initStruct.period = 50;		// 24MHz / 1024 / 50 / 8 = 58Hz
	SLED_initStruct.high   = 45;		// 45 / 50 = 90%
	SLED_initStruct.cominv = 0;
   	SLED_Init(SLED, &SLED_initStruct);
	
	SLED_Start(SLED);
	
	while(1==1)
	{
		for(i = 0; i < 8; i++)
		{
			for(j = 0; j < 20; j++)
			{
				SLED->DATA[i] = (1 << j);
				
				for(k = 0; k < 10000000; k++) __NOP();
			}
		}
	}
}

void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTA, PIN5, PORTA_PIN5_UART0_RX, 1);	//GPIOA.5配置为UART0输入引脚
	PORT_Init(PORTA, PIN4, PORTA_PIN4_UART0_TX, 0);	//GPIOA.4配置为UART0输出引脚
 	
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
