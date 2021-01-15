/* -*- asm -*- */

        .text
_form1: .asciz  "%d"
        .text
        .align  16
	.global	readi32
readi32:
        pushl   %ebp
        movl    %esp, %ebp

        subl    $4, %esp
        and     $-16, %esp
        leal    -4(%ebp), %eax

        sub     $16, %esp
        mov     %eax, 4(%esp)
        movl    $_form1, (%esp)
#ifdef __APPLE__
        call    _scanf
#else
        call    scanf
#endif
        movl    %eax, %ecx
        movl    -4(%ebp), %eax
        clc
        cmpl    $1, %ecx
        je      readi32x
        stc
readi32x:       
        movl    %ebp, %esp
        popl    %ebp
        ret

        .text
_form2: .asciz  "%d"
        .text
        .align  16
    	.global	writei32
writei32:       
        pushl   %ebp
        movl    %esp, %ebp
        and     $-16, %esp

        sub     $16, %esp
        mov     %eax, 4(%esp)
        movl    $_form2, (%esp)
#ifdef __APPLE__
        call    _printf
#else
        call    printf
#endif

        movl    %ebp, %esp
        popl    %ebp
        ret

        .align  16
        .global	nl
nl:
        pushl   %ebp
        movl    %esp, %ebp
        and     $-16, %esp
        sub     $16, %esp
        movl    $'\n', (%esp)
#ifdef __APPLE__
        call    _putchar
#else
        call    putchar
#endif
        movl    %ebp, %esp
        popl    %ebp
        ret

        .text
        .align  16
        .global finish
finish:
        and     $-16, %esp
        sub     $16, %esp
        movl    $0, (%esp)
#ifdef __APPLE__
        call    _exit
#else
        call    exit
#endif

        .text
_form3: .asciz  "%lld"
        .align  16
        .global	readi64
readi64:
        pushl   %ebp
        movl    %esp, %ebp
        sub     $8, %esp
        and     $-16, %esp

        leal    -8(%ebp), %eax
        sub     $16, %esp
        mov     %eax, 4(%esp)
        movl    $_form3, (%esp)
#ifdef __APPLE__
        call    _scanf
#else
        call    scanf
#endif
        movl    %eax, %ecx
        movl    -8(%ebp), %eax
        movl    -4(%ebp), %edx
        clc
        cmpl    $1, %ecx
        je      readi64x
        stc
readi64x:       
        movl    %ebp, %esp
        popl    %ebp
        ret

        .text
_form4: .asciz  "%lld"
        .align  16
        .global	writei64
writei64:       
        pushl   %ebp
        movl    %esp, %ebp
        and     $-16, %esp

        sub     $16, %esp
        mov     %edx, 8(%esp)
        mov     %eax, 4(%esp)
        movl    $_form4, (%esp)
#ifdef __APPLE__
        call    _printf
#else
        call    printf
#endif

        movl    %ebp, %esp
        popl    %ebp
        ret
