#include "SWM241.h"

uint32_t TStart = 0xFFFFFF;	//Timer��ʼֵ
uint32_t Period = 0,		//PWM���ڳ���
	     LWidth = 0;		//PWM�͵�ƽ���

void SerialInit(void);
void TestSignal(void);

int main(void)
{	
	uint32_t i;
	
	SystemInit();
	
	SerialInit();
	
	TestSignal();	//���������źŹ�HALL���ܲ���
	
	PORT_Init(PORTD, PIN6, PORTD_PIN6_HALL_A, 1);	//PD6 -> HALL_A
	PORT_Init(PORTD, PIN4, PORTD_PIN4_HALL_B, 1);	//PD4 -> HALL_B
	PORT_Init(PORTD, PIN5, PORTD_PIN5_HALL_C, 1);	//PD5 -> HALL_C
	
	TIMR_Init(TIMR0, TIMR_MODE_TIMER, 1, TStart, 1);
	
	TIMRG->HALLMD = (3 << TIMRG_HALLMD_IN0_Pos) |
					(3 << TIMRG_HALLMD_IN1_Pos) |
					(3 << TIMRG_HALLMD_IN2_Pos);	//����ͨ������˫���ش���
	TIMRG->HALLIE = 1;
	TIMRG->HALLIM = 0;
	NVIC_EnableIRQ(SPI1_HALL_GPIOC9_IRQn);
	
	TIMR_Start(TIMR0);
	
	while(1==1)
	{
		printf("Period = %d, HWidth = %d, Duty = %%%d\r\n", Period, Period-LWidth, (Period-LWidth)*100/Period);
		
		for(i = 0; i < 1000000; i++);
	}
}

void SPI1_HALL_GPIOC9_Handler(void)
{
	if(TIMRG->HALLSR & TIMRG_HALLIF_IN0_Msk)
	{
		if(TIMRG->HALLSR & TIMRG_HALLSR_IN0_Msk)		//������
		{
			LWidth = TStart > TIMRG->HALLV0 ? TStart - TIMRG->HALLV0 : (0xFFFFFF + TStart) - TIMRG->HALLV0;
		}
		else											//�½���
		{
			Period = TStart > TIMRG->HALLV0 ? TStart - TIMRG->HALLV0 : (0xFFFFFF + TStart) - TIMRG->HALLV0;
			
			TStart = TIMRG->HALLV0;
		}
		
		TIMRG->HALLSR = (1 << TIMRG_HALLIF_IN0_Pos);	//����жϱ�־
	}
}

void TestSignal(void)
{
	PWM_InitStructure PWM_initStruct;
	
	PWM_initStruct.PWMnXN = 1;					//ͬʱ���PWM0A��PWM0AN
	PWM_initStruct.clkdiv = 8;					//F_PWM = 24M/8 = 3M
	PWM_initStruct.cycle = 10000;				//3M/10000 = 300Hz��PWMnXN = 1ʱƵ�ʽ��͵�150Hz
	PWM_initStruct.hduty =  2500;				//2500/10000 = 25%
	PWM_initStruct.initLevel = 1;
	PWM_initStruct.HEndIE = 0;
	PWM_initStruct.NCycleIE = 0;
	PWM_initStruct.HCycleIE = 0;	
	PWM_Init(PWM0B, &PWM_initStruct);
	PWM_Init(PWM1A, &PWM_initStruct);
	
	PORT_Init(PORTD, PIN1,  PORTD_PIN1_PWM0A,  0);
	PORT_Init(PORTD, PIN0,  PORTD_PIN0_PWM0AN, 0);
	PORT_Init(PORTC, PIN15, PORTC_PIN15_PWM0B, 0);
	PORT_Init(PORTA, PIN5,  PORTA_PIN5_PWM0BN, 0);
	PORT_Init(PORTB, PIN14, PORTB_PIN14_PWM1A, 0);
	PORT_Init(PORTA, PIN4,  PORTA_PIN4_PWM1AN, 0);
	PORT_Init(PORTA, PIN9,  PORTA_PIN9_PWM1B,  0);
	PORT_Init(PORTA, PIN3,  PORTA_PIN3_PWM1BN, 0);
	
	PWMG->CHEN |= (1 << PWMG_CHEN_PWM0A_Pos) | (1 << PWMG_CHEN_PWM0B_Pos) | (1 << PWMG_CHEN_PWM1A_Pos) | (1 << PWMG_CHEN_PWM1B_Pos);	//��·ͬʱ����
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
