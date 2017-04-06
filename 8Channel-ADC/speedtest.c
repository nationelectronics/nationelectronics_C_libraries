/*
 *  C demo code for:
 *  Raspberry Pi HAT - 8 Channel ADC V1.1
 *  Microchip MCP3208 chip
 *
 *  compile:
 *  gcc -lwiringPi -o speedtest speedtest.c
 *
 *  run:
 *  ./speedtest
 *
 *  Updated 04/06/2017
 */

#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <math.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <time.h>

  /* channel 0..7 */
int readADCChannel(chip, channel){
  unsigned char cntrlStack[3];
  
  /* figure 6-1 of MCP3208 datasheet */
  cntrlStack[0] = 6 + ((channel & 4) >> 2);
  cntrlStack[1] = (channel & 3) << 6;
  cntrlStack[2] = 0;

  wiringPiSPIDataRW (chip, cntrlStack, 3);

  return ((cntrlStack[1] & 15) << 8) + cntrlStack[2];
}

int main(int argc, char **argv){
  int    chip = 0; // (jumper CE0 on) chip = 0 (default), (jumper CE1 on) chip = 1
  int    result;
  int    errno;

  if (wiringPiSPISetup (chip, 32000000) < 0){  //this is the maximus Hz, you can down this number if the reading become unstable due to noise.
     fprintf (stderr, "SPI Setup failed: %s\n", strerror (errno));
     exit(1);
  }
  clock_t begin = clock();
  int i=0;
  int num=1000000;
  while (i<num) {  //read the channel 0, num times
     i++;
     result = readADCChannel( chip, 0);
  }
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  double ksps = (double) num / time_spent / 1000;

  fprintf(stdout,"\n\nSampled: %i samples", num); 
  fprintf(stdout,"\n\nTime: %8.6f seconds", time_spent);
  fprintf(stdout,"\n\nSpeed: %8.6f ksps\n\n", ksps); 

  return;
}
