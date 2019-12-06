import sys
import time
import cv2
from Tkinter import  *
import Tkinter as tk
import RPi.GPIO as GPIO
from PIL import ImageTk, Image
import threading
import numpy as np
import glob, os
import subprocess
from PIL import Image
import sqlite3
import servo

GPIO.setmode(GPIO.BOARD)
GPIO.setwarnings(False)
GPIO.setup(18, GPIO.OUT)

box = servo.Box()
            
def getImagesWithID(path):
	imagePaths = [os.path.join(path,f) for f in os.listdir(path)]
	faces = []
	IDs = []
	for imagePath in imagePaths:
            try:
		faceImg = Image.open(imagePath).convert('L')
		faceNp = np.array(faceImg,'uint8')
		ID = int(os.path.split(imagePath)[-1].split('.')[1])
		faces.append(faceNp)
		IDs.append(ID)
	    except:
                continue
	return np.array(IDs), faces


def face_check():
        face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
        recognizer = cv2.face.LBPHFaceRecognizer_create()
        recognizer.read('recognizer/trainingData.yml')
        path = 'dataset'
        box = servo.Box()
        kq=''
         
        print ('Check face')
        conn = sqlite3.connect('database.db')
        c = conn.cursor()
        temp=0
        have_face=0
        cap = cv2.VideoCapture(0)
        ret, img = cap.read()
        
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        faces = face_cascade.detectMultiScale(gray, 1.2, 8)
                            
        for (x,y,w,h) in faces:
            try:
                cv2.rectangle(img,(x,y-25),(x+w,y+h+25),(0,255,0),3)
                ids,conf = recognizer.predict(gray[y-25:y+h+25,x:x+w])
                cv2.imwrite("1.jpg",gray[y-20:y+h+20,x+5:x+w-5])
                c.execute('select name from users where id = (?);', (ids,))
                result = c.fetchall()
                name = result[0][0]
                conf=round(100-conf,2)
                have_face=conf
                if (conf >= 63):
                    if conf>temp:
                        temp=conf
                        kq=name
            except:
                print("error")
        cap.release()
        if (kq!=''):
            box.lock()
            print('Nhan dang khuon mat ',kq)
            kq=''
                    
        elif have_face==0:
            print('khong co khuon mat')
            box.alarm_on()
            time.sleep(3)
            box.alarm_off()
        else:
            print('khong nhan dang duoc')
            box.alarm_on()
            time.sleep(3)
            box.alarm_off()
        conn.close()


def add_face():

        face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
        
        recognizer = cv2.face.LBPHFaceRecognizer_create()
        
        path = 'dataset'
        uname = 'Master'        
        
        if not os.path.exists('./recognizer'):
            os.makedirs('./recognizer')
         
        
        if not os.path.exists('./dataset'):
            os.makedirs('./dataset')
        print ('add face')
        
        conn = sqlite3.connect('database.db')
        c = conn.cursor()
        c.execute('INSERT INTO users (name) VALUES (?)', (uname,))
        uid = c.lastrowid
        
        cap = cv2.VideoCapture(0)
                
        sampleNum = 0
        while True:
                ret, img = cap.read()
                gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
                faces = face_cascade.detectMultiScale(gray, 1.2, 8)
                for (x,y,w,h) in faces:
                  try:
                       sampleNum = sampleNum+1
                       cv2.imwrite("dataset/User."+str(uid)+"."+str(sampleNum)+".jpg",gray[y-20:y+h+20,x+5:x+w-5])
                       time.sleep(0.02)
                  except:
                       break
                                  
                if sampleNum >= 30:
                  break
        cap.release()
        conn.commit()
        conn.close()
        time.sleep(2)


def train_face():
    

        face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
        
        recognizer = cv2.face.LBPHFaceRecognizer_create()
        
        path = 'dataset'
        
        
        
        if not os.path.exists('./recognizer'):
            os.makedirs('./recognizer')
         
        
        if not os.path.exists('./dataset'):
            os.makedirs('./dataset')
        print ('trainning')
            
                                     
        Ids, faces = getImagesWithID(path)
        recognizer.train(faces,Ids)
        recognizer.write('recognizer/trainingData.yml')
def BAT():
    GPIO.output(18, GPIO.HIGH)    
def TAT():
    GPIO.output(18, GPIO.LOW)

def change_pic():
    photo1 = '1.jpg'
    image = Image.open(photo1)
    image = image.resize((60, 60), Image.ANTIALIAS)
    root.photo1 = ImageTk.PhotoImage(image)
    vlabel.configure(image=root.photo1)

root = tk.Tk()

pt = ImageTk.PhotoImage(Image.open('camera.png'))
pt1 = ImageTk.PhotoImage(Image.open('nhandang.png'))
pt2 = ImageTk.PhotoImage(Image.open('update.png'))
pt3 = ImageTk.PhotoImage(Image.open('off.png'))
pt4 = ImageTk.PhotoImage(Image.open('on.png'))
pt5 = ImageTk.PhotoImage(Image.open('train.png'))
pt6 = ImageTk.PhotoImage(Image.open('door.png'))
photo = 'd.jpg'
image = Image.open(photo)
image = image.resize((60, 60), Image.ANTIALIAS)
root.photo = ImageTk.PhotoImage(image)

vlabel=tk.Label(root,image=root.photo)
vlabel.pack()
root.title("Nhan Dang Khuon Mat")
Nut_bat = tk.Button(root, image = pt4, text = "BAT DEN", command = TAT)
Nut_tat = tk.Button(root, image = pt3, text = "TAT DEN", command = BAT)
Nut_bat.pack()
Nut_tat.pack()
button0 = tk.Button(root, text = 'cap nhat', image = pt2, command = change_pic)
button0.pack()
button1 = tk.Button(root, text = 'Nhan dang', image = pt1 ,command = face_check)
button1.pack()
button2 = tk.Button(root, text = 'Them khuon mat', image = pt ,command = add_face)
button2.pack()
button3 = tk.Button(root, text = 'Train du lieu', image = pt5, command = train_face)
button3.pack()
#button4 = tk.Button(root, text = 'Khoa cua', height = 3, width = 80, command = box.lock)
#button4.pack()
button5 = tk.Button(root, text = 'Mo cua', image = pt6, command = box.unlock)
button5.pack()

root.mainloop()

 



