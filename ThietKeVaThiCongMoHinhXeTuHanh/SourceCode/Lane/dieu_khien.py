import RPi.GPIO as GPIO          
from time import sleep

in1 = 24
in2 = 23
in3 = 17
in4 = 27
en1 = 25
en2 = 20
servoPIN = 7


GPIO.setmode(GPIO.BCM)
GPIO.setup(in1,GPIO.OUT)
GPIO.setup(in2,GPIO.OUT)
GPIO.output(in1,GPIO.LOW)
GPIO.output(in2,GPIO.LOW)

GPIO.setup(in3,GPIO.OUT)
GPIO.setup(in4,GPIO.OUT)
GPIO.output(in3,GPIO.LOW)
GPIO.output(in4,GPIO.LOW)

GPIO.setup(en1,GPIO.OUT)
GPIO.setup(en2,GPIO.OUT)

GPIO.setup(servoPIN, GPIO.OUT)
GPIO.setwarnings(False)
servo = GPIO.PWM(servoPIN, 50) # GPIO 17 for PWM with 50Hz
servo.start(8.0) # Initialization

p1=GPIO.PWM(en1,1000)
p1.start(25)
p2=GPIO.PWM(en2,1000)
p2.start(25) 

def run(speed):
    GPIO.output(in1,GPIO.LOW)
    GPIO.output(in2,GPIO.HIGH)
    GPIO.output(in3,GPIO.HIGH)
    GPIO.output(in4,GPIO.LOW)
    p1.ChangeDutyCycle(speed-0)
    p2.ChangeDutyCycle(speed)

def stop():
    GPIO.output(in1,GPIO.LOW)
    GPIO.output(in2,GPIO.LOW)
    GPIO.output(in3,GPIO.LOW) 
    GPIO.output(in4,GPIO.LOW)
    

def queo(radian):
    servo.ChangeDutyCycle(radian)
 
