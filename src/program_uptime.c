#include "lcd.h"
#include "timer.h"
#include "button.h"
#include <stdio.h>

void program_uptime(void) {
    char buf[16] = {0};
    lcd_clear();
    while (1) {
        snprintf(buf, 16, "%lu", millis() / 1000);
        lcd_puts(buf);

        if (button_sleep(1000, BUTTON_LEFT)) return;
        lcd_return_home();
    }
}
