/****************************************************************************************************************************************** 
* 文件名称: SWM241_wdt.c
* 功能说明:	SWM241单片机的WDT看门狗功能驱动库
* 技术支持:	http://www.synwit.com.cn/e/tool/gbook/?bid=1
* 注意事项:
* 版本日期:	V1.0.0		2016年1月30日
* 升级记录:  
*
*
*******************************************************************************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION 
* REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE 
* FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT 
* OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
* -ECTION WITH THEIR PRODUCTS.
*
* COPYRIGHT 2012 Synwit Technology
*******************************************************************************************************************************************/
#include "SWM241.h"
#include "SWM241_wdt.h"


/****************************************************************************************************************************************** 
* 函数名称:	WDT_Init()
* 功能说明:	WDT看门狗初始化
* 输    入: WDT_TypeDef * WDTx		指定要被设置的看门狗，有效值包括WDT
*			WDT_InitStructure * initStruct	包含WDT相关设定值的结构体
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void WDT_Init(WDT_TypeDef * WDTx, WDT_InitStructure * initStruct)
{
	SYS->CLKEN0 |= (1 << SYS_CLKEN0_WDT_Pos);
	
	WDT_Stop(WDTx);		//设置前先关闭
	
	switch(initStruct->clksrc)
	{
	case WDT_CLKSRC_HSI:
		SYS->HRCCR |= (1 << SYS_HRCCR_ON_Pos); 
		break;
	
	case WDT_CLKSRC_LSI:
		SYS->LRCCR |= (1 << SYS_LRCCR_ON_Pos);
		break;
	}
	
	SYS->CLKSEL &= ~SYS_CLKSEL_WDT_Msk;
	SYS->CLKSEL |= (initStruct->clksrc << SYS_CLKSEL_WDT_Pos);
	
	WDTx->CR = (initStruct->rst_en << WDT_CR_RSTEN_Pos) |
			   (initStruct->int_en << WDT_CR_INTEN_Pos) |
			   (initStruct->win_en << WDT_CR_WINEN_Pos) |
			   (initStruct->clkdiv << WDT_CR_CLKDIV_Pos);
	
	WDTx->INTVAL = initStruct->int_period;
	
	WDTx->RSTVAL = initStruct->rst_period;
	
	WDTx->IF = 0;
	if(initStruct->int_en) NVIC_EnableIRQ(WDT_IRQn);
}

/****************************************************************************************************************************************** 
* 函数名称:	WDT_Start()
* 功能说明:	启动指定WDT，开始倒计时
* 输    入: WDT_TypeDef * WDTx		指定要被设置的看门狗，有效值包括WDT
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void WDT_Start(WDT_TypeDef * WDTx)
{
	WDTx->CR |=  (1 << WDT_CR_EN_Pos);
}

/****************************************************************************************************************************************** 
* 函数名称:	WDT_Stop()
* 功能说明:	关闭指定WDT，停止倒计时
* 输    入: WDT_TypeDef * WDTx		指定要被设置的看门狗，有效值包括WDT
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void WDT_Stop(WDT_TypeDef * WDTx)
{
	WDTx->CR &= ~(1 << WDT_CR_EN_Pos);
}

/****************************************************************************************************************************************** 
* 函数名称:	WDT_Feed()
* 功能说明:	喂狗，重新从装载值开始倒计时
* 输    入: WDT_TypeDef * WDTx		指定要被设置的看门狗，有效值包括WDT
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void WDT_Feed(WDT_TypeDef * WDTx)
{
	WDTx->FEED = 0x55;
}

/****************************************************************************************************************************************** 
* 函数名称:	WDT_INTClr()
* 功能说明:	中断标志清除
* 输    入: WDT_TypeDef * WDTx		指定要被设置的看门狗，有效值包括WDT
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void WDT_INTClr(WDT_TypeDef * WDTx)
{
	WDTx->IF = 1;
}

/****************************************************************************************************************************************** 
* 函数名称:	WDT_INTStat()
* 功能说明:	中断状态查询
* 输    入: WDT_TypeDef * WDTx		指定要被设置的看门狗，有效值包括WDT
* 输    出: int32_t					1 发生中断溢出    0 未发生中断溢出
* 注意事项: 无
******************************************************************************************************************************************/
uint32_t WDT_INTStat(WDT_TypeDef * WDTx)
{
	return WDTx->IF;
}
