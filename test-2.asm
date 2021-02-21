# Name and general description of program
# ----------------------------------------
# Data declarations go in this section.
.data
# program specific data declarations
    inputline:    .asciiz  "Enter number of points: "
    iterator: .asciiz "Enter Data for point no. "
    xinput: .asciiz  "X coordinate : "
    yinput: .asciiz  "Y coordinate : "
    printarea: .asciiz "Area so far : "
    newline:  .asciiz "\n"
    areafinalprompt: .asciiz "Final calculated Area is : "
    points: .word    0
    area:   .word  0
    i:      .word   1
    two:    .double 2.0
    

# ----------------------------------------
# Program code goes in this section.
.text
.globl  main
.ent    main
main:
# -----
# your program code goes here.
# ======================== Input of number of points Start ==================
    # input n
    li		$v0, 4		
    la		$a0, inputline 
    syscall
    li		$v0, 5 		 
    syscall
    sw		$v0, points		
    lw		$t0, points		
# ======================== Input End ==================    

# ======================== initialise start ==================
    # input taken and stored into memory location points

    li		$t1, 1		# $t1 = 1
    lw		$t2, area		# $t2 = 0
# ======================== initialise End ==================

# ======================== first point start ==================

    # take x and y input

    li		$v0, 4		# $v0 = 4
    la		$a0, iterator		# 
    syscall
    li		$v0, 1		# $v0 = 4
    move	$a0, $t1		# 
    syscall
    li		$v0, 4		# $v0 = 4
    la		$a0, newline		# 
    syscall
    li		$v0, 4		# $v0 = 4
    la		$a0, xinput		# 
    syscall
    li		$v0, 5		# $v0 = 5
    syscall
    move 	$t3, $v0		# $t3 = $v0    
    li		$v0, 4		# $v0 = 4
    la		$a0, yinput		# 
    syscall
    li		$v0, 5		# $v0 = 5
    syscall
    move 	$t4, $v0		# $t4 = $v0
    

    li		$v0, 4		# $v0 = 4
    la		$a0, printarea		# 
    syscall
    li		$v0, 1		# $v0 = 4
    lw		$a0, area		# 
    syscall
    li		$v0, 4		# $v0 = 4
    la		$a0, newline		# 
    syscall
# ======================== first point End ==================

    inputLoop:
        add		$t1, $t1, 1		# $t1 = $t1 + $t2

        # ======================== point input start ==================

        li		$v0, 4		# $v0 = 4
        la		$a0, iterator		# 
        syscall
        li		$v0, 1		# $v0 = 4
        move	$a0, $t1		# 
        syscall
        li		$v0, 4		# $v0 = 4
        la		$a0, newline		# 
        syscall
        li		$v0, 4		# $v0 = 4
        la		$a0, xinput		# 
        syscall
        li		$v0, 5		# $v0 = 5
        syscall
        move 	$t5, $v0		# $v0 = $t3
        
        
        li		$v0, 4		# $v0 = 4
        la		$a0, yinput		# 
        syscall
        li		$v0, 5 		# $v0   = 
        syscall
        move 	$t6, $v0		# $v0 = $t3

        # ======================== point input End ==================


        # (xnew-xold)*(ynew+yold)/2
        # sub		$t7, $t5, $t3		# $t7 = $t1 - $t2
        # add		$t8, $t4, $t6		# $t8 = $t4 + $t6
        # mul     $t7, $t7, $t8
        # # cvt.d.w $t9, $t7
        
        # add	$t9, $t9, $t2		# $t1 = $t1 + 1
        # ld		$t8, 	two	# $t8 = 2
        
        # div   $t9, $t9, $t8
        ##################################
        sub		$t7, $t5, $t3		# $t7 = $t1 - $t2
        add		$t8, $t4, $t6		# $t8 = $t4 + $t6
        mul     $t7, $t7, $t8
        # cvt.d.w $t9, $t7
        add   $t2, $t2, $t7
        
        # ======================== Print area to keep track ====================
        move 	$t3, $t5		# $t3 = $t5
        move 	$t4, $t6		# $t4 = $t6
        sw		$t2, area		# 

        li		$v0, 4		# $v0 = 4
        la		$a0, printarea		# 
        syscall
        li		$v0, 1		# $v0 = 4
        lw		$a0, area		# 
        syscall
        li		$v0, 4		# $v0 = 4
        la		$a0, newline		# 
        syscall
        # ======================== Print area to keep track END ====================
        blt		$t1, $t0, inputLoop	# if $t1 <= $t0 then inputLoop
            

    li		$v0, 1 		# $v0    4= 
    lw		$a0, areafinalprompt		# 
    syscall
    # assume input comes as n till here.

# -----
# Done, terminate program.
    li		$v0, 10		# $v0 = 10
    syscall # all done!
.end main
