#include "lcd.h"
#include "button.h"
#include "timer.h"

#include <stdlib.h>

#define BLINK_MS 750
#define BUTTONS BUTTON_RIGHT | BUTTON_LEFT
void program_hello(void) {
    for (int iterations = 0; ; iterations++) {
        if (button_left()) {
            lcd_disable_cursor();
            return;
        }

        lcd_enable_cursor();
        while (button_right()); // wait for button to be released
        lcd_clear();

        for (char* s = "Hello, world!"; *s; s++) {
            if ((iterations & 0xf) > (rand() & 0xf)) {
                lcd_putchar('0' + rand() & 0x3f);
            } else {
                lcd_putchar(*s);
            }

            if (button_sleep(100, BUTTONS)) goto reset;
        }

        while (1) {
            lcd_disable_cursor();
            if (button_sleep(BLINK_MS, BUTTONS)) goto reset;
            lcd_enable_cursor();
            if (button_sleep(BLINK_MS, BUTTONS)) goto reset;
        }
reset: {}
    }
}
