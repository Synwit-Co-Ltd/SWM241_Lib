/****************************************************************************************************************************************** 
* 文件名称:	SWM241_safety.c
* 功能说明:	SWM241单片机的安全功能驱动库
* 技术支持:	http://www.synwit.com.cn/e/tool/gbook/?bid=1
* 注意事项:
* 版本日期: V1.0.0		2016年1月30日
* 升级记录: 
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
#include "SWM241_safety.h"


/****************************************************************************************************************************************** 
* 函数名称: SAFETY_PerWP_Open()
* 功能说明:	外设写保护开启
* 输    入: uint32_t peripherals	SAFETY_WP_IER、SAFETY_WP_IOCFGR、SAFETY_WP_CLKCFGR、SAFETY_WP_ANACFGR 及其 按位或
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void SAFETY_PerWP_Open(uint32_t peripherals)
{
	SAFETY->PERWP |= (peripherals << SAFETY_PERWP_IER_Pos);
}

/****************************************************************************************************************************************** 
* 函数名称: SAFETY_PerWP_Close()
* 功能说明:	外设写保护关闭
* 输    入: uint32_t peripherals	SAFETY_WP_IER、SAFETY_WP_IOCFGR、SAFETY_WP_CLKCFGR、SAFETY_WP_ANACFGR 及其 按位或
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void SAFETY_PerWP_Close(uint32_t peripherals)
{
	SAFETY->PERWP &= ~(peripherals << SAFETY_PERWP_IER_Pos);
}

/****************************************************************************************************************************************** 
* 函数名称: SAFETY_RAMWP_Open()
* 功能说明:	RAM 写保护开启
* 输    入: uint32_t addr	需要写保护的RAM的起始地址
*			uint32_t size	需要写保护的RAM的大小，可取值：SAFETY_SIZE_256B、SAFETY_SIZE_256B、SAFETY_SIZE_1024B
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void SAFETY_RAMWP_Open(uint32_t addr, uint32_t size)
{
	addr = (addr - 0x20000000) / 256;
	
	SAFETY->RAMWP = (addr << SAFETY_RAMWP_ADDR_Pos) |
					(size << SAFETY_RAMWP_SIZE_Pos);
}

/****************************************************************************************************************************************** 
* 函数名称: SAFETY_RAMWP_Close()
* 功能说明:	RAM 写保护关闭
* 输    入: 无
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void SAFETY_RAMWP_Close(void)
{
	SAFETY->RAMWP = 0;
}

/****************************************************************************************************************************************** 
* 函数名称: SAFETY_IAAD_Open()
* 功能说明:	非法地址访问检测（Illegal Address Access Detection）开启
* 输    入: 无
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void SAFETY_IAAD_Open(void)
{
	SAFETY->PERWP |= (1 << SAFETY_PERWP_IAADEN_Pos);
}

/****************************************************************************************************************************************** 
* 函数名称: SAFETY_IAAD_Close()
* 功能说明:	非法地址访问检测（Illegal Address Access Detection）关闭
* 输    入: 无
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void SAFETY_IAAD_Close(void)
{
	SAFETY->PERWP &= ~(1 << SAFETY_PERWP_IAADEN_Pos);
}

/****************************************************************************************************************************************** 
* 函数名称: SAFETY_IAAD_Close()
* 功能说明:	非法地址访问检测（Illegal Address Access Detection）区域配置
* 输    入: uint32_t region		想要配置的区域，可取值：SAFETY_REGION_0、SAFETY_REGION_1、SAFETY_REGION_2、SAFETY_REGION_3
*			SAFETY_Region_InitStructure * initStruct	区域的配置值
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void SAFETY_IAADRegion_Config(uint32_t region, SAFETY_Region_InitStructure * initStruct)
{
	SAFETY->REGION[region].BADDR = initStruct->bottom_addr;
	SAFETY->REGION[region].TADDR = initStruct->top_addr;
	
	switch(region)
	{
	case SAFETY_REGION_0:
		SAFETY->IAACR &= ~SAFETY_IAACR_R0INT_Msk;
		SAFETY->IAACR |= (initStruct->action << SAFETY_IAACR_R0INT_Pos);
	
		SAFETY->IF = (1 << SAFETY_IF_R0_Pos);
		if(initStruct->inten)
			SAFETY->IE |=  (1 << SAFETY_IE_R0_Pos);
		else
			SAFETY->IE &= ~(1 << SAFETY_IE_R0_Pos);
		break;
	
	case SAFETY_REGION_1:
		SAFETY->IAACR &= ~SAFETY_IAACR_R1INT_Msk;
		SAFETY->IAACR |= (initStruct->action << SAFETY_IAACR_R1INT_Pos);
	
		SAFETY->IF = (1 << SAFETY_IF_R1_Pos);
		if(initStruct->inten)
			SAFETY->IE |=  (1 << SAFETY_IE_R1_Pos);
		else
			SAFETY->IE &= ~(1 << SAFETY_IE_R1_Pos);
		break;
	
	case SAFETY_REGION_2:
		SAFETY->IAACR &= ~SAFETY_IAACR_R2INT_Msk;
		SAFETY->IAACR |= (initStruct->action << SAFETY_IAACR_R2INT_Pos);
	
		SAFETY->IF = (1 << SAFETY_IF_R2_Pos);
		if(initStruct->inten)
			SAFETY->IE |=  (1 << SAFETY_IE_R2_Pos);
		else
			SAFETY->IE &= ~(1 << SAFETY_IE_R2_Pos);
		break;
	
	case SAFETY_REGION_3:
		SAFETY->IAACR &= ~SAFETY_IAACR_R3INT_Msk;
		SAFETY->IAACR |= (initStruct->action << SAFETY_IAACR_R3INT_Pos);
	
		SAFETY->IF = (1 << SAFETY_IF_R3_Pos);
		if(initStruct->inten)
			SAFETY->IE |=  (1 << SAFETY_IE_R3_Pos);
		else
			SAFETY->IE &= ~(1 << SAFETY_IE_R3_Pos);
		break;
	}
	
	if(initStruct->inten)
		NVIC_EnableIRQ(PWM3_GPIOC_SAFETY_IRQn);
}


/****************************************************************************************************************************************** 
* 函数名称:	SAFETY_IAAD_INTEn()
* 功能说明:	非法地址访问检测中断使能
* 输    入: uint32_t region			想要配置的区域，可取值：SAFETY_REGION_0、SAFETY_REGION_1、SAFETY_REGION_2、SAFETY_REGION_3
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void SAFETY_IAAD_INTEn(uint32_t region)
{
	SAFETY->IE |= (1 << region);
}

/****************************************************************************************************************************************** 
* 函数名称: SAFETY_IAAD_INTDis()
* 功能说明:	非法地址访问检测中断禁能
* 输    入: uint32_t region			想要配置的区域，可取值：SAFETY_REGION_0、SAFETY_REGION_1、SAFETY_REGION_2、SAFETY_REGION_3
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void SAFETY_IAAD_INTDis(uint32_t region)
{
	SAFETY->IE &= ~(1 << region);
}

/****************************************************************************************************************************************** 
* 函数名称:	SAFETY_IAAD_INTClr()
* 功能说明:	非法地址访问检测中断标志清除
* 输    入: uint32_t region			想要配置的区域，可取值：SAFETY_REGION_0、SAFETY_REGION_1、SAFETY_REGION_2、SAFETY_REGION_3
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void SAFETY_IAAD_INTClr(uint32_t region)
{
	SAFETY->IF = (1 << region);
}

/****************************************************************************************************************************************** 
* 函数名称: SAFETY_IAAD_INTStat()
* 功能说明: 非法地址访问检测中断状态
* 输    入: uint32_t region			想要配置的区域，可取值：SAFETY_REGION_0、SAFETY_REGION_1、SAFETY_REGION_2、SAFETY_REGION_3
* 输    出: uint32_t 				0 未产生中断    1 产生了中断
* 注意事项: 无
******************************************************************************************************************************************/
uint32_t SAFETY_IAAD_INTStat(uint32_t region)
{
	return (SAFETY->IF >> region) & 1;
}
