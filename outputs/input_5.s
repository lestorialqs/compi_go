.data
print_fmt: .string "%ld \n"
.text
.globl id
id:
 pushq %rbp
 movq %rsp, %rbp
 movq %rdi,-8(%rbp)
 subq $8, %rsp
 movq -8(%rbp), %rax
 jmp .end_id
.end_id:
leave
ret
.globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 subq $0, %rsp
 movq $42, %rax
 mov %rax, %rdi
call id
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movl $0, %eax
 call printf@PLT
 movq $0, %rax
 jmp .end_main
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
