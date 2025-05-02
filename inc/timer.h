#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "button.h"

void timer_init();
bool timer_handle_interrupt();
void sleep(uint32_t millis);
uint32_t millis();
bool button_sleep(uint32_t millis, button_t mask);
