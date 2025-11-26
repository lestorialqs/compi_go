.data
print_fmt: .string "%ld \n"
string_fmt: .string "%s \n"
.text
.globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 subq $32, %rsp
 # Calculando dirección para asignación en grades
 movq $0, %rax
 imulq $-8, %rax
 addq $-8, %rax
 pushq %rax  # Guardamos la dirección de destino
 movq $10, %rax
 popq %rcx   # Recuperamos la dirección de destino
 movq %rax, (%rbp, %rcx) # Asignación final
 # Calculando dirección para asignación en grades
 movq $1, %rax
 imulq $-8, %rax
 addq $-8, %rax
 pushq %rax  # Guardamos la dirección de destino
 movq $3, %rax
 popq %rcx   # Recuperamos la dirección de destino
 movq %rax, (%rbp, %rcx) # Asignación final
 # Calculando dirección para asignación en grades
 movq $2, %rax
 imulq $-8, %rax
 addq $-8, %rax
 pushq %rax  # Guardamos la dirección de destino
 movq $13, %rax
 popq %rcx   # Recuperamos la dirección de destino
 movq %rax, (%rbp, %rcx) # Asignación final
 # Calculando dirección para asignación en grades
 movq $3, %rax
 imulq $-8, %rax
 addq $-8, %rax
 pushq %rax  # Guardamos la dirección de destino
 movq $11, %rax
 popq %rcx   # Recuperamos la dirección de destino
 movq %rax, (%rbp, %rcx) # Asignación final
 # Calculando dirección para asignación en grades
 movq $1, %rax
 imulq $-8, %rax
 addq $-8, %rax
 pushq %rax  # Guardamos la dirección de destino
 movq $5, %rax
 popq %rcx   # Recuperamos la dirección de destino
 movq %rax, (%rbp, %rcx) # Asignación final
 # Acceso unificado a grades
 movq $0, %rax
 imulq $-8, %rax
 addq $-8, %rax
 movq (%rbp,%rax), %rax
 pushq %rax
 # Acceso unificado a grades
 movq $1, %rax
 imulq $-8, %rax
 addq $-8, %rax
 movq (%rbp,%rax), %rax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 pushq %rax
 # Acceso unificado a grades
 movq $2, %rax
 imulq $-8, %rax
 addq $-8, %rax
 movq (%rbp,%rax), %rax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 pushq %rax
 # Acceso unificado a grades
 movq $3, %rax
 imulq $-8, %rax
 addq $-8, %rax
 movq (%rbp,%rax), %rax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 pushq %rax
 movq $4, %rax
 movq %rax, %rcx
 popq %rax
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movl $0, %eax
 call printf@PLT
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
