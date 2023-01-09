#include "SWM241.h"


int main(void)
{
	uint32_t i;
	WDT_InitStructure WDT_initStruct;

	SystemInit();
	
	for(i=0;i<6000000;i++);						//��λ���һ����ٵ���LED
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);			//�������LED
	GPIO_SetBit(GPIOA, PIN5);					//����LED
	
	WDT_initStruct.clksrc = WDT_CLKSRC_HSI;
	WDT_initStruct.clkdiv = WDT_CLKDIV_32768;
	WDT_initStruct.int_period = 100;			//Լ100ms
	WDT_initStruct.int_en = 0;
	WDT_initStruct.rst_period = 500;			//Լ500ms
	WDT_initStruct.rst_en = 1;
	WDT_initStruct.win_en = 0;
	WDT_Init(WDT, &WDT_initStruct);
	
	WDT_Start(WDT);								//����WDT
	
	while(1==1)
	{
		
	}
}
