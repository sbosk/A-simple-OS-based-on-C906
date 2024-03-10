#include "interrupt.h"
#include "printf.h"
#include "riscv64.h"
#define CLINT (0x14000000L)
struct trap_Thread trap_thread;
extern void trap_vector(void);
uint64_t counter(void)
{
    uint64_t cnt;
    __asm__ __volatile__("csrr %0, time\n" : "=r"(cnt)::"memory");
    return cnt;
}
void clint_soft_irq_start(void)
{
    write32(CLINT, 1);
}
void clint_set_next_initerr(uint32_t add_time)
{
    write32(CLINT + 0x4000, (uint32_t)counter() + 10000);
    write32(CLINT + 0x4004, 0);
}
void clint_soft_irq_init(void)
{
    csr_clear(mie, MIP_MTIP | MIP_MSIP);
    csr_set(mie, MIP_MSIP); // set m-mode sip
}

void trap_init(void) 
{
    asm volatile("csrw mtvec, %0" : : "r"(trap_vector)); // 声明发生异常的时候跳转到trap_vector
}
reg_t trap_handler(reg_t epc, reg_t cause)
{
    write32(CLINT, 0);
    reg_t cause_code = cause & 0xf;
    if (cause & ((reg_t)1 << 63))
    {
        debug("中断参数:0x:%x", cause);
        debug("触发了中断time:0x:%x", counter());
        clint_set_next_initerr(1000000);
        delay_ms(500);
    }
    else
    {
        debug("触发了异常time:0x:%x", counter());
        clint_set_next_initerr(1000000);
        delay_ms(500);
    }
    while(1)
    {
        debug("in irq");
        delay_ms(1000);
    }
    return epc;
}