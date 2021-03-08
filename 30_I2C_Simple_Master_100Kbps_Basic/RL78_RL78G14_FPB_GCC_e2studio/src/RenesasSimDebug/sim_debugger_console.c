void sim_debugger_console(const char *s); static void _sim_debugger_console(const char *s); static const char * volatile message; static const char __far * volatile msg; static void _sim_debugger_console(const char *s){ msg = message = s;
}
void sim_debugger_console(const char *s) { msg = message = s; _sim_debugger_console(s); }
/* This is a stub function to output a string to the debugger console
using an Action Event of the CS+ or a Dynamic Printf of the e2 studio
for the Renesas RL78 Simulator. The message is for CS+ and the msg is
for e2 studio. */
