@Blake Lufkin
@Modified Code
@9/23/2021

@ Syscall defines
.equ SYSCALL_EXIT,     1


        .globl _start
_start:

        @================================
        @ Exit
        @================================
exit:
	mov	r0, #42			@		;moves 42 into r0
	mov	r7, #SYSCALL_EXIT	@	;moves Syscall_exit into r7
	swi	0			@			;software interrupt to trigger system call
