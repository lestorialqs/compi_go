.data
print_fmt: .string "%ld \n"
string_fmt: .string "%s \n"
.L.str_3: .string "Diana"
.L.str_2: .string "Charlie"
.L.str_1: .string "Bob"
.L.str_0: .string "Alice"
.text
.globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 subq $120, %rsp
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
 leaq .L.str_0(%rip), %rax
 movq %rax, -80(%rbp)  # String ptr [0]
 leaq .L.str_1(%rip), %rax
 movq %rax, -88(%rbp)  # String ptr [1]
 movq %rax, -96(%rbp)  # Valor [2]
 leaq .L.str_2(%rip), %rax
 movq %rax, -104(%rbp)  # String ptr [3]
 leaq .L.str_3(%rip), %rax
 movq %rax, -112(%rbp)  # String ptr [4]
 movq %rax, -120(%rbp)  # Valor [5]
 movq $1, %rax
 movq %rax, -112(%rbp)  # Valor [0]
 movq $0, %rax
 movq %rax, -120(%rbp)  # Valor [1]
 # Acceso unificado a numeros
 movq $1, %rax
 pushq %rax
 movq $1, %rax
 movq %rax, %rcx
 popq %rax
 imulq $3, %rax
 addq %rcx, %rax
 imulq $-8, %rax
 addq $-8, %rax
 movq (%rbp,%rax), %rax
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movl $0, %eax
 call printf@PLT
 # Acceso unificado a nombres
 movq $0, %rax
 pushq %rax
 movq $1, %rax
 movq %rax, %rcx
 popq %rax
 imulq $2, %rax
 addq %rcx, %rax
 imulq $-8, %rax
 addq $-80, %rax
 movq (%rbp,%rax), %rax
 movq %rax, %rsi
 leaq string_fmt(%rip), %rdi
 movl $0, %eax
 call printf@PLT
 # Acceso unificado a flags
 movq $0, %rax
 imulq $-8, %rax
 addq $-112, %rax
 movq (%rbp,%rax), %rax
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movl $0, %eax
 call printf@PLT
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
