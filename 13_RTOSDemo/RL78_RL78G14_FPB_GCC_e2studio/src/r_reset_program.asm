
///*********************************************************************************************************************
//* DISCLAIMER
//* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
//* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
//* applicable laws, including copyright laws. 
//* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
//* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
//* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
//* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
//* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
//* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
//* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
//* following link:
//* http://www.renesas.com/disclaimer
//*
//* Copyright (C) 2011, 2020 Renesas Electronics Corporation. All rights reserved.
//*********************************************************************************************************************/

///*
//**********************************************************************************************************************
//* File Name    : r_reset_program.asm
//* Version      : CodeGenerator for RL78/G14 V2.05.05.01 [25 Nov 2020]
//* Device(s)    : R5F104ML
//* Tool-Chain   : gccrl78
//* Description  : This is start up file for RL78.
//* Creation Date: 
//**********************************************************************************************************************
//*/

	/*reset_program.asm*/

	.list

	.global _PowerON_Reset  /*! global Start routine */
	.short _PowerON_Reset

#ifdef CPPAPP    
___dso_handle:
    .global ___dso_handle
#endif
	
	.global __rl78_fini
		__rl78_fini:
		br $__rl78_fini

	.extern _HardwareSetup /*! external Sub-routine to initialise Hardware*/
	.extern _data
	.extern _mdata
	.extern _ebss
	.extern _bss
	
	.extern _edata
	.extern _main 
	.extern _stack
	.extern _exit

	/*  ;; HL = start of list
        ;; DE = end of list
        ;; BC = step direction (+2 or -2)
	*/
	
	.text

/* call to _PowerON_Reset */
_PowerON_Reset:
/* initialise user stack pointer */
	movw sp,#_stack /* Set stack pointer */


/* load data section from ROM to RAM */	
;; block move to initialize .data

        ;; we're copying from 00:[_romdatastart] to 0F:[_datastart]
        ;; and our data is not in the mirrored area.
    mov     es, #0
#ifndef __RL78_G10__
    sel     rb0         /*  ;;bank 0 */
#endif

    movw    de, #_mdata	/* src ROM address of data section in de */
    movw    hl, #_data 	/* dest start RAM address of data section in hl */

#ifndef __RL78_G10__		
    sel     rb1              /* bank 1 */
    movw    hl, #_data 	/* dest start RAM address of data section in hl */
#endif
    movw    ax, #_edata   /*    ;; size of romdata section in ax */
    subw    ax,hl  		/* store data size */
    shrw	ax,1
1:
    cmpw    ax, #0		   /* check if end of data */
    bz      $1f
    decw    ax
#ifdef __RL78_G10__
    movw    bc, ax          /* save ax value in bc */
#endif

#ifndef __RL78_G10__
    sel     rb0             /* bank 0 */
#endif
    movw    ax, es:[de]
    movw    [hl], ax
    incw    de
    incw	de
    incw    hl
	incw	hl
#ifndef __RL78_G10__
    sel     rb1         /* bank 1  -  compare and decrement*/
#endif

#ifdef __RL78_G10__
    movw     ax, bc     /* restore ax value */
#endif
    br      $1b
1:

	
/* bss initialisation : zero out bss */
#ifndef __RL78_G10__
    sel     rb0         /* bank 0 */
    movw    ax, #0		/* load AX reg with zero */
    sel     rb1         /* bank 1 */
    movw    ax, #_ebss  /* store the end address (size of) bss section in ax */
	movw    hl, #_bss  	/* store the start address of bss in hl */
    subw    ax,hl ;; store data size
    shrw    ax,1
#endif

#ifdef __RL78_G10__
    movw    hl, #_bss   /* store the start address of bss in hl */
    movw    ax, #_ebss  /* store the end address (size of) bss section in ax */
    subw    ax,hl ;; store data size
    shrw    ax,1
#endif

1:
    cmpw    ax, #0
    bz      $1f
    decw    ax
#ifdef __RL78_G10__
    movw     bc, ax        /* save ax value in bc */
    movw    ax, #0
#endif
#ifndef __RL78_G10__
    sel     rb0         /* bank 0 */
#endif
    movw    [hl], ax
    incw    hl
    incw	hl
#ifdef __RL78_G10__
    movw    ax, bc        /* restore ax value */
#endif
#ifndef __RL78_G10__
    sel     rb1
#endif
    br      $1b
1:
#ifndef __RL78_G10__
    sel     rb0             /* bank 0 */
#endif

/* call the hardware initialiser */
	call	!!_HardwareSetup
	nop


/* start user program */

    movw    ax, #0
    push    ax      /* envp */
    push    ax      /* argv */
    push    ax      /* argc */
    call	!!_main


/* call to exit*/
_exit:
     br	$_exit

	.end
