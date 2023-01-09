#ifndef __SWM241_I2C_H__
#define __SWM241_I2C_H__


typedef struct {
	uint8_t  Master;		//1 ����ģʽ    0 �ӻ�ģʽ
	uint32_t MstClk;		//��������ʱ��Ƶ��
	
	uint8_t  Addr10b;		//1 10λ��ַģʽ     0 7λ��ַģʽ
	uint16_t SlvAddr;		//�ӻ���ַ
	uint16_t SlvAddrMsk;
	
	uint8_t  TXEmptyIEn;	//���ͼĴ������ж�ʹ��
	uint8_t  RXNotEmptyIEn;	//���ռĴ����ǿ��ж�ʹ��
	uint8_t  SlvSTADetIEn;	//�ӻ���⵽��ʼ�ж�ʹ��
	uint8_t  SlvSTODetIEn;	//�ӻ���⵽��ֹ�ж�ʹ��
} I2C_InitStructure;


void I2C_Init(I2C_TypeDef * I2Cx, I2C_InitStructure * initStruct);

void I2C_Open(I2C_TypeDef * I2Cx);
void I2C_Close(I2C_TypeDef * I2Cx);

uint8_t I2C_Start(I2C_TypeDef * I2Cx, uint8_t addr);
void I2C_Stop(I2C_TypeDef * I2Cx);
uint8_t I2C_Write(I2C_TypeDef * I2Cx, uint8_t data);
uint8_t I2C_Read(I2C_TypeDef * I2Cx, uint8_t ack);


#endif //__SWM241_I2C_H__
