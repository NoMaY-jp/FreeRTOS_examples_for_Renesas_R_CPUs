#if defined(RENESAS_SIMULATOR_DEBUGGING)

/* Switch Key for Renesas Simulator Debugging
 *
 * non 0x0001 : Emulator or No debugger
 * 0x0001     : Renesas RL78 Simulator
 */
const unsigned short renesas_simulator_debugging_key = 0xFFFF;

#endif
