#define ENABLE 0x08
#define READ_WRITE 0x04
#define REGISTER_SELECT 0x02

static volatile char* port_b = (char*)0x6000;
static volatile char* data_direction_b = (char*)0x6002;

static void lcd_wait(void) {
    asm("sei");

    *data_direction_b = 0x0f;

    unsigned char response = 0;
    do {
        // send read request to lcd
        *port_b = READ_WRITE;
        *port_b = READ_WRITE | ENABLE;

        // read response from lcd
        response = *port_b;

        // read out the rest of the response
        *port_b = READ_WRITE;
        *port_b = READ_WRITE | ENABLE;
    } while (response & 0x80);

    *data_direction_b = 0xff;

    asm("cli");
}

void lcd_putchar(unsigned char character) {
    lcd_wait();

    asm("sei");
    const char higher = character & 0xf0 | REGISTER_SELECT;
    const char lower = ((character & 0x0f) << 4) | REGISTER_SELECT;

    *port_b = higher;
    *port_b = higher | ENABLE;
    *port_b = higher;

    *port_b = lower;
    *port_b = lower | ENABLE;
    *port_b = lower;
    asm("cli");
}

void lcd_puts(const char* s) {
    while (*s != 0) {
        lcd_putchar(*s);
        s += 1;
    }
}

void lcd_instruction(unsigned char instruction) {

    lcd_wait();

    asm("sei");
    const char higher = instruction & 0xf0;
    const char lower = (instruction & 0x0f) << 4;

    *port_b = higher;
    *port_b = higher | ENABLE;
    *port_b = higher;

    *port_b = lower;
    *port_b = lower | ENABLE;
    *port_b = lower;
    asm("cli");
}

void lcd_enable_cursor(void) {
    lcd_instruction(0b00001110);
}

void lcd_disable_cursor(void) {
    lcd_instruction(0b00001100);
}

void lcd_clear(void) {
    lcd_instruction(0x01);
}

void lcd_return_home(void) {
    lcd_instruction(0x02);
}

void lcd_init(void) {
    *data_direction_b = 0xff;

    // set 8 bit mode (in case we're resetting w/o disconnecting power)
    char msg = 0b00110000;
    *port_b = msg;
    *port_b = msg | ENABLE;
    *port_b = msg;

    *port_b = 0;
    *port_b = ENABLE;
    *port_b = 0;

    // read busy flag
    while (1) {
        *port_b = READ_WRITE;
        *port_b = READ_WRITE | ENABLE;

        char response = *port_b;

        *port_b = READ_WRITE;

        if ((response & 0x80) == 0) break;
    }

    // set 4 bit mode
    msg = 0b00100000;
    *port_b = msg;
    *port_b = msg | ENABLE;
    *port_b = msg;

    lcd_instruction(0b00101000); // function set: 2 line 5x8 characters
    lcd_disable_cursor();
    lcd_instruction(0b00000110); // turn on cursor increment and disable shift
    lcd_clear();
    lcd_return_home();
}

void __putchar(char character) {
    lcd_putchar(character);
}
