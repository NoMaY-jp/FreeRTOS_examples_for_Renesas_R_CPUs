#include "platform.h"
#include "r_sci_rx_platform.h"
#include "u_sci_rx_ext_if.h"

/* There are two types of R_SCI_Control() API depending on the specified MCU. One is able
 * to use two commands of SCI_CMD_SET_RXI_PRIORITY and SCI_CMD_SET_TXI_PRIORITY. The other
 * is not able to use them. This code adds a similar command of SCI_CMD_SET_INT_PRIORITY
 * and a similar API of U_SCI_Control_EXT() independent of the specified MCU.
 */

sci_err_t U_SCI_Control_EXT(sci_hdl_t const hdl, sci_cmd_t const cmd, void *p_args)
{
    if( cmd == SCI_CMD_SET_INT_PRIORITY )
    {
#if defined(BSP_MCU_RX130) || defined(BSP_MCU_RX231)

    #if SCI_CFG_PARAM_CHECKING_ENABLE
        /* Check argument hdl */
        if ((NULL == hdl) || (FIT_NO_PTR == hdl))
        {
            return SCI_ERR_NULL_PTR;
        }

        /* Check argument p_args*/
        if ((NULL == p_args) || (FIT_NO_PTR == p_args))
        {
            return SCI_ERR_NULL_PTR;
        }
        if ((SCI_MODE_OFF == hdl->mode) || (SCI_MODE_MAX <= hdl->mode))
        {
            return SCI_ERR_BAD_MODE;
        }
        /* Casting void* type is valid */
        if ((1 > (*(uint8_t *)p_args)) || (BSP_MCU_IPL_MAX < (*(uint8_t *)p_args)))
        {
            return SCI_ERR_INVALID_ARG;
        }
    #endif
        *hdl->rom->ipr = *(uint8_t *)p_args;

#elif defined(BSP_MCU_RX65N) || defined(BSP_MCU_RX72N)

        R_SCI_Control( hdl, SCI_CMD_SET_RXI_PRIORITY, p_args );
        R_SCI_Control( hdl, SCI_CMD_SET_TXI_PRIORITY, p_args );

#else

    #error "Currently the specified MCU is not supported. Please modify the code here."

#endif
        return SCI_SUCCESS;
    }

    return R_SCI_Control( hdl, cmd, p_args );
}
