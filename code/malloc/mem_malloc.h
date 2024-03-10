#ifndef _MALLOC_H_
#define _MALLOC_H_
#include "basc.h"

extern uint64_t bss_end;
extern uint64_t memory_end;
void block_printf(void);
uint32_t read_4byte(uint64_t addr);
int heap_init(void);
void * mem_alloc(uint32_t size);
int mem_free(uint64_t addr);
#endif 