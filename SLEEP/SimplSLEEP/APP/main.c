#include "SWM241.h"


void SerialInit(void);

int main(void)
{	
	uint32_t i;
	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);				//接 LED，指示程序执行状态
	GPIO_SetBit(GPIOA, PIN5);						//点亮LED
	for(i = 0; i < SystemCoreClock/2; i++);			//延时，防止上电后SWD立即切掉无法下载程序
	
	GPIO_Init(GPIOA, PIN4, 0, 1, 0, 0);				//接按键，上拉使能
	SYS->PAWKEN |= (1 << PIN4);						//开启PA4引脚低电平唤醒
	
	/* 加入这三句可将功耗降至最低 */
#if 0
	PORT_Init(PORTA, PIN15, 0, 0);
	
	PORT_Init(PORTA, PIN10, 0, 0);
	PORT_Init(PORTA, PIN11, 0, 0);
#endif
	
	while(1==1)
	{
		GPIO_SetBit(GPIOA, PIN5);					//点亮LED
		for(i = 0; i < SystemCoreClock/8; i++);
		GPIO_ClrBit(GPIOA, PIN5);					//熄灭LED
		
		EnterSleepMode();
		
		while((SYS->PAWKSR & (1 << PIN4)) == 0);	//等待唤醒条件
		SYS->PAWKSR |= (1 << PIN4);					//清除唤醒状态
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
