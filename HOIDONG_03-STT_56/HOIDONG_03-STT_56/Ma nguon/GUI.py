#-*-coding: utf8;-*-
from tkinter import *
from tkinter import filedialog
from PIL import Image, ImageTk

GUI= Tk()
GUI.title("Recognition")
GUI.geometry("600x550")

def open_img(images):
	img=Image.open(images)
	my_img=ImageTk.PhotoImage(img)
	return my_img

def Recognition(path):
	import numpy as np
	import os
	from keras.preprocessing import image
	import tensorflow as tf
	from keras.models import load_model

	os.environ['TF_CPP_MIN_LOG_LEVEL']='3'
	tf.logging.set_verbosity(tf.logging.ERROR)

	model=load_model('Trained_model.h5')
	test_image=image.load_img(path,target_size=(64,64))
	test_image=image.img_to_array(test_image)
	test_image=np.expand_dims(test_image,axis=0)
	result=model.predict(test_image)

	if result[0][0]==1:
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
	else: img_text='UNKNOW'
	return img_text

def Open():
    global my_img
    ftypes = [('JPEG image', '*.jpg'), ('All files', '*')]
    file=filedialog.askopenfilename(filetypes=ftypes)
    my_img=open_img(file)
    canvas.create_image(125,125,anchor=CENTER,image=my_img)
    canvas.place(x=150,y=70)

    check=Recognition(file)
    lblResult=Label(GUI,text=check,font=("Times New Romand",14,"bold"),bg="white",fg="red",width=20,height=1)
    lblResult.place(x=150,y=350)
    
btnSelect=Button(GUI,text="Select Picture",font=("Times New Romand",14,"bold"),bg="white",fg="black",width=20,height=1,command=Open)
btnSelect.place(x=150,y=30)

canvas= Canvas(GUI,width=250,height=250,bg="white")
canvas.create_text(125,125,anchor=CENTER,text="Picture")
canvas.place(x=150,y=70)

lblResult=Label(GUI,text="Result",font=("Times New Romand",14,"bold"),bg="white",fg="black",width=20,height=1)
lblResult.place(x=150,y=350)

picture = Image.open("logo SPKT.jpg")
logo = ImageTk.PhotoImage(picture)
label1 = Label(GUI, image=logo)
label1.image = logo
label1.place(x=10, y=30)

lblTen=Label(GUI,text="TRƯỜNG ĐẠI HỌC SƯ PHẠM KỸ THUẬT TP. HỒ CHÍ MINH",font=("Times New Romand",14,"bold"),fg="black")#,width=100,height=1)
lblTen.place(x=50,y=0)

GUI.mainloop()
