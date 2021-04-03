#ifndef CC_PATCH_H
#define CC_PATCH_H

#if defined(__CCRX__)

/* This file has to be included by using CC-RX's -preinclude option. */

/* Workaround to reduce the following information message caused by math.h.
 *
 * M0520193: Zero used for undefined preprocessing identifier "XXX"
 */
#ifndef _FEVAL
#define _FEVAL 0
#endif
#ifndef _FEVVAL
#define _FEVVAL 0
#endif
#ifndef _HAS_C9X_FAST_FMA
#define _HAS_C9X_FAST_FMA 0
#endif

/* Workaround to reduce the following information message caused by stdarg.h's
 * va_start() macro definition and __builtin_va_start() implementation.
 *
 * M0520549: Variable "XXX" is used before its value is set
 */
#if defined(__RENESAS_VERSION__) && (__RENESAS_VERSION__ >= 0x03020000)
#define __builtin_va_start(...) __builtin_va_startxxx##__VA_ARGS__
#define __builtin_va_startxxxva_list __builtin_va_start(va_list)
#define __builtin_va_startxxx(args) ((args)=NULL,__builtin_va_start((args)))
#else
#define __builtin_va_start(...) __builtin_va_startXXX##__VA_ARGS__()
#define __builtin_va_startXXXva_list() __builtin_va_start(va_list)
#define __builtin_va_startXXXargs() (args=NULL,__builtin_va_start(args))
#endif

#endif /* defined(__CCRX__) */

#endif /* CC_PATCH_H */
