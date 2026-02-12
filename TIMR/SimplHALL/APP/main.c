#include "SWM241.h"


uint32_t Period = 0,	//PWM周期长度
	     LWidth = 0;	//PWM低电平宽度

void SerialInit(void);
void TestSignal(void);

int main(void)
{
	SystemInit();
	
	SerialInit();
	
	TestSignal();	//产生测试信号供HALL功能测量
	
	PORT_Init(PORTD, PIN6, PORTD_PIN6_HALL_IN0, 1);	//PD6 -> HALL_IN0
	PORT_Init(PORTD, PIN4, PORTD_PIN4_HALL_IN1, 1);	//PD4 -> HALL_IN1
	PORT_Init(PORTD, PIN5, PORTD_PIN5_HALL_IN2, 1);	//PD5 -> HALL_IN2
	
	TIMR_Init(TIMR0, TIMR_MODE_TIMER, CyclesPerUs, 2000000, 1);		//2秒钟未检测到HALL输入变化，触发超时中断
	
	TIMRG->HALLEN = 1;
	TIMRG->HALLIF = 7;
	TIMRG->HALLIE = 1;
	NVIC_EnableIRQ(SPI1_HALL_GPIOA9_IRQn);
	
	TIMR_Start(TIMR0);
	
	while(1==1)
	{
	}
}

void SPI1_HALL_GPIOA9_Handler(void)
{
    printf("%08X,", TIMRG->HALLIF);
	TIMRG->HALLIF = 7;
	
	printf("%dus\r\n", TIMRG->HALLDR);
}

void TIMR0_Handler(void)
{
	TIMR0->IF = (1 << TIMR_IF_TO_Pos);
	
	printf("HALL Time-out\r\n");
}

void TestSignal(void)
{
	GPIO_Init(GPIOB, PIN4, 1, 0, 0, 0);
	GPIO_Init(GPIOB, PIN5, 1, 0, 0, 0);
	GPIO_Init(GPIOB, PIN6, 1, 0, 0, 0);
	
	TIMR_Init(TIMR1, TIMR_MODE_TIMER, CyclesPerUs, 100000, 1);
	TIMR_Start(TIMR1);
}

void TIMR1_Handler(void)
{
	static uint32_t step = 0;
	
	TIMR1->IF = (1 << TIMR_IF_TO_Pos);
	
	switch(step++)
	{
	case 0: GPIO_SetBit(GPIOB, PIN4); break;
	case 1: GPIO_SetBit(GPIOB, PIN5); break;
	case 2: GPIO_SetBit(GPIOB, PIN6); break;
	case 3: GPIO_ClrBit(GPIOB, PIN4); break;
	case 4: GPIO_ClrBit(GPIOB, PIN5); break;
	case 5: GPIO_ClrBit(GPIOB, PIN6); step = 0; break;
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
