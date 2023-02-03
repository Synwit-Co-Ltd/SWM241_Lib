#include "SWM241.h"


void SerialInit(void);
void RTC_Calibrate(void);
void RTC_Alarm_Config(void);

int main(void)
{
	uint32_t i;
	RTC_DateTime dateTime;
	RTC_InitStructure RTC_initStruct;
		
	SystemInit();
	
	SerialInit();
	
	RTC_initStruct.Year = 2016;
	RTC_initStruct.Month = 5;
	RTC_initStruct.Date = 5;
	RTC_initStruct.Hour = 15;
	RTC_initStruct.Minute = 5;
	RTC_initStruct.Second = 5;
	RTC_initStruct.SecondIEn = 0;
	RTC_initStruct.MinuteIEn = 0;
	RTC_Init(RTC, &RTC_initStruct);
	
	RTC_Start(RTC);
	
	RTC_Calibrate();	// 片内32KHz时钟校准
	
	RTC_Alarm_Config();	// 5秒钟定时
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);	
	
	while(1==1)
	{
		RTC_GetDateTime(RTC, &dateTime);
		printf("Now Time: %d Minute %d Second\r\n", dateTime.Minute, dateTime.Second);
		
		for(i = 0; i < 10000000; i++);
	}
}

void RTC_Calibrate(void)
{
	PORT_Init(PORTD, PIN2, PORTD_PIN2_XTAL_IN,  0);
	PORT_Init(PORTD, PIN3, PORTD_PIN3_XTAL_OUT, 0);
	
	SYS->CLKEN0 |= (1 << SYS_CLKEN0_ANAC_Pos);
	SYS->XTALCR |= (1 << SYS_XTALCR_ON_Pos) | (7 << SYS_XTALCR_DRV_Pos);		// 使用外部晶振用作校准基准时钟，因此需要开启
	
	SYS->CLKSEL &= ~SYS_CLKSEL_RTC_Msk; 
	SYS->CLKSEL |= (3 << SYS_CLKSEL_RTC_Pos);	// 校准基准时钟取值范围2.2~4.1MHz，板上外部晶振24MHz，因此只能用XTAL/8用作校准基准时钟
	
	RTC->CALIBREFCNT = (24000000 / 8) / 2;		// 校准基准时钟频率的一半
	RTC->CALIBEN     = 1;						// 必须启动RTC的情况下启动校准
}

void RTC_Alarm_Config(void)
{
	RTC_DateTime dateTime;
	RTC_AlarmStructure RTC_alarmStruct;
	
	RTC_GetDateTime(RTC, &dateTime);
	
	if(dateTime.Second + 5 < 60)
	{
		RTC_alarmStruct.Second = dateTime.Second + 5;
		RTC_alarmStruct.Minute = dateTime.Minute;
		RTC_alarmStruct.Hour = dateTime.Hour;
	}
	else
	{
		RTC_alarmStruct.Second = dateTime.Second + 5 - 60;
		
		if(dateTime.Minute + 1 < 60)
		{
			RTC_alarmStruct.Minute = dateTime.Minute + 1;
			RTC_alarmStruct.Hour = dateTime.Hour;
		}
		else
		{
			RTC_alarmStruct.Minute = 0;
			RTC_alarmStruct.Hour = dateTime.Hour + 1;
		}
	}
	RTC_alarmStruct.Days = RTC_SUN|RTC_MON|RTC_TUE|RTC_WED|RTC_THU|RTC_FRI|RTC_SAT;
	RTC_alarmStruct.AlarmIEn = 1;
	
	RTC_AlarmSetup(RTC, &RTC_alarmStruct);
}

void RTC_Handler(void)
{
	if(RTC_IntAlarmStat(RTC))
	{
		RTC_IntAlarmClr(RTC);
		
		RTC_Alarm_Config();
		
		GPIO_InvBit(GPIOA, PIN5);	// 每5秒钟翻转一次
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
