from keras.preprocessing.image import img_to_array
import cv2
from keras.models import load_model
import numpy as np
import time
import PIL.Image, PIL.ImageTk
import numpy as np
from tkinter import*
def emotion_image(img_org):
         frame = cv2.imread(img_org)
         detection_model_path = 'C:/Users/Minh Hieu Hoang Thi/AppData/Local/Programs/Python/Python35/src/cascades/data/haarcascade_frontalface_default.xml'
         emotion_model_path = 'E:/HUMAN_E/_mini_XCEPTION.102-0.66.hdf5'
         face_detection = cv2.CascadeClassifier(detection_model_path)
         emotion_classifier = load_model(emotion_model_path, compile=False)
         EMOTIONS = ["ANGRY" ,"DISGUST","SCARED", "HAPPY", "SAD", "SUPRISED","NEUTRAL"]
         
         gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
         faces = face_detection.detectMultiScale(gray,scaleFactor=1.1,minNeighbors=5,minSize=(30,30),flags=cv2.CASCADE_SCALE_IMAGE)
         xac_suat = np.zeros((250, 170,3), dtype="uint8")
         xac_suat[:,:170] = [239, 239, 239]
         if len(faces) > 0:
               faces = sorted(faces, reverse=True,key=lambda x: (x[2] - x[0]) * (x[3] - x[1]))[0]
               (fX, fY, fW, fH) = faces
               roi = gray[fY:fY + fH, fX:fX + fW]
               roi = cv2.resize(roi, (64, 64))
               roi = roi.astype("float") / 255.0
               roi = img_to_array(roi)
               roi = np.expand_dims(roi, axis=0)
               
               preds = emotion_classifier.predict(roi)[0]
               emotion_probability = np.max(preds)
               label = EMOTIONS[preds.argmax()]
                                
               for (i, (emotion, prob)) in enumerate(zip(EMOTIONS, preds)):
                         text = "{}: {:.2f}%".format(emotion, prob * 100)
                         w = int(prob * 300)
                         cv2.putText(xac_suat, text, (5, (i * 35) + 23),cv2.FONT_HERSHEY_SIMPLEX, 0.5,(116,60,255), 2)
                         xacsuat = PIL.ImageTk.PhotoImage(image = PIL.Image.fromarray(xac_suat))

               cv2.putText(frame, label, (fX, fY - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,0), 2)
               cv2.rectangle(frame, (fX, fY), (fX + fW, fY + fH),(255,255,0), 2)
               frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
               frame = cv2.resize(frame, (500,470))
               photo = PIL.ImageTk.PhotoImage(image = PIL.Image.fromarray(frame))
               return (xacsuat, photo)

