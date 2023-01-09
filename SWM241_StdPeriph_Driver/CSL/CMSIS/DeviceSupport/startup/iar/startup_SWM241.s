;******************************************************************************************************************************************
; 文件名称:    startup_SWM241.s
; 功能说明:    SWM241单片机的启动文件
; 技术支持:    http://www.synwit.com.cn/e/tool/gbook/?bid=1
; 注意事项:
; 版本日期: V1.0.0        2016年1月30日
; 升级记录:
;
;
;******************************************************************************************************************************************
; @attention
;
; THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION
; REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE
; FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
; OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
; -ECTION WITH THEIR PRODUCTS.
;
; COPYRIGHT 2012 Synwit Technology
;******************************************************************************************************************************************

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        PUBLIC  __vector_table

        DATA
__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler
        DCD     NMI_Handler
        DCD     HardFault_Handler
        DCD     0
        DCD     0
        DCD     0
        DCD     0
        DCD     0
        DCD     0
        DCD     0
        DCD     SVC_Handler
        DCD     0
        DCD     0
        DCD     PendSV_Handler
        DCD     SysTick_Handler

        ; External Interrupts
        DCD    UART0_Handler
        DCD    TIMR0_Handler
        DCD    SPI0_Handler
        DCD    UART1_Handler
        DCD    UART2_Handler
        DCD    TIMR1_Handler
        DCD    DMA_Handler
        DCD    PWM0_Handler
        DCD    I2C1_Handler
        DCD    TIMR2_Handler
        DCD    TIMR3_Handler
        DCD    WDT_Handler
        DCD    I2C0_Handler
        DCD    UART3_Handler
        DCD    ADC_Handler
        DCD    TIMR4_Handler
        DCD    GPIOC0_GPIOD1_Handler
        DCD    GPIOB1_GPIOC2_Handler
        DCD    TIMR5_GPIOC3_Handler
        DCD    GPIOA0_GPIOD6_Handler
        DCD    TIMR6_GPIOC1_Handler
        DCD    GPIOA1_GPIOD8_Handler
        DCD    GPIOB7_GPIOD9_Handler
        DCD    GPIOB5_GPIOD10_Handler
        DCD    GPIOA2_GPIOB2_GPIOD13_Handler
        DCD    TIMR7_XTALSTOP_GPIOD12_Handler
        DCD    PWM1_GPIOA_Handler
        DCD    PWM2_GPIOB_Handler
        DCD    BOD_PWMBRK_GPIOD11_Handler
        DCD    PWM3_GPIOC_SAFETY_Handler
        DCD    SPI1_HALL_GPIOA9_Handler
        DCD    RTC_GPIOD_Handler


        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler
        LDR     R0, =__iar_program_start
        BX      R0
        
        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        B NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
        B HardFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B SVC_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B SysTick_Handler

        PUBWEAK UART0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART0_Handler
        B UART0_Handler

        PUBWEAK TIMR0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMR0_Handler
        B TIMR0_Handler

        PUBWEAK SPI0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPI0_Handler
        B SPI0_Handler

        PUBWEAK UART1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART1_Handler
        B UART1_Handler

        PUBWEAK UART2_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART2_Handler
        B UART2_Handler

        PUBWEAK TIMR1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMR1_Handler
        B TIMR1_Handler

        PUBWEAK DMA_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA_Handler
        B DMA_Handler

        PUBWEAK PWM0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PWM0_Handler
        B PWM0_Handler

        PUBWEAK I2C1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C1_Handler
        B I2C1_Handler

        PUBWEAK TIMR2_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMR2_Handler
        B TIMR2_Handler

        PUBWEAK TIMR3_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMR3_Handler
        B TIMR3_Handler

        PUBWEAK WDT_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
WDT_Handler
        B WDT_Handler

        PUBWEAK I2C0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C0_Handler
        B I2C0_Handler

        PUBWEAK UART3_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART3_Handler
        B UART3_Handler

        PUBWEAK ADC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
ADC_Handler
        B ADC_Handler

        PUBWEAK TIMR4_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMR4_Handler
        B TIMR4_Handler

        PUBWEAK GPIOC0_GPIOD1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOC0_GPIOD1_Handler
        B GPIOC0_GPIOD1_Handler

        PUBWEAK GPIOB1_GPIOC2_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOB1_GPIOC2_Handler
        B GPIOB1_GPIOC2_Handler

        PUBWEAK TIMR5_GPIOC3_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMR5_GPIOC3_Handler
        B TIMR5_GPIOC3_Handler

        PUBWEAK GPIOA0_GPIOD6_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOA0_GPIOD6_Handler
        B GPIOA0_GPIOD6_Handler

        PUBWEAK TIMR6_GPIOC1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMR6_GPIOC1_Handler
        B TIMR6_GPIOC1_Handler

        PUBWEAK GPIOA1_GPIOD8_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOA1_GPIOD8_Handler
        B GPIOA1_GPIOD8_Handler

        PUBWEAK GPIOB7_GPIOD9_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOB7_GPIOD9_Handler
        B GPIOB7_GPIOD9_Handler

        PUBWEAK GPIOB5_GPIOD10_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOB5_GPIOD10_Handler
        B GPIOB5_GPIOD10_Handler

        PUBWEAK GPIOA2_GPIOB2_GPIOD13_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIOA2_GPIOB2_GPIOD13_Handler
        B GPIOA2_GPIOB2_GPIOD13_Handler

        PUBWEAK TIMR7_XTALSTOP_GPIOD12_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMR7_XTALSTOP_GPIOD12_Handler
        B TIMR7_XTALSTOP_GPIOD12_Handler

        PUBWEAK PWM1_GPIOA_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PWM1_GPIOA_Handler
        B PWM1_GPIOA_Handler

        PUBWEAK PWM2_GPIOB_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PWM2_GPIOB_Handler
        B PWM2_GPIOB_Handler

        PUBWEAK BOD_PWMBRK_GPIOD11_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BOD_PWMBRK_GPIOD11_Handler
        B BOD_PWMBRK_GPIOD11_Handler

        PUBWEAK PWM3_GPIOC_SAFETY_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PWM3_GPIOC_SAFETY_Handler
        B PWM3_GPIOC_SAFETY_Handler

        PUBWEAK SPI1_HALL_GPIOA9_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPI1_HALL_GPIOA9_Handler
        B SPI1_HALL_GPIOA9_Handler

        PUBWEAK RTC_GPIOD_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
RTC_GPIOD_Handler
        B RTC_GPIOD_Handler

        END
