#include "SWM241.h"


int main(void)
{
	PWM_InitStructure  PWM_initStruct;
	
	SystemInit();
	
	PORT_Init(PORTD, PIN1,  PORTD_PIN1_PWM0A,  0);
	PORT_Init(PORTD, PIN0,  PORTD_PIN0_PWM0AN, 0);
	PORT_Init(PORTA, PIN9,  PORTA_PIN9_PWM0B,  0);
	PORT_Init(PORTA, PIN5,  PORTA_PIN5_PWM0BN, 0);
	PORT_Init(PORTA, PIN3,  PORTA_PIN3_PWM1A,  0);
	PORT_Init(PORTA, PIN4,  PORTA_PIN4_PWM1AN, 0);
	PORT_Init(PORTD, PIN15, PORTD_PIN15_PWM1B, 0);
	PORT_Init(PORTD, PIN14, PORTD_PIN14_PWM1BN,0);
	
	PWM_initStruct.PWMnXN = 1;					//同时输出PWM0A和PWM0AN
	PWM_initStruct.clkdiv = 8;					//F_PWM = 48M/8 = 6M
	PWM_initStruct.cycle = 10000;				//6M/10000 = 600Hz，PWMnXN = 1时频率降低到300Hz
	PWM_initStruct.hduty =  2500;				//2500/10000 = 25%
    PWM_initStruct.deadzone = 0;
	PWM_initStruct.initLevel = 1;
	PWM_initStruct.HEndIE = 0;
	PWM_initStruct.NCycleIE = 0;
	PWM_initStruct.HCycleIE = 0;
	PWM_Init(PWM0A, &PWM_initStruct);
	PWM_Init(PWM0B, &PWM_initStruct);
	PWM_Init(PWM1A, &PWM_initStruct);
	PWM_Init(PWM1B, &PWM_initStruct);
	
	PWMG->CHEN |= (1 << PWMG_CHEN_PWM0A_Pos) | (1 << PWMG_CHEN_PWM0B_Pos) | (1 << PWMG_CHEN_PWM1A_Pos) | (1 << PWMG_CHEN_PWM1B_Pos);
// 	PWM_Start(PWM0A);
// 	PWM_Start(PWM0B);
// 	PWM_Start(PWM1A);
// 	PWM_Start(PWM1B);
	
	while(1==1)
	{
	
	}
}
