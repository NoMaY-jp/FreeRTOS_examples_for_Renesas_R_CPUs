//4.0.0.28400                                                                      
                                                                                   
                                                                                   
                                                                                   
                                                                                   
                                                                                   
                                                                                   
                                                                                   
                                                                                   
                                                                                   
/************************************************************************/
/* Header file generated from device file:                              */
/*    DR5F104ML.DVF                                                     */
/*    V2.30 (2018/04/03)                                                */
/*    Copyright(C) 2018 Renesas                                         */
/* Tool Version: 4.0.0                                                  */
/* Date Generated: 2018/05/11                                           */
/************************************************************************/

#ifndef __INTRINSIC_FUNCTIONS
#define __INTRINSIC_FUNCTIONS

#define DI() asm("di")
#define EI() asm("ei")
#define HALT() asm("halt")
#define NOP() asm("nop")
#define STOP() asm("stop")

#endif

#ifndef __IOREG_BIT_STRUCTURES
#define __IOREG_BIT_STRUCTURES
typedef struct {
	unsigned char no0 :1;
	unsigned char no1 :1;
	unsigned char no2 :1;
	unsigned char no3 :1;
	unsigned char no4 :1;
	unsigned char no5 :1;
	unsigned char no6 :1;
	unsigned char no7 :1;
} __BITS8;

typedef struct {
	unsigned short no0 :1;
	unsigned short no1 :1;
	unsigned short no2 :1;
	unsigned short no3 :1;
	unsigned short no4 :1;
	unsigned short no5 :1;
	unsigned short no6 :1;
	unsigned short no7 :1;
	unsigned short no8 :1;
	unsigned short no9 :1;
	unsigned short no10 :1;
	unsigned short no11 :1;
	unsigned short no12 :1;
	unsigned short no13 :1;
	unsigned short no14 :1;
	unsigned short no15 :1;
} __BITS16;

#endif

#ifndef IODEFINE_EXT_H
#define IODEFINE_EXT_H

/*
 IO Registers
 */
