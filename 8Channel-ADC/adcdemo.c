/*
 *  C demo code for:
 *  Raspberry Pi HAT - 8 Channel ADC V1.1
 *  Microchip MCP3208 chip
 *
 *  compile:
 *  gcc -lwiringPi -o adcdemo adcdemo.c
 *
 *  Updated 10/30/2016
 */

#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <math.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

  /* channel 0..7 */
int readADCChannel(chip, channel){
  unsigned char cntrlStack[3];
  unsigned char tmp;
  uint result;

  /* figure 6-1 of MCP3208 datasheet */
  cntrlStack[0] = 6 + ((channel & 4) >> 2);
  cntrlStack[1] = (channel & 3) << 6;
  cntrlStack[2] = 0;

  wiringPiSPIDataRW (chip, cntrlStack, 3);

  return ((cntrlStack[1] & 15) << 8) + cntrlStack[2];
}

int main(int argc, char **argv){
  int    chip = 0; // (jumper CE0 on) chip = 0 (default), (jumper CE1 on) chip = 1
  int    channel; 
  int    result;
  int    errno;
  double realResult;
  double vref = 5.0; // jumper selected: 5.0 (default), 3.3, 1.0, or 0.3 Volts

  if (wiringPiSPISetup (chip, 200000) < 0){
     fprintf (stderr, "SPI Setup failed: %s\n", strerror (errno));
     exit(1);
  }

  while (1) {
     system("clear");
     for (channel = 0; channel < 8; channel++) {
       result = readADCChannel( chip, channel); // from 0 to 4095
       realResult = (double)result / (double)4095 * vref; // from 0 to Vref
       fprintf(stdout,"chn: %i result: %f V = %i\n", channel, realResult, result);
     }
     usleep(2000000); //wait for two second
  }
  return;
}
