/* -*- asm -*- */
        .text
        .align  4
        .global readi32
readi32:
        stmfd   sp!, { fp, lr }
        sub     sp, sp, #8
        adr     r0, msg2
        mov     r1, sp
        bl      scanf
        mov     r1, #0
        cmp     r0, #1
        movne   r1, #1
        movs    r1, r1, asr #1
        ldr     r0, [sp]
        add     sp, sp, #8
        ldmfd   sp!, { fp, pc }
msg2:   .asciz  "%d"

        .text
        .align  4
        .global writei32
writei32:
        stmfd   sp!, { fp, lr }
        mov     r1, r0
        adr     r0, msg1
        bl      printf
        ldmfd   sp!, { fp, pc }
msg1:   .asciz  "%d"

        .text
        .align  4
        .global nl
nl:
        stmfd   sp!, { fp, lr }
        mov     r0, #'\n'
        bl      putchar
        ldmfd   sp!, { fp, pc }

        .text
        .align  4
        .global finish
finish:
        mov     r0, #0
        bl      exit

        .text
        .align  4
        .global readi64
readi64:
        stmfd   sp!, { fp, lr }
        sub     sp, sp, #8
        adr     r0, msg3
        mov     r1, sp
        bl      scanf
        mov     r1, #0
        cmp     r0, #1
        movne   r1, #1
        movs    r1, r1, asr #1
        ldm     sp, { r0, r1 }
        add     sp, sp, #8
        ldmfd   sp!, { fp, pc }
msg3:   .asciz  "%lld"

        .text
        .align  4
        .global writei64
writei64:
        stmfd   sp!, { fp, lr }
        mov     r2, r0
        mov     r3, r1
        adr     r0, msg4
        bl      printf
        ldmfd   sp!, { fp, pc }
msg4:   .asciz  "%lld"
