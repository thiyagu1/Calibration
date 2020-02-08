from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import pandas


fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

F3D= pandas.read_csv("C:/Users/varjo/source/repos/Varjo_Foundation/Varjo_Foundation/Logs/Final_3D.csv", header=None) # , names=['X', 'Y','Z']
print(F3D)
x =F3D[0]
y =F3D[1]
z =F3D[2]


ax.scatter(x, y, z, '.')
ax.set_xlabel("X")
ax.set_ylabel("Y")
ax.set_zlabel("Z")
ax.set_xlim(-0.5, 0.5)
ax.set_ylim(-0.5, 0.5)
ax.set_zlim(-1, -2)
plt.show()

'''
ax.set_xlim(-0.5,0.5)
ax.set_ylim(-1,1)
ax.set_zlim(-1,-2)

'''
