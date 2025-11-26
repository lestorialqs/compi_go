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
 movq %rax, -8(%rbp)  # Valor [0]
 movq $2, %rax
 movq %rax, -16(%rbp)  # Valor [1]
 movq $3, %rax
 movq %rax, -24(%rbp)  # Valor [2]
 movq %rax, -32(%rbp)  # Valor [3]
 movq $4, %rax
 movq %rax, -40(%rbp)  # Valor [4]
 movq $5, %rax
 movq %rax, -48(%rbp)  # Valor [5]
 movq $6, %rax
 movq %rax, -56(%rbp)  # Valor [6]
 movq %rax, -64(%rbp)  # Valor [7]
 movq $7, %rax
 movq %rax, -72(%rbp)  # Valor [8]
 movq $8, %rax
 movq %rax, -80(%rbp)  # Valor [9]
 movq $9, %rax
 movq %rax, -88(%rbp)  # Valor [10]
 movq %rax, -96(%rbp)  # Valor [11]
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
