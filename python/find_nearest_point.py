import pandas as pd
import numpy as np
from scipy.spatial import KDTree
import time
# KDTree搜索
def find_nearest_points_KDTree(kdtree,point,index_range=None):
    if index_range is None:
        nearest_dist, nearest_idx = kdtree.query(point)
        return nearest_dist, nearest_idx
    else:
        searchFlag = True;
        k_neighbors =10
        while(searchFlag):
            if(k_neighbors > len(kdtree.data)):
                k_neighbors = len(kdtree.data)
                searchFlag=False
            nearest_dist, nearest_idx = kdtree.query(point,k_neighbors)
            for i in range(len(nearest_idx)):
                if nearest_idx[i]>=index_range[0] and nearest_idx[i]<index_range[1]:
                    return nearest_dist[i], nearest_idx[i]
            k_neighbors = k_neighbors*10

# 暴力遍历搜索
def find_nearest_points_traverse(path,position, index_range=None):
    # 输入参数
    # position: [x,y] 类型 np.ndarray
    # path: [x1,y1;x2,y2;...;xn,yn] 类型 np.ndarray
    # index_range: [range_upper,range_lower] range_upper>1,range_lower<size(path,1) 类型 np.ndarray
    # 输出参数
    # nearest_index 最近点索引 nearest_dis 最近点距离
    if index_range is None:
        index_range = [0, path.shape[0]]
    start_index, end_index = index_range
    new_path = path[start_index:end_index, :2]
    distance = np.sum((new_path - position) ** 2, axis=1)
    nearest_index = start_index + np.argmin(distance)
    nearest_dis = np.sqrt(np.min(distance))
    return nearest_dis, nearest_index             

            
if __name__ == '__main__':
    # 读取数据
    df=pd.read_csv('road.csv',header=0)
    path=df[['x','y']].values
    # 构建KDtree
    t1=time.time()
    kdtree = KDTree(path)
    t2=time.time()
    print("kdtree构建耗时:",1000*(t2-t1),"ms")
    # 查询点
    point = [-550.9,37.5]
    t3=time.time()
    for i in range(1000):
        nearest_dist, nearest_idx = find_nearest_points_KDTree(kdtree,point,index_range=[0,6500])
        # nearest_dist, nearest_idx = find_nearest_points_KDTree(kdtree,point)
    t4=time.time()
    print("kdtree搜索耗时:",1000*(t4-t3),"ms")

    print("Nearest point index:", nearest_idx, "Distance:", nearest_dist)

    t5=time.time()
    for i in range(1000):
        nearest_dist, nearest_idx = find_nearest_points_traverse(path,point,index_range=[0,6500])

        # nearest_dist, nearest_idx = find_nearest_points_traverse(path,point)
    t6=time.time()
    print("遍历搜索耗时:",1000*(t6-t5),"ms")
    
    print("Nearest point index:", nearest_idx, "Distance:", nearest_dist)


