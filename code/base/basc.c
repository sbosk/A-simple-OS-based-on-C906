#include "basc.h"
#include "printf.h"
uint64_t read_register(uint8_t register_num)
{
    uint64_t value = 0;
    if (register_num == 1)
    {
        asm volatile(
            "csrr %0, mstatus\n"
            : "=r"(value)
            :
            :); // shouldn't need clobber
    }
    if (register_num == 2)
    {
        asm volatile(
            "csrr %0, mie\n"
            : "=r"(value)
            :
            :); // shouldn't need clobber
    }
    if (register_num == 3)
    {
        asm volatile(
            "csrr %0, 0x7C0\n"
            : "=r"(value)
            :
            :); // shouldn't need clobber
    }
    if(register_num == 4)
    {
        value = read32(0x14000000L+0x4000);
    }
    return value;
}
inline void put32(uintptr_t addr, uint32_t val)
{
    *((volatile uint32_t *)addr) = val;
}
inline uint32_t get32(uintptr_t addr)
{
    return *((volatile uint32_t *)addr);
}
inline __attribute__((__always_inline__)) uint32_t read32(virtual_addr_t addr)
{
    return (*((volatile uint32_t *)(addr)));
};
inline __attribute__((__always_inline__)) void write32(virtual_addr_t addr, uint32_t value)
{
    *((volatile uint32_t *)(addr)) = value;
}
inline uint64_t get_arch_counter(void)
{
    uint64_t cnt = 0;
    asm volatile(
        "csrr %0, time\n"
        : "=r"(cnt)
        :
        :); // shouldn't need clobber
    return cnt;
}
void delay_us(uint64_t us)
{
    uint64_t us24 = us * 24UL;
    uint64_t start = get_arch_counter();
    while (1)
    {
        if (get_arch_counter() - start >= us24)
        {
            break;
        }
    }
}
inline void delay_ms(uint64_t ms)
{
    return delay_us(ms * 1000UL);
}
void assert(int i)
{
    if (i == 0)
    {
        debug("assert error");
    }
}