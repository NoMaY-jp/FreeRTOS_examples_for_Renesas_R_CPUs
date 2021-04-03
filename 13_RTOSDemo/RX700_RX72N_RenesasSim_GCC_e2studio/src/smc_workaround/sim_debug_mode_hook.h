#ifndef SIM_DEBUG_MODE_HOOK_H
#define SIM_DEBUG_MODE_HOOK_H

#if defined(RENESAS_SIMULATOR_DEBUGGING)

#include "stdint.h"
extern const uint32_t renesas_simulator_debugging_key;

#define IsRenesasSimDebugMode() (0x00000001 == renesas_simulator_debugging_key)

#define operating_frequency_set(...) operating_frequency_setXXX##__VA_ARGS__()
#define operating_frequency_setXXX() do{ if (!IsRenesasSimDebugMode()) { operating_frequency_set(); } }while(0)
#define operating_frequency_setXXXvoid() operating_frequency_set(void)

#define rom_cache_function_set(...) rom_cache_function_setXXX##__VA_ARGS__()
#define rom_cache_function_setXXX() do{ if (!IsRenesasSimDebugMode()) { rom_cache_function_set(); } }while(0)
#define rom_cache_function_setXXXvoid() rom_cache_function_set(void)

#endif

#endif
