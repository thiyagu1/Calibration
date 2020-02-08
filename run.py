import numpy as np
import cv2
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def camera_center(P):
    """Extract camera projection center from projection matrix P."""
    print(P)
    u, s, vt = np.linalg.svd(P)
    print("u", u)
    print("s", s)
    print("vt", vt)
    print("Final_return", vt[3, :] )
    return vt[3, :]

def x_matrix(a):
    """Return cross product matrix of a."""
    a0 = a[0]
    a1 = a[1]
    a2 = a[2]
    return np.array([[0, -a2, a1],
                     [a2, 0, -a0],
                     [-a1, a0, 0]])

def fmatrix_from_cameras(P1, P2):
    """Fundamental matrix between the projection matrices P1 and P2."""
    # Hartley & Zisserman (page 246, section 9.2.4 Properties of the fundamental matrix)
    C1 = camera_center(P1)
    #print("C1 : ", C1)
    e2 = P2 @ C1
    #print("E2 : ", e2)
    #print ("Matrix E2 : ", x_matrix(e2))
    #print("Matrix P2: ", P2)
    #print("Matrix inv: ", np.linalg.pinv(P1))
    return x_matrix(e2) @ P2 @ np.linalg.pinv(P1)

class Ray:

    def __init__(self, point, direction):
        self.point = point
        self.direction = direction

    def distance_to_point(self, q):
        p = self.point
        n = self.direction
        c = p - q
        #print("P :", q)
        nearest_point = p - ((c.dot(n)) / (n.dot(n))) * n
        print("NP", nearest_point)
        print("Q", q)
        return np.linalg.norm(nearest_point - q)

def triangulate(P1, x1, P2, x2):
    """Triangulate the euclidean image points x1 and x2.
    Return the triangulated euclidean 3d point."""

    # Hartley & Zisserman (page 312, section 12.2 Linear triangulation methods)

    assert(len(x1) == 2)
    assert(len(x2) == 2)
    A = np.zeros((4, 4), dtype=np.float64)
    x = x1[0]
    y = x1[1]
    xp = x2[0]
    yp = x2[1]
    A[0, :] = x * P1[2, :] - P1[0, :]
    A[1, :] = y * P1[2, :] - P1[1, :]
    A[2, :] = xp * P2[2, :] - P2[0, :]
    A[3, :] = yp * P2[2, :] - P2[1, :]
    #print("Triangulation : ", A)
    u, s, vt = np.linalg.svd(A)
    p = vt[3, :]
    #print("Vt : ", vt)
    #print("P : ", (p/p[3])[:3])
    return (p / p[3])[:3]

def backprojection_ray(P, x):
    """Backprojection ray from projection matrix P and image point x."""
    # Hartley & Zisserman (page 162, section 6.2.2 Action of a
    # projective camera on points)
    assert(len(x) == 2)
    #print("P", P)
    #print("x", x)
    M = P[:, :3]
    p4 = P[:, 3]
    #print("M", M)
    #print("p4", p4)
    point = -np.linalg.inv(M) @ p4
    #print("point", point)

    direction = np.linalg.inv(M) @ np.array([x[0], x[1], 1])
    #print("DIRECTION", direction)
    #print("DIRECTION1", np.linalg.norm(direction))
    direction = direction / np.linalg.norm(direction)
    return Ray(point, direction)

