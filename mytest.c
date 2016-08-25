/*
    gcc mytest.c -o mytest -lwiringPi -lwiringPiDev
    Usage: ./mytest "first line" "2nd line"
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wiringSerial.h>

#define LCD_RS  3               //Register select pin
#define LCD_E   0               //Enable Pin
#define LCD_D4  6               //Data pin 4
#define LCD_D5  1               //Data pin 5
#define LCD_D6  5               //Data pin 6
#define LCD_D7  4               //Data pin 7


int lcd; //global LCD handler


void initLCD() {
  if (lcd = lcdInit (2,16,4,LCD_RS,LCD_E,LCD_D4,LCD_D5,LCD_D6,LCD_D7,0,0,0,0)){
    printf("Failed to init LCD\n");
    exit(-1); // return from void :) ?
   }
}

int main(int argc, char **argv) {
  wiringPiSetup();
  initLCD();
  lcdClear(lcd);
  lcdPuts(lcd,argv[1]);
  lcdPosition(lcd,0,1);
  lcdPuts(lcd,argv[2]);
}
