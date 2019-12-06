from imutils import paths
from sklearn.preprocessing import LabelEncoder
from sklearn.model_selection import train_test_split
import os
import cv2
from sklearn.preprocessing import LabelBinarizer
from keras.utils import np_utils
from keras.models import Sequential
from keras.layers import Conv2D, MaxPooling2D
from keras.layers import Dense
from keras.layers import Flatten
import numpy as np
from keras.optimizers import Adam
  
image_path = list(paths.list_images('./dataset_eyes'))
labels = [p.split(os.path.sep)[-2] for p in image_path]
le = LabelEncoder()
labels = le.fit_transform(labels)

list_image = []
for (j, imagePath) in enumerate(image_path):
  image = cv2.imread(imagePath,cv2.IMREAD_GRAYSCALE)
  image = cv2.resize(image,(40,150))    
  list_image.append(np.array(image))

list_image = np.asarray(list_image)
list_image = list_image.reshape((list_image.shape[0], list_image.shape[1], list_image.shape[2], 1))
list_image = list_image/255
X_train, X_test, y_train, y_test = train_test_split(list_image , labels, test_size=0.1, random_state=41)
X_train, X_val, y_train, y_val = train_test_split(X_train , y_train, test_size=0.1, random_state=41)

print(X_train[0])
  
model = Sequential()

model.add(Conv2D(32, (3, 3), activation='relu', input_shape=(150,40,1)))

model.add(MaxPooling2D(pool_size=(2,2)))
    
model.add(Conv2D(64, (3, 3), activation='relu'))
    
model.add(MaxPooling2D(pool_size=(2,2)))
    
model.add(Conv2D(64, (3, 3), activation='relu'))
    
model.add(MaxPooling2D(pool_size=(2,2)))
    
model.add(Flatten())
    
model.add(Dense(128, activation='relu'))
    
model.add(Dense(1, activation='sigmoid'))
    
model.compile(loss='binary_crossentropy',
    optimizer=Adam(0.001),
    metrics=['accuracy']) 
model.summary()

H = model.fit(X_train, y_train, validation_data=(X_val, y_val),
              batch_size=32, epochs=10, verbose=1)
   
import matplotlib.pyplot as plt
score = model.evaluate(X_test, y_test, verbose=0)
print(score)
i=2
plt.imshow(X_test[i].reshape(150,40), cmap='gray')

y_predict = model.predict(X_test[i].reshape(1,150,40,1))
print('Giá trị dự đoán: ', y_predict > 0.5)


