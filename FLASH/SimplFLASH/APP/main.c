#include "SWM241.h"


/* ע�⣺SWM241_Flash.c�еĺ���������RAM��ִ�У�Keil��ʵ�ַ����У�
   ����һ��Scatter file
   ��������SWM240_Flash.c���Ҽ� =��Options for File "SWM241_Flash.c" =��Properties =��Memory Assignment =��Code/Conts ѡ�� IRAM1
*/


void SerialInit(void);

#define EEPROM_ADDR	  0x8000

uint32_t WrBuff[20] = {0x14141414, 0x15151515, 0x16161616, 0x17171717, 0x18181818,
				       0x19191919, 0x1A1A1A1A, 0x1B1B1B1B, 0x1C1C1C1C, 0x1D1D1D1D,
				       0x1E1E1E1E, 0x1F1F1F1F, 0x20202020, 0x21212121, 0x22222222,
				       0x23232323, 0x24242424, 0x25252525, 0x26262626, 0x27272727};

int main(void)
{
	uint32_t i;
		
	SystemInit();
	
	SerialInit();
	
	printf("\r\nAfter Erase: \r\n");
	
	FLASH_Erase(EEPROM_ADDR);
	
	for(i = 0; i < 20; i++) printf("0x%X, ", FLASH_Read(EEPROM_ADDR+i*4));
	
	printf("\r\nAfter Write: \r\n");
   	
	FLASH_Write(EEPROM_ADDR, WrBuff, 20);
	
	for(i = 0; i < 20; i++) printf("0x%X, ", FLASH_Read(EEPROM_ADDR+i*4));
	
	while(1==1)
	{
	}
}


void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTB, PIN11, PORTB_PIN11_UART0_RX, 1);	//GPIOB.11����ΪUART0��������
	PORT_Init(PORTB, PIN12, PORTB_PIN12_UART0_TX, 0);	//GPIOB.12����ΪUART0�������
 	
	UART_initStruct.Baudrate = 57600;
	UART_initStruct.DataBits = UART_DATA_8BIT;
	UART_initStruct.Parity = UART_PARITY_NONE;
	UART_initStruct.StopBits = UART_STOP_1BIT;
	UART_initStruct.RXThresholdIEn = 0;
	UART_initStruct.TXThresholdIEn = 0;
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
