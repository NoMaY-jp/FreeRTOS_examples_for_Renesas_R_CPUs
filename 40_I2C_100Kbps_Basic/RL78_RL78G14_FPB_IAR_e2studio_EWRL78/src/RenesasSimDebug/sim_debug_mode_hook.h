#ifndef SIM_DEBUG_MODE_HOOK_H
#define SIM_DEBUG_MODE_HOOK_H

#if defined(RENESAS_SIMULATOR_DEBUGGING)

extern const unsigned short renesas_simulator_debugging_key;

#define IsRenesasSimDebugMode() (0x0001 == renesas_simulator_debugging_key)

#endif

#endif
