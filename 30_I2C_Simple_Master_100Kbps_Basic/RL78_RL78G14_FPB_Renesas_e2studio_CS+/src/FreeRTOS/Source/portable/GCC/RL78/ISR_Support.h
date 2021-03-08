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


#include "FreeRTOSConfig.h"

	 r8    =    0xffef0
	 r9    =    0xffef1
	r10    =    0xffef2
	r11    =    0xffef3
	r12    =    0xffef4
	r13    =    0xffef5
	r14    =    0xffef6
	r15    =    0xffef7
	r16    =    0xffee8
	r17    =    0xffee9
	r18    =    0xffeea
	r19    =    0xffeeb
	r20    =    0xffeec
	r21    =    0xffeed
	r22    =    0xffeee
	r23    =    0xffeef
	r24    =    0xffee0
	r25    =    0xffee1
	r26    =    0xffee2
	r27    =    0xffee3
	r28    =    0xffee4
	r29    =    0xffee5
	r30    =    0xffee6
	r31    =    0xffee7

; Variables used by scheduler
;------------------------------------------------------------------------------
	.extern    _pxCurrentTCB
	.extern    _usCriticalNesting
	.extern    _ucInterruptStackNesting
	.extern    _pxInterruptedTaskStack
	.extern    _stack

;------------------------------------------------------------------------------
;   portSAVE_CONTEXT MACRO
;   Saves the context of the general purpose registers, CS and ES registers,
;   the usCriticalNesting Value and the Stack Pointer of the active Task
;   onto the task stack
;------------------------------------------------------------------------------
.macro portSAVE_CONTEXT
	SEL       RB0                   ; Save the AX register of bank 0.
	PUSH      AX
;	Switch stack pointers.  Interrupts which call FreeRTOS API functions
;	ending with FromISR cannot be nested.  On the other hand, high priority
;	interrupts which don't call any FreeRTOS API functions can be nested.
	MOVW      AX, SP
	MOVW      SP, #_stack                   ; Set stack pointer.
	ONEB      !_ucInterruptStackNesting     ; Change the value: 0 --> 1.
;	Don't enable nested interrupts from the beginning of interrupt until
;	the completion of switching the stack from task stacks to interrupt
;	stack.  If it is enabled before switching the stack to interrupt
;	stack, each task stack need additional space for nested interrupt.
;	Moreover ucInterruptStackNesting has to be modified in the same DI
;	period so that the next switching of the stack is perfomed correctly.
	EI
	SUBW      AX, #(28 - 2)         ; 28 is all context, 2 is pushed AX
	XCHW      AX, DE                ; Save the DE, BC and HL registers of bank 0.
	MOVW      [DE+22], AX
	MOVW      AX, BC
	MOVW      [DE+24], AX
	MOVW      AX, HL
	MOVW      [DE+20], AX
	MOV       A, CS                 ; Save CS register.
	MOV       X, A
	MOV       A, ES                 ; Save ES register.
	MOVW      [DE+18], AX
;	Save the other register banks - only necessary in the GCC port.
	MOVW      AX, r8
	MOVW      [DE+16], AX
	MOVW      AX, r10
	MOVW      [DE+14], AX
	MOVW      AX, r12
	MOVW      [DE+12], AX
	MOVW      AX, r14
	MOVW      [DE+10], AX
	MOVW      AX, r16
	MOVW      [DE+8], AX
	MOVW      AX, r18
	MOVW      [DE+6], AX
	MOVW      AX, r20
	MOVW      [DE+4], AX
	MOVW      AX, r22
	MOVW      [DE+2], AX
;	Registers in bank 3 are for ISR use only so don't need saving.
	MOVW      AX, !_usCriticalNesting   ; Save the usCriticalNesting value.
	MOVW      [DE], AX
	MOVW      HL, !_pxCurrentTCB    ; Save the task Stack Pointer.
	MOVW      AX, DE
	MOVW      [HL], AX
;	Set CS register to 0 - only necessary in the GCC port.
	MOV       CS, #0
	.endm
