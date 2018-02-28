import numpy as np
np.random.seed(123)
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation, Flatten
from keras.layers import Convolution2D, Convolution3D, MaxPooling2D, MaxPooling3D
from keras.utils import np_utils
from keras import optimizers
from sklearn.preprocessing import MinMaxScaler
from keras.layers.normalization import BatchNormalization
from keras.callbacks import ReduceLROnPlateau

###Load file

X_train=np.load('C:/Users/ketkarah/Desktop/cs446/train_X.npy')
Y_train=np.load('C:/Users/ketkarah/Desktop/cs446/train_binary_Y.npy')
"""X_train=X_train.astype('float32')

###Standardize and reshape

X_train-=np.mean(X_train)
X_train /=np.max(X_train)"""
X_train=X_train.reshape(X_train.shape[0],1,X_train.shape[1],X_train.shape[2],X_train.shape[3])

###CNN model

model = Sequential()
model.add(Convolution3D(32, (5, 5, 5), input_shape=(1,26,31,23),data_format='channels_first'))
model.add(Activation("relu"))
model.add(MaxPooling3D(pool_size=(2,2,2),strides=(2,2,2)))
model.add(Convolution3D(64, (5, 5, 5)))
model.add(BatchNormalization())
model.add(Activation("relu"))
"""model.add(Convolution3D(32, (3, 3, 3), strides=(2,2,2), activation='relu'))"""
model.add(Convolution3D(64, (3, 3, 3)))
model.add(BatchNormalization())
model.add(Activation("relu"))
"""model.add(Convolution3D(64, (5, 5, 5)))"""
model.add(Flatten())

###Fully connected layers

model.add(Dense(1024))
model.add(Activation("relu"))
model.add(Dropout(0.5))
model.add(Dense(512))
model.add(BatchNormalization())
model.add(Activation("relu"))
model.add(Dropout(0.5))
model.add(Dense(25))
model.add(Dense(19, activation='sigmoid'))

###Hyperparameters

opt = optimizers.SGD(lr=0.1,decay=1e-6)
model.compile(loss='categorical_crossentropy',
              optimizer=opt)
reduce_lr = ReduceLROnPlateau(monitor='loss', factor=0.2,
                              patience=5, min_lr=0.001)

###Train model

model.fit(X_train, Y_train, 
          batch_size=128, nb_epoch=155, verbose=1, callbacks=[reduce_lr])
"""To be run after the above script
X_test=np.load("C:/Users/ketkarah/Desktop/cs446/valid_test_X.npy")
X_test=X_test.reshape(X_test.shape[0],1,X_test.shape[1],X_test.shape[2],X_test.shape[3])
Predict_y=model.predict(X_test)
Predict_ybin=np.array([[1 if Y_local > 0.5 else 0 for Y_local in Y_localrow] for Y_localrow in Predict_y])
np.save('C:/Users/ketkarah/Desktop/cs446/Predict_bin.npy',Predict_ybin)"""
