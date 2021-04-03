#ifndef U_BSP_LOWLVL_EXT_H
#define U_BSP_LOWLVL_EXT_H

#include "mcu/all/lowlvl.h"

/* Check ready flag to output one character to standard output
 * (the E1 Virtual Console or a serial port via user own charput function) */
bool is_charput_ready (void);

/* Check ready flag to input one character from standard input
 * (the E1 Virtual Console or a serial port via user own charget function) */
bool is_charget_ready (void);

#endif /* U_BSP_LOWLVL_EXT_H */
