#include "SWM241.h"


int main(void)
{	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);			//�������LED
	
	SysTick_Config(SystemCoreClock/4);			//ÿ0.25���Ӵ���һ���жϣ�ע�⣺���ڼĴ�����24λ�ģ�����������Լ16_000_000
		
	while(1==1)
	{
	}
}

void SysTick_Handler(void)
{	
	GPIO_InvBit(GPIOA, PIN5);	//��תLED����״̬
}
