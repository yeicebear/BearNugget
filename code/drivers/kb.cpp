#include "kb.hpp"
#include <stdint.h>

/*
keyboard driver implementation
reads hardware scancodes from port 0x60
translates scancodes to ascii characters
maintains last scancode to filter repeats
ignores key release events by checking bit 7
returns zero if no key is currently pressed
returns zero for repeated key events
uses inline assembly to read hardware ports
the inb function does a single byte input
scancodes table maps scancode to character
static variable tracks previous scancode
kb_getchar returns the character or zero
this is called from the shell readline
it is the only entry point from this driver
*/

static const char scancodes[128] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' '
};

static uint8_t inb(uint16_t port) {
    uint8_t val;
    asm volatile("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

char kb_getchar() {
    static uint8_t last_sc = 0;

    if (!(inb(0x64) & 1)) {
        return 0;
    }

    uint8_t sc = inb(0x60);

    if (sc == last_sc) {
        return 0;
    }

    last_sc = sc;

    if (sc & 0x80) {
        return 0;
    }

    return scancodes[sc];
}

