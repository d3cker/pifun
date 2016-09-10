/*
    gcc mytest2.c -o mytest2 -lwiringPi -lwiringPiDev
    Usage: sudo ./mytest2
    * early WIP *

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

#define TRIG 05
#define DATA 06

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

    pinMode(TRIG,OUTPUT);
    pinMode(DATA,INPUT);

//    initLCD();
//    lcdClear(lcd);
//    pinMode(BUTPINA,INPUT);
//    pinMode(BUTPINB,INPUT);
//    pinMode(BLUELED,OUTPUT);
//    pinMode(REDLED,OUTPUT);
//    pullUpDnControl(BUTPINA, PUD_UP);
//    pullUpDnControl(BUTPINB, PUD_UP);
//    LedsOff();
}


//bieda wersja
void trigMeasure() {
    digitalWrite(TRIG,HIGH);
    usleep(10);
    digitalWrite(TRIG,LOW);
}


int startRead = 0;
int endRead = 0;
int feedBack = 0;
int dist = 0;
struct timeval startT,endT;


void main(int argc, char **argv){
    initMyPi();
    printf("PING\n");  

    while(1) {
    startRead = 0;
    endRead = 0;

    trigMeasure();
	while (endRead < 1) {
	    feedBack = digitalRead(DATA);
	    if(feedBack>0) {
		if(startRead < 1) { 
		    gettimeofday(&startT,NULL);
		    startRead = 1;
		}
	    } else {
		if(startRead > 0) {
		    gettimeofday(&endT,NULL);
		    endRead = 1;	
		}
	    }
	}


    dist = ((endT.tv_usec-startT.tv_usec)*34)/1000/2;
    printf("ECHO: %d [cm]                                        \r",dist);

    printf("\n---------\n");

    sleep(1);
    }
    


}





/*
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
*/