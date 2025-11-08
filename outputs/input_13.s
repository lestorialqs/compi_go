.data
print_fmt: .string "%ld \n"
.text
.globl fac
fac:
 pushq %rbp
 movq %rsp, %rbp
 movq %rdi,-8(%rbp)
 subq $8, %rsp
 movq -8(%rbp), %rax
 pushq %rax
 movq $2, %rax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movl $0, %eax
 setle %al
 movzbq %al, %rax
 cmpq $0, %rax
 je else_0
 movq -8(%rbp), %rax
 jmp .end_fac
 jmp endif_0
 else_0:
 movq -8(%rbp), %rax
 pushq %rax
 movq $1, %rax
 movq %rax, %rcx
 popq %rax
 subq %rcx, %rax
 mov %rax, %rdi
call fac
 pushq %rax
 movq -8(%rbp), %rax
 movq %rax, %rcx
 popq %rax
 imulq %rcx, %rax
 jmp .end_fac
endif_0:
.end_fac:
leave
ret
.globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 subq $8, %rsp
 movq $1, %rax
 movq %rax, -8(%rbp)
while_1:
 movq -8(%rbp), %rax
 pushq %rax
 movq $20, %rax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movl $0, %eax
 setle %al
 movzbq %al, %rax
 cmpq $0, %rax
 je endwhile_1
 movq -8(%rbp), %rax
 mov %rax, %rdi
call fac
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movl $0, %eax
 call printf@PLT
 movq -8(%rbp), %rax
 pushq %rax
 movq $1, %rax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 movq %rax, -8(%rbp)
 jmp while_1
endwhile_1:
 movq $0, %rax
 jmp .end_main
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
