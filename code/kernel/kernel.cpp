#include "../include/vga.hpp"
#include "../include/fs.hpp"
#include "../include/shell.hpp"

extern "C" void kmain() {
    vga_clear();
    vga_println("BearNugget 0.1");
    vga_println("type 'help' for commands");
    vga_putchar('\n');
    fs_init();
    shell_run();
}