union un_adm2 {
	unsigned char adm2;
	__BITS8 BIT;
};
union un_pu0 {
	unsigned char pu0;
	__BITS8 BIT;
};
union un_pu1 {
	unsigned char pu1;
	__BITS8 BIT;
};
union un_pu3 {
	unsigned char pu3;
	__BITS8 BIT;
};
union un_pu4 {
	unsigned char pu4;
	__BITS8 BIT;
};
union un_pu5 {
	unsigned char pu5;
	__BITS8 BIT;
};
union un_pu6 {
	unsigned char pu6;
	__BITS8 BIT;
};
union un_pu7 {
	unsigned char pu7;
	__BITS8 BIT;
};
union un_pu10 {
	unsigned char pu10;
	__BITS8 BIT;
};
union un_pu11 {
	unsigned char pu11;
	__BITS8 BIT;
};
union un_pu12 {
	unsigned char pu12;
	__BITS8 BIT;
};
union un_pu14 {
	unsigned char pu14;
	__BITS8 BIT;
};
union un_pim0 {
	unsigned char pim0;
	__BITS8 BIT;
};
union un_pim1 {
	unsigned char pim1;
	__BITS8 BIT;
};
union un_pim3 {
	unsigned char pim3;
	__BITS8 BIT;
};
union un_pim4 {
	unsigned char pim4;
	__BITS8 BIT;
};
union un_pim5 {
	unsigned char pim5;
	__BITS8 BIT;
};
union un_pim14 {
	unsigned char pim14;
	__BITS8 BIT;
};
union un_pom0 {
	unsigned char pom0;
	__BITS8 BIT;
};
union un_pom1 {
	unsigned char pom1;
	__BITS8 BIT;
};
union un_pom3 {
	unsigned char pom3;
	__BITS8 BIT;
};
union un_pom4 {
	unsigned char pom4;
	__BITS8 BIT;
};
union un_pom5 {
	unsigned char pom5;
	__BITS8 BIT;
};
union un_pom7 {
	unsigned char pom7;
	__BITS8 BIT;
};
union un_pom14 {
	unsigned char pom14;
	__BITS8 BIT;
};
union un_pmc0 {
	unsigned char pmc0;
	__BITS8 BIT;
};
union un_pmc1 {
	unsigned char pmc1;
	__BITS8 BIT;
};
union un_pmc10 {
	unsigned char pmc10;
	__BITS8 BIT;
};
union un_pmc12 {
	unsigned char pmc12;
	__BITS8 BIT;
};
union un_pmc14 {
	unsigned char pmc14;
	__BITS8 BIT;
};
union un_nfen0 {
	unsigned char nfen0;
	__BITS8 BIT;
};
union un_nfen1 {
	unsigned char nfen1;
	__BITS8 BIT;
};
union un_nfen2 {
	unsigned char nfen2;
	__BITS8 BIT;
};
union un_isc {
	unsigned char isc;
	__BITS8 BIT;
};
union un_per1 {
	unsigned char per1;
	__BITS8 BIT;
};
union un_pms {
	unsigned char pms;
	__BITS8 BIT;
};
union un_gdidis {
	unsigned char gdidis;
	__BITS8 BIT;
};
union un_dflctl {
	unsigned char dflctl;
	__BITS8 BIT;
};
union un_per0 {
	unsigned char per0;
	__BITS8 BIT;
};
union un_rpectl {
	unsigned char rpectl;
	__BITS8 BIT;
};
union un_se0l {
	unsigned char se0l;
	__BITS8 BIT;
};
union un_ss0l {
	unsigned char ss0l;
	__BITS8 BIT;
};
union un_st0l {
	unsigned char st0l;
	__BITS8 BIT;
};
union un_soe0l {
	unsigned char soe0l;
	__BITS8 BIT;
};
union un_se1l {
	unsigned char se1l;
	__BITS8 BIT;
};
union un_ss1l {
	unsigned char ss1l;
	__BITS8 BIT;
};
union un_st1l {
	unsigned char st1l;
	__BITS8 BIT;
};
union un_soe1l {
	unsigned char soe1l;
	__BITS8 BIT;
};
union un_te0l {
	unsigned char te0l;
	__BITS8 BIT;
};
union un_ts0l {
	unsigned char ts0l;
	__BITS8 BIT;
};
union un_tt0l {
	unsigned char tt0l;
	__BITS8 BIT;
};
union un_toe0l {
	unsigned char toe0l;
	__BITS8 BIT;
};
union un_te1l {
	unsigned char te1l;
	__BITS8 BIT;
};
union un_ts1l {
	unsigned char ts1l;
	__BITS8 BIT;
};
union un_tt1l {
	unsigned char tt1l;
	__BITS8 BIT;
};
union un_toe1l {
	unsigned char toe1l;
	__BITS8 BIT;
};
union un_iicctl00 {
	unsigned char iicctl00;
	__BITS8 BIT;
};
union un_iicctl01 {
	unsigned char iicctl01;
	__BITS8 BIT;
};
union un_iicctl10 {
	unsigned char iicctl10;
	__BITS8 BIT;
};
union un_iicctl11 {
	unsigned char iicctl11;
	__BITS8 BIT;
};
union un_trjioc0 {
	unsigned char trjioc0;
	__BITS8 BIT;
};
union un_trjmr0 {
	unsigned char trjmr0;
	__BITS8 BIT;
};
union un_trjisr0 {
	unsigned char trjisr0;
	__BITS8 BIT;
};
union un_trgmr {
	unsigned char trgmr;
	__BITS8 BIT;
};
union un_trgcntc {
	unsigned char trgcntc;
	__BITS8 BIT;
};
union un_trgcr {
	unsigned char trgcr;
	__BITS8 BIT;
};
union un_trgier {
	unsigned char trgier;
	__BITS8 BIT;
};
union un_trgsr {
	unsigned char trgsr;
	__BITS8 BIT;
};
union un_trgior {
	unsigned char trgior;
	__BITS8 BIT;
};
union un_trdelc {
	unsigned char trdelc;
	__BITS8 BIT;
};
union un_trdmr {
	unsigned char trdmr;
	__BITS8 BIT;
};
union un_trdpmr {
	unsigned char trdpmr;
	__BITS8 BIT;
};
union un_trdfcr {
	unsigned char trdfcr;
	__BITS8 BIT;
};
union un_trdoer1 {
	unsigned char trdoer1;
	__BITS8 BIT;
};
union un_trdoer2 {
	unsigned char trdoer2;
	__BITS8 BIT;
};
union un_trdocr {
	unsigned char trdocr;
	__BITS8 BIT;
};
union un_trddf0 {
	unsigned char trddf0;
	__BITS8 BIT;
};
union un_trddf1 {
	unsigned char trddf1;
	__BITS8 BIT;
};
union un_trdcr0 {
	unsigned char trdcr0;
	__BITS8 BIT;
};
union un_trdiora0 {
	unsigned char trdiora0;
	__BITS8 BIT;
};
union un_trdiorc0 {
	unsigned char trdiorc0;
	__BITS8 BIT;
};
union un_trdsr0 {
	unsigned char trdsr0;
	__BITS8 BIT;
};
union un_trdier0 {
	unsigned char trdier0;
	__BITS8 BIT;
};
union un_trdpocr0 {
	unsigned char trdpocr0;
	__BITS8 BIT;
};
union un_trdcr1 {
	unsigned char trdcr1;
	__BITS8 BIT;
};
union un_trdiora1 {
	unsigned char trdiora1;
	__BITS8 BIT;
};
union un_trdiorc1 {
	unsigned char trdiorc1;
	__BITS8 BIT;
};
union un_trdsr1 {
	unsigned char trdsr1;
	__BITS8 BIT;
};
union un_trdier1 {
	unsigned char trdier1;
	__BITS8 BIT;
};
union un_trdpocr1 {
	unsigned char trdpocr1;
	__BITS8 BIT;
};
union un_dtcen0 {
	unsigned char dtcen0;
	__BITS8 BIT;
};
union un_dtcen1 {
	unsigned char dtcen1;
	__BITS8 BIT;
};
union un_dtcen2 {
	unsigned char dtcen2;
	__BITS8 BIT;
};
union un_dtcen3 {
	unsigned char dtcen3;
	__BITS8 BIT;
};
union un_dtcen4 {
	unsigned char dtcen4;
	__BITS8 BIT;
};
union un_crc0ctl {
	unsigned char crc0ctl;
	__BITS8 BIT;
};
union un_compmdr {
	unsigned char compmdr;
	__BITS8 BIT;
};
union un_compfir {
	unsigned char compfir;
	__BITS8 BIT;
};
union un_compocr {
	unsigned char compocr;
	__BITS8 BIT;
};

