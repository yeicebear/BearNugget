namespace bear {
    void kernel_main() {
        asm volatile ("jmp $");
    }
}

extern "C" void kmain() {
    bear::kernel_main();
}
