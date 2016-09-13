/*
 *  C demo code for:
 *  Raspberry Pi HAT - 8 Channel ADC V1.1
 *  Microchip MCP3208 chip
 *
 *  compile:
 *  gcc -lwiringPi -o adcdemo adcdemo.c
 *
 *
 */

#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <math.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

double readADCChannel(chip, channel){
  unsigned char cntrlStack[3];
  unsigned char tmp;
  uint result;
  double realResult;
  /* figure 6-1 of MCP3208 datasheet */
  switch(channel){
  case 0:
    cntrlStack[0] = 0b00000110;
    cntrlStack[1] = 0b00000000;
    break;
  case 1:
    cntrlStack[0] = 0b00000110;
    cntrlStack[1] = 0b01000000;
    break;
  case 2:
    cntrlStack[0] = 0b00000110;
    cntrlStack[1] = 0b10000000;
    break;
  case 3:
    cntrlStack[0] = 0b00000110;
    cntrlStack[1] = 0b11000000;
    break;
  case 4:
    cntrlStack[0] = 0b00000111;
    cntrlStack[1] = 0b00000000;
    break;
  case 5:
    cntrlStack[0] = 0b00000111;
    cntrlStack[1] = 0b01000000;
    break;
  case 6:
    cntrlStack[0] = 0b00000111;
    cntrlStack[1] = 0b10000000;
    break;
  case 7:
    cntrlStack[0] = 0b00000111;
    cntrlStack[1] = 0b11000000;
    break;
  }
  cntrlStack[2] = 0b00000000;

  wiringPiSPIDataRW (chip, cntrlStack, 3);

  tmp=(cntrlStack[1]<<4);
  result=(uint)tmp;
  result=(result<<4);
  result=result + (uint)cntrlStack[2];
  return result;
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
       realResult = (double)result / (double)4095 * vref;
       fprintf(stdout,"chn: %i result: %f V = %i\n", channel, realResult, result);
     }
     usleep(1000000); //wait for one second
  }
  return;
}
