import matplotlib
matplotlib.use('tkAgg')
import matplotlib.pyplot as plt
import random
import numpy as np
import cv2
import keras
import glob
import os
from time import time
from keras import optimizers
from keras.models import Sequential
from keras.layers import Dense, Dropout, Flatten
from keras.layers import Conv2D, MaxPooling2D
from keras import backend as K
from keras.utils import np_utils
from sklearn.model_selection import train_test_split
import pickle
from keras.callbacks import TensorBoard
number_class = 3

y1 = len(glob.glob(os.path.join("E:\\HOCTAP\\classification\\red_sign" ,'*jpg')))
y2 = len(glob.glob(os.path.join("E:\\HOCTAP\\classification\\green_sign" ,'*jpg')))
y3 = len(glob.glob(os.path.join("E:\\HOCTAP\\classification\\stop_sign" ,'*jpg')))


# X_data
X = []
count = 0
for i in glob.glob(os.path.join("E:\\HOCTAP\\classification\\red_sign" ,'*jpg')): #100
	img = cv2.imread(i)	
	img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
	if count == 0:
		X = img 
		count = 1
	else:
		X = np.append(X,img, axis = 0)	
for i in glob.glob(os.path.join("E:\\HOCTAP\\classification\\green_sign" ,'*jpg')): #115
	img = cv2.imread(i)	
	img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
	X = np.append(X,img, axis = 0)

for i in glob.glob(os.path.join("E:\\HOCTAP\\classification\\stop_sign" ,'*jpg')): #130
	img = cv2.imread(i)	
	img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
	X = np.append(X,img, axis = 0)



print("Finish")
X = X.reshape(-1,64,64,1)
X.astype('float32')

#y data
y = np.asarray([0]*y1 + [1]*y2 + [2]*y3 )
y.astype('uint8')

# with open("data.txt", "wb") as data:
#     pickle.dump((X,y), data)

# with open("data.txt", "rb") as data:
#     X,y = pickle.load(data)

X_train, X_valid, y_train, y_valid = train_test_split(X, y, test_size=0.2, random_state=1)
y_train = np_utils.to_categorical(y_train, number_class)
y_valid = np_utils.to_categorical(y_valid, number_class)
print(X_train.shape)
print(X_valid.shape)
print(y_train.shape)
print(y_valid.shape)
print(y_valid)

#Khởi tạo model
model = Sequential()

model.add(Conv2D(8, kernel_size=(7, 7),				 
                 activation='relu',
				 strides=(2, 2),
                 input_shape= (64,64,1),
				 padding='valid',))
model.add(Conv2D(16, kernel_size=(5, 5), 
				 activation='relu',
				 strides=(2, 2),
				 padding='valid',))
model.add(Conv2D(32, kernel_size=(5, 5), 
				 activation='relu',
				 strides=(2, 2),
				 padding='valid',))
model.add(Conv2D(64, kernel_size=(3, 3), 
				 activation='relu',
				 strides=(1, 1),
				 padding='valid',))

model.add(Flatten())
model.add(Dense(100, activation='relu'))
model.add(Dense(50, activation='relu'))
model.add(Dense(20, activation='relu'))
model.add(Dense(number_class, activation='softmax'))
model.summary()
checkpoint_path = os.path.join("E:\\HOCTAP\\classification\\logs", "{epoch:04d}.h5")
tensorboard = [TensorBoard(log_dir="E:\\HOCTAP\\classification\\logs",
							histogram_freq=0, write_graph=True, write_images=False),
							keras.callbacks.ModelCheckpoint(checkpoint_path,
							verbose=0, save_best_only=True),]
Adam = keras.optimizers.Adam(lr=0.0003, beta_1=0.9, beta_2=0.999, epsilon=None, decay=0.0, amsgrad=False)
model.compile(loss='categorical_crossentropy', optimizer=Adam, metrics=['accuracy'])
model.fit(X_train, y_train,
					batch_size=2,
					epochs=50,
					shuffle=True,
					validation_data=(X_valid, y_valid),
					callbacks=tensorboard)






