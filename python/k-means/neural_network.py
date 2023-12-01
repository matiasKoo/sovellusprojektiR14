import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

def relu(x):
	return(np.maximum(x,0)) 

def softmax(x):
    return np.exp(x)/np.sum(np.exp(x))


#df = pd.read_csv('kuva4.csv',header=None)  # näin saadaan luettua kaikki rivit
#a0 = df.to_numpy()                         # activation a0 tätä voidaan käyttää neuroverkon inputtina
#kuva = df.to_numpy();                      # Ja tätä voit sitten muokata kuvan tulostamiseksi

#tässä data syötetty käsin
a0 = np.array([[1491],[1757],[1556]])

df = pd.read_csv('w1.csv',header=None)     # Tästä tiedostosta luetaan opetetut painokertoimet
w1 = df.to_numpy()                         # w1 = 3*10 matriisi, w1.shape=(3,10)

df = pd.read_csv('w2.csv',header=None)     # Tästä tiedostosta luetaan opetetut painokertoimet
w2 = df.to_numpy()                         # w2 = 10*6 matriisi, w2.shape=(10,6)

df = pd.read_csv('b1.csv',header=None)     # Tästä tiedostosta luetaan opetetut bias arvot
b1 = df.to_numpy()                         # b1.shape = (10,1)

df = pd.read_csv('b2.csv',header=None)     # Tästä tiedostosta luetaan opetetut bias arvot
b2 = df.to_numpy()                         # b2-shape = (6,1)


#neuronien arvo on painoarvot kertaa input plus bias
neuronit = relu(np.matmul(w1.transpose(),a0) + b1)

neuronit = softmax(np.matmul(w2.transpose(),neuronit) + b2)


print(np.argmax(neuronit))