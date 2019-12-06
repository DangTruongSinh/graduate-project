# Part 1 - Building the CNN
#importing the Keras libraries and packages
import os
import h5py
import tensorflow as tf
from keras import optimizers
from keras.models import Sequential
from keras.layers import Conv2D
from keras.layers import MaxPooling2D
from keras.layers import Flatten
from keras.layers import Dense, Dropout
from keras.preprocessing.image import ImageDataGenerator

os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'
tf.logging.set_verbosity(tf.logging.ERROR)

# Initialing the CNN
model = Sequential()

# Step 1 - Convolutio Layer 
model.add(Conv2D(32,(3,3),input_shape = (64, 64, 3),padding='same',activation = 'relu'))

#step 2 - Pooling
model.add(MaxPooling2D(pool_size =(2,2)))

# Adding second convolution layer
model.add(Conv2D(64,(3,3),padding='same',activation = 'relu'))
model.add(MaxPooling2D(pool_size =(2,2)))

#Adding 3rd Concolution Layer
model.add(Conv2D(128,(5,5),padding='same',activation = 'relu'))
model.add(MaxPooling2D(pool_size =(2,2)))

#Step 3 - Flattening
model.add(Flatten())

#Step 4 - Full Connection
model.add(Dense(128, activation = 'relu'))
model.add(Dropout(0.25))
model.add(Dense(10, activation = 'softmax'))
model.summary()
#Compiling The CNN
model.compile( optimizer ='adam',
               loss = 'categorical_crossentropy',
               metrics = ['accuracy'])

#Part 2 Fittting the CNN to the image
train_datagen = ImageDataGenerator(
        rescale=1./255,
        shear_range=0.2,
        zoom_range=0.2,
        horizontal_flip=True)

test_datagen = ImageDataGenerator(rescale=1./255)

training_set = train_datagen.flow_from_directory(
        'TrainData/Train',
        target_size=(64, 64),
        batch_size=32,
        class_mode='categorical',)

test_set = test_datagen.flow_from_directory(
        'TrainData/Test',
        target_size=(64, 64),
        batch_size=32,
        class_mode='categorical',
        )

History=model.fit_generator(training_set,
                     steps_per_epoch=len(training_set),
                     epochs=15, 
                     validation_data = test_set,
                     validation_steps = len(test_set),
                  )

#Saving the model
model.save('Trained_model.h5')
print("Model Saved")

print(History.history.keys())
import matplotlib.pyplot as plt
# summarize history for accuracy
plt.plot(History.history['acc'])
plt.plot(History.history['val_acc'])
plt.title('model accuracy')
plt.ylabel('accuracy')
plt.xlabel('epoch')
plt.legend(['train', 'test'], loc='upper left')
plt.show()
# summarize history for loss
plt.plot(History.history['loss'])
plt.plot(History.history['val_loss'])
plt.title('model loss')
plt.ylabel('loss')
plt.xlabel('epoch')
plt.legend(['train', 'test'], loc='upper left')
plt.show()

