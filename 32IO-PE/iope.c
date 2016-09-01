/*
 ================================================
 Raspberry Pi HAT - 32-IO Port Expander
 Version 1.1 Created 10/06/2015
 ================================================


 The Microchip MCP23017 chip has two 8-bit ports (A and B).  
 Port A and Port B have 8 pins from 0 to 7, where 7 is the most significant bit
 and 0 is the least significant bit when writing to or reading from a port.
 
 address1(chip1) --> PortA --> Pin0 to Pin7
                 --> PortB --> Pin0 to Pin7

 address2(chip2) --> PortA --> Pin0 to Pin7
                 --> PortB --> Pin0 to Pin7

                 
 Required package{
 apt-get install libi2c-dev
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>

static int i2cbus;
const char *fileName = "/dev/i2c-1"; // change to /dev/i2c-0 if you are using a revision 0002 or 0003 model B

static char IODIRA = 0x00; // IO direction A - 1= input 0 = output
static char IODIRB = 0x01; // IO direction B - 1= input 0 = output
	// Input polarity A - If a bit is set, the corresponding GPIO register bit
	// will reflect the inverted value on the pin.
static char IPOLA = 0x02;
	// Input polarity B - If a bit is set, the corresponding GPIO register bit
	// will reflect the inverted value on the pin.
static char IPOLB = 0x03;
	// The GPINTEN register controls the interrupt-onchange feature for each
	// pin on port A.
static char GPINTENA = 0x04;
	// The GPINTEN register controls the interrupt-onchange feature for each
	// pin on port B.
static char GPINTENB = 0x05;
	// Default value for port A - These bits set the compare value for pins
	// configured for interrupt-on-change. If the associated pin level is the
	// opposite from the register bit, an interrupt occurs.
static char DEFVALA = 0x06;
	// Default value for port B - These bits set the compare value for pins
	// configured for interrupt-on-change. If the associated pin level is the
	// opposite from the register bit, an interrupt occurs.
static char DEFVALB = 0x07;
	// Interrupt control register for port A.  If 1 interrupt is fired when the
	// pin matches the default value, if 0 the interrupt is fired on state
	// change
static char INTCONA = 0x08;
	// Interrupt control register for port B.  If 1 interrupt is fired when the
	// pin matches the default value, if 0 the interrupt is fired on state
	// change
static char INTCONB = 0x09;
static char IOCON = 0x0A; // see datasheet for configuration register
static char GPPUA = 0x0C; // pull-up resistors for port A
static char GPPUB = 0x0D; // pull-up resistors for port B
	// The INTF register reflects the interrupt condition on the port A pins of
	// any pin that is enabled for interrupts. A set bit indicates that the
	// associated pin caused the interrupt.
static char INTFA = 0x0E;
	// The INTF register reflects the interrupt condition on the port B pins of
	// any pin that is enabled for interrupts. A set bit indicates that the
	// associated pin caused the interrupt.
static char INTFB = 0x0F;
	// The INTCAP register captures the GPIO port A value at the time the
	// interrupt occurred.
static char INTCAPA = 0x10;
	// The INTCAP register captures the GPIO port B value at the time the
	// interrupt occurred.
static char INTCAPB = 0x11;
static char GPIOA = 0x12; // data port A
static char GPIOB = 0x13; // data port B
static char OLATA = 0x14; // output latches A
static char OLATB = 0x15; // output latches B

static char PORTA = 0;
static char PORTB = 1;

	// variables
	// initial configuration - see IOCON page in the MCP23017 datasheet for
	// more information.
static char config = 0x22;

unsigned char buf[10] = { 0 };

// local methods

int read_byte_data(char address, char reg) {

	if ((i2cbus = open(fileName, O_RDWR)) < 0) {
		printf("Failed to open i2c port for read %s \n", strerror(errno));
		exit(1);
	}

	if (ioctl(i2cbus, I2C_SLAVE, address) < 0) {
		printf("Failed to write to i2c port for read\n");
		exit(1);
	}

	buf[0] = reg;

	if ((write(i2cbus, buf, 1)) != 1) {
		printf("Failed to write to i2c device for read\n");
		exit(1);
	}

	if (read(i2cbus, buf, 1) != 1) { // Read back data into buf[]
		printf("Failed to read from slave\n");
		exit(1);
	}

	close(i2cbus);
	return (buf[0]);
}

void write_byte_data(char address, char reg, char value) {
	if ((i2cbus = open(fileName, O_RDWR)) < 0) {
		printf("Failed to open i2c port for write\n");
		exit(1);
	}

	if (ioctl(i2cbus, I2C_SLAVE, address) < 0) {
		printf("Failed to write to i2c port for write\n");
		exit(1);
	}

	buf[0] = reg;
	buf[1] = value;

	if ((write(i2cbus, buf, 2)) != 2) {
		printf("Failed to write to i2c device for write\n");
		exit(1);
	}
	close(i2cbus);
}

static char updatebyte(char byte, char bit, char value) {
	/*
	 internal method for setting the value of a single bit within a byte
	 */
	if (value == 0) {
		return (byte &= ~(1 << bit));

	} else {
		return (byte |= 1 << bit);
	}
}

static char checkbit(char byte, char bit) {
	/*
	 internal method for reading the value of a single bit within a byte
	 */
	if (byte & (1 << bit)) {
		return (1);
	} else {
		return (0);
	}
}

// public methods

void set_pin_direction(char address, char port, char pin, char direction) {
	/*
	 set IO direction for an individual pin
	 port PORTA or PORTB
	 pins 0 to 7
	 direction 1 = input, 0 = output
	 ex. set chip1 on address 0x20, portB, pin5 to input
	         set_pin_direction(0x20, PORTB, 5, 1);  
	 */
	char cval = 0;
	port=(port)?IODIRB:IODIRA;
    cval = read_byte_data(address, port);
    cval = updatebyte(cval, pin, direction);
	write_byte_data(address, port, cval);
}

void set_port_direction(char address, char port, char direction) {
	/*
	 set direction for an IO port
	 port PORTA or PORTB
	 direction 1 = input, 0 = output
	 ex. set chip1 on address 0x21, portA to output
	         set_port_direction(0x21, PORTA, 0);
	 */
	port=(port)?IODIRB:IODIRA;
    write_byte_data(address, port, direction);
}

void set_pin_pullup(char address, char port, char pin, char value) {
	/*
	 set the internal 100K pull-up resistors for an individual pin
	 port PORTA or PORTB
	 pins 0 to 7
	 value 1 = enabled, 0 = disabled
	 */
	char cval = 0;
	port=(port)?GPPUB:GPPUA;
    cval = read_byte_data(address, port);
	cval = updatebyte(cval, pin, value);
	write_byte_data(address, port, cval);
}

void set_port_pullups(char address, char port, char value) {
	/*
	 set the internal 100K pull-up resistors for the selected port 
	 port PORTA or PORTB
	 value 1 = enabled, 0 = disabled
	 */
	port=(port)?GPPUB:GPPUA; 
	write_byte_data(address, port, value);
}

void write_pin(char address, char port, char pin, char value) {
	/*
	 write to an individual pin
	 port PORTA or PORTB
	 pins 0 to 7
	 value 1 = logic level high (5V), 0 = logic level low (GND)
	 */
	port=(port)?GPIOB:GPIOA; 
	char cval = 0;
	cval = read_byte_data(address, GPIOA);
	cval = updatebyte(cval, pin, value);
	write_byte_data(address, GPIOA, cval);
}

void write_port(char address, char port, char value) {
	/*
	 write to all pins on the selected port
	 port PORTA or PORTB
	 value = number between 0x00 and 0xFF or 0 and 255 or 0b00000000 and 0b11111111
	 remember pin7 is the most significant bit and pin0 is the least significant bit
	 ex. set chip1 on address 0x20, portB, all pins to 5V but pin2
	         set_pin_direction(0x20, PORTB, 0b11111011);
	         set_pin_direction(0x20, PORTB, 251);  
	 */
	port=(port)?GPIOB:GPIOA; 
    write_byte_data(address, port, value);
}

int read_pin(char address, char port, char pin) {
	/*
	 read the value of an individual pin
	 port PORTA or PORTB
	 pins 0 to 7
	 returns 1 = logic level high (5V), 0 = logic level low (GND)
	 */
	port=(port)?GPIOB:GPIOA; 
    return (checkbit(read_byte_data(address, port), pin));
}

