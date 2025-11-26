.data
print_fmt: .string "%ld \n"
string_fmt: .string "%s \n"
.text
.globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 subq $168, %rsp
 movq $1, %rax
 movq %rax, -8(%rbp)  # Elemento plano [0]
 movq $2, %rax
 movq %rax, -16(%rbp)  # Elemento plano [1]
 movq $3, %rax
 movq %rax, -24(%rbp)  # Elemento plano [2]
 movq $6, %rax
 movq %rax, -32(%rbp)  # Elemento plano [3]
 movq %rax, -40(%rbp)  # Elemento plano [4]
 movq $4, %rax
 movq %rax, -48(%rbp)  # Elemento plano [5]
 movq $5, %rax
 movq %rax, -56(%rbp)  # Elemento plano [6]
 movq $6, %rax
 movq %rax, -64(%rbp)  # Elemento plano [7]
 movq $97, %rax
 movq %rax, -72(%rbp)  # Elemento plano [8]
 movq %rax, -80(%rbp)  # Elemento plano [9]
 movq $6, %rax
 movq %rax, -88(%rbp)  # Elemento plano [10]
 movq $7, %rax
 movq %rax, -96(%rbp)  # Elemento plano [11]
 movq $8, %rax
 movq %rax, -104(%rbp)  # Elemento plano [12]
 movq $556, %rax
 movq %rax, -112(%rbp)  # Elemento plano [13]
 movq %rax, -120(%rbp)  # Elemento plano [14]
 movq $6, %rax
 movq %rax, -128(%rbp)  # Elemento plano [15]
 movq $7, %rax
 movq %rax, -136(%rbp)  # Elemento plano [16]
 movq $8, %rax
 movq %rax, -144(%rbp)  # Elemento plano [17]
 movq $68, %rax
 movq %rax, -152(%rbp)  # Elemento plano [18]
 movq %rax, -160(%rbp)  # Elemento plano [19]
 movq $1, %rax
 movq %rax, -144(%rbp)  # Elemento plano [0]
 movq $2, %rax
 movq %rax, -152(%rbp)  # Elemento plano [1]
 movq $998888, %rax
 movq %rax, -160(%rbp)  # Elemento plano [2]
 movq $6, %rax
 movq %rax, -168(%rbp)  # Elemento plano [3]
 # Calculando dirección para asignación en array
 movq $3, %rax
 pushq %rax
 movq $1, %rax
 movq %rax, %rcx
 popq %rax
 imulq $4, %rax
 addq %rcx, %rax
 imulq $-8, %rax
 addq $-8, %rax
 pushq %rax  # Guardamos la dirección de destino
 movq $0, %rax
 popq %rcx   # Recuperamos la dirección de destino
 movq %rax, (%rbp, %rcx) # Asignación final
 # Acceso unificado a array
 movq $3, %rax
 pushq %rax
 movq $1, %rax
 movq %rax, %rcx
 popq %rax
 imulq $4, %rax
 addq %rcx, %rax
 imulq $-8, %rax
 addq $-8, %rax
 movq (%rbp,%rax), %rax
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movl $0, %eax
 call printf@PLT
 # Acceso unificado a hola
 movq $2, %rax
 imulq $-8, %rax
 addq $-144, %rax
 movq (%rbp,%rax), %rax
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movl $0, %eax
 call printf@PLT
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
