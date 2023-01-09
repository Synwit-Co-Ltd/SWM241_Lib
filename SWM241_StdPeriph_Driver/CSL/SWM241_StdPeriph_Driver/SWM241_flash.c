/****************************************************************************************************************************************** 
* 文件名称:	SWM241_flash.c
* 功能说明:	使用芯片的IAP功能将片上Flash模拟成EEPROM来保存数据，掉电后不丢失
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
#include "SWM241_flash.h"

typedef uint32_t (*IAPFunc1)(uint32_t addr);
typedef uint32_t (*IAPFunc2)(uint32_t faddr, uint32_t raddr, uint32_t cnt);

IAPFunc1 FLASH_PageErase = (IAPFunc1)0x1000501;

IAPFunc2 FLASH_PageWrite = (IAPFunc2)0x1000601;


/****************************************************************************************************************************************** 
* 函数名称:	FLASH_Erase()
* 功能说明:	FLASH扇区擦除，每个扇区1K字节
* 输    入: uint32_t addr		要擦除扇区的地址，必须1K对齐，即addr%1024 == 0
* 输    出: uint32_t			FLASH_RES_OK、FLASH_RES_TO、FLASH_RES_ERR
* 注意事项: 无
******************************************************************************************************************************************/
#if defined ( __ICCARM__ )
__ramfunc
#endif
uint32_t FLASH_Erase(uint32_t addr)
{
	if(addr >= 64*1024) return FLASH_RES_ERR;
	
	__disable_irq();
	
	FMC->ERASE = (1u << FMC_ERASE_REQ_Pos) | ((addr >> 10) << FMC_ERASE_PAGE_Pos);
	while(FMC->STAT & FMC_STAT_ERASEBUSY_Msk) __NOP();
	
	__enable_irq();
	
	return FLASH_RES_OK;
}


/****************************************************************************************************************************************** 
* 函数名称:	FLASH_Write()
* 功能说明:	FLASH数据写入
* 输    入: uint32_t addr		数据要写入到Flash中的地址，字对齐
*			uint32_t buff[]		要写入Flash中的数据
*			uint32_t cnt		要写的数据的个数，以字为单位，最大256
* 输    出: uint32_t			FLASH_RES_OK、FLASH_RES_TO、FLASH_RES_ERR
* 注意事项: 要写入的数据必须全部在同一页内，每页1K，即addr/1024 == (addr+(cnt-1)*4)/1024
******************************************************************************************************************************************/
#if defined ( __ICCARM__ )
__ramfunc
#endif
uint32_t FLASH_Write(uint32_t addr, uint32_t buff[], uint32_t cnt)
{	
	uint32_t i;
	
	if((addr+cnt*4) >= 64*1024) return FLASH_RES_ERR;
	
	if(addr/1024 != (addr+(cnt-1)*4)/1024) return FLASH_RES_ERR;	// 跨页
	
	__disable_irq();
	
	FMC->ADDR = (1u << FMC_ADDR_WREN_Pos) | (addr << FMC_ADDR_ADDR_Pos);
	for(i = 0; i < cnt; i++)
	{
		FMC->DATA = buff[i];
		while(FMC->ADDR & FMC_ADDR_BUSY_Msk) __NOP();
	}
	while(FMC->STAT & FMC_STAT_PROGBUSY_Msk) __NOP();
	
	FMC->ADDR = 0;
	
	__enable_irq();
	
	return FLASH_RES_OK;
}