#define ADM2 (*(volatile union un_adm2 *)0xF0010).adm2
#define ADM2_bit (*(volatile union un_adm2 *)0xF0010).BIT
#define ADUL (*(volatile unsigned char *)0xF0011)
#define ADLL (*(volatile unsigned char *)0xF0012)
#define ADTES (*(volatile unsigned char *)0xF0013)
#define PU0 (*(volatile union un_pu0 *)0xF0030).pu0
#define PU0_bit (*(volatile union un_pu0 *)0xF0030).BIT
#define PU1 (*(volatile union un_pu1 *)0xF0031).pu1
#define PU1_bit (*(volatile union un_pu1 *)0xF0031).BIT
#define PU3 (*(volatile union un_pu3 *)0xF0033).pu3
#define PU3_bit (*(volatile union un_pu3 *)0xF0033).BIT
#define PU4 (*(volatile union un_pu4 *)0xF0034).pu4
#define PU4_bit (*(volatile union un_pu4 *)0xF0034).BIT
#define PU5 (*(volatile union un_pu5 *)0xF0035).pu5
#define PU5_bit (*(volatile union un_pu5 *)0xF0035).BIT
#define PU6 (*(volatile union un_pu6 *)0xF0036).pu6
#define PU6_bit (*(volatile union un_pu6 *)0xF0036).BIT
#define PU7 (*(volatile union un_pu7 *)0xF0037).pu7
#define PU7_bit (*(volatile union un_pu7 *)0xF0037).BIT
#define PU10 (*(volatile union un_pu10 *)0xF003A).pu10
#define PU10_bit (*(volatile union un_pu10 *)0xF003A).BIT
#define PU11 (*(volatile union un_pu11 *)0xF003B).pu11
#define PU11_bit (*(volatile union un_pu11 *)0xF003B).BIT
#define PU12 (*(volatile union un_pu12 *)0xF003C).pu12
#define PU12_bit (*(volatile union un_pu12 *)0xF003C).BIT
#define PU14 (*(volatile union un_pu14 *)0xF003E).pu14
#define PU14_bit (*(volatile union un_pu14 *)0xF003E).BIT
#define PIM0 (*(volatile union un_pim0 *)0xF0040).pim0
#define PIM0_bit (*(volatile union un_pim0 *)0xF0040).BIT
#define PIM1 (*(volatile union un_pim1 *)0xF0041).pim1
#define PIM1_bit (*(volatile union un_pim1 *)0xF0041).BIT
#define PIM3 (*(volatile union un_pim3 *)0xF0043).pim3
#define PIM3_bit (*(volatile union un_pim3 *)0xF0043).BIT
#define PIM4 (*(volatile union un_pim4 *)0xF0044).pim4
#define PIM4_bit (*(volatile union un_pim4 *)0xF0044).BIT
#define PIM5 (*(volatile union un_pim5 *)0xF0045).pim5
#define PIM5_bit (*(volatile union un_pim5 *)0xF0045).BIT
#define PIM14 (*(volatile union un_pim14 *)0xF004E).pim14
#define PIM14_bit (*(volatile union un_pim14 *)0xF004E).BIT
#define POM0 (*(volatile union un_pom0 *)0xF0050).pom0
#define POM0_bit (*(volatile union un_pom0 *)0xF0050).BIT
#define POM1 (*(volatile union un_pom1 *)0xF0051).pom1
#define POM1_bit (*(volatile union un_pom1 *)0xF0051).BIT
#define POM3 (*(volatile union un_pom3 *)0xF0053).pom3
#define POM3_bit (*(volatile union un_pom3 *)0xF0053).BIT
#define POM4 (*(volatile union un_pom4 *)0xF0054).pom4
#define POM4_bit (*(volatile union un_pom4 *)0xF0054).BIT
#define POM5 (*(volatile union un_pom5 *)0xF0055).pom5
#define POM5_bit (*(volatile union un_pom5 *)0xF0055).BIT
#define POM7 (*(volatile union un_pom7 *)0xF0057).pom7
#define POM7_bit (*(volatile union un_pom7 *)0xF0057).BIT
#define POM14 (*(volatile union un_pom14 *)0xF005E).pom14
#define POM14_bit (*(volatile union un_pom14 *)0xF005E).BIT
#define PMC0 (*(volatile union un_pmc0 *)0xF0060).pmc0
#define PMC0_bit (*(volatile union un_pmc0 *)0xF0060).BIT
#define PMC1 (*(volatile union un_pmc1 *)0xF0061).pmc1
#define PMC1_bit (*(volatile union un_pmc1 *)0xF0061).BIT
#define PMC10 (*(volatile union un_pmc10 *)0xF006A).pmc10
#define PMC10_bit (*(volatile union un_pmc10 *)0xF006A).BIT
#define PMC12 (*(volatile union un_pmc12 *)0xF006C).pmc12
#define PMC12_bit (*(volatile union un_pmc12 *)0xF006C).BIT
#define PMC14 (*(volatile union un_pmc14 *)0xF006E).pmc14
#define PMC14_bit (*(volatile union un_pmc14 *)0xF006E).BIT
#define NFEN0 (*(volatile union un_nfen0 *)0xF0070).nfen0
#define NFEN0_bit (*(volatile union un_nfen0 *)0xF0070).BIT
#define NFEN1 (*(volatile union un_nfen1 *)0xF0071).nfen1
#define NFEN1_bit (*(volatile union un_nfen1 *)0xF0071).BIT
#define NFEN2 (*(volatile union un_nfen2 *)0xF0072).nfen2
#define NFEN2_bit (*(volatile union un_nfen2 *)0xF0072).BIT
#define ISC (*(volatile union un_isc *)0xF0073).isc
#define ISC_bit (*(volatile union un_isc *)0xF0073).BIT
#define TIS0 (*(volatile unsigned char *)0xF0074)
#define ADPC (*(volatile unsigned char *)0xF0076)
#define PIOR0 (*(volatile unsigned char *)0xF0077)
#define IAWCTL (*(volatile unsigned char *)0xF0078)
#define PIOR1 (*(volatile unsigned char *)0xF0079)
#define PER1 (*(volatile union un_per1 *)0xF007A).per1
#define PER1_bit (*(volatile union un_per1 *)0xF007A).BIT
#define PMS (*(volatile union un_pms *)0xF007B).pms
#define PMS_bit (*(volatile union un_pms *)0xF007B).BIT
#define GDIDIS (*(volatile union un_gdidis *)0xF007D).gdidis
#define GDIDIS_bit (*(volatile union un_gdidis *)0xF007D).BIT
#define DFLCTL (*(volatile union un_dflctl *)0xF0090).dflctl
#define DFLCTL_bit (*(volatile union un_dflctl *)0xF0090).BIT
#define HIOTRM (*(volatile unsigned char *)0xF00A0)
#define HOCODIV (*(volatile unsigned char *)0xF00A8)
#define PER0 (*(volatile union un_per0 *)0xF00F0).per0
#define PER0_bit (*(volatile union un_per0 *)0xF00F0).BIT
#define OSMC (*(volatile unsigned char *)0xF00F3)
#define RPECTL (*(volatile union un_rpectl *)0xF00F5).rpectl
#define RPECTL_bit (*(volatile union un_rpectl *)0xF00F5).BIT
#define BCDADJ (*(volatile unsigned char *)0xF00FE)
#define SSR00 (*(volatile unsigned short *)0xF0100)
#define SSR00L (*(volatile unsigned char *)0xF0100)
#define SSR01 (*(volatile unsigned short *)0xF0102)
#define SSR01L (*(volatile unsigned char *)0xF0102)
#define SSR02 (*(volatile unsigned short *)0xF0104)
#define SSR02L (*(volatile unsigned char *)0xF0104)
#define SSR03 (*(volatile unsigned short *)0xF0106)
#define SSR03L (*(volatile unsigned char *)0xF0106)
#define SIR00 (*(volatile unsigned short *)0xF0108)
#define SIR00L (*(volatile unsigned char *)0xF0108)
#define SIR01 (*(volatile unsigned short *)0xF010A)
#define SIR01L (*(volatile unsigned char *)0xF010A)
#define SIR02 (*(volatile unsigned short *)0xF010C)
#define SIR02L (*(volatile unsigned char *)0xF010C)
#define SIR03 (*(volatile unsigned short *)0xF010E)
#define SIR03L (*(volatile unsigned char *)0xF010E)
#define SMR00 (*(volatile unsigned short *)0xF0110)
#define SMR01 (*(volatile unsigned short *)0xF0112)
#define SMR02 (*(volatile unsigned short *)0xF0114)
#define SMR03 (*(volatile unsigned short *)0xF0116)
#define SCR00 (*(volatile unsigned short *)0xF0118)
#define SCR01 (*(volatile unsigned short *)0xF011A)
#define SCR02 (*(volatile unsigned short *)0xF011C)
#define SCR03 (*(volatile unsigned short *)0xF011E)
#define SE0 (*(volatile unsigned short *)0xF0120)
#define SE0L (*(volatile union un_se0l *)0xF0120).se0l
#define SE0L_bit (*(volatile union un_se0l *)0xF0120).BIT
#define SS0 (*(volatile unsigned short *)0xF0122)
#define SS0L (*(volatile union un_ss0l *)0xF0122).ss0l
#define SS0L_bit (*(volatile union un_ss0l *)0xF0122).BIT
#define ST0 (*(volatile unsigned short *)0xF0124)
#define ST0L (*(volatile union un_st0l *)0xF0124).st0l
#define ST0L_bit (*(volatile union un_st0l *)0xF0124).BIT
#define SPS0 (*(volatile unsigned short *)0xF0126)
#define SPS0L (*(volatile unsigned char *)0xF0126)
#define SO0 (*(volatile unsigned short *)0xF0128)
#define SOE0 (*(volatile unsigned short *)0xF012A)
#define SOE0L (*(volatile union un_soe0l *)0xF012A).soe0l
#define SOE0L_bit (*(volatile union un_soe0l *)0xF012A).BIT
#define SOL0 (*(volatile unsigned short *)0xF0134)
#define SOL0L (*(volatile unsigned char *)0xF0134)
#define SSC0 (*(volatile unsigned short *)0xF0138)
#define SSC0L (*(volatile unsigned char *)0xF0138)
#define SSR10 (*(volatile unsigned short *)0xF0140)
#define SSR10L (*(volatile unsigned char *)0xF0140)
#define SSR11 (*(volatile unsigned short *)0xF0142)
#define SSR11L (*(volatile unsigned char *)0xF0142)
#define SSR12 (*(volatile unsigned short *)0xF0144)
#define SSR12L (*(volatile unsigned char *)0xF0144)
#define SSR13 (*(volatile unsigned short *)0xF0146)
#define SSR13L (*(volatile unsigned char *)0xF0146)
#define SIR10 (*(volatile unsigned short *)0xF0148)
#define SIR10L (*(volatile unsigned char *)0xF0148)
#define SIR11 (*(volatile unsigned short *)0xF014A)
#define SIR11L (*(volatile unsigned char *)0xF014A)
#define SIR12 (*(volatile unsigned short *)0xF014C)
#define SIR12L (*(volatile unsigned char *)0xF014C)
#define SIR13 (*(volatile unsigned short *)0xF014E)
#define SIR13L (*(volatile unsigned char *)0xF014E)
#define SMR10 (*(volatile unsigned short *)0xF0150)
#define SMR11 (*(volatile unsigned short *)0xF0152)
#define SMR12 (*(volatile unsigned short *)0xF0154)
#define SMR13 (*(volatile unsigned short *)0xF0156)
#define SCR10 (*(volatile unsigned short *)0xF0158)
#define SCR11 (*(volatile unsigned short *)0xF015A)
#define SCR12 (*(volatile unsigned short *)0xF015C)
#define SCR13 (*(volatile unsigned short *)0xF015E)
#define SE1 (*(volatile unsigned short *)0xF0160)
#define SE1L (*(volatile union un_se1l *)0xF0160).se1l
#define SE1L_bit (*(volatile union un_se1l *)0xF0160).BIT
#define SS1 (*(volatile unsigned short *)0xF0162)
#define SS1L (*(volatile union un_ss1l *)0xF0162).ss1l
#define SS1L_bit (*(volatile union un_ss1l *)0xF0162).BIT
#define ST1 (*(volatile unsigned short *)0xF0164)
#define ST1L (*(volatile union un_st1l *)0xF0164).st1l
#define ST1L_bit (*(volatile union un_st1l *)0xF0164).BIT
#define SPS1 (*(volatile unsigned short *)0xF0166)
#define SPS1L (*(volatile unsigned char *)0xF0166)
#define SO1 (*(volatile unsigned short *)0xF0168)
#define SOE1 (*(volatile unsigned short *)0xF016A)
#define SOE1L (*(volatile union un_soe1l *)0xF016A).soe1l
#define SOE1L_bit (*(volatile union un_soe1l *)0xF016A).BIT
#define SOL1 (*(volatile unsigned short *)0xF0174)
#define SOL1L (*(volatile unsigned char *)0xF0174)
#define SSC1 (*(volatile unsigned short *)0xF0178)
#define SSC1L (*(volatile unsigned char *)0xF0178)
#define TCR00 (*(volatile unsigned short *)0xF0180)
#define TCR01 (*(volatile unsigned short *)0xF0182)
#define TCR02 (*(volatile unsigned short *)0xF0184)
#define TCR03 (*(volatile unsigned short *)0xF0186)
#define TMR00 (*(volatile unsigned short *)0xF0190)
#define TMR01 (*(volatile unsigned short *)0xF0192)
#define TMR02 (*(volatile unsigned short *)0xF0194)
#define TMR03 (*(volatile unsigned short *)0xF0196)
#define TSR00 (*(volatile unsigned short *)0xF01A0)
#define TSR00L (*(volatile unsigned char *)0xF01A0)
#define TSR01 (*(volatile unsigned short *)0xF01A2)
#define TSR01L (*(volatile unsigned char *)0xF01A2)
#define TSR02 (*(volatile unsigned short *)0xF01A4)
#define TSR02L (*(volatile unsigned char *)0xF01A4)
#define TSR03 (*(volatile unsigned short *)0xF01A6)
#define TSR03L (*(volatile unsigned char *)0xF01A6)
#define TE0 (*(volatile unsigned short *)0xF01B0)
#define TE0L (*(volatile union un_te0l *)0xF01B0).te0l
#define TE0L_bit (*(volatile union un_te0l *)0xF01B0).BIT
#define TS0 (*(volatile unsigned short *)0xF01B2)
#define TS0L (*(volatile union un_ts0l *)0xF01B2).ts0l
#define TS0L_bit (*(volatile union un_ts0l *)0xF01B2).BIT
#define TT0 (*(volatile unsigned short *)0xF01B4)
#define TT0L (*(volatile union un_tt0l *)0xF01B4).tt0l
#define TT0L_bit (*(volatile union un_tt0l *)0xF01B4).BIT
#define TPS0 (*(volatile unsigned short *)0xF01B6)
#define TO0 (*(volatile unsigned short *)0xF01B8)
#define TO0L (*(volatile unsigned char *)0xF01B8)
#define TOE0 (*(volatile unsigned short *)0xF01BA)
#define TOE0L (*(volatile union un_toe0l *)0xF01BA).toe0l
#define TOE0L_bit (*(volatile union un_toe0l *)0xF01BA).BIT
#define TOL0 (*(volatile unsigned short *)0xF01BC)
#define TOL0L (*(volatile unsigned char *)0xF01BC)
#define TOM0 (*(volatile unsigned short *)0xF01BE)
#define TOM0L (*(volatile unsigned char *)0xF01BE)
#define TCR10 (*(volatile unsigned short *)0xF01C0)
#define TCR11 (*(volatile unsigned short *)0xF01C2)
#define TCR12 (*(volatile unsigned short *)0xF01C4)
#define TCR13 (*(volatile unsigned short *)0xF01C6)
#define TMR10 (*(volatile unsigned short *)0xF01D0)
#define TMR11 (*(volatile unsigned short *)0xF01D2)
#define TMR12 (*(volatile unsigned short *)0xF01D4)
#define TMR13 (*(volatile unsigned short *)0xF01D6)
#define TSR10 (*(volatile unsigned short *)0xF01E0)
#define TSR10L (*(volatile unsigned char *)0xF01E0)
#define TSR11 (*(volatile unsigned short *)0xF01E2)
#define TSR11L (*(volatile unsigned char *)0xF01E2)
#define TSR12 (*(volatile unsigned short *)0xF01E4)
#define TSR12L (*(volatile unsigned char *)0xF01E4)
#define TSR13 (*(volatile unsigned short *)0xF01E6)
#define TSR13L (*(volatile unsigned char *)0xF01E6)
#define TE1 (*(volatile unsigned short *)0xF01F0)
#define TE1L (*(volatile union un_te1l *)0xF01F0).te1l
#define TE1L_bit (*(volatile union un_te1l *)0xF01F0).BIT
#define TS1 (*(volatile unsigned short *)0xF01F2)
#define TS1L (*(volatile union un_ts1l *)0xF01F2).ts1l
#define TS1L_bit (*(volatile union un_ts1l *)0xF01F2).BIT
#define TT1 (*(volatile unsigned short *)0xF01F4)
#define TT1L (*(volatile union un_tt1l *)0xF01F4).tt1l
#define TT1L_bit (*(volatile union un_tt1l *)0xF01F4).BIT
#define TPS1 (*(volatile unsigned short *)0xF01F6)
#define TO1 (*(volatile unsigned short *)0xF01F8)
#define TO1L (*(volatile unsigned char *)0xF01F8)
#define TOE1 (*(volatile unsigned short *)0xF01FA)
#define TOE1L (*(volatile union un_toe1l *)0xF01FA).toe1l
#define TOE1L_bit (*(volatile union un_toe1l *)0xF01FA).BIT
#define TOL1 (*(volatile unsigned short *)0xF01FC)
#define TOL1L (*(volatile unsigned char *)0xF01FC)
#define TOM1 (*(volatile unsigned short *)0xF01FE)
#define TOM1L (*(volatile unsigned char *)0xF01FE)
#define IICCTL00 (*(volatile union un_iicctl00 *)0xF0230).iicctl00
#define IICCTL00_bit (*(volatile union un_iicctl00 *)0xF0230).BIT
#define IICCTL01 (*(volatile union un_iicctl01 *)0xF0231).iicctl01
#define IICCTL01_bit (*(volatile union un_iicctl01 *)0xF0231).BIT
#define IICWL0 (*(volatile unsigned char *)0xF0232)
#define IICWH0 (*(volatile unsigned char *)0xF0233)
#define SVA0 (*(volatile unsigned char *)0xF0234)
#define IICCTL10 (*(volatile union un_iicctl10 *)0xF0238).iicctl10
#define IICCTL10_bit (*(volatile union un_iicctl10 *)0xF0238).BIT
#define IICCTL11 (*(volatile union un_iicctl11 *)0xF0239).iicctl11
#define IICCTL11_bit (*(volatile union un_iicctl11 *)0xF0239).BIT
#define IICWL1 (*(volatile unsigned char *)0xF023A)
#define IICWH1 (*(volatile unsigned char *)0xF023B)
#define SVA1 (*(volatile unsigned char *)0xF023C)
#define TRJCR0 (*(volatile unsigned char *)0xF0240)
#define TRJIOC0 (*(volatile union un_trjioc0 *)0xF0241).trjioc0
#define TRJIOC0_bit (*(volatile union un_trjioc0 *)0xF0241).BIT
#define TRJMR0 (*(volatile union un_trjmr0 *)0xF0242).trjmr0
#define TRJMR0_bit (*(volatile union un_trjmr0 *)0xF0242).BIT
#define TRJISR0 (*(volatile union un_trjisr0 *)0xF0243).trjisr0
#define TRJISR0_bit (*(volatile union un_trjisr0 *)0xF0243).BIT
#define TRGMR (*(volatile union un_trgmr *)0xF0250).trgmr
#define TRGMR_bit (*(volatile union un_trgmr *)0xF0250).BIT
#define TRGCNTC (*(volatile union un_trgcntc *)0xF0251).trgcntc
#define TRGCNTC_bit (*(volatile union un_trgcntc *)0xF0251).BIT
#define TRGCR (*(volatile union un_trgcr *)0xF0252).trgcr
#define TRGCR_bit (*(volatile union un_trgcr *)0xF0252).BIT
#define TRGIER (*(volatile union un_trgier *)0xF0253).trgier
#define TRGIER_bit (*(volatile union un_trgier *)0xF0253).BIT
#define TRGSR (*(volatile union un_trgsr *)0xF0254).trgsr
#define TRGSR_bit (*(volatile union un_trgsr *)0xF0254).BIT
#define TRGIOR (*(volatile union un_trgior *)0xF0255).trgior
#define TRGIOR_bit (*(volatile union un_trgior *)0xF0255).BIT
#define TRG (*(volatile unsigned short *)0xF0256)
#define TRGGRA (*(volatile unsigned short *)0xF0258)
#define TRGGRB (*(volatile unsigned short *)0xF025A)
#define TRDELC (*(volatile union un_trdelc *)0xF0260).trdelc
#define TRDELC_bit (*(volatile union un_trdelc *)0xF0260).BIT
#define TRDSTR (*(volatile unsigned char *)0xF0263)
#define TRDMR (*(volatile union un_trdmr *)0xF0264).trdmr
#define TRDMR_bit (*(volatile union un_trdmr *)0xF0264).BIT
#define TRDPMR (*(volatile union un_trdpmr *)0xF0265).trdpmr
#define TRDPMR_bit (*(volatile union un_trdpmr *)0xF0265).BIT
#define TRDFCR (*(volatile union un_trdfcr *)0xF0266).trdfcr
#define TRDFCR_bit (*(volatile union un_trdfcr *)0xF0266).BIT
#define TRDOER1 (*(volatile union un_trdoer1 *)0xF0267).trdoer1
#define TRDOER1_bit (*(volatile union un_trdoer1 *)0xF0267).BIT
#define TRDOER2 (*(volatile union un_trdoer2 *)0xF0268).trdoer2
#define TRDOER2_bit (*(volatile union un_trdoer2 *)0xF0268).BIT
#define TRDOCR (*(volatile union un_trdocr *)0xF0269).trdocr
#define TRDOCR_bit (*(volatile union un_trdocr *)0xF0269).BIT
#define TRDDF0 (*(volatile union un_trddf0 *)0xF026A).trddf0
#define TRDDF0_bit (*(volatile union un_trddf0 *)0xF026A).BIT
#define TRDDF1 (*(volatile union un_trddf1 *)0xF026B).trddf1
#define TRDDF1_bit (*(volatile union un_trddf1 *)0xF026B).BIT
#define TRDCR0 (*(volatile union un_trdcr0 *)0xF0270).trdcr0
#define TRDCR0_bit (*(volatile union un_trdcr0 *)0xF0270).BIT
#define TRDIORA0 (*(volatile union un_trdiora0 *)0xF0271).trdiora0
#define TRDIORA0_bit (*(volatile union un_trdiora0 *)0xF0271).BIT
#define TRDIORC0 (*(volatile union un_trdiorc0 *)0xF0272).trdiorc0
#define TRDIORC0_bit (*(volatile union un_trdiorc0 *)0xF0272).BIT
#define TRDSR0 (*(volatile union un_trdsr0 *)0xF0273).trdsr0
#define TRDSR0_bit (*(volatile union un_trdsr0 *)0xF0273).BIT
#define TRDIER0 (*(volatile union un_trdier0 *)0xF0274).trdier0
#define TRDIER0_bit (*(volatile union un_trdier0 *)0xF0274).BIT
#define TRDPOCR0 (*(volatile union un_trdpocr0 *)0xF0275).trdpocr0
#define TRDPOCR0_bit (*(volatile union un_trdpocr0 *)0xF0275).BIT
#define TRD0 (*(volatile unsigned short *)0xF0276)
#define TRDGRA0 (*(volatile unsigned short *)0xF0278)
#define TRDGRB0 (*(volatile unsigned short *)0xF027A)
#define TRDCR1 (*(volatile union un_trdcr1 *)0xF0280).trdcr1
#define TRDCR1_bit (*(volatile union un_trdcr1 *)0xF0280).BIT
#define TRDIORA1 (*(volatile union un_trdiora1 *)0xF0281).trdiora1
#define TRDIORA1_bit (*(volatile union un_trdiora1 *)0xF0281).BIT
#define TRDIORC1 (*(volatile union un_trdiorc1 *)0xF0282).trdiorc1
#define TRDIORC1_bit (*(volatile union un_trdiorc1 *)0xF0282).BIT
#define TRDSR1 (*(volatile union un_trdsr1 *)0xF0283).trdsr1
#define TRDSR1_bit (*(volatile union un_trdsr1 *)0xF0283).BIT
#define TRDIER1 (*(volatile union un_trdier1 *)0xF0284).trdier1
#define TRDIER1_bit (*(volatile union un_trdier1 *)0xF0284).BIT
#define TRDPOCR1 (*(volatile union un_trdpocr1 *)0xF0285).trdpocr1
#define TRDPOCR1_bit (*(volatile union un_trdpocr1 *)0xF0285).BIT
#define TRD1 (*(volatile unsigned short *)0xF0286)
#define TRDGRA1 (*(volatile unsigned short *)0xF0288)
#define TRDGRB1 (*(volatile unsigned short *)0xF028A)
#define DTCBAR (*(volatile unsigned char *)0xF02E0)
#define DTCEN0 (*(volatile union un_dtcen0 *)0xF02E8).dtcen0
#define DTCEN0_bit (*(volatile union un_dtcen0 *)0xF02E8).BIT
#define DTCEN1 (*(volatile union un_dtcen1 *)0xF02E9).dtcen1
#define DTCEN1_bit (*(volatile union un_dtcen1 *)0xF02E9).BIT
#define DTCEN2 (*(volatile union un_dtcen2 *)0xF02EA).dtcen2
#define DTCEN2_bit (*(volatile union un_dtcen2 *)0xF02EA).BIT
#define DTCEN3 (*(volatile union un_dtcen3 *)0xF02EB).dtcen3
#define DTCEN3_bit (*(volatile union un_dtcen3 *)0xF02EB).BIT
#define DTCEN4 (*(volatile union un_dtcen4 *)0xF02EC).dtcen4
#define DTCEN4_bit (*(volatile union un_dtcen4 *)0xF02EC).BIT
#define CRC0CTL (*(volatile union un_crc0ctl *)0xF02F0).crc0ctl
#define CRC0CTL_bit (*(volatile union un_crc0ctl *)0xF02F0).BIT
#define PGCRCL (*(volatile unsigned short *)0xF02F2)
#define CRCD (*(volatile unsigned short *)0xF02FA)
#define ELSELR00 (*(volatile unsigned char *)0xF0300)
#define ELSELR01 (*(volatile unsigned char *)0xF0301)
#define ELSELR02 (*(volatile unsigned char *)0xF0302)
#define ELSELR03 (*(volatile unsigned char *)0xF0303)
#define ELSELR04 (*(volatile unsigned char *)0xF0304)
#define ELSELR05 (*(volatile unsigned char *)0xF0305)
#define ELSELR06 (*(volatile unsigned char *)0xF0306)
#define ELSELR07 (*(volatile unsigned char *)0xF0307)
#define ELSELR08 (*(volatile unsigned char *)0xF0308)
#define ELSELR09 (*(volatile unsigned char *)0xF0309)
#define ELSELR10 (*(volatile unsigned char *)0xF030A)
#define ELSELR11 (*(volatile unsigned char *)0xF030B)
#define ELSELR12 (*(volatile unsigned char *)0xF030C)
#define ELSELR13 (*(volatile unsigned char *)0xF030D)
#define ELSELR14 (*(volatile unsigned char *)0xF030E)
#define ELSELR15 (*(volatile unsigned char *)0xF030F)
#define ELSELR16 (*(volatile unsigned char *)0xF0310)
#define ELSELR17 (*(volatile unsigned char *)0xF0311)
#define ELSELR18 (*(volatile unsigned char *)0xF0312)
#define ELSELR19 (*(volatile unsigned char *)0xF0313)
#define ELSELR20 (*(volatile unsigned char *)0xF0314)
#define ELSELR21 (*(volatile unsigned char *)0xF0315)
#define ELSELR22 (*(volatile unsigned char *)0xF0316)
#define ELSELR23 (*(volatile unsigned char *)0xF0317)
#define ELSELR24 (*(volatile unsigned char *)0xF0318)
#define ELSELR25 (*(volatile unsigned char *)0xF0319)
#define COMPMDR (*(volatile union un_compmdr *)0xF0340).compmdr
#define COMPMDR_bit (*(volatile union un_compmdr *)0xF0340).BIT
#define COMPFIR (*(volatile union un_compfir *)0xF0341).compfir
#define COMPFIR_bit (*(volatile union un_compfir *)0xF0341).BIT
#define COMPOCR (*(volatile union un_compocr *)0xF0342).compocr
#define COMPOCR_bit (*(volatile union un_compocr *)0xF0342).BIT
#define TRJ0 (*(volatile unsigned short *)0xF0500)

/*
 Sfr bits
 */
#define ADTYP ADM2_bit.no0
#define AWC ADM2_bit.no2
#define ADRCK ADM2_bit.no3
#define TRJ0EN PER1_bit.no0
#define DTCEN PER1_bit.no3
#define TRD0EN PER1_bit.no4
#define CMPEN PER1_bit.no5
#define TRGEN PER1_bit.no6
#define DACEN PER1_bit.no7
#define DFLEN DFLCTL_bit.no0
#define TAU0EN PER0_bit.no0
#define TAU1EN PER0_bit.no1
#define SAU0EN PER0_bit.no2
#define SAU1EN PER0_bit.no3
#define IICA0EN PER0_bit.no4
#define ADCEN PER0_bit.no5
#define IICA1EN PER0_bit.no6
#define RTCEN PER0_bit.no7
#define RPEF RPECTL_bit.no0
#define RPERDIS RPECTL_bit.no7
#define SPT0 IICCTL00_bit.no0
#define STT0 IICCTL00_bit.no1
#define ACKE0 IICCTL00_bit.no2
#define WTIM0 IICCTL00_bit.no3
#define SPIE0 IICCTL00_bit.no4
#define WREL0 IICCTL00_bit.no5
#define LREL0 IICCTL00_bit.no6
#define IICE0 IICCTL00_bit.no7
#define PRS0 IICCTL01_bit.no0
#define DFC0 IICCTL01_bit.no2
#define SMC0 IICCTL01_bit.no3
#define DAD0 IICCTL01_bit.no4
#define CLD0 IICCTL01_bit.no5
#define WUP0 IICCTL01_bit.no7
#define SPT1 IICCTL10_bit.no0
#define STT1 IICCTL10_bit.no1
#define ACKE1 IICCTL10_bit.no2
#define WTIM1 IICCTL10_bit.no3
#define SPIE1 IICCTL10_bit.no4
#define WREL1 IICCTL10_bit.no5
#define LREL1 IICCTL10_bit.no6
#define IICE1 IICCTL10_bit.no7
#define PRS1 IICCTL11_bit.no0
#define DFC1 IICCTL11_bit.no2
#define SMC1 IICCTL11_bit.no3
#define DAD1 IICCTL11_bit.no4
#define CLD1 IICCTL11_bit.no5
#define WUP1 IICCTL11_bit.no7
#define TRGPWM TRGMR_bit.no0
#define TRGMDF TRGMR_bit.no1
#define TRGDFA TRGMR_bit.no2
#define TRGDFB TRGMR_bit.no3
#define TRGDFCK0 TRGMR_bit.no4
#define TRGDFCK1 TRGMR_bit.no5
#define TRGELCICE TRGMR_bit.no6
#define TRGSTART TRGMR_bit.no7
#define TRGTCK0 TRGCR_bit.no0
#define TRGTCK1 TRGCR_bit.no1
#define TRGTCK2 TRGCR_bit.no2
#define TRGCKEG0 TRGCR_bit.no3
#define TRGCKEG1 TRGCR_bit.no4
#define TRGCCLR0 TRGCR_bit.no5
#define TRGCCLR1 TRGCR_bit.no6
#define TRGIMIEA TRGIER_bit.no0
#define TRGIMIEB TRGIER_bit.no1
#define TRGUDIE TRGIER_bit.no2
#define TRGOVIE TRGIER_bit.no3
#define TRGIMFA TRGSR_bit.no0
#define TRGIMFB TRGSR_bit.no1
#define TRGUDF TRGSR_bit.no2
#define TRGOVF TRGSR_bit.no3
#define TRGDIRF TRGSR_bit.no4
#define TRGIOA0 TRGIOR_bit.no0
#define TRGIOA1 TRGIOR_bit.no1
#define TRGIOA2 TRGIOR_bit.no2
#define TRGBUFA TRGIOR_bit.no3
#define TRGIOB0 TRGIOR_bit.no4
#define TRGIOB1 TRGIOR_bit.no5
#define TRGIOB2 TRGIOR_bit.no6
#define TRGBUFB TRGIOR_bit.no7
#define TRDSYNC TRDMR_bit.no0
#define TRDBFC0 TRDMR_bit.no4
#define TRDBFD0 TRDMR_bit.no5
#define TRDBFC1 TRDMR_bit.no6
#define TRDBFD1 TRDMR_bit.no7
#define TRDPWMB0 TRDPMR_bit.no0
#define TRDPWMC0 TRDPMR_bit.no1
#define TRDPWMD0 TRDPMR_bit.no2
#define TRDPWMB1 TRDPMR_bit.no4
#define TRDPWMC1 TRDPMR_bit.no5
#define TRDPWMD1 TRDPMR_bit.no6
#define TRDSHUTS TRDOER2_bit.no0
#define TRDPTO TRDOER2_bit.no7
#define CRC0EN CRC0CTL_bit.no7
#define C0ENB COMPMDR_bit.no0
#define C0MON COMPMDR_bit.no3
#define C1ENB COMPMDR_bit.no4
#define C1MON COMPMDR_bit.no7
#define C0IE COMPOCR_bit.no0
#define C0OE COMPOCR_bit.no1
#define C0OP COMPOCR_bit.no2
#define C1IE COMPOCR_bit.no4
#define C1OE COMPOCR_bit.no5
#define C1OP COMPOCR_bit.no6
#define SPDMD COMPOCR_bit.no7

/*
 Interrupt vector addresses
 */
#endif
