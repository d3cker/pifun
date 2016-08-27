/*
    gcc mytest.c -o mytest -lwiringPi -lwiringPiDev
    Usage: sudo ./mytest "first line" "2nd line"
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wiringPi.h>

#define LCD_RS  22             //Register select pin
#define LCD_E   17             //Enable Pin
#define LCD_D4  18             //Data pin 4
#define LCD_D5  25             //Data pin 5
#define LCD_D6  24             //Data pin 6
#define LCD_D7  23             //Data pin 7

int lcd; //global LCD handler


void initLCD() {
  if (lcd = lcdInit (2,16,4,LCD_RS,LCD_E,LCD_D4,LCD_D5,LCD_D6,LCD_D7,0,0,0,0)){
    printf("Failed to init LCD\n");
    exit(-1); // return from void :) ?
   }
}

int main(int argc, char **argv) {
  wiringPiSetupGpio();
  initLCD();
  lcdClear(lcd);
  lcdPuts(lcd,argv[1]);
  lcdPosition(lcd,0,1);
  lcdPuts(lcd,argv[2]);
}
