#include "SWM241.h"

#include "cmsis_os.h"


void TaskADC(void const *arg);
void TaskPWM(void const *arg);

osThreadId  taskADC;
osThreadDef(TaskADC, osPriorityLow,  1, 256);

osThreadId  taskPWM;
osThreadDef(TaskPWM, osPriorityHigh, 1, 256);


osMessageQId  queueADC;
osMessageQDef(queueADC, 16, uint32_t);


void SerialInit(void);

int main(void)
{	
	SystemInit();
	
	SerialInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);		//����ָʾ�ź�
	
	osKernelInitialize();
	
	taskADC = osThreadCreate(osThread(TaskADC), NULL);
	taskPWM = osThreadCreate(osThread(TaskPWM), NULL);
	
	queueADC = osMessageCreate(osMessageQ(queueADC), NULL);
	
	osKernelStart();
	
	while(1)
	{
		// do nothing
		
		osDelay(500);
	}
}

/****************************************************************************************************************************************** 
* ��������:	TaskADC()
* ����˵��: ����ADC�ɼ�����
* ��    ��: void const *arg		��������Ĳ���
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void TaskADC(void const *arg)
{
	ADC_InitStructure ADC_initStruct;
	
	PORT_Init(PORTA, PIN14, PORTA_PIN14_ADC_IN0, 0);//PA.14 => ADC.CH0
	PORT_Init(PORTA, PIN13, PORTA_PIN13_ADC_IN1, 0);//PA.13 => ADC.CH1
	
	ADC_initStruct.clk_src = ADC_CLKSRC_SYSCLK_DIV4;
	ADC_initStruct.clk_div = 6;
	ADC_initStruct.channels = ADC_CH1;
	ADC_initStruct.trig_src = ADC_TRIGSRC_TIMR2;
	ADC_initStruct.samplAvg = ADC_AVG_SAMPLE2;
	ADC_initStruct.Continue = 0;					//������ģʽ��������ģʽ
	ADC_initStruct.EOC_IEn = ADC_CH1;
	ADC_initStruct.OVF_IEn = 0;
	ADC_Init(ADC, &ADC_initStruct);					//����ADC
	
	ADC_Open(ADC);									//ʹ��ADC
	
	TIMR_Init(TIMR2, TIMR_MODE_TIMER, SystemCoreClock/10, 0);	//ÿ���Ӳ���10��
	TIMR_Start(TIMR2);
	
	while(1)
	{
		// do nothing
		
		osDelay(500);
	}
}

void ADC_Handler(void)
{	
	ADC_IntEOCClr(ADC, ADC_CH1);	//����жϱ�־
	
	osMessagePut(queueADC, ADC_Read(ADC, ADC_CH1), 0);
	
	GPIO_InvBit(GPIOA, PIN5);
}

/****************************************************************************************************************************************** 
* ��������:	TaskPWM()
* ����˵��: �ȴ�ADCת�����������ADCת���������PWMռ�ձ�
* ��    ��: void const *arg		��������Ĳ���
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void TaskPWM(void const *arg)
{
	PWM_InitStructure  PWM_initStruct;
	
	PWM_initStruct.clk_div = PWM_CLKDIV_4;		//F_PWM = 24M/4 = 6M
	
	PWM_initStruct.mode = PWM_MODE_INDEP;		//A·��B·�������					
	PWM_initStruct.cycleA = 10000;				//6M/10000 = 600Hz			
	PWM_initStruct.hdutyA =  2500;				//2500/10000 = 25%
	PWM_initStruct.initLevelA = 1;
	PWM_initStruct.cycleB = 10000;
	PWM_initStruct.hdutyB =  5000;				//5000/10000 = 50%
	PWM_initStruct.initLevelB = 1;
	PWM_initStruct.HEndAIEn = 0;
	PWM_initStruct.NCycleAIEn = 0;
	PWM_initStruct.HEndBIEn = 0;
	PWM_initStruct.NCycleBIEn = 0;
	
	PWM_Init(PWM0, &PWM_initStruct);
	
	PORT_Init(PORTA, PIN3,  PORTA_PIN3_PWM0A, 0);
	PORT_Init(PORTB, PIN2,  PORTB_PIN2_PWM0B, 0);
	
	PWM_Start(PWM0, 1, 1);
	
	while(1)
	{
		osEvent evt = osMessageGet(queueADC, 1);
		if(evt.status == osEventMessage)
		{
			uint16_t duty = evt.value.v;
			if(duty < 10)  duty = 10;
			if(duty > 500) duty = 500;
			
			PWM_SetHDuty(PWM0, PWM_CH_A, 10000 * duty / 512);
			PWM_SetHDuty(PWM0, PWM_CH_B, 10000 - PWM_GetHDuty(PWM0, PWM_CH_A));
		}
	}
}


void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTB, PIN11, PORTB_PIN11_UART0_RX, 1);	//GPIOB.11����ΪUART0��������
	PORT_Init(PORTB, PIN12, PORTB_PIN12_UART0_TX, 0);	//GPIOB.12����ΪUART0�������
 	
 	UART_initStruct.Baudrate = 57600;
	UART_initStruct.DataBits = UART_DATABIT_8;
	UART_initStruct.Parity = UART_PARITY_NONE;
	UART_initStruct.StopBits = UART_STOPBIT_1;
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
