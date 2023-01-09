#ifndef __SWM241_FLASH_H__
#define __SWM241_FLASH_H__


uint32_t FLASH_Erase(uint32_t addr);
uint32_t FLASH_Write(uint32_t addr, uint32_t buff[], uint32_t cnt);

#define FLASH_Read(addr)  *((volatile uint32_t *)(addr))



#define FLASH_RES_OK	0
#define FLASH_RES_TO	1	//Timeout
#define FLASH_RES_ERR	2


#endif //__SWM241_FLASH_H__
