#ifndef U_SCI_RX_EXT_IF_H
#define U_SCI_RX_EXT_IF_H

#include "r_sci_rx_if.h"

/* There are two types of R_SCI_Control() API depending on the specified MCU. One is able
 * to use two commands of SCI_CMD_SET_RXI_PRIORITY and SCI_CMD_SET_TXI_PRIORITY. The other
 * is not able to use them. This code adds a similar command of SCI_CMD_SET_INT_PRIORITY
 * and a similar API of U_SCI_Control_EXT() independent of the specified MCU.
 */

#if defined(BSP_MCU_RX130) || defined(BSP_MCU_RX231)

    /* SCI_CMD_SET_RXI_PRIORITY and SCI_CMD_SET_TXI_PRIORITY are available. */

#elif defined(BSP_MCU_RX65N) || defined(BSP_MCU_RX72N)

    /* SCI_CMD_SET_RXI_PRIORITY and SCI_CMD_SET_TXI_PRIORITY are not available. */

#else

    #error "Currently the specified MCU is not supported. Please modify the code here."

#endif

#define SCI_CMD_SET_INT_PRIORITY ((sci_cmd_t)(-1))

sci_err_t U_SCI_Control_EXT(sci_hdl_t const hdl, sci_cmd_t const cmd, void *p_args);

#endif /* U_SCI_RX_EXT_IF_H */
