import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

def relu(x):
	return(np.maximum(x,0)) 

def softmax(x):
    return np.exp(x)/np.sum(np.exp(x))

def createArrayData(data):
    dataString = ""
    #array näyttää tältä: arr[i][j] = { {},{},{} };, sisempiä i kpl joissa j kpl elementtejä
    #tämä funktio luo sisemmät, eli {1,2,3},{4,5,6},{7,8,9} jne
    for i in range(len(data)):
        dataString += "{"

        for j in range(len(data[i])):


            if j >= len(data[i])-1:
                dataString = dataString + str(data[i][j])
            else:
                dataString = dataString + str(data[i][j]) + ","
                
        
        if i >= len(data)-1:
            dataString += "}"
        else:
            dataString += "},"

    
    return dataString

def dataToHeader(w1,w2,b1,b2):
    data_w1 = createArrayData(w1)
    data_w2 = createArrayData(w2)
    data_b1 = createArrayData(b1)
    data_b2 = createArrayData(b2)
                                       
    str_w1 = "float str_w1["+ str(w1.shape[0]) +"]["+ str(w1.shape[1]) +"] = {"
    str_w2 = "float str_w2["+ str(w2.shape[0]) +"]["+ str(w2.shape[1]) +"] = {"
    str_b1 = "float str_b1["+ str(b1.shape[0]) +"]["+ str(b1.shape[1]) +"] = {"
    str_b2 = "float str_b2["+ str(b2.shape[0]) +"]["+ str(b2.shape[1]) +"] = {"

    dataString = "#ifndef NEUROVERKONKERTOIMET_H\n#define NEUROVERKONKERTOIMET_H\n"
    dataString = dataString + str_w1 + data_w1 + "};\n" + str_w2 + data_w2 + "};\n" + str_b1 + data_b1 + "};\n" + str_b2 + data_b2 + "};\n#endif"

    with open("neuroverkonKertoimet.h","w") as f:
        f.write(dataString)

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

print(createArrayData(w1))

dataToHeader(w1,w2,b1,b2)