char read_port(char address, char port) {
	/*
	 read all pins on the selected port
	 port PORTA or PORTB
	 returns number between 0 and 255 each bit match a pin
	 remember pin7 is the most significant bit and pin0 is the least significant bit
	 */
	port=(port)?GPIOB:GPIOA; 
	return (read_byte_data(address, port));
}

void invert_port(char address, char port, char polarity) {
	/*
	 invert the polarity of the pins on a selected port
	 port PORTA or PORTB
	 polarity 0 = same logic state of the input pin, 1 = inverted logic
	 state of the input pin
	 */
	port=(port)?IPOLB:IPOLA; 
	write_byte_data(address, port, polarity);
}

void invert_pin(char address, char port, char pin, char polarity) {
	/*
	 invert the polarity of the selected pin
	 port PORTA or PORTB
	 pins 0 to 7
	 polarity 0 = same logic state of the input pin, 1 = inverted logic
	 state of the input pin
	 */
	port=(port)?IPOLB:IPOLA; 
	char cval = 0;
	cval = read_byte_data(address, port);
	cval = updatebyte(cval, pin, polarity);
	write_byte_data(address, port, cval);
}

void mirror_interrupts(char address, char value) {
	/*
	 1 = The char pins are internally connected, 0 = The char pins are not
	 connected. INTA is associated with PortA and INTB is associated with
	 PortB
	 */
	value=(value)?1:0; 
	config = updatebyte(config, 6, value);
	write_byte_data(address, IOCON, config);
}

void set_interrupt_polarity(char address, char value) {
	/*
	 This sets the polarity of the char output pins - 1 = Active-high.
	 0 = Active-low.
	 */
	value=(value)?1:0; 
	config = updatebyte(config, 1, value);
	write_byte_data(address, IOCON, config);
}

void set_interrupt_type(char address, char port, char value) {
	/*
	 Sets the type of interrupt for each pin on the selected port
	 port PORTA or PORTB
	 1 = interrupt is fired when the pin matches the default value
	 0 = the interrupt is fired on state change
	 */
	port=(port)?INTCONB:INTCONA; 
	write_byte_data(address, port, value);
}

void set_interrupt_defaults(char address, char port, char value) {
	/*
	 These bits set the compare value for pins configured for
	 interrupt-on-change on the selected port.
	 If the associated pin level is the opposite from the register bit, an
	 interrupt occurs.
	 port PORTA or PORTB
	 value from 0 to 255
	 */
	port=(port)?DEFVALB:DEFVALA; 
	write_byte_data(address, port, value);
}

void set_interrupt_on_port(char address, char port, char value) {
	/*
	 Enable interrupts for the pins on the selected port
	 port PORTA or PORTB
	 value = number between 0x00 and 0xFF or 0 and 255 or 0b00000000 and 0b11111111
	 */
	port=(port)?GPINTENB:GPINTENA; 
	write_byte_data(address, port, value);
}

void set_interrupt_on_pin(char address, char port, char pin, char value) {
	/*
	 Enable interrupts for the selected pin
	 port PORTA or PORTB
	 pins 0 to 7
	 Value 0 = interrupt disabled, 1 = interrupt enabled
	 */
	port=(port)?GPINTENB:GPINTENA; 
	char cval = 0;
	cval = read_byte_data(address, port);
	cval = updatebyte(cval, pin, value);
	write_byte_data(address, port, cval);
}

char read_interrupt_status(char address, char port) {
	/*
	 read the interrupt status for the pins on the selected port
	 port PORTA or PORTB
	 */
	port=(port)?INTFB:INTFA; 
	return (read_byte_data(address, port));
}

char read_interrupt_capture(char address, char port) {
	/*
	 read the value from the selected port at the time of the last
	 interrupt trigger
	 port PORTA or PORTB
	 */
	port=(port)?INTCAPB:INTCAPA; 
	return (read_byte_data(address, port));
}

void reset_interrupts(char address) {
	/*
	 set the interrupts A and B to 0
	 */
	read_interrupt_capture(address, 0);
	read_interrupt_capture(address, 1);
}

void iope_init(char address) {
	write_byte_data(address, IOCON, config);
	write_byte_data(address, IODIRA, 0xFF);
	write_byte_data(address, IODIRB, 0xFF);
	set_port_pullups(address, PORTA, 0x00);
	set_port_pullups(address, PORTB, 0x00);
	invert_port(address, PORTA, 0x00);
	invert_port(address, PORTB, 0x00);
}
