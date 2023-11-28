import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
df = pd.read_csv("py_data.csv",header=None)

a = df.iloc[:,6:9].to_numpy()



fig, ax = plt.subplots(subplot_kw={"projection": "3d"})
ax.scatter(a[:,0],a[:,1],a[:,2])
'''
ax.set(xticklabels=[],
       yticklabels=[],
       zticklabels=[])
'''
plt.show()
