#include "switch.h"
#include "mem_malloc.h"
struct Thread os_thread;

extern void switch_to(struct Context *next);
void init_so_thread(void)
{
    uintptr_t os_context_address = (uintptr_t)&os_thread.context;
    asm volatile (
        "csrw mscratch, %0" // 将地址存储到 mscratch
        :
        : "r" (os_context_address) // 输入操作数，存储 os_thread 的地址
    );
}
struct Thread * init_thread(void function(void))
{
    struct Thread * thread = mem_alloc(sizeof(struct Thread));
    thread->context.sp = (reg_t )&thread->stack[1024 * 1024 * 8];
    thread->context.ra = (reg_t)function;
}
void schedule( struct Thread * targetThread)
{
    switch_to(&(targetThread -> context));
}
