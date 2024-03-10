#ifndef _SWITCH_H_
#define _SWITCH_H_
#include "basc.h"
extern struct Thread os_thread;
typedef uint64_t reg_t;
struct Context {
	/* ignore x0 */
	reg_t ra;
	reg_t sp; //栈指针
	reg_t gp;
	reg_t tp;
	reg_t t0;
	reg_t t1;
	reg_t t2;
	reg_t s0;
	reg_t s1;
	reg_t a0;
	reg_t a1;
	reg_t a2;
	reg_t a3;
	reg_t a4;
	reg_t a5;
	reg_t a6;
	reg_t a7;
	reg_t s2;
	reg_t s3;
	reg_t s4;
	reg_t s5;
	reg_t s6;
	reg_t s7;
	reg_t s8;
	reg_t s9;
	reg_t s10;
	reg_t s11;
	reg_t t3;
	reg_t t4;
	reg_t t5;
	reg_t t6;
};
struct Thread
{
    struct  Context context;
    uint8_t stack[1024 * 1024 * 8];
};
struct Thread * init_thread(void function(void));
void schedule( struct Thread * thread);
void init_so_thread(void);
#endif