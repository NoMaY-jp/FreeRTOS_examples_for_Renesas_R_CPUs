#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__CDT_PARSER__) && defined(__CCRL__)
#if 0 /* -lang=c and -strict_std */
#define __STDC_VERSION__ 199409L
#elif 0 /* -lang=c99 */
#define __STDC_VERSION__ 199901L
#define __STDC_HOSTED__  0
#define __STDC_IEC_559__ 1
#else
#undef __STDC_VERSION__
#endif
#endif

#if defined(__GNUC__)
#define asm(str) __asm volatile (str)
#endif
#if (defined(__CCRL__) && defined(_STDINT_H)) || (defined(__ICCRL78__) && defined(_STDINT))
#define int8_t   INT8_T_DUPLICATED_TYPEDEF
#define uint8_t  UINT8_T_DUPLICATED_TYPEDEF
#define int16_t  INT16_T_DUPLICATED_TYPEDEF
#define uint16_t UINT16_T_DUPLICATED_TYPEDEF
#define int32_t  INT32_T_DUPLICATED_TYPEDEF
#define uint32_t UINT32_T_DUPLICATED_TYPEDEF
#endif
#include "r_cg_macrodriver.h"
#if (defined(__CCRL__) && defined(_STDINT_H)) || (defined(__ICCRL78__) && defined(_STDINT))
#undef INT8_T_DUPLICATED_TYPEDEF
#undef UINT8_T_DUPLICATED_TYPEDEF
#undef INT16_T_DUPLICATED_TYPEDEF
#undef UINT16_T_DUPLICATED_TYPEDEF
#undef INT32_T_DUPLICATED_TYPEDEF
#undef UINT32_T_DUPLICATED_TYPEDEF
#endif
#if defined(__CCRL__) && !defined(_STDINT_H)
#define _STDINT_H
#endif
#if defined(__ICCRL78__) && !defined(_STDINT)
#define _STDINT
#endif
#if defined(__GNUC__)
#undef asm
#endif

#include "r_cg_userdefine.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "croutine.h"
#include "timers.h"
#include "event_groups.h"

#if defined(__CCRL__) && (!defined(__STDC_VERSION__) || (__STDC_VERSION__ < 199901L))
#define bool    _Bool
#define true    1
#define false   0
#define __bool_true_false_are_defined   1
#else
#include <stdbool.h>
#endif
#include <stdio.h>
#include <stddef.h>
#include <string.h>

#if defined(__GNUC__)
#define BRK()     __asm volatile ("brk")
#endif

#if defined(__ICCRL78__)
#define BRK       __break
#endif

#define di()      DI()
#define ei()      EI()
#define halt()    HALT()
#define nop()     NOP()
#define stop()    STOP()
#define brk()     BRK()

#define NOP0()    do{ }while (0)
#define NOP1()    do{ NOP(); }while (0)
#define NOP2()    do{ NOP();NOP(); }while (0)
#define NOP3()    do{ NOP();NOP();NOP(); }while (0)
#define NOP4()    do{ NOP();NOP();NOP();NOP(); }while (0)
#define NOP5()    do{ NOP();NOP();NOP();NOP();NOP(); }while (0)
#define NOP6()    do{ NOP5();NOP(); }while (0)
#define NOP7()    do{ NOP5();NOP();NOP(); }while (0)
#define NOP8()    do{ NOP5();NOP();NOP();NOP(); }while (0)
#define NOP9()    do{ NOP5();NOP();NOP();NOP();NOP(); }while (0)

#define NOP10()   do{ NOP5();NOP5(); }while (0)
#define NOP11()   do{ NOP10();NOP(); }while (0)
#define NOP12()   do{ NOP10();NOP();NOP(); }while (0)
#define NOP13()   do{ NOP10();NOP();NOP();NOP(); }while (0)
#define NOP14()   do{ NOP10();NOP();NOP();NOP();NOP(); }while (0)
#define NOP15()   do{ NOP10();NOP();NOP();NOP();NOP();NOP(); }while (0)
#define NOP16()   do{ NOP15();NOP(); }while (0)
#define NOP17()   do{ NOP15();NOP();NOP(); }while (0)
#define NOP18()   do{ NOP15();NOP();NOP();NOP(); }while (0)
#define NOP19()   do{ NOP15();NOP();NOP();NOP();NOP(); }while (0)

