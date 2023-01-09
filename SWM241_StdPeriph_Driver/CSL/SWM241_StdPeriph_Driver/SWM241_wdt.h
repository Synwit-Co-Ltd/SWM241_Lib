#ifndef __SWM241_WDT_H__
#define	__SWM241_WDT_H__


typedef struct {
	uint8_t  clksrc;		// WDT_CLKSRC_HSI��WDT_CLKSRC_LSI
	uint8_t  clkdiv;		// ʱ�ӷ�Ƶ��ϵͳʱ�Ӿ��˷�Ƶ�����WDT����ʱ��
	uint16_t int_period;	// WDT �ж�����
	uint8_t  int_en;		// WDT �ж�ʹ��
	uint16_t rst_period;	// WDT ��λ����
	uint8_t  rst_en;		// WDT ��λʹ��
	uint8_t  win_en;		// WDT ���ڹ���ʹ�ܣ��������ڹ��ܺ�ֻ�����жϵ����󡢸�λ����ǰι�������жϵ���ǰι���ᵼ��������λ
} WDT_InitStructure;


#define WDT_CLKSRC_HSI		0	//Ƭ�ڸ�ƵRC������24/48MHz��
#define WDT_CLKSRC_LSI		2	//Ƭ�ڵ�ƵRC������32KHz��

#define WDT_CLKDIV_2		0
#define WDT_CLKDIV_4		1
#define WDT_CLKDIV_8		2
#define WDT_CLKDIV_16		3
#define WDT_CLKDIV_32		4
#define WDT_CLKDIV_64		5
#define WDT_CLKDIV_128		6
#define WDT_CLKDIV_256		7
#define WDT_CLKDIV_512		8
#define WDT_CLKDIV_1024		9
#define WDT_CLKDIV_2048		10
#define WDT_CLKDIV_4096		11
#define WDT_CLKDIV_8192		12
#define WDT_CLKDIV_16384	13
#define WDT_CLKDIV_32768	14
#define WDT_CLKDIV_65536	15


void WDT_Init(WDT_TypeDef * WDTx, WDT_InitStructure * initStruct);
void WDT_Start(WDT_TypeDef * WDTx);			//����ָ��WDT����ʼ����ʱ
void WDT_Stop(WDT_TypeDef * WDTx);			//�ر�ָ��WDT��ֹͣ����ʱ

void WDT_Feed(WDT_TypeDef * WDTx);			//ι�������´�װ��ֵ��ʼ����ʱ


void WDT_INTClr(WDT_TypeDef * WDTx);		//�жϱ�־���
uint32_t WDT_INTStat(WDT_TypeDef * WDTx);	//�ж�״̬��ѯ
 
#endif //__SWM241_WDT_H__
