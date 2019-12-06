#!/usr/bin/env python

""" A simple continuous receiver class. """

# Copyright 2015 Mayer Analytics Ltd.
#
# This file is part of pySX127x.
#
# pySX127x is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
# License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# pySX127x is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
# warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
# details.
#
# You can be released from the requirements of the license by obtaining a commercial license. Such a license is
# mandatory as soon as you develop commercial activities involving pySX127x without disclosing the source code of your
# own applications, or shipping pySX127x with a closed source product.
#
# You should have received a copy of the GNU General Public License along with pySX127.  If not, see
# <http://www.gnu.org/licenses/>.
import datetime
# Import OpenCV2 for image processing
import cv2

import serial
# Import numpy for matrices calculations
import numpy as np
from time import sleep
import json
import packer
import time
import sys
sys.path.insert(0, '../../pySX127x')        
from SX127x.LoRa import *
from SX127x.board_config import BOARD

ser = serial.Serial('/dev/ttyUSB0',9600)
# Create Local Binary Patterns Histograms for face recognization
recognizer = cv2.face.LBPHFaceRecognizer_create()

# Load the trained mode
recognizer.read('trainer/trainer.yml')

# Load prebuilt model for Frontal Face
cascade_facePath = "haarcascade_frontalface_default.xml"
cascade_eyePath = "haarcascade_eye.xml"
# Create classifier from prebuilt model
faceCascade = cv2.CascadeClassifier(cascade_facePath)
eyeCascade = cv2.CascadeClassifier(cascade_eyePath)
# Set the font style
font = 2
class LoRaBeacon(LoRa):
    def __init__(self, verbose=False):
        super(LoRaBeacon, self).__init__(verbose)
        self.set_mode(MODE.SLEEP)
        self._id = "3"
        self.rx_done = False

    def on_rx_timeout(self):
        print("\non_RxTimeout")
        print(self.get_irq_flags())

##    def on_rx_done(self):
##        self.clear_irq_flags(RxDone=1)
##        payload = self.read_payload(nocheck=True)
##        data = ''.join([chr(c) for c in payload])
##
##        if data is not None and len(data):
##            try:
##                _length, _data = packer.Unpack_Str(data)
##                print "Time:", str(time.ctime()) 
##                #print "Rawinput:", payload
##                print "Receive:", _data
##            except:
##                print "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
##                print "Non-hexadecimal digit found..."
##                print "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
##                print "Receive:", data 
##
##        self.set_mode(MODE.SLEEP)
##        #self.reset_ptr_rx()
##        #self.set_mode(MODE.RXCONT)
##        self.rx_done = True


    def start(self):
        while True:
            print '----------------------------------'      
            ##########
            # Initialize and start the video frame capture
            #cam = cv2.VideoCapture(0)
            flag = "0"
            dem = 0
            count =0
            vantay = "0"
            add = "0"
            delete = "0"
            cam = cv2.VideoCapture(0)
