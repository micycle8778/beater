#include "lcd.h"

void _irqbrk(void) {
}

__attribute__((section(".call_main")))
void _start(void) {
    lcd_init();
    lcd_enable_cursor();
    puts("Hello, world!");

    while (1) {
    }
}
