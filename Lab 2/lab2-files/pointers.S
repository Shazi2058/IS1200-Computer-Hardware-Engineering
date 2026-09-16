# pointers.S
# By David Broman, 2015-09-14
# Ported 2024/??/?? by W Szczerek (from MIPS to RISC-V)
# This file is in the public domain.


# Choose the syntax for RARS or DTEK-V board.
# The syntax for RARS is probably due to its MIPS heritage.
###################
# Macros for RARS #
###################
.macro	PUSH (%reg)
	addi	sp,sp,-4
	sw	%reg,0(sp)
.end_macro

.macro	POP (%reg)
	lw	%reg,0(sp)
	addi	sp,sp,4
.end_macro
###################
# Macros for DTEK #
###################
#.macro	PUSH reg
#	addi sp,sp,-4
#	sw \reg,0(sp)
#.endm

#.macro	POP reg
#	lw	\reg,0(sp)
#	addi	sp,sp,4
#.endm
###################

# ---------------------------------------------------------------------
# .data section: two source strings and two destination buffers, plus
# one shared integer counter. You will need analogous declarations in
# pointers.c.
#
# Directive cheat sheet:
#   .asciz "..."   stores a null-terminated string of bytes.
#   .space N       reserves N bytes of uninitialised storage.
#   .word V        stores one machine word (4 bytes on RISC-V),
#                  initialised to the value V.
# ---------------------------------------------------------------------
	.data
text1:	.asciz	"This is a string."
text2:	.asciz	"Yet another thing."

	.align 2
list1:	.space	80		# 80 uninitialised bytes
list2:	.space	80
counter:.word	0		# one 4-byte int, initialised to 0

	.text
	.globl main
	.type main, @function

# ---------------------------------------------------------------------
# main: calls work() once, then spins forever so the simulator stops.
# ---------------------------------------------------------------------
main:
	jal	work
stop:	j	stop


# ---------------------------------------------------------------------
# function work()
#
# Sets up arguments for copycodes twice. The RISC-V calling convention
# puts the first three arguments in registers a0, a1, a2 (in that
# order). The `la` (load address) instruction places the memory
# address at which a labelled variable lives -- i.e., a pointer to
# it -- into the destination register. It is essentially "give me the
# address of this name", not "give me the value stored there". The
# same counter is reused for both calls.
# ---------------------------------------------------------------------
work:
	PUSH	ra			# save return address (we will call copycodes)
	la	a0,text1		# a0 = address of text1 (i.e. a pointer to its first byte)
	la	a1,list1		# a1 = address of list1
	la	a2,counter		# a2 = address of counter
	jal	copycodes

	la	a0,text2		# a0 = address of text2
	la	a1,list2		# a1 = address of list2
	la	a2,counter		# a2 = address of the same counter
	jal	copycodes
	POP	ra			# restore return address
	jr	ra


# ---------------------------------------------------------------------
# function copycodes()
#
# Receives three pointers in registers a0, a1, a2 (RISC-V calling
# convention). The loop reads one byte through a0, writes it through
# a1, increments the integer at a2, and stops when a zero byte is
# read. Pay attention to the two `addi` lines: a0 advances by 1 each
# iteration while a1 advances by 4. Think why.
# ---------------------------------------------------------------------
copycodes:
loop:
	lb	t0,0(a0)		# read one byte through a0
	beqz	t0,done			# When does the loop stop? What does that value represent in this context?
	sw	t0,0(a1)		# store t0 (4 bytes) at the address in a1

	addi	a0,a0,1			# a0 += 1
	addi	a1,a1,4			# a1 += 4

	lw	t1,0(a2)		# Think what a2 is.
	addi	t1,t1,1
	sw	t1,0(a2)
	j	loop
done:
	jr	ra