def main():

    left = cv2.imread(r"C:\Users\varjo\Desktop\Calibration_Hedy\SimulatorAruCo3_2020_01_27\4_left.png")
    right = cv2.imread(r"C:\Users\varjo\Desktop\Calibration_Hedy\SimulatorAruCo3_2020_01_27\4_right.png")

    K = np.array([[1.13063025 * 1008, 0, 0.5 * 1008],
                  [0, 1.13063025 * 1008, 0.5 * 1008],
                  [0, 0, 1]])

    #print(K)



    params = cv2.aruco.DetectorParameters_create()
    params.cornerRefinementMethod = cv2.aruco.CORNER_REFINE_APRILTAG

    dictionary = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_5X5_1000)
    corners, ids, _ = cv2.aruco.detectMarkers(left, dictionary, parameters = params)
    corners2, ids2, _ = cv2.aruco.detectMarkers(right, dictionary, parameters = params)

    #print(ids, corners)

    # Pick upper left corner for validation.
    corners = np.array([c[0, 0, :] for c in corners])
    corners2 = np.array([c[0, 0, :] for c in corners2])

    # XXX debug
    #corners = np.array([c + np.random.normal(scale=1, size=2) for c in corners])

    ids = ids.flatten()
    ids2 = ids2.flatten()

    index_order = np.argsort(ids)
    index_order2 = np.argsort(ids2)
    corners = corners[index_order, :]
    corners2 = corners2[index_order2, :]
    ids = ids[index_order]
    ids2 = ids2[index_order2]

    for c in corners:
        cv2.circle(left, (int(c[0]), int(c[1])), 3, (255, 0, 0), -1)

    for c in corners2:
        cv2.circle(right, (int(c[0]), int(c[1])), 3, (255, 0, 0), -1)

    R1 = np.eye(3)
    T1 = np.zeros(3)

    R2 = np.eye(3)
    T2 = np.array([0.064, 0, 0])

    P1 = np.zeros((3, 4), dtype=np.float)
    P1[:, :3] = R1
    P1[:, 3] = T1

    P2 = np.zeros((3, 4), dtype=np.float64)
    P2[:, :3] = R2
    P2[:, 3] = T2

    P1 = K @ P1
    P2 = K @ P2

    F = fmatrix_from_cameras(P1, P2)
    print("F : ", F)
    # -----------------------------------------------------------------------------------------------------------------


    corners = corners.reshape((1, 70, 2))
    corners2 = corners2.reshape((1, 70, 2))

    #print(corners.shape)
   # print(F.flatten())
    F /= np.linalg.norm(F.flatten())
    #print(F)
    #cornersh, cornersh2 = cv2.correctMatches(F, np.copy(corners), np.copy(corners2))
    cornersh = corners.copy()
    cornersh2 = corners2.copy()
    #print(cornersh[0, :, :])
    # Reshape into a nicer form
    cornersh = cornersh[0, :, :]
    cornersh2 = cornersh2[0, :, :]
    corners = corners[0, :, :]
    corners2 = corners2[0, :, :]

    # Triangulate corners into 3d points
    points = np.array([triangulate(P1, x, P2, y) for x, y in zip(cornersh, cornersh2)])

    corners = corners.astype(np.float64)
    corners2 = corners2.astype(np.float64)
    cornersh = cornersh.astype(np.float64)
    cornersh2 = cornersh2.astype(np.float64)

    # Plot triangulated 3d points
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.scatter(points[:, 0], points[:, 1], points[:, 2], '.')
    ax.set_xlabel("X")
    ax.set_ylabel("Y")
    ax.set_zlabel("Z")
    ax.set_xlim(-0.5, 0.5)
    ax.set_ylim(-0.5, 0.5)
    ax.set_zlim(-1, -2)
    plt.show()

    # Compute reprojection errors
    reprojection_errors1 = []
    reprojection_errors2 = []
    for x, y, p in zip(corners, corners2, points):
        #p = triangulate(P1, x, P2, y)
        xh = P1 @ np.array([p[0], p[1], p[2], 1])
        #print("old:", xh)
        xh = (xh / xh[2])[:2]
        #print("new:", xh)
        yh = P2 @ np.array([p[0], p[1], p[2], 1])
        yh = (yh / yh[2])[:2]
        #print(x, xh)
        print("REEL : ", np.linalg.norm(x - xh))
        print("REER : ", np.linalg.norm(y - yh))
        reprojection_errors1.append(np.linalg.norm(x - xh, ord=2))
        reprojection_errors2.append(np.linalg.norm(y - yh, ord=2))

    # Plot reprojection errors
    plt.plot(ids, reprojection_errors1)
    plt.plot(ids2, reprojection_errors2)
    plt.legend(["Left", "Right"])
    plt.ylabel("Reprojection error (pixels)")
    plt.show()

    # Compute error from triangulated point to back projected rays.
    distances1 = []
    distances2 = []
    for x, y, p in zip(corners, corners2, points):

       #print("xxx", x)
        ray_x = backprojection_ray(P1, x)
        ray_y = backprojection_ray(P2, y)
        #print("RPoint", ray_y.point)
        #print("RDirection", ray_y.direction)

        d1 = ray_x.distance_to_point(p)
        d2 = ray_y.distance_to_point(p)
        print("d1", d1)
        print("d2", d2)
        distances1.append(d1)
        distances2.append(d2)

    distances1 = np.array(distances1)
    distances2 = np.array(distances2)
    # Convert to mm
    distances1 = distances1 * 1000.0
    distances2 = distances2 * 1000.0
    plt.plot(distances1)
    plt.plot(distances2)
    plt.title("Triangulation error in 3d space")
    plt.ylabel("Error (mm)")
    plt.show()


    cv2.imshow("left", left)
    cv2.imshow("right", right)
    cv2.waitKey(0)



if __name__ == '__main__':
    main()