#ifndef __ENTITY__
#define __ENTITY__

#include "basic.h"

class LightRay
{
public:
	Ray3 ray;
	Color color;
	LightRay() {}
	LightRay(Ray3 _ray, Color _color) : ray(_ray), color(_color) {}	
};

class Material
{
public:
	double d_reflect; // 漫反射率
	double ior; // 折射系数
	double rhod; // 漫反射耗散项

	Material() {}
	Material(double _d_reflect, double _ior, double _rhod) : 
	d_reflect(_d_reflect), ior(_ior), rhod(_rhod) {} 
};

class Object3
{
public:
	Material compose;
	Color color;
	Color emit; // 发光颜色

	Object3() {}
	Object3(Material _compose, Color _color, Color _emit) :
	compose(_compose), color(_color), emit(_emit) {}

	virtual bool inside(const Point3 &p) = 0; // p点是否在内部
    virtual InsertInfo insertLight(Ray3 ray, bool pre_inside) = 0; // 光线与物体相交信息(第一个交点)
	virtual pair<Color, Color> getColorAndEmit(Point3 p); // 返回p点颜色和发光信息
};

class RawPlane3 // 元平面
{
public:
	Ray3 normal; // 平面法向量
	
	RawPlane3() {}
	RawPlane3(Ray3 _normal) : normal(_normal) {}

	bool onPlane(const Point3 &p);
	InsertInfo insertRay(Ray3 ray);
};

class Plane3 : public Object3, public RawPlane3
{
public:

	Plane3() {}
	Plane3(Material _compose, Color _color, Color _emit, Ray3 _normal) :
	Object3(_compose, _color, _emit), RawPlane3(_normal) {}

	virtual bool inside(const Point3 &p) override;
	virtual InsertInfo insertLight(Ray3 ray) override;
};

class Square3 : public Plane3
{
public:
	int w_pixel, h_pixel; // 图像所占像素
	Ray3 x_dir, y_dir; // x,y轴正方向 
	Point3 dl_corner, ur_corner; // 左下角与右上角坐标
	double w_len, h_len; // 图像真实长宽
	Color **p_color; // 每个像素颜色
	Color **p_emit; // 每个像素发光
	double x_interval, y_interval; // x,y每个像素宽度

	void setColor(int x, int y, Color c) {p_color[x][y] = c;}
	void setEmit(int x, int y, Color c) {p_emit[x][y] = c;}
	bool inSquare(Point3 p);
	Square3(Material _compose, Color _color, Color _emit, Ray3 _normal, 
			int _w_pixel, int _h_pixel, double _w_len, double _h_len, Ray3 _x_dir, bool flip_y = false);
	virtual pair<Color, Color> getColorAndEmit(Point3 p) override;
	~Square3();
};

class Sphere : public Object3
{
public:
	Point3 center;
	double radius;
	double radius2;
	
	Sphere(Material _compose, Color _color, Color _emit, Point3 _center, double _radius);
	Vector3 normal(const Point3 &p); // p点向外法向
	virtual bool inside(const Point3 &p) override;
	bool onSphere(const Point3 &p);
	virtual InsertInfo insertLight(Ray3 ray) override;
	void print() {cerr << "center :" << center << "radius : " << radius << endl;}
};

class localLightProc // 处理一次trace中其他光的方向等信息
{
public:
	Ray3 normal;
	const Material *material;
	bool inside;
	Ray3 raydir; // trace的光线方向
	Color color; // 物体照射处颜色
	Color emit; // 物体照射处发光
	Vector3 reflect_dir; 
	Vector3 refract_dir;
	double emit_prop; // 发光所乘比例
	double reflect_prop; // 反射比例
	double refract_prop; // 折射比例
	int diff_num; // 漫反射追踪光线数
	vector<Vector3> diff_dir; // 漫反射的追踪方向
	vector<double> frac; // 每条漫反射光线的比例
	Point3 p0;
	Vector3 z_dir, x_dir, y_dir; // p0为原点（normal.start），局部坐标系中x轴与y轴方向（z轴方向为normal方向）

	localLightProc(Object3* obj, const InsertInfo& info, const Ray3 &ray, int num, bool only_emit = false);
};

#endif // __ENTITY__
