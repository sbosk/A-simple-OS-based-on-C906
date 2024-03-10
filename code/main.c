#include "uart.h"
#include "basc.h"
#include "printf.h"
#include "mem_malloc.h"
#include "switch.h"
#include "interrupt.h"
uint64_t stack_pointer;
void show_hello(void)
{
	while(1)
	{
		debug("hello_risc-v");
		delay_ms(1000);
		schedule(&os_thread);
	}
}
void main(void)
{
	uart_init(0, 1);
	heap_init();
	init_so_thread();
	trap_init();
	struct Thread * thread = init_thread(show_hello);
	all_interrupt_enable();
	clint_soft_irq_init();
	clint_set_next_initerr(10000);
	while (1)
	{
		debug("hello_os");
		delay_ms(2000);
		// clint_soft_irq_start();
		debug("mstatus:0x%x", read_register(1));
		debug("mie:0x%x", read_register(2));
		debug("mxstatus:0x%x", read_register(3));
		debug("time:0x%x:0x%x", read_register(4),counter());
	}
}
