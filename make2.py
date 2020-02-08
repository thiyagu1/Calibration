import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

a,b,c,d = 1,2,3,4

x = np.linspace(0,4,5)
print(x)
y = np.linspace(8,12,5)
print(y)

X,Y = np.meshgrid(x,y)
print("X : ", X)

print("Y : ", Y)
plt.plot(X, Y, marker='.', color='k', linestyle='none')
Z = (d - a*X - b*Y) / c
print("Z : ", Z)

fig = plt.figure()
ax = fig.gca(projection='3d')

surf = ax.plot_surface(X, Y, Z)
plt.show()