#ifndef FREERTOS_HELPER_H
#define FREERTOS_HELPER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "platform.h"

#if defined(__CCRL__)

#ifndef __CDT_PARSER__
#define R_CG_ASM(...) __VA_ARGS__
#else
#define R_CG_ASM(...)
#endif

extern __near void vPortFreeRTOSInterruptCommonHandler_C(void);
extern __near void vPortInterruptCommonHandler_C(void);

#pragma inline_asm vPortFreeRTOSInterruptCommonHandler_C_Helper1
static void __near vPortFreeRTOSInterruptCommonHandler_C_Helper1(void)
{
    /* Switch stack pointers.  Interrupts which call FreeRTOS API functions
    ending with FromISR cannot be nested.  On the other hand, high priority
    interrupts which don't call any FreeRTOS API functions can be nested. */
    R_CG_ASM(  MOVW DE, SP  );
    R_CG_ASM(  MOVW SP, #LOWW(__STACK_ADDR_START)  );
    R_CG_ASM(  ONEB !_ucInterruptStackNesting  ); /* change: 0 --> 1 */
    /* Don't enable nested interrupts from the beginning of interrupt until
    the completion of switching the stack from task stacks to interrupt
    stack.  If it is enabled before switching the stack to interrupt
    stack, each task stack need additional space for nested interrupt.
    Moreover ucInterruptStackNesting has to be modified under DI state
    so that the stack isn't switched correctly. */
}

#pragma inline_asm vPortFreeRTOSInterruptCommonHandler_C_Helper2
static void __near vPortFreeRTOSInterruptCommonHandler_C_Helper2(void (__near *func)(void))
{
    /* vPortFreeRTOSInterruptCommonHandler_C() doesn't return here. */
    R_CG_ASM(  MOVW BC, AX  );
    R_CG_ASM(  BR !_vPortFreeRTOSInterruptCommonHandler_C  );
}

#define R_CG_FREERTOS_INTERRUPT(function) \
    function(void); \
    static void __near _##function(void); \
    static void __near function(void) \
    { \
        /* vPortFreeRTOSInterruptCommonHandler_C_Helper() doesn't return here. */ \
        vPortFreeRTOSInterruptCommonHandler_C_Helper1(); \
        vPortFreeRTOSInterruptCommonHandler_C_Helper2( (void (__near *)(void))_##function ); \
        /* The following is intended to remove the epilogue code. */ \
        while (1) {} \
    } \
    static void __near _##function

#define R_CG_FREERTOS_INTERRUPT_EI(function) \
    function(void); \
    static void __near _##function(void); \
    static void __near function(void) \
    { \
        /* vPortFreeRTOSInterruptCommonHandler_C_Helper() doesn't return here. */ \
        vPortFreeRTOSInterruptCommonHandler_C_Helper1(); \
        __EI(); \
        vPortFreeRTOSInterruptCommonHandler_C_Helper2( (void (__near *)(void))_##function ); \
        /* The following is intended to remove the epilogue code. */ \
        while (1) {} \
    } \
    static void __near _##function

#pragma inline_asm vPortInterruptCommonHandler_C_Helper1
static void __near vPortInterruptCommonHandler_C_Helper1(void)
{
    /* Switch stack pointers.  Interrupts which call FreeRTOS API functions
    ending with FromISR cannot be nested.  On the other hand, high priority
    interrupts which don't call any FreeRTOS API functions can be nested. */
    R_CG_ASM(  MOVW DE, SP  );
    R_CG_ASM(  CMP0 !_ucInterruptStackNesting  );
    R_CG_ASM(  SKNZ  );
    R_CG_ASM(  MOVW SP, #LOWW(__STACK_ADDR_START)  );
    R_CG_ASM(  INC !_ucInterruptStackNesting  ); /* change: 0~3 --> 1~4 */
    /* Don't enable nested interrupts from the beginning of interrupt until
    the completion of switching the stack from task stacks to interrupt
    stack.  If it is enabled before switching the stack to interrupt
    stack, each task stack need additional space for nested interrupt.
    Moreover ucInterruptStackNesting has to be modified under DI state
    so that the stack isn't switched correctly. */
}

#pragma inline_asm vPortInterruptCommonHandler_C_Helper2
static void __near vPortInterruptCommonHandler_C_Helper2(void (__near *func)(void))
{
    /* vPortFreeRTOSInterruptCommonHandler_C() doesn't return here. */
    R_CG_ASM(  MOVW BC, AX  );
    R_CG_ASM(  BR !_vPortInterruptCommonHandler_C  );
}

#define R_CG_INTERRUPT(function) \
    function(void); \
    static void __near _##function(void); \
    static void __near function(void) \
    { \
        /* vPortInterruptCommonHandler_C_Helper() doesn't return here. */ \
        vPortInterruptCommonHandler_C_Helper1(); \
        vPortInterruptCommonHandler_C_Helper2( (void (__near *)(void))_##function ); \
        /* The following is intended to remove the epilogue code. */ \
        while (1) {} \
    } \
    static void __near _##function

#define R_CG_INTERRUPT_EI(function) \
    function(void); \
    static void __near _##function(void); \
    static void __near function(void) \
    { \
        /* vPorInterruptCommonHandler_C_Helper() doesn't return here. */ \
        vPortInterruptCommonHandler_C_Helper1(); \
        __EI(); \
        vPortInterruptCommonHandler_C_Helper2( (void (__near *)(void))_##function ); \
        /* The following is intended to remove the epilogue code. */ \
        while (1) {} \
    } \
    static void __near _##function

#elif defined(__GNUC__)

#define R_CG_ASM(...) __VA_ARGS__ "\n"

extern volatile void * stack;
extern volatile uint8_t ucInterruptStackNesting;
extern void vPortFreeRTOSInterruptCommonHandler_C(void) __attribute__((section(".lowtext")));
extern void vPortInterruptCommonHandler_C(void) __attribute__((section(".lowtext")));

/* The function attribute 'naked' is available for our purpos
 * but it causes the following compiler warning.
 *
 * warning: stack usage computation not supported for this target
 *
 * So the following asm code outside function is used instead.
 */

#define R_CG_FREERTOS_INTERRUPT(function) \
    _##function(void) __attribute__((section(".lowtext." "_" #function))); \
    __asm \
    ( \
        R_CG_ASM("  .SECTION .lowtext." #function ",\"ax\",@progbits  ") \
        R_CG_ASM("  .GLOBAL _" #function "  ") \
        R_CG_ASM("  _" #function ":  ") \
        R_CG_ASM("  SEL RB0  ") \
        R_CG_ASM("  PUSH AX  ") \
        R_CG_ASM("  MOVW AX, SP  ") \
        R_CG_ASM("  MOVW SP, #_stack  ") \
        R_CG_ASM("  ONEB !_ucInterruptStackNesting  ") /* change: 0 --> 1 */ \
        \
        R_CG_ASM("  SUBW AX, #(28 - 2)  ") \
        R_CG_ASM("  XCHW AX, DE  ") \
        R_CG_ASM("  MOVW [DE+22], AX  ") \
        R_CG_ASM("  MOVW AX, BC  ") \
        R_CG_ASM("  MOVW [DE+24], AX  ") \
        R_CG_ASM("  MOVW BC, #__" #function "  ") \
        R_CG_ASM("  BR !_vPortFreeRTOSInterruptCommonHandler_C  ") \
    ); \
    void _##function

#define R_CG_FREERTOS_INTERRUPT_EI(function) \
    _##function(void) __attribute__((section(".lowtext." "_" #function))); \
    __asm \
    ( \
        R_CG_ASM("  .SECTION .lowtext." #function ",\"ax\",@progbits  ") \
        R_CG_ASM("  .GLOBAL _" #function "  ") \
        R_CG_ASM("  _" #function ":  ") \
        R_CG_ASM("  SEL RB0  ") \
        R_CG_ASM("  PUSH AX  ") \
        R_CG_ASM("  MOVW AX, SP  ") \
        R_CG_ASM("  MOVW SP, #_stack  ") \
        R_CG_ASM("  ONEB !_ucInterruptStackNesting  ") /* change: 0 --> 1 */ \
        \
        R_CG_ASM("  EI  ") \
        \
        R_CG_ASM("  SUBW AX, #(28 - 2)  ") \
        R_CG_ASM("  XCHW AX, DE  ") \
        R_CG_ASM("  MOVW [DE+22], AX  ") \
        R_CG_ASM("  MOVW AX, BC  ") \
        R_CG_ASM("  MOVW [DE+24], AX  ") \
        R_CG_ASM("  MOVW BC, #__" #function "  ") \
        R_CG_ASM("  BR !_vPortFreeRTOSInterruptCommonHandler_C  ") \
    ); \
    void _##function

#define R_CG_INTERRUPT(function) \
    _##function(void) __attribute__((section(".lowtext." "_" #function))); \
    __asm \
    ( \
        R_CG_ASM("  .SECTION .lowtext." #function ",\"ax\",@progbits  ") \
        R_CG_ASM("  .GLOBAL _" #function "  ") \
        R_CG_ASM("  _" #function ":  ") \
        R_CG_ASM("  SEL RB0  ") \
        R_CG_ASM("  PUSH AX  ") \
        R_CG_ASM("  MOVW AX, SP  ") \
        R_CG_ASM("  CMP0 !_ucInterruptStackNesting  ") \
        R_CG_ASM("  SKNZ  ") \
        R_CG_ASM("  MOVW SP, #_stack  ") \
        R_CG_ASM("  INC !_ucInterruptStackNesting  ") /* change: 0~3 --> 1~4 */ \
        \
        R_CG_ASM("  PUSH BC  ") \
        R_CG_ASM("  MOVW BC, #__" #function "  ") \
        R_CG_ASM("  PUSH DE  ") \
        R_CG_ASM("  MOVW DE, AX  ") \
        R_CG_ASM("  BR !_vPortInterruptCommonHandler_C  ") \
    ); \
    void _##function

#define R_CG_INTERRUPT_EI(function) \
    _##function(void) __attribute__((section(".lowtext." "_" #function))); \
    __asm \
    ( \
        R_CG_ASM("  .SECTION .lowtext." #function ",\"ax\",@progbits  ") \
        R_CG_ASM("  .GLOBAL _" #function "  ") \
        R_CG_ASM("  _" #function ":  ") \
        R_CG_ASM("  SEL RB0  ") \
        R_CG_ASM("  PUSH AX  ") \
        R_CG_ASM("  MOVW AX, SP  ") \
        R_CG_ASM("  CMP0 !_ucInterruptStackNesting  ") \
        R_CG_ASM("  SKNZ  ") \
        R_CG_ASM("  MOVW SP, #_stack  ") \
        R_CG_ASM("  INC !_ucInterruptStackNesting  ") /* change: 0~3 --> 1~4 */ \
        \
        R_CG_ASM("  EI  ") \
        \
        R_CG_ASM("  PUSH BC  ") \
        R_CG_ASM("  MOVW BC, #__" #function "  ") \
        R_CG_ASM("  PUSH DE  ") \
        R_CG_ASM("  MOVW DE, AX  ") \
        R_CG_ASM("  BR !_vPortInterruptCommonHandler_C  ") \
    ); \
    void _##function

#elif defined(__ICCRL78__)

#define R_CG_ASM(...) __VA_ARGS__ "\n"

extern volatile __near uint8_t ucInterruptStackNesting;
extern __near_func void vPortFreeRTOSInterruptCommonHandler_C(void);
extern __near_func void vPortInterruptCommonHandler_C(void);

#if defined(RENESAS_SIMULATOR_DEBUGGING)

#define vPortFreeRTOSInterruptCommonHandler_C_Helper0() \
__asm \
( \
    /* The following ICCRL78 interrupt code doesn't work with Renesas RL78 simulator. \
    R_CG_ASM("  MOVW AX, 0xFFFFC  ") \
    R_CG_ASM("  PUSH AX  ") \
    So the following code is added. */ \
    R_CG_ASM("  MOV  A, ES  ") \
    R_CG_ASM("  MOV [SP+1], A  ") \
)

#else

#define vPortFreeRTOSInterruptCommonHandler_C_Helper0()

#endif

#define vPortFreeRTOSInterruptCommonHandler_C_Helper1() \
__asm \
( \
    /* Switch stack pointers.  Interrupts which call FreeRTOS API functions \
    ending with FromISR cannot be nested.  On the other hand, high priority \
    interrupts which don't call any FreeRTOS API functions can be nested. */ \
    R_CG_ASM("  MOVW DE, SP  ") \
    R_CG_ASM("  MOVW SP, #LWRD(SFE(CSTACK))  ") \
    R_CG_ASM("  ONEB _ucInterruptStackNesting  ") /* change: 0 --> 1 */ \
    /* Don't enable nested interrupts from the beginning of interrupt until \
    the completion of switching the stack from task stacks to interrupt \
    stack.  If it is enabled before switching the stack to interrupt \
    stack, each task stack need additional space for nested interrupt. \
    Moreover ucInterruptStackNesting has to be modified under DI state \
    so that the stack isn't switched correctly. */ \
)

#define vPortFreeRTOSInterruptCommonHandler_C_Helper2(_function) \
__asm \
( \
    /* vPortFreeRTOSInterruptCommonHandler_C() doesn't return here. */ \
    R_CG_ASM("  MOVW BC, #" "_" #_function  ) \
    R_CG_ASM("  BR _vPortFreeRTOSInterruptCommonHandler_C  ") \
)

#define R_CG_FREERTOS_INTERRUPT(function) \
    function(void); \
    __near_func void _##function(void); \
    static __interrupt void function(void) \
    { \
        /* vPortFreeRTOSInterruptCommonHandler_C_Helper() doesn't return here. */ \
        vPortFreeRTOSInterruptCommonHandler_C_Helper0(); \
        vPortFreeRTOSInterruptCommonHandler_C_Helper1(); \
        vPortFreeRTOSInterruptCommonHandler_C_Helper2(_##function); \
        /* The following lines are just to prevent the symbol getting optimised away. */ \
        ucInterruptStackNesting = 1; \
        vPortFreeRTOSInterruptCommonHandler_C(); \
        /* The following is intended to remove the epilogue code. */ \
        while (1) {} \
    } \
    __near_func void _##function

#define R_CG_FREERTOS_INTERRUPT_EI(function) \
    function(void); \
    __near_func void _##function(void); \
    static __interrupt void function(void) \
    { \
        /* vPortFreeRTOSInterruptCommonHandler_C_Helper() doesn't return here. */ \
        vPortFreeRTOSInterruptCommonHandler_C_Helper0(); \
        vPortFreeRTOSInterruptCommonHandler_C_Helper1(); \
        __enable_interrupt(); \
        vPortFreeRTOSInterruptCommonHandler_C_Helper2(_##function); \
        /* The following lines are just to prevent the symbol getting optimised away. */ \
        ucInterruptStackNesting = 1; \
        vPortFreeRTOSInterruptCommonHandler_C(); \
        /* The following is intended to remove the epilogue code. */ \
        while (1) {} \
    } \
    __near_func void _##function

#define vPortInterruptCommonHandler_C_Helper0() vPortFreeRTOSInterruptCommonHandler_C_Helper0()

#define vPortInterruptCommonHandler_C_Helper1() \
__asm \
( \
    /* Switch stack pointers.  Interrupts which call FreeRTOS API functions \
    ending with FromISR cannot be nested.  On the other hand, high priority \
    interrupts which don't call any FreeRTOS API functions can be nested. */ \
    R_CG_ASM("  MOVW DE, SP  ") \
    R_CG_ASM("  CMP0 _ucInterruptStackNesting  ") \
    R_CG_ASM("  SKNZ  ") \
    R_CG_ASM("  MOVW SP, #LWRD(SFE(CSTACK))  ") \
    R_CG_ASM("  INC _ucInterruptStackNesting  ") /* change: 0~3 --> 1~4 */ \
    /* Don't enable nested interrupts from the beginning of interrupt until \
    the completion of switching the stack from task stacks to interrupt \
    stack.  If it is enabled before switching the stack to interrupt \
    stack, each task stack need additional space for nested interrupt. \
    Moreover ucInterruptStackNesting has to be modified in the same DI \
    period so that the next switching of the stack is perfomed correctly. */ \
)

#define vPortInterruptCommonHandler_C_Helper2(_function) \
__asm \
( \
    /* vPortFreeRTOSInterruptCommonHandler_C() doesn't return here. */ \
    R_CG_ASM("  MOVW BC, #" "_" #_function  ) \
    R_CG_ASM("  BR _vPortInterruptCommonHandler_C  ") \
)

#define R_CG_INTERRUPT(function) \
    function(void); \
    __near_func void _##function(void); \
    static __interrupt void function(void) \
    { \
        /* vPortFreeRTOSInterruptCommonHandler_C_Helper() doesn't return here. */ \
        vPortInterruptCommonHandler_C_Helper0(); \
        vPortInterruptCommonHandler_C_Helper1(); \
        vPortInterruptCommonHandler_C_Helper2(_##function); \
        /* The following line is just to prevent the symbol getting optimised away. */ \
        vPortInterruptCommonHandler_C(); \
        /* The following is intended to remove the epilogue code. */ \
        while (1) {} \
    } \
    __near_func void _##function

#define R_CG_INTERRUPT_EI(function) \
    function(void); \
    __near_func void _##function(void); \
    static __interrupt void function(void) \
    { \
        /* vPortFreeRTOSInterruptCommonHandler_C_Helper() doesn't return here. */ \
        vPortInterruptCommonHandler_C_Helper0(); \
        vPortInterruptCommonHandler_C_Helper1(); \
        __enable_interrupt(); \
        vPortInterruptCommonHandler_C_Helper2(_##function); \
        /* The following line is just to prevent the symbol getting optimised away. */ \
        vPortInterruptCommonHandler_C(); \
        /* The following is intended to remove the epilogue code. */ \
        while (1) {} \
    } \
    __near_func void _##function

#endif

extern void vTaskNotifyGiveFromISR_R_Helper(TaskHandle_t *pxTask);
extern void xTaskNotifyFromISR_R_Helper(TaskHandle_t *pxTask, uint32_t ulValue);
extern uint32_t ulTaskNotifyTake_R_Helper(TickType_t xTicksToWait);
extern uint32_t ulTaskNotifyTake_R_Helper_Ex2__helper(TaskHandle_t *pxTask, MD_STATUS xStatus, TickType_t xTicksToWait);
extern void ulTaskNotifyTake_R_Abort_Helper(TaskHandle_t *pxTask);
extern TaskHandle_t xTaskGetCurrentTaskHandle_R_Helper(void);

#define xTaskCreateStatic_R_Helper(pxTaskCode, pcName, usStackDepth, pvParameters, uxPriority, pxCreatedTask) \
do{ \
    static StaticTask_t pxTaskCode##_xTCBBuffer; \
    static StackType_t  pxTaskCode##_xStackBuffer[usStackDepth]; \
    TaskHandle_t xCreatedTask; \
    xCreatedTask = xTaskCreateStatic( pxTaskCode, (pcName), (usStackDepth), (pvParameters), (uxPriority), pxTaskCode##_xStackBuffer, &pxTaskCode##_xTCBBuffer ); \
    if (0U != (uint8_t *)(TaskHandle_t *)(pxCreatedTask) - (uint8_t *)NULL) \
    { \
        *(TaskHandle_t *)(pxCreatedTask) = xCreatedTask; \
    } \
}while (0)

#define xSemaphoreCreateMutexStatic_R_Helper(pxCreatedSemaphore) \
do{ \
    static StaticSemaphore_t xStaticSemaphore; \
    QueueHandle_t xCreatedQueue; \
    xCreatedQueue = xSemaphoreCreateMutexStatic( &xStaticSemaphore ); \
    if (0U != (uint8_t *)(QueueHandle_t *)(pxCreatedSemaphore) - (uint8_t *)NULL) \
    { \
        *(QueueHandle_t *)(pxCreatedSemaphore) = xCreatedQueue; \
    } \
}while (0)

#define ulTaskNotifyTake_R_Helper_Ex(pxTask, vStartFunc, xTicksToWait) \
( \
    /* Setup the interrupt/callback ready to post a notification */ \
    (*(pxTask) = xTaskGetCurrentTaskHandle_R_Helper()), \
    /* Wait for a notification from the interrupt/callback */ \
    ulTaskNotifyTake_R_Helper( \
        ( \
            (vStartFunc), \
            (xTicksToWait) \
        ) \
    ) \
)

#define ulTaskNotifyTake_R_Helper_Ex2(pxTask, xStartFunc, xTicksToWait) \
( \
    /* Setup the interrupt/callback ready to post a notification */ \
    (*(pxTask) = xTaskGetCurrentTaskHandle_R_Helper()), \
    /* Wait for a notification from the interrupt/callback */ \
    ulTaskNotifyTake_R_Helper_Ex2__helper( \
        (pxTask), \
        (xStartFunc), \
        (xTicksToWait) \
    ) \
)

#define ulTaskNotifyTake_R_Abort_Helper_Ex(pxTask, vAbortFunc) \
do{ \
    /* Abort the interrupt/callback posting a notification */ \
    (vAbortFunc); \
    ulTaskNotifyTake_R_Abort_Helper(pxTask); \
}while (0)

#ifdef __cplusplus
}
#endif

#endif /* FREERTOS_HELPER_H */
