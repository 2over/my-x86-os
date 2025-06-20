[BITS 32]
[SECTION .text]

; fork如果用C实现，就会因为栈问题导致出错
global fork
fork:
    mov eax, 2
    int 0x80

    ret