#!/usr/bin/python

import RPi.GPIO as GPIO
import sys
import Adafruit_DHT
import threading
import time
import os
import subprocess

import Adafruit_GPIO.SPI as SPI
import Adafruit_SSD1306

from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont

RST = None     # on the PiOLED this pin isnt used
               # Note the following are only used with SPI:
DC = 23
SPI_PORT = 0
SPI_DEVICE = 0

# GPIO mode pin no.
ledpins=[6,13,19,26]
ledstate=["OFF","OFF","OFF","OFF"]
blinktime=[0,0,0,0]
blinkstatus=[False,False,False,False]
switchpins=[22]
switchstate=["OFF"]
threads=[]
humi=0
temp=0

disp=0
draw=0
width=0
height=0
top=0
bottom=0
font=0
image=0

maxtemp=25
mintemp=25

def initScreen():
    global disp
    global draw
    global width
    global height
    global top
    global bottom
    global font
    global image
    try:
        disp = Adafruit_SSD1306.SSD1306_128_32(rst=RST)
        disp.begin()
        disp.clear()
        disp.display()
        width = disp.width
        height = disp.height
        image = Image.new('1', (width, height))
        draw = ImageDraw.Draw(image)

        # Draw a black filled box to clear the image.
        draw.rectangle((0,0,width,height), outline=0, fill=0)

        # Draw some shapes.
        # First define some constants to allow easy resizing of shapes.
        padding = -2
        top = padding
        bottom = height-padding
        # Move left to right keeping track of the current x position for drawing shapes.
        x = 0
        # Load default font.
        font = ImageFont.load_default()
        draw.rectangle((0,0,width,height), outline=0, fill=0)
        draw.text((x, top+18),    "Init....",  font=font, fill=255)
        disp.image(image)
        disp.display()
    except:
	exit(-1)
    return



def setOut(ledarray):
    for i in range(len(ledarray)):
        GPIO.setup(int(ledarray[i]),GPIO.OUT)
    return

def handleSwitch(switchnum,tnum):
    global ledstate,swithstate,switchpins
    while True:
        if switchstate[tnum] == "ON":
	    GPIO.output(int(switchpins[tnum]),GPIO.LOW)
	    ledstate[2]="ON"
	    ledstate[3]="OFF"
	else:
	    GPIO.output(int(switchpins[tnum]),GPIO.HIGH)
	    ledstate[2]="OFF"
	    ledstate[3]="ON"
	time.sleep(0.1)
    return


def handleLed(lednum,tnum):
    global ledstate,blinkstatus,ledpins
    while True:
	if ledstate[tnum] != "BLINK":
	    if ledstate[tnum] == "ON":
		GPIO.output(int(ledpins[tnum]),True)
	    else:
		GPIO.output(int(ledpins[tnum]),False)
	else:
	    last=blinktime[tnum]
	    now=time.time()
	    if (now - last > 1):
		blinktime[tnum]=now
		blinkstatus[tnum] = not blinkstatus[tnum]
		GPIO.output(int(ledpins[tnum]),blinkstatus[tnum])
	time.sleep(0.1)
    return


def saveData():
    try:
	file = open("/tmp/sensor.txt","w")
	file.write(str(humi) + " ; " + str(temp))
	file.close()
    except:
	print "ooops. File save problem"
    return

def updateRRD():
    return

def handleDHT():
    global humi,temp
    while True:
	ledstate[1]="BLINK"
	humi, temp = Adafruit_DHT.read_retry(11, 17)
#	print 'Temp: {0} C  Humidity: {1} %'.format(temp, humi)
	if humi is not None and temp is not None and humi < 100:
	    ledstate[0]="OFF"
	    ledstate[1]="ON"
	    saveData()
	    updateRRD()
	else:
	    ledstate[0]="ON"
	time.sleep(10)
    return

def handleOled():
    global disp
    global draw
    global width
    global height
    global top
    global bottom
    global font
    global image
    x=0
    while True:
        cmd = "hostname -I | cut -d\' \' -f1"
        IP = subprocess.check_output(cmd, shell = True )

        cmd = "iwconfig wlan0 | grep Link | cut -f2 -d\"=\" | cut -f1 -d\" \""
        LINK = subprocess.check_output(cmd, shell = True )
        QUAL = str.split(LINK,"/")

        draw.rectangle((0,0,width,height), outline=0, fill=0)
        draw.text((x, top),       "Wilgotnosc: " + str(humi) + " %",  font=font, fill=255)
        draw.text((x, top+8),     "Temperatura: " + str(temp) +"\xb0C",  font=font, fill=255)
        draw.text((x, top+16),    "IP: " + str(IP),  font=font, fill=255)
        draw.text((x, top+25),    "Link: " + str(QUAL[0]),  font=font, fill=255)
        if(QUAL[0].isdigit()):
	    draw.rectangle((55,25,55 + int(QUAL[1]),31), outline=1, fill=0)
	    draw.rectangle((55,25,55 + int(QUAL[0]),31), outline=1, fill=255)

        # Display image.
        disp.image(image)
        disp.display()
	time.sleep(5)
    return


def clearScreen():
    draw.rectangle((0,0,width,height), outline=0, fill=0)
    disp.image(image)
    disp.display()
    return

#init GPIO and LEDs
initScreen()
setOut(ledpins)
setOut(switchpins)

#spawn led threads
for i in range(len(ledpins)):
    t = threading.Thread(target=handleLed, args=(ledpins[i],i,))
    threads.append(t)
    t.start()

#spawn switch threads (using list - add more swithces in the future)
for i in range(len(switchpins)):
    t = threading.Thread(target=handleSwitch, args=(switchpins[i],i,))
    threads.append(t)
    t.start()

#spawn dht thread
t = threading.Thread(target=handleDHT)
threads.append(t)
t.start()

#spawn OLED thread
t = threading.Thread(target=handleOled)
threads.append(t)
t.start()


## Main loop here
try:
    while True:
	if temp > maxtemp:
	    switchstate[0]="ON"
	if temp < mintemp:
	    switchstate[0]="OFF"
	print 'Temp: {0} C  Humidity: {1} %'.format(temp, humi)
	time.sleep(1)

except KeyboardInterrupt:
    print "bye"
    disp.clear()
    disp.display()
    time.sleep(2)
    clearScreen()
    GPIO.cleanup()
    os._exit(0)
finally:
    disp.clear()
    disp.display()
    clearScreen()
    time.sleep(2)
    GPIO.cleanup()
    os._exit(0)
