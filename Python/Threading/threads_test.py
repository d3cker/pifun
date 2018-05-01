#!/usr/bin/python

import RPi.GPIO as GPIO ## Import GPIO library
import threading
import time
import os


leds=[31,33,35,37]
blinktime=[0,0,0,0]
blinkstatus=[False,False,False,False]
zmienna=["OFF","OFF","OFF","OFF"]
switchpin=[15]
switchstate=["OFF"]

threads=[]


def setOut(ledarray):
    for i in range(len(ledarray)):
        GPIO.setup(int(ledarray[i]),GPIO.OUT)
    return

def handleSwitch(switchnum,tnum):
    while True:
        if switchstate[tnum] == "ON":
	    GPIO.output(int(switchpin[tnum]),GPIO.LOW)
	    zmienna[2]="ON"
	    zmienna[3]="OFF"
	else:
	    GPIO.output(int(switchpin[tnum]),GPIO.HIGH)
	    zmienna[2]="OFF"
	    zmienna[3]="ON"
	time.sleep(0.1)
    return



def handleLed(lednum,tnum):
    while True:
#	print " Thread [" + str(tnum) + "] :: Led [" + str(lednum) + "] :: Status [" + zmienna[tnum] + "]"
	if zmienna[tnum] != "BLINK":
	    if zmienna[tnum] == "ON":
		GPIO.output(int(leds[tnum]),True)
	    else:
		GPIO.output(int(leds[tnum]),False)
	else:
	    last=blinktime[tnum]
	    now=time.time()
	    if (now - last > 1):
		blinktime[tnum]=now
		blinkstatus[tnum] = not blinkstatus[tnum]
		GPIO.output(int(leds[tnum]),blinkstatus[tnum])
	time.sleep(0.1)
    return

#init GPIO and LEDs
GPIO.setmode(GPIO.BOARD)
setOut(leds)
setOut(switchpin)

#spawn led threads
for i in range(len(leds)):
    t = threading.Thread(target=handleLed, args=(leds[i],i,))
    threads.append(t)
    t.start()

#spawn switch threads
for i in range(len(switchpin)):
    t = threading.Thread(target=handleSwitch, args=(switchpin[i],i,))
    threads.append(t)
    t.start()

zmienna[0]="BLINK"
zmienna[1]="OFF"
zmienna[2]="OFF"
zmienna[3]="OFF"

try:
    while True:
	switchstate[0]="OFF"
	time.sleep(0.5)
	switchstate[0]="ON"
	time.sleep(0.5)
except KeyboardInterrupt:
    print "bye"
    GPIO.cleanup()
    os._exit(0)
finally:
    GPIO.cleanup()
