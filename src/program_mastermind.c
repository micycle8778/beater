#include "lcd.h"
#include "button.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void program_mastermind(void) {
    char code[6];
    const char code_size = sizeof(code) / sizeof(code[0]);

reset:
    while (1) {
        lcd_clear();
        // create secret code
        for (char idx = 0; idx < code_size; idx++)
            code[idx] = ((unsigned char)rand()) % 8;
        
        // setup guess data
        char guess_count = 0;
        char guess_idx = 0;
        char guess[code_size];
        memset(guess, 0, code_size);

        while (1) {
            guess_count++;
            // loop until player submits a guess
            while (1) {
                // display guess
                lcd_disable_cursor();
                lcd_return_home();
                for (int idx = 0; idx < code_size; idx++) {
                    lcd_putchar('0' + guess[idx]);
                }

                lcd_instruction(0x80 + guess_idx);
                lcd_enable_cursor();

                button_t button = button_detect(BUTTON_ALL);
                while(button_pressed(BUTTON_ALL));
                
                if (button & BUTTON_LEFT) {
                    if (guess_idx == 0) return;
                    guess_idx -= 1;
                } else if (button & BUTTON_RIGHT) {
                    if (guess_idx == code_size - 1) break;
                    else guess_idx += 1;
                } else if (button & BUTTON_UP) {
                    guess[guess_idx] += 1;
                    guess[guess_idx] = guess[guess_idx] % 8;
                } else if (button & BUTTON_DOWN) {
                    if (guess[guess_idx] == 0) guess[guess_idx] = 7;
                    else guess[guess_idx] -= 1;
                }
            }

            char unmatched_char_count[8] = {0};
            char correct_count = 0;
            char report[code_size];
            memset(report, ' ', code_size);
            for (int idx = 0; idx < code_size; idx++) {

                if (code[idx] == guess[idx]) {
                    report[idx] = ':';
                    correct_count += 1;
                } else {
                    unmatched_char_count[code[idx]]++;
                }
            }

            if (correct_count == code_size) {
                char buf[16] = {0};
                lcd_clear();
                lcd_puts("You win!");
                lcd_second_line();
                snprintf(buf, 16, "Guesses: %d", guess_count);
                lcd_puts(buf);

                button_t button = button_detect(BUTTON_LEFT | BUTTON_RIGHT);
                while(button_pressed(BUTTON_ALL));

                if (button & BUTTON_RIGHT) goto reset;
                else return;
            }

            
            for (int idx = 0; idx < code_size; idx++) {
                if (report[idx] == ':') continue;
                if (unmatched_char_count[guess[idx]]) {
                    unmatched_char_count[guess[idx]]--;
                    report[idx] = '.';
                }
            }

            // print guess on the right
            lcd_instruction(0x80 + (16 - code_size));
            for (int idx = 0; idx < code_size; idx++) {
                lcd_putchar('0' + guess[idx]);
            }

            // print report on the right
            lcd_instruction(0xc0 + (16 - code_size));
            lcd_puts(report);
        }
    }
}
