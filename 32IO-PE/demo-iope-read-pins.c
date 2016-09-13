/*
 * demo-iope-read-pins.c
 *
 *  Created on: 10/06/2015
 *
 *      compile with "gcc iope.c demo-iope-read-pins.c -o demo-iope-read-pins"
 *      run with "./demo-iope-read-pins"
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
   int i;

   iope_init(0x20); // initialise one of the io pi buses on i2c address 0x20, default address for chip 1

   set_port_direction(0x20,PORTA, 0xFF); // set port A to be inputs
   set_port_direction(0x20,PORTB, 0xFF); // set port B to be inputs
   set_port_pullups(0x20, PORTA, 0xFF);  // enable internal pullups for port A
   invert_port(0x20,PORTA,0xFF);         // invert output so port A will read as 0
   set_port_pullups(0x20, PORTB, 0xFF);  // enable internal pullups for port B
   invert_port(0x20,PORTB,0xFF);         // invert output so port B will read as 0


   while (1){
      system("clear");
      for (i=0;i<8;i++) {
         printf("Port A Pin %i: %x \n", i, read_pin(0x20, PORTA, i)); // grounding a pins will change the value
      }
      for (i=0;i<8;i++) {
         printf("Port B Pin %i: %x \n", i, read_pin(0x20, PORTB, i)); // grounding a pins will change the value
      }
      usleep(500000); // sleep 0.5 seconds
   }

   return (0);
}
