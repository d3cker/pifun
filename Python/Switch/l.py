#!/usr/bin/python

import RPi.GPIO as GPIO ## Import GPIO library
import time
import random


try:
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(35,GPIO.OUT)
    GPIO.output(35,GPIO.LOW)
    time.sleep(1)
    GPIO.output(35,GPIO.HIGH)
    time.sleep(1)



#    while True:
#	randomState(ledlist)
#	time.sleep(0.05)

except KeyboardInterrupt:
    print "\nCTRL+C, exit"
#except:
#    print "oooops wtf"
finally:
    GPIO.cleanup()