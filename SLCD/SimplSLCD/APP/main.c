#include "SWM241.h"


void SerialInit(void);

int main(void)
{	
	uint32_t i;
	SLCD_InitStructure SLCD_initStruct;
	
	SystemInit();
	
	SerialInit();
	
	PORT_Init(PORTD, PIN5, PORTD_PIN5_SLCD_COM0, 0);
	PORT_Init(PORTD, PIN6, PORTD_PIN6_SLCD_COM1, 0);
	PORT_Init(PORTD, PIN7, PORTD_PIN7_SLCD_COM2, 0);
	PORT_Init(PORTD, PIN8, PORTD_PIN8_SLCD_COM3, 0);
	
	PORT_Init(PORTB, PIN1, PORTB_PIN1_SLCD_SEG0, 0);
	PORT_Init(PORTB, PIN2, PORTB_PIN2_SLCD_SEG1, 0);
	PORT_Init(PORTB, PIN3, PORTB_PIN3_SLCD_SEG2, 0);
	PORT_Init(PORTB, PIN4, PORTB_PIN4_SLCD_SEG3, 0);
	PORT_Init(PORTB, PIN5, PORTB_PIN5_SLCD_SEG4, 0);
	PORT_Init(PORTB, PIN6, PORTB_PIN6_SLCD_SEG5, 0);
	PORT_Init(PORTB, PIN7, PORTB_PIN7_SLCD_SEG6, 0);
	PORT_Init(PORTB, PIN8, PORTB_PIN8_SLCD_SEG7, 0);
    PORT_Init(PORTB, PIN9, PORTB_PIN9_SLCD_SEG8, 0);
    PORT_Init(PORTA, PIN7, PORTA_PIN7_SLCD_SEG9, 0);
    PORT_Init(PORTA, PIN8, PORTA_PIN8_SLCD_SEG10, 0);
    PORT_Init(PORTA, PIN9, PORTA_PIN9_SLCD_SEG11, 0);
//    PORT_Init(PORTA, PIN10, PORTA_PIN10_SLCD_SEG12, 0); //SWCLK
//    PORT_Init(PORTA, PIN11, PORTA_PIN11_SLCD_SEG13, 0); //SWDIO
    PORT_Init(PORTA, PIN12, PORTA_PIN12_SLCD_SEG14, 0);
    PORT_Init(PORTD, PIN13, PORTD_PIN13_SLCD_SEG15, 0);
    PORT_Init(PORTD, PIN14, PORTD_PIN14_SLCD_SEG16, 0);
    PORT_Init(PORTD, PIN15, PORTD_PIN15_SLCD_SEG17, 0);
    PORT_Init(PORTA, PIN3, PORTA_PIN3_SLCD_SEG18, 0);
    PORT_Init(PORTA, PIN4, PORTA_PIN4_SLCD_SEG19, 0);
    PORT_Init(PORTA, PIN6, PORTA_PIN6_SLCD_SEG20, 0);
    PORT_Init(PORTD, PIN0, PORTD_PIN0_SLCD_SEG21, 0);
    PORT_Init(PORTD, PIN1, PORTD_PIN1_SLCD_SEG22, 0);
	PORT_Init(PORTD, PIN4, PORTD_PIN4_SLCD_SEG23, 0);
    PORT_Init(PORTC, PIN0, PORTC_PIN0_SLCD_SEG24, 0);
    PORT_Init(PORTC, PIN1, PORTC_PIN1_SLCD_SEG25, 0);
    PORT_Init(PORTC, PIN2, PORTC_PIN2_SLCD_SEG26, 0);
    PORT_Init(PORTC, PIN3, PORTC_PIN3_SLCD_SEG27, 0);
    PORT_Init(PORTD, PIN10, PORTD_PIN10_SLCD_SEG28, 0);
    PORT_Init(PORTD, PIN11, PORTD_PIN11_SLCD_SEG29, 0);
    PORT_Init(PORTD, PIN12, PORTD_PIN12_SLCD_SEG30, 0);
    PORT_Init(PORTB, PIN0, PORTB_PIN0_SLCD_SEG31, 0);
	
	SLCD_initStruct.Duty = SLCD_DUTY_1DIV4;
	SLCD_initStruct.Bias = SLCD_BIAS_1DIV2;
	SLCD_initStruct.FrameFreq = SLCD_FRAMEFREQ_16Hz;
	SLCD_initStruct.DriveCurr = SLCD_DRIVECURR_25uA;
	SLCD_Init(SLCD, &SLCD_initStruct);
	
	SLCD_Open(SLCD);
	
	while(1==1)
	{
		SLCD_AllOn(SLCD);
		for(i = 0; i < 10000000; i++);
		
		SLCD_Clear(SLCD);
		for(i = 0; i < 10000000; i++);
		
		SLCD_SegWrite(SLCD_COM0, SLCD_SEG0, 1);
		SLCD_SegWrite(SLCD_COM1, SLCD_SEG1, 1);
		SLCD_SegWrite(SLCD_COM2, SLCD_SEG2, 1);
		SLCD_SegWrite(SLCD_COM3, SLCD_SEG3, 1);
		for(i = 0; i < 10000000; i++);
	}
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
