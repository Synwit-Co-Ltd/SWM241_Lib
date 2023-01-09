#include "SWM241.h"


int main(void)
{	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);			//输出，接LED
	
	SYS->BODCR = (1 << SYS_BODCR_IE_Pos) |
				 (0 << SYS_BODCR_INTLVL_Pos);	//2.7V产生中断
	
	NVIC_EnableIRQ(BOD_PWMBRK_GPIOC7_IRQn);
		
	while(1==1)
	{
		GPIO_ClrBit(GPIOA, PIN5);
	}
}

void BOD_PWMBRK_GPIOC7_Handler(void)
{
	GPIO_SetBit(GPIOA, PIN5);
}
