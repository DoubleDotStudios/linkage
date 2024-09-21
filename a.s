.section .text
.global _start
_start:
pushl 0(%esp)
pushl 4(%esp)
call main
addl $4, %esp
movl %eax, %ebx
movl $1, %eax
int $0x80

.global main
main:

pushl %ebp
movl %esp, %ebp
movl 4(%esp), %eax
movl 4(%eax), %eax
movl %ebp, %esp
popl %ebp
ret

