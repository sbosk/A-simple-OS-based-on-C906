#include "uart.h"
struct uart_control *uart0_ctl;
void uart_clock_init(int port);
void uart_set_gpio(void);
struct uart_control *uart_init(int port, int set_gpio)
{
    if (!(port >= 0 && port <= 5))
    {
        return (struct uart_control *)0;
    }
    /// set clock
    uart_clock_init(port);

    /// set gpio
    if (set_gpio)
    {
        uart_set_gpio();
    }
    struct uart_control *ctl = (struct uart_control *)((uintptr_t)UART0_BASE + port * 0x400UL);
    struct uart_halt *hlt = (struct uart_halt *)((uintptr_t)UART0_BASE + port * 0x400UL + 0x00A4UL);
    uint32_t uart_clock_divisor = (24000000 + 8 * UART_BAUD) / (16 * UART_BAUD);
    // write fcr[FIFOE] to 1 to enable TX/RX fifos
    ctl->fcr |= (1 << 0);
    // write halt[HALT_TX] to 1 to disable TX transfer
    hlt->halt |= (1 << 0);
    // set lcr[DLAB] to 1 to access the dlh and dll registers
    ctl->lcr |= 0x80; // enable the divisor latch access bit to set baud: lcr[DLAB] = 1
    // set the baud rate
    ctl->dlh = ((uart_clock_divisor >> 8) & 0xFF);
    ctl->dll = (uart_clock_divisor & 0xFF);
    // set lcr[DLAB] to 0 to disable latch access
    ctl->lcr &= (~0x80); // disable the latch access
    ////////////////////////////
    // set data width, stop bit, parity bit
    ctl->lcr = ((PARITY & 0x03) << 3) | ((STOP & 0x01) << 2) | (DLEN & 0x03); // 8n1
    // reset, enable FIFO and set FIFO trigger condition by writing to the fcr register
    ctl->fcr = 0x7;
    // set flow control parameter by writing to the mcr register
    ctl->mcr = 0b11;
    // set halt[HALT_TX] to 0 to enable TX transfer
    hlt->halt &= ~(1 << 0);
    if (port == 0)
    {
        uart0_ctl = ctl;
    }
    return ctl;
}
void inline uart_putc(char c) {
    while ((uart0_ctl->lsr & (1 << 6)) == 0) {} 
    uart0_ctl->thr = c; 
}
void uart_send(const char *str)
{
    while (*str != '\0')
    {
        uart_putc(*str);
        str++;
    }
}
void uart_clock_init(int port)
{
    uint32_t val;
    /* enable module clock */
    val = get32(CCU_UART_BGR_REG);
    val |= 1 << (CCM_UART_GATING_OFFSET + port);
    put32(CCU_UART_BGR_REG, val);

    /* de-assert the module */
    val = get32(CCU_UART_BGR_REG);
    val |= 1 << (CCM_UART_RST_OFFSET + port);
    put32(CCU_UART_BGR_REG, val);
}
#define PB_CFG1 0x0034
#define PB_DRV1 0x0048
#define PB_PULL0 0x0054
#define GPIO_BASE (0x02000000)
void uart_set_gpio(void)
{
    write32(GPIO_BASE + PB_CFG1, 0x66);
    write32(GPIO_BASE + PB_DRV1, 0x22);
    write32(GPIO_BASE + PB_PULL0, 0x50000);
}
char uart_getc()
{
    while ((uart0_ctl->lsr & 1) == 0)
    {
    }
    return uart0_ctl->rbr;
}