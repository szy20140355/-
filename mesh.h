#ifndef __MESH__
#define __MESH__

#include "entity.h"

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
	bool inside(Point3 p);
	bool insertRay(Ray3 ray, InsertInfo &info);
};

class KDNode // kd树节点
{
public:
	Face face; // 当前点的面
	ParalleBox box; // 该节点的包围盒
	KDNode* son[2];

	KDNode(Face _face);
};

class Mesh : public Object3
{
public:
	int num_points, num_faces;
	Point3* points;
	Face* faces;
	KDNode* root;
	Transform trans;
	int insert_count;

	KDNode* build(int l, int r, int dim, int deep);
	Mesh(string s_file, int _num_points, int _num_faces, Vector3 move, Vector3 scale,
	double rotate_x, double rotate_y, double rotate_z, Material _compose, Color _color, Color _emit);
	void updateInsert(KDNode *curr, const Ray3& ray, InsertInfo &info);
	virtual bool inside(const Point3 &p) override;
	virtual InsertInfo insertLight(Ray3 ray, bool pre_inside) override;
};

#endif // __MESH__