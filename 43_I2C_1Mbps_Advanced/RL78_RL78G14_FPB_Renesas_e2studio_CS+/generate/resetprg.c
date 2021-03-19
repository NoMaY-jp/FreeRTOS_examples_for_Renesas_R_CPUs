void PowerON_Reset_PC(void);

extern void hdwinit(void);
extern void INIT_BSS(void);
extern void INIT_DATA(void);
extern void main(void);
extern void exit(int status);

void PowerON_Reset_PC(void)
{
    hdwinit();
    INIT_BSS();
    INIT_DATA();
    main();
    exit( 0 );
}
