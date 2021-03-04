MEMORY
{
	VEC : ORIGIN = 0x0, LENGTH = 4
	IVEC : ORIGIN = 0x4, LENGTH = 188
	OPT : ORIGIN = 0xC0, LENGTH = 4
	SEC_ID : ORIGIN = 0xC4, LENGTH = 10
	OCDSTAD : ORIGIN = 0xCE, LENGTH = 10
	OCDROM : ORIGIN = 0x7ff00, LENGTH = 0x100
	ROM : ORIGIN = 0xD8, LENGTH = 523560
	MIRROR : ORIGIN = 0xF3000, LENGTH = 3840
	RAM : ORIGIN = 0xF3F00, LENGTH = 49152
}
SECTIONS
{
	.vec 0x0: AT(0x0)
	{
		KEEP(*(.vec))
	} > VEC
	.vects 0x4: AT(0x4)
	{
		KEEP(*(.vects))
	} > IVEC
	.option_bytes 0xc0 : AT(0xc0)
	{
		KEEP(*(.option_bytes))
	} > OPT
	.security_id 0xc4 : AT(0xc4)
	{
		KEEP(*(.security_id))
	} > SEC_ID
	.ocdstad 0xCE : AT(0xCE)
	{
		LONG( 0xFFFFFFFF );
		LONG( 0xFFFFFFFF );
		SHORT( 0xFFFF );
	} > OCDSTAD
	.lowtext 0xD8: AT(0xD8)
	{
		*(.plt)
		*(.lowtext)
		*(.lowtext.*)
		. = ALIGN(2);
	} > ROM
	.tors : /* TODO: Does this section work? Should below items be in the .rodata section? */
	{
		__CTOR_LIST__ = .;
		. = ALIGN(2);
		___ctors = .;
		KEEP(*(.ctors))
		___ctors_end = .;
		__CTOR_END__ = .;
		__DTOR_LIST__ = .;
		___dtors = .;
		KEEP(*(.dtors))
		___dtors_end = .;
		__DTOR_END__ = .;
		. = ALIGN(2);
	} > ROM
	.mdata (NOLOAD) :
	{
		_mdata = .;
		. += __romdatacopysize;
	} > ROM
/*
	.text :
	{
		*(.text)
		*(.text.*)
		etext = .;
		. = ALIGN(2);
	} > ROM
*/
	PROVIDE(__rl78_abs__ = 0); /* TODO: What is this? */
	.init : /* TODO: Is there any restriction to locate this section? */
	{
		*(.init)
	} > ROM
	.fini : /* TODO: Is there any restriction to locate this section? */
	{
		KEEP(*(.fini))
	} > ROM
	.got : /* TODO: Is there any restriction to locate this section? */
	{
		*(.got)
		*(.got.plt)
	} > ROM
	PROVIDE(__mirror = 0x3000);
	PROVIDE(__mirror_end = __mirror + 3840);
	PROVIDE(__rodata = ADDR(.rodata));
	.rodata MAX(., __mirror): 
	{
		. = ALIGN(2);
		*(.rodata)
		*(.rodata.*)
		_erodata = .;
		ASSERT(!(SIZEOF(.rodata) > (__mirror_end - __mirror)), "Error: Too much const data - the size exceeds the mirror area");
		ASSERT(!(SIZEOF(.rodata) > (__mirror_end - __mirror)), "Please change some of them from 'const' to 'const __far'");
		ASSERT(!((SIZEOF(.rodata) <= (__mirror_end - __mirror)) && (SIZEOF(.rodata) > 0) && (ABSOLUTE(__rodata) >= __mirror_end)), "Error: No room left for const data - the start address exceeds the mirror area");
		ASSERT(!((SIZEOF(.rodata) <= (__mirror_end - __mirror)) && (SIZEOF(.rodata) > 0) && (ABSOLUTE(__rodata) < __mirror_end) && (ABSOLUTE(.) > __mirror_end)), "Error: No room left for const data - the end address exceeds the mirror area");
		ASSERT(!((SIZEOF(.rodata) <= (__mirror_end - __mirror)) && (SIZEOF(.rodata) > 0) && (ABSOLUTE(.) > __mirror_end) && (ADDR(.text) < __rodata)), "Please move the .text section from lower address to higher address");
		ASSERT(!((SIZEOF(.rodata) <= (__mirror_end - __mirror)) && (SIZEOF(.rodata) > 0) && (ABSOLUTE(.) > __mirror_end) && (__rodata < ADDR(.text))), "Please check the map file");
	} > ROM
	.text :
	{
		*(.text)
		*(.text.*)
		etext = .;
		. = ALIGN(2);
	} > ROM
	.frodata :
	{
		. = ALIGN(2);
		*(.frodata)
		*(.frodata.*)
		_efrodata = .;
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
		. = ALIGN(2);
	} > ROM
	.data 0xF3F00: AT(_mdata)
	{
		. = ALIGN(2);
		_data = .;
		*(.data)
		*(.data.*)
		. = ALIGN(2);
		_edata = .;
	} > RAM
	PROVIDE(__romdatacopysize = SIZEOF(.data));
	.bss :
	{
		. = ALIGN(2);
		_bss = .;
		*(.bss)
		*(.bss.**)
		. = ALIGN(2);
		*(COMMON)
		. = ALIGN(2);
		_ebss = .;
		_end = .;
	} > RAM
	PROVIDE(stack_size = 0x64);
	.stack 0xFFCFE (NOLOAD) : AT(0xFFCFE)
	{
		_stack = .;
		ASSERT((_stack > (_end + stack_size)), "Error: Too much data - no room left for the stack");
	} > RAM
}
