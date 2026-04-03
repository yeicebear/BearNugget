#include "../include/shell.hpp"
#include "../include/vga.hpp"
#include "../drivers/kb.hpp"
#include "../include/str.hpp"
#include "../include/fs.hpp"

/*
this module implements the interactive shell
it reads commands from the user and executes them
it parses command line arguments separately
it supports help clear echo ls touch cat write rm uname
exec dispatches to specific command handlers
arg extracts command line tokens by index
readline gets input and handles backspace
it builds a command history in a buffer
it calls filesystem and display functions
it loops forever reading and executing commands
the prompt shows bear shell is ready
each command returns after execution
the main entry point is shell_run
*/

static char line[256];
static int  len = 0;

static void readline() {
    len = 0;
    char c;
    while (true) {
        c = kb_getchar();
        if (!c) continue;
        if (c == '\n') { line[len] = 0; vga_putchar('\n'); return; }
        if (c == '\b' && len > 0) { len--; vga_putchar('\b'); continue; }
        if (len < 255) { line[len++] = c; vga_putchar(c); }
    }
}

static const char* arg(int n) {
    static char buf[128];
    int i = 0, word = 0;
    while (line[i]) {
        while (line[i] == ' ') i++;
        if (!line[i]) break;
        int start = i;
        while (line[i] && line[i] != ' ') i++;
        if (word == n) {
            int len = i - start;
            for (int j = 0; j < len; j++) buf[j] = line[start+j];
            buf[len] = 0;
            return buf;
        }
        word++;
    }
    return "";
}

static void exec() {
    const char* cmd = arg(0);

    if (k_strcmp(cmd, "") == 0) return;

    if (k_strcmp(cmd, "help") == 0) {
        vga_println("help  clear  echo  ls  touch  cat  write  rm  uname");
        return;
    }
    if (k_strcmp(cmd, "clear") == 0) { vga_clear(); return; }
    if (k_strcmp(cmd, "uname") == 0) { vga_println("BearNugget 0.1"); return; }

    if (k_strcmp(cmd, "echo") == 0) {
        vga_println(arg(1));
        return;
    }
    if (k_strcmp(cmd, "ls") == 0)   { fs_list(); return; }
    if (k_strcmp(cmd, "touch") == 0) {
        if (fs_create(arg(1)) < 0) vga_println("touch: failed");
        return;
    }
    if (k_strcmp(cmd, "rm") == 0) {
        if (fs_delete(arg(1)) < 0) vga_println("rm: not found");
        return;
    }
    if (k_strcmp(cmd, "cat") == 0) {
        char buf[512];
        if (fs_read(arg(1), buf, 512) < 0) vga_println("cat: not found");
        else vga_println(buf);
        return;
    }
    if (k_strcmp(cmd, "write") == 0) {
        if (fs_write(arg(1), arg(2)) < 0) vga_println("write: not found");
        return;
    }

    vga_print(cmd);
    vga_println(": command not found");
}

void shell_run() {
    while (true) {
        vga_print("bear> ");
        readline();
        exec();
    }
}
