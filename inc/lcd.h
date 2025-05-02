#pragma once

void lcd_puts(char* s);
void lcd_putchar(unsigned char c);
void lcd_instruction(unsigned char instruction);
void lcd_enable_cursor(void);
void lcd_disable_cursor(void);
void lcd_clear(void);
void lcd_return_home(void);
void lcd_second_line(void);
void lcd_init(void);
