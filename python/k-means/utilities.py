import socket
import pandas as pd
import numpy as np

winRecord = np.zeros((6,4))

def randomCenterPoint(low,high):
    return np.random.randint(low,high,3)

def initializeCenterPoints(n,parameters):
    #luodaan tyhjä taulukko joka on muotoa n kertaa parameters (= mittaustulosten
    #lukumäärä per tieto)
    cp = np.zeros((n,parameters))
    for i in range(n):
        cp[i,:]=randomCenterPoint(1000,2000)
    
    return cp

def calculateDistance(p1,p2):
    return np.sqrt(np.sum((p1-p2)**2))


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



if __name__ == "__main__":
    x = initializeCenterPoints(6,3)
    #recordWinningPoint(1,np.array([1,2,3]))
    
