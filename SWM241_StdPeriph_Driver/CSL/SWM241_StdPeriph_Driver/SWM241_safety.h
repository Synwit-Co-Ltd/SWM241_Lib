#ifndef __SWM241_SAFETY_H__
#define	__SWM241_SAFETY_H__

typedef struct {
	uint32_t bottom_addr;	//区域底部地址
	uint32_t top_addr;		//区域顶部地址，注意：REGION_3的顶部地址固定为0xFFFFFFFF
	uint32_t action;		//触发地址非法访问时的动作，可取值：SAFETY_ACTION_RESET、SAFETY_ACTION_INT
	uint32_t inten;			//地址非法访问中断使能
} SAFETY_Region_InitStructure;


#define SAFETY_WP_IER		(1 << SAFETY_PERWP_IER_Pos)			//中断使能寄存器写保护
#define SAFETY_WP_IOCFGR	(1 << SAFETY_PERWP_IOCFGR_Pos)		// IO 配置寄存器写保护
#define SAFETY_WP_CLKCFGR	(1 << SAFETY_PERWP_CLKCFGR_Pos)		//时钟配置寄存器写保护
#define SAFETY_WP_ANACFGR	(1 << SAFETY_PERWP_ANACFGR_Pos)		//模拟配置寄存器写保护

#define SAFETY_SIZE_256B	1
#define SAFETY_SIZE_512B	2
#define SAFETY_SIZE_1024B	3

#define SAFETY_REGION_0		0
#define SAFETY_REGION_1		1
#define SAFETY_REGION_2		2
#define SAFETY_REGION_3		3

#define SAFETY_ACTION_RESET	0
#define SAFETY_ACTION_INT	1


void SAFETY_PerWP_Open(uint32_t peripherals);
void SAFETY_PerWP_Close(uint32_t peripherals);
void SAFETY_RAMWP_Open(uint32_t addr, uint32_t size);
void SAFETY_RAMWP_Close(void);
void SAFETY_IAAD_Open(void);
void SAFETY_IAAD_Close(void);
void SAFETY_IAADRegion_Config(uint32_t region, SAFETY_Region_InitStructure * initStruct);

void SAFETY_IAAD_INTEn(uint32_t region);
void SAFETY_IAAD_INTDis(uint32_t region);
void SAFETY_IAAD_INTClr(uint32_t region);
uint32_t SAFETY_IAAD_INTStat(uint32_t region);


#endif // __SWM241_SAFETY_H__