##            cam.set(cv2.CAP_PROP_FPS, 30)
            while True:
                while ser.in_waiting:
                    read_serial=ser.readline()
                    s = str(int (read_serial,16))
                    flag = "1"
                    print s
                    if s=="222":#quet
                        read_serial=ser.readline()
                        vantay = str(int (read_serial,16))
                        print vantay
                    elif s=="555":#them
                        read_serial=ser.readline()
                        vantay = str(int (read_serial,16))
                        add = "1"

                        print vantay
                    elif s=="444":#xoa
                        read_serial=ser.readline()
                        vantay = str(int (read_serial,16))
                        delete = "1"
                        print vantay
                    
                
                # Read the video frame
                ret, im =cam.read()

                # Convert the captured frame into grayscale
                gray = cv2.cvtColor(im,cv2.COLOR_BGR2GRAY)

                # Get all face from the video frame
                faces = faceCascade.detectMultiScale(gray, 1.2,5)

                # For each face in faces
                for(x,y,w,h) in faces:
                    gray_face=gray[y: y+h,x: x+w]
                    eyes = eyeCascade.detectMultiScale(gray_face)
                    for(ex,ey,ew,eh) in eyes:
            
                        # Create rectangle around the face
                        cv2.rectangle(im, (x-20,y-20), (x+w+20,y+h+20), (0,255,0), 4)

                        # Recognize the face belongs to which ID
                        Id = recognizer.predict(gray[y:y+h,x:x+w])

                        # Check the ID if exist 
                        if (Id[0] == 1) and (Id[1] <= 80):
                            Id_reg = "KHANG"
                            dem = 0
                        else:    
                            Id_reg = "UNKNOWN"  
                            currentDT = datetime.datetime.now()
                            #cv2.imwrite("Unknown_person_data/Unknown.jpg",gray_face)
                            cv2.imwrite("Unknown_person_data/Unknown." + str(currentDT.strftime("%H-%M-%S")) + '_' + str(currentDT.strftime("%d-%m-%Y")) + ".jpg", gray_face)
                            dem+=1
                        
                        
                        
                        # Put text describe who is in the picture
                        #cv2.rectangle(im, (x-22,y-90), (x+w+22, y-22), (0,0,255), -1)
                        #cv2.putText(im, str(Id_reg), (x,y-40), font, 1, (255,255,255), 2)
                # Display the video frame with the bounded rectangle
                cv2.imshow('Face_Recognition',im) 
                # If 'q' is pressed, close program
                if dem >= 30:
                    camera = "1"
                    flag = "2"
                    count = 0
                else:
                    if count >= 50:
                        count = 0
                        dem = 0
                    else:
                        count += 1
##                    
                print("gia tri dem: " , dem )    
                print("gia tri flag:" + flag )
                
                if cv2.waitKey(10) & 0xFF == ord('q') or flag == "1" or flag == "2":
                    break

            # Stop the camera
            cam.release()

            # Close all windows
            cv2.destroyAllWindows()
            ##########
            try:
                if flag == "1":
                    rawinput = vantay
                    print rawinput 
                elif flag == "2":
                    rawinput = camera
            except KeyboardInterrupt:
                lora.set_mode(MODE.SLEEP)
                BOARD.teardown()
                print "exit()"


            if len(rawinput) < 200:
                self.set_mode(MODE.STDBY)
                self.clear_irq_flags(TxDone=1)
                if flag == "1":
                    data = {"d":rawinput,"del":delete,"a":add,"ty":flag}
                elif flag == "2":
                    data = {"d":rawinput,"ty":flag}
                _length, _payload = packer.Pack_Str( json.dumps(data) )
                print "Rawinput:", data
                data = [int(hex(ord(c)), 0) for c in _payload]
                print "data", _payload

                sleep(1)
                self.write_payload(data)                                       
                self.set_mode(MODE.TX)

                sleep(.5)
                self.set_mode(MODE.SLEEP)
                self.set_dio_mapping([0] * 6)
                sleep(.5)
                self.set_mode(MODE.STDBY)
                sleep(.5)
                self.reset_ptr_rx()
                self.set_mode(MODE.RXCONT)

                for _ in range(t):
                    sleep(.1)

                    if self.rx_done == True:
                        self.rx_done = False
                        break


#    
# initialize sx1278
#    
BOARD.setup()

sf = 7
bw = 8
cr = 1
t = sf * bw * cr

lora = LoRaBeacon()
lora.set_mode(MODE.SLEEP)
lora.set_pa_config(pa_select=1)
lora.set_freq(433)
lora.set_spreading_factor(sf)  # 7-12
lora.set_bw(bw)  # 0-9
lora.set_coding_rate(cr)  # 1-4
lora.clear_irq_flags(TxDone=1)
print(lora)

try:
    lora.start()
finally:
    lora.set_mode(MODE.SLEEP)
    BOARD.teardown()
    print "exit()"
