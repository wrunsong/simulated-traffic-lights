OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

STACK_FIQ_SIZE 	= 0x0400;
STACK_IRQ_SIZE 	= 0x0400;
STACK_ABT_SIZE 	= 0x0400;
STACK_UND_SIZE 	= 0x0400;
STACK_SRV_SIZE 	= 0x8000;

MEMORY
{
	rom (rx)	: org = 0x34000000, len = 0x02000000	/* 32 MB */
	ram (rwx)	: org = 0x36000000, len = 0x0a000000	/* 160 MB */
}

SECTIONS
{   
	.text :
	{
		. = ALIGN(8);
		PROVIDE (__text_start = .);
		.obj/source/startup/start.o (.text)
		.obj/source/startup/s5pv210-irom.o (.text)
		*(.text)
		*(.text.*)

		. = ALIGN(8);
		*(.rodata);
		*(.rodata.*);

		. = ALIGN(8);
		*(.glue_7);
		*(.glue_7t);

		. = ALIGN(8);
		PROVIDE (__text_end = .);
	} > rom

	.data_shadow ALIGN(8) :
	{
		PROVIDE (__data_shadow_start = .);
		PROVIDE (__data_shadow_end = (. + SIZEOF (.data)) );
	} > rom

	.data : AT ( ADDR (.data_shadow) )
	{
		PROVIDE (__data_start = .);	
		*(.data)
		. = ALIGN(8);
  		PROVIDE (__data_end = .);		
	} > ram

	.bss ALIGN(8) (NOLOAD) :
	{
		PROVIDE (__bss_start = .);
		*(.bss)
		*(.bss.*)
		*(.sbss)
		*(COMMON)
		PROVIDE (__bss_end = .);
		
		. = ALIGN(8);
		PROVIDE (__heap_start = .);
		*(.heap)
		. = ALIGN(8);
		PROVIDE (__heap_end = .);
		
		. = ALIGN(8);
		PROVIDE (__stack_start = .);
		PROVIDE (__stack_fiq_start = .);
		. += STACK_FIQ_SIZE;
		PROVIDE (__stack_fiq_end = .);
		. = ALIGN(8);
		PROVIDE (__stack_irq_start = .);
		. += STACK_IRQ_SIZE;
		PROVIDE (__stack_irq_end = .);
		. = ALIGN(8);
		PROVIDE (__stack_abt_start = .);
		. += STACK_ABT_SIZE;
		PROVIDE (__stack_abt_end = .);
		. = ALIGN(8);
		PROVIDE (__stack_und_start = .);
		. += STACK_UND_SIZE;
		PROVIDE (__stack_und_end = .);
		. = ALIGN(8);
		PROVIDE (__stack_srv_start = .);
		. += STACK_SRV_SIZE;
		PROVIDE (__stack_srv_end = .);
		. = ALIGN(8);
		PROVIDE (__stack_end = .);
	} > ram

	/*
	 * Stabs debugging sections.
	 */
	.stab 0 : { *(.stab) }
	.stabstr 0 : { *(.stabstr) }
	.stab.excl 0 : { *(.stab.excl) }
	.stab.exclstr 0 : { *(.stab.exclstr) }
	.stab.index 0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }
	.comment 0 : { *(.comment) }
	.debug_abbrev 0 : { *(.debug_abbrev) }
	.debug_info 0 : { *(.debug_info) }
	.debug_line 0 : { *(.debug_line) }
	.debug_pubnames 0 : { *(.debug_pubnames) }
	.debug_aranges 0 : { *(.debug_aranges) }
}
