#include "mesh.h"

Face::Face(Point3 *_p)
{
	for(int i = 0; i < 3; i++)
		p[i] = _p[i];
	center = (p[0] + p[1] + p[2]) / 3.0;
	plane.normal.start = center;
	plane.normal.dir = ((p[1] - p[0]) ^ (p[2] - p[0])).normalize();
}
InsertInfo Face::insertRay(Ray3 ray)
{
	InsertInfo ret = plane.insertRay(ray);
	if(!ret.insert)
	{
		return ret;
	}
	int cnt = 0; // 投影到x,y平面，向y轴正方向发出的射线与边界相交次数
	double tx = ret.normal.start.x, ty = ret.normal.start.y;
	for(int i = 0; i < 3; i++)
	{
		double x_1 = p[i].x, y_1 = p[i].y, x_2 = p[(i + 1) % 3].x, y_2 = p[(i + 1) % 3].y;
		if(x_1 == tx && y_1 > ty) cnt ++;
		if((tx - x_1) * (tx - x_2) < 0
		 && y_1 + (y_2 - y_1) / (x_2 - x_1) * (tx - x_1) >= ty ) cnt ++;
	}
	if(cnt & 1) 
	{
		if(ret.normal.dir.dot(plane.normal.dir) < 0) 
			ret.inside = true;
		return ret;
	}
	return InsertInfo(false);
}

ParalleBox::ParalleBox()
{
	min_coord.x = inf; max_coord.x = -inf;
	min_coord.y = inf; max_coord.y = -inf;
	min_coord.z = inf; max_coord.z = -inf;
}
void ParalleBox::update(Point3 p)
{
	min_coord.x = min(min_coord.x, p.x);
	min_coord.y = min(min_coord.y, p.y);
	min_coord.z = min(min_coord.z, p.z);

	max_coord.x = max(max_coord.x, p.x);
	max_coord.y = max(max_coord.y, p.y);
	max_coord.z = max(max_coord.z, p.z);
}
void ParalleBox::update(const ParalleBox &box)
{
	min_coord.x = min(min_coord.x, box.min_coord.x);
	min_coord.y = min(min_coord.y, box.min_coord.y);
	min_coord.z = min(min_coord.z, box.min_coord.z);

	max_coord.x = max(max_coord.x, box.max_coord.x);
	max_coord.y = max(max_coord.y, box.max_coord.y);
	max_coord.z = max(max_coord.z, box.max_coord.z);
}
bool ParalleBox::inside(Point3 p)
{
	if(p.x < min_coord.x || p.x > max_coord.x) return false;
	if(p.y < min_coord.y || p.y > max_coord.y) return false;
	if(p.z < min_coord.z || p.z > max_coord.z) return false;
	return true;
}
bool ParalleBox::insertRay(Ray3 ray, InsertInfo &info)
{
	double t_min = -inf, t_max = inf;
	if(inside(ray.start)) return true;
	for(int i = 0; i < 3; i++)
	{
		double t = ray.dir.getDim(i);
		if(fabs(t) < eps) continue;
		double v1 = (min_coord.getDim(i) - ray.start.getDim(i)) / (ray.dir.getDim(i));
		double v2 = (max_coord.getDim(i) - ray.start.getDim(i)) / (ray.dir.getDim(i));
		if(v1 > v2) swap(v1, v2);
		t_min = max(t_min, v1);
		t_max = min(t_max, v2);
	}
	if(t_min >= t_max || t_max < 0) return false;
	if(info.insert && info.distance(ray.start) < t_min) return false;
	return true;
}

KDNode::KDNode(Face _face) : face(_face) 
{
	son[0] = son[1] = nullptr;
}

KDNode* Mesh::build(int l, int r, int dim, int deep)
{
	Point3 mean_coord(0, 0, 0);
	for(int i = l; i < r; i++)
		mean_coord = mean_coord + faces[i].center;
	mean_coord = mean_coord / (double)(r - l);
	
	////////////////////////
	//if(l == 12313)
	/*{
		cerr << "$$$ l = " << l << "  r = " << r << endl;
		cerr << "mean_coord : " <<mean_coord << endl; 
		fflush(stderr);
	}*/
	///////////////////////

	int lp = l, rp = r - 1;
	while(lp < rp)
	{
		while(faces[lp].center.getDim(dim) <= mean_coord.getDim(dim) && lp < rp) lp++;
		while(faces[rp].center.getDim(dim) > mean_coord.getDim(dim) && lp < rp) rp--;
		if(lp < rp) swap(faces[lp], faces[rp]);
	}

	KDNode *node = new KDNode(faces[lp]);
	if(lp > l) node -> son[0] = build(l, lp, (dim + 1) % 3, deep + 1);
	if(r > rp + 1) node -> son[1] = build(rp + 1, r, (dim + 1) % 3, deep + 1);

	for(int i = 0; i < 3; i++)
		(node -> box).update(faces[lp].p[i]);
	for(int i = 0; i < 2; i++)
		if(node -> son[i])
			(node -> box).update(node -> son[i] -> box);
	
	return node;
}
Mesh::Mesh(string s_file, int _num_points, int _num_faces, Vector3 move, Vector3 scale, 
double rotate_x, double rotate_y, double rotate_z, Material _compose, Color _color, Color _emit) : 
num_points(_num_points), num_faces(_num_faces), Object3(_compose, _color, _emit)
{
	points = new Point3[num_points];
	faces = new Face[num_faces];
	trans.move(move);
	trans.scale(scale);
	trans.rotateZ(rotate_z);
	trans.rotateY(rotate_y);
	trans.rotateX(rotate_x);

	FILE* file = fopen(s_file.c_str(), "r");
	char c;
	Point3 tp[3];
	for(int t_faces = 0, t_points = 0; (c = getc(file)) != EOF;)
	{
		if(c == 'v')
		{
			fscanf(file, "%lf%lf%lf", &points[t_points].x, &points[t_points].y, &points[t_points].z);
			points[t_points] = trans.trans(points[t_points]);
			++t_points;
		}
		else if(c == 'f')
		{
			int p1, p2 ,p3;
			fscanf(file, "%d%d%d", &p1, &p2, &p3); // 1 base
			tp[0] = points[p1 - 1]; tp[1] = points[p2 - 1]; tp[2] = points[p3 - 1];
			faces[t_faces] = Face(tp);
			++t_faces;
		}
	}
	fclose(file);
	root = build(0, num_faces, 0, 0);
	
	for(int i = 0; i < num_faces; i++)
	{
		Ray3 &normal = faces[i].plane.normal;
		insertLight(Ray3(normal.start + normal.dir * bias, normal.dir));
		if(insert_count & 1) normal.dir = - normal.dir;
	}
}
bool Mesh::inside(const Point3 &p) 
{
	cerr << "no inside method in mesh" << endl;
	assert(0);
	return false;
}
void Mesh::updateInsert(KDNode *curr, const Ray3& ray, InsertInfo &info)
{
	if(!curr || !(curr -> box).insertRay(ray, info))
		return;
	InsertInfo t_info = (curr -> face).insertRay(ray);
	if(t_info.insert)
	{
		insert_count ++;
		if(!info.insert || info.distance2(ray.start) > t_info.distance2(ray.start))
			info = t_info;
	}
	updateInsert(curr -> son[0], ray, info);
	updateInsert(curr -> son[1], ray, info);
}
InsertInfo Mesh::insertLight(Ray3 ray)
{
	insert_count = 0;
	InsertInfo ret = false;
	updateInsert(root, ray, ret);
	return ret;
}
