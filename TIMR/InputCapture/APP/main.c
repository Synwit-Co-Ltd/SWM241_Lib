#include "SWM241.h"

void SerialInit(void);
void TestSignal(void);

int main(void)
{	
	SystemInit();
	
	SerialInit();
	
	TestSignal();	//���������źŹ�Input Capture���ܲ���
	
	PORT_Init(PORTB, PIN2, PORTB_PIN2_TIMR2_IN, 1);
	
	TIMR_Init(TIMR2, TIMR_MODE_IC, CyclesPerUs, 0xFFFFFF, 0);
	
	TIMR_IC_Init(TIMR2, TIMR_ICEDGE_BOTH, 1, 1);
	
    TIMR_Start(TIMR2);
	while(1==1)
	{
	}
}

void TestSignal(void)
{
	PWM_InitStructure PWM_initStruct;
	
	PWM_initStruct.PWMnXN = 1;					//ͬʱ���PWM0A��PWM0AN
	PWM_initStruct.clkdiv = 8;					//F_PWM = 48M/8 = 6M
	PWM_initStruct.cycle = 10000;				//6M/10000 = 600Hz��PWMnXN = 1ʱƵ�ʽ��͵�300Hz
	PWM_initStruct.hduty =  2500;				//2500/10000 = 25%
    PWM_initStruct.deadzone = 0;
	PWM_initStruct.initLevel = 1;
	PWM_initStruct.HEndIE = 0;
	PWM_initStruct.NCycleIE = 0;
	PWM_initStruct.HCycleIE = 0;
	PWM_Init(PWM0A, &PWM_initStruct);
	
	PORT_Init(PORTD, PIN1,  PORTD_PIN1_PWM0A,  0);
	PORT_Init(PORTD, PIN0,  PORTD_PIN0_PWM0AN, 0);
	
	PWM_Start(PWM0A);
}

void TIMR2_Handler(void)
{
	if(TIMR_IC_CaptureH_INTStat(TIMR2))
	{
		TIMR_IC_CaptureH_INTClr(TIMR2);
		
		printf("H: %d\r\n", TIMR_IC_GetCaptureH(TIMR2));
	}
	else if(TIMR_IC_CaptureL_INTStat(TIMR2))
	{
		TIMR_IC_CaptureL_INTClr(TIMR2);
		
		printf("L: %d\r\n", TIMR_IC_GetCaptureL(TIMR2));
	}
}

void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTD, PIN13, PORTD_PIN13_UART0_RX, 1);	//GPIOD.13����ΪUART0��������
	PORT_Init(PORTD, PIN14, PORTD_PIN14_UART0_TX, 0);	//GPIOD.14����ΪUART0�������
 	
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
