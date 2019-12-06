import cv2
import os
import numpy as np
import tensorflow as tf
from keras.models import load_model
from keras.preprocessing import image

os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'
tf.logging.set_verbosity(tf.logging.ERROR)

def nothing(x):
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

cam = cv2.VideoCapture(0)
model = load_model('Trained_model.h5')
print("--> Model Loaded")
img_text =""
while cam.isOpened():
    ret,img = cam.read()
    flip_img = cv2.flip(img,1)
    img =cv2.rectangle(flip_img, (300,50),(550,300), (255,0,0),3)
    imgcrop = img[50:300,300:550]
    hsv= cv2.cvtColor(imgcrop,cv2.COLOR_BGR2YCR_CB)
    blur = cv2.GaussianBlur(hsv,(11,11),0)
    skin_min = np.array((0, 138, 67))
    skin_max = np.array((255, 173, 133))
    mask = cv2.inRange(blur, skin_min, skin_max)
    contours,hierarchy = cv2.findContours(mask.copy(),cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
    cnt=getMaxContour(contours,4000)
    if cnt is not None:
        x1,y1,w1,h1 = cv2.boundingRect(cnt)
        save_img=imgcrop[y1:y1+h1,x1:x1+w1]
        save_img=cv2.bitwise_and(save_img,save_img,mask=mask[y1:y1+h1,x1:x1+w1])
        save_img=cv2.resize(save_img,(200,200))
        cv2.imshow("Test",save_img)
        cv2.imwrite('Test.jpg',save_img )

        test_image = image.load_img('Test.jpg', target_size=(64, 64))
        test_image = image.img_to_array(test_image)
        test_image = np.expand_dims(test_image, axis = 0)
        result = model.predict(test_image)

        if result[0][0] ==1:
            img_text='A'
        elif result[0][1] ==1:
            img_text='B'
        elif result[0][2] ==1:
            img_text='C'
        elif result[0][3] ==1:
            img_text ='D'
        elif result[0][4] ==1:
            img_text ='E'
        elif result[0][5] ==1:
            img_text ='I'
        elif result[0][6] ==1:
            img_text ='L'
        elif result[0][7] ==1:
            img_text ='O'
        elif result[0][8] ==1:
            img_text ='U'
        elif result[0][9] ==1:
            img_text ='V'
        else:
           img_text='No Data'
        cv2.putText(img, img_text, (350, 350), cv2.FONT_HERSHEY_TRIPLEX,2, (0,0, 255))
        print(img_text)
        
    cv2.imshow("test", img)
    cv2.imshow("mask", mask)
    
    if cv2.waitKey(1) == 27:
        break

cam.release()
cv2.destroyAllWindows()
