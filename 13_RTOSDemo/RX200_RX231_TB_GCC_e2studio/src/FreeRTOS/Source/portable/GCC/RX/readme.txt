The generic RX port layer in this folder can be used for the following MCUs in case of other than using
tickless idle functionality of RX100 port layers and/or using C89 specification and/or using old version
of ICCRX.


RX MCU Group    CPU     FPU         FPU
                Core    (Single     (Double
                Type    Precision)  Precision)

RX110           RXv1    No          ---
RX111           RXv1    No          ---
RX113           RXv1    No          ---
RX130           RXv1    No          ---
RX13T           RXv1    Yes         ---

RX210           RXv1    No          ---
RX21A           RXv1    No          ---
RX220           RXv1    No          ---
RX230,RX231     RXv2    Yes         ---
RX23E-A         RXv2    Yes         ---
RX23W           RXv2    Yes         ---
RX23T           RXv2    Yes         ---
RX24T           RXv2    Yes         ---
RX24U           RXv2    Yes         ---

RX610           RXv1    Yes         ---
RX62N,RX621     RXv1    Yes         ---
RX630           RXv1    Yes         ---
RX634           RXv1    Yes         ---
RX63N,RX631     RXv1    Yes         ---
RX64M           RXv2    Yes         ---
RX65N,RX651     RXv2    Yes         ---
RX66N           RXv3    Yes         Yes
RX62T           RXv1    Yes         ---
RX62G           RXv1    Yes         ---
RX63T           RXv1    Yes         ---
RX66T           RXv3    Yes         No

RX71M           RXv2    Yes         ---
RX72M           RXv3    Yes         Yes
RX72N           RXv3    Yes         Yes
RX72T           RXv3    Yes         No

Notes:

(1) When you use the FIT configurator or the Smart Configurator, platform.h has to be used.  In this case,
the following definition is necessary in FreeRTOSConfig.h.

/* If configINCLUDE_PLATFORM_H_INSTEAD_OF_IODEFINE_H is set to 0 then iodefine.h
is included and used in FreeRTOS Kernel's Renesas RX port.  If the macro is set
to 1 then platform.h is included and used in the port.  If the macro is set to 2
then neither iodefine.h nor platform.h are included.  If the macro is undefined,
it is set to 0 (CC-RX/GNURX) or 2 (ICCRX) internally for backward compatibility.
When the FIT configurator or the Smart Configurator is used, platform.h has to be
used. */
#define configINCLUDE_PLATFORM_H_INSTEAD_OF_IODEFINE_H  1

(2) When you use the CC-RX compiler of Renesas or the GNURX compiler, the definition of configTICK_VECTOR
is like the following:

/* The peripheral used to generate the tick interrupt is configured as part of
the application code.  This constant should be set to the vector number of the
peripheral chosen.  As supplied this is CMT0. */
#define configTICK_VECTOR   _CMT0_CMI0

(3) When you use the ICCRX compiler of IAR, the definition of configTICK_VECTOR depends on the setting of
configINCLUDE_PLATFORM_H_INSTEAD_OF_IODEFINE_H like the followings:

(3-1) In case of configINCLUDE_PLATFORM_H_INSTEAD_OF_IODEFINE_H == 0 or 1

#define configTICK_VECTOR   _CMT0_CMI0

(3-2) In case of configINCLUDE_PLATFORM_H_INSTEAD_OF_IODEFINE_H == 2 or undefined

#define configTICK_VECTOR   VECT_CMT0_CMI0

(4) Moreover in the case of above (3-2), I/O register definition header file which is provided from IAR
has to be included in FreeRTOSConfig.h like the followings:

/* Renesas hardware definition header. */
#include <iorx71m.h>


For more information about Renesas RX MCUs, please visit the following URL:

https://www.renesas.com/products/microcontrollers-microprocessors/rx.html

