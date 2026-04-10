#include "SWM241.h"


int main(void)
{	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);			//输出，接LED
	
	TIMR_Init(TIMR0, TIMR_MODE_TIMER, CyclesPerUs, 10000, 1);	//每10毫秒钟触发一次中断
	
	TIMR_Start(TIMR0);
	
	while(1==1)
	{
	}
}

void TIMR0_Handler(void)
{
	TIMR_INTClr(TIMR0, TIMR_IT_TO);
	
	GPIO_InvBit(GPIOA, PIN5);	//反转LED亮灭状态
}
