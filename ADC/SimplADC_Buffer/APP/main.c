#include "SWM241.h"

uint16_t Buffer[1000] = {0};

void SerialInit(void);

int main(void)
{
	uint32_t i;
	ADC_InitStructure ADC_initStruct;
	
	SystemInit();
	
	SerialInit();
	
	PORT_Init(PORTA, PIN14, PORTA_PIN14_ADC_IN0, 0);//PA.14 => ADC.CH0
	PORT_Init(PORTA, PIN13, PORTA_PIN13_ADC_IN1, 0);//PA.13 => ADC.CH1
	PORT_Init(PORTA, PIN12, PORTA_PIN12_ADC_IN2, 0);//PA.12 => ADC.CH2
	PORT_Init(PORTA, PIN11, PORTA_PIN11_ADC_IN3, 0);//PA.11 => ADC.CH3
	PORT_Init(PORTA, PIN10, PORTA_PIN10_ADC_IN4, 0);//PA.10 => ADC.CH4
	PORT_Init(PORTA, PIN9,  PORTA_PIN9_ADC_IN5,  0);//PA.9  => ADC.CH5
	PORT_Init(PORTA, PIN8,  PORTA_PIN8_ADC_IN6,  0);//PA.8  => ADC.CH6
	PORT_Init(PORTA, PIN7,  PORTA_PIN7_ADC_IN7,  0);//PA.7  => ADC.CH7
	
	ADC_initStruct.clk_src = ADC_CLKSRC_HRC;
	ADC_initStruct.clk_div = 12;
	ADC_initStruct.ref_src = ADC_REFSRC_VREFP;
	ADC_initStruct.channels = ADC_CH1;
	ADC_initStruct.trig_src = ADC_TRIGSRC_SW;
	ADC_initStruct.samplAvg = ADC_AVG_SAMPLE1;
	ADC_initStruct.Continue = 0;					//非连续模式，即单次模式
	ADC_initStruct.EOC_IEn = 0;
	ADC_initStruct.OVF_IEn = 0;
	ADC_Init(ADC, &ADC_initStruct);					//配置ADC
	
	ADC_Open(ADC);									//使能ADC
	
	while(1==1)
	{
		for(i = 0; i < 1000; i++)
		{
			ADC_Start(ADC);
			while((ADC->CH[1].STAT & ADC_STAT_EOC_Msk) == 0);
			ADC->CH[1].STAT = (1 << ADC_STAT_EOC_Pos);
			
			Buffer[i] = ADC_Read(ADC, ADC_CH1);
		}
		
		for(i = 0; i < 1000; i++)
			printf("%4d,", Buffer[i]);
		printf("\r\n\r\n");
	}
}


void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTB, PIN11, PORTB_PIN11_UART0_RX, 1);	//GPIOB.11配置为UART0输入引脚
	PORT_Init(PORTB, PIN12, PORTB_PIN12_UART0_TX, 0);	//GPIOB.12配置为UART0输出引脚
 	
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
