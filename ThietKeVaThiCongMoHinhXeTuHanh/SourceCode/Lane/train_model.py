import pylint
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
from sklearn.model_selection import StratifiedShuffleSplit
number_class = 6

# y1 = len(glob.glob(os.path.join("small" ,'*jpg')))
# print(y1)
# y2 = len(glob.glob(os.path.join("small1" ,'*jpg')))
# print(y2)
with open("data.txt", "rb") as data:
    X,y = pickle.load(data)

X=X.reshape(-1,75,200,3)
print(X.shape)
print(y.shape)
y = np.asarray([0]*900 + [1]*850 +[2]*658+ [3]*932 +[4]*937 +[5]*600)

#Khởi tạo model
model = Sequential()
model.add(Conv2D(8, kernel_size=(5, 5),				 
                 activation='relu',
				 strides=(2, 2),
                 input_shape= (75,200,3),
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
model.add(Conv2D(64, kernel_size=(3, 3), 
				 activation='relu',
 				 strides=(1, 1),
 				 padding='valid',))
model.add(Flatten())
model.add(Dense(100, activation='relu'))
model.add(Dense(50, activation='relu'))
model.add(Dense(20, activation='relu'))
model.add(Dense(number_class, activation='softmax'))# ví dụ 1 matrix thì + giũ nguyên, - thì = 0
model.summary()

Adam = keras.optimizers.Adam(lr=0.0001, beta_1=0.9, beta_2=0.999, epsilon=None, decay=0.0, amsgrad=False)
model.compile(loss='categorical_crossentropy', optimizer=Adam, metrics=['accuracy'])

y = np_utils.to_categorical(y, number_class)

kfold = StratifiedShuffleSplit(n_splits=10, random_state=7,test_size=0.15)
i = 0

for train_index, test_index in kfold.split(X,y):
    checkpoint_path = os.path.join("C:\\Users\\Hung\\Desktop\\lane\\logs","Model_fold_" + str(i) + "_Epoch_{epoch:04d}.h5")
    tensorboard = [TensorBoard(log_dir="C:\\Users\\Hung\\Desktop\\lane\\logs",
 					histogram_freq=0, write_graph=True, write_images=False),
					keras.callbacks.ModelCheckpoint(checkpoint_path,
 					verbose=0, save_best_only=True),]
    X_train, X_test = X[train_index], X[test_index]
    y_train, y_test = y[train_index], y[test_index]
    model.fit(X_train, y_train, 
                        batch_size=2, # số phần tử 1 lần train, càng cao thì càng tổng quan nhưng phải ở trong 1 giới hạn.
                        epochs=10,
                        shuffle=True,
                        validation_data=(X_test, y_test),
                        callbacks=tensorboard)
    i = i +1