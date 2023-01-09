/****************************************************************************************************************************************** 
* �ļ�����:	SWM241_i2c.c
* ����˵��:	SWM241��Ƭ����I2C���нӿڹ���������
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
#include "SWM241_i2c.h"


/****************************************************************************************************************************************** 
* ��������:	I2C_Init()
* ����˵��:	I2C��ʼ��
* ��    ��: I2C_TypeDef * I2Cx		ָ��Ҫ�����õ�I2C����Чֵ����I2C0��I2C1
*			I2C_InitStructure * initStruct	����I2C����趨ֵ�Ľṹ��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void I2C_Init(I2C_TypeDef * I2Cx, I2C_InitStructure * initStruct)
{
	switch((uint32_t)I2Cx)
	{
	case ((uint32_t)I2C0):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_I2C0_Pos);
		break;
	
	case ((uint32_t)I2C1):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_I2C1_Pos);
		break;
	}
	
	I2C_Close(I2Cx);	//һЩ�ؼ��Ĵ���ֻ����I2C�ر�ʱ����
	
	if(initStruct->Master == 1)
	{
		I2Cx->CR |= (1 << I2C_CR_MASTER_Pos);
		
		I2Cx->CLK = ((SystemCoreClock/1000000/3*2)  << I2C_CLK_SCLL_Pos) |
					((SystemCoreClock/1000000/3*1)  << I2C_CLK_SCLH_Pos) |
					((1000000 / initStruct->MstClk) << I2C_CLK_DIV_Pos);
		
		I2Cx->IF = 0xFFFFFFFF;
		I2Cx->IE = (initStruct->TXEmptyIEn << I2C_IE_TXE_Pos) |
				   (initStruct->RXNotEmptyIEn << I2C_IE_RXNE_Pos);
		
		switch((uint32_t)I2Cx)
		{
		case ((uint32_t)I2C0):		
			if(initStruct->TXEmptyIEn | initStruct->RXNotEmptyIEn)
			{
				NVIC_EnableIRQ(I2C0_IRQn);
			}
			else
			{
				NVIC_DisableIRQ(I2C0_IRQn);
			}
			break;
		
		case ((uint32_t)I2C1):			
			if(initStruct->TXEmptyIEn | initStruct->RXNotEmptyIEn)
			{
				NVIC_EnableIRQ(I2C1_IRQn);
			}
			else
			{
				NVIC_DisableIRQ(I2C1_IRQn);
			}
			break;
		}
	}
	else
	{
		I2Cx->CR &= ~(1 << I2C_CR_MASTER_Pos);
		
		I2Cx->SCR &= ~I2C_SCR_ADDR10_Msk;
		I2Cx->SCR |= (initStruct->Addr10b << I2C_SCR_ADDR10_Pos);
		
		if(initStruct->Addr10b)
			I2Cx->SADDR = (initStruct->SlvAddr    << I2C_SADDR_ADDR10_Pos) |
						  (initStruct->SlvAddrMsk << I2C_SADDR_MASK10_Pos);
		else
			I2Cx->SADDR = (initStruct->SlvAddr    << I2C_SADDR_ADDR7_Pos) |
						  (initStruct->SlvAddrMsk << I2C_SADDR_MASK7_Pos);
		
		I2Cx->IF = 0xFFFFFFFF;
		I2Cx->IE = (initStruct->TXEmptyIEn    << I2C_IE_TXE_Pos) |
				   (initStruct->RXNotEmptyIEn << I2C_IE_RXNE_Pos) |
				   (initStruct->SlvSTADetIEn  << I2C_IE_RXSTA_Pos) |
				   (initStruct->SlvSTODetIEn  << I2C_IE_RXSTO_Pos);
	
		switch((uint32_t)I2Cx)
		{
		case ((uint32_t)I2C0):
			if(initStruct->SlvSTADetIEn | initStruct->SlvSTODetIEn | initStruct->TXEmptyIEn | initStruct->RXNotEmptyIEn)
			{
				NVIC_EnableIRQ(I2C0_IRQn);
			}
			else
			{
				NVIC_DisableIRQ(I2C0_IRQn);
			}
			break;
		
		case ((uint32_t)I2C1):
			if(initStruct->SlvSTADetIEn | initStruct->SlvSTODetIEn | initStruct->TXEmptyIEn | initStruct->RXNotEmptyIEn)
			{
				NVIC_EnableIRQ(I2C1_IRQn);
			}
			else
			{
				NVIC_DisableIRQ(I2C1_IRQn);
			}
			break;
		}
	}
}

/****************************************************************************************************************************************** 
* ��������:	I2C_Open()
* ����˵��:	I2C�򿪣������շ�
* ��    ��: I2C_TypeDef * I2Cx		ָ��Ҫ�����õ�I2C����Чֵ����I2C0��I2C1
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void I2C_Open(I2C_TypeDef * I2Cx)
{
	I2Cx->CR |= (0x01 << I2C_CR_EN_Pos);
}

/****************************************************************************************************************************************** 
* ��������:	I2C_Close()
* ����˵��:	I2C�رգ���ֹ�շ�
* ��    ��: I2C_TypeDef * I2Cx		ָ��Ҫ�����õ�I2C����Чֵ����I2C0��I2C1
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void I2C_Close(I2C_TypeDef * I2Cx)
{
	I2Cx->CR &= ~(0x01 << I2C_CR_EN_Pos);
}

/****************************************************************************************************************************************** 
* ��������:	I2C_Start()
* ����˵��:	������ʼ�źŲ������豸��ַ
* ��    ��: I2C_TypeDef * I2Cx		ָ��Ҫ�����õ�I2C����Чֵ����I2C0��I2C1
*			uint8_t addr			�豸��ַ
* ��    ��: uint8_t					1 ���յ�ACK   0 ���յ�NACK
* ע������: ��
******************************************************************************************************************************************/
uint8_t I2C_Start(I2C_TypeDef * I2Cx, uint8_t addr)
{
	I2Cx->TXDATA = addr;
	I2Cx->MCR = (1 << I2C_MCR_STA_Pos) |		
				(1 << I2C_MCR_WR_Pos);				//������ʼλ�ʹӻ���ַ
	while(I2Cx->MCR & I2C_MCR_WR_Msk) __NOP();		//�ȴ��������
	
	return (I2Cx->TR & I2C_TR_RXACK_Msk) ? 0 : 1;
}

