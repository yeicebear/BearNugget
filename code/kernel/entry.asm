[BITS 32]
global _start
extern kmain

section .text
_start:
    call kmain
    hlt
