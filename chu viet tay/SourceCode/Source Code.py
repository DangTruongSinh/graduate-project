import tkinter
from tkinter.ttk import Frame, Button, Style, Label

from tkinter import *
from tkinter import filedialog, font
from PIL import ImageTk, Image

import PIL.Image
import PIL.ImageTk
import cv2

import numpy as np
import tensorflow as tf

new_model = tf.keras.models.load_model("Con6432_D02_Den128_D02_Den50.model")

CATEGORIES = ["none", 'a', 'b', 'c', 'd', 'e', 'f', 'g',
              'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
              'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z']


# create a window
class Example(Frame):
    img = cv2.imread("1.jpg", cv2.IMREAD_UNCHANGED)

    def __init__(self, parent, **kw):
        Frame.__init__(self, parent)
        super().__init__(**kw)
        # size of window
        parent.minsize(width=700, height=650)
        # define theme as default
        self.style = Style()
        self.style.theme_use("default")
        self.pack(fill=BOTH, expand=1)
        # use parent to save a window
        self.parent = parent
        self.parent.title("ĐỒ ÁN TỐT NGHIỆP")
        myFont = font.Font(family='Helvetica', size=9, weight='bold')
        # create Label
        self.Label = Label(self, text="NHẬN DẠNG KÝ TỰ VIẾT TAY DÙNG MẠNG NƠ-RON NHÂN TẠO", font=('TimesBold', 15))
        self.Label.place(x=60, y=10)
        self.Label = self.Label.config(fg='red')
        self.Label = Label(self, text="Ký tự nhận dạng", font=myFont)
        self.Label.place(x=410, y=80)
        self.Label = self.Label.config(fg='red')
        self.Label = Label(self, text="Nguyễn Thanh Tuyết Hân", font=('Times13'))
        self.Label.place(x=445, y=560)
        self.Label = Label(self, text="Cao Văn Cảnh", font='Times13')
        self.Label.place(x=445, y=585)
        # create Button
        self.Button = Button(self, text="Chọn ảnh", font=myFont, command=self.open_img, height=1, width=10)
        self.Button.place(x=20, y=50)
        self.Button = Button(self, text="Mở camera", font=myFont, command=self.video, height=1, width=10)
        self.Button.place(x=120, y=50)
        self.Button = Button(self, text="Chụp ảnh", font=myFont, command=self.snapshot, height=1, width=10)
        self.Button.place(x=210, y=50)
        self.Button = Button(self, text="Nhận dạng", font=myFont, command=self.reg, height=1, width=10)
        self.Button.place(x=310, y=50)
        self.Button = Button(self, text="Lưu", font=myFont, command=self.save_as_file, height=1, width=10)
        self.Button.place(x=608, y=50)
        self.Button = Button(self, text="Xóa", font=myFont, command=self.clear, height=1, width=10)
        self.Button.place(x=518, y=50)
        # create text to show result
        self.Text = Text(root, height=10, width=12, font=('TimesBold', 30))
        self.Text.place(x=410, y=100)

    def clear(self):
        textsave = self.Text.delete(1.0, END)

    def save_as_file(self):
        f = filedialog.asksaveasfile(mode='w', defaultextension=".txt", filetypes=[("Text files", ".txt"),
                                                                                   ("Word files", ".doc")],
                                     initialdir="dir", title="ĐỒ ÁN TỐT NGHIỆP")
        if f is None:
            return
        textsave = self.Text.get(1.0, END)
        f.write(textsave)
        f.close()

    def open_img(self):
        global result
        # choose and save image path
        file = filedialog.askopenfile(parent=self, mode='rb', title='Choose a file')
        path = file.name
        # Resize image
        self.img = Image.open(path)
        self.img = self.img.resize((360, 270), Image.ANTIALIAS)  # Resize
        self.img = ImageTk.PhotoImage(self.img)
        # Create a label to show image
        self.label = Label(image=self.img)
        self.label.place(x=20, y=370)
        result = cv2.imread(path, 1)

    def video(self, video_source=0):
        self.video_source = video_source
        self.vid = MyVideoCapture(self.video_source)
        ret, frame = self.vid.get_frame()
        resized = cv2.resize(frame, (360, 270), interpolation=cv2.INTER_AREA)
        if ret:
            self.canvas = tkinter.Canvas(self, width=360, height=270)
            self.canvas.place(x=20, y=100)
            self.photo = PIL.ImageTk.PhotoImage(image=PIL.Image.fromarray(resized))
            self.canvas.create_image(0, 0, image=self.photo, anchor=tkinter.NW)
        self.delay = 25
        self.update()

    def snapshot(self):
        # Get a frame from the video source
        global result
        ret, frame = self.vid.get_frame()
        if ret:
            cv2.imwrite("frame.jpg", cv2.cvtColor(frame, cv2.COLOR_RGB2BGR))
            self.img = Image.open("frame.jpg")
            self.img = self.img.resize((360, 270), Image.ANTIALIAS)  # Resize
            self.img = ImageTk.PhotoImage(self.img)
            # Create a label to show image
            self.label = Label(image=self.img)
            self.label.place(x=20, y=370)
        result = cv2.imread("frame.jpg", 1)

    def reg(self):
        def prepare(filepath):
            IMG_SIZE = 28
            img_array = cv2.imread(filepath, cv2.IMREAD_GRAYSCALE)
            new_array = cv2.resize(img_array, (IMG_SIZE, IMG_SIZE))
            inverted_image = 255 - new_array
            inverted_image = inverted_image / 255
            return inverted_image.reshape(1, IMG_SIZE, IMG_SIZE, 1)  # convolutional neural network
        # image detection
        gray = cv2.cvtColor(result, cv2.COLOR_BGR2GRAY)
        blurred = cv2.GaussianBlur(gray, (5, 5), 0)
        ret, threshFilled = cv2.threshold(blurred, 130, 255, cv2.THRESH_BINARY_INV | cv2.THRESH_OTSU)
        # dilation
        kernel = np.ones((5, 1), np.uint8)
        img_dilation = cv2.dilate(threshFilled, kernel, iterations=1)
        # find contours
        ctrs, hier = cv2.findContours(img_dilation.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        # sort contours
        sorted_ctrs = sorted(ctrs, key=lambda ctr: cv2.boundingRect(ctr)[0])
        sentence = " "
        # self.Text.insert(1.0, '\n')
        for i, ctrs in enumerate(sorted_ctrs):
            # Get bounding box
            x, y, w, h = cv2.boundingRect(ctrs)
            # Getting ROI
            roi = img_dilation[y - 3:y + h + 3, x - 3:x + w + 3]
            # roi = img_dilation[y :y + h , x :x + w ]

            ret, thresh1 = cv2.threshold(roi, 0, 255, cv2.THRESH_BINARY_INV | cv2.THRESH_OTSU)
            if (w > 15 and h > 15) or (w > 28 and h > 28):
                resized = cv2.resize(thresh1, (28, 28), interpolation=cv2.INTER_NEAREST)
                cv2.imwrite('i.jpg'.format(i), resized)
                cv2.imwrite('result/{}.jpg'.format(i), resized)
                predictions = new_model.predict(prepare("i.jpg"))
                sentence = sentence + str(CATEGORIES[np.argmax(predictions)])
        self.Text.insert(END, sentence)

    def update(self):
        ret, frame = self.vid.get_frame()  # Get a frame from the video source
        resized = cv2.resize(frame, (360, 270), interpolation=cv2.INTER_AREA)
        if ret:
            self.canvas = tkinter.Canvas(self, width=360, height=270)
            self.canvas.place(x=20, y=100)
            self.photo = PIL.ImageTk.PhotoImage(image=PIL.Image.fromarray(resized))
            self.canvas.create_image(0, 0, image=self.photo, anchor=tkinter.NW)
        self.parent.after(self.delay, self.update)


class MyVideoCapture:
    def __init__(self, video_source=0):
        # Open the video source
        self.vid = cv2.VideoCapture(video_source)
        if not self.vid.isOpened():
            raise ValueError("Unable to open video source", video_source)
        # Get video source width and height
        self.width = self.vid.get(cv2.CAP_PROP_FRAME_WIDTH)
        self.height = self.vid.get(cv2.CAP_PROP_FRAME_HEIGHT)

    def get_frame(self):
        ret, frame = self.vid.read()
        if self.vid.isOpened():
            if ret:
                # Return a boolean success flag and the current frame converted to BGR
                return (ret, cv2.cvtColor(frame, cv2.COLOR_BGR2RGB))
            else:
                return (ret, None)
        else:
            return (ret, None)

    def __del__(self):
        if self.vid.isOpened():
            self.vid.release()

root = Tk()
app = Example(root)
app.parent.iconbitmap('icon.ico')
root.mainloop()