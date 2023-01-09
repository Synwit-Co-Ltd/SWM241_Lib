;******************************************************************************************************************************************
; 文件名称:	startup_SWM241.s
; 功能说明:	SWM241单片机的启动文件
; 技术支持:	http://www.synwit.com.cn/e/tool/gbook/?bid=1
; 注意事项:
; 版本日期: V1.0.0		2016年1月30日
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


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size	  EQU	 0x400;

				AREA	STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem	   SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size	   EQU	 0x000;

				AREA	HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem		SPACE   Heap_Size
__heap_limit


				PRESERVE8
				THUMB

; Vector Table Mapped to Address 0 at Reset

				AREA	RESET, DATA, READONLY
				EXPORT  __Vectors
				EXPORT  __Vectors_End
				EXPORT  __Vectors_Size

__Vectors	    DCD	 Stack_Mem + Stack_Size	; Top of Stack
				DCD	 Reset_Handler			 ; Reset Handler
				DCD	 NMI_Handler			   ; NMI Handler
				DCD	 HardFault_Handler		 ; Hard Fault Handler
				DCD	 0
				DCD	 0
				DCD	 0
				DCD	 0
				DCD	 0
				DCD	 0
				DCD	 0
				DCD	 SVC_Handler			   ; SVCall Handler
				DCD	 0
				DCD	 0
				DCD	 PendSV_Handler			; PendSV Handler
				DCD	 SysTick_Handler		   ; SysTick Handler

				; External Interrupts
				DCD	 GPIOA0_Handler
				DCD	 WDT_Handler
				DCD	 TIMR3_Handler
				DCD	 PWMNC_Handler
				DCD	 ADC_Handler
				DCD	 TIMR2_Handler
				DCD	 RTC_Handler
				DCD	 GPIOA1_Handler
				DCD	 SPI1_Handler
				DCD	 GPIOD1_Handler
				DCD	 CAN_Handler
				DCD	 UART0_Handler
				DCD	 I2C0_Handler
				DCD	 SPI0_Handler
				DCD	 GPIOA_Handler
				DCD	 TIMR1_Handler
				DCD	 GPIOB0_Handler
				DCD	 UART1_Handler
				DCD	 GPIOB_Handler
				DCD	 TIMR0_Handler
				DCD	 GPIOB1_Handler
				DCD	 PWMHE_Handler
				DCD	 UART2_Handler
				DCD	 BOD_Handler
				DCD	 I2C1_Handler
				DCD	 GPIOC0_Handler
				DCD	 GPIOC1_Handler
				DCD	 UART3_Handler
				DCD	 PWMHALT_Handler
				DCD	 GPIOC_Handler
				DCD	 GPIOD_Handler
				DCD	 DMA_Handler

__Vectors_End

__Vectors_Size 	EQU 	__Vectors_End - __Vectors



				AREA	|.text|, CODE, READONLY

; Reset Handler

Reset_Handler   PROC
				EXPORT  Reset_Handler			[WEAK]
				IMPORT  __main

				LDR	 R0, =__main
				BX	  R0
				ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler	 PROC
				EXPORT  NMI_Handler				[WEAK]
				B	   .
				ENDP

HardFault_Handler	PROC
				EXPORT  HardFault_Handler		[WEAK]
				B	   .
				ENDP

SVC_Handler	 PROC
				EXPORT  SVC_Handler			    [WEAK]
				B	   .
				ENDP

PendSV_Handler  PROC
				EXPORT  PendSV_Handler			[WEAK]
				B	   .
				ENDP

SysTick_Handler PROC
				EXPORT  SysTick_Handler		   	[WEAK]
				B	   .
				ENDP

GPIOA0_Handler PROC
				EXPORT  GPIOA0_Handler		   	[WEAK]
				B	   .
				ENDP

WDT_Handler PROC
			  	EXPORT WDT_Handler		  		[WEAK]
				B	   .
				ENDP

TIMR3_Handler PROC
				EXPORT  TIMR3_Handler		   	[WEAK]
				B	   .
				ENDP

PWMNC_Handler PROC
			  	EXPORT PWMNC_Handler		  		[WEAK]
				B	   .
				ENDP

