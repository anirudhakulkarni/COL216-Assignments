#####################################################
#		Assignment1
# Entry no 1: Anirudha Kulkarni (2019CS50421)
# This program Prints the area under the curve determined by input points.
#####################################################
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
    debug: .asciiz "\nDebug here\n"
    term: .asciiz "\nProgram Terminated"
    badterm: .asciiz "\nBad number of points!\nNumber of Points must be greater than 0...\nProgram Terminated"
    points: .word    0
    area:   .double  0.0
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
    l.d		$f4, area		# $t2 = 0
    li		$t5, 1		# $t5 = 2
    blt		$t0, $t5, badterminate	# if $t0 < $t5 then terminate
    ble     $t0, $t5, terminate
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
    li		$v0, 3		# $v0 = 4
    l.d		$f12, area		# 
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

        # ======================== Area Calculations ==================

        blt		$t6, 0, negativey2	# if $t6 < 0 then negativey2
        blt		$t4, 0, negativey1	# if $t4 < 0 then negativey1
        
        # Normal code
        sub		$t7, $t5, $t3		# $t7 = x2-x1
        add		$t8, $t4, $t6		# $t8 = y2+y1
        mtc1    $t7, $f6
        cvt.d.w $f6, $f6
        mtc1    $t8, $f8
        cvt.d.w $f8, $f8
        mul.d   $f6, $f8, $f6
#        mul     $t7, $t7, $t8       # (y2+y1)*(x2-x1)

        # mtc1    $t7, $f6
        # cvt.d.w $f6, $f6
        l.d     $f8, two
        div.d   $f6, $f6, $f8       # divide by 2
        add.d   $f4, $f4, $f6       # add to answer
        j		endif				# jump to endif

        # Negative y1 but positive y2
        negativey1:
            mul     $t6, $t6, -1
            mul     $t4, $t4, -1
            # mul     $t8, $t4, $t4       # y1 squared
            # mul     $t7, $t6, $t6       # y2 squared
            # add		$t7, $t7, $t8		# $t7 = $t7 + $t8
            
            # sub		$t8, $t5, $t3		# $t8 = $t4 - $t6
            # mul     $t7, $t7, $t8

            # mtc1    $t7, $f6
            # cvt.d.w $f6, $f6            # $f6 = (x2-x1)*(y1 squared + y2 squared)

            # # move 	$t7, $t6		# $t7 = $t6
            # sub		$t7, $t4, $t6		# $t7 = y1-y2
            # abs     $t7, $t7
            # mtc1    $t7, $f8
            # cvt.d.w $f8, $f8
            # div.d   $f6, $f6, $f8
            # l.d     $f8, two
            # div.d   $f6, $f6, $f8
            # add.d   $f4, $f4, $f6

            mtc1    $t4, $f6
            cvt.d.w $f6, $f6
            mtc1    $t6, $f8
            cvt.d.w $f8, $f8
            mul.d   $f6, $f6, $f6
            mul.d   $f8, $f8, $f8
            add.d $f6, $f8, $f6
#-------------------------------------------------------------------
            # mul     $t8, $t4, $t4       # y1 squared
            # mul     $t7, $t6, $t6       # y2 squared
            # add		$t7, $t7, $t8		# $t7 = $t7 + $t8
            
            sub		$t8, $t5, $t3		# $t8 = $t4 - $t6
            mtc1    $t8, $f8
            cvt.d.w $f8, $f8
            mul.d   $f6, $f6, $f8



            # mtc1    $t7, $f6
            # cvt.d.w $f6, $f6            # $f6 = (x2-x1)*(y1 squared + y2 squared)

            # move 	$t7, $t6		# $t7 = $t6
            sub		$t7, $t4, $t6		# $t7 = y1-y2
            abs     $t7, $t7
            mtc1    $t7, $f8
            cvt.d.w $f8, $f8
            div.d   $f6, $f6, $f8
            l.d     $f8, two
            div.d   $f6, $f6, $f8
            add.d   $f4, $f4, $f6
            mul     $t6, $t6, -1
            mul     $t4, $t4, -1
            j		endif				# jump to endif
        

        negativey2: 
            blt		$t4, 0, negativeboth	# if $t4 < 0 then negativeboth
            # if y2 is negative y1 positive

            mtc1    $t4, $f6
            cvt.d.w $f6, $f6
            mtc1    $t6, $f8
            cvt.d.w $f8, $f8
            mul.d   $f6, $f6, $f6
            mul.d   $f8, $f8, $f8
            add.d $f6, $f8, $f6
