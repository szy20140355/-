#include <bits/stdc++.h>
using namespace std;
#ifndef __CONST__
#define __CONST__

const string output_path = "./02.ppm";
const string debug_path = "tt1.out";

#define PATH_TRACING
#ifdef PATH_TRACING
const int trace_time = 100000; // 漫反射追踪的光线数量（随光强呈正比减小）
const double layer_inc = 10.0; // 搜索树中后一层比前一层增加的光线数量
#endif // PATH_TRACING

const double PI = acos(-1);
const double bias = 1e-5; // 反射与折射光线射出位置偏移量
const int color_range = 255; // 最大颜色值
const double eps = 1e-7; // 判断是否垂直时eps
const double inf = 1e20; 
const int max_depth = 5; // 光线追踪最大深度
const double min_prop = 1e-9; // 最小能接受的比率

#endif // __CONST__
