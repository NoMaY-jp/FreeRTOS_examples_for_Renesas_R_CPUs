#ifndef FREERTOS_STACK_H
#define FREERTOS_STACK_H

#ifdef __cplusplus
extern "C" {
#endif

#define stackBYTES_TO_DEPTH( ulBytes ) ( ( ( uint32_t ) ( ulBytes ) + ( sizeof( StackType_t ) - 1 ) ) / sizeof( StackType_t ) )

#define stackBUFF_BYTES( task, interruptstub, devprogressmergin ) ( ( task ) + ( interruptstub ) + ( devprogressmergin ))

#if defined(__CCRL__)

/* The stack usage analysis tool 'Call Walker' provides detail stack usage information.
 * The following table is based on the result of analysis 19-Mar-2021.
 *
 * The stack space for interrupt stub is calculated as follows:
 *
 * interrupt stub size = MAX( space for task context, space for nested interrupt, MAX(TM3, IIC00) interrupt )
 *                     = MAX( (PC + PSW + REGS + usCriticalNesting), (PC + PSW + REGS), MAX(TM3, IIC00) interrupt )
 *                     = MAX( (3 + 1 + 10 + 2), (3 + 1 + 10), MAX(TM3, IIC00) interrupt )
 *                     = MAX( 16, 14, MAX(TM3, IIC00) interrupt )
 */

#if defined(NDEBUG)

/* The following table is for the case of using compile optimization of CC-RL. */

/* The stack space for TM3 & IIC00 interrupt is MAX(20, 4) = 20 */

#define task_LED0_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES(  62, 20, 32 ) )
#define task_LED1_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES(  64, 20, 32 ) )
#define task_DMSLV_STACK_BUFF_DEPTH stackBYTES_TO_DEPTH( stackBUFF_BYTES(  50, 20, 32 ) )
#define main_task_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES(   4, 20, 32 ) )
#define IdleTask_STACK_BUFF_DEPTH   stackBYTES_TO_DEPTH( stackBUFF_BYTES(   4, 20, 32 ) )
#define TimerTask_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES(   0,  0,  0 ) )

#else

/* The following table is for the case of NOT using compile optimization of CC-RL. */

/* The stack space for TM3 & IIC00 interrupt is MAX(36, 4) = 36 */

#define task_LED0_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES( 106, 36, 32 ) )
#define task_LED1_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES( 108, 36, 32 ) )
#define task_DMSLV_STACK_BUFF_DEPTH stackBYTES_TO_DEPTH( stackBUFF_BYTES(  84, 36, 32 ) )
#define main_task_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES(   6, 36, 32 ) )
#define IdleTask_STACK_BUFF_DEPTH   stackBYTES_TO_DEPTH( stackBUFF_BYTES(  10, 36, 32 ) )
#define TimerTask_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES(   0,  0,  0 ) )

#endif

#elif defined(__ICCRL78__)

/* The linker log file's stack usage call graph provides detail stack usage information.
 * The following table is based on the result of analysis 19-Mar-2021.
 *
 * The stack space for interrupt stub is calculated as follows:
 *
 * interrupt stub size = MAX( space for task context, space for nested interrupt, MAX(TM3, IIC00) interrupt )
 *                     = MAX( (PC + PSW + REGS + usCriticalNesting), (PC + PSW + REGS), MAX(TM3, IIC00) interrupt )
 *                     = MAX( (3 + 1 + 10 + 2), (3 + 1 + 10), MAX(TM3, IIC00) interrupt )
 *                     = MAX( 16, 14, MAX(TM3, IIC00) interrupt )
 */

#if defined(NDEBUG)

/* The following table is for the case of using compile optimization of ICCRL78. */

/* The stack space forTM3 & IIC00 interrupt is MAX(24, 4) = 24 */

#define task_LED0_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES(  70, 24, 32 ) )
#define task_LED1_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES(  72, 24, 32 ) )
#define task_DMSLV_STACK_BUFF_DEPTH stackBYTES_TO_DEPTH( stackBUFF_BYTES(  54, 24, 32 ) )
#define main_task_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES(   4, 24, 32 ) )
#define IdleTask_STACK_BUFF_DEPTH   stackBYTES_TO_DEPTH( stackBUFF_BYTES(   4, 24, 32 ) )
#define TimerTask_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES(   0,  0,  0 ) )

#else

/* The following table is for the case of NOT using compile optimization of ICCRL78. */

/* The stack space forTM3 & IIC00 interrupt is MAX(30, 4) = 30 */

#define task_LED0_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES(  90, 30, 32 ) )
#define task_LED1_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES(  92, 30, 32 ) )
#define task_DMSLV_STACK_BUFF_DEPTH stackBYTES_TO_DEPTH( stackBUFF_BYTES(  56, 30, 32 ) )
#define main_task_STACK_BUFF_DEPTH  stackBYTES_TO_DEPTH( stackBUFF_BYTES(   4, 30, 32 ) )
#define IdleTask_STACK_BUFF_DEPTH   stackBYTES_TO_DEPTH( stackBUFF_BYTES(  10, 30, 32 ) )
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
