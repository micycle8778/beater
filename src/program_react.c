#include "lcd.h"
#include "button.h"
#include "timer.h"
#include <stdio.h>
#include <stdlib.h>

#define BUTTONS (BUTTON_LEFT | BUTTON_RIGHT)
void program_react(void) {
reset:
    while (1) {
        lcd_clear();
        lcd_puts("Please wait...");

        char count = 0;
        while (count < 14) {
            // render bar
            lcd_second_line();
            lcd_putchar('|');
            for (char idx = count; idx > 0; idx--)
                lcd_putchar(0b10100101); // middle dot character: ・
            lcd_instruction(0xc0 + 15); // set ddram address
            lcd_putchar('|');

            // increment count
            count += 1;

            // sleep 256 ms
            if (button_sleep(256, BUTTONS)) {
                if (button_left()) return;
                goto reset;
            }

            // sleep random amount
            if (button_sleep(rand() & 0xff, BUTTONS)) {
                if (button_left()) return;
                goto reset;
            }
        }

        // display message
        lcd_clear();
        lcd_puts("PRESS RIGHT");

        // wait for user to press right
        uint32_t start = millis();
        while (!button_right());
        uint32_t delta = millis() - start;
        while (button_right());

        // output score
        char buf[16] = {0};
        snprintf(buf, 16, "%lu", delta);
        lcd_clear();
        lcd_puts("Your score:");
        lcd_second_line();
        lcd_puts(buf);


        unsigned char button = button_detect(BUTTONS);
        if (button & BUTTON_LEFT) return;
    }
}
