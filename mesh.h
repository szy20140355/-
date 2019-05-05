#ifndef __MESH__
#define __MESH__

#include "entity.h"

class Transform // 对物体变换
{
public:
	double mat[3][3]; // 变换矩阵
	Point3 p0; // 原点
	Transform();
	void move(Vector3 v); // 平移
	void scale(Vector3 v); // 缩放
	Point3 trans(Point3 p);
};

class Face // Mesh中的一个面
{
public:
	RawPlane3 plane;
	Point3 p[3], center;
	Face() {}
	Face(Point3 *_p);
   	InsertInfo insertRay(Ray3 ray);
};

class ParalleBox // 与坐标轴平行的盒子
{
public:
	Point3 min_coord, max_coord;
	
	ParalleBox();
	ParalleBox(Point3 _min_coord, Point3 _max_coord) : min_coord(_min_coord), max_coord(_max_coord) {}
	void update(const ParalleBox &box);
	void update(Point3 p);
	bool insertRay(Ray3 ray);
};

class KDNode // kd树节点
{
public:
	Face face; // 当前点的面
	ParalleBox box; // 该节点的包围盒
	KDNode* son[2];

	KDNode(Face _face);
};

class Mesh
{
public:
	int num_points, num_faces;
	Point3* points;
	Face* faces;
	KDNode* root;
	Transform trans;
	int insert_count;

	KDNode* build(int l, int r, int dim);
	Mesh(string file, int _num_points, int _num_faces, Vector3 move, Vector3 scale);
	void updateInsert(KDNode *curr, const Ray3& ray, InsertInfo &info);
	virtual bool inside(const Point3 &p) override;
	virtual InsertInfo insertLight(Ray3 ray) override;
};

#endif // __MESH__