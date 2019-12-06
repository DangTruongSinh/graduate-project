from tkinter import *
import os
from tkinter import messagebox
window = Tk()

window.title("Attendance")

window.geometry('250x100')

lbl = Label(window)

lbl.grid(column=0, row=0)

txt = Entry(window, width=20)

txt.grid(column=1, row=0)

def clicked():
    mystring = txt.get()
    print(mystring)
    if(mystring ==''):
        messagebox.showwarning('Warning', 'Please enter a value ')
    else:

        myCmd = "python captureface.py -c haarcascade_frontalface_default.xml -o/" + mystring
        os.system(myCmd)
        print(myCmd)


def clicked1():
    myCmd1 = "python encode_faces.py -i dataset -e encodings.pickle"
    os.system(myCmd1)

def clicked2():
    myCmd2 = "python pi_face_recognition.py -c haarcascade_frontalface_default.xml -e encodings.pickle"
    os.system(myCmd2)

btn = Button(window, text="Add",width=10, command=clicked)
btn1 = Button(window, text="Encode",width=10, command=clicked1)
btn2 = Button(window, text="Recognition",width=10, command=clicked2)

btn.grid(column=23, row=0)
btn1.grid(column=23, row=1)
btn2.grid(column=23, row=2)


window.mainloop()