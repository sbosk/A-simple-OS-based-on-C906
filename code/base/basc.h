#ifndef _MAIN_H_
#define _MAIN_H_
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int int16_t;
typedef unsigned short int uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
typedef signed long int int64_t;
typedef unsigned long int uint64_t;
typedef unsigned long int	uintptr_t;
typedef unsigned long long		virtual_addr_t;
void put32(uintptr_t addr, uint32_t val);
uint32_t get32(uintptr_t addr);
uint32_t read32(virtual_addr_t addr);
void write32(virtual_addr_t addr, uint32_t value);
uint64_t get_arch_counter(void);
void delay_us(uint64_t us);
void delay_ms(uint64_t ms);
void assert(int i);
uint64_t read_register(uint8_t register_num);
#endif