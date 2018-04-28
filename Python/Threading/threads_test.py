#!/usr/bin/python

import RPi.GPIO as GPIO ## Import GPIO library
import threading
import time
import os


leds=[31,33,35,37]
blinktime=[0,0,0,0]
blinkstatus=[False,False,False,False]
zmienna=["ON","ON","ON","ON"]


threads=[]


def setOut(ledarray):
    for i in range(len(ledarray)):
        GPIO.setup(int(ledarray[i]),GPIO.OUT)
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


#spawn led threads
for i in range(len(leds)):
    t = threading.Thread(target=handleLed, args=(leds[i],i,))
    threads.append(t)
    t.start()


try:
    while True:
	zmienna[0]="BLINK"
	zmienna[1]="BLINK"
	zmienna[2]="BLINK"
	zmienna[3]="BLINK"
	time.sleep(1)
	zmienna[0]="BLINK"
	zmienna[1]="BLINK"
	zmienna[2]="BLINK"
	zmienna[3]="BLINK"
	time.sleep(1)
except KeyboardInterrupt:
    print "bye"
    GPIO.cleanup()
    os._exit(0)
finally:
    GPIO.cleanup()
