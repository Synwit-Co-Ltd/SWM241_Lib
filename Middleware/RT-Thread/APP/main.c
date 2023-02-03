#include "SWM241.h"

#include "rtthread.h"


void TaskADC(void *arg);
void TaskPWM(void *arg);

struct rt_thread taskADC;
static uint8_t stackADC[256];
static uint8_t priorityADC = 4;

struct rt_thread taskPWM;
static uint8_t stackPWM[256];
static uint8_t priorityPWM = 5;

struct rt_mailbox queueADC;
static uint32_t mbPool[16];


void SerialInit(void);

int main(void)
{	
	//	SystemInit();		在 rt_hw_board_init() 中调用
	
	SerialInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);		//调试指示信号
	
	rt_mb_init(&queueADC, "ADC", mbPool, sizeof(mbPool)/sizeof(mbPool[0]), RT_IPC_FLAG_FIFO);
		
	rt_thread_init(&taskADC, "ADC", TaskADC, RT_NULL, stackADC, sizeof(stackADC), priorityADC, 20);
	rt_thread_startup(&taskADC);
	
	rt_thread_init(&taskPWM, "PWM", TaskPWM, RT_NULL, stackPWM, sizeof(stackPWM), priorityPWM, 20);
	rt_thread_startup(&taskPWM);
		
	while(1)
	{		
		// do nothing
		
		rt_thread_delay(500 / (1000 / RT_TICK_PER_SECOND));
	}
}

/****************************************************************************************************************************************** 
* 函数名称:	TaskADC()
* 功能说明: 启动ADC采集任务
* 输    入: void *arg		传给任务的参数
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void TaskADC(void *arg)
{
	ADC_InitStructure ADC_initStruct;
	
	PORT_Init(PORTA, PIN14, PORTA_PIN14_ADC_IN0, 0);//PA.14 => ADC.CH0
	PORT_Init(PORTA, PIN13, PORTA_PIN13_ADC_IN1, 0);//PA.13 => ADC.CH1
	
	ADC_initStruct.clk_src = ADC_CLKSRC_SYSCLK_DIV4;
	ADC_initStruct.clk_div = 6;
	ADC_initStruct.channels = ADC_CH1;
	ADC_initStruct.trig_src = ADC_TRIGSRC_TIMR2;
	ADC_initStruct.samplAvg = ADC_AVG_SAMPLE2;
	ADC_initStruct.Continue = 0;					//非连续模式，即单次模式
	ADC_initStruct.EOC_IEn = ADC_CH1;
	ADC_initStruct.OVF_IEn = 0;
	ADC_Init(ADC, &ADC_initStruct);					//配置ADC
	
	ADC_Open(ADC);									//使能ADC
	
	TIMR_Init(TIMR2, TIMR_MODE_TIMER, SystemCoreClock/10, 0);	//每秒钟采样10次
	TIMR_Start(TIMR2);
	
	while(1)
	{
		// do nothing
		
		rt_thread_delay(500 / (1000 / RT_TICK_PER_SECOND));
	}
}

void ADC_Handler(void)
{	
	ADC_IntEOCClr(ADC, ADC_CH1);	//清除中断标志
	
	rt_mb_send(&queueADC, ADC_Read(ADC, ADC_CH1));
	
	GPIO_InvBit(GPIOA, PIN5);
}

/****************************************************************************************************************************************** 
* 函数名称:	TaskPWM()
* 功能说明: 等待ADC转换结果，根据ADC转换结果设置PWM占空比
* 输    入: void *arg		传给任务的参数
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void TaskPWM(void *arg)
{
	rt_uint32_t duty;
	PWM_InitStructure  PWM_initStruct;
	
	PWM_initStruct.clk_div = PWM_CLKDIV_4;		//F_PWM = 24M/4 = 6M
	
	PWM_initStruct.mode = PWM_MODE_INDEP;		//A路和B路独立输出					
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
		if(rt_mb_recv(&queueADC, &duty, 100) == RT_EOK)
		{
			if(duty <  10) duty =  10;
			if(duty > 500) duty = 500;
			
			PWM_SetHDuty(PWM0, PWM_CH_A, 10000 * duty / 512);
			PWM_SetHDuty(PWM0, PWM_CH_B, 10000 - PWM_GetHDuty(PWM0, PWM_CH_A));
		}
	}
}


void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTD, PIN13, PORTD_PIN13_UART0_RX, 1);	//GPIOD.13配置为UART0输入引脚
	PORT_Init(PORTD, PIN14, PORTD_PIN14_UART0_TX, 0);	//GPIOD.14配置为UART0输出引脚
 	
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
