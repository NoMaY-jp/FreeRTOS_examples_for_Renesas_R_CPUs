MEMORY
{
	RAM : ORIGIN = 0x4, LENGTH = 0xfffc
	ROM : ORIGIN = 0xFFF80000, LENGTH = 524288
	OFS : ORIGIN = 0x120000, LENGTH = 256
}
SECTIONS
{
	.exvectors 0xFFFFFF80: AT(0xFFFFFF80)
	{
		_exvectors_start = .;
		KEEP(*(.exvectors))
		_exvectors_end = .;
	} >ROM
	.fvectors 0xFFFFFFFC: AT(0xFFFFFFFC)
	{
		KEEP(*(.fvectors))
	} > ROM
	.text 0xFFF80000: AT(0xFFF80000)
	{
		*(.text)
		KEEP(*(.text.*ISR))
		KEEP(*(.text.*_isr))
		KEEP(*(.text.*_interrupt))
		*(.text.*)
		*(P)
		etext = .;
	} > ROM
	.rvectors ALIGN(4):
	{
		_rvectors_start = .;
		INCLUDE ../src/smc_gen/r_bsp/mcu/all/linker_script_rvectors.inc
		_rvectors_end = .;
	} > ROM
	.init :
	{
		KEEP(*(.init))
		__preinit_array_start = .;
		KEEP(*(.preinit_array))
		__preinit_array_end = .;
		__init_array_start = (. + 3) & ~ 3;
		KEEP(*(.init_array))
		KEEP(*(SORT(.init_array.*)))
		__init_array_end = .;
		__fini_array_start = .;
		KEEP(*(.fini_array))
		KEEP(*(SORT(.fini_array.*)))
		__fini_array_end = .;
	} > ROM
	.fini :
	{
		KEEP(*(.fini))
	} > ROM
	.got :
	{
		*(.got)
		*(.got.plt)
	} > ROM
	.rodata :
	{
		*(.rodata)
		*(.rodata.*)
		*(C_1)
		*(C_2)
		*(C)
		_erodata = .;
	} > ROM
	.eh_frame_hdr :
	{
		*(.eh_frame_hdr)
	} > ROM
	.eh_frame :
	{
		*(.eh_frame)
	} > ROM
	.jcr :
	{
		*(.jcr)
	} > ROM
	.tors :
	{
		__CTOR_LIST__ = .;
		. = ALIGN(2);
		___ctors = .;
		*(.ctors)
		___ctors_end = .;
		__CTOR_END__ = .;
		__DTOR_LIST__ = .;
		___dtors = .;
		*(.dtors)
		___dtors_end = .;
		__DTOR_END__ = .;
		. = ALIGN(2);
		_mdata = .;
	} > ROM
	.data : AT(_mdata)
	{
		_data = .;
		*(.data)
		*(.data.*)
		*(D)
		*(D_1)
		*(D_2)
		_edata = .;
	} > RAM
	.gcc_exc :
	{
		*(.gcc_exc)
	} > RAM
	.bss :
	{
		_bss = .;
		*(.bss)
		*(.bss.**)
		*(COMMON)
		*(B)
		*(B_1)
		*(B_2)
		_ebss = .;
		_end = .;
	} > RAM
	.ofs1 0x120040: AT(0x120040)
	{
		KEEP(*(.ofs1))
	} > OFS
	.ofs2 0x120048: AT(0x120048)
	{
		KEEP(*(.ofs2))
	} > OFS
	.ofs3 0x120050: AT(0x120050)
	{
		KEEP(*(.ofs3))
	} > OFS
	.ofs4 0x120060: AT(0x120060)
	{
		KEEP(*(.ofs4))
	} > OFS
	.r_bsp_NULL :
	{
		. += 0x100;
		"_r_bsp_NULL_end" = .;
	} >RAM
.r_bsp_istack BLOCK(0x4) (NOLOAD) :
	{
		KEEP(*(.r_bsp_istack))
	} >RAM
.istack :
	{
		"_istack" = .;
	} >RAM
.r_bsp_ustack BLOCK(0x4) (NOLOAD) :
	{
		KEEP(*(.r_bsp_ustack))
	} >RAM
.ustack :
	{
		"_ustack" = .;
	} >RAM
}
