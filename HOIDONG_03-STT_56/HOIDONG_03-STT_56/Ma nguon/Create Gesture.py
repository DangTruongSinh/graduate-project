import cv2
import numpy as np
import os
import time

path="TrainData"
cap=cv2.VideoCapture(0)
num=0
def nothing(x) :
    pass

def getMaxContour(contours,minArea=200):
    maxC=None
    maxArea=minArea
    for cnt in contours:
        area=cv2.contourArea(cnt)
        if(area>maxArea):
            maxArea=area
            maxC=cnt
    return maxC

if not os.path.exists(path):
    os.mkdir(path)

name=input('Nhap ten thu muc:')
try:
    os.mkdir(path+"/"+str(name))
except:
     pass
while(cap.isOpened()):
    ret,frame = cap.read()
    frame =cv2.flip(frame,1)
    img = cv2.rectangle(frame,(350,50),(600,300),(0,255,0),3)
    skin_min = np.array((0, 138, 67))
    skin_max = np.array((255, 173, 133))
    imgcrop= img[50:300,350:600]
    img_ycrcb = cv2.cvtColor(imgcrop, cv2.COLOR_BGR2YCR_CB)
    blur=cv2.GaussianBlur(img_ycrcb,(11,11),0) 
    mask = cv2.inRange(blur, skin_min, skin_max)
    contours,hierarchy = cv2.findContours(mask.copy(),cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
    cnt=getMaxContour(contours,4000)
    if num < 1500:
        if cnt is not None:
            x1,y1,w1,h1 = cv2.boundingRect(cnt)
            imgT=imgcrop[y1:y1+h1,x1:x1+w1]
            imgT=cv2.bitwise_and(imgT,imgT,mask=mask[y1:y1+h1,x1:x1+w1])
            imgT=cv2.resize(imgT,(250,250))
            cv2.imshow('Trainer',imgT)
            dirs=path+"/"+str(name)+"/"+str(num)+".jpg"
            cv2.imwrite(dirs,imgT)
            num =num +1 
            print(num)
    else: break
            
    cv2.imshow("Frame",img)
    cv2.imshow("Mask",mask)
    keypress=cv2.waitKey(1)
    if keypress==27:
        break
        
cap.release()
cv2.destroyAllWindows()
