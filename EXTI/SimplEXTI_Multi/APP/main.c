#include "SWM241.h"

void SerialInit(void);

int main(void)
{	
	SystemInit();
	
	SerialInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);			//�������LED
	
	GPIO_Init(GPIOC, PIN0, 0, 1, 0, 0);
	EXTI_Init(GPIOC, PIN0, EXTI_FALL_EDGE);
	EXTI_Open(GPIOC, PIN0);
	
	GPIO_Init(GPIOC, PIN1, 0, 1, 0, 0);
	EXTI_Init(GPIOC, PIN1, EXTI_FALL_EDGE);
	EXTI_Open(GPIOC, PIN1);
	
	GPIO_Init(GPIOC, PIN2, 0, 1, 0, 0);
	EXTI_Init(GPIOC, PIN2, EXTI_FALL_EDGE);
	EXTI_Open(GPIOC, PIN2);
	
	NVIC_EnableIRQ(PWM3_GPIOC_SAFETY_IRQn);
	
	while(1==1)
	{
	}
}

void PWM3_GPIOC_SAFETY_Handler(void)
{
	if(EXTI_State(GPIOC, PIN0))
	{
		EXTI_Clear(GPIOC, PIN0);
		
		GPIO_InvBit(GPIOA, PIN5);
	}
	else if(EXTI_State(GPIOC, PIN1))
	{
		EXTI_Clear(GPIOC, PIN1);
		
		GPIO_InvBit(GPIOA, PIN5);
	}
	else if(EXTI_State(GPIOC, PIN2))
	{
		EXTI_Clear(GPIOC, PIN2);
		
		GPIO_InvBit(GPIOA, PIN5);
	}
	else 
	{
		printf("Error!\r\n");
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
