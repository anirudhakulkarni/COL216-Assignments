# MIPS Assembly Program for evaluating an expression in postfix format

# -------------------------

# Data Declarations

.data 

    prompt:     .asciiz     "Enter the Postfix Expression: "
    result:     .asciiz     "The Expression evaluates to: "
    lengthIs:     .asciiz     "Length of exp is: "
    buffer:     .space      100
    endl:       .asciiz "\n"
# -------------------------
# Text / Code Section

.text
.globl  main
.ent    main
main:

    li		$v0, 4		
    la		$a0, prompt 
    syscall
    
    # Getting user's input
    li $v0, 8
    la $a0, buffer		# 
    li $a1, 100
    syscall
    move 	$s0, $a0
    move 	$s1, $s0
    # we have input string address in s0 and s1
    jal		stringLength				# jump to stringLength and save position to $ra
    # now string length in s0
    # print length of the string
    
    jal		printLength				# jump to printEnd and save position to $ra
    jal		printEnd				# jump to printEnd and save position to $ra


    # Loop initialisation
    li		$t0, 1		# $t0 = 1 Loop iteral
    move 	$t1, $s1		# $t1 = $s1 address
    forLoop:
        lb		$a0, 0($t1)	
        andi $a0,$a0,0x0F
        add		$a0, $a0, 15		# $a0 = $a0 + 15
        addi	$t1, $t1, 1			# $t1 = $t1 + 1
        
        li		$v0, 1		# $v0 =0 
        syscall
        jal		printEnd				# jump to printEnd and save position to $ra
        
        addi	$t0, $t0, 1			# $t0 = $t0 + 1
        ble		$t0, $s0, forLoop	# if $t0 <= $s0 then forLoop
        
        

    

    la		$a0, result		# 
    li		$v0, 4		# $v0 = 4
    syscall

    la		$a0, buffer		# 
    li		$v0, 4		# $v0 = 4
    syscall
        li		$v0, 10		# $v0 = 10
        syscall # all done!
.end main

stringLength:
    li		$t0, -1		# $t0 = 0
    loop:
        
        lb		$t1, 0($s0)		# 
        beqz    $t1, exit
        addi	$s0, $s0, 1			# $s0 = $s0 + 1
        addi	$t0, $t0, 1			# $t0 = $t0 + 1
        j		loop				# jump to loop
    exit:

        move 	$s0, $t0		# $s0 = $s0
        jr		$ra					# jump to $ra
        
printEnd:
    li		$v0, 4		# $v0 = 4
    la		$a0, endl		# 
    syscall
    jr		$ra					# jump to $ra

printLength:
    li		$v0, 4		# $v0 = 4
    la		$a0, lengthIs		# 
    syscall
    li		$v0, 1		# $v0 = 4
    move	$a0, $s0		# 
    syscall
    jr		$ra					# jump to $ra
