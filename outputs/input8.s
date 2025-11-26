.data
print_fmt: .string "%ld \n"
string_fmt: .string "%s \n"
.text
.globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 subq $72, %rsp
 movq $1, %rax
 movq %rax, -8(%rbp)  # Elemento plano [0]
 movq $2, %rax
 movq %rax, -16(%rbp)  # Elemento plano [1]
 movq $3, %rax
 movq %rax, -24(%rbp)  # Elemento plano [2]
 movq %rax, -32(%rbp)  # Elemento plano [3]
 movq $4, %rax
 movq %rax, -40(%rbp)  # Elemento plano [4]
 movq $5, %rax
 movq %rax, -48(%rbp)  # Elemento plano [5]
 movq $6, %rax
 movq %rax, -56(%rbp)  # Elemento plano [6]
 movq %rax, -64(%rbp)  # Elemento plano [7]
 movq $7, %rax
 movq %rax, -72(%rbp)  # Elemento plano [8]
 movq $8, %rax
 movq %rax, -80(%rbp)  # Elemento plano [9]
 movq $9, %rax
 movq %rax, -88(%rbp)  # Elemento plano [10]
 movq %rax, -96(%rbp)  # Elemento plano [11]
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
