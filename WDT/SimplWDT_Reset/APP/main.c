#include "SWM241.h"


int main(void)
{
	uint32_t i;

	SystemInit();
	
	for(i=0;i<6000000;i++);						//复位后等一会儿再点亮LED
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);			//输出，接LED
	GPIO_SetBit(GPIOA, PIN5);					//点亮LED
	
	WDT_Init(WDT, 0, 1000);
	WDT_Start(WDT);								//启动WDT
	
	while(1==1)
	{
		
	}
}