#-------------------------------------------------------------------
            # mul     $t8, $t4, $t4       # y1 squared
            # mul     $t7, $t6, $t6       # y2 squared
            # add		$t7, $t7, $t8		# $t7 = $t7 + $t8
            
            sub		$t8, $t5, $t3		# $t8 = $t4 - $t6
            mtc1    $t8, $f8
            cvt.d.w $f8, $f8
            mul.d   $f6, $f6, $f8



            # mtc1    $t7, $f6
            # cvt.d.w $f6, $f6            # $f6 = (x2-x1)*(y1 squared + y2 squared)

            # move 	$t7, $t6		# $t7 = $t6
            sub		$t7, $t4, $t6		# $t7 = y1-y2
            abs     $t7, $t7
            mtc1    $t7, $f8
            cvt.d.w $f8, $f8
            div.d   $f6, $f6, $f8
            l.d     $f8, two
            div.d   $f6, $f6, $f8
            add.d   $f4, $f4, $f6
            j		endif				# jump to endif

            negativeboth:
            # if both y are negative
                li		$t8, 0 		# $t8 =0    
                
                sub		$t7, $t5, $t3		# $t7 = $t1 - $t2
                sub		$t8, $t8, $t4 	# $t8 = $t4 + $t6
                sub		$t8, $t8, $t6 	# $t8 = $t4 + $t6
                
                mtc1    $t7, $f6
                cvt.d.w $f6, $f6
                mtc1    $t8, $f8
                cvt.d.w $f8, $f8
                mul.d   $f6, $f8, $f6
                l.d     $f8, two
                # mul     $t7, $t7, $t8
                
                
                # mtc1    $t7, $f6
                # cvt.d.w $f6, $f6
                # l.d     $f8, two
                div.d   $f6, $f6, $f8
                add.d   $f4, $f4, $f6
                j		endif				# jump to endif
                
            


        # ======================== Print area to keep track ====================
        endif:
            # Loop propogation and printing all the details

            move 	$t3, $t5		# $t3 = $t5
            move 	$t4, $t6		# $t4 = $t6
            s.d		$f4, area		# 

            li		$v0, 4		# $v0 = 4
            la		$a0, printarea		# 
            syscall
            li		$v0, 3		# $v0 = 4
            l.d		$f12, area		# 
            syscall
            li		$v0, 4		# $v0 = 4
            la		$a0, newline		# 
            syscall
            # ======================== Print area to keep track END ====================
            blt		$t1, $t0, inputLoop	# if $t1 <= $t0 then inputLoop
            



# -----
# Done, terminate program.
    terminate:
        li		$v0, 4 		# $v0    4= 
        la		$a0, areafinalprompt		# 
        syscall
        li		$v0, 3 		# $v0    4= 
        l.d		$f12, area		# 
        syscall
        # assume input comes as n till here.
        li		$v0, 4 		# $v0    4= 
        la		$a0, term		# 
        syscall
        li		$v0, 10		# $v0 = 10
        syscall # all done!
    
    badterminate:

        # assume input comes as n till here.
        li		$v0, 4 		# $v0    4= 
        la		$a0, badterm		# 
        syscall
        li		$v0, 10		# $v0 = 10
        syscall # all done!
.end main