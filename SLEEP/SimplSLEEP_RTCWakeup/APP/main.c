#include "SWM241.h"

#define USE_XTAL32K 0

void XTAL32K_Enable(void);
RTC_AlarmStructure RTC_AlarmStruct;

int main(void)
{	
	uint32_t i;
	RTC_InitStructure RTC_initStruct;
	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN6, 1, 0, 0, 0);
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);				//接 LED，指示程序执行状态
	GPIO_SetBit(GPIOA, PIN5);						//点亮LED
	for(i = 0; i < SystemCoreClock/2; i++);			//延时，防止上电后SWD立即切掉无法下载程序
	
#if USE_XTAL32K  
  XTAL32K_Enable();
	RTC_initStruct.clksrc = RTC_CLKSRC_XTAL32K;  
#else
  RTC_initStruct.clksrc = RTC_CLKSRC_LRC32K;
#endif
	RTC_initStruct.Year = 2016;
	RTC_initStruct.Month = 5;
	RTC_initStruct.Date = 5;
	RTC_initStruct.Hour = 15;
	RTC_initStruct.Minute = 5;
	RTC_initStruct.Second = 5;
	RTC_initStruct.SecondIEn = 0;
	RTC_initStruct.MinuteIEn = 0;
	RTC_Init(RTC, &RTC_initStruct);
	
	RTC_AlarmStruct.Days = RTC_SUN | RTC_MON | RTC_TUE | RTC_WED | RTC_THU | RTC_FRI | RTC_SAT;
	RTC_AlarmStruct.Hour = RTC_initStruct.Hour;
	RTC_AlarmStruct.Minute = RTC_initStruct.Minute;
	RTC_AlarmStruct.Second = RTC_initStruct.Second + 5;
	RTC_AlarmStruct.AlarmIEn = 1;
	RTC_AlarmSetup(RTC, &RTC_AlarmStruct);
	
	RTC_Start(RTC);
	
	SYS->RTCWKCR = (1 << SYS_RTCWKCR_EN_Pos);
	
	while(1==1)
	{
		GPIO_SetBit(GPIOA, PIN5);					//点亮LED
		for(i = 0; i < SystemCoreClock/8; i++);
		GPIO_ClrBit(GPIOA, PIN5);					//熄灭LED
		
		EnterSleepMode();
		
		while((SYS->RTCWKSR & SYS_RTCWKSR_FLAG_Msk) == 0);	//等待唤醒条件
		SYS->RTCWKSR |= (1 << SYS_RTCWKSR_FLAG_Pos);		//清除唤醒状态
	}
}

void XTAL32K_Enable(void)
{
	uint32_t i;
	
  GPIO_Init(GPIOD, PIN0, 0, 0, 0, 0);
  GPIO_Init(GPIOA, PIN6, 0, 0, 0, 0);
  
	SYS->XTALCR |= (1 << SYS_XTALCR_32KON_Pos) | (7 << SYS_XTALCR_32KDRV_Pos);
	for(i = 0; i < 1000; i++) __NOP();
}

void RTC_GPIOD_Handler(void)
{
	RTC_DateTime dateTime;
	
	if(RTC_INTStat(RTC, RTC_IT_SECOND))
	{
		RTC_INTClr(RTC, RTC_IT_SECOND);
	}
	
	if(RTC_INTStat(RTC, RTC_IT_MINUTE))
	{
		RTC_INTClr(RTC, RTC_IT_MINUTE);
	}
	
	if(RTC_INTStat(RTC, RTC_IT_ALARM))
	{
		RTC_INTClr(RTC, RTC_IT_ALARM);
		
		RTC_GetDateTime(RTC, &dateTime);
		if(dateTime.Second < 55)
		{
			RTC_AlarmStruct.Second = dateTime.Second + 5;
		}
		else
		{
			RTC_AlarmStruct.Second = 0;
			RTC_AlarmStruct.Minute = dateTime.Minute + 1;
		}
		RTC_AlarmSetup(RTC, &RTC_AlarmStruct);
		
		GPIO_InvBit(GPIOA, PIN6);
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
