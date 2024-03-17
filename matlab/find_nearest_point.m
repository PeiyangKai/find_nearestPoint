function [nearest_index, nearest_dis]= find_nearest_point(position,path,index_range)
% 输入参数
% position: [x,y]
% path: [x1,y1;x2,y2;...;xn,yn]
% index_range: [range_upper,range_lower] range_upper>1,range_lower<size(path,1)
% 输出参数
% nearest_index 最近点索引 nearest_dis 最近点距离
if nargin < 3 || isempty(index_range)
    index_range = [1,size(path,1)];  % 如果没有提供 y 参数或者 y 参数为空，则使用默认值 10
end
start_index=index_range(1);
end_index=index_range(2);
new_path=path(start_index:end_index,1:2);
distance=(new_path(:,1)-position(1)).^2+(new_path(:,2)-position(2)).^2;
[nearest_dis,relative_index]=min(abs(distance));
nearest_dis=sqrt(nearest_dis);
nearest_index=start_index+relative_index-1;
end

%% 测试代码
% position=[5.1,5.1];
% path=[1,1;2,2;3,3;4,4;5,5;6,6;7,7;8,8];
% index_range=[6,7];
% [nearest_index, nearest_dis]= find_nearest_points_new(position,path,index_range);
% nearest_index
% nearest_dis