from keras.preprocessing.image import img_to_array
import cv2
from keras.models import load_model
import numpy as np
import time
import PIL.Image, PIL.ImageTk
import numpy as np
from tkinter import*
def emotion_video(framevideo):
     detection_model_path = 'C:/Users/Minh Hieu Hoang Thi/AppData/Local/Programs/Python/Python35/src/cascades/data/haarcascade_frontalface_default.xml'
     emotion_model_path = 'E:/HUMAN_E/_mini_XCEPTION.102-0.66.hdf5'

     face_detection = cv2.CascadeClassifier(detection_model_path)
     emotion_classifier = load_model(emotion_model_path, compile=False)
     EMOTIONS = ["ANGRY" ,"DISGUST","SCARED", "HAPPY", "SAD", "SUPRISED","NEUTRAL"]
     # starting video streaming
     #cv2.namedWindow('your_face')
     camera = framevideo
     while True:
         frame = camera.read()[1]
         cv2.imwrite("E:/anh_GUI/user" + time.strftime("%d-%m-%Y-%H-%M-%S") + ".jpg", frame)
         gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
         faces = face_detection.detectMultiScale(gray,scaleFactor=1.1,minNeighbors=5,minSize=(30,30),flags=cv2.CASCADE_SCALE_IMAGE)
         
         canvas = np.zeros((250, 170, 3), dtype="uint8")
         #xac_suat = np.zeros((250, 170,3), dtype="uint8")
         canvas[:,:170] = [239, 239, 239]
         frameClone = frame.copy()
         
         if len(faces) > 0:
             faces = sorted(faces, reverse=True,
             key=lambda x: (x[2] - x[0]) * (x[3] - x[1]))[0]
             (fX, fY, fW, fH) = faces
                         # Extract the ROI of the face from the grayscale image, resize it to a fixed 48x48 pixels, and then prepare
                 # the ROI for classification via the CNN
             roi = gray[fY:fY + fH, fX:fX + fW]
             roi = cv2.resize(roi, (64, 64))
             roi = roi.astype("float") / 255.0
             roi = img_to_array(roi)
             roi = np.expand_dims(roi, axis=0)
             
             
             preds = emotion_classifier.predict(roi)[0]
             emotion_probability = np.max(preds)
             label = EMOTIONS[preds.argmax()]

      
             for (i, (emotion, prob)) in enumerate(zip(EMOTIONS, preds)):
                         # construct the label text
                    text = "{}: {:.2f}%".format(emotion, prob * 100)
                    w = int(prob * 300)
                    #cv2.rectangle(canvas, (7, (i * 35) + 5),(w, (i * 35) + 35), (0, 0, 255), -1)
                    cv2.putText(canvas, text, (5, (i * 35) + 23),cv2.FONT_HERSHEY_SIMPLEX, 0.5,(116,60,255), 2)
                    
                    cv2.putText(frameClone, label, (fX, fY - 10),cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,0), 2)
                    cv2.rectangle(frameClone, (fX, fY), (fX + fW, fY + fH), (255, 255, 0), 2)
                    frameClone = cv2.cvtColor(frameClone, cv2.COLOR_BGR2RGB)
                    xacsuat = PIL.ImageTk.PhotoImage(image = PIL.Image.fromarray(canvas))
                    #self.show_xs.configure(image = self.xacsuat)
                    photo = PIL.ImageTk.PhotoImage(image = PIL.Image.fromarray(frameClone))
             return (xacsuat, photo)
##         cv2.imshow('your_face', frameClone)
           #cv2.imshow("Probabilities", canvas)
##         if cv2.waitKey(1) & 0xFF == ord('q'):
##             break
##
##     camera.release()
##     cv2.destroyAllWindows()
