#ifndef DEMO_MAIN_H
#define DEMO_MAIN_H

/* Set mainCREATE_SIMPLE_BLINKY_DEMO_ONLY to one to run the simple blinky demo,
or 0 to run the more comprehensive test and demo application. */
#define mainCREATE_SIMPLE_BLINKY_DEMO_ONLY	0

/* The following demo_main() is placed in the src/frtos_startup/freertos_start.c
and it calls main_blinky() or main_full() according to the macro setting. */
extern void demo_main( void );

/* The following functions are placed in the src/frtos_startup/freertos_start.c
and these are called from the src/FreeRTOS_Demo/Blinky_Demo/main_blinky.c or the
src/FreeRTOS_Demo/Full_Demo/main_full.c according to the macro setting. */
extern void vToggleLED( void );
extern void vSendString( const char * const pcString );

#endif
