.data
print_fmt: .string "%ld \n"
string_fmt: .string "%s \n"
.text
.globl op
op:
 pushq %rbp
 movq %rsp, %rbp
 subq $24, %rsp
 movq %rdi,-8(%rbp)
 movq %rsi,-16(%rbp)
 movq %rdx,-24(%rbp)
 movq -8(%rbp), %rax
 pushq %rax
 movq -16(%rbp), %rax
 pushq %rax
 movq -24(%rbp), %rax
 movq %rax, %rcx
 popq %rax
 imulq %rcx, %rax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 jmp .end_op
.end_op:
leave
ret
.globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 subq $0, %rsp
 movq $5, %rax
 movq $4, %rax
 mov %rax, %rdi
 mov %rax, %rsi
 movq $2, %rax
 mov %rax, %rdx
call op
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movl $0, %eax
 call printf@PLT
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
