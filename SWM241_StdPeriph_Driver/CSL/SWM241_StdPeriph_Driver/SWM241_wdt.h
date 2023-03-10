#ifndef __SWM241_WDT_H__
#define	__SWM241_WDT_H__

typedef struct {
	uint8_t  clksrc;		// WDT_CLKSRC_HSI、WDT_CLKSRC_LSI
	uint8_t  clkdiv;		// 时钟分频，系统时钟经此分频后产生WDT计数时钟
	uint16_t int_period;	// WDT 中断周期
	uint8_t  int_en;		// WDT 中断使能
	uint16_t rst_period;	// WDT 复位周期
	uint8_t  rst_en;		// WDT 复位使能
	uint8_t  win_en;		// WDT 窗口功能使能，开启窗口功能后只能在中断到来后、复位到来前喂狗，在中断到来前喂狗会导致立即复位
} WDT_InitStructure;

void WDT_Init(WDT_TypeDef * WDTx, WDT_InitStructure * initStruct);
void WDT_Start(WDT_TypeDef * WDTx);			//启动指定WDT，开始倒计时
void WDT_Stop(WDT_TypeDef * WDTx);			//关闭指定WDT，停止倒计时

void WDT_Feed(WDT_TypeDef * WDTx);			//喂狗，重新从装载值开始倒计时


void WDT_INTClr(WDT_TypeDef * WDTx);		//中断标志清除
uint32_t WDT_INTStat(WDT_TypeDef * WDTx);	//中断状态查询
 
#endif //__SWM241_WDT_H__
