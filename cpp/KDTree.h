#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <stack>
#include "BoundedPQueue.h"
// https://blog.csdn.net/weixin_42694889/article/details/124753575?ops_request_misc=&request_id=&biz_id=102&utm_term=KDtree%20c++&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduweb~default-3-124753575.142^v99^pc_search_result_base9&spm=1018.2226.3001.4187
template <typename T>
struct Point3D
{
	T x, y, z;
	int index;
	Point3D() : x(0), y(0), z(0), index(-1) {};
	Point3D(T x, T y, T z) : x(x), y(y), z(z), index(-1) {};
	Point3D(T x, T y, T z, int index) : x(x), y(y), z(z), index(index) {};
	inline T& operator[](int i) {
		return i == 0 ? x : i == 1 ? y : z;
	}
};

template <typename T>
struct Point2D
{
	T x, y;
	int index;
	Point2D() : x(0), y(0), index(-1) {};
	Point2D(T x, T y) : x(x), y(y), index(-1) {};
	Point2D(T x, T y, int index) : x(x), y(y), index(index) {};
	inline T& operator[](int i) {
		return i == 0 ? x : y;
	}
};

struct KDNode 
{
	int index; //该节点存储数据点的索引
	int axis; //划分的维度
	KDNode* left;
	KDNode* right;
	KDNode(int index, int axis, KDNode* left = nullptr, KDNode* right = nullptr) {
		this->index = index;
		this->axis = axis;
		this->left = left;
		this->right = right;
	}
};

template <typename T>
class KDTree
{
private:
	int ndim; //维度
	KDNode* root;
	KDNode* build(std::vector<T>&); //构建KDTree
	std::set<int> visited; //记录回溯搜索时是否已访问
	std::stack<KDNode*> stackNode;
	std::vector<T> m_data;

	void release(KDNode*);
	void printNode(KDNode*);
	int chooseAxis(std::vector<T>&);//选择划分的轴
	void dfs(KDNode*, T);//深度优先搜索
	//计算点之间的距离
	inline double distanceT(KDNode*, T);
	inline double distanceT(int, T);
	//点于超平面的距离
	inline double distanceP(KDNode*, T);
	//判断父节点的超平面是否在超球体中
	inline bool checkParent(KDNode*, T, double);
public:
	int NodeNums;
	KDTree(std::vector<T>&,int dim);
	~KDTree();
	void Print();
	std::vector<std::pair<int, double>> findNearestPoint(T point, int k=1);
};

//构造函数
template <typename T> //T可以是Point3D，也可以是Point2D
KDTree<T>::KDTree(std::vector<T>& data, int dim) {
	this->ndim = dim;
	NodeNums = data.size();
	this->m_data = data;//拷贝原始数据
	root = build(data); 
}

//析构函数
template <typename T>
KDTree<T> ::~KDTree() {
	release(root);
}

template <typename T>
void KDTree<T>::Print() {
	printNode(root);
}

template <typename T>
void KDTree<T>:: release(KDNode* node) {
	if (node == nullptr) {
		return;
	}
	release(node->left);
	release(node->right);
	delete node;
	node = nullptr;
}

template <typename T>
KDNode*  KDTree<T>:: build(std::vector<T>& data) {
	if (data.empty()) return nullptr;
	std::vector<T> temp = data;
	int mid_index = static_cast<int>(data.size() / 2);//数据索引中间值
	int axis = data.size() > 1 ? chooseAxis(temp) : -1; // 根据每个维度的方差大小选择二分的维度,叶子结点无法二分，默认为-1
	std::sort(temp.begin(), temp.end(), [axis](T a, T b) {return a[axis] < b[axis]; }); //这里用了一个仿函数
	std::vector<T> leftData, rightData;
	leftData.assign(temp.begin(), temp.begin() + mid_index);
	rightData.assign(temp.begin() + mid_index + 1, temp.end());
	KDNode* leftNode = build(leftData);
	KDNode* rightNode = build(rightData);
	
	KDNode* root =new KDNode(temp[mid_index].index, axis, leftNode, rightNode);
	return root;

}


template <typename T>
void KDTree<T>::printNode(KDNode* node) {
	if (node) {
		std::cout << "Index: " << node->index << "\tAxis: " << node->axis << std::endl;
		printNode(node->left);
		printNode(node->right);
	}
}

template <typename T>
int KDTree<T>::chooseAxis(std::vector<T>& data) {
	int axis = -1;
	double max_var = -1;


	for (int i = 0; i < ndim; i++) {

		//计算均值
		double mean = 0;
		for (auto j : data) {
			mean += static_cast<double>(j[i]);
		}
		mean = mean / static_cast<double>(data.size());

		//计算方差
		double var = 0;
		for (auto j : data) {
			var += (static_cast<double>(j[i]) - mean) *(static_cast<double>(j[i]) - mean);
		}
		var = var / static_cast<double>(data.size());
		if (var > max_var) {
			max_var = var;
			axis = i;
		}
	}
	return axis;
}
template <typename T>
inline double KDTree<T>::distanceT(KDNode* node, T point)
{
	double dis = 0;
	for (int i = 0; i < ndim; i++) {
		dis += (m_data[node->index][i] - point[i]) * (m_data[node->index][i] - point[i]);
	}
	dis = sqrt(dis);
	return dis;
}

template <typename T>
inline double KDTree<T>::distanceT(int index, T point) {
	double dis = 0;
	for (int i = 0; i < ndim; i++) {
		dis += (m_data[index][i] - point[i]) * (m_data[index][i] - point[i]);
	}
	dis = sqrt(dis);
	return dis;
}

template <class T>
double KDTree<T>::distanceP(KDNode* node, T point) {
	int axis = node->axis;
	double dis = m_data[node->index][axis] - point[axis];
	return abs(dis);

}

template <class T>
bool KDTree<T>::checkParent(KDNode* node, T pt, double distT)
{
	double dis = distanceP(node, pt);
	return dis <= distT;
}

template <typename T>
std::vector<std::pair<int, double>> KDTree<T>::findNearestPoint(T point,int k) {
	visited.clear();
	while (!stackNode.empty()) 
	{
		stackNode.pop();
	}//清空stackNode
	BoundedPQueue<int> pQueue(k); // BPQ with maximum size k

	double min_dist = DBL_MAX;
	int resNodeIdx = -1;
	dfs(root, point);
	while (!stackNode.empty()) {
		KDNode* curNode = stackNode.top();
		stackNode.pop();
		double dist = distanceT(curNode, point);
		pQueue.enqueue(curNode->index, dist);

		if (!stackNode.empty()) {
			KDNode* parentNode = stackNode.top();
			int parentAxis = parentNode->axis;
			int parentIndex = parentNode->index;
			if (pQueue.size() < pQueue.maxSize() || checkParent(parentNode, point, pQueue.worst()))//如果最短半径超球体与分割超平面相交了
			{
				if (m_data[curNode->index][parentAxis] < m_data[parentIndex][parentAxis]) {
					dfs(parentNode->right, point);
				}
				else {
					dfs(parentNode->left, point);
				}

			}
		}
	}
	std::vector<std::pair<int,double>> NearestKPointIndex;
	while (!pQueue.empty()) {
		NearestKPointIndex.push_back(pQueue.dequeueMin());
	}
	return NearestKPointIndex;
}


template <class T>
void KDTree<T>::dfs(KDNode* node, T pt) 
{
	if (node) {
		if (visited.find(node->index) != visited.end())
			return;//访问过了
		stackNode.push(node);
		visited.insert(node->index);
		if (pt[node->axis] <= m_data[node->index][node->axis] && node->left)
			dfs(node->left,pt);
		else if(pt[node->axis] > m_data[node->index][node->axis] && node->right)
		{
			dfs(node->right,pt);
		}
		else if ((node->left == nullptr) ^ (node->right == nullptr))
		{
			dfs(node->left, pt);
			dfs(node->right, pt);
		}
	}
}