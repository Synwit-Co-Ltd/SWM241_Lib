#include <string.h>
#include "SWM241.h"

volatile int TX_Index = 0;
char TX_Buffer[2][128] = {
	"Hello from Synwit!\r\n",
	"Hi from HuaXinWeiTe!\r\n"
};


void SerialInit(void);

int main(void)
{
	DMA_InitStructure DMA_initStruct;
	
	SystemInit();
	
	SerialInit();
   	
	DMA_initStruct.Mode = DMA_MODE_SINGLE;
	DMA_initStruct.Unit = DMA_UNIT_BYTE;
	DMA_initStruct.Count = strlen(TX_Buffer[TX_Index]);
	DMA_initStruct.SrcAddr = (uint32_t)TX_Buffer[TX_Index];
	DMA_initStruct.SrcAddrInc = 1;
	DMA_initStruct.DstAddr = (uint32_t)&UART0->DATA;
	DMA_initStruct.DstAddrInc = 0;
	DMA_initStruct.Handshake = DMA_CH0_UART0TX;
	DMA_initStruct.Priority = DMA_PRI_LOW;
	DMA_initStruct.INTEn = DMA_IT_DONE;
	DMA_CH_Init(DMA_CH0, &DMA_initStruct);
	DMA_CH_Open(DMA_CH0);
	
	while(1==1)
	{
	}
}


void DMA_Handler(void)
{
	int i;
	
	if(DMA_CH_INTStat(DMA_CH0, DMA_IT_DONE))
	{
		DMA_CH_INTClr(DMA_CH0, DMA_IT_DONE);
		
		for(i = 0; i < SystemCoreClock/8; i++)  __NOP();	//延时一会儿
		
		/* 发送下一条信息 */
		TX_Index = 1 - TX_Index;
		DMA_CH_SetSrcAddress(DMA_CH0, (uint32_t)TX_Buffer[TX_Index]);
		DMA_CH_SetCount(DMA_CH0, strlen(TX_Buffer[TX_Index]));
		
		DMA_CH_Open(DMA_CH0);	// 重新开始，再次搬运
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
