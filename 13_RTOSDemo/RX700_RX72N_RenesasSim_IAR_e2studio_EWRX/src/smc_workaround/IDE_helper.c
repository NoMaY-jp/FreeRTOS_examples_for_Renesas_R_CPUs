#include "platform.h"

#undef PORT0
#undef PORT1
#undef PORT2
#undef PORT3
#undef PORT4
#undef PORT5
#undef PORT6
#undef PORT7
#undef PORT8
#undef PORT9
#undef PORTA
#undef PORTB
#undef PORTC
#undef PORTD
#undef PORTE
#undef PORTF
#undef PORTG
#undef PORTJ

VOLATILE st_port0_t * const PORT0 = (st_port0_t *)0x8C000;
VOLATILE st_port1_t * const PORT1 = (st_port1_t *)0x8C001;
VOLATILE st_port2_t * const PORT2 = (st_port2_t *)0x8C002;
VOLATILE st_port3_t * const PORT3 = (st_port3_t *)0x8C003;
VOLATILE st_port4_t * const PORT4 = (st_port4_t *)0x8C004;
VOLATILE st_port5_t * const PORT5 = (st_port5_t *)0x8C005;
VOLATILE st_port6_t * const PORT6 = (st_port6_t *)0x8C006;
VOLATILE st_port7_t * const PORT7 = (st_port7_t *)0x8C007;
VOLATILE st_port8_t * const PORT8 = (st_port8_t *)0x8C008;
VOLATILE st_port9_t * const PORT9 = (st_port9_t *)0x8C009;
VOLATILE st_porta_t * const PORTA = (st_porta_t *)0x8C00A;
VOLATILE st_portb_t * const PORTB = (st_portb_t *)0x8C00B;
VOLATILE st_portc_t * const PORTC = (st_portc_t *)0x8C00C;
VOLATILE st_portd_t * const PORTD = (st_portd_t *)0x8C00D;
VOLATILE st_porte_t * const PORTE = (st_porte_t *)0x8C00E;
VOLATILE st_portf_t * const PORTF = (st_portf_t *)0x8C00F;
VOLATILE st_portg_t * const PORTG = (st_portg_t *)0x8C010;
VOLATILE st_portj_t * const PORTJ = (st_portj_t *)0x8C012;

/* The following function is just to prevent the symbol getting optimized away
 * for e2 studio's Visual Expression View. */
void e2_studio_visual_expression_view_helper(void)
{
    void * volatile e;

    e = (void *)&PORT0;
    e = (void *)&PORT1;
    e = (void *)&PORT2;
    e = (void *)&PORT3;
    e = (void *)&PORT4;
    e = (void *)&PORT5;
    e = (void *)&PORT6;
    e = (void *)&PORT7;
    e = (void *)&PORT8;
    e = (void *)&PORT9;
    e = (void *)&PORTA;
    e = (void *)&PORTB;
    e = (void *)&PORTC;
    e = (void *)&PORTD;
    e = (void *)&PORTE;
    e = (void *)&PORTF;
    e = (void *)&PORTG;
    e = (void *)&PORTJ;

    e = (void *)&(*PORT0).PODR.BIT;
    e = (void *)&(*PORT1).PODR.BIT;
    e = (void *)&(*PORT2).PODR.BIT;
    e = (void *)&(*PORT3).PODR.BIT;
    e = (void *)&(*PORT4).PODR.BIT;
    e = (void *)&(*PORT5).PODR.BIT;
    e = (void *)&(*PORT6).PODR.BIT;
    e = (void *)&(*PORT7).PODR.BIT;
    e = (void *)&(*PORT8).PODR.BIT;
    e = (void *)&(*PORT9).PODR.BIT;
    e = (void *)&(*PORTA).PODR.BIT;
    e = (void *)&(*PORTB).PODR.BIT;
    e = (void *)&(*PORTC).PODR.BIT;
    e = (void *)&(*PORTD).PODR.BIT;
    e = (void *)&(*PORTE).PODR.BIT;
    e = (void *)&(*PORTF).PODR.BIT;
    e = (void *)&(*PORTG).PODR.BIT;
    e = (void *)&(*PORTJ).PODR.BIT;

    INTERNAL_NOT_USED( e );
}