#define NOP20()   do{ NOP10();NOP10(); }while (0)
#define NOP21()   do{ NOP20();NOP(); }while (0)
#define NOP22()   do{ NOP20();NOP();NOP(); }while (0)
#define NOP23()   do{ NOP20();NOP();NOP();NOP(); }while (0)
#define NOP24()   do{ NOP20();NOP();NOP();NOP();NOP(); }while (0)
#define NOP25()   do{ NOP20();NOP();NOP();NOP();NOP();NOP(); }while (0)
#define NOP26()   do{ NOP25();NOP(); }while (0)
#define NOP27()   do{ NOP25();NOP();NOP(); }while (0)
#define NOP28()   do{ NOP25();NOP();NOP();NOP(); }while (0)
#define NOP29()   do{ NOP25();NOP();NOP();NOP();NOP(); }while (0)

#define NOP30()   do{ NOP10();NOP10();NOP10(); }while (0)
#define NOP31()   do{ NOP30();NOP(); }while (0)
#define NOP32()   do{ NOP30();NOP();NOP(); }while (0)

#define NOP00()   do{ }while (0)
#define NOP40()   do{ NOP10();NOP10();NOP10();NOP10(); }while (0)
#define NOP50()   do{ NOP10();NOP10();NOP10();NOP10();NOP10(); }while (0)
#define NOP60()   do{ NOP50();NOP10(); }while (0)
#define NOP70()   do{ NOP50();NOP10();NOP10(); }while (0)
#define NOP80()   do{ NOP50();NOP10();NOP10();NOP10(); }while (0)
#define NOP90()   do{ NOP50();NOP10();NOP10();NOP10();NOP10(); }while (0)

#define NOP000()  do{ }while (0)
#define NOP100()  do{ NOP50();NOP50(); }while (0)
#define NOP200()  do{ NOP100();NOP100(); }while (0)
#define NOP300()  do{ NOP100();NOP100();NOP100(); }while (0)
#define NOP400()  do{ NOP100();NOP100();NOP100();NOP100(); }while (0)
#define NOP500()  do{ NOP100();NOP100();NOP100();NOP100();NOP100(); }while (0)
#define NOP600()  do{ NOP500();NOP100(); }while (0)
#define NOP700()  do{ NOP500();NOP100();NOP100(); }while (0)
#define NOP800()  do{ NOP500();NOP100();NOP100();NOP100(); }while (0)
#define NOP900()  do{ NOP500();NOP100();NOP100();NOP100();NOP100(); }while (0)

#define NOP0000() do{ }while (0)
#define NOP1000() do{ NOP500();NOP500(); }while (0)
#define NOP2000() do{ NOP1000();NOP1000(); }while (0)
#define NOP3000() do{ NOP1000();NOP1000();NOP1000(); }while (0)
#define NOP4000() do{ NOP1000();NOP1000();NOP1000();NOP1000(); }while (0)
#define NOP5000() do{ NOP1000();NOP1000();NOP1000();NOP1000();NOP1000(); }while (0)
#define NOP6000() do{ NOP5000();NOP1000(); }while (0)
#define NOP7000() do{ NOP5000();NOP1000();NOP1000(); }while (0)
#define NOP8000() do{ NOP5000();NOP1000();NOP1000();NOP1000(); }while (0)
#define NOP9000() do{ NOP5000();NOP1000();NOP1000();NOP1000();NOP1000(); }while (0)

#define NOP00000() do{ }while (0)
#define NOP10000() do{ NOP5000();NOP5000(); }while (0)
#define NOP20000() do{ NOP10000();NOP10000(); }while (0)
#define NOP30000() do{ NOP10000();NOP10000();NOP10000(); }while (0)
#define NOP40000() do{ NOP10000();NOP10000();NOP10000();NOP10000(); }while (0)
#define NOP50000() do{ NOP10000();NOP10000();NOP10000();NOP10000();NOP10000(); }while (0)
#define NOP60000() do{ NOP50000();NOP10000(); }while (0)
#define NOP70000() do{ NOP50000();NOP10000();NOP10000(); }while (0)
#define NOP80000() do{ NOP50000();NOP10000();NOP10000();NOP10000(); }while (0)
#define NOP90000() do{ NOP50000();NOP10000();NOP10000();NOP10000();NOP10000(); }while (0)

