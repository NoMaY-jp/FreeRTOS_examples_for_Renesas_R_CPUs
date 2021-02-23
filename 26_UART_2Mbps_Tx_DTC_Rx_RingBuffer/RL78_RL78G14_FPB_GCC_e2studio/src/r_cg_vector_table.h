#ifndef VECTOR_TABLE_H
#define VECTOR_TABLE_H

/* Defines for user */

/*
 * INT_IT (0x38 or 0x3C)
 */
#if INTIT_vect == 0x38

#define R_Vector_0x38 vPortTickISR

#elif INTIT_vect == 0x3C

#define R_Vector_0x3C vPortTickISR

#else

#error Neither vector 0x38 nor vector 0x3C is available for the tick interrupt.

#endif

void vPortTickISR(void) __attribute__ ((interrupt));

/*
 * INT_BRK_I (0x7E)
 */
#define R_Vector_0x7E vPortYield

void vPortYield(void) __attribute__ ((interrupt));

/*
 * INT_WDTI (0x04)
 */
#define R_Vector_0x4 u_wdt_interrupt

void u_wdt_interrupt(void) __attribute__ ((interrupt));

/* Defines for default */

#ifndef R_Vector_0x4
#define R_Vector_0x4  R_Dummy
#endif
#ifndef R_Vector_0x6
#define R_Vector_0x6  R_Dummy
#endif
#ifndef R_Vector_0x8
#define R_Vector_0x8  R_Dummy
#endif
#ifndef R_Vector_0xA
#define R_Vector_0xA  R_Dummy
#endif
#ifndef R_Vector_0xC
#define R_Vector_0xC  R_Dummy
#endif
#ifndef R_Vector_0xE
#define R_Vector_0xE  R_Dummy
#endif

#ifndef R_Vector_0x10
#define R_Vector_0x10 R_Dummy
#endif
#ifndef R_Vector_0x12
#define R_Vector_0x12 R_Dummy
#endif
#ifndef R_Vector_0x14
#define R_Vector_0x14 R_Dummy
#endif
#ifndef R_Vector_0x16
#define R_Vector_0x16 R_Dummy
#endif
#ifndef R_Vector_0x18
#define R_Vector_0x18 R_Dummy
#endif
#ifndef R_Vector_0x1A
#define R_Vector_0x1A R_Dummy
#endif
#ifndef R_Vector_0x1C
#define R_Vector_0x1C R_Dummy
#endif
#ifndef R_Vector_0x1E
#define R_Vector_0x1E R_Dummy
#endif

#ifndef R_Vector_0x20
#define R_Vector_0x20 R_Dummy
#endif
#ifndef R_Vector_0x22
#define R_Vector_0x22 R_Dummy
#endif
#ifndef R_Vector_0x24
#define R_Vector_0x24 R_Dummy
#endif
#ifndef R_Vector_0x26
#define R_Vector_0x26 R_Dummy
#endif
#ifndef R_Vector_0x28
#define R_Vector_0x28 R_Dummy
#endif
#ifndef R_Vector_0x2A
#define R_Vector_0x2A R_Dummy
#endif
#ifndef R_Vector_0x2C
#define R_Vector_0x2C R_Dummy
#endif
#ifndef R_Vector_0x2E
#define R_Vector_0x2E R_Dummy
#endif

#ifndef R_Vector_0x30
#define R_Vector_0x30 R_Dummy
#endif
#ifndef R_Vector_0x32
#define R_Vector_0x32 R_Dummy
#endif
#ifndef R_Vector_0x34
#define R_Vector_0x34 R_Dummy
#endif
#ifndef R_Vector_0x36
#define R_Vector_0x36 R_Dummy
#endif
#ifndef R_Vector_0x38
#define R_Vector_0x38 R_Dummy
#endif
#ifndef R_Vector_0x3A
#define R_Vector_0x3A R_Dummy
#endif
#ifndef R_Vector_0x3C
#define R_Vector_0x3C R_Dummy
#endif
#ifndef R_Vector_0x3E
#define R_Vector_0x3E R_Dummy
#endif

#ifndef R_Vector_0x40
#define R_Vector_0x40 R_Dummy
#endif
#ifndef R_Vector_0x42
#define R_Vector_0x42 R_Dummy
#endif
#ifndef R_Vector_0x44
#define R_Vector_0x44 R_Dummy
#endif
#ifndef R_Vector_0x46
#define R_Vector_0x46 R_Dummy
#endif
#ifndef R_Vector_0x48
#define R_Vector_0x48 R_Dummy
#endif
#ifndef R_Vector_0x4A
#define R_Vector_0x4A R_Dummy
#endif
#ifndef R_Vector_0x4C
#define R_Vector_0x4C R_Dummy
#endif
#ifndef R_Vector_0x4E
#define R_Vector_0x4E R_Dummy
#endif

#ifndef R_Vector_0x50
#define R_Vector_0x50 R_Dummy
#endif
#ifndef R_Vector_0x52
#define R_Vector_0x52 R_Dummy
#endif
#ifndef R_Vector_0x54
#define R_Vector_0x54 R_Dummy
#endif
#ifndef R_Vector_0x56
#define R_Vector_0x56 R_Dummy
#endif
#ifndef R_Vector_0x58
#define R_Vector_0x58 R_Dummy
#endif
#ifndef R_Vector_0x5A
#define R_Vector_0x5A R_Dummy
#endif
#ifndef R_Vector_0x5C
#define R_Vector_0x5C R_Dummy
#endif
#ifndef R_Vector_0x5E
#define R_Vector_0x5E R_Dummy
#endif

#ifndef R_Vector_0x60
#define R_Vector_0x60 R_Dummy
#endif
#ifndef R_Vector_0x62
#define R_Vector_0x62 R_Dummy
#endif
#ifndef R_Vector_0x64
#define R_Vector_0x64 R_Dummy
#endif
#ifndef R_Vector_0x66
#define R_Vector_0x66 R_Dummy
#endif
#ifndef R_Vector_0x68
#define R_Vector_0x68 R_Dummy
#endif
#ifndef R_Vector_0x6A
#define R_Vector_0x6A R_Dummy
#endif
#ifndef R_Vector_0x6C
#define R_Vector_0x6C R_Dummy
#endif
#ifndef R_Vector_0x6E
#define R_Vector_0x6E R_Dummy
#endif

#ifndef R_Vector_0x70
#define R_Vector_0x70 R_Dummy
#endif
#ifndef R_Vector_0x72
#define R_Vector_0x72 R_Dummy
#endif
#ifndef R_Vector_0x74
#define R_Vector_0x74 R_Dummy
#endif
#ifndef R_Vector_0x76
#define R_Vector_0x76 R_Dummy
#endif
#ifndef R_Vector_0x78
#define R_Vector_0x78 R_Dummy
#endif
#ifndef R_Vector_0x7A
#define R_Vector_0x7A R_Dummy
#endif
#ifndef R_Vector_0x7C
#define R_Vector_0x7C R_Dummy
#endif
#ifndef R_Vector_0x7E
#define R_Vector_0x7E R_Dummy
#endif

#endif
