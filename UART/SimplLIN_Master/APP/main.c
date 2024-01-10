#include "SWM241.h"

#define LIN_ID_Sensor	0x11	// 此从机是一个传感器，只能从它读数据
#define LIN_ID_Switch	0x21	// 此从机是一个开关，  只能向它写数据

#define LIN_NB_Sensor	6		// Number of Byte, 传感器的数据是 6 字节
#define LIN_NB_Switch	4

uint8_t Buffer_Sensor[LIN_NB_Sensor] = { 0 };
uint8_t Buffer_Switch[LIN_NB_Switch] = { 0x12, 0x34, 0x56, 0x78 };


volatile uint8_t  LIN_Now_ID = 0;
volatile bool	  LIN_Now_enhanced_checksum = false;

volatile uint32_t LIN_Rcv_Index = 0;
volatile bool     LIN_Rcv_Complete = false;

volatile uint32_t LIN_Trx_Index = 0;


void SerialInit(void);
void LIN_Master_Init(void);
void LIN_Master_Start(uint8_t lin_id, bool enhanced_checksum);

int main(void)
{
	SystemInit();
	
	SerialInit();
	
	LIN_Master_Init();
	
	TIMR_Init(TIMR4, TIMR_MODE_TIMER, CyclesPerUs, 1000000/9600, 1);	// 发出 break 后，延迟一个 bit 时间再发送后续内容
	
	TIMR_Init(TIMR5, TIMR_MODE_TIMER, CyclesPerUs, 20000, 0);
	
	while(1==1)
	{
		LIN_Master_Start(LIN_ID_Sensor, false);		// 从 Sensor 读数据
		
		TIMR_Stop(TIMR5);
		TIMR_Start(TIMR5);
		TIMR_INTClr(TIMR5);
		while(!LIN_Rcv_Complete)
		{
			if(TIMR_INTStat(TIMR5))	// 20ms 未接收到数据，接收超时
			{
				goto retry;
			}
		}
		LIN_Rcv_Complete = false;
		
		printf("Data from Sensor: ");
		for(int i = 0; i < LIN_NB_Sensor; i++)
		{
			printf("%02X, ", Buffer_Sensor[i]);
		}
		printf("\n\n");
		
		LIN_Master_Start(LIN_ID_Switch, false);		// 向 Switch 写数据
		
retry:
		for(int i = 0; i < SystemCoreClock/4; i++) __NOP();
	}
}


void LIN_Master_Init(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTB, PIN9, PORTB_PIN9_UART1_RX, 1);
	PORT_Init(PORTD, PIN9, PORTD_PIN9_UART1_TX, 0);
 	
 	UART_initStruct.Baudrate = 9600;
	UART_initStruct.DataBits = UART_DATA_8BIT;
	UART_initStruct.Parity = UART_PARITY_NONE;
	UART_initStruct.StopBits = UART_STOP_1BIT;
	UART_initStruct.RXThreshold = 0;
	UART_initStruct.RXThresholdIEn = 1;
	UART_initStruct.TXThreshold = 0;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutTime = 10;
	UART_initStruct.TimeoutIEn = 0;
 	UART_Init(UART1, &UART_initStruct);
	
	UART_LINConfig(UART1, 13, 0, 14, 1);
	
	UART_Open(UART1);
}


void LIN_Master_Start(uint8_t lin_id, bool enhanced_checksum)
{
	LIN_Now_ID = lin_id;
	LIN_Now_enhanced_checksum = enhanced_checksum;
	
	UART_LINGenerate(UART1);
}


void UART1_Handler(void)
{
	if(UART_LININTStat(UART1, UART_IT_LIN_GEN))
	{
		UART_LININTClr(UART1, UART_IT_LIN_GEN);
		
		LIN_Rcv_Index = 0;
		LIN_Trx_Index = 0;
		
		TIMR_Start(TIMR4);
	}
	else if(UART_INTStat(UART1, UART_IT_RX_THR))
	{
		uint32_t chr;
		
		UART_ReadByte(UART1, &chr);
		
		if(LIN_Rcv_Index < 2)
		{
			// 主机发出的 0x55 和 ID 经过收发器，主机自己也会接收到
		}
		else if(LIN_Rcv_Index < 2 + LIN_NB_Sensor)
		{
			Buffer_Sensor[LIN_Rcv_Index - 2] = chr;
		}
		else if(LIN_Rcv_Index == 2 + LIN_NB_Sensor)
		{
			/* TODO: 计算 Checksum 并比较 */
			
			LIN_Rcv_Complete = true;
		}
		LIN_Rcv_Index++;
	}
	else if(UART_INTStat(UART1, UART_IT_TX_THR))
	{
		if(LIN_Trx_Index < LIN_NB_Switch)
		{
			UART_WriteByte(UART1, Buffer_Switch[LIN_Trx_Index++]);
		}
		else if(LIN_Trx_Index == LIN_NB_Switch)
		{
			uint8_t checksum = UART_LIN_Checksum(LIN_Now_ID, Buffer_Switch, LIN_NB_Switch, LIN_Now_enhanced_checksum);
			
			UART_WriteByte(UART1, checksum);
			
			UART_INTDis(UART1, UART_IT_TX_THR);
		}
	}
}


void TIMR4_Handler(void)
{
	TIMR_Stop(TIMR4);
	TIMR_INTClr(TIMR4);
	
	UART_WriteByte(UART1, 0x55);
	UART_WriteByte(UART1, UART_LIN_IDParity(LIN_Now_ID));
	
	if(LIN_Now_ID == LIN_ID_Switch)
	{
		UART_INTEn(UART1, UART_IT_TX_THR);
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
