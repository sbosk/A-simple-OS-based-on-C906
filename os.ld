OUTPUT_FORMAT("elf64-littleriscv", "elf64-littleriscv", "elf64-littleriscv")
OUTPUT_ARCH("riscv")
ENTRY(_start)
MEMORY
{
	ram   (wxa!ri) : ORIGIN = 0x40000000, LENGTH = 512M
}
/*小地址到大地址*/
SECTIONS
{
	
	.text : {
		PROVIDE(_text_start = .);/*赋值符号_text_start，可做一个变量来使用*/
		*(.entry)
		*(.text .text.*)
		PROVIDE(_text_end = .);
	} >ram

	.rodata : { /*只读数据段*/
		PROVIDE(_rodata_start = .);
		*(.rodata .rodata.*)
		PROVIDE(_rodata_end = .);
	} >ram

	.data : {
		. = ALIGN(4096);
		PROVIDE(_data_start = .);
		*(.sdata .sdata.*)
		*(.data .data.*)
		PROVIDE(_data_end = .);
	} >ram

	.bss :{ 
		PROVIDE(_bss_start = .);
		*(.sbss .sbss.*)
		*(.bss .bss.*)
		*(COMMON)
		PROVIDE(_bss_end = .);
	} >ram
	PROVIDE(_stack_start = _bss_end);
    /*ORIGIN(ram)是内存的开始地址*/ 
	PROVIDE(_memory_start = ORIGIN(ram));
	PROVIDE(_memory_end = ORIGIN(ram) + LENGTH(ram));
}

