/*
 *  iope.h
 *
 *  Raspberry HAT - 32 I/O Ports Expander Library
 *  Created on: 10/06/2015
 * 
 */


static char PORTA=0;
static char PORTB=1;


void iope_init(char address);
/* Initialise the IO Port Expander - run before calling any other methods */


// local methods

int read_byte_data(char address, char reg);
void write_byte_data(char address, char reg, char value);
static char updatebyte(char address, char byte, char bit, char value);
static char checkbit(char address, char byte, char bit);

// public methods

void set_pin_direction(char address, char port, char pin, char direction);
/*
 set IO direction for an individual pin
 port PORTA or PORTB
 pins 0 to 7
 direction 1 = input, 0 = output
 ex. set chip1 on address 0x20, portB, pin5 to input
         set_pin_direction(0x20, PORTB, 5, 1);  
 */

void set_port_direction(char address, char port, char direction);
/*
 set direction for an IO port
 port PORTA or PORTB
 direction 1 = input, 0 = output
 ex. set chip1 on address 0x21, portA to output
         set_port_direction(0x21, PORTA, 0);
 */

void set_pin_pullup(char address, char port, char pin, char value);
/*
 set the internal 100K pull-up resistors for an individual pin
 port PORTA or PORTB
 pins 0 to 7
 value 1 = enabled, 0 = disabled
 */

void set_port_pullups(char address, char port, char value);
/*
 set the internal 100K pull-up resistors for the selected port 
 port PORTA or PORTB
 value 1 = enabled, 0 = disabled
 */

void write_pin(char address, char port, char pin, char value);
/*
 write to an individual pin
 port PORTA or PORTB
 pins 0 to 7
 value 1 = logic level high (5V), 0 = logic level low (GND)
 */

void write_port(char address, char port, char value);
/*
 write to all pins on the selected port
 port PORTA or PORTB
 value = number between 0x00 and 0xFF or 0 and 255 or 0b00000000 and 0b11111111
 remember pin7 is the most significant bit and pin0 is the least significant bit
 ex. set chip1 on address 0x20, portB, all pins to 5V but pin2
         set_pin_direction(0x20, PORTB, 0b11111011);
         set_pin_direction(0x20, PORTB, 251);  
 */

int read_pin(char address, char port, char pin);
/*
 read the value of an individual pin
 port PORTA or PORTB
 pins 0 to 7
 returns 1 = logic level high (5V), 0 = logic level low (GND)
 */

char read_port(char address, char port);
/*
 read all pins on the selected port
 port PORTA or PORTB
 returns number between 0 and 255 each bit match a pin
 remember pin7 is the most significant bit and pin0 is the least significant bit
 */

void invert_port(char address, char port, char polarity);
/*
 invert the polarity of the pins on a selected port
 port PORTA or PORTB
 polarity 0 = same logic state of the input pin, 1 = inverted logic
 state of the input pin
 */

void invert_pin(char address, char port, char pin, char polarity);
/*
 invert the polarity of the selected pin
 port PORTA or PORTB
 pins 0 to 7
 polarity 0 = same logic state of the input pin, 1 = inverted logic
 state of the input pin
 */

void mirror_interrupts(char address, char value);
/*
 1 = The char pins are internally connected, 0 = The char pins are not
 connected. INTA is associated with PortA and INTB is associated with
 PortB
 */

void set_interrupt_polarity(char address, char value);
/*
 This sets the polarity of the char output pins - 1 = Active-high.
 0 = Active-low.
 */

void set_interrupt_type(char address, char port, char value);
/*
 Sets the type of interrupt for each pin on the selected port
 port PORTA or PORTB
 1 = interrupt is fired when the pin matches the default value
 0 = the interrupt is fired on state change
 */

void set_interrupt_defaults(char address, char port, char value);
/*
 These bits set the compare value for pins configured for
 interrupt-on-change on the selected port.
 If the associated pin level is the opposite from the register bit, an
 interrupt occurs.
 port PORTA or PORTB
 value from 0 to 255
 */

void set_interrupt_on_port(char address, char port, char value);
/*
 Enable interrupts for the pins on the selected port
 port PORTA or PORTB
 value = number between 0x00 and 0xFF or 0 and 255 or 0b00000000 and 0b11111111
 */

void set_interrupt_on_pin(char address, char port, char pin, char value);
/*
 Enable interrupts for the selected pin
 port PORTA or PORTB
 pins 0 to 7
 Value 0 = interrupt disabled, 1 = interrupt enabled
 */

char read_interrupt_status(char address, char port);
/*
 read the interrupt status for the pins on the selected port
 port PORTA or PORTB
 */

char read_interrupt_capture(char address, char port);
/*
 read the value from the selected port at the time of the last
 interrupt trigger
 port PORTA or PORTB
 */

void reset_interrupts(char address);
/*
 set the interrupts A and B to 0
 */
