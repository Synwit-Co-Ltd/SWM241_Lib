/****************************************************************************************************************************************** 
* 文件名称:	SWM241_sled.c
* 功能说明:	SWM241单片机的Segment LED功能驱动库
* 技术支持: http://www.synwit.com.cn/e/tool/gbook/?bid=1
* 注意事项:
* 版本日期: V1.0.0		2016年1月30日
* 升级记录:  
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
#include "SWM241_sled.h"


/****************************************************************************************************************************************** 
* 函数名称: SLED_Init()
* 功能说明:	Segment LED模块初始化
* 输    入: SLED_TypeDef * SLEDx			指定要设置的SLED模块，有效值包括SLED
*			SLED_InitStructure * initStruct	包含SLED相关设定值的结构体
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void SLED_Init(SLED_TypeDef * SLEDx, SLED_InitStructure * initStruct)
{
	SYS->CLKEN0 |= (0x01u << SYS_CLKEN0_SLED_Pos);
	
	SLEDx->CR &= ~(SLED_CR_DUTY_Msk | SLED_CR_COMINV_Msk);
	SLEDx->CR |= (initStruct->duty   << SLED_CR_DUTY_Pos) |
				 (initStruct->cominv << SLED_CR_COMINV_Pos);
	
	SLEDx->CLKDIV = initStruct->clkdiv;
	
	SLEDx->TIM = (((initStruct->period > 2) ? (initStruct->period - 1) : 2) << SLED_TIM_PERIOD_Pos) |
				 (((initStruct->high > 1) ? (initStruct->high - 1) : 1) << SLED_TIM_HIGH_Pos);
}

/****************************************************************************************************************************************** 
* 函数名称: SLED_Start()
* 功能说明:	Segment LED模块开始工作
* 输    入: SLED_TypeDef * SLEDx		指定要设置的SLED模块，有效值包括SLED
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void SLED_Start(SLED_TypeDef * SLEDx)
{
	SLEDx->CR |= (1 << SLED_CR_EN_Pos);
}

/****************************************************************************************************************************************** 
* 函数名称: SLED_Stop()
* 功能说明:	Segment LED模块停止工作
* 输    入: SLED_TypeDef * SLEDx		指定要设置的SLED模块，有效值包括SLED
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void SLED_Stop(SLED_TypeDef * SLEDx)
{
	SLEDx->CR &= ~(1 << SLED_CR_EN_Pos);
}
