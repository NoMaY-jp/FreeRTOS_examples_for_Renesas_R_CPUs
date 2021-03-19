;/*
; * FreeRTOS Kernel V10.4.3
; * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
; *
; * Permission is hereby granted, free of charge, to any person obtaining a copy of
; * this software and associated documentation files (the "Software"), to deal in
; * the Software without restriction, including without limitation the rights to
; * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
; * the Software, and to permit persons to whom the Software is furnished to do so,
; * subject to the following conditions:
; *
; * The above copyright notice and this permission notice shall be included in all
; * copies or substantial portions of the Software.
; *
; * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
; * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
; * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
; * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
; * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
; *
; * https://www.FreeRTOS.org
; * https://github.com/FreeRTOS
; *
; * 1 tab == 4 spaces!
; */

$include "ISR_Support.h"

	portPSW_ISP_SYSCALL_INTERRUPT_DISABLE .set (2 << 1)

;;	CS    .SET                  0xFFFFC
;;	ES    .SET                  0xFFFFD

	.PUBLIC    _vPortYield
	.PUBLIC    _vPortStartFirstTask
	.PUBLIC    _vPortTickISR
	.PUBLIC    _vPortFreeRTOSInterruptCommonHandler_C
	.PUBLIC    _vPortInterruptCommonHandler_C

	.EXTERN    _vTaskSwitchContext
	.EXTERN    _xTaskIncrementTick

; FreeRTOS yield handler.  This is installed as the BRK software interrupt
; handler.  This BRK handler is called not only outside a critical section
; but also inside a critical section.  The ISP bits value of PSW depends on
; each case but the value is saved by BRK instruction as a part of PSW and
; restored by RETB instruction as a part of PSW.  Of course, FreeRTOS is
; designed carefully that such call of the yield handler inside a critical
; section doesn't make internal data structures corrupted.
	.SECTION .text,TEXT
_vPortYield:
	.STACK _vPortYield = 16
;	The following code can be optimized using set1 and clr1 instructions as follows.
;	push	ax				        ; Mask the tick interrupt (kernel interrupt)
;	mov		a, psw			        ; and user interrupts which call FreeRTOS
;	and		a, #0b11111001          ; API functions ending with FromISR (i.e.
;	or		a, #portPSW_ISP_SYSCALL_INTERRUPT_DISABLE ; SYSCALL interrupts) while
;	mov		psw, a			        ; the kernel structures are being accessed
;	pop		ax				        ; and interrupt dedicated stack is being used.
$if (portPSW_ISP_SYSCALL_INTERRUPT_DISABLE & 0b00000010) == 0
	clr1	psw.1
$else
	set1	psw.1
$endif
$if (portPSW_ISP_SYSCALL_INTERRUPT_DISABLE & 0b00000100) == 0
	clr1	psw.2
$else
	set1	psw.2
$endif
	portSAVE_CONTEXT		        ; Save the context of the current task.
							        ; Additionally re-enable high priority interrupts
							        ; but any FreeRTOS API functions cannot be called
							        ; in its ISRs.
	call@	_vTaskSwitchContext     ; Call the scheduler to select the next task.
	portRESTORE_CONTEXT		        ; Restore the context of the next task to run.
	retb


; Starts the scheduler by restoring the context of the task that will execute
; first.
	.SECTION .text,TEXT
_vPortStartFirstTask:
	.STACK _vPortStartFirstTask = 4
	portRESTORE_CONTEXT	            ; Restore the context of whichever task the ...
	reti					        ; An interrupt stack frame is used so the task
                                    ; is started using a RETI instruction.

; FreeRTOS tick handler.  This is installed as the interval timer interrupt
; handler.
	.SECTION .text,TEXT
_vPortTickISR:
	.STACK _vPortTickISR = 16
	portSAVE_CONTEXT		        ; Save the context of the current task.
							        ; Additionally re-enable high priority interrupts
							        ; but any FreeRTOS API functions cannot be called
							        ; in its ISRs.
	call@	_xTaskIncrementTick     ; Call the timer tick function.
	cmp0	x			            ; Check the return value is zero or not(== one).
	skz						        ; Skip the scheduler call if the value is zero.
	call@	_vTaskSwitchContext     ; Call the scheduler to select the next task.
	portRESTORE_CONTEXT		        ; Restore the context of the next task to run.
	reti


; Common interrupt handler.
	.SECTION .text,TEXT
_vPortFreeRTOSInterruptCommonHandler_C:
	.STACK _vPortFreeRTOSInterruptCommonHandler_C = 2
	; Argument: BC is the target interrupt handler address.
	;           DE is the stack pointer value before switching stack.
	portSAVE_CONTEXT_C		       ; Save the context of the current task.
	; Call the target interrupt handler.
	mov		cs, #0
	call	bc					   ; Call the target interrupt handler.
	portRESTORE_CONTEXT		       ; Restore the context of the next task to run.
	reti


; Common interrupt handler.
	.SECTION .text,TEXT
_vPortInterruptCommonHandler_C:
	.STACK _vPortFreeRTOSInterruptCommonHandler_C = 0
	; Argument: BC is the target interrupt handler address.
	;           DE is the stack pointer value before switching stack, or meaningless
	;           depending on the ucInterruptStackNesting value.
	portSAVE_REGISTERS_C	       ; Save the content of the registers.
	; Call the target interrupt handler.
	mov		cs, #0
	call	bc					   ; Call the target interrupt handler.
	portRESTORE_REGISTERS	       ; Restore the content of the registers.
	reti


; Install the interrupt handlers

	_vPortTickISR    .VECTOR    configTICK_VECTOR

	_vPortYield      .VECTOR    0x7E

