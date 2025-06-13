[ORG 0x500]

[SECTION .data]
KERNEL_ADDR equ 0x1200

[SECTION .gdt]
SEG_BASE equ 0
SEG_LIMIT equ 0xfffff

CODE_SELECTOR equ (1 << 3)
DATA_SELECTOR equ (2 << 3)

gdt_base:
    dd 0, 0

;gdt_code:
;    dw SEG_LIMIT & 0xffff
;    dw SEG_BASE & 0xffff
;    db SEG_BASE >> 16 & 0xff
;    ;    P_DPL_S_TYPE
;    db 0b1_00_1_1000
;    ;    G_DB_AVL_LIMIT
;    db 0b0_1_00_0000 | (SEG_LIMIT >> 16 & 0xf)
;    db SEG_BASE >> 24 & 0xf
gdt_code:
    dw SEG_LIMIT & 0xffff
    dw SEG_BASE & 0xffff
    db SEG_BASE >> 16 & 0xff
    ;   P_DPL_S_TYPE
    db 0b1_00_1_1000
    ;   G_DB_AVL_LIMIT
    db 0b0_1_00_0000 | (SEG_LIMIT >> 16 & 0xf)
    db SEG_BASE >> 24 & 0xf

gdt_data:
    dw SEG_LIMIT & 0xffff
    dw SEG_BASE & 0xffff
    db SEG_BASE >> 16 & 0xff

    ;   P_DPL_S_TYPE
    db 0b1_00_1_0010
    ;   G_DB_AVL_LIMIT
    db 0b1_1_00_0000 | (SEG_LIMIT >> 16 & 0xf)
    db SEG_BASE >> 24 & 0xf

gdt_ptr:
    dw $ - gdt_base - 1
    dd gdt_base

[SECTION .text]
[BITS 16]
global setup_start
setup_start:
    mov     ax, 0
    mov     ss, ax
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     si, ax
    mov     si, prepare_enter_protected_mode_msg
    call    print

enter_protected_mode:
    ; 关终端
    cli

    ; 加载gdt表
    lgdt [gdt_ptr]

    ; 开A20
    in  al, 92h
    or  al, 00000010b
    out 92h, al

    ; 设置保护模式
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp CODE_SELECTOR:protected_mode

; 如何调用
; mov   si, msg     ; 1 传入字符串
; call  print       ; 2 调用
print:
    mov ah, 0x0e
    mov bh, 0
    mov bl, 0x01

.loop:
    mov al, [si]
    cmp al, 0
    jz .done
    int 0x10

    inc si
    jmp .loop

.done:
    ret

[BITS 32]
protected_mode:
    mov ax, DATA_SELECTOR
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov esp, 0x9fbff

    ; 测试是否成功进入保护模式，如果没有，写入内存是失败的
    xchg bx, bx
    mov byte [0x100000], 0xaa

    jmp $

prepare_enter_protected_mode_msg:
    db "Prepare to go into protected mode ...", 10, 13, 0
