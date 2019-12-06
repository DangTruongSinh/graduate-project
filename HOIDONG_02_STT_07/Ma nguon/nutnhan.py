import RPi.GPIO as GPIO
from time import sleep
import servo
GPIO.setmode(GPIO.BOARD)

ledPin =12
buttonPin = 16
buttonPin1 = 22
GPIO.setup(ledPin,GPIO.OUT)
GPIO.setup(buttonPin,GPIO.IN,pull_up_down=GPIO.PUD_UP)
GPIO.setup(buttonPin1,GPIO.IN,pull_up_down=GPIO.PUD_UP)

box = servo.Box()
while True:
     buttonState = GPIO.input(buttonPin)
     buttonState1 = GPIO.input(buttonPin1)
     if buttonState == False:
         GPIO.output(ledPin, GPIO.HIGH)
         box.lock()
         
     else:
         GPIO.output(ledPin, GPIO.LOW)
         
     if buttonState1 == False:
         GPIO.output(ledPin, GPIO.HIGH)
         box.unlock()
         
     else:
         GPIO.output(ledPin, GPIO.LOW)