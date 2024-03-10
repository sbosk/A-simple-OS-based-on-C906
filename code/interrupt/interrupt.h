#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_
#include "switch.h"
#include "basc.h"
#include "riscv64.h"
//mtvec 存放中断基地址，中断的跳转方式
//mepc 发生中断的时候，存储当前程序执行位置（pc的值）的寄存器
//指令mret 会从mepc中获取值，然后返回到那个位置
//mcause 获取异常或中断的原因
//MTVAL  获取更多的信息
//MSTATUS 管理中断的开启与关闭，和发生中断的时候，cpu的一些信息
struct trap_Thread
{
    struct  Context context;
    uint8_t stack[1024 * 1024 * 1]; //异常处理我们只给1M的栈空间
};
extern void all_interrupt_enable(void);
void triggerSoftwareInterrupt(void);
void trap_init(void);
void clint_soft_irq_init(void);
uint64_t counter(void);
void clint_set_next_initerr(uint32_t add_time);
#define D1_MSIP0        0x14000000
#define D1_MTIMECMPL0   0x14004000
#define D1_MTIMECMPH0   0x14004004

#define D1_SSIP0        0x1400C000
#define D1_STIMECMPL0   0x1400D000
#define D1_STIMECMPH0   0x1400D004
#endif
