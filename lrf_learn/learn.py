import keras
# from keras.datasets import mnist
from keras.models import Sequential
from keras.layers import Dense, Dropout
from keras.optimizers import RMSprop

from tensorflow.keras.models import Model
from tensorflow.keras.layers import Input, concatenate, Dense

# import libraries
# import pandas as pds
from keras.layers import Input, BatchNormalization
from keras.wrappers.scikit_learn import KerasRegressor
from sklearn.model_selection import cross_val_score
from sklearn.model_selection import KFold
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.pipeline import Pipeline
from sklearn.metrics import mean_squared_error
# from sklearn.datasets import load_diabetes

# use diabetes sample data from sklearn
# diabetes = load_diabetes()

import json
import numpy as np
import random
from copy import deepcopy


# create regression model
def reg_model():
    # 入力を定義
    input1 = Input(shape=(360,))

    # 入力1から結合前まで
    x = Dense(64, activation="relu")(input1)
    x = Dense(32, activation="relu")(x)
    x = Dense(32, activation="relu")(x)
    x = Model(inputs=input1, outputs=x)

    # 入力2から結合前まで
    input2 = Input(shape=(3,))
    y = Dense(3, activation="relu")(input2)
    y = Dense(16, activation="relu")(y)
    y = Model(inputs=input2, outputs=y)

    # 結合
    combined = concatenate([x.output, y.output])

    # 密結合
    z = Dense(16, activation="relu")(combined)
    z = Dense(3, activation="relu")(combined)

    # モデル定義とコンパイル
    model = Model(inputs=[x.input, y.input], outputs=z)
    model.compile(loss='mean_squared_error', optimizer='adam', metrics=['acc'])
    model.summary()

    return model

    # model = Sequential()
    # model.add(Dense(360, input_dim=360, activation='relu'))
    # model.add(Dense(64, activation='relu'))
    # model.add(Dense(64, activation='relu'))
    # model.add(Dense(32, activation='relu'))
    # model.add(Dense(16, activation='relu'))
    # model.add(Dense(3, activation='relu'))

    # # compile model
    # model.compile(loss='mean_squared_error', optimizer='adam')
    # model.summary()
    # return model


MAX_DISTANCE = 1200
MAX_WIDTH = 500
MAX_HEIGHT = 500

json_open = open('../data_set.json', 'r')
json_load = json.load(json_open)

distances_array = json_load["distances_array"]
pos_array = json_load["pos_array"]

learn_data = np.array(distances_array, dtype=np.float32)
result_data = np.array(pos_array, dtype=np.float32)

learn_data /= MAX_DISTANCE
result_data[:, 0] /= MAX_WIDTH
result_data[:, 1] /= MAX_HEIGHT
result_data[:, 2] /= 360

near_data = deepcopy(result_data) 
for i, near in enumerate(near_data):
    near_data[i][0] = near[0] + random.uniform(-0.01, 0.01)
    near_data[i][1] = near[1] + random.uniform(-0.01, 0.01)
    near_data[i][2] = near[2] + random.uniform(-0.01, 0.01)

#np.zeros(result_data.shape)

print(learn_data)
print(result_data)
print(near_data)


# use data split and fit to run the model
x1_train, x1_test, x2_train, x2_test, y_train, y_test = train_test_split(learn_data, near_data, result_data, test_size=0.1)
# estimator = KerasRegressor(build_fn=reg_model, epochs=100, batch_size=10, verbose=0)
# estimator.fit(x_train, y_train)

estimator = reg_model()
estimator.fit([x1_train, x2_train],
            y_train, 
            epochs=5, 
            batch_size=20, 
            validation_data=([x1_train, x2_train], y_train),
            verbose=1)

# show its root mean square error
y_pred = estimator.predict([x1_train, x2_train])
mse = mean_squared_error(y_train, y_pred)
np.set_printoptions(suppress=True)
print(y_train)
print(y_pred)
print("KERAS REG RMSE : %.2f" % (mse ** 0.5))

# show its root mean square error
y_pred = estimator.predict([x1_test, x2_test])
mse = mean_squared_error(y_test, y_pred)
np.set_printoptions(suppress=True)
print(y_test)
print(y_pred)
print("KERAS REG RMSE : %.2f" % (mse ** 0.5))




# batch_size = 128
# num_classes = 10
# epochs = 20

# # the data, split between train and test sets
# (x_train, y_train), (x_test, y_test) = mnist.load_data()

# x_train = x_train.reshape(60000, 784)
# x_test = x_test.reshape(10000, 784)
# x_train = x_train.astype('float32')
# x_test = x_test.astype('float32')
# x_train /= 255
# x_test /= 255
# print(x_train.shape[0], 'train samples')
# print(x_test.shape[0], 'test samples')

# # convert class vectors to binary class matrices
# y_train = keras.utils.to_categorical(y_train, num_classes)
# y_test = keras.utils.to_categorical(y_test, num_classes)

# model = Sequential()
# model.add(Dense(512, activation='relu', input_shape=(784,)))
# model.add(Dropout(0.2))
# model.add(Dense(512, activation='relu'))
# model.add(Dropout(0.2))
# model.add(Dense(num_classes, activation='softmax'))

# model.summary()

# model.compile(loss='categorical_crossentropy',
#               optimizer=RMSprop(),
#               metrics=['accuracy'])

# print(x_test.shape)
# print(y_test.shape)

# history = model.fit(x_train, y_train,
#                     batch_size=batch_size,
#                     epochs=epochs,
#                     verbose=1,
#                     validation_data=(x_test, y_test))
# score = model.evaluate(x_test, y_test, verbose=0)
# print('Test loss:', score[0])
# print('Test accuracy:', score[1])




# import keras
# # from keras.datasets import mnist
# from keras.models import Sequential
# from keras.layers import Dense, Dropout
# from keras.optimizers import RMSprop

# from tensorflow.keras.models import Model
# from tensorflow.keras.layers import Input, concatenate, Dense

# # import libraries
# # import pandas as pds
# from keras.layers import Input, BatchNormalization
# from keras.wrappers.scikit_learn import KerasRegressor
# from sklearn.model_selection import cross_val_score
# from sklearn.model_selection import KFold
# from sklearn.model_selection import train_test_split
# from sklearn.preprocessing import StandardScaler
# from sklearn.pipeline import Pipeline
# from sklearn.metrics import mean_squared_error
# # from sklearn.datasets import load_diabetes

# # use diabetes sample data from sklearn
# # diabetes = load_diabetes()

# import json
# import numpy as np

# # create regression model
# def reg_model():
#     # 入力を定義
#     input1 = Input(shape=(360,))
#     input2 = Input(shape=(1,))

#     # 入力1から結合前まで
#     x = Dense(64, activation="relu")(input1)
#     x = Model(inputs=input1, outputs=x)

#     # 入力2から結合前まで
#     y = Dense(1, activation="relu")(input2)
#     y = Model(inputs=input2, outputs=y)

#     # 結合
#     combined = concatenate([x.output, y.output])

#     # 密結合
#     z = Dense(32, activation="relu")(combined)
#     z = Dense(1, activation="relu")(z)

#     # モデル定義とコンパイル
#     model = Model(inputs=[x.input, y.input], outputs=z)
#     model.compile(loss='mean_squared_error', optimizer='adam', metrics=['acc'])
#     model.summary()

#     # model = Sequential()
#     # model.add(Dense(360, input_dim=360, activation='relu'))
#     # model.add(Dense(64, activation='relu'))
#     # model.add(Dense(64, activation='relu'))
#     # model.add(Dense(32, activation='relu'))
#     # model.add(Dense(16, activation='relu'))
#     # model.add(Dense(3, activation='relu'))

#     # # compile model
#     # model.compile(loss='mean_squared_error', optimizer='adam')
#     # model.summary()
#     # return model


# MAX_DISTANCE = 1200
# MAX_WIDTH = 500
# MAX_HEIGHT = 500

# json_open = open('../data_set_ang0.json', 'r')
# json_load = json.load(json_open)

# distances_array = json_load["distances_array"]
# pos_array = json_load["pos_array"]

# learn_data = np.array(distances_array, dtype=np.float32)
# result_data = np.array(pos_array, dtype=np.float32)

# learn_data /= MAX_DISTANCE
# result_data[:, 0] /= MAX_WIDTH
# result_data[:, 1] /= MAX_HEIGHT
# result_data[:, 2] /= 360

# near_data = np.zeros(result_data.shape())

# print(learn_data)
# print(result_data)
# print(near_data)


# # use data split and fit to run the model
# x_train1, x_train2, x_test1, x_test2 y_train, y_test = train_test_split(learn_data, near_data, result_data, test_size=0.1)
# # estimator = KerasRegressor(build_fn=reg_model, epochs=100, batch_size=10, verbose=0)
# # estimator.fit(x_train, y_train)
# estimator = reg_model()
# estimator.fit(x_train,
#             y_train, 
#             epochs=100, 
#             batch_size=20, 
#             validation_data=(x_train, y_train),
#             verbose=1)

# # show its root mean square error
# y_pred = estimator.predict(x_train)
# mse = mean_squared_error(y_train, y_pred)
# np.set_printoptions(suppress=True)
# print(y_train)
# print(y_pred)
# print("KERAS REG RMSE : %.2f" % (mse ** 0.5))

# # show its root mean square error
# y_pred = estimator.predict(x_test)
# mse = mean_squared_error(y_test, y_pred)
# np.set_printoptions(suppress=True)
# print(y_test)
# print(y_pred)
# print("KERAS REG RMSE : %.2f" % (mse ** 0.5))