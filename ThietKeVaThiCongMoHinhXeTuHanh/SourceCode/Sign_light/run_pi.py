import keras
import cv2
import numpy as np

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
import dieu_khien
model = load_model("model.h5")
camera = cv2.VideoCapture(0)
img_crop = []
cout=0
speed = 20
#path ="C:\\Users\\Hung\\Desktop\\sign_light\\dataset1\\img_red\\"
#files =[i for i in os.listdir(path) if i.endswith(".jpg")]
#print(files)
while 1:
    a = time.time()
    _, img = camera.read() 
    img = img[60:240, 320:639]
    img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    #cv2.imshow("img_gray", img_gray)

    img_green = cv2.subtract(img[:,:,1],img_gray)# tach kenh mau RGB 0:Blue 1:Red 2:Green
    img_red = cv2.subtract(img[:,:,2],img_gray) # roi tru cho anh xam ban dau ta duoc 2 vung object
    #cv2.imshow("img_red", img_red)

    ret, thresohd1 = cv2.threshold(img_red, 20, 255, cv2.THRESH_BINARY )#tim nguong cua object mau do,
    ret, thresohd2 = cv2.threshold(img_green, 20, 255, cv2.THRESH_BINARY )# <20: mau den,>20 mau trang==> chuyen thanh anh nhi phan
    threshold = cv2.bitwise_or(thresohd1,thresohd2)# merge 2 anh lai voi nhau  de tim tat ca cac doi tuong tren cung 1 anh
    #Scv2.imshow("thresold",threshold)
    kernel = np.ones((15,15),np.uint8)#kernel de dan no anh 15*15 (tat ca bang 1)
    dilation = cv2.dilate(threshold,kernel,iterations = 1)# Dan no anh, de nhung vung bien bi nhieu khong bi tach ra lam doi
    # Neu nhu 1 vung bien deu la 0 het nhung co 1 gia tri la 1 thi no bang 1 het
    #cv2.imshow("img_dilation", dilation)

    mask,contours,hierarchy = cv2.findContours(dilation, cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)# tim duong bao cua object
    contours = sorted(contours, key = cv2.contourArea, reverse = True)[:1]
    rects = []
    dieu_khien.run(speed)
    print("Run")
    if contours is not None:
        for c in contours:
            peri = cv2.arcLength(c, True)
            approx = cv2.approxPolyDP(c, 0.02 * peri, True)
            x, y, w, h = cv2.boundingRect(approx)

            rect = (x, y, w, h)
            #print(w)
            #print(h)
            if h > 40 and h < 200 and w > 40: 
                #cv2.rectangle(dilation, (x, y), (x+w, y+h), (255), 5)# ve duong bao 
                cv2.rectangle(img, (x, y), (x+w, y+h), (255), 5)
                img_crop = img_red[y:y+h, x:x+w]
                #cv2.imshow("anhnnnnnnnnn", img_crop)

                img_crop = cv2.resize(img_crop, (64,64),interpolation=cv2.INTER_AREA)      
                data = img_crop.reshape(1,64,64,1)
                data = data.astype('float32')

                img_class = model.predict(data)
                print(img_class)
                if img_class.argmax()== 0:
                    cv2.putText(img, 'red light', (x,y), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0),2)
                    print("red_light: dung lai")
                    dieu_khien.stop()
                elif img_class.argmax()== 1:
                    cv2.putText(img, 'green light', (x,y), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0),2)
                    print("green_light: di thang")
                    dieu_khien.run(speed)
                elif img_class.argmax()== 2:
                    cv2.putText(img, 'stop sign', (x,y), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0),2)
                    print("stop_sign: dung lai")  
                    dieu_khien.stop()
                    a = time.time()
                    
                    while True:
                        if (time.time()- a >3):
                            break
                    dieu_khien.run(speed)
    
    #cv2.imshow("anh_binary", dilation)                                                                                             
    cv2.imshow("resulf", img) 

    if cv2.waitKey(10) == 27:
        break
cv2.waitKey(5)
cv2.destroyAllWindows()