;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
;   portRESTORE_CONTEXT MACRO
;   Restores the task Stack Pointer then use this to restore usCriticalNesting,
;   general purpose registers and the CS and ES registers of the selected task
;   from the task stack
;------------------------------------------------------------------------------
.macro portRESTORE_CONTEXT MACRO
	SEL       RB0
	MOVW      HL, !_pxCurrentTCB    ; Restore the task Stack Pointer.
	MOVW      AX, [HL]
	MOVW      DE, AX
	MOVW      AX, [DE]
	MOVW      !_usCriticalNesting, AX   ; Restore usCriticalNesting value.
;	Restore the alternative register banks (r8~r23) - only necessary in the GCC port.
	MOVW      AX, [DE+2]
	MOVW      r22, AX
	MOVW      AX, [DE+4]
	MOVW      r20, AX
	MOVW      AX, [DE+6]
	MOVW      r18, AX
	MOVW      AX, [DE+8]
	MOVW      r16, AX
	MOVW      AX, [DE+10]
	MOVW      r14, AX
	MOVW      AX, [DE+12]
	MOVW      r12, AX
	MOVW      AX, [DE+14]
	MOVW      r10, AX
	MOVW      AX, [DE+16]
	MOVW      r8, AX
;	Register bank 3 is dedicated for interrupts use so is not saved or restored.
	MOVW      AX, [DE+18]
	MOV       ES, A                 ; Restore the ES register.
	MOV       A, X
	MOV       CS, A                 ; Restore the CS register.
	MOVW      AX, [DE+20]           ; Restore the HL, BC and DE registers of bank 0.
	MOVW      HL, AX
	MOVW      AX, [DE+24]
	MOVW      BC, AX
	MOVW      AX, [DE+22]
	XCHW      AX, DE
;	Don't enable nested interrupts from the completion of switching the
;	stack from interrupt stack to task stacks until the RETI or RETB
;	instruction is executed.  If it is enabled after switching the stack,
;	each task stack needs additional space for nested interrupts.
;	Moreover ucInterruptStackNesting has to be modified in the same DI
;	period so that the next switching of the stack is perfomed correctly.
	ADDW      AX, #(28 - 2)         ; 28 is all context, 2 is pushed AX
	DI
	MOVW      SP, AX
	CLRB      !_ucInterruptStackNesting ; Change the value: 1 --> 0.
	POP       AX                    ; Restore the AX register of bank 0.
	.endm
;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
;   portSAVE_CONTEXT_C MACRO
;   Saves the context of the general purpose registers, CS and ES registers,
;   the usCriticalNesting Value and the Stack Pointer of the active Task
;   onto the task stack
;------------------------------------------------------------------------------
.macro portSAVE_CONTEXT_C MACRO
;   It is assumed that the following registers have been saved and
;   the stack pointer has been switched as follows at the beginning
;   of an interrupt function.
;	SEL		RB0
;	PUSH	AX
;	Switch stack pointers.  Interrupts which call FreeRTOS API functions
;	ending with FromISR cannot be nested.  On the other hand, high priority
;	interrupts which don't call any FreeRTOS API functions can be nested.
;	MOVW	AX, SP
;	MOVW	SP, #_stack
;	ONEB	!_ucInterruptStackNesting
;	Don't enable nested interrupts from the beginning of interrupt until
;	the completion of switching the stack from task stacks to interrupt
;	stack.  If it is enabled before switching the stack to interrupt
;	stack, each task stack need additional space for nested interrupt.
;	Moreover ucInterruptStackNesting has to be modified in the same DI
;	period so that the next switching of the stack is perfomed correctly.
;	EI or without EI
;	SUBW      AX, #(28 - 2)         ; 28 is all context, 2 is pushed AX
;	XCHW      AX, DE
;	MOVW      [DE+22], AX
;	MOVW      AX, BC
;	MOVW      [DE+24], AX
	MOVW      AX, HL                ; Save the HL register of bank 0.
	MOVW      [DE+20], AX
	MOV       A, CS                 ; Save CS register.
	MOV       X, A
	MOV       A, ES                 ; Save ES register.
	MOVW      [DE+18], AX
