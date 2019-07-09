#ifndef __BASIC__
#define __BASIC__

#include "const.h"
#include <bits/stdc++.h>
using namespace std;

class Array3;

class Unit3
{
public:
	double x, y, z;
	Unit3(){x = 0, y = 0, z = 0;}
	Unit3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
	double& getDim(int c);
	operator Array3();
};

class Point3;
class Vector3;

class Point3 : public Unit3
{
public:
	using Unit3::Unit3;
	Point3 operator + (const Vector3 &v) const;
	Point3 operator + (const Point3 &p) const;
	Point3 operator - (const Vector3 &v) const;
	Vector3 operator - (const Point3 &p) const;
	Point3 operator * (const double c) const;
	Point3 operator / (const double c) const;
	friend ostream& operator << (ostream& out, const Point3& s);
	double dot(const Vector3 &v);
};

class Vector3 : public Unit3
{
public:
	using Unit3::Unit3;
	Vector3 operator + (const Vector3 &v) const;
	Vector3 operator - (const Vector3 &v) const;
	Vector3 operator * (const double c) const;
	Vector3 operator * (const Vector3 &v) const; // 点积
	Vector3 operator ^ (const Vector3 &v) const; // 叉积
	Vector3 operator - () const;
	friend ostream& operator << (ostream& out, const Vector3& s);
	double dot(const Vector3 &v);
	double dot(const Point3 &p);
	double length2();
	double length();
	Vector3& normalize();
};

class Ray3
{
public:
	Point3 start;
	Vector3 dir;
	Ray3() {}
	Ray3(Point3 _start, Vector3 _dir) : start(_start), dir(_dir) {dir.normalize();}
	Ray3& normalize();
	Ray3 fullReflect(Vector3 incomming, const Ray3 &normal); 
	// 全反射方向, 传入两个单位向量
};

class Color
{
public:
	double r, g, b; // 介于0到1之间
	Color() {r = 0; g = 0; b = 0;}
	Color(double _r, double _g, double _b) : r(_r), g(_g), b(_b) {}
	Color operator + (const Color &c) const;
	Color operator * (const double c) const;
	Color operator * (const Color &c) const;
	friend bool operator == (const Color &c1, const Color &c2);
	friend bool operator != (const Color &c1, const Color &c2);
	friend ostream& operator << (ostream& out, const Color& s);
};

const Color background(0, 0, 0.5);
const Color black(0, 0, 0);
const Color white(1.0 ,1.0, 1.0);
const Color red(1.0, 0, 0);
const Color green(0, 1.0, 0);
const Color blue(0, 0, 1.0);

class InsertInfo // 光线与物体相交信息（第一个交点）
{
public:
	bool insert; // 光线是否与物体相交
	bool inside; // 光线是否在物体内部（折射情况）
	Ray3 normal; // 法向量射线
	InsertInfo() {}
	InsertInfo(bool _insert, bool _inside = false, Ray3 _normal = Ray3()) :
	insert(_insert), inside(_inside), normal(_normal){}
	double distance2(const Point3 &p);
	double distance(const Point3 &p);
};

class RandomGen
{
public:
    random_device rd;
    mt19937 gen; 
	uniform_real_distribution<double> dist;
	RandomGen() : gen(rd()), dist(0, 1) {}
	double getRand()
	{
		return dist(gen);
	}
};

class Transform // 对物体变换
{
public:
	double mat[3][3]; // 变换矩阵
	Point3 p0; // 原点
	Transform();
	void move(Vector3 v); // 平移
	void scale(Vector3 v); // 缩放
	void rotateX(double phi); //绕x轴旋转
	void rotateY(double phi); //绕y轴旋转
	void rotateZ(double phi); //绕z轴旋转
	Point3 trans(Point3 p);
};

class Matrix3
{
public: 
    double a[3][3];

    Matrix3() {}
    double algMinor(int x, int y); // 代数余子式
    double det();
    Matrix3 inv(double det);
    Matrix3 operator + (const Matrix3& p);
    Matrix3 operator - (const Matrix3& p);
    Matrix3& operator += (const Matrix3& p);
};

class Array3
{
public:
    double a[3];
    
    Array3() {}
    Array3(double a0, double a1, double a2) {a[0] = a0; a[1] = a1; a[2] = a2;}
    Matrix3 mul(const Array3& v);
    double dot(const Array3& v);
	double length2();
    Array3 operator - (const Array3& v);
    Array3 operator + (const Array3& v);
    Array3 operator * (double c);
    friend Array3 operator * (const Matrix3& p, const Array3& v);
	operator Unit3() {return Unit3(a[0], a[1], a[2]);}
	operator Point3() {return Point3(a[0], a[1], a[2]);}
};

class Point2
{
public:
	double x, y;
	Point2() {}
	Point2(double _x, double _y) : x(_x), y(_y) {}
};

class Image
{
public:
    int h,w;
    Color** pix;
    void readImage(string path);
};

#endif // __BASIC__