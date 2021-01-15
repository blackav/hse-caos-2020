        .text
        .align  16
        .global readi32
readi32:
        push    %rbp
        mov     %rsp, %rbp

        sub     $24, %rsp
        xor     %eax, %eax
        lea     .L2(%rip), %rdi
        mov     %rsp, %rsi
        call    scanf
        mov     %eax, %ecx
        mov     (%rsp), %eax
        clc
        cmp     $1, %ecx
        je      .L1
        stc
.L1:
        lea     24(%rsp), %rsp
        pop     %rbp
        ret
        .section    .string, "aMS", @progbits, 1
.L2:    .asciz  "%d"

        .text
        .align  16
        .global writei32
writei32:
        push    %rbp
        mov     %rsp, %rbp
        mov     %edi, %esi
        xor     %eax, %eax
        lea     .L3(%rip), %rdi
        call    printf
        pop     %rbp
        ret
        .section    .string, "aMS", @progbits, 1
.L3:    .asciz  "%d"

        .text
        .align  16
        .global finish
finish:
        xor     %edi, %edi
        call    exit

        .text
        .align  16
        .global nl
nl:
        push    %rbp
        mov     %rsp, %rbp
        mov     $'\n', %edi
        call    putchar
        pop     %rbp
        ret

        .text
        .align  16
        .global readi64
readi64:
        push    %rbp
        mov     %rsp, %rbp

        sub     $24, %rsp
        xor     %eax, %eax
        lea     .L4(%rip), %rdi
        mov     %rsp, %rsi
        call    scanf
        mov     %eax, %ecx
        mov     (%rsp), %rax
        clc
        cmp     $1, %ecx
        je      .L5
        stc
.L5:
        lea     24(%rsp), %rsp
        pop     %rbp
        ret
        .section    .string, "aMS", @progbits, 1
.L4:    .asciz  "%lld"

        .text
        .align  16
        .global writei64
writei64:
        push    %rbp
        mov     %rsp, %rbp
        mov     %rdi, %rsi
        xor     %eax, %eax
        lea     .L6(%rip), %rdi
        call    printf
        pop     %rbp
        ret
        .section        .string, "aMS", @progbits, 1
.L6:    .asciz  "%lld"
