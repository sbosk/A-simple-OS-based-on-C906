/*mxstatus*/
#define    EN_THEADISAEE    (0x1 << 22 | 0x1 << 17)
#define mcor 0x7C2
#define mxstatus 0x7C0
.extern _stack_start
.globl _start 
.section .entry
_start:
    /*disable interrupt*/
    csrw mie, zero

    /*enable theadisaee*/
    li t1, EN_THEADISAEE
    csrs mxstatus, t1
    
    /*invaild ICACHE/DCACHE/BTB/BHT*/
    li t2, 0x30013
    csrs mcor, t2

        # 加上8M的偏移量
    li t1, 8*1024*1024
    la t0,  _stack_start
    add t0, t0, t1

    jal main 
    # return to FEL 
uart_send:
    li a0,0x02500000
    li a1,'A'
    sw a1, 0(a0)
    j uart_send
