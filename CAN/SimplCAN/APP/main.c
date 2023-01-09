#include "SWM241.h"

CAN_RXMessage CAN_RXMsg;

void SerialInit(void);

int main(void)
{	
	uint32_t i;
	uint8_t tx_data[8] = {1, 2, 3, 4, 5, 6, 7, 8};
	CAN_InitStructure CAN_initStruct;
	
	SystemInit();
	
	SerialInit();
	
	PORT_Init(PORTB, PIN5, PORTB_PIN5_CAN_RX, 1);	//GPIOB.5配置为CAN输入引脚
	PORT_Init(PORTB, PIN4, PORTB_PIN4_CAN_TX, 0);	//GPIOB.4配置为CAN输出引脚
	
	CAN_initStruct.Mode = CAN_MODE_SELFTEST;	//自测模式下TX、RX要连接在一起
	CAN_initStruct.CAN_bs1 = CAN_BS1_5tq;
	CAN_initStruct.CAN_bs2 = CAN_BS2_4tq;
	CAN_initStruct.CAN_sjw = CAN_SJW_2tq;
	CAN_initStruct.Baudrate = 50000;
	CAN_initStruct.RXNotEmptyIEn = 0;
	CAN_initStruct.ArbitrLostIEn = 0;
	CAN_initStruct.ErrPassiveIEn = 0;
	CAN_Init(CAN, &CAN_initStruct);
	
	CAN_SetFilter32b(CAN, CAN_FILTER_1, 0x00122122, 0x1FFFFFFE);		//接收ID为0x00122122、0x00122123的扩展包
	CAN_SetFilter16b(CAN, CAN_FILTER_2, 0x122, 0x7FE, 0x101, 0x7FF);	//接收ID为0x122、123、0x101的标准包
	
	CAN_Open(CAN);
	
	CAN_Transmit(CAN, CAN_FRAME_STD, 0x122, tx_data, 8, 1);
	while(CAN_TXComplete(CAN) == 0) __NOP();
	
	CAN_Transmit(CAN, CAN_FRAME_EXT, 0x122122, tx_data, 8, 1);
	while(CAN_TXComplete(CAN) == 0) __NOP();
	
	while(CAN_RXDataAvailable(CAN))
	{
		CAN_Receive(CAN, &CAN_RXMsg);
		
		printf("\r\nReceive %s %08X: ", CAN_RXMsg.format == CAN_FRAME_STD ? "STD" : "EXT", CAN_RXMsg.id);
		for(i = 0; i < CAN_RXMsg.size; i++) printf("%02X, ", CAN_RXMsg.data[i]);
	}
   	
	while(1==1)
	{
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
