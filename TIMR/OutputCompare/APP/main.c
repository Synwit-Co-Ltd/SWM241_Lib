#include "SWM241.h"


int main(void)
{	
	SystemInit();
	
	PORT_Init(PORTB, PIN2,  PORTB_PIN2_TIMR2_OUT, 0);
	
	TIMR_Init(TIMR2, TIMR_MODE_OC, CyclesPerUs, 10000, 0);
	
	TIMR_OC_Init(TIMR2, 2500, 0, 0);				//Õ¼¿Õ±È2500/10000 = 25%
	
	TIMR_Start(TIMR2);
	
	while(1==1)
	{
	}
}
