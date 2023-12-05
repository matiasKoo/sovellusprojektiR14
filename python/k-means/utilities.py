import socket
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

winRecord = np.zeros((6,4))

def randomCenterPoint(low,high):
    return np.random.randint(low,high,3)

def initializeCenterPoints(n,parameters):
    #luodaan tyhjä taulukko joka on muotoa n kertaa parameters (= mittaustulosten
    #lukumäärä per tieto)
    cp = np.zeros((n,parameters))
    for i in range(n):
        cp[i,:]=randomCenterPoint(1200,1700)
    
    return cp

def calculateDistance(p1,p2):
    return np.sqrt(np.sum((p1-p2)**2))

def getClosest(p,cp):
    #tämä on huonosti tehty, mutta tehty kuitenkin
    smallest_dist = 10000000
    smallest_index = 0

    for i in range(cp.shape[0]):
        dist = calculateDistance(p,cp[i])
        if dist < smallest_dist :
            smallest_dist = dist
            smallest_index = i
    
    return smallest_index,cp[smallest_index]

def averagePoints(pisteet):
    #otetaan keskiarvo uusille keskipisteille ja uudet satunnais arvot pisteille joita ei valittu
    #pisteet = np.zeros((6,3))
    for i in range(6):
        if winRecord[i,3] != 0:
            pisteet[i] = winRecord[i,:3] / winRecord[i,3]
        else:
            pisteet[i] = randomCenterPoint(1200,1700)
    
    return(pisteet)

def recordWinningPoint(winner,coordinate):
    # lisätään voittaneen pisteen tiedot taulukkoon
    winRecord[winner,:3] = winRecord[winner,:3] + coordinate
    winRecord[winner,3]  += 1

def getData():
    #yhdistetään tietokantaan ja lähetetään ryhmänro
    #takaisin tulee kaikki ryhmänroa vastaavat datat
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(('172.20.241.9', 20000))
    s.sendall(b'14\n')

    chunks = []
    while True:
        data = s.recv(1024)
        if len(data) == 0:
            break
        chunks.append(data.decode('utf-8').replace(" ", ",")) #replacella csv-muoto

    #alustaa tiedoston tyhjäksi
    open("py_data.csv","w").close()

    for i in chunks:

        f = open("py_data.csv", "a")
        f.write(i)


    s.close()

def teachingLoop():
    #tää data ois fiksumpi kerätä aliohjelman ulkopuolella, mutta se on voi voi
    data = pd.read_csv("py_data.csv",header=None).iloc[:,6:9].to_numpy()
    #pythonissa globaalit muuttujat toimii näin ":D"
    global winRecord
    global centerPoints
    for i in range(len(data)):
        smallestIndex = getClosest(data[i],centerPoints)[0]
        closestPoint = getClosest(data[i],centerPoints)[1]
        recordWinningPoint(smallestIndex,data[i])
    
    #print(centerPoints)
    #visualize(centerPoints)
    centerPoints = averagePoints(centerPoints)
    winRecord = np.zeros((6,4))
    
def createCArray(centerPoints):
    dataString = ""

    for i in range(len(centerPoints)):

        for j in range(len(centerPoints[i])):
            if j == 0:
                dataString = dataString+"{"+str(centerPoints[i][j])+","
            if j == 1:
                dataString = dataString+str(centerPoints[i][j])+","
            if j == 2:
                dataString = dataString+str(centerPoints[i][j])+"}"
        if i == 5:
            return dataString
        else:        
            dataString = dataString+","
    
    #return dataString
        

def centerPointsToHeader():
    data = createCArray(centerPoints)

    dataString = "#ifndef KMEANS_H\n#define KMEANS_H\nint CP[6][3] = {"
    dataString = dataString + data + "};\n#endif"

    with open("kmeans.h","w") as f:
        f.write(dataString)

def visualize(cp):
    df = pd.read_csv("py_data.csv",header=None)

    a = df.iloc[:,6:9].to_numpy()


    fig, ax = plt.subplots(subplot_kw={"projection": "3d"})
    ax.scatter(a[:,0],a[:,1],a[:,2])
    ax.scatter(cp[:,0],cp[:,1],cp[:,2],color = "red")

    plt.show()

if __name__ == "__main__":


    
    getData()
    centerPoints = initializeCenterPoints(6,3)
    print(centerPoints)

    for i in range(10):
        teachingLoop()
        

    print(centerPoints)

    centerPointsToHeader()
    


    