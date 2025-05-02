#pragma once
#include <stdbool.h>

#define BUTTON_UP 0x80
#define BUTTON_RIGHT 0x40
#define BUTTON_DOWN 0x20
#define BUTTON_LEFT 0x10

void button_init();
bool button_up();
bool button_right();
bool button_down();
bool button_left();
bool button_pressed(unsigned char mask);
