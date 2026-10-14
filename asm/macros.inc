@ Function headers for the original assembly (build/expected/*.s includes asm/UNIT/FUNC.s
@ between them). agbcc starts every function on a 4-byte boundary with zero padding.

	.macro thumb_func_start name
	.align 2, 0
	.globl \name
	.thumb
	.thumb_func
	.type \name, %function
\name:
	.endm

	.macro non_word_aligned_thumb_func_start name
	.globl \name
	.thumb
	.thumb_func
	.type \name, %function
\name:
	.endm

	.macro arm_func_start name
	.align 2, 0
	.globl \name
	.arm
	.type \name, %function
\name:
	.endm

	.macro func_end name
	.size \name, .-\name
	.endm
