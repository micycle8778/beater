#include "button.h"
#include "timer.h"
#include "lcd.h"
#include <stdio.h>

void interrupt_handler(void) {
    if (timer_handle_interrupt()) return;
}

__attribute__((interrupt))
void _irqbrk(void) {
    interrupt_handler();
}


#define BLINK_MS 750
__attribute__((section(".call_main")))
void _start(void) {
    asm("sei");
        button_init();
        timer_init();
        lcd_init();
    asm("cli");

reset:
    lcd_enable_cursor();
    while (button_right());
    lcd_clear();

    for (char* s = "Hello, world!"; *s; s++) {
        lcd_putchar(*s);
        if (button_sleep(100, BUTTON_RIGHT)) goto reset;
    }

    while (1) {
        lcd_disable_cursor();
        if (button_sleep(BLINK_MS, BUTTON_RIGHT)) goto reset;
        lcd_enable_cursor();
        if (button_sleep(BLINK_MS, BUTTON_RIGHT)) goto reset;
    }
}
