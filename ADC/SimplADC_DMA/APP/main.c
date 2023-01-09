#include "SWM241.h"

#define ADC_N   2000
uint16_t ADC_Result[ADC_N];

void SerialInit(void);

int main(void)
{
	ADC_InitStructure ADC_initStruct;
	DMA_InitStructure DMA_initStruct;
	
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
	PORT_Init(PORTB, PIN9,  PORTB_PIN9_ADC_IN8,  0);//PB.9  => ADC.CH8
	PORT_Init(PORTB, PIN11, PORTB_PIN11_ADC_IN9, 0);//PB.11 => ADC.CH9
// 	PORT_Init(PORTB, PIN12, PORTB_PIN12_ADC_IN10,0);//PB.12 => ADC.CH10���� UART0.TX ��ͬһ������
	PORT_Init(PORTB, PIN14, PORTB_PIN14_ADC_IN11,0);//PB.14 => ADC.CH11
	
	ADC_initStruct.clk_src = ADC_CLKSRC_HRC;
	ADC_initStruct.clk_div = 6;
	ADC_initStruct.ref_src = ADC_REFSRC_VREFP;
	ADC_initStruct.channels = ADC_CH1;				//ʹ��ͨ��1
	ADC_initStruct.trig_src = ADC_TRIGSRC_SW;
	ADC_initStruct.samplAvg = ADC_AVG_SAMPLE1;
	ADC_initStruct.Continue = 1;					//����ģʽ
	ADC_initStruct.EOC_IEn = 0;						//ʹ��DMA�Ͳ���Ҫʹ��ADC�ж���	
	ADC_initStruct.OVF_IEn = 0;
	ADC_Init(ADC, &ADC_initStruct);					//����ADC
	
	ADC->CTRL |= (1 << ADC_CTRL_RES2FF_Pos) | (1 << ADC_CTRL_DMAEN_Pos);
	
	DMA_initStruct.Mode = DMA_MODE_SINGLE;
	DMA_initStruct.Unit = DMA_UNIT_HALFWORD;
	DMA_initStruct.Count = ADC_N;
	DMA_initStruct.SrcAddr = (uint32_t)&ADC->FFDATA;
	DMA_initStruct.SrcAddrInc = 0;
	DMA_initStruct.DstAddr = (uint32_t)ADC_Result;
	DMA_initStruct.DstAddrInc = 1;
	DMA_initStruct.Trigger = DMA_CH0_ADC0;
	DMA_initStruct.Priority = DMA_PRI_LOW;
	DMA_initStruct.DoneIE = 1;
	DMA_CH_Init(DMA_CH0, &DMA_initStruct);
	
	DMA_CH_Open(DMA_CH0);
	
	ADC_Open(ADC);									//ʹ��ADC
	ADC_Start(ADC);									//����ADC����ʼת��
	
	while(1==1)
	{
	}
}

void DMA_Handler(void)
{
	uint32_t i, res, chn;
	
	if(DMA_CH_INTStat(DMA_CH0))
	{
		ADC_Stop(ADC);
		
		DMA_CH_INTClr(DMA_CH0);		//����жϱ�־
		
		for(i = 0; i < ADC_N; i++)
		{
			chn = (ADC_Result[i] >> 12) & 0x07;
			res = (ADC_Result[i] & 0xFFF);
			printf("%d,", res);
		}
		
		DMA_CH_Open(DMA_CH0);		//�ڴ�����ɺ�ͨ���������Զ�����
		ADC_Start(ADC);
	}
}

void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTB, PIN11, PORTB_PIN11_UART0_RX, 1);	//GPIOB.11����ΪUART0��������
	PORT_Init(PORTB, PIN12, PORTB_PIN12_UART0_TX, 0);	//GPIOB.12����ΪUART0�������
	 	
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
* ��������: fputc()
* ����˵��: printf()ʹ�ô˺������ʵ�ʵĴ��ڴ�ӡ����
* ��    ��: int ch		Ҫ��ӡ���ַ�
*			FILE *f		�ļ����
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
int fputc(int ch, FILE *f)
{
	UART_WriteByte(UART0, ch);
	
	while(UART_IsTXBusy(UART0));
 	
	return ch;
}
