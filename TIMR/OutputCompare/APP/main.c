#include "SWM241.h"


int main(void)
{	
	SystemInit();
	
	PORT_Init(PORTB, PIN2,  PORTB_PIN2_TIMR2_OUT, 0);
	PORT_Init(PORTB, PIN14, PORTB_PIN14_TIMR2_OUT, 0);
	
	TIMR_Init(TIMR2, TIMR_MODE_TIMER, 1, 10000, 0);	//输出波形时TIMR只有16位，频率为24MHz/10000 = 2.4KHz
	
	TIMR_OC_Init(TIMR2, 2500, 0, 0);				//占空比2500/10000 = 25%
	
	TIMR_Start(TIMR2);
	
	while(1==1)
	{
	}
}
