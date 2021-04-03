#include "platform.h"
#include "u_bsp_lowlvl_ext.h"

/* When using the user startup program, disable the following code. */
#if BSP_CFG_STARTUP_DISABLE == 0

#define BSP_PRV_E1_DBG_PORT (*(volatile st_dbg_t     R_BSP_EVENACCESS_SFR *)0x84080)
#define BSP_PRV_TXFL0EN     (0x00000100)          /* debug tx flow control bit */
#define BSP_PRV_RXFL0EN     (0x00001000)          /* debug RX flow control bit */

typedef struct
{
    uint32_t   tx_data;     /* Debug Virtual Console TX data */
    char       wk1[12];     /* spacer */
    uint32_t   rx_data;     /* Debug Virtual Console RX data */
    char       wk2[44];     /* spacer */
    uint32_t   dbgstat;     /* Debug Virtual Console Status */
} st_dbg_t;

#if BSP_CFG_USER_CHARPUT_ENABLED != 0
/* If user has indicated they want to provide their own charput function then here is the prototype. */
#endif

#if BSP_CFG_USER_CHARGET_ENABLED != 0
/* If user has indicated they want to provide their own charget function then here is the prototype. */
#endif

bool is_charput_ready (void)
{
#if BSP_CFG_USER_CHARPUT_ENABLED == 1
    /* If user has provided their own charput() function, then call it here. */
#else
    return (0 == (BSP_PRV_E1_DBG_PORT.dbgstat & BSP_PRV_TXFL0EN));
#endif
}

bool is_charget_ready (void)
{
#if BSP_CFG_USER_CHARGET_ENABLED == 1
    /* If user has provided their own charget() function, then call it here. */
#else
    return (0 != (BSP_PRV_E1_DBG_PORT.dbgstat & BSP_PRV_RXFL0EN));
#endif
}

#endif /* BSP_CFG_STARTUP_DISABLE == 0 */

