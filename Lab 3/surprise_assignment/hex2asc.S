  # hexmain.S
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
	# Declare hexasc global too so it can be called from another file
	# (e.g. timetemplate.S, or the auto-grader's test harness).
	.globl hex2asc, hexasc

hex2asc:
	li	a0, 15		# test number (from 0 to 15)
	
	addi    sp,sp,-4
	sw      ra,0(sp)
	
	jal	hexasc		# call hexasc
	
	li	a7, 11	# write a0 to stdout
	ecall

	lw      ra,0(sp)
	addi    sp,sp,4
	jr      ra	

  # You can write your own code for hexasc here
 hexasc:
  	andi a0 , a0, 0xF 	# masking lowest 4 bits because 15 requires 4 bits like 1111 = 15
  	li t0,9 		    # loading the value into a temporary register to do the comparison
  	ble a0,t0, num	 	# if a0 is less than or equal to t0 which is 9  then goto the num function
  	addi a0, a0, 0x37 	# for (A-F)
  	jr ra			    # return
  num:
  	addi a0, a0, 0x30   # for (0-9)
  	jr ra               # return
