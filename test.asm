# Example program to display an array.
# Demonstrates use of QtSpim system service calls.
# -----------------------------------------------------
# Data Declarations
.data
hdr: .asciiz "Squaring Example\n"
test: .double 0.0
# -----------------------------------------------------
# text/code section
.text
.globl main
.ent main
main:
    li		$v0, 4		# $v0 = 4
    la		$a0, hdr		# 
    syscall

    li		$v0, 3		# $v0 = 4
    l.d		$f12, test		# 
    syscall
    # Done, terminate program.
    li $v0, 10 # terminate
    syscall # system call
.end main
