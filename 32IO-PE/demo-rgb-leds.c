/*
 *
 * Required package:
 *  apt-get install libi2c-dev
 *
 * compile:
 *  gcc -lm -o demo-rgb-leds demo-rgb-leds.c
 *
 * see demo-rgb-leds.jpg for wiring instructions
 *
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
#include <signal.h>
#include <time.h>

int fd;
char *fileName = "/dev/i2c-1";

volatile sig_atomic_t fatal_error_in_progress = 0;

int ADDR1 = 0x20; //The I2C address of 1st chip
int ADDR2 = 0x21; //The I2C address of 2nd chip
int DIRA  = 0x00; //PortA I/O direction, by pin. 0=output, 1=input
int DIRB  = 0x01; //PortB I/O direction, by pin. 0=output, 1=input
int PORTA = 0x12; //Register address for PortA
int PORTB = 0x13; //Register address for PortB

int BLACK  = 0;  // 000 (POWER OFF)
int RED    = 1;  // 100
int GREEN  = 2;  // 010
int YELLOW = 3;  // 110
int BLUE   = 4;  // 001
int PINK   = 5;  // 101
int CYAN   = 6;  // 011
int WHITE  = 7;  // 111
int RAND07 = 8;  // ANY (FROM 0 TO 7)
int RAND17 = 9;  // NON-BLACK (FROM 1 TO 7)

unsigned char buf[10];

void EnableLEDS () ;
void DisableLEDS () ;
void SetLEDS (int, int, int, int, int, int, int, int, int, int, double) ;
void fatal_error_signal (int) ;
void cleanup (int) ;

int main(int argc, char **argv) {
    int i, n;
    FILE *fp;

    printf(" LEDs vector library\n");

    signal( SIGINT,  fatal_error_signal);
    signal( SIGTERM, fatal_error_signal);

    EnableLEDS();

    while (1) {

      SetLEDS (1,1,1,1,1,1,1,1,1,1, 500);
      SetLEDS (2,2,2,2,2,2,2,2,2,2, 500);
      SetLEDS (3,3,3,3,3,3,3,3,3,3, 500);
      SetLEDS (4,4,4,4,4,4,4,4,4,4, 500);
      SetLEDS (5,5,5,5,5,5,5,5,5,5, 500);
      SetLEDS (6,6,6,6,6,6,6,6,6,6, 500);
      SetLEDS (7,7,7,7,7,7,7,7,7,7, 500);

      SetLEDS (0,0,0,0,0,0,0,0,0,0, 250);
      SetLEDS (1,1,1,1,1,1,1,1,1,1, 250);
      SetLEDS (0,0,0,0,0,0,0,0,0,0, 250);
      SetLEDS (2,2,2,2,2,2,2,2,2,2, 250);
      SetLEDS (0,0,0,0,0,0,0,0,0,0, 250);
      SetLEDS (4,4,4,4,4,4,4,4,4,4, 250);
      SetLEDS (0,0,0,0,0,0,0,0,0,0, 250);

      SetLEDS (0,0,0,0,0,0,0,0,0,0, 500);
      SetLEDS (9,0,0,0,0,0,0,0,0,0, 500);
      SetLEDS (9,9,0,0,0,0,0,0,0,0, 500);
      SetLEDS (9,9,9,0,0,0,0,0,0,0, 500);
      SetLEDS (9,9,9,9,0,0,0,0,0,0, 500);
      SetLEDS (9,9,9,9,9,0,0,0,0,0, 500);
      SetLEDS (9,9,9,9,9,9,0,0,0,0, 500);
      SetLEDS (9,9,9,9,9,9,9,0,0,0, 500);
      SetLEDS (9,9,9,9,9,9,9,9,0,0, 500);
      SetLEDS (9,9,9,9,9,9,9,9,9,0, 500);
      SetLEDS (9,9,9,9,9,9,9,9,9,9, 500);
      SetLEDS (9,9,9,9,9,9,9,9,9,0, 500);
      SetLEDS (9,9,9,9,9,9,9,9,0,0, 500);
      SetLEDS (9,9,9,9,9,9,9,0,0,0, 500);
      SetLEDS (9,9,9,9,9,9,0,0,0,0, 500);
      SetLEDS (9,9,9,9,9,0,0,0,0,0, 500);
      SetLEDS (9,9,9,9,0,0,0,0,0,0, 500);
      SetLEDS (9,9,9,0,0,0,0,0,0,0, 500);
      SetLEDS (9,9,0,0,0,0,0,0,0,0, 500);
      SetLEDS (9,0,0,0,0,0,0,0,0,0, 500);
      SetLEDS (0,0,0,0,0,0,0,0,0,0, 500);

      SetLEDS (0,0,0,0,0,0,0,0,0,0, 250);
      SetLEDS (1,1,1,1,1,1,1,1,1,1, 250);
      SetLEDS (0,0,0,0,0,0,0,0,0,0, 250);
      SetLEDS (2,2,2,2,2,2,2,2,2,2, 250);
      SetLEDS (0,0,0,0,0,0,0,0,0,0, 250);
      SetLEDS (4,4,4,4,4,4,4,4,4,4, 250);
      SetLEDS (0,0,0,0,0,0,0,0,0,0, 250);

      SetLEDS (0,0,0,0,1,1,0,0,0,0, 500);
      SetLEDS (0,0,0,2,1,1,2,0,0,0, 500);
      SetLEDS (0,0,3,2,1,1,2,3,0,0, 500);
      SetLEDS (0,4,3,2,1,1,2,3,4,0, 500);
      SetLEDS (5,4,3,2,1,1,2,3,4,5, 500);
      SetLEDS (5,4,3,2,0,0,2,3,4,5, 500);
      SetLEDS (5,4,3,0,0,0,0,3,4,5, 500);
      SetLEDS (5,4,0,0,0,0,0,0,4,5, 500);
      SetLEDS (5,0,0,0,0,0,0,0,0,5, 500);
      SetLEDS (0,0,0,0,0,0,0,0,0,0, 500);
      SetLEDS (5,0,0,0,0,0,0,0,0,5, 500);
      SetLEDS (5,6,0,0,0,0,0,0,6,5, 500);
      SetLEDS (5,6,7,0,0,0,0,7,6,5, 500);
      SetLEDS (5,6,7,1,0,0,1,7,6,5, 500);
      SetLEDS (5,6,7,1,2,2,1,7,6,5, 500);
      SetLEDS (5,6,7,1,0,0,1,7,6,5, 500);
      SetLEDS (5,6,7,0,0,0,0,7,6,5, 500);
      SetLEDS (5,6,0,0,0,0,0,0,6,5, 500);
      SetLEDS (5,0,0,0,0,0,0,0,0,5, 500);

      SetLEDS (0,0,0,0,0,0,0,0,0,0, 250);
      SetLEDS (1,1,1,1,1,1,1,1,1,1, 250);
      SetLEDS (0,0,0,0,0,0,0,0,0,0, 250);
      SetLEDS (2,2,2,2,2,2,2,2,2,2, 250);
      SetLEDS (0,0,0,0,0,0,0,0,0,0, 250);
      SetLEDS (4,4,4,4,4,4,4,4,4,4, 250);
      SetLEDS (0,0,0,0,0,0,0,0,0,0, 250);

      SetLEDS (9,9,9,9,9,9,9,9,9,9, 500);
      SetLEDS (9,9,9,9,9,9,9,9,9,9, 500);
      SetLEDS (9,9,9,9,9,9,9,9,9,9, 500);
      SetLEDS (9,9,9,9,9,9,9,9,9,9, 500);
      SetLEDS (9,9,9,9,9,9,9,9,9,9, 500);
      SetLEDS (9,9,9,9,9,9,9,9,9,9, 500);
      SetLEDS (9,9,9,9,9,9,9,9,9,9, 500);
      SetLEDS (9,9,9,9,9,9,9,9,9,9, 500);
      SetLEDS (9,9,9,9,9,9,9,9,9,9, 500);
      SetLEDS (9,9,9,9,9,9,9,9,9,9, 500);
      SetLEDS (9,9,9,9,9,9,9,9,9,9, 500);

      SetLEDS (0,0,0,0,0,0,0,0,0,0, 250);
      SetLEDS (1,1,1,1,1,1,1,1,1,1, 250);
      SetLEDS (0,0,0,0,0,0,0,0,0,0, 250);
      SetLEDS (2,2,2,2,2,2,2,2,2,2, 250);
      SetLEDS (0,0,0,0,0,0,0,0,0,0, 250);
      SetLEDS (4,4,4,4,4,4,4,4,4,4, 250);
      SetLEDS (0,0,0,0,0,0,0,0,0,0, 250);

   }  // del while

   DisableLEDS();

}

void writetoIO(int address, int val1, int val2) {
    ioctl(fd,I2C_SLAVE,address);
    buf[0] = val1;
    buf[1] = val2;
    write(fd, buf, 2);
}

void SetLEDS (int ch0, int ch1, int ch2, int ch3, int ch4, int ch5, int ch6, int ch7, int ch8, int ch9, double t) {
    int AValue1, BValue1, AValue2, BValue2, temp;

    ch0=(ch0==8) ? (rand()%8) : ch0;
    ch1=(ch1==8) ? (rand()%8) : ch1;
    ch2=(ch2==8) ? (rand()%8) : ch2;
    ch3=(ch3==8) ? (rand()%8) : ch3;
    ch4=(ch4==8) ? (rand()%8) : ch4;
    ch5=(ch5==8) ? (rand()%8) : ch5;
    ch6=(ch6==8) ? (rand()%8) : ch6;
    ch7=(ch7==8) ? (rand()%8) : ch7;
    ch8=(ch8==8) ? (rand()%8) : ch8;
    ch9=(ch9==8) ? (rand()%8) : ch9;

    ch0=(ch0==9) ? (rand()%7+1) : ch0;
    ch1=(ch1==9) ? (rand()%7+1) : ch1;
    ch2=(ch2==9) ? (rand()%7+1) : ch2;
    ch3=(ch3==9) ? (rand()%7+1) : ch3;
    ch4=(ch4==9) ? (rand()%7+1) : ch4;
    ch5=(ch5==9) ? (rand()%7+1) : ch5;
    ch6=(ch6==9) ? (rand()%7+1) : ch6;
    ch7=(ch7==9) ? (rand()%7+1) : ch7;
    ch8=(ch8==9) ? (rand()%7+1) : ch8;
    ch9=(ch9==9) ? (rand()%7+1) : ch9;


    printf(" %i  %i  %i  %i  %i  %i  %i  %i  %i  %i  t=%f\n", ch0, ch1, ch2, ch3, ch4, ch5, ch6, ch7, ch8,  ch9, t);

    temp=((((((((ch4<<3)+ch3)<<3)+ch2)<<3)+ch1)<<3)+ch0);
    AValue1=temp&255;
    BValue1=temp>>8;
    temp=((((((((ch9<<3)+ch8)<<3)+ch7)<<3)+ch6)<<3)+ch5);
    AValue2=temp&255;
    BValue2=temp>>8;

    writetoIO(ADDR1,PORTA,AValue1);
    writetoIO(ADDR1,PORTB,BValue1);
    writetoIO(ADDR2,PORTA,AValue2);
    writetoIO(ADDR2,PORTB,BValue2);
    usleep(t*1000);
    //usleep(t*1000000);
}

void EnableLEDS () {
    if ((fd = open(fileName, O_RDWR)) < 0) {
         printf("Failed to open 12c port\n");
         exit(1);
    }
    writetoIO(ADDR1, 0x00, 0x00); // Set PORTA to output on 1st chip
    writetoIO(ADDR1, 0x01, 0x00); // Set PORTB to output on 1st chip
    writetoIO(ADDR2, 0x00, 0x00); // Set PORTA to output on 2nd chip
    writetoIO(ADDR2, 0x01, 0x00); // Set PORTB to output on 2nd chip
    srand(time(NULL));
}

void DisableLEDS () { 
    //Set all outputs to high-impedance by making them inputs
    writetoIO(ADDR1, 0x00, 0xFF);
    writetoIO(ADDR1, 0x01, 0xFF);
    writetoIO(ADDR2, 0x00, 0xFF);
    writetoIO(ADDR2, 0x01, 0xFF);
}

void fatal_error_signal (int sig) {
    if (fatal_error_in_progress)
        raise (sig);
    fatal_error_in_progress = 1;
    //printf(" Sign vector terminated by Ctrl-C\n");
    cleanup(sig);
    signal (sig, SIG_DFL);
    raise (sig);
}

void cleanup (int sig) {
    printf(" Sign vector clenning up\n");
    DisableLEDS();
}