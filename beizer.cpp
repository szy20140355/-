#include "beizer.h"

RawBeizer::RawBeizer(int _n, FILE* input) : n(_n) // n:控制点数
{
	p = new Point2[n];
	bn = new double[n];
	bn1 = new double[n];

	double** C = new double*[n];
	for(int i = 0; i < n; i++) C[i] = new double[n];

	for(int i = 0; i < n; i++) C[i][0] = 1;
	for(int i = 1; i < n; i++)
		for(int j = 1; j <= i; j++)
			C[i][j] = C[i - 1][j] + C[i - 1][j - 1];

	for(int i = 0; i < n; i++) bn[i] = C[n - 1][i];
	for(int i = 0; i < n - 1; i++) bn1[i] = C[n - 2][i];

	for(int i = 0; i < n; i++) delete[] C[i];
	delete[] C;

	for(int i = 0; i < n; i++)
		fscanf(input, "%lf%lf", &p[i].x, &p[i].y); // x坐标与z坐标
}

Point2 RawBeizer::getVal(double x)
{
	double *vx, *vx1;
	vx = new double[n];
	vx1 = new double[n];
	vx[0] = vx1[0] = 1;

	for(int i = 1; i < n; i++)
	{
		vx[i] = vx[i - 1] * x;
		vx1[i] = vx1[i - 1] * (1.0 - x);
	}
	Point2 ret(0, 0);
	for(int i = 0; i < n; i++)
	{
		double t = bn[i] * vx[i] * vx1[n - i - 1];
		ret.x += p[i].x * t;
		ret.y += p[i].y * t;
	}
	delete[] vx;
	delete[] vx1;
	return ret;
}

Point2 RawBeizer::getDeriVal(double x)
{
	double *vx, *vx1;
	vx = new double[n];
	vx1 = new double[n];
	vx[0] = vx1[0] = 1;

	for(int i = 1; i < n - 1; i++)
	{
		vx[i] = vx[i - 1] * x;
		vx1[i] = vx1[i - 1] * (1.0 - x);
	}
	Point2 ret(0, 0);
	for(int i = 0; i < n - 1; i++)
	{
		double t = bn1[i] * vx[i] * vx1[n - i - 2];
		ret.x += (p[i + 1].x - p[i].x) * t;
		ret.y += (p[i + 1].y - p[i].y) * t;
	}
	ret.x *= n - 1; ret.y *= n - 1;

	delete[] vx;
	delete[] vx1;
	return ret;
}

const double RotatingBody::iter_eps = 1e-7;
const double RotatingBody::out_eps = 1e-4;

RotatingBody::RotatingBody(string s_file, int _num_beizer, Vector3 _move, double _rot_ang, 
Material _compose, Color _color, Color _emit) :
num_beizer(_num_beizer), move(_move), rot_ang(_rot_ang), Object3(_compose, _color, _emit)
{
	beizer = new RawBeizer[num_beizer];
	has_img = false;
	border_up = -inf;
	border_down = inf;
	radius = 0;

	FILE* file = fopen(s_file.c_str(), "r");
	for(int i = 0, tn = 0; i < num_beizer; i++)
	{
		fscanf(file, "%d", &tn);
		beizer[i] = RawBeizer(tn, file);
		for(int j = 0; j < tn; j++)
		{
			Point2 tv = beizer[i].getVal((double)j / (tn - 1));
			radius = max(radius, tv.x);
			radius = max(radius, beizer[i].p[j].x);
			border_up = max(border_up, tv.y);
			border_down = min(border_down, tv.y);
		}
	}
	radius2 = radius * radius;
	fclose(file);
}
	
void RotatingBody::setImg(Image _img, double _up, double _down)
{
	has_img = true;
	img = _img;
	img_up = _up;
	img_down = _down;
}

bool RotatingBody::inside(const Point3 &p) 
{
	cerr << "no inside method in RotatingBody" << endl;
	assert(0);
	return false;
}

bool RotatingBody::insertBox(Ray3 ray, double &ins0, double &ins1)
{
	Ray3 t_ray = ray;
	t_ray.start.z = t_ray.dir.z = 0;
	double t_l1 = t_ray.dir.length();
	t_ray.dir.normalize();
	Point3 tp = Point3(move.x, move.y, 0);
	double tlen = (t_ray.dir).dot(tp - t_ray.start);
	double tdis2 = (t_ray.start + t_ray.dir * tlen - tp).length2();
	if(tdis2 > radius2) return false;
	double tl = sqrt(radius2 - tdis2);
	ins0 = (tlen - tl) / t_l1;
	ins1 = (tlen + tl) / t_l1;
	if(ins1 < 0) return false;
	ins0 = max(ins0, 0.0);
	
	double tz = move.z + border_up;
	tl = (tz - ray.start.z) / ray.dir.z;
	if(tl > 0)
	{
		tp = ray.start + ray.dir * tl;
		if((tp.x - move.x) * (tp.x - move.x) + (tp.y - move.y) * (tp.y - move.y) < radius2)
		{
			if(ray.start.z > tz) ins0 = tl;
			else ins1 = tl;
		}
	}
	tz = move.z + border_down;
	tl = (tz - ray.start.z) / ray.dir.z;
	if(tl > 0)
	{
		tp = ray.start + ray.dir * tl;
		if((tp.x - move.x) * (tp.x - move.x) + (tp.y - move.y) * (tp.y - move.y) < radius2)
		{
			if(ray.start.z < tz) ins0 = tl;
			else ins1 = tl;
		}
	}

	tz = ray.start.z + ins0 * ray.dir.z;
	if(tz > move.z + border_up + eps || tz + eps < move.z + border_down) return false;
	tz = ray.start.z + ins1 * ray.dir.z;
	if(tz > move.z + border_up + eps || tz + eps < move.z + border_down) return false;
	return true;
}

