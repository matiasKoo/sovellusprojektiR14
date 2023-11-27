import socket
import pandas as pd

def getData():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(('172.20.241.9', 20000))
    s.sendall(b'14\n')

    chunks = []
    while True:
        data = s.recv(1024)
        if len(data) == 0:
            break
        chunks.append(data.decode('utf-8').replace(" ", ","))

    open("py_data.csv","w").close()

    for i in chunks:

        f = open("py_data.csv", "a")
        f.write(i)

        print(i, end = '')

    s.close()


if __name__ == "__main__":
    getData()
    
