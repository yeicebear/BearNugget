#!/bin/zsh

gen_objs() {
    find "$1" -type f \
    | sed 's|^.*/src/||; s|/|_|g; s|\.|_|g; s|^|.obj/|; s|$|.o|' \
    | paste -sd' ' - | tr -d '\n'
}

case "$1" in
    KERNEL_CXX)
        gen_objs src/cpp/kernel
        ;;
    KERNEL_ASM)
        gen_objs src/asm/kernel
        ;;
    COMMON)
        gen_objs src/cpp/common
        ;;
    BOOTLOADER_CXX)
        gen_objs src/cpp/boot
        ;;
    BOOTLOADER_ASM)
        gen_objs src/asm/boot
        ;;
esac

