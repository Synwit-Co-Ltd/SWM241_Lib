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

void WDT_Init(WDT_TypeDef * WDTx, WDT_InitStructure * initStruct);
void WDT_Start(WDT_TypeDef * WDTx);			//����ָ��WDT����ʼ����ʱ
void WDT_Stop(WDT_TypeDef * WDTx);			//�ر�ָ��WDT��ֹͣ����ʱ

void WDT_Feed(WDT_TypeDef * WDTx);			//ι�������´�װ��ֵ��ʼ����ʱ


void WDT_INTClr(WDT_TypeDef * WDTx);		//�жϱ�־���
uint32_t WDT_INTStat(WDT_TypeDef * WDTx);	//�ж�״̬��ѯ
 
#endif //__SWM241_WDT_H__
