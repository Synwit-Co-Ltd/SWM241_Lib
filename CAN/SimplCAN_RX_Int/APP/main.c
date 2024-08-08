#include "SWM241.h"


/* 测试条件：SWM221 <==CAN_RX/TX==> CAN 收发器（如 TJA1050）<==CAN_H/L==> CAN 分析仪
 * 错误处理演示：
 *	1、将 CAN 收发器与 CAN 分析仪断开，触发 ACK 错误，CAN->TXERR 从 0 递增到 128，然后保持不变
 *	2、将 SWM221 与 CAN 收发器断开，触发位错误，CAN->TXERR 继续递增到 255，触发 Bus Off，CAN->CR.RST 自动置 1，CAN 模块处于复位状态
 *	3、ISR 中在检测到 Bus Off 后执行 CAN_Open(CAN0) 使 CAN 模块退出复位状态，此时将上面断开的连接恢复，CAN->TXERR 将逐渐递减到0，CAN 模块恢复正常工作
*/


void SerialInit(void);

int main(void)
{
	CAN_InitStructure CAN_initStruct;
	
	SystemInit();
	
	SerialInit();
	
	PORT_Init(PORTB, PIN5, PORTB_PIN5_CAN_RX, 1);	//GPIOB.5配置为CAN输入引脚
	PORT_Init(PORTB, PIN4, PORTB_PIN4_CAN_TX, 0);	//GPIOB.4配置为CAN输出引脚
	
	CAN_initStruct.Mode = CAN_MODE_NORMAL;
	CAN_initStruct.CAN_bs1 = CAN_BS1_5tq;
	CAN_initStruct.CAN_bs2 = CAN_BS2_4tq;
	CAN_initStruct.CAN_sjw = CAN_SJW_2tq;
	CAN_initStruct.Baudrate = 50000;
	CAN_initStruct.RXNotEmptyIEn = 1;
	CAN_initStruct.ArbitrLostIEn = 0;
	CAN_initStruct.ErrPassiveIEn = 1;
	CAN_Init(CAN, &CAN_initStruct);
	
	CAN_INTEn(CAN, CAN_IT_ERR_WARN | CAN_IT_RX_OVERFLOW);
	
	CAN_SetFilter32b(CAN, CAN_FILTER_1, 0x00122122, 0x1FFFFFFE);		//接收ID为0x00122122、0x00122123的扩展包
	CAN_SetFilter16b(CAN, CAN_FILTER_2, 0x122, 0x7FE, 0x101, 0x7FF);	//接收ID为0x122、123、0x101的标准包
	
	TIMR_Init(TIMR0, TIMR_MODE_TIMER, CyclesPerUs, 1000, 1);			//注意：没有 CAN 中断向量，开启定时中断，周期性查询 CAN 中断状态
	TIMR_Start(TIMR0);
	
	CAN_Open(CAN);
	
	while(1==1)
	{
		uint8_t tx_data[8] = {1, 2, 3, 4, 5, 6, 7, 8};
		
		CAN_Transmit(CAN, CAN_FRAME_STD, 0x133, tx_data, 8, 1);
		while(CAN_TXComplete(CAN) == 0) __NOP();
		
		printf("\r\nCAN->TXERR: %d\r\n", CAN->TXERR);
		
		for(int i = 0; i < SystemCoreClock / 16; i++) __NOP();
	}
}


void TIMR0_Handler(void)
//void CAN_Handler(void)
{
	uint32_t int_sr = CAN_INTStat(CAN);
	
	TIMR_INTClr(TIMR0);
	
	if(int_sr & CAN_IF_RXDA_Msk)
	{
		CAN_RXMessage msg;
		
		CAN_Receive(CAN, &msg);
		
		if(msg.size > 0)
		{
			printf("\r\nReceive %s: %08X, ", msg.format == CAN_FRAME_STD ? "STD" : "EXT", msg.id);
			for(int i = 0; i < msg.size; i++) printf("%02X, ", msg.data[i]);
		}
		else if(msg.remote == 1)	//远程帧
		{
			printf("\r\nReceive %s Remote Request", msg.format == CAN_FRAME_STD ? "STD" : "EXT");
		}
	}
	
	if(int_sr & CAN_IF_RXOV_Msk)
	{
		printf("\r\nCAN RX Overflow\r\n");
		
		CAN_Close(CAN);
		for(int i = 0; i < CyclesPerUs; i++) __NOP();
		CAN_Open(CAN);
	}
	
	if(int_sr & CAN_IF_ERRWARN_Msk)
	{
		if(CAN->SR & CAN_SR_BUSOFF_Msk)
		{
			printf("\r\nCAN Bus Off\r\n");
			printf("\r\nCAN->CR.RST = %d\r\n", CAN->CR & CAN_CR_RST_Msk ? 1 : 0);
			
			CAN_Open(CAN);			//Bus Off recovery
		}
		else if(CAN->SR & CAN_SR_ERRWARN_Msk)
		{
			printf("\r\nCAN Error Warning\r\n");
		}
	}
	
	if(int_sr & CAN_IF_ERRPASS_Msk)
	{
		printf("\r\nCAN Error Passive\r\n");
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
