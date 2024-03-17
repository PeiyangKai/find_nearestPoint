#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <sstream>
#include "KDTree.h"
using namespace std;


std::pair<int, double> find_nearest_points_traverse(Point2D<double> position, std::vector<Point2D<double>>& path, std::pair<int, int> index_range = { 0, -1 }) {
    // 输入参数
    // position: [x,y]
    // path: [x1,y1;x2,y2;...;xn,yn]
    // index_range: [range_upper,range_lower] range_upper>1,range_lower<size(path,1)
    // 输出参数
    // nearest_index 最近点索引 nearest_dis 最近点距离
    if (index_range.second == -1) {
        index_range.second = path.size();
    }
    int start_index = index_range.first;
    int end_index = index_range.second;
    std::vector<Point2D<double>> new_path(path.begin() + start_index, path.begin() + end_index);
    double nearest_dis = std::numeric_limits<double>::max();
    int nearest_index = start_index;
    for (int i = 0; i < new_path.size(); i++) {
        double distance = pow(new_path[i].x - position.x, 2) + pow(new_path[i].y - position.y, 2);
        if (distance < nearest_dis) {
            nearest_dis = distance;
            nearest_index = start_index + i;
        }
    }
    nearest_dis = sqrt(nearest_dis);
    return std::make_pair(nearest_index, nearest_dis);
}


std::pair<int, double> find_nearest_points_KDTree(KDTree<Point2D<double>>& kdt, Point2D<double> position, std::pair<int, int> index_range = { 0, -1 })
{
    // 输入参数
    // position: [x,y]
    // path: [x1,y1;x2,y2;...;xn,yn]
    // index_range: [range_upper,range_lower] range_upper>1,range_lower<size(path,1)
    // 输出参数
    // nearest_index 最近点索引 nearest_dis 最近点距离
    if (index_range.second == -1) {
        index_range.second = kdt.NodeNums;
    }
    int searchKPoint = 10;
    bool searchFlag = true;
    while (searchFlag) {
        if (searchKPoint > kdt.NodeNums) {
            searchKPoint = kdt.NodeNums;
            searchFlag = false;
        }
        auto nearestKPoint = kdt.findNearestPoint(position, searchKPoint);
        for (auto pt : nearestKPoint) {
            if (pt.first >= index_range.first && pt.first < index_range.second) {
                return pt;
            }
        }

        searchKPoint = searchKPoint * 10;

    }
}


void read_path(std::vector<Point2D<double>>& data, string fileName) {
    std::ifstream file(fileName);
    std::string line;
    Point2D<double> pt = {0,0,0};
    int id = 0;
    // 读取第一行（列名）但不处理
    std::getline(file, line);
    // 逐行读取文件
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        std::vector<std::string> tokens;

        // 逐个读取逗号分隔的单元格
        while (std::getline(ss, cell, ',')) {
            tokens.push_back(cell);
        }

        // 检查是否有足够的数据（至少包含 x 和 y）
        if (tokens.size() >= 2) {
            // 将 x 和 y 值转换为浮点数
            pt.x = std::stod(tokens[0]);
            pt.y= std::stod(tokens[1]);
            pt.index = id++;
            data.push_back(pt);
        }
    }
}


 

int main() {

    //读取数据
    std::vector<Point2D<double>> path;
    read_path(path, "E:\\Desktop\\blog\\road.csv");

    //搜索目标
    Point2D<double> position = { -550.9,37.5 };
    std::pair<int, int> index_range = { 0, 6500 };
    std::pair<int, double> nearest_point;
    //暴力遍历搜索
    auto t1 = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch())
        .count();
    for (int i = 0; i < 1000; i++) {
        nearest_point = find_nearest_points_traverse(position, path);

        //nearest_point = find_nearest_points_traverse(position, path, index_range);
    }
    auto t2 = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch())
        .count();
    cout << "最近点索引： " << nearest_point.first << " 最近点距离： " << nearest_point.second << endl;
    cout << "遍历搜索耗时： " << t2 - t1 << " ms" << endl;

    //KD树搜索
    auto t3 = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch())
        .count();
	int dim = 2;
    KDTree<Point2D<double>> kdt(path, dim);
    auto t4 = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch())
        .count();
    cout << "构建KD树耗时： " << t4 - t3 << " ms" << endl;




    auto t5 = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch())
        .count();
    vector<pair<int, double>> nearestKPoint;
    for (int i = 0; i < 1000; i++) {
        nearest_point = find_nearest_points_KDTree(kdt, position);

       // nearest_point = find_nearest_points_KDTree(kdt,position, index_range);
    }
    auto t6 = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch())
        .count();
    cout << "最近点索引： " << nearest_point.first << " 最近点距离： " << nearest_point.second << endl;
    cout << "KD树搜索耗时： " << t6 - t5 << " ms" << endl;



	return 0;
}