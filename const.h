#include <bits/stdc++.h>
using namespace std;
#ifndef __CONST__
#define __CONST__

#define PATH_TRACING
#define DEPTH_OF_FIELD

const string output_path = "./03.ppm";
const string debug_path = "tt1.out";
const string mesh_path = "./kitten.obj";
const string image_path = "./image.out";
//const string mesh_path = "./test.obj";

#ifdef PATH_TRACING
const int trace_time = 80; // 漫反射追踪的光线数量（随光强呈正比减小）
const double layer_inc = 3; // 搜索树中后一层比前一层增加的光线数量
#endif // PATH_TRACING


#ifdef DEPTH_OF_FIELD // 景深
const double focal_length = 15.0; // 焦距
const double lens_diameter = 0.5; // 光圈直径，0时为没有景深
const double ray_num = 30; // 每个像素的光线数量
#endif // DEPTH_OF_FIELD

const double PI = acos(-1);
const double bias = 1e-5; // 反射与折射光线射出位置偏移量
const int color_range = 255; // 最大颜色值
const double eps = 1e-7; // 判断是否垂直时eps
const double inf = 1e20; 
const int max_depth = 5; // 光线追踪最大深度
const double min_prop = 1e-9; // 最小能接受的比率

#endif // __CONST__
