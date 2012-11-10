.section  .data
score:
.int  76,82,90,86,79,62
credit:
.int  2,2,1,2,2,3
string0:
.asciz "please input your student number:"
string1:
.asciz "%d"
string2:
.asciz "the score of student number %d is %f higher than 60.\n"
string3:
.asciz "the score of student number %d is %f lower than 60.\n"

.section .bss
.comm stu_number,4
.comm mean,4
.comm sum,4
.comm temp,4
.comm i,4
.comm itof, 4

.section .text
.globl main
main:
	finit
	pushl  $string0
	call printf
	addl $4, %esp
	pushl $stu_number
	pushl $string1

	call scanf 
	addl $8,%esp
	pushl $0
	popl  %eax
	movl %eax,  sum
	pushl $0
	popl  %eax
	movl %eax,  temp
	pushl $0
	popl  %eax
	movl %eax,  i
.code_seg0:
	pushl  i
	pushl $6
	popl %eax
	popl %ebx
	cmpl %eax, %ebx
	jge .code_seg1
	flds  sum
	pushl  i
	popl %eax
	movl score(, %eax, 4) ,%ebx
	pushl %ebx
	pushl  i
	popl %eax
	movl credit(, %eax, 4) ,%ebx
	pushl %ebx
	popl %eax
	popl %ebx
	mull %ebx
	pushl %eax
	popl %eax
	movl %eax, itof
	fiadd itof
	fstps sum
	pushl  temp
	pushl  i
	popl %eax
	movl credit(, %eax, 4) ,%ebx
	pushl %ebx
	popl %eax
	popl %ebx
	addl %eax, %ebx
	pushl %ebx
	popl  %eax
	movl %eax,  temp
	incl i
	jmp .code_seg0
.code_seg1:
	flds  sum
	pushl  temp
	popl %eax
	movl %eax, itof
	fidiv itof
	fstps mean
.code_seg2:
	flds  mean
	pushl $60
	popl %eax
	movl %eax, itof
	ficomp itof
	fstsw
	sahf
	jl .code_seg3
	flds  mean
	pushl $60
	popl %eax
	movl %eax, itof
	fisub itof
	fstps mean
	flds mean
	fstpl (%esp)
	pushl stu_number
	pushl  $string2
	call   printf  
	addl $12, %esp
	jmp  .code_seg4
.code_seg3:
	pushl $60
	flds  mean
	popl %eax
	movl %eax, itof
	fisubr itof
	fstps mean
	flds mean
	fstpl (%esp)
	pushl stu_number
	pushl  $string3
	call   printf  
	addl $12, %esp
.code_seg4:
	pushl $0
	call exit

