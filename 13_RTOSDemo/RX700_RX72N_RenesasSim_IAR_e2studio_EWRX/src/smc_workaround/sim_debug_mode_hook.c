#if defined(RENESAS_SIMULATOR_DEBUGGING)

/* Switch Key for Renesas Simulator Debugging
 *
 * non 0x00000001 : Emulator or No debugger
 * 0x00000001     : Renesas RX Simulator
 */
#include "stdint.h"
const uint32_t renesas_simulator_debugging_key = 0xFFFFFFFF;

#endif
