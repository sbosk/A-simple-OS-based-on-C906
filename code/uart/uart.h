#ifndef _UART_H
#define _UART_H
#include "basc.h"

#define UART0_BASE 0x02500000
#define UART1_BASE 0x02500400
#define UART2_BASE 0x02500800
#define UART3_BASE 0x02500C00
#define UART4_BASE 0x02501000
#define UART5_BASE 0x02501400
#define CCU_BASE                  0x02001000
#define CCU_UART_BGR_REG          (CCU_BASE+0x90C)
#define CCM_UART_RST_OFFSET       (16)
#define CCM_UART_GATING_OFFSET    (0)
#define CCM_UART_ADDR_OFFSET      (0x400)
/* UART Line Control Parameter */
/* Parity: 0,2 - no parity; 1 - odd parity; 3 - even parity */
#define   PARITY       0 
/* Number of Stop Bit: 0 - 1bit; 1 - 2(or 1.5)bits */
#define   STOP         0
/* Data Length: 0 - 5bits; 1 - 6bits; 2 - 7bits; 3 - 8bit */
#define   DLEN         3 

#define   UART_BAUD    115200
// transmit holding register 
#define thr rbr  
// divisor latch low register 
#define dll rbr
// divisor latch high register 
#define dlh ier 
// interrupt identity register, NOTE: iir is all read, fcr is all write, so no conflict  
#define iir fcr
struct uart_control {
    volatile uint32_t rbr;  // receive buffer register 
    volatile uint32_t ier;  // interrupt enable register 
    volatile uint32_t fcr;  // fifo control register 
    volatile uint32_t lcr;  // line control register 
    volatile uint32_t mcr;  // modem control register 
    volatile uint32_t lsr;  // line status register 
    volatile uint32_t msr;  // modem status register 
    volatile uint32_t sch;  // scratch register 
};
struct uart_halt {
    volatile uint32_t halt;  // uart halt tx register 
};
extern struct uart_control *uart0_ctl;
void uart_putc(char c);
void uart_send(const char *str);
char uart_getc();
struct uart_control *uart_init(int port, int set_gpio);
#endif