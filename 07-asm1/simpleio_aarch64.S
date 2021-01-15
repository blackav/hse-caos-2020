/* -*- asm -*- */
        .text
        .align  4
        .global readi32
readi32:
        stp     fp, lr, [sp, -16]!
        mov     fp, sp
        sub     sp, sp, #16
        adr     x0, msg2
        mov     x1, sp
        bl      scanf
        cmp     w0, #1
        cset    w0, ge
        subs    wzr, wzr, w0    // this set CF in case of EOF
        ldr     w0, [sp]
        mov     sp, fp
        ldp     fp, lr, [sp], 16
        ret
        .section    .string, "aMS", @progbits, 1
msg2:   .asciz  "%d"

        .text
        .align  4
        .global writei32
writei32:
        stp     fp, lr, [sp, -16]!
        mov     w1, w0
        adr     x0, msg1
        bl      printf
        ldp     fp, lr, [sp], 16
        ret
        .section    .string, "aMS", @progbits, 1
msg1:   .asciz  "%d"

        .text
        .align  4
        .global nl
nl:
        stp     fp, lr, [sp, -16]!
        mov     w0, #'\n'
        bl      putchar
        ldp     fp, lr, [sp], 16
        ret

        .text
        .align  4
        .global finish
finish:
        mov     w0, #0
        b       exit

        .text
        .align  4
        .global readi64
readi64:
        stp     fp, lr, [sp, -16]!
        mov     fp, sp
        sub     sp, sp, #16
        adr     x0, msg3
        mov     x1, sp
        bl      scanf
        cmp     w0, #1
        cset    w0, ge
        subs    wzr, wzr, w0    // this set CF in case of EOF
        ldr     x0, [sp]
        mov     sp, fp
        ldp     fp, lr, [sp], 16
        ret
        .section    .string, "aMS", @progbits, 1
msg3:   .asciz  "%lld"

        .text
        .align  4
        .global writei64
writei64:
        stp     fp, lr, [sp, -16]!
        mov     x1, x0
        adr     x0, msg4
        bl      printf
        ldp     fp, lr, [sp], 16
        ret
        .section    .string, "aMS", @progbits, 1
msg4:   .asciz  "%lld"

