#ifndef __SWM241_SLED_H__
#define	__SWM241_SLED_H__


typedef struct {
	uint8_t  duty;		// COM引脚个数，可取值SLED_DUTY_4、SLED_DUTY_8
	uint8_t  clkdiv;	// 时钟分频，系统时钟经此分频后产生SLED工作时钟
	uint16_t period;	// COM周期时间长度
	uint16_t high;		// COM周期内COM输出高电平的时间长度
	uint8_t  cominv;	// COM输出电平取反
} SLED_InitStructure;

#define SLED_DUTY_4	 0
#define SLED_DUTY_8	 1

#define SLED_CLKDIV_2		1
#define SLED_CLKDIV_4		2
#define SLED_CLKDIV_8		3
#define SLED_CLKDIV_16		4
#define SLED_CLKDIV_32		5
#define SLED_CLKDIV_64		6
#define SLED_CLKDIV_128		7
#define SLED_CLKDIV_256		8
#define SLED_CLKDIV_512		9
#define SLED_CLKDIV_1024	10
#define SLED_CLKDIV_2048	11
#define SLED_CLKDIV_4096	12
#define SLED_CLKDIV_8192	13
#define SLED_CLKDIV_16384	14
#define SLED_CLKDIV_32768	15


void SLED_Init(SLED_TypeDef * SLEDx, SLED_InitStructure * initStruct);
void SLED_Start(SLED_TypeDef * SLEDx);
void SLED_Stop(SLED_TypeDef * SLEDx);


#endif //__SWM241_SLED_H__
