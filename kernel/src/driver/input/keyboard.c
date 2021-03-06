#include <stdbool.h>

#include "lib/int.h"
#include "common/init.h"
#include "common/asm.h"
#include "arch/gdt.h"
#include "arch/idt.h"
#include "input/keyboard.h"

static bool key_state[128];
static const char key_map[] = {
          // 0x00
          0, 0,
          '\1', '\1',
          '1', '!',
          '2', '@',
          '3', '#',
          '4', '$',
          '5', '%',
          '6', '^',
          '7', '&',
          '8', '*',
          '9', '(',
          '0', ')',
          '-', '_',
          '=', '+',
          '\x7f', '\x7f', // backspace
          ' ', ' ',

          // 0x10
          'q', 'Q',
          'w', 'W',
          'e', 'E',
          'r', 'R',
          't', 'T',
          'y', 'Y',
          'u', 'U',
          'i', 'I',
          'o', 'O',
          'p', 'P',
          '[', '{',
          ']', '}',
          '\n', '\n',
            0, 0,
          'a', 'A',
          's', 'S',

          // 0x20
          'd', 'D',
          'f', 'F',
          'g', 'G',
          'h', 'H',
          'j', 'J',
          'k', 'K',
          'l', 'L',
          ';', ':',
          '\'', '\"',
          '`', '~',
          0, 0,
          '\\', '|',
          'z', 'Z',
          'x', 'X',
          'c', 'C',
          'v', 'V',

          // 0x30
          'b', 'B',
          'n', 'N',
          'm', 'M',
          ',', '<',
          '.', '>',
          '/', '?',
          0, 0,
          '*', '*',
          0, 0,
          ' ', ' ',
          0, 0,
          0, 0,
          0, 0,
          0, 0,
          0, 0,
          0, 0,

          // 0x40
          0,  0,
          0,  0,
          0,  0,
          0,  0,
          0,  0,
          0,  0,
          0,  0,
          '7', '7',
          '\3', '\3',
          '9', '9',
          '-', '-',
          '4', '4',
          '5', '5',
          '6', '6',
          '+', '+',
          '1', '1',

          // 0x50
          '\5', '\5',
          '3', '3',
          '0', '0',
          '.', '.',
          0, 0
};

static void (*keyup)(char);
static void (*keydown)(char);

void keyboard_register_key_up(void (*handler)(char)) {
    keyup = handler;
}

void keyboard_register_key_down(void (*handler)(char)) {
    keydown = handler;
}

bool shift_down() {
    return key_state[42] || key_state[54];
}

bool control_down() {
    return key_state[29];
}

bool alt_down() {
    return key_state[56];
}

static char translate_code(uint16_t code) {
    bool shift = shift_down();
    if(key_state[58]) {
        shift = !shift;
    }

    return key_map[code * 2 + (shift ? 1 : 0)];
}

static void dispatch(uint16_t code) {
    if(((uint32_t) code) >> 7) {
        code -= 128;
        key_state[code] = false;

        if(keyup != 0) {
            (*keyup)(translate_code(code));
        }
    } else {
        key_state[code] = true;

        if(keydown != 0) {
            (*keydown)(translate_code(code));
        }
    }
}

static void handle_keyboard(interrupt_t UNUSED(*interrupt)) {
    while(inb(0x64) & 2);
    dispatch(inb(0x60));
}

static INITCALL keyboard_init() {
    idt_register(33, CPL_KERNEL, handle_keyboard);

    return 0;
}

subsys_initcall(keyboard_init); //FIXME create input subsystem
