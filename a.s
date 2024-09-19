.section .text
.global _start
_start:
  call main
  movl %eax, %ebx
  movl $1, %eax
  int $0x80

.global main
main:
  /*id argc*/
  /*id argv*/

  movl $3, %eax
  ret