;	Save the other register banks (r8~r23) - only necessary in the GCC port.
	MOVW      AX, r8
	MOVW      [DE+16], AX
	MOVW      AX, r10
	MOVW      [DE+14], AX
	MOVW      AX, r12
	MOVW      [DE+12], AX
	MOVW      AX, r14
	MOVW      [DE+10], AX
	MOVW      AX, r16
	MOVW      [DE+8], AX
	MOVW      AX, r18
	MOVW      [DE+6], AX
	MOVW      AX, r20
	MOVW      [DE+4], AX
	MOVW      AX, r22
	MOVW      [DE+2], AX
;	Registers in bank 3 are for ISR use only so don't need saving.
	MOVW      AX, !_usCriticalNesting   ; Save the usCriticalNesting value.
	MOVW      [DE], AX
	MOVW      HL, !_pxCurrentTCB    ; Save the task Stack Pointer.
	MOVW      AX, DE
	MOVW      [HL], AX
;	Set CS register to 0 - only necessary in the GCC port.
	MOV       CS, #0
	.endm
;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
;   portSAVE_REGISTERS_C MACRO
;   Saves the content of the general purpose registers, CS and ES registers.
;------------------------------------------------------------------------------
.macro portSAVE_REGISTERS_C MACRO
;   It is assumed that the following registers have been saved and
;   the stack pointer has been switched as follows at the beginning
;   of an interrupt function.
;	SEL       RB0
;	PUSH      AX
;	Switch stack pointers.  Interrupts which call FreeRTOS API functions
;	ending with FromISR cannot be nested.  On the other hand, high priority
;	interrupts which don't call any FreeRTOS API functions can be nested.
;	MOVW      AX, SP
;	CMP0      !_ucInterruptStackNesting
;	SKNZ
;	MOVW      SP, _stack
;	INC       !_ucInterruptStackNesting
;	Don't enable nested interrupts from the beginning of interrupt until
;	the completion of switching the stack from task stacks to interrupt
;	stack.  If it is enabled before switching the stack to interrupt
;	stack, each task stack need additional space for nested interrupt.
;	Moreover ucInterruptStackNesting has to be modified in the same DI
;	period so that the next switching of the stack is perfomed correctly.
;	(On the other hand, referring ucInterruptStackNesting as follows
;	works with no problem.)
;	EI or without EI
;	PUSH      BC
;	PUSH      DE
	PUSH      HL                    ; Save the HL register of bank 0.
	MOV       A, CS                 ; Save CS register.
	MOV       X, A
	MOV       A, ES                 ; Save ES register.
	PUSH      AX
	SEL       RB1                   ; Save the register bank1 (r8~r15) -
	PUSH      AX                    ; only necessary in the GCC port.
	PUSH      BC
	PUSH      DE
	PUSH      HL
	SEL       RB0
	MOVW      AX, DE
	CMP       !_ucInterruptStackNesting, #1
	SKNZ
	MOVW      !_pxInterruptedTaskStack, AX  ; Save the Stack Pointer.
;	Set CS register to 0 - only necessary in the GCC port.
	MOV       CS, #0
	.endm
;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
;   portRESTORE_REGISTERS MACRO
;   Restores the general purpose registers and the CS and ES registers.
;------------------------------------------------------------------------------
.macro portRESTORE_REGISTERS MACRO
	SEL       RB1                   ; Restore the register bank1 (r8~r15) -
	POP       HL                    ; only necessary in the GCC port.
	POP       DE
	POP       BC
	POP       AX
	SEL       RB0
	POP       AX
	MOV       ES, A                 ; Restore the ES register.
	MOV       A, X
	MOV       CS, A                 ; Restore the CS register.
	POP       HL                    ; Restore the HL, DE and BC registers of bank 0.
	POP       DE
	POP       BC
;	Don't enable nested interrupts from the completion of switching the
;	stack from interrupt stack to task stacks until the RETI or RETB
;	instruction is executed.  If it is enabled after switching the stack,
;	each task stack needs additional space for nested interrupts.
;	Moreover ucInterruptStackNesting has to be modified in the same DI
;	period so that the next switching of the stack is perfomed correctly.
	MOVW      AX, !_pxInterruptedTaskStack
	DI
	DEC       !_ucInterruptStackNesting
	SKNZ
	MOVW      SP, AX                ; Restore the Stack Pointer.
	POP       AX                    ; Restore the AX register of bank 0.
	.endm
;------------------------------------------------------------------------------

