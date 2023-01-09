#include "SWM241.h"

void SerialInit(void);
void TestSignal(void);

int main(void)
{	
	SystemInit();
	
	SerialInit();
	
	TestSignal();	//产生测试信号供Input Capture功能测量
	
	PORT_Init(PORTB, PIN2, PORTB_PIN2_TIMR2_IN, 1);
	
	TIMR_Init(TIMR2, TIMR_MODE_TIMER, 1, 0xFFFFFF, 0);
	
	TIMR_IC_Init(TIMR2, TIMR_ICEDGE_BOTH, 1, 1);
		
	while(1==1)
	{
	}
}

void TestSignal(void)
{
	PWM_InitStructure PWM_initStruct;
	
	PWM_initStruct.clkdiv = 8;					//F_PWM = 24M/8 = 3M
	PWM_initStruct.PWMnXN = 1;					//同时输出PWM0A和PWM0AN
	PWM_initStruct.cycle = 10000;				//3M/10000 = 300Hz，PWMnXN = 1时频率降低到150Hz
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
	
	PWMG->CHEN |= (1 << PWMG_CHEN_PWM0A_Pos) | (1 << PWMG_CHEN_PWM0B_Pos) | (1 << PWMG_CHEN_PWM1A_Pos) | (1 << PWMG_CHEN_PWM1B_Pos);	//多路同时启动
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
