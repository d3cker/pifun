/*
    gcc mytest3.c -o mytest3 -lwiringPi -lwiringPiDev
    Usage: sudo ./mytest3

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

//sonar stuff
int startRead = 0;
int endRead = 0;
int feedBack = 0;
int dist = 0;
struct timeval startT,endT;
struct timeval startEcho,lastEcho;

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
    pinMode(TRIG,OUTPUT);
    pinMode(DATA,INPUT);
    LedsOff();
}


void trigMeasure() {
    digitalWrite(TRIG,HIGH);
    usleep(10);
    digitalWrite(TRIG,LOW);
}


int dataRead(){
    while (!endRead) {
        feedBack = digitalRead(DATA);
        if(feedBack) {
	    if(!startRead) { 
	        gettimeofday(&startT,NULL);
	        startRead = 1;
	    }
	} else {
	    if(startRead) {
	        gettimeofday(&endT,NULL);
	        endRead = 1;
	    }
	}
    }
    // from doc
    // distance [cm] = ( high level time [us] * 34 ) / 1000 / 2 
    dist = ((endT.tv_usec-startT.tv_usec)*34)/1000/2;
    if(dist>3000 || dist<2) dist=0;
    return(dist);
}


int doSonar() {
    startRead = 0;
    endRead = 0;
    trigMeasure();
    return dataRead();
}


void clearLine(int line) {
    lcdPosition(lcd,0,line);
    lcdPuts(lcd,"                ");
}

void main(int argc, char **argv) {
int bar,maxBar;
char cBar[16];

    initMyPi();
    while(1) {
	if(!BS_Status) {
	    lcdPosition(lcd,0,0);
	    if(digitalRead(BUTPINB)) {
		lcdPuts(lcd,"* L = SONAR *   ");
		RS_Status = 0;
		digitalWrite(REDLED, LOW);
	    } else {
		lcdPosition(lcd,0,0);
		digitalWrite(REDLED, HIGH);
		if (!RS_Status) {
		    lcdClear(lcd);
		    gettimeofday(&startEcho,NULL);
		    RS_Status = !RS_Status;
		}
		lcdPuts(lcd,"DIST: ");
		if(lastEcho.tv_sec - startEcho.tv_sec > 1) {
		    //draw Distance
		    digitalWrite(BLUELED, HIGH); 
		    lcdPrintf(lcd,"%d [cm]  ",doSonar());
		    startEcho.tv_sec = lastEcho.tv_sec;
		    digitalWrite(BLUELED, LOW); 

		    //draw Bar 
		    clearLine(1);
		    lcdPosition(lcd,0,1);
		    maxBar = ((dist*16)/200); //from docs , max distance 2m
		    if(maxBar>16) maxBar=16;
		    if(maxBar<0) maxBar = 0;
		    bzero(&cBar,sizeof(cBar));
		    for (bar=0; bar< maxBar;bar++) cBar[bar]='#';
		    lcdPrintf(lcd,"%s",cBar);

		}
		gettimeofday(&lastEcho.tv_sec,NULL);
	    }
	}// END of left switch

	lcdPosition(lcd,0,1);
	if(!RS_Status) {
	    if(digitalRead(BUTPINA)) {
		lcdPuts(lcd,"* R = getIP *  ");
		BS_Status = 0;
	    } else {
		lcdPuts(lcd,"BS: ON  :: BL: ");
		if (!BS_Status) {
		    lcdClear(lcd);
		    BS_Status = !BS_Status;
		}
	    }
	} // END of right switch
    }// END of main while() loop
}
