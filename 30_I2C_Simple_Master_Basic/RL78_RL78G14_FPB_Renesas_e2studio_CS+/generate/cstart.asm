                                                                        
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                          
; Copyright (C) 2014, 2018 Renesas Electronics Corporation
; RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
; This program must be used solely for the purpose for which
; it was furnished by Renesas Electronics Corporation. No part of this
; program may be reproduced or disclosed to others, in any
; form, without the prior written permission of Renesas Electronics
; Corporation.

;   NOTE       : THIS IS A TYPICAL EXAMPLE.

$IFNDEF __RENESAS_VERSION__
__RENESAS_VERSION__	.EQU	0x01000000
$ENDIF

	.extern	_PowerON_Reset_PC

	.public	_start
	.public	_INIT_BSS
	.public	_INIT_DATA
	.public _exit

;-----------------------------------------------------------------------------
;	RAM section
;-----------------------------------------------------------------------------
.SECTION .dataR, DATA
.SECTION .sdataR, DATA
; .SECTION .datafR, DATAF
; .SECTION .textfR, TEXTF

$IF (__RENESAS_VERSION__ < 0x01010000)	; for CC-RL V1.00
;-----------------------------------------------------------------------------
;	stack area
;-----------------------------------------------------------------------------
; !!! [CAUTION] !!!
; Set up stack size suitable for a project.
.SECTION .stack_bss, BSS
_stackend:
	.DS     0x200
_stacktop:
$ENDIF

;-----------------------------------------------------------------------------
;	RESET vector
;-----------------------------------------------------------------------------
_start	.VECTOR	0

;-----------------------------------------------------------------------------
;	startup
;-----------------------------------------------------------------------------
.SECTION .text, TEXT
_start:
	.STACK _start = 0

	;--------------------------------------------------
	; setting register bank
	;--------------------------------------------------
;	SEL	RB0

	;--------------------------------------------------
	; setting mirror area
	;--------------------------------------------------
;	ONEB    !PMC		; mirror area = 10000-1FFFFH

	;--------------------------------------------------
	; setting the stack pointer
	;--------------------------------------------------
$IF (__RENESAS_VERSION__ >= 0x01010000)
	MOVW	SP,#LOWW(__STACK_ADDR_START)
$ELSE	; for CC-RL V1.00
	MOVW	SP,#LOWW(_stacktop)
$ENDIF

	;--------------------------------------------------
	; initializing stack area
	;--------------------------------------------------
$IF (__RENESAS_VERSION__ >= 0x01010000)
	MOVW	HL,#LOWW(__STACK_ADDR_END)
$ELSE	; for CC-RL V1.00
	MOVW	HL,#LOWW(_stackend)
$ENDIF
	MOVW	AX,SP
	SUBW	AX,HL		; SUBW  AX,#LOWW _@STEND
	BNH	$.LSTINIT3	; goto end
	SHRW	AX,5		; loop count for 32 byte transfer
	MOVW	BC,AX
	CLRW	AX
.LSTINIT1:
	CMPW	AX,BC
	BZ	$.LSTINIT2
	MOVW	[HL],AX
	MOVW	[HL+2],AX
	MOVW	[HL+4],AX
	MOVW	[HL+6],AX
	MOVW	[HL+8],AX
	MOVW	[HL+10],AX
	MOVW	[HL+12],AX
	MOVW	[HL+14],AX
	MOVW	[HL+16],AX
	MOVW	[HL+18],AX
	MOVW	[HL+20],AX
	MOVW	[HL+22],AX
	MOVW	[HL+24],AX
	MOVW	[HL+26],AX
	MOVW	[HL+28],AX
	MOVW	[HL+30],AX
	XCHW	AX,HL
	ADDW	AX,#0x20
	XCHW	AX,HL
	DECW	BC
	BR	$.LSTINIT1
.LSTINIT2:
	MOVW	AX,SP
	CMPW	AX,HL
	BZ	$.LSTINIT3	; goto end
	CLRW	AX
	MOVW	[HL],AX
	INCW	HL
	INCW	HL
	BR	$.LSTINIT2
.LSTINIT3:

	;--------------------------------------------------
	; jump PowerON_Reset_PC function
	;--------------------------------------------------
	BR	!!_PowerON_Reset_PC

;--------------------------------------------------
; initializing BSS
;--------------------------------------------------
.SECTION .textf, TEXTF
_INIT_BSS:
	.STACK _INIT_BSS = 4

	; clear external variables which doesn't have initial value (near)
	MOVW	HL,#LOWW(STARTOF(.bss))
	MOVW	AX,#LOWW(STARTOF(.bss) + SIZEOF(.bss))
	BR	$.L2_BSS
