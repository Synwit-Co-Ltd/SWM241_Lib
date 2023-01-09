/****************************************************************************************************************************************** 
* �ļ�����: SWM241_wdt.c
* ����˵��:	SWM241��Ƭ����WDT���Ź�����������
* ����֧��:	http://www.synwit.com.cn/e/tool/gbook/?bid=1
* ע������:
* �汾����:	V1.0.0		2016��1��30��
* ������¼:  
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
* ��������:	WDT_Init()
* ����˵��:	WDT���Ź���ʼ��
* ��    ��: WDT_TypeDef * WDTx		ָ��Ҫ�����õĿ��Ź�����Чֵ����WDT
*			WDT_InitStructure * initStruct	����WDT����趨ֵ�Ľṹ��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void WDT_Init(WDT_TypeDef * WDTx, WDT_InitStructure * initStruct)
{
	SYS->CLKEN0 |= (1 << SYS_CLKEN0_WDT_Pos);
	
	WDT_Stop(WDTx);		//����ǰ�ȹر�
	
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
* ��������:	WDT_Start()
* ����˵��:	����ָ��WDT����ʼ����ʱ
* ��    ��: WDT_TypeDef * WDTx		ָ��Ҫ�����õĿ��Ź�����Чֵ����WDT
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void WDT_Start(WDT_TypeDef * WDTx)
{
	WDTx->CR |=  (1 << WDT_CR_EN_Pos);
}

/****************************************************************************************************************************************** 
* ��������:	WDT_Stop()
* ����˵��:	�ر�ָ��WDT��ֹͣ����ʱ
* ��    ��: WDT_TypeDef * WDTx		ָ��Ҫ�����õĿ��Ź�����Чֵ����WDT
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void WDT_Stop(WDT_TypeDef * WDTx)
{
	WDTx->CR &= ~(1 << WDT_CR_EN_Pos);
}

/****************************************************************************************************************************************** 
* ��������:	WDT_Feed()
* ����˵��:	ι�������´�װ��ֵ��ʼ����ʱ
* ��    ��: WDT_TypeDef * WDTx		ָ��Ҫ�����õĿ��Ź�����Чֵ����WDT
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void WDT_Feed(WDT_TypeDef * WDTx)
{
	WDTx->FEED = 0x55;
}

/****************************************************************************************************************************************** 
* ��������:	WDT_INTClr()
* ����˵��:	�жϱ�־���
* ��    ��: WDT_TypeDef * WDTx		ָ��Ҫ�����õĿ��Ź�����Чֵ����WDT
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void WDT_INTClr(WDT_TypeDef * WDTx)
{
	WDTx->IF = 1;
}

/****************************************************************************************************************************************** 
* ��������:	WDT_INTStat()
* ����˵��:	�ж�״̬��ѯ
* ��    ��: WDT_TypeDef * WDTx		ָ��Ҫ�����õĿ��Ź�����Чֵ����WDT
* ��    ��: int32_t					1 �����ж����    0 δ�����ж����
* ע������: ��
******************************************************************************************************************************************/
uint32_t WDT_INTStat(WDT_TypeDef * WDTx)
{
	return WDTx->IF;
}
