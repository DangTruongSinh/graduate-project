import keras
import cv2
import numpy as np

import RPi.GPIO as GPIO          
from time import sleep

import time
from keras.models import Sequential, load_model
from keras.layers import Dense,Dropout, Flatten
from keras.layers import Conv2D
from keras import backend as K
from keras.utils import np_utils
from matplotlib import pyplot as plt
import matplotlib
matplotlib.use('TkAgg')
import os
import cv2
import numpy as np
import dieu_khien

camera = cv2.VideoCapture(0)
model = load_model('/home/pi/Desktop/test/Model.h5')
img_crop = []
speed =20

while 1:

    _, img = camera.read()
    img_crop = img[250:479, 0:639]
    img_crop = cv2.resize(img, (200,75),interpolation=cv2.INTER_AREA)
           
    data = img_crop.reshape(1,75,200,3)
    data = data.astype('float32')

    img_class = model.predict(data)
    if img_class.argmax()== 0:
        dieu_khien.run(speed)
        dieu_khien.queo(8.0)
        print("di thang")

    elif img_class.argmax()== 1:  
        dieu_khien.run(speed)
        dieu_khien.queo(7.5)
        print("trai1")

    elif img_class.argmax()== 2:  
        dieu_khien.run(speed)
        dieu_khien.queo(7.0)
        print("trai2")

    elif img_class.argmax()== 3:
        dieu_khien.run(speed)
        dieu_khien.queo(8.5)
        print("pha1")
    
    elif img_class.argmax()== 4:
        dieu_khien.run(speed)
        dieu_khien.queo(9.0)
        print("phai2")
    

cv2.waitKey(0)
cv2.destroyAllWindows()

