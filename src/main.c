#include "button.h"
#include "timer.h"
#include "lcd.h"
#include "programs.h"
#include <stdio.h>
#include <stdlib.h>

__attribute__((interrupt))
void _irqbrk(void) {
    if (timer_handle_interrupt()) return;
}

typedef struct {
    char* name;
    void (*fp)(void);
} program;

const program programs[] = {
    { "Hello", program_hello },
    { "Uptime", program_uptime },
    { "React", program_react },
    { "Mastermind", program_mastermind },
};

__attribute__((section(".call_main")))
void _start(void) {
    asm("sei");
        button_init();
        lcd_init();
        timer_init();
    asm("cli");

    const unsigned char num_programs = sizeof(programs) / sizeof(programs[0]);
    unsigned char top_idx = 0;
    bool bottom = false; // true if cursor is on bottom line
    while (1) {
        lcd_clear();

        if (!bottom) lcd_putchar(' ');
        lcd_puts(programs[top_idx].name);

        lcd_second_line();
        if (bottom) lcd_putchar(' ');
        lcd_puts(programs[top_idx + 1].name);

        while (button_pressed(BUTTON_ALL)); // wait for every button to be released
        unsigned char button = button_detect(BUTTON_RIGHT | BUTTON_UP | BUTTON_DOWN);

        if (button & BUTTON_RIGHT) {
            unsigned char idx = bottom ? top_idx + 1 : top_idx;
            while (button_pressed(BUTTON_ALL)); // wait for every button to be released
            programs[idx].fp();
        }
        else if (button & BUTTON_UP) {
            if (bottom) {
                bottom = false;
            } else {
                top_idx = top_idx == 0 ? 0 : top_idx - 1;
            }
        }
        else if (button & BUTTON_DOWN) {
            if (!bottom) {
                bottom = true;
            } else {
                top_idx = top_idx == num_programs - 2 ? num_programs - 2 : top_idx + 1;
            }
        }
        else {
            // __builtin_unreachable();
        }
    }
}
