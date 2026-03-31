#!/bin/zsh

if [[ "$1" == "KERNEL_CXX" ]]; then
    find "$(realpath src/cpp/kernel)" -type f | paste -sd' ' - | tr -d '\n'
fi

if [[ "$1" == "KERNEL_ASM" ]]; then
    find "$(realpath src/asm/kernel)" -type f | paste -sd' ' - | tr -d '\n'
fi

if [[ "$1" == "COMMON" ]]; then
    find "$(realpath src/cpp/common)" -type f | paste -sd' ' - | tr -d '\n'
fi

if [[ "$1" == "BOOTLOADER_CXX" ]]; then
    find "$(realpath src/cpp/boot)" -type f | paste -sd' ' - | tr -d '\n'
fi

if [[ "$1" == "BOOTLOADER_ASM" ]]; then
    find "$(realpath src/asm/boot)" -type f | paste -sd' ' - | tr -d '\n'
fi
