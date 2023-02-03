#include "SWM241.h"
#include <string.h>

void SerialInit(void);

int main(void)
{		
	uint32_t i, res;
	char data[] = "Hi from SynWit";
	CRC_InitStructure CRC_initStruct;
	
	SystemInit();
	
	SerialInit();
	
	CRC_initStruct.Poly = CRC_POLY_104C11DB7;
	CRC_initStruct.init_crc = 0x00000000;
	CRC_initStruct.in_width = CRC_WIDTH_8;
	CRC_initStruct.in_rev = CRC_REV_NOT;
	CRC_initStruct.in_not = false;
	CRC_initStruct.out_rev = CRC_REV_NOT;
	CRC_initStruct.out_not = false;
	
	CRC_Init(CRC, &CRC_initStruct);
	
	for(i = 0; i < strlen("Hi from SynWit"); i++)
		CRC_Write(data[i]);
	
	res = CRC_Result();
	
	if(res == 0xC4D9CB63)
		printf("CRC Pass! Result = 0x%08X\r\n", res);
	else
		printf("CRC Fail! Result = 0x%08X\r\n", res);
	
	while(1==1)
	{
		if(GPIO_GetBit(GPIOA, PIN4) == 0)		//����������
		{
			GPIO_SetBit(GPIOA, PIN5);
		}
		else
		{
			GPIO_ClrBit(GPIOA, PIN5);
		}
	}
}


void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTD, PIN13, PORTD_PIN13_UART0_RX, 1);	//GPIOD.13����ΪUART0��������
	PORT_Init(PORTD, PIN14, PORTD_PIN14_UART0_TX, 0);	//GPIOD.14����ΪUART0�������
 	
	UART_initStruct.Baudrate = 57600;
	UART_initStruct.DataBits = UART_DATA_8BIT;
	UART_initStruct.Parity = UART_PARITY_NONE;
	UART_initStruct.StopBits = UART_STOP_1BIT;
	UART_initStruct.RXThreshold = 3;
	UART_initStruct.RXThresholdIEn = 0;
	UART_initStruct.TXThreshold = 3;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutTime = 10;
	UART_initStruct.TimeoutIEn = 0;
 	UART_Init(UART0, &UART_initStruct);
	UART_Open(UART0);
}

/****************************************************************************************************************************************** 
* ��������: fputc()
* ����˵��: printf()ʹ�ô˺������ʵ�ʵĴ��ڴ�ӡ����
* ��    ��: int ch		Ҫ��ӡ���ַ�
*			FILE *f		�ļ����
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
int fputc(int ch, FILE *f)
{
	UART_WriteByte(UART0, ch);
	
	while(UART_IsTXBusy(UART0));
 	
	return ch;
}
