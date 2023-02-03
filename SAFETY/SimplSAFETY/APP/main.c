#include "SWM241.h"


#define TEST_RAM_ADDR  0x20001800


void SerialInit(void);
void test_PerWP(void);
void test_RAMWP(void);
void test_IAAD(void);


int main(void)
{	
	SystemInit();
	
	SerialInit();
	
	test_PerWP();	// ����д��������
	
	test_RAMWP();	// �ڴ�д��������
	
	test_IAAD();	// �Ƿ���ַ���ʼ�����
	
	while(1==1)
	{
	}
}

void test_PerWP(void)
{
	PORTB->PULLU |= (1 << PIN11);
	if((PORTB->PULLU & (1 << PIN11)) == 0)
	{
		printf("test_PerWP init fail\r\n");
		return;
	}
	
	
	// after peripheral write-protect opened
	SAFETY_PerWP_Open(SAFETY_WP_IOCFGR | SAFETY_WP_IER);
	
	PORTB->PULLU &= ~(1 << PIN11);
	if((PORTB->PULLU & (1 << PIN11)) == 0)
	{
		printf("test_PerWP protect fail\r\n");
		return;
	}
	
	
	// after peripheral write-protect closed
	SAFETY_PerWP_Close(SAFETY_WP_IOCFGR | SAFETY_WP_IER);
	
	PORTB->PULLU &= ~(1 << PIN11);
	if((PORTB->PULLU & (1 << PIN11)) == 0)
	{
		printf("test_PerWP test pass\r\n");
	}
	else
	{
		printf("test_PerWP test fail\r\n");
	}
}

void test_RAMWP(void)
{
	volatile uint32_t * ptr = (uint32_t *)TEST_RAM_ADDR;	// ��ֹ�������Ż�����ִ�ж�����
	
	*ptr = 1;
	if(*ptr == 0)
	{
		printf("test_RAMWP init fail\r\n");
		return;
	}
	
	
	// after RAM write-protect opened
	SAFETY_RAMWP_Open(TEST_RAM_ADDR, SAFETY_SIZE_512B);
	
	*ptr = 0;
	if(*ptr == 0)
	{
		printf("test_RAMWP protect fail\r\n");
		return;
	}
	
	
	// after RAM write-protect closed
	SAFETY_RAMWP_Close();
	
	*ptr = 0;
	if(*ptr == 0)
	{
		printf("test_RAMWP test pass\r\n");
	}
	else
	{
		printf("test_RAMWP test fail\r\n");
	}
}

void test_IAAD(void)
{
	SAFETY_Region_InitStructure SAFETY_Region_initStruct;
	volatile uint32_t * ptr = (uint32_t *)TEST_RAM_ADDR;	//��ֹ�������Ż�����ִ��д����
	
	SAFETY_Region_initStruct.bottom_addr = TEST_RAM_ADDR;
	SAFETY_Region_initStruct.top_addr    = TEST_RAM_ADDR + 0x800;
	SAFETY_Region_initStruct.action = SAFETY_ACTION_INT;
	SAFETY_Region_initStruct.inten = 1;
	SAFETY_IAADRegion_Config(SAFETY_REGION_0, &SAFETY_Region_initStruct);
	
	SAFETY_IAAD_Open();
	
	*ptr = 1;
}

void PWM3_GPIOC_SAFETY_Handler(void)
{
	SAFETY_IAAD_INTClr(SAFETY_REGION_0);
	printf("Illegal Address Access detected\r\n");
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
