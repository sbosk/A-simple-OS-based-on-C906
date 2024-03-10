.macro reg_save base
	sd  ra, 0(\base)
	sd  sp, 8(\base)
	sd  gp, 16(\base)
	sd  tp, 24(\base)
	sd  t0, 32(\base)
	sd  t1, 40(\base)
	sd  t2, 48(\base)
	sd  s0, 56(\base)
	sd  s1, 64(\base)
	sd  a0, 72(\base)
	sd  a1, 80(\base)
	sd  a2, 88(\base)
	sd  a3, 96(\base)
	sd  a4, 104(\base)
	sd  a5, 112(\base)
	sd  a6, 120(\base)
	sd  a7, 128(\base)
	sd  s2, 136(\base)
	sd  s3, 144(\base)
	sd  s4, 152(\base)
	sd  s5, 160(\base)
	sd  s6, 168(\base)
	sd  s7, 176(\base)
	sd  s8, 184(\base)
	sd  s9, 192(\base)
	sd  s10, 200(\base)
	sd  s11, 208(\base)
	sd  t3, 216(\base)
	sd  t4, 224(\base)
	sd  t5, 232(\base)
	sd  t6, 240(\base)
.endm
.macro reg_restore base
	ld  ra, 0(\base)
	ld  sp, 8(\base)
	ld  gp, 16(\base)
	ld  tp, 24(\base)
	ld  t0, 32(\base)
	ld  t1, 40(\base)
	ld  t2, 48(\base)
	ld  s0, 56(\base)
	ld  s1, 64(\base)
	ld  a0, 72(\base)
	ld  a1, 80(\base)
	ld  a2, 88(\base)
	ld  a3, 96(\base)
	ld  a4, 104(\base)
	ld  a5, 112(\base)
	ld  a6, 120(\base)
	ld  a7, 128(\base)
	ld  s2, 136(\base)
	ld  s3, 144(\base)
	ld  s4, 152(\base)
	ld  s5, 160(\base)
	ld  s6, 168(\base)
	ld  s7, 176(\base)
	ld  s8, 184(\base)
	ld  s9, 192(\base)
	ld  s10, 200(\base)
	ld  s11, 208(\base)
	ld  t3, 216(\base)
	ld  t4, 224(\base)
	ld  t5, 232(\base)
	ld  t6, 240(\base)
.endm
# interrupts and exceptions while in machine mode come here.
.globl trap_vector
# the trap vector base address must always be aligned on a 4-byte boundary
.align 4
trap_vector: #在跳转到这个函数的时候，cpu会自动实现 寄存器mstatus MIE清0(关中断),设置mepc为pc
            #pc值变为mtvec的地址的值，根据trap的种类，设置mcause，再把hart的权限设置为machine模式
    csrrw	t6, mscratch, t6	# swap t6 and mscratch
	reg_save t6                 #保存被打断的流的环境
    csrr t5,mscratch            # t5 为最初 t6的值
	sd  t5,240(t6)				#把t5的值(t6)写入结构体, 完成上下文保存
    csrw	mscratch, t6
    	# call the C trap handler in trap.c
	csrr	a0, mepc     //传参：被打断流的pc指针的值
	csrr	a1, mcause  //传参：发生中断的原因
	call	trap_handler//调用函数
    	# trap_handler will return the return address via a0.
	csrw	mepc, a0    //返回值：写入mepc
    	# restore context(registers).
	csrr	t6, mscratch
	reg_restore t6
    mret //恢复权限，开中断，pc = mepc
.global all_interrupt_enable

.align 4
all_interrupt_enable:
    csrs mstatus, 8
ret