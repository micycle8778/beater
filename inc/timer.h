#pragma once
#include <stdbool.h>
#include <stdint.h>

void timer_init();
bool timer_handle_interrupt();
void sleep(uint32_t millis);
uint32_t millis();
bool button_sleep(uint32_t millis, char mask);