/****************************************************************************************************************************************** 
* ��������:	I2C_Stop()
* ����˵��:	����ֹͣ�ź�
* ��    ��: I2C_TypeDef * I2Cx		ָ��Ҫ�����õ�I2C����Чֵ����I2C0��I2C1
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void I2C_Stop(I2C_TypeDef * I2Cx)
{
	I2Cx->MCR = (1 << I2C_MCR_STO_Pos);
	while(I2Cx->MCR & I2C_MCR_STO_Msk) __NOP();		//�ȴ��������
}

/****************************************************************************************************************************************** 
* ��������:	I2C_Write()
* ����˵��:	д��һ������
* ��    ��: I2C_TypeDef * I2Cx		ָ��Ҫ�����õ�I2C����Чֵ����I2C0��I2C1
*			uint8_t data			Ҫд������
* ��    ��: uint8_t					1 ���յ�ACK   0 ���յ�NACK
* ע������: ��
******************************************************************************************************************************************/
uint8_t I2C_Write(I2C_TypeDef * I2Cx, uint8_t data)
{	
	I2Cx->TXDATA = data;
	I2Cx->MCR = (1 << I2C_MCR_WR_Pos);
	while(I2Cx->MCR & I2C_MCR_WR_Msk) __NOP();		//�ȴ��������
	
	return (I2Cx->TR & I2C_TR_RXACK_Msk) ? 0 : 1;
}

/****************************************************************************************************************************************** 
* ��������:	I2C_Read()
* ����˵��:	��ȡһ������
* ��    ��: I2C_TypeDef * I2Cx		ָ��Ҫ�����õ�I2C����Чֵ����I2C0��I2C1
*			uint8_t ack				1 ����ACK   0 ����NACK
* ��    ��: uint8_t					��ȡ��������
* ע������: ��
******************************************************************************************************************************************/
uint8_t I2C_Read(I2C_TypeDef * I2Cx, uint8_t ack)
{
	I2Cx->TR = ((ack ? 0 : 1) << I2C_TR_TXACK_Pos);
	
	I2Cx->MCR = (1 << I2C_MCR_RD_Pos);
	while(I2Cx->MCR & I2C_MCR_RD_Msk) __NOP();		//�ȴ��������
    
    return I2Cx->RXDATA;
}
