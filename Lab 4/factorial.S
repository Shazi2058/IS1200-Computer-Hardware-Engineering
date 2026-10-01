.text
	addi t0,zero,3     # t0 = input 
	addi t2,zero,1	  # Starter 1 1*1
	addi  t1,zero,1  # To check if the input is 1

outer_loop:
	beq t0, zero, stop # 0! is 1 so checking if input is 0
	beq t0, t1, stop # 1! is 1 so checking if input is 1
	addi sp, zero, 0  # the additon for multiplication
	addi gp, zero, 0 # Counter for inner loop

inner_loop:
	add sp, sp, t2 # sp = sp + t2
	addi gp, gp, 1 # n= n+1
	beq gp, t0, done # adding t2 total of t0 times
	beq zero, zero, inner_loop # jump back to look if havent added t2 total of t0 times

done:
	add t2, zero, sp        # t2 = new sp / new total
	addi t0, t0, -1         # n = n - 1
	beq zero, zero, outer_loop # jump back to outer_loop

stop:
  	beq zero, zero, stop  # Infinite loop to end program
		