#define nop0()     NOP0()
#define nop1()     NOP1()
#define nop2()     NOP2()
#define nop3()     NOP3()
#define nop4()     NOP4()
#define nop5()     NOP5()
#define nop6()     NOP6()
#define nop7()     NOP7()
#define nop8()     NOP8()
#define nop9()     NOP9()

#define nop10()    NOP10()
#define nop11()    NOP11()
#define nop12()    NOP12()
#define nop13()    NOP13()
#define nop14()    NOP14()
#define nop15()    NOP15()
#define nop16()    NOP16()
#define nop17()    NOP17()
#define nop18()    NOP18()
#define nop19()    NOP19()

#define nop20()    NOP20()
#define nop21()    NOP21()
#define nop22()    NOP22()
#define nop23()    NOP23()
#define nop24()    NOP24()
#define nop25()    NOP25()
#define nop26()    NOP26()
#define nop27()    NOP27()
#define nop28()    NOP28()
#define nop29()    NOP29()

#define nop30()    NOP30()
#define nop31()    NOP31()
#define nop32()    NOP32()

#define nop00()    NOP00()
#define nop40()    NOP40()
#define nop50()    NOP50()
#define nop60()    NOP60()
#define nop70()    NOP70()
#define nop80()    NOP80()
#define nop90()    NOP90()

#define nop000()   NOP000()
#define nop100()   NOP100()
#define nop200()   NOP200()
#define nop300()   NOP300()
#define nop400()   NOP400()
#define nop500()   NOP500()
#define nop600()   NOP600()
#define nop700()   NOP700()
#define nop800()   NOP800()
#define nop900()   NOP900()

#define nop0000()  NOP0000()
#define nop1000()  NOP1000()
#define nop2000()  NOP2000()
#define nop3000()  NOP3000()
#define nop4000()  NOP4000()
#define nop5000()  NOP5000()
#define nop6000()  NOP6000()
#define nop7000()  NOP7000()
#define nop8000()  NOP8000()
#define nop9000()  NOP9000()

#define nop00000() NOP00000()
#define nop10000() NOP10000()
#define nop20000() NOP20000()
#define nop30000() NOP30000()
#define nop40000() NOP40000()
#define nop50000() NOP50000()
#define nop60000() NOP60000()
#define nop70000() NOP70000()
#define nop80000() NOP80000()
#define nop90000() NOP90000()

#define INTERNAL_NOT_USED(p)        ((void)(p))
#define R_CG_PRAGMA(...)            _Pragma(#__VA_ARGS__)

#if defined(__CCRL__)

#define R_CG_ISR_UNUSED(function, dummy_vect) \
    function##_UNUSED(void); \
    void function##__UNUSED(void); \
    void function##__UNUSED(void){function##_UNUSED();} \
    static void __near function##_UNUSED

#define R_CG_API_UNUSED(function) \
    function

#define R_CG_API_DO_NOT_USE(function) \
    function

#elif defined(__GNUC__)

#define R_CG_ISR_UNUSED(function, dummy_vect) \
    function##_UNUSED(void) __attribute__ ((unused)); \
    __attribute__ ((unused)) void function##_UNUSED

#define R_CG_API_UNUSED(function) \
    function

#define R_CG_API_DO_NOT_USE(function) \
    function

#elif defined(__ICCRL78__)

#define R_CG_ISR_UNUSED(function, dummy_vect) \
    function##___UNUSED(void); \
    R_CG_PRAGMA(vector = dummy_vect) \
    __interrupt static void function##__UNUSED(void); \
    __interrupt static void function##__UNUSED(void){} \
    static void function##_UNUSED

#define R_CG_API_UNUSED(function) \
    function

#define R_CG_API_DO_NOT_USE(function) \
    function

#endif

#if defined(RENESAS_SIMULATOR_DEBUGGING)
#include "RenesasSimDebug/sim_debug_mode_hook.h"
#endif

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_H */
