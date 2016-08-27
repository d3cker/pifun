/*
    gcc mytest2.c -o mytest2 -lwiringPi -lwiringPiDev
    Usage: sudo ./mytest2

    Swith:
    GPIO27(GEN2) ------+---[10k]----3.3v 
                       |
                       * \*---------|:

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wiringPi.h>


//LCD SetUp
#define LCD_RS  22             //Register select pin
#define LCD_E   17             //Enable Pin
#define LCD_D4  18             //Data pin 4
#define LCD_D5  25             //Data pin 5
#define LCD_D6  24             //Data pin 6
#define LCD_D7  23             //Data pin 7

//Switches and LEDs SetUp
#define BUTPINA 21
#define BUTPINB 20
#define BLUELED 12
#define REDLED 16

int lcd; //global LCD handler
int RedStatus;   //RED LED Status
int BlueStatus;  //BLUE LED status
int RS_Status;   //RED switch status
int BS_Status;   //BLUE switch status

void initLCD() {
  if (lcd = lcdInit (2,16,4,LCD_RS,LCD_E,LCD_D4,LCD_D5,LCD_D6,LCD_D7,0,0,0,0)){
    printf("Failed to init LCD\n");
    exit(-1); // return from void :) ?
   }
}

void LedsOff() {
    RedStatus = LOW;
    BlueStatus = LOW;
}

void initMyPi() {
    wiringPiSetupGpio();
    initLCD();
    lcdClear(lcd);
    pinMode(BUTPINA,INPUT);
    pinMode(BUTPINB,INPUT);
    pinMode(BLUELED,OUTPUT);
    pinMode(REDLED,OUTPUT);
    pullUpDnControl(BUTPINA, PUD_UP);
    pullUpDnControl(BUTPINB, PUD_UP);
    LedsOff();
}

void main(int argc, char **argv) {
    initMyPi();
    while(1) {
	lcdPosition(lcd,0,0);
	if(digitalRead(BUTPINB)) {
	    lcdPuts(lcd,"RS: OFF :: RL: ");
	    RS_Status = 0;
	} else {
	    lcdPuts(lcd,"RS: ON  :: RL: ");
	    if (!RS_Status) {
		RedStatus = !RedStatus;
		RS_Status = !RS_Status;
	    }
	}
	lcdPrintf(lcd,"%d",RedStatus);
	lcdPosition(lcd,0,1);
	if(digitalRead(BUTPINA)) {
	    lcdPuts(lcd,"BS: OFF :: BL: ");
	    BS_Status = 0;
	} else {
	    lcdPuts(lcd,"BS: ON  :: BL: ");
	    if (!BS_Status) {
		BlueStatus = !BlueStatus;
		BS_Status = !BS_Status;
	    }
	}
	lcdPrintf(lcd,"%d",BlueStatus);
	//should we sleep here for a while ?
	digitalWrite(REDLED, RedStatus);
	digitalWrite(BLUELED, BlueStatus);
    }
}
