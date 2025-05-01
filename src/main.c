#include "timer.h"
#include "lcd.h"

void interrupt_handler(void) {
    if (timer_handle_interrupt()) return;
}

__attribute__((interrupt))
void _irqbrk(void) {
    asm("sei");
    interrupt_handler();
    asm("cli");
}


#define SLEEP_MS 1000
__attribute__((section(".call_main")))
void _start(void) {
    asm("sei");
        timer_init();
        lcd_init();
    asm("cli");

    lcd_enable_cursor();

    for (char* s = "Hello, world!"; *s; s++) {
        lcd_putchar(*s);
        sleep(100);
    }

    while (1) {
        lcd_disable_cursor();
        sleep(1000);
        lcd_enable_cursor();
        sleep(1000);
    }

}
