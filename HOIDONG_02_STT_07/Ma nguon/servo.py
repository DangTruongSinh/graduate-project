import RPi.GPIO as GPIO
from time import sleep
GPIO.setmode(GPIO.BOARD)
GPIO.setup(11, GPIO.OUT)
GPIO.setup(13, GPIO.OUT)

class Box(object):
    
        def lock(self):
                pwm = GPIO.PWM(11,240)
                pwm.start(0)
                duty = 180 / 18 + 0
                pwm.ChangeDutyCycle(duty)
                sleep(2)
                pwm.stop(0 / 18 + 40)
            
        def unlock(self):
                pwm = GPIO.PWM(11,240)
                pwm.start(0 / 18 + 40)
                duty1 = 0 / 18 + 40
                pwm.ChangeDutyCycle(duty1)
                sleep(2)
                pwm.stop()

        
        def alarm_on(self):
                GPIO.output(13, GPIO.HIGH)
        
        def alarm_off(self):
                GPIO.output(13, GPIO.LOW)

