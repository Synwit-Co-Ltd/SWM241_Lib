#include "SWM241.h"

#include "ucos_ii.h"

OS_EVENT *queueADC;                          
void *queueADCTbl[16];

static  OS_STK TaskADCStk[TASK_ADC_STK_SIZE];
static  OS_STK TaskPWMStk[TASK_PWM_STK_SIZE];

void TaskADC(void *arg);
void TaskPWM(void *arg);
void SerialInit(void);

int main(void)
{ 	
 	SystemInit();
	
	SerialInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);		//����ָʾ�ź�
	
	OSInit();
	
	queueADC = OSQCreate(&queueADCTbl[0], 16);
	
	OSTaskCreate((void (*)(void *))TaskADC, (void *)0, &TaskADCStk[TASK_ADC_STK_SIZE - 1], TASK_LED_PRIO);
	OSTaskCreate((void (*)(void *))TaskPWM, (void *)0, &TaskPWMStk[TASK_PWM_STK_SIZE - 1], TASK_COM_PRIO);
	
	OS_CPU_SysTickInit(SystemCoreClock / OS_TICKS_PER_SEC);
	OSStart();
}

/****************************************************************************************************************************************** 
* ��������:	TaskADC()
* ����˵��: ����ADC�ɼ�����
* ��    ��: void *arg
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void TaskADC(void *arg)
{
	ADC_InitStructure ADC_initStruct;
	
	PORT_Init(PORTA, PIN14, PORTA_PIN14_ADC_IN0, 0);//PA.14 => ADC.CH0
	PORT_Init(PORTA, PIN13, PORTA_PIN13_ADC_IN1, 0);//PA.13 => ADC.CH1
	
	ADC_initStruct.clk_src = ADC_CLKSRC_HRC;
	ADC_initStruct.clk_div = 12;
	ADC_initStruct.ref_src = ADC_REFSRC_VREFP;
	ADC_initStruct.channels = ADC_CH1;
	ADC_initStruct.trig_src = ADC_TRIGSRC_TIMR2;
	ADC_initStruct.samplAvg = ADC_AVG_SAMPLE2;
	ADC_initStruct.Continue = 0;					//������ģʽ��������ģʽ
	ADC_initStruct.EOC_IEn = ADC_CH1;
	ADC_initStruct.OVF_IEn = 0;
	ADC_Init(ADC, &ADC_initStruct);					//����ADC
	
	ADC_Open(ADC);									//ʹ��ADC
	
	TIMR_Init(TIMR2, TIMR_MODE_TIMER, CyclesPerUs, 100000, 0);	//ÿ���Ӳ���10��
	TIMR_Start(TIMR2);
	
	while(1)
	{
		// do nothing
		
		OSTimeDly(OS_TICKS_PER_SEC/10);
	}
}

void ADC_Handler(void)
{	
	uint32_t val;
	
	ADC_IntEOCClr(ADC, ADC_CH1);	//����жϱ�־
	
	val = ADC_Read(ADC, ADC_CH1);
	
	OSQPost(queueADC, (void *)val);
	
	GPIO_InvBit(GPIOA, PIN5);
}

/****************************************************************************************************************************************** 
* ��������:	TaskPWM()
* ����˵��: �ȴ�ADCת�����������ADCת���������PWMռ�ձ�
* ��    ��: void *arg
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void TaskPWM(void *arg)
{
	uint8_t err;
	uint32_t duty;
	PWM_InitStructure  PWM_initStruct;
	
	PWM_initStruct.PWMnXN = 1;					//ͬʱ���PWM0A��PWM0AN
	PWM_initStruct.clkdiv = 8;					//F_PWM = 24M/8 = 3M
	PWM_initStruct.cycle = 10000;				//3M/10000 = 300Hz��PWMnXN = 1ʱƵ�ʽ��͵�150Hz
	PWM_initStruct.hduty =  2500;				//2500/10000 = 25%
	PWM_initStruct.initLevel = 1;
	PWM_initStruct.HEndIE = 0;
	PWM_initStruct.NCycleIE = 0;
	PWM_initStruct.HCycleIE = 0;
	PWM_Init(PWM0A, &PWM_initStruct);
	PWM_Init(PWM0B, &PWM_initStruct);
	PWM_Init(PWM1A, &PWM_initStruct);
	PWM_Init(PWM1B, &PWM_initStruct);
	
	PORT_Init(PORTD, PIN1,  PORTD_PIN1_PWM0A,  0);
	PORT_Init(PORTD, PIN0,  PORTD_PIN0_PWM0AN, 0);
	PORT_Init(PORTA, PIN9,  PORTA_PIN9_PWM0B,  0);
	PORT_Init(PORTA, PIN5,  PORTA_PIN5_PWM0BN, 0);
	PORT_Init(PORTA, PIN3,  PORTA_PIN3_PWM1A,  0);
	PORT_Init(PORTA, PIN4,  PORTA_PIN4_PWM1AN, 0);
	PORT_Init(PORTD, PIN15, PORTD_PIN15_PWM1B, 0);
	PORT_Init(PORTD, PIN14, PORTD_PIN14_PWM1BN,0);
	
	PWM_Start(PWM0A);
	PWM_Start(PWM0B);
	PWM_Start(PWM1A);
	PWM_Start(PWM1B);
	
	while(1)
	{
		duty = (uint32_t)OSQPend(queueADC, 10, &err);
		if(err == OS_ERR_NONE)
		{
			if(duty <  10) duty =  10;
			if(duty > 500) duty = 500;
			
			PWM_SetHDuty(PWM0A, 10000 * duty / 512);
			PWM_SetHDuty(PWM0B, 10000 - PWM_GetHDuty(PWM0A));
		}
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
