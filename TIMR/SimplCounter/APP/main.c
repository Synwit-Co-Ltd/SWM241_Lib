#include "SWM241.h"


int main(void)
{	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);
	
	PORT_Init(PORTB, PIN2, PORTB_PIN2_TIMR2_IN, 1);
	
	TIMR_Init(TIMR2, TIMR_MODE_COUNTER, 1, 3, 1);	//ÿ��4�����ؽ����жϣ�3�����彫����ֵ��3����0������һ���������ֵ����3��ͬʱ�����ж�
	
	TIMR_Start(TIMR2);
	
	while(1==1)
	{
	}
}

void TIMR2_Handler(void)
{
	TIMR_INTClr(TIMR2);
	
	GPIO_InvBit(GPIOA, PIN5);
}
