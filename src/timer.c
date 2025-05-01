#include <stdbool.h>
#include <stdint.h>

// core counter of the timer
// counts milliseconds in a 32bit integer
// the clock will roll over after 49.7 days
static volatile uint32_t counter = 0;
static volatile uint32_t idle_counter = 0;

static volatile unsigned char* interrupt_enable_register    = (unsigned char*)(0x600e);
static volatile unsigned char* interrupt_flag_register      = (unsigned char*)(0x600d);
static volatile unsigned char* auxiliary_control_register   = (unsigned char*)(0x600b);

static volatile unsigned char* timer_1_counter_low          = (unsigned char*)(0x6004);
static volatile unsigned char* timer_1_counter_high         = (unsigned char*)(0x6005);

// setup T1 as our timer
// we want to use T1 instead of T2 as T2 can be used with the built in shift register
void timer_init() {
    idle_counter = 0;
    counter = 0;
    *interrupt_enable_register = 0b11000000;  // enable T1 interrupt
    *auxiliary_control_register = 0b01000000; // set T1 to continuous interrupts

    // count every 1000 cycles (at 1MHz, the timer will fire every millisecond)
    const uint16_t cycles = 1000 - 2; // subtract 2 to account for timer overhead
    const unsigned char low_cycles = cycles & 0xff; // low byte
    const unsigned char high_cycles = cycles >> 8;  // high byte

    *timer_1_counter_low = low_cycles;
    *timer_1_counter_high = high_cycles; // writing here starts the timer
}

// handles a potential timer interrupt
// return true if the interrupt was a timer interrupt
// returns false if not
bool timer_handle_interrupt() {
    // check the interrupt flag register for a timer 1 interrupt
    // if no such interrupt exists, return false
    if ((*interrupt_flag_register & 0x40) == 0) return false;

    // read timer 1 counter low to clear the interrupt
    *timer_1_counter_low;
    // counter += 1;
    if (idle_counter > 0) idle_counter--;
    return true;
}

__attribute__((noinline))
void sleep(uint32_t millis) {
    idle_counter = millis;
    while (idle_counter) {
        asm("wai");
    }
}

uint32_t millis() {
    return counter;
}
