        .text
        .align  16

        // gthr_switch(from, to)
        .global gthr_switch
gthr_switch:
        sub     $12, %esp
        mov     16(%esp), %eax
        mov     20(%esp), %edx
        // eax - from, edx - to

        mov     %ebx, (%eax)
        mov     %esi, 4(%eax)
        mov     %edi, 8(%eax)
        mov     %ebp, 12(%eax)
        mov     %esp, 16(%eax)

        mov     16(%edx), %esp
        mov     12(%edx), %ebp
        mov     8(%edx), %edi
        mov     4(%edx), %esi
        mov     (%edx), %ebx

        add     $12, %esp
        ret

        .align  16
        .global gthr_entry
gthr_entry:
        sub     $12, %esp
        mov     16(%esp), %eax
        mov     20(%esp), %edx
        // eax - from, edx - to

        mov     %ebx, (%eax)
        mov     %esi, 4(%eax)
        mov     %edi, 8(%eax)
        mov     %ebp, 12(%eax)
        mov     %esp, 16(%eax)

        mov     16(%edx), %esp
        mov     12(%edx), %ebp
        mov     8(%edx), %edi
        mov     4(%edx), %esi
        mov     (%edx), %ebx
        
        jmp     gthr_trampoline
