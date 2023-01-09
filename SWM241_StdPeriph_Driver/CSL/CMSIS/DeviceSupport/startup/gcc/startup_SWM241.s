    .syntax unified
    .arch armv6-m

/* Memory Model
   The HEAP starts at the end of the DATA section and grows upward.
   
   The STACK starts at the end of the RAM and grows downward     */
    .section .stack
    .align 3
    .globl    __StackTop
    .globl    __StackLimit
__StackLimit:
    .space    0x800
__StackTop:


    .section .heap
    .align 3
    .globl    __HeapBase
    .globl    __HeapLimit
__HeapBase:
    .space    0x000
__HeapLimit:


    .section .isr_vector
    .align 2
    .globl __isr_vector
__isr_vector:
    .long    __StackTop            
    .long    Reset_Handler         
    .long    NMI_Handler          
    .long    HardFault_Handler     
    .long    0     
    .long    0      
    .long    0   
    .long    0                    
    .long    0                    
    .long    0                    
    .long    0
    .long    SVC_Handler          
    .long    0     
    .long    0                     
    .long    PendSV_Handler           
    .long    SysTick_Handler         

    /* External interrupts */
    .long    UART0_Handler
	.long    TIMR0_Handler
	.long    SPI0_Handler
	.long    UART1_Handler
	.long    UART2_Handler
	.long    TIMR1_Handler
	.long    DMA_Handler
	.long    PWM0_Handler
	.long    I2C1_Handler
	.long    TIMR2_Handler
	.long    TIMR3_Handler
	.long    WDT_Handler
	.long    I2C0_Handler
	.long    UART3_Handler
	.long    ADC_Handler
	.long    TIMR4_Handler
	.long    GPIOC0_GPIOD1_Handler
	.long    GPIOB1_GPIOC2_Handler
	.long    TIMR5_GPIOC3_Handler
	.long    GPIOA0_GPIOD6_Handler
	.long    TIMR6_GPIOC1_Handler
	.long    GPIOA1_GPIOD8_Handler
	.long    GPIOB7_GPIOD9_Handler
	.long    GPIOB5_GPIOD10_Handler
	.long    GPIOA2_GPIOB2_GPIOD13_Handler
	.long    TIMR7_XTALSTOP_GPIOD12_Handler
	.long    PWM1_GPIOA_Handler
	.long    PWM2_GPIOB_Handler
	.long    BOD_PWMBRK_GPIOD11_Handler
	.long    PWM3_GPIOC_SAFETY_Handler
	.long    SPI1_HALL_GPIOA9_Handler
	.long    RTC_GPIOD_Handler

	.section .text.Reset_Handler
    .align 2
    .thumb
    .globl    Reset_Handler
    .type     Reset_Handler, %function
Reset_Handler:
/* Loop to copy data from read only memory to RAM. The ranges
 * of copy from/to are specified by symbols evaluated in linker script.  */
    ldr    r1, =__data_load__
    ldr    r2, =__data_start__
    ldr    r3, =__data_end__

    subs   r3, r2
    ble .Lflash_to_ram_done

.Lflash_to_ram_loop:
    subs r3, #4
    ldr r0, [r1, r3]
    str r0, [r2, r3]
    bgt .Lflash_to_ram_loop
.Lflash_to_ram_done:


    ldr    r2, =__bss_start__
    ldr    r3, =__bss_end__

    subs   r3, r2
    ble .Lbss_to_ram_done

    movs    r0, 0
.Lbss_to_ram_loop:
    subs r3, #4
    str r0, [r2, r3]
    bgt .Lbss_to_ram_loop
.Lbss_to_ram_done:

    ldr    r0, =main
    bx     r0
    .pool    


    .text
/* Macro to define default handlers. 
   Default handler will be weak symbol and just dead loops. */
    .macro    def_default_handler    handler_name
    .align 1
    .thumb_func
    .weak    \handler_name
    .type    \handler_name, %function
\handler_name :
    b    .
    .endm

    def_default_handler    NMI_Handler
    def_default_handler    HardFault_Handler
    def_default_handler    SVC_Handler
    def_default_handler    PendSV_Handler
    def_default_handler    SysTick_Handler

	def_default_handler    UART0_Handler
	def_default_handler    TIMR0_Handler
	def_default_handler    SPI0_Handler
	def_default_handler    UART1_Handler
	def_default_handler    UART2_Handler
	def_default_handler    TIMR1_Handler
	def_default_handler    DMA_Handler
	def_default_handler    PWM0_Handler
	def_default_handler    I2C1_Handler
	def_default_handler    TIMR2_Handler
	def_default_handler    TIMR3_Handler
	def_default_handler    WDT_Handler
	def_default_handler    I2C0_Handler
	def_default_handler    UART3_Handler
	def_default_handler    ADC_Handler
	def_default_handler    TIMR4_Handler
	def_default_handler    GPIOC0_GPIOD1_Handler
	def_default_handler    GPIOB1_GPIOC2_Handler
	def_default_handler    TIMR5_GPIOC3_Handler
	def_default_handler    GPIOA0_GPIOD6_Handler
	def_default_handler    TIMR6_GPIOC1_Handler
	def_default_handler    GPIOA1_GPIOD8_Handler
	def_default_handler    GPIOB7_GPIOD9_Handler
	def_default_handler    GPIOB5_GPIOD10_Handler
	def_default_handler    GPIOA2_GPIOB2_GPIOD13_Handler
	def_default_handler    TIMR7_XTALSTOP_GPIOD12_Handler
	def_default_handler    PWM1_GPIOA_Handler
	def_default_handler    PWM2_GPIOB_Handler
	def_default_handler    BOD_PWMBRK_GPIOD11_Handler
	def_default_handler    PWM3_GPIOC_SAFETY_Handler
	def_default_handler    SPI1_HALL_GPIOA9_Handler
	def_default_handler    RTC_GPIOD_Handler
	
    def_default_handler    Default_Handler

    .end
