#include "SWM241.h"


int main(void)
{
	uint32_t i;

	SystemInit();
	
	for(i=0;i<6000000;i++);						//��λ���һ����ٵ���LED
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);			//�������LED
	GPIO_SetBit(GPIOA, PIN5);					//����LED
	
	WDT_Init(WDT, 0, 1000);
	WDT_Start(WDT);								//����WDT
	
	while(1==1)
	{
		
	}
}
