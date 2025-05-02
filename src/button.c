#include <stdbool.h>
#include "button.h"

static volatile unsigned char* port_a = (volatile unsigned char*)(0x6001);
static volatile unsigned char* data_direction_a = (volatile unsigned char*)(0x6003);

void button_init() {
    // set top 4 bits to read
    // technically not needed because when the VIA resets, it sets all pins to input
    // *data_direction_a = 0x0f;
}

inline bool button_up() {
    return *port_a & BUTTON_UP;
}

inline bool button_right() {
    return *port_a & BUTTON_RIGHT;
}

inline bool button_down() {
    return *port_a & BUTTON_DOWN;
}

inline bool button_left() {
    return *port_a & BUTTON_LEFT;
}

inline bool button_pressed(unsigned char mask) {
    return *port_a & mask;
}

// wait for a specified button(s) to be pressed
// returns the button that was pressed
unsigned char button_detect(unsigned char mask) {
    while (!button_pressed(mask));
    return *port_a & mask;
}
