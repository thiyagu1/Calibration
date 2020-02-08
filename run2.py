import numpy as np
import cv2
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D



def x_matrix(a):
    """Return cross product matrix of a."""
    a0 = a[0]
    a1 = a[1]
    a2 = a[2]
    return np.array([[0, -a2, a1],
                     [a2, 0, -a0],
                     [-a1, a0, 0]])


left = cv2.imread(r"C:\Users\varjo\Desktop\Calibration_Hedy\SimulatorAruCo3_2020_01_27\4_left.png")
right = cv2.imread(r"C:\Users\varjo\Desktop\Calibration_Hedy\SimulatorAruCo3_2020_01_27\4_right.png")

K = np.array([[1.13063025 * 1008, 0, 0.5 * 1008],
              [0, 1.13063025 * 1008, 0.5 * 1008],
              [0, 0, 1]])

print(K)

params = cv2.aruco.DetectorParameters_create()
params.cornerRefinementMethod = cv2.aruco.CORNER_REFINE_APRILTAG

dictionary = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_5X5_1000)
corners, ids, _ = cv2.aruco.detectMarkers(left, dictionary, parameters=params)
corners2, ids2, _ = cv2.aruco.detectMarkers(right, dictionary, parameters=params)

# Pick upper left corner for validation.
corners = np.array([c[0, 0, :] for c in corners])
corners2 = np.array([c[0, 0, :] for c in corners2])

print(ids2)

ids2 = ids2.flatten()

print(ids2)

index_order = np.argsort(ids)
index_order2 = np.argsort(ids2)
corners = corners[index_order, :]
corners2 = corners2[index_order2, :]
ids = ids[index_order]
ids2 = ids2[index_order2]
print(ids2)

'''
for c in corners:
    cv2.circle(left, (int(c[0]), int(c[1])), 3, (255, 0, 0), -1)

for c in corners2:
    cv2.circle(right, (int(c[0]), int(c[1])), 3, (255, 0, 0), -1)

'''

R1 = np.eye(3)
T1 = np.zeros(3)

print(R1, T1)

R2 = np.eye(3)
T2 = np.array([0.064, 0, 0])

print(R2, T2)

P1 = np.zeros((3, 4), dtype=np.float)
P1[:, :3] = R1
P1[:, 3] = T1


P2 = np.zeros((3, 4), dtype=np.float64)
P2[:, :3] = R2
P2[:, 3] = T2

print(P1, P2)

P1 = K @ P1
P2 = K @ P2

print(P1)
print("P2", P2)

u, s, vt = np.linalg.svd(P2)

print("u",u)
print("s",s)
print("vt",vt)
print("vt",vt[3, :])

u, s, vt = np.linalg.svd(P2)
svd_v_1x4 = vt[3,:]
print(svd_v_1x4)

Camera_M_3x3 = P2[:,0:3]
print(Camera_M_3x3)

Camera_C4 = P2[:,3]
Camera_C4_3x1= Camera_C4.reshape(3,1)
print(Camera_C4_3x1)

Camera_M_inverse = np.linalg.inv(Camera_M_3x3)
Camera_center_3x1 =Camera_M_inverse @ Camera_C4_3x1
print(Camera_center_3x1)

Homo_CC= Camera_center_3x1.flatten()
CC= np.array([Homo_CC[0],Homo_CC[1],Homo_CC[2], 1])
print(CC)
camera_center_final = np.asmatrix(CC)
CCF= camera_center_final.reshape(4,1)
print("CCF :", CCF)

E_dash = P1 @ CCF
print("E_dash : ", E_dash)
flat  = np.squeeze(np.asarray(E_dash))
print("E_dash : ", flat)

print ("Matrix : ", x_matrix(flat))

Fundamental = x_matrix(flat) @ P2 @ np.linalg.pinv(P1)
print ("Matrix : ", Fundamental)
corners = corners.reshape((1, 70, 2))
#print ("corners : ", corners)
F = Fundamental.flatten()
print ("SQRT : ", F/np.sqrt(np.sum(np.absolute(np.square(F)))))
Fundamental = Fundamental/np.linalg.norm(F)
print ("Matrix : ", Fundamental)



'''
M = P2[:,0:3]
p4 = P2[:,3]

p4= p4.reshape(3,1)

print("M ", M.shape)
print("P4 ",p4.shape)

M_inverse = np.linalg.inv(M)
print("inverse ",M_inverse)

center =-M_inverse @ p4
print("center ",center.shape)
print("center ",center)

svd_data= pcc.reshape(4,1)
nomalised_scd = svd_data/svd_data[3]
print(pcc)
print(nomalised_scd)


e2 = P2 @ center
print("e2 ",e2)

mmm = P2 @ np.linalg.pinv(P1)
print(mmm.shape, e2.shape)

print ("Matrix : ", x_matrix(e2))

resd= x_matrix(e2) @ mmm
print("cross :", resd)

'''
