#include "SWM241.h"
#include <string.h>

/* 注意：1、使用 DMA_EXHS_ 握手信号时，Mode 只能用 DMA_MODE_SINGLE
 *		 2、使用 DMA_EXHS_ 握手信号时，DMA 搬运不会自动终止；只能在 DMA_IT_DONE 中断中清零 EXTHSEN 位终止传输
*/


char str_hi[] = "Hi from Synwit\n";

void SerialInit(void);

int main(void)
{
	DMA_InitStructure DMA_initStruct;
	
	SystemInit();
	
	SerialInit();
	
	DMA_initStruct.Mode = DMA_MODE_SINGLE;
	DMA_initStruct.Unit = DMA_UNIT_BYTE;
	DMA_initStruct.Count = strlen(str_hi);
	DMA_initStruct.SrcAddr = (uint32_t)str_hi;
	DMA_initStruct.SrcAddrInc = 1;
	DMA_initStruct.DstAddr = (uint32_t)&UART0->DATA;
	DMA_initStruct.DstAddrInc = 0;
	DMA_initStruct.Handshake = DMA_EXHS_TIMR0;
	DMA_initStruct.Priority = DMA_PRI_LOW;
	DMA_initStruct.INTEn = 0;
	DMA_CH_Init(DMA_CH0, &DMA_initStruct);
	DMA_CH_Open(DMA_CH0);
	
	TIMR_Init(TIMR0, TIMR_MODE_TIMER, CyclesPerUs, 500000, 0);	//每0.5秒钟触发DMA向UART0->DATA搬运一个字节
	TIMR_Start(TIMR0);
	
	while(1==1)
	{
	}
}


void DMA_Handler(void)
{
	if(DMA_CH_INTStat(DMA_CH0, DMA_IT_DONE))
	{
		DMA_CH_INTClr(DMA_CH0, DMA_IT_DONE);
		
		DMA->CH[0].MUX &= ~DMA_MUX_EXTHSEN_Msk;
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
