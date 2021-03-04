#ifndef FREERTOS_STACK_H
#define FREERTOS_STACK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "platform.h"

#define stackBYTES_TO_DEPTH( ulBytes ) ( ( ( uint32_t ) ( ulBytes ) + ( sizeof( StackType_t ) - 1 ) ) / sizeof( StackType_t ) )

#define stackBUFF_BYTES( task, interruptstub, devprogressmergin ) ( ( task ) + ( interruptstub ) + ( devprogressmergin ))

#if defined(__CCRL__)

/* The stack usage analysis tool 'Call Walker' provides detail stack usage information.
 * The following table is based on the result of analysis 04-Mar-2021.
 *
 * The stack space for interrupt stub is calculated as follows:
 *
 * interrupt stub size = MAX( space for task context, space for nested interrupt )
 *                     = MAX( (PC + PSW + REGS + usCriticalNesting), (PC + PSW + REGS) )
 *                     = MAX( (3 + 1 + 10 + 2), (3 + 1 + 10) )
 *                     = MAX( 16, 14 )
 *                     = 16
 */

#if defined(NDEBUG)

/* The following table is for the case of using compile optimization of CC-RL. */

#define task_LED0_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES(  68, 16, 32 ) )
#define task_LED1_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES(  70, 16, 32 ) )
#define task_DMSLV_STACK_BUFF_DEPTH stackBYTES_TO_DEPTH( stackBUFF_BYTES(  54, 16, 32 ) )
#define main_task_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES(   4, 16, 32 ) )
#define IdleTask_STACK_BUFF_DEPTH   stackBYTES_TO_DEPTH( stackBUFF_BYTES(   4, 16, 32 ) )
#define TimerTask_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES(   0,  0,  0 ) )

#else

/* The following table is for the case of NOT using compile optimization of CC-RL. */

#define task_LED0_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES( 134, 16, 32 ) )
#define task_LED1_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES( 136, 16, 32 ) )
#define task_DMSLV_STACK_BUFF_DEPTH stackBYTES_TO_DEPTH( stackBUFF_BYTES(  82, 16, 32 ) )
#define main_task_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES(   6, 16, 32 ) )
#define IdleTask_STACK_BUFF_DEPTH   stackBYTES_TO_DEPTH( stackBUFF_BYTES(  10, 16, 32 ) )
#define TimerTask_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES(   0,  0,  0 ) )

#endif

#elif defined(__ICCRL78__)

/* The following table is based on the result of analysis 04-Mar-2021.
 *
 * The stack space for interrupt stub is calculated as follows:
 *
 * interrupt stub size = MAX( space for task context, space for nested interrupt )
 *                     = MAX( (PC + PSW + REGS + usCriticalNesting), (PC + PSW + REGS) )
 *                     = MAX( (3 + 1 + 10 + 2), (3 + 1 + 10) )
 *                     = MAX( 16, 14 )
 *                     = 16
 */

#if defined(NDEBUG)

/* The following table is for the case of using compile optimization of CC-RL. */

#define task_LED0_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES(  72, 16, 32 ) )
#define task_LED1_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES(  74, 16, 32 ) )
#define task_DMSLV_STACK_BUFF_DEPTH stackBYTES_TO_DEPTH( stackBUFF_BYTES(  62, 16, 32 ) )
#define main_task_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES(   4, 16, 32 ) )
#define IdleTask_STACK_BUFF_DEPTH   stackBYTES_TO_DEPTH( stackBUFF_BYTES(   4, 16, 32 ) )
#define TimerTask_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES(   0,  0,  0 ) )

#else

/* The following table is for the case of NOT using compile optimization of CC-RL. */

#define task_LED0_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES( 114, 16, 32 ) )
#define task_LED1_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES( 116, 16, 32 ) )
#define task_DMSLV_STACK_BUFF_DEPTH stackBYTES_TO_DEPTH( stackBUFF_BYTES(  66, 16, 32 ) )
#define main_task_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES(   4, 16, 32 ) )
#define IdleTask_STACK_BUFF_DEPTH   stackBYTES_TO_DEPTH( stackBUFF_BYTES(  10, 16, 32 ) )
#define TimerTask_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES(   0,  0,  0 ) )

#endif

#elif defined(__GNUC__)

/* Tentative */

#define task_LED0_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( 256 )
#define task_LED1_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( 256 )
#define task_DMSLV_STACK_BUFF_DEPTH stackBYTES_TO_DEPTH( 256 )
#define main_task_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH(  64 )
#define IdleTask_STACK_BUFF_DEPTH   stackBYTES_TO_DEPTH(  64 )
#define TimerTask_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH(   0 )

#endif

#ifdef __cplusplus
}
#endif

#endif /* FREERTOS_STACK_H */
