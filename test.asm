# Example program to display an array.
# Demonstrates use of QtSpim system service calls.
# -----------------------------------------------------
# Data Declarations
.data
hdr: .asciiz "Squaring Example\n"
hdr2: .asciiz "Fuck you\n"
# -----------------------------------------------------
# text/code section
.text
.globl main
.ent main
main:
    li		$v0, 5			# 
    syscall
    move 	$t0, $v0		# $t0 = $v0
    
    blt		$t0, 0, test	# if $t0 < 0 then test
    li		$v0, 4		# $v0 = 1
    la		$a0, hdr2		# 
    syscall

    test: 
        li		$v0, 4# $v0 = 1
        la		$a0, hdr		# 
        syscall
    # Done, terminate program.
        li $v0, 10 # terminate
        syscall # system call
.end main
