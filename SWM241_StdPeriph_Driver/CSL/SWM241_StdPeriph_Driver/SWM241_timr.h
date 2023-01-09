#ifndef __SWM241_TIMR_H__
#define __SWM241_TIMR_H__


#define TIMR_MODE_TIMER		0
#define TIMR_MODE_COUNTER	2

#define TIMR_ICEDGE_BOTH	0
#define TIMR_ICEDGE_RISE	1
#define TIMR_ICEDGE_FALL	2


void TIMR_Init(TIMR_TypeDef * TIMRx, uint32_t mode, uint32_t clkdiv, uint32_t period, uint32_t int_en);	//定时器/计数器初始化
void TIMR_Start(TIMR_TypeDef * TIMRx);							//启动定时器，从初始值开始计时/计数
void TIMR_Stop(TIMR_TypeDef * TIMRx);							//停止定时器
void TIMR_Halt(TIMR_TypeDef * TIMRx);							//暂停定时器，计数值保持不变
void TIMR_Resume(TIMR_TypeDef * TIMRx);							//恢复定时器，从暂停处继续计数

void TIMR_SetPeriod(TIMR_TypeDef * TIMRx, uint32_t period);		//设置定时/计数周期
uint32_t TIMR_GetPeriod(TIMR_TypeDef * TIMRx);					//获取定时/计数周期
uint32_t TIMR_GetCurValue(TIMR_TypeDef * TIMRx);				//获取当前计数值

void TIMR_INTEn(TIMR_TypeDef * TIMRx);							//使能中断
void TIMR_INTDis(TIMR_TypeDef * TIMRx);							//禁能中断
void TIMR_INTClr(TIMR_TypeDef * TIMRx);							//清除中断标志
uint32_t TIMR_INTStat(TIMR_TypeDef * TIMRx);					//获取中断状态


void TIMR_OC_Init(TIMR_TypeDef * TIMRx, uint16_t match, uint32_t match_int_en, uint32_t init_lvl);

void TIMR_OC_OutputEn(TIMR_TypeDef * TIMRx);
void TIMR_OC_OutputDis(TIMR_TypeDef * TIMRx, uint32_t level);

void TIMR_OC_SetMatch(TIMR_TypeDef * TIMRx, uint16_t match);
uint16_t TIMR_OC_GetMatch(TIMR_TypeDef * TIMRx);

void TIMR_OC_INTEn(TIMR_TypeDef * TIMRx);
void TIMR_OC_INTDis(TIMR_TypeDef * TIMRx);
void TIMR_OC_INTClr(TIMR_TypeDef * TIMRx);
uint32_t TIMR_OC_INTStat(TIMR_TypeDef * TIMRx);


void TIMR_IC_Init(TIMR_TypeDef * TIMRx, uint32_t start_lvl, uint32_t captureH_int_en, uint32_t captureL_int_en);

uint32_t TIMR_IC_GetCaptureH(TIMR_TypeDef * TIMRx);
uint32_t TIMR_IC_GetCaptureL(TIMR_TypeDef * TIMRx);

void TIMR_IC_CaptureH_INTEn(TIMR_TypeDef * TIMRx);
void TIMR_IC_CaptureH_INTDis(TIMR_TypeDef * TIMRx);
void TIMR_IC_CaptureH_INTClr(TIMR_TypeDef * TIMRx);
uint32_t TIMR_IC_CaptureH_INTStat(TIMR_TypeDef * TIMRx);
void TIMR_IC_CaptureL_INTEn(TIMR_TypeDef * TIMRx);
void TIMR_IC_CaptureL_INTDis(TIMR_TypeDef * TIMRx);
void TIMR_IC_CaptureL_INTClr(TIMR_TypeDef * TIMRx);
uint32_t TIMR_IC_CaptureL_INTStat(TIMR_TypeDef * TIMRx);

#endif //__SWM241_TIMR_H__
