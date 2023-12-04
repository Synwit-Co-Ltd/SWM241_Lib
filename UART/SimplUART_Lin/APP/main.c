#include "SWM241.h"
#include "CircleBuffer.h"

volatile bool msg_rcvd0 = false;
volatile bool msg_rcvd1 = false;
CircleBuffer_t CirBuf;
uint32_t chr;
uint32_t chr_count=0;
uint8_t chr_buff[256];
char TX_Str1[] = "SWM241 LIN send test\r\n";
void SerialLin_Init(void);
void SerialInit(void);
void LIN_FrameSend(UART_TypeDef * UARTx,char data[], uint32_t count);
int main(void)
{
	uint32_t i;
	SystemInit();
	
	SerialInit();
	SerialLin_Init();   	
	while(1==1)
	{
//		LIN_FrameSend(TX_Str1, strlen(TX_Str1));
//		for(i = 0; i < SystemCoreClock/50; i++) __NOP();

		if(msg_rcvd0)
		{
			chr_count=CirBuf_Count(&CirBuf);
			CirBuf_Read(&CirBuf, chr_buff, chr_count);
			
			printf("LIN0 RCV:%s\r\n",chr_buff );
			LIN_FrameSend(UART0,(char *)chr_buff,chr_count);
			msg_rcvd0=0;
		}
		if(msg_rcvd1)
		{
			chr_count=CirBuf_Count(&CirBuf);
			CirBuf_Read(&CirBuf, chr_buff, chr_count);
			
			printf("LIN1 RCV:%s\r\n",chr_buff );
			LIN_FrameSend(UART1,(char *)chr_buff,chr_count);
			msg_rcvd1=0;
		}
	}
}



void UART0_Handler(void)
{
	uint32_t chr;
	
	if(UART_INTStat(UART0, UART_IT_RX_THR | UART_IT_RX_TOUT))
	{
		while(UART_IsRXFIFOEmpty(UART0) == 0)
		{
			if(UART_ReadByte(UART0, &chr) == 0)
			{
				CirBuf_Write(&CirBuf, (uint8_t *)&chr, 1);
			}
		}
		
		if(UART_INTStat(UART0, UART_IT_RX_TOUT))
		{
			UART_INTClr(UART0, UART_IT_RX_TOUT);
			
			msg_rcvd0 = true;
		}
	}
	else if(UART_LINIsDetected(UART0))
	{
		UART0->LINCR |= UART_LINCR_BRKDETIF_Msk;	//清除中断标志
	}
}

void UART1_Handler(void)
{

	if(UART_INTStat(UART1, UART_IT_RX_THR | UART_IT_RX_TOUT))
	{
		while(UART_IsRXFIFOEmpty(UART1) == 0)
		{
			if(UART_ReadByte(UART1, &chr) == 0)
			{	
				CirBuf_Write(&CirBuf, (uint8_t *)&chr, 1);
			}
		}
		
		if(UART_INTStat(UART1, UART_IT_RX_TOUT))
		{
			UART_INTClr(UART1, UART_IT_RX_TOUT);
			
			msg_rcvd1 = true;
		}
	}
	else if(UART_LINIsDetected(UART1))
	{
		UART1->LINCR |= UART_LINCR_BRKDETIF_Msk;	//清除中断标志
	}
}
void LIN_FrameSend(UART_TypeDef * UARTx,char data[], uint32_t count)
{
	uint32_t i;

	UART_LINGenerate(UARTx);
	while(UARTx->LINCR & UART_LINCR_GENBRK_Msk) __NOP();	//完成自动清零
	
//	while(UART_IsTXFIFOFull(UARTx)) __NOP();		
//	UART_WriteByte(UARTx,0x55);//Sync
//	while(UART_IsTXFIFOFull(UARTx)) __NOP();		
//	UART_WriteByte(UARTx,0xaa);//ID
	
	for(i = 0; i < count; i++)
	{
		while(UART_IsTXFIFOFull(UARTx)) __NOP();
		
		UART_WriteByte(UARTx, data[i]);
	}
}

void SerialLin_Init(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTB, PIN9, PORTB_PIN9_UART1_RX, 1);
	PORT_Init(PORTB, PIN8, PORTB_PIN8_UART1_TX, 0);
 	
 	UART_initStruct.Baudrate = 57600;
	UART_initStruct.DataBits = UART_DATA_8BIT;
	UART_initStruct.Parity = UART_PARITY_NONE;
	UART_initStruct.StopBits = UART_STOP_1BIT;
	UART_initStruct.RXThreshold = 3;//3;
	UART_initStruct.RXThresholdIEn = 1;
	UART_initStruct.TXThreshold = 3;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutTime = 10;		//10个字符时间内未接收到新的数据则触发超时中断
	UART_initStruct.TimeoutIEn = 1;
 	UART_Init(UART1, &UART_initStruct);
	
	NVIC_EnableIRQ(UART1_IRQn);
	UART_LINConfig(UART1, 13, 1, 14, 0);
	UART_Open(UART1);
}
void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTD, PIN13, PORTD_PIN13_UART0_RX, 1);	//GPIOD.13配置为UART0输入引脚
	PORT_Init(PORTD, PIN14, PORTD_PIN14_UART0_TX, 0);	//GPIOD.14配置为UART0输出引脚
 	
 	UART_initStruct.Baudrate = 57600;//57600;
	UART_initStruct.DataBits = UART_DATA_8BIT;
	UART_initStruct.Parity = UART_PARITY_NONE;
	UART_initStruct.StopBits = UART_STOP_1BIT;
	UART_initStruct.RXThreshold = 3;
	UART_initStruct.RXThresholdIEn = 1;
	UART_initStruct.TXThreshold = 3;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutTime = 10;		//10个字符时间内未接收到新的数据则触发超时中断
	UART_initStruct.TimeoutIEn = 1;
 	UART_Init(UART0, &UART_initStruct);
	
	NVIC_EnableIRQ(UART0_IRQn);
	UART_LINConfig(UART0, 13, 1, 14, 0);	
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
	//UART_WriteByte(UART0, ch);	
	//while(UART_IsTXBusy(UART0));

	UART_WriteByte(UART1, ch);
	while(UART_IsTXBusy(UART1)); 	
	return ch;
}
