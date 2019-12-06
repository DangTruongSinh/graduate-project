from keras.preprocessing.image import img_to_array #dung keras
import imutils
import cv2
import numpy as np
from keras.models import load_model
import os
import serial
import time


dem = 10;
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'

face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')

eye_cascade = cv2.CascadeClassifier('haarcascade_eye.xml')
emotion_model_path = 'model_eye/_mini_XCEPTION.10-1.00.hdf5' #import file da duoc train vao
model = load_model(emotion_model_path, compile=False)
EMOTION = ["open","close"]
cap = cv2.VideoCapture(0)


try: 
      
    if not os.path.exists('dataset_eyes_opencv'): 
        os.makedirs('dataset_eyes_opencv') 

except OSError: 
    print ('Error: Creating directory of data') 
  
while 1:
   
    ret, img = cap.read()
    ret, Khuon_mat = cap.read()
    ret, Mat = cap.read()
    
    
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    gray_Khuon_mat = cv2.cvtColor(Khuon_mat, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.3,minNeighbors=5,minSize=(30,30),flags=cv2.CASCADE_SCALE_IMAGE)
    for (x,y,w,h) in faces:
        cv2.rectangle(img,(x,y),(x+w,y+h),(255,0,0),2)    
        roi_gray_Khuon_mat = gray[y:y+h, x:x+w]
        roi_color_Khuon_mat = img[y:y+h, x:x+w]
        eyes = eye_cascade.detectMultiScale(roi_gray_Khuon_mat, scaleFactor=1.1,minNeighbors=5,minSize=(5,5),flags=cv2.CASCADE_SCALE_IMAGE)
        canvas = np.zeros((40, 150, 3), dtype="uint8")
       
        if len(eyes) > 0:
            eyes = sorted(eyes, reverse=True,
            key=lambda x: (x[2] - x[0]) * (x[3] - x[1]))[0]
            (ex, ey, ew, eh) = eyes
            roi_gray_eye = roi_gray_Khuon_mat[ey:ey+eh, ex:ex+3*ew]
            roi = cv2.resize(roi_gray_eye, (40, 150))
            print (roi.shape)
            frameClone = roi_gray_eye.copy()
            roi = roi.astype("float") / 255.0
            roi = img_to_array(roi)
            roi = np.expand_dims(roi, axis=0)
            cv2.rectangle(roi_color_Khuon_mat,(ex,ey),(ex+4*ew,ey+eh),(0,255,0),2)
        preds = model.predict(roi)[0]
        print('Giá trị dự đoán: ', preds > 0.5)
            
        emotion_probability = np.max(preds)
        label = EMOTION[preds.argmax()]   
   
    for (i, (emotion, prob)) in enumerate(zip(EMOTION, preds)):
        text = "{}: {:.2f}%".format(emotion, prob * 100)
        time.sleep(0.1)  
        if ((len(eyes) > 0) &(len(faces)>0)):
            
           if ((preds > 0.5)== True):
               dem = 10
               print('Giá trị dem: ', dem)
               
           elif (((prob*100)< 15)& (dem > 0) & (( preds > 0.5) == False)):
               dem = dem - 1
               print('Giá trị dem: ', dem)
    
           elif (((prob*100)< 15)& (dem == 0)& ((preds > 0.5)== False)):
                for i in range(3):
                    with serial.Serial('COM4', 9600, timeout=1) as ser:
                        time.sleep(0.2)   
                        ser.write(b'H')   
                        time.sleep(0.1)   
                        ser.write(b'L')   
                dem = 10

        w = int(prob * 150)
        cv2.rectangle(canvas, (7, (i * 35) + 5),
        (w, (i * 35) + 35), (0, 0, 255), -1)
        cv2.putText(canvas, text, (10, (i * 35) + 23),
        cv2.FONT_HERSHEY_SIMPLEX, 0.45,
        (255, 255, 255), 2)
        cv2.putText(frameClone, label, (ex, ey - 10),
        cv2.FONT_HERSHEY_SIMPLEX, 0.45, (0, 0, 255), 2)
        cv2.rectangle(frameClone, (ex, ey), (ex + 3*ew, ey + eh),(0, 0, 255), 2)
    cv2.imshow('img',img)
    cv2.imshow('Mat',roi_gray_eye)
    cv2.imshow('Mat', frameClone)
    cv2.imshow("Probabilities", canvas)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
cap.release()
cv2.destroyAllWindows()
