# 寻找最近路径点

## 功能概述

给定当前位置`position:[x,y]`,给定路径点序列`path：[[x1,y1];[x2,y2];...;[xn,yn]],size=n*2`，返回`path`中距离`position`最近的路径点索引`nearest_index`与最短距离`nearest_dis`。另外还有1个可选参数，`index_range: [range_upper,range_lower]`,表示`nearest_index`所在的范围，其存在的意义有2个，1是可以减少搜索范围，从而减少计算量，2是为了应对交叉路径，防止最近点突变，具体情况如下图所示：

![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/52dec12019f545d98b68ea559912bf6d.png)

黑点为历史的位置轨迹，旁边的绿点为对应的最近路径点，蓝点为当前位置，大部分情况下（例如在自动驾驶轨迹跟踪任务中），我们希望找到的蓝点的最近点为旁边的绿点，但是如果仅用最近距离来判断则会返回红点，如果我们用`index_range`限制最近点的范围，则可以避免这种情况。如果不设置`index_range`,则默认返回最短距离的路径点。

## 具体实现

支持python\c++\matlab.

每种语言均提供了暴力遍历搜索和KDtree高效搜索两种搜索方式并进行了耗时对比，同时提供`index_range`参数设置来限制最近点的索引范围。

