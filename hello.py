import numpy as np
import scipy.optimize

from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import pandas



fig = plt.figure()
ax = fig.gca(projection='3d')

def fitPlaneLTSQ(XYZ):
    (rows, cols) = XYZ.shape
    print(rows, cols)
    G = np.ones((rows, 3))
    print(XYZ)
    print(XYZ[0])
    G[:, 0] = XYZ[0]  # X
    G[:, 1] = XYZ[1]  # Y
    print(G)
    Z = XYZ[2]
    (a, b, c), resid, rank, s = np.linalg.lstsq(G, Z)
    print("a : ", a )
    print("b : ", b )
    print("c : ", c)
    print("Residual : ", resid)
    print("Rank : ", rank)
    print("Singular Value", s)

    normal = (a, b, -1)
    print("normal : ", normal)

    '''
    normala = abs(pow(a,2)+pow(b,2)+pow(-1,2))
    np.sqrt(normala)
    abb=normal/normala
    print("Normala : ",normala)
    print("Normala : ", abb)
    
    '''

    nn = np.linalg.norm(normal)
    print("nn : ", nn)
    normal = normal/nn

    print("Normal : ", normal)

    return (c, normal)





def calculate(df, normal):

    data = df
    normal1 = normal
    print("NEW : ")
    print(data)
    c, normal = fitPlaneLTSQ(data)
    print(c, normal)
    # plot fitted plane
    maxx = np.max(data[0])
    maxy = np.max(data[1])
    minx = np.min(data[0])
    miny = np.min(data[1])
    print(maxx,maxy, minx, miny)
    point = np.array([0.0, 0.0, c])
    print("Point : ", point)
    d = -point.dot(normal)
    print("D : ",  d)

    # plot original points
    ax.scatter(data[0], data[1], data[2])
    ax.quiver(data[0], data[1], data[2], normal1[0], normal1[1], normal1[2], length=0.2)
    # compute needed points for plane plotting
    xx, yy = np.meshgrid([minx, maxx], [miny, maxy])


    print(xx)
    print(yy)
    print("minx : ", minx)
    print("maxx : ", maxx)
    print("miny : ", miny)
    print("maxy : ", maxy)
    print("xx : ", xx)
    print("yy : ", yy)

    z = (-normal[0]*xx - normal[1]*yy - d)*1. / normal[2]

    unit1 = np.sqrt(pow(normal[0], 2) + pow(normal[1],2) + pow(normal[2],2))
    print("Unit 1 : ", unit1)
    Error = abs(normal[0]*data[0] + normal[1]*data[1] + normal[2]*data[2] + d)/unit1
    print("Error", Error)


    print("Z : ", z)
    # plot plane
    ax.plot_surface(xx, yy, z, alpha=0.2)
    ax.set_xlim(-1, 1)
    ax.set_ylim(-1,1)
    ax.set_zlim(1,2)
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('z')
    plt.show()
    return Error








#Import Data from CSV

Point_L = pandas.read_csv("C:/Users/varjo/source/repos/Varjo_Foundation/Varjo_Foundation/Logs/points_L.csv", header=None) # , names=['X', 'Y','Z']
print(Point_L)

Normal_Left = pandas.read_csv("C:/Users/varjo/source/repos/Varjo_Foundation/Varjo_Foundation/Logs/pose_left.csv", header=None) # , names=['X', 'Y','Z']
print(Normal_Left)

Error_L = calculate(Point_L, Normal_Left)
P_Error_L = pandas.DataFrame(Error_L)
print("Print : ", P_Error_L)
P_Error_L.to_csv("C:/Users/varjo/source/repos/Varjo_Foundation/Varjo_Foundation/Logs/Py_Error_L.csv")



Point_R = pandas.read_csv("C:/Users/varjo/source/repos/Varjo_Foundation/Varjo_Foundation/Logs/points_R.csv", header=None) # , names=['X', 'Y','Z']
print(Point_R)

Normal_Right = pandas.read_csv("C:/Users/varjo/source/repos/Varjo_Foundation/Varjo_Foundation/Logs/pose_right.csv", header=None) # , names=['X', 'Y','Z']
print(Normal_Right)

Error_R = calculate(Point_R, Normal_Right)
P_Error_R = pandas.DataFrame(Error_R)
print("Print : ", P_Error_R)
P_Error_R.to_csv("C:/Users/varjo/source/repos/Varjo_Foundation/Varjo_Foundation/Logs/Py_Error_R.csv")









#abc = pandas.read_csv("C:/Users/varjo/source/repos/Varjo_Foundation/Varjo_Foundation/Logs/abc.csv", header=None) # , names=['X', 'Y','Z']
#print(abc)

#standard normal distribution / Bell.
#np.random.seed(seed=1)

