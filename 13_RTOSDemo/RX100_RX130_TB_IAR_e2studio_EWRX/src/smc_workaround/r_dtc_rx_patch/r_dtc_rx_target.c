/* Workaround for warning messages caused by undefined preprocessing identifier.
 */
#include "../../smc_gen/r_dtc_rx/r_dtc_rx_if.h"
#include "../../smc_workaround/r_dtc_rx_patch/r_dtc_rx_private.h"
#if defined(BSP_MCU_RX130)
    #include "../../smc_gen/r_dtc_rx/src/targets/rx130/r_dtc_rx_target.c"
#elif defined(BSP_MCU_RX231)
    #include "../../smc_gen/r_dtc_rx/src/targets/rx231/r_dtc_rx_target.c"
#elif defined(BSP_MCU_RX65N)
    #include "../../smc_gen/r_dtc_rx/src/targets/rx65n/r_dtc_rx_target.c"
#elif defined(BSP_MCU_RX72N)
    #include "../../smc_gen/r_dtc_rx/src/targets/rx72n/r_dtc_rx_target.c"
#else
    #error "Currently the specified MCU is not supported. Please modify the code here."
#endif
