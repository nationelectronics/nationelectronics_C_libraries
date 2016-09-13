/*
 * demo-iope-write.c
 *
 *  Created on: 10/06/2015
 *
 *      compile with "gcc iope.c demo-iope-write.c -o demo-iope-write"
 *      run with "./demo-iope-write"
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
#include <time.h>


#include "iope.h"

int main(int argc, char **argv){
   iope_init(0x20);                       // initialise one chip at address 0x20 (chip 1 default)
   set_port_direction(0x20, PORTA, 0x00); // set the direction for port A to be outputs
   set_port_direction(0x20, PORTB, 0x00); // set the direction for port B to be outputs

   set_port_pullups(0x20, PORTA, 0);     // disable internal pullups for port A
   invert_port(0x20,PORTA,0);            // set output to 0 so port A will read as 1
   set_port_pullups(0x20, PORTA, 0);     // disable internal pullups for port A
   invert_port(0x20,PORTA,0);            // set output to 0 so port A will read as 1

   write_port(0x20, PORTA, 0);           // turn port A off
   write_port(0x20, PORTB, 0);           // turn port B off

   while (1){
      write_pin(0x20, PORTA, 0, 1);      // turn port A pin 0 on and off
      usleep(1000000);                   // connect a LED with a in series 300R resistor from pin 0 to GND
      write_pin(0x20, PORTA, 0, 0);      // you will see it blinking at 1 second intervals
      usleep(1000000);
   }
   return (0);
}