ADC_Handler PROC
			  	EXPORT ADC_Handler		  		[WEAK]
				B	   .
				ENDP

TIMR2_Handler PROC
			  	EXPORT TIMR2_Handler		  	[WEAK]
				B	   .
				ENDP

RTC_Handler PROC
			  	EXPORT RTC_Handler		  		[WEAK]
				B	   .
				ENDP

GPIOA1_Handler PROC
			  	EXPORT GPIOA1_Handler		  	[WEAK]
				B	   .
				ENDP

SPI1_Handler PROC
			  	EXPORT SPI1_Handler		  	    [WEAK]
				B	   .
				ENDP

GPIOD1_Handler PROC
			  	EXPORT GPIOD1_Handler		  	[WEAK]
				B	   .
				ENDP

CAN_Handler PROC
			  	EXPORT CAN_Handler		  		[WEAK]
				B	   .
				ENDP

UART0_Handler PROC
			  	EXPORT UART0_Handler		  	[WEAK]
				B	   .
				ENDP

I2C0_Handler PROC
			  	EXPORT I2C0_Handler		  		[WEAK]
				B	   .
				ENDP

SPI0_Handler PROC
			  	EXPORT SPI0_Handler		  		[WEAK]
				B	   .
				ENDP

GPIOA_Handler PROC
			  	EXPORT GPIOA_Handler		  	[WEAK]
				B	   .
				ENDP

TIMR1_Handler PROC
			  	EXPORT TIMR1_Handler		  	[WEAK]
				B	   .
				ENDP

GPIOB0_Handler PROC
			  	EXPORT GPIOB0_Handler		  	[WEAK]
				B	   .
				ENDP

UART1_Handler PROC
			  	EXPORT UART1_Handler		  	[WEAK]
				B	   .
				ENDP

GPIOB_Handler PROC
				EXPORT  GPIOB_Handler		   	[WEAK]
				B	   .
				ENDP

TIMR0_Handler PROC
				EXPORT  TIMR0_Handler		   	[WEAK]
				B	   .
				ENDP

GPIOB1_Handler PROC
				EXPORT  GPIOB1_Handler		   	[WEAK]
				B	   .
				ENDP

PWMHE_Handler PROC
				EXPORT  PWMHE_Handler    	[WEAK]
				B	   .
				ENDP

UART2_Handler PROC
				EXPORT  UART2_Handler		   	[WEAK]
				B	   .
				ENDP

BOD_Handler PROC
				EXPORT  BOD_Handler		   		[WEAK]
				B	   .
				ENDP

I2C1_Handler PROC
				EXPORT  I2C1_Handler		   	[WEAK]
				B	   .
				ENDP

GPIOC0_Handler PROC
				EXPORT  GPIOC0_Handler		   	[WEAK]
				B	   .
				ENDP

GPIOC1_Handler PROC
				EXPORT  GPIOC1_Handler		   	[WEAK]
				B	   .
				ENDP

UART3_Handler PROC
				EXPORT  UART3_Handler		   	[WEAK]
				B	   .
				ENDP

PWMHALT_Handler PROC
				EXPORT  PWMHALT_Handler		[WEAK]
				B	   .
				ENDP
				ALIGN

GPIOC_Handler PROC
				EXPORT  GPIOC_Handler		   	[WEAK]
				B	   .
				ENDP
				ALIGN

GPIOD_Handler PROC
				EXPORT  GPIOD_Handler		   	[WEAK]
				B	   .
				ENDP
DMA_Handler PROC
				EXPORT  DMA_Handler		   		[WEAK]
				B	   .
				ENDP
				ALIGN



; User Initial Stack & Heap

				IF	  :DEF:__MICROLIB

				EXPORT  __initial_sp
				EXPORT  __heap_base
				EXPORT  __heap_limit

				ELSE

				IMPORT  __use_two_region_memory
				EXPORT  __user_initial_stackheap
__user_initial_stackheap

				LDR	 R0, =  Heap_Mem
				LDR	 R1, =(Stack_Mem + Stack_Size)
				LDR	 R2, = (Heap_Mem +  Heap_Size)
				LDR	 R3, = Stack_Mem
				BX	 LR

				ALIGN

				ENDIF


				END