Array3 RotatingBody::envalue(Point2 val, double sin_the, double cos_the)
{
	Array3 ret = (Array3)move;
	ret.a[0] += sin_the * val.x;
	ret.a[1] += cos_the * val.x;
	ret.a[2] += val.y;
	return ret;
}

InsertInfo RotatingBody::insertLight(Ray3 ray, bool pre_inside)
{
	double ins0, ins1; // 射线与包围盒相交的参数范围
	InsertInfo info = false; 
	if(!insertBox(ray, ins0, ins1)) return info;

	bool insert = false;
	int id; // 曲线编号
	Array3 fx; // 最终交点参数

	for(int i = 0; i < num_beizer; i++)
	{
		for(int step = 0; step < rand_times; step++)
		{
			double t = ins0 + random_gen.getRand() * (ins1 - ins0);
			double u = random_gen.getRand();
			double the = random_gen.getRand() * 2 * PI;

			Array3 x(t, u, the);
			bool find_ins = false;
			for(int j = 0; j < iter_times; j++)
			{
				Point2 p_val = beizer[i].getVal(x.a[1]);
				double sin_the = sin(x.a[2]), cos_the = cos(x.a[2]);

				Array3 tv = envalue(p_val, sin_the, cos_the) - (Array3)(ray.start + ray.dir * x.a[0]);
				if(x.a[0] > ins0 && x.a[0] < ins1 && x.a[1] > 0 && x.a[1] < 1)
				{
					if(tv.length2() < iter_eps) {find_ins = true; break;}
				}
				Matrix3 mat;
				Point2 p_deri_val = beizer[i].getDeriVal(x.a[1]);
				mat.a[0][0] = -ray.dir.x; 
				mat.a[1][0] = -ray.dir.y; 
				mat.a[2][0] = -ray.dir.z;
				mat.a[0][1] = sin_the * p_deri_val.x;
				mat.a[1][1] = cos_the * p_deri_val.x;
				mat.a[2][1] = p_deri_val.y;
				mat.a[0][2] = cos_the * p_val.x;
				mat.a[1][2] = -sin_the * p_val.x;
				mat.a[2][2] = 0;
				double det = mat.det();
				if(fabs(det) < eps_det) break;
				mat = mat.inv(det);
				x = x - mat * tv;
			}
			if(find_ins)
			{
				if(!insert || fx.a[0] > x.a[0])
				{
					insert = true;
					id = i;
					fx = x;
				}
			}
		}
	}
	if(!insert) return info;
	info.insert = true;
	info.inside = pre_inside;
	Point2 deri_val = beizer[id].getDeriVal(fx.a[1]);
	Point2 t_val = beizer[id].getVal(fx.a[1]);
	double sin_the = sin(fx.a[2]), cos_the = cos(fx.a[2]);
	info.normal.start = (Point3)envalue(t_val, sin_the, cos_the);

	Vector3 deri_the(cos_the * t_val.x, -sin_the * t_val.x, 0);
	Vector3 deri_u(sin_the * deri_val.x, cos_the * deri_val.x, deri_val.y);
	info.normal.dir = deri_the ^ deri_u;
	info.normal.dir.normalize();
	if(info.normal.dir.dot(info.normal.start - ray.start) > 0)
		info.normal.dir = -info.normal.dir;
	info.normal.start = info.normal.start + info.normal.dir * out_eps;

	return info;
}

pair<Color, Color> RotatingBody::getColorAndEmit(Point3 p)
{
	if(!has_img) 
		return make_pair(color, emit);
	p = p - move;
	if(p.z < img_down || p.z > img_up) 
		return make_pair(color, emit);
	double the = atan2(p.y, p.x);
	the += rot_ang;
	while(the < 0) the += 2 * PI;
	while(the > 2 * PI) the -= 2 * PI;
	int pw = (int)(the / (2 * PI) * img.w);
	int ph = (int)((1.0 - (p.z - img_down) / (img_up - img_down)) * img.h);
	return make_pair(img.pix[ph][pw], emit);
}