.L1_BSS:
	MOV	[HL+0],#0
	INCW	HL
.L2_BSS:
	CMPW	AX,HL
	BNZ	$.L1_BSS

	; clear saddr variables which doesn't have initial value
	MOVW	HL,#LOWW(STARTOF(.sbss))
	MOVW	AX,#LOWW(STARTOF(.sbss) + SIZEOF(.sbss))
	BR	$.L2_SBSS
.L1_SBSS:
	MOV	[HL+0],#0
	INCW	HL
.L2_SBSS:
	CMPW	AX,HL
	BNZ	$.L1_SBSS

	; clear external variables which doesn't have initial value (far)
;	MOV	ES,#HIGHW(STARTOF(.bssf))
;	MOVW	HL,#LOWW(STARTOF(.bssf))
;	MOVW	AX,#LOWW(STARTOF(.bssf) + SIZEOF(.bssf))
;	BR	$.L2_BSSF
;.L1_BSSF:
;	MOV	ES:[HL+0],#0
;	INCW	HL
;.L2_BSSF:
;	CMPW	AX,HL
;	BNZ	$.L1_BSSF

	RET

;--------------------------------------------------
; ROM data copy
;--------------------------------------------------
.SECTION .textf, TEXTF
_INIT_DATA:
	.STACK _INIT_DATA = 4

	; copy external variables having initial value (near)
	MOV	ES,#HIGHW(STARTOF(.data))
	MOVW	BC,#LOWW(SIZEOF(.data))
	BR	$.L2_DATA
.L1_DATA:
	DECW	BC
	MOV	A,ES:LOWW(STARTOF(.data))[BC]
	MOV	LOWW(STARTOF(.dataR))[BC],A
.L2_DATA:
	CLRW	AX
	CMPW	AX,BC
	BNZ	$.L1_DATA

	; copy saddr variables having initial value
	MOV	ES,#HIGHW(STARTOF(.sdata))
	MOVW	BC,#LOWW(SIZEOF(.sdata))
	BR	$.L2_SDATA
.L1_SDATA:
	DECW	BC
	MOV	A,ES:LOWW(STARTOF(.sdata))[BC]
	MOV	LOWW(STARTOF(.sdataR))[BC],A
.L2_SDATA:
	CLRW	AX
	CMPW	AX,BC
	BNZ	$.L1_SDATA

	; copy external variables having initial value (far)
;	MOVW	BC,#LOWW(SIZEOF(.dataf))
;	BR	$.L2_DATAF
;.L1_DATAF:
;	DECW	BC
;	MOV	ES,#HIGHW(STARTOF(.dataf))
;	MOV	A,ES:LOWW(STARTOF(.dataf))[BC]
;	MOV	ES,#HIGHW(STARTOF(.datafR))
;	MOV	ES:LOWW(STARTOF(.datafR))[BC],A
;.L2_DATAF:
;	CLRW	AX
;	CMPW	AX,BC
;	BNZ	$.L1_DATAF

	; copy .text to RAM
;	MOV	C,#HIGHW(STARTOF(.textf))
;	MOVW	HL,#LOWW(STARTOF(.textf))
;	MOVW	DE,#LOWW(STARTOF(.textfR))
;	BR	$.L2_TEXT
;.L1_TEXT:
;	MOV	A,C
;	MOV	ES,A
;	MOV	A,ES:[HL]
;	MOV	[DE],A
;	INCW	DE
;	INCW	HL
;	CLRW	AX
;	CMPW	AX,HL
;	SKNZ
;	INC	C
;.L2_TEXT:
;	MOVW	AX,HL
;	CMPW	AX,#LOWW(STARTOF(.text) + SIZEOF(.text))
;	BNZ	$.L1_TEXT

	RET

;--------------------------------------------------
; exit function
;--------------------------------------------------
.SECTION .textf, TEXTF
_exit:
	.STACK _exit = 4
	BR	$_exit

;-----------------------------------------------------------------------------
;	section
;-----------------------------------------------------------------------------
$IF (__RENESAS_VERSION__ >= 0x01010000)
.SECTION .RLIB, TEXTF
.L_section_RLIB:
.SECTION .SLIB, TEXTF
.L_section_SLIB:
$ENDIF
.SECTION .textf, TEXTF
.L_section_textf:
.SECTION .const, CONST
.L_section_const:
.SECTION .constf, CONSTF
.L_section_constf:
.SECTION .data, DATA
.L_section_data:
;.SECTION .dataf, DATAF
;.L_section_dataf:
.SECTION .sdata, SDATA
.L_section_sdata:
.SECTION .bss, BSS
.L_section_bss:
;.SECTION .bssf, BSSF
;.L_section_bssf:
.SECTION .sbss, SBSS
.L_section_sbss:
