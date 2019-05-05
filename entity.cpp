#include "entity.h"

pair<Color, Color> Object3::getColorAndEmit(Point3 p)
{
	return make_pair(color, emit);
}
bool RawPlane3::onPlane(const Point3 &p)
{
	return fabs((p - normal.start).dot(normal.dir)) < eps;
}	
InsertInfo RawPlane3::insertRay(Ray3 ray) 
{
	double dot = ray.dir.dot(normal.dir);
	if(fabs(dot) < eps)
	{
		return InsertInfo(false);
	}
	double t = ( normal.start.dot(normal.dir) - normal.dir.dot(ray.start) ) / dot;
	if(t < 0) 
	{
		return InsertInfo(false);
	}
	
	if(!onPlane(ray.start + ray.dir * t))
	{
		cerr << "###" << "plane insert error" << endl;
	}

	if(dot < 0)
		return InsertInfo(true, false, Ray3(ray.start + ray.dir * t, normal.dir));
	else return InsertInfo(true, false, Ray3(ray.start + ray.dir * t, -normal.dir));
}

bool Plane3::inside(const Point3 &p) 
{
	return false;
}
InsertInfo Plane3::insertLight(Ray3 ray) 
{
	return insertRay(ray);
}

Square3::Square3(Material _compose, Color _color, Color _emit, Ray3 _normal, 
	int _w_pixel, int _h_pixel, double _w_len, double _h_len, Ray3 _x_dir):
Plane3(_compose, _color, _emit, _normal),
w_pixel(_w_pixel), h_pixel(_h_pixel), w_len(_w_len), h_len(_h_len), x_dir(_x_dir)
{
	x_dir.dir.normalize();
	p_color = new Color*[w_pixel];
	p_emit = new Color*[w_pixel];
	for(int i = 0; i < w_pixel; i++)
	{
		p_color[i] = new Color[h_pixel];
		p_emit[i] = new Color[h_pixel];
	}

	cerr << "$$$@" << p_color[0][0] << endl;

	y_dir.start = x_dir.start;
	y_dir.dir = normal.dir ^ x_dir.dir;
	y_dir.dir.normalize();
	dl_corner = x_dir.start;
	ur_corner = dl_corner + x_dir.dir * w_len + y_dir.dir * h_len;
	x_interval = w_len / w_pixel;
	y_interval = h_len / h_pixel;
	
	cerr << "$$$@" << p_color[0][0] << endl;
} 
Square3::~Square3()
{
	for(int i = 0; i < w_pixel; i++)
	{
		delete[] p_color[i];
		delete[] p_emit[i];
	}
	delete[] p_color;
	delete[] p_emit;
}
bool Square3::inSquare(Point3 p)
{
	return (p.x - dl_corner.x) * (p.x - ur_corner.x) <= eps && 
		   (p.y - dl_corner.y) * (p.y - ur_corner.y) <= eps &&
		   (p.z - dl_corner.z) * (p.z - ur_corner.z) <= eps;
}
pair<Color, Color> Square3::getColorAndEmit(Point3 p) 
{
	Color t_color = color;
	Color t_emit = emit;
	if(inSquare(p))
	{
		double lx = x_dir.dir.dot(p - x_dir.start);
		double ly = y_dir.dir.dot(p - y_dir.start);
		int px = (int)(lx / x_interval);
		int py = (int)(ly / y_interval);
		if(px < w_pixel && py < h_pixel)
		{
			t_color = p_color[px][py];
			t_emit = p_emit[px][py];
		}
	}
	return make_pair(t_color, t_emit);
}

Sphere::Sphere(Material _compose, Color _color, Color _emit, Point3 _center, double _radius) :
Object3(_compose, _color, _emit), center(_center), radius(_radius)
{
	radius2 = radius * radius;
}
Vector3 Sphere::normal(const Point3 &p)
{
	return (p - center).normalize();
}
bool Sphere::inside(const Point3 &p) 
{
	return (p - center).length2() < radius2;
}
bool Sphere::onSphere(const Point3 &p)
{
	return fabs((p - center).length2() - radius2) < eps;
}
InsertInfo Sphere::insertLight(Ray3 ray) 
{
	Vector3 l = center - ray.start;
	double tca = l.dot(ray.dir);
	bool ins = inside(ray.start);
	if(tca <= 0 && !ins)
	{
		return InsertInfo(false);
	}
	double d2 = l.dot(l) - tca * tca;
	if(d2 > radius2)
	{
		return InsertInfo(false);
	}
	double thc = sqrt(radius2 - d2);
	if(ins)
	{
		Point3 pt = ray.start + ray.dir * (tca + thc);

		if(!onSphere(pt))
		{
			cerr << "##" << "sphere insert error" << endl;
			cerr << "ray:" << ray.start << ray.dir;
			cerr << pt;
			this -> print();
		}

		return InsertInfo(true, true, Ray3(pt, -normal(pt)));
	}
	Point3 pt = ray.start + ray.dir * (tca - thc) ;

	if(!onSphere(pt))
	{
		cerr << "##" << "sphere insert error" << endl;
		cerr << "ray:" << ray.start << ray.dir;
		cerr << pt;
		this -> print();
	}
	
	return InsertInfo(true, false, Ray3(pt, normal(pt)));
}

localLightProc::localLightProc(Object3* obj, const InsertInfo& info, const Ray3 &ray, int num, bool only_emit):
normal(info.normal), material(&(obj -> compose)), inside(info.inside), raydir(ray), diff_num(num)
{
	static RandomGen random_gen;
	pair<Color, Color> temp = obj -> getColorAndEmit(normal.start);
	color = temp.first; emit = temp.second;

	double cos_t1 = -normal.dir.dot(ray.dir);
	emit_prop = cos_t1;

	if(only_emit) return;

	reflect_dir = ray.dir + normal.dir * 2 * cos_t1;
	reflect_dir.normalize();

	if(material -> ior == 0) // 没有折射
	{
		refract_prop = 0.0;
		reflect_prop = 1.0 - material -> d_reflect;
	}
	else
	{
		if(!inside)
		{
			double sin_t2 = 1.0 / (material -> ior) * sqrt(max(0.0, 1.0 - cos_t1 * cos_t1));
			
			double cos_t2 = sqrt(1.0 - sin_t2 * sin_t2);
			refract_dir = (raydir.dir + normal.dir * cos_t1) * (1.0/(material -> ior)) - normal.dir * cos_t2 ;

			double fr0 = pow((material -> ior * cos_t1 - cos_t2) / (material -> ior * cos_t1 + cos_t2), 2.0);
			double fr1 = pow((material -> ior * cos_t2 - cos_t1) / (material -> ior * cos_t2 + cos_t1), 2.0);
			double fr = (fr0 + fr1) * 0.5;
			reflect_prop = fr * (1.0 - material -> d_reflect);
			refract_prop = (1.0 - fr) * (1.0 - material -> d_reflect);
		}
		else
		{
			double sin_t2 = (material -> ior) * sqrt(max(0.0, 1.0 - cos_t1 * cos_t1));
			
			if(sin_t2 > 1.0) // 全反射
			{
				refract_prop = 0.0;
				reflect_prop = 1.0 - material -> d_reflect;			
			}
			else
			{
				double cos_t2 = sqrt(1.0 - sin_t2 * sin_t2);
				refract_dir = (raydir.dir + normal.dir * cos_t1) * (material -> ior) - normal.dir * cos_t2 ;

				double fr0 = pow((cos_t1 - material -> ior * cos_t2) / (cos_t1 + material -> ior * cos_t2), 2.0);
				double fr1 = pow((cos_t2 - material -> ior * cos_t1) / (cos_t2 + material -> ior * cos_t1), 2.0);
				double fr = (fr0 + fr1) * 0.5;
				reflect_prop = fr * (1.0 - material -> d_reflect);
				refract_prop = (1.0 - fr) * (1.0 - material -> d_reflect);			
			}
		}
	}

	p0 = normal.start;
	z_dir = normal.dir;
	if(fabs(z_dir.x) > fabs(z_dir.y) && fabs(z_dir.x) > fabs(z_dir.z))
		x_dir = Vector3(-(z_dir.y + z_dir.z) / z_dir.x, 1.0, 1.0);
	else if(fabs(z_dir.y) > fabs(z_dir.x) && fabs(z_dir.y) > fabs(z_dir.z))
		x_dir = Vector3(1.0, -(z_dir.x + z_dir.z) / z_dir.y, 1.0);
	else 
		x_dir = Vector3(1.0, 1.0, -(z_dir.x + z_dir.y) / z_dir.z);

	x_dir.normalize();
	y_dir = (z_dir ^ x_dir).normalize();

	if(fabs(x_dir.dot(normal.dir)) > eps)
	{
		cerr << "###" << "local x_dir error" << endl;
	}

	for(int i = 0; i < diff_num; i++)
	{
		double phi = random_gen.getRand() * 2.0 * PI;
		double costhe = random_gen.getRand();
		double sinthe = sqrt(max(0.0, 1.0 - costhe * costhe));
		Vector3 p(sinthe * cos(phi), sinthe * sin(phi), costhe);
	
		Vector3 dir_world(
			p.x * x_dir.x + p.y * y_dir.x + p.z * z_dir.x,
			p.x * x_dir.y + p.y * y_dir.y + p.z * z_dir.y,
			p.x * x_dir.z + p.y * y_dir.z + p.z * z_dir.z
		);

		diff_dir.push_back(dir_world);
		frac.push_back(2.0 * (material -> rhod) * (material -> d_reflect) * costhe / diff_num);
	}
}
