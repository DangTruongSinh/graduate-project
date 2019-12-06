import tkinter
import os
import cv2
import PIL.Image, PIL.ImageTk
import time
import tkinter.filedialog
from keras.preprocessing.image import img_to_array
from keras.models import load_model
from camera.load_video import emotion_video
from camera.load_image import emotion_image
import numpy as np
from tkinter import*

class App:
    def __init__(self, window, window_title):
         self.window = window
         self.window.title(window_title)
         self.window.minsize(1370,700)
         self.vid = cv2.VideoCapture(0)
        
         img = PIL.Image.open("C:/Users/Minh Hieu Hoang Thi/Desktop/hienthindung.jpg")
         photo = PIL.ImageTk.PhotoImage(img)
         label_logo = Label(window,image=photo)
         label_logo.place(x = 0,y=2)
         
         img2 = PIL.Image.open("C:/Users/Minh Hieu Hoang Thi/Desktop/dothi.jpg")
         photo2 = PIL.ImageTk.PhotoImage(img2)
         label_dothi = Label(window,image=photo2)
         label_dothi.place(x = 940,y=330)


         img1 = PIL.Image.open("C:/Users/Minh Hieu Hoang Thi/Desktop/anh1.jpg")
         photo1 = PIL.ImageTk.PhotoImage(img1)
         self.show_video = Label(window, image = photo1)
         self.show_video.place(x=280,y= 210)

         self.show_xs = Label(window)
         self.show_xs.place(x=60,y= 380)
         
         self.btn_emo=tkinter.Button(window, text="OPEN_IMAGE",width = 12,fg="white", command=self.Image,font=(14),bg='#6C2DC7')
         self.btn_emo.place(x = 70, y = 220)
         self.btn_emo=tkinter.Button(window, text="WEBCAM", width = 12,fg="white",command=self.webcam,font=(14),bg='#6C2DC7')
         self.btn_emo.place(x = 70, y = 260)
         self.btn_emo=tkinter.Button(window, text="CLOSE",width =7, fg="white",command=self.Close_w,font=(14),bg='Indigo')
         self.btn_emo.place(x = 70, y = 300)
         self.quit = tkinter.Button(window, text='QUIT',fg="white", command=window.destroy,font=(14),bg='red')
         self.quit.place(x = 139, y = 300)

         labe0_title=Label(window,text="SVTH: Hoàng Thị Minh Hiếu_15141158\n            Nguyễn Thị Hồng Vy_15141335",fg='blue',font=("Times New Roman", 16,"bold"))
         labe0_title.place(x = 960, y = 220)
         labe0_title=Label(window,text="GVHD: Ths.Trương Quang Phúc",fg='blue',font=("Times New Roman", 16,"bold"))
         labe0_title.place(x = 960, y = 280)

         self.label_clock = Label(text="", fg="SaddleBrown", font=("Helvetica", 20))
         self.label_clock.place(x=1070,y=650)
         self.update_clock()
         img_org=""
         self.window.mainloop()
    def update_clock(self):
        now = time.strftime("%d-%m-%Y %H:%M:%S")
        self.label_clock.configure(text=now)
        self.window.after(1000, self.update_clock)
    def Close_w(self):
        self.show_video.configure(image = "")
        self.show_xs.configure(image = "")
        #if self.vid.isOpened():
        self.vid.release()
    def webcam(self):
         self.show_video.configure(image = "")
         self.show_xs.configure(image = "")
         xacsuat, photo = emotion_video(self.vid)
         self.xacsuat = xacsuat
         self.photo = photo
         
         self.show_xs.configure(image = self.xacsuat)
         self.show_video.configure(image = self.photo)               
         self.window.after(10,self.webcam)

    def Image(self):
         self.show_video.configure(image = "")
         self.show_xs.configure(image = "")
         global img_org
         img_org = filedialog.askopenfilename(filetypes=(("JPEG","*.jpg"),("PNG","*.png"),("Bitmap Files","*.bmp"),("GIF","*.gif")))
         xacsuath, photoh = emotion_image(img_org)
         self.xacsuath = xacsuath
         self.photoh = photoh
         
         self.show_xs.configure(image = self.xacsuath)
         self.show_video.configure(image = self.photoh)  

App(tkinter.Tk(), "NHAN_DIEN_CAM_XUC")
