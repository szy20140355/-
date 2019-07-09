#ifndef __BEIZER__
#define __BEIZER__

#include "basic.h"
#include "entity.h"

class RawBeizer
{  
public:
	int n; // 控制点数
	Point2* p; // 控制点坐标
	double *bn, *bn1; // C_ni, C_ni-1
	RawBeizer() {}
	RawBeizer(int _n, FILE* input); 
	Point2 getVal(double x); // 曲线在u处的值 
	Point2 getDeriVal(double x); // 曲线的导数在u处的值 
};

class RotatingBody : public Object3
{
	static const int rand_times = 10; // 随机起点次数
	static const int iter_times = 40; // 最大迭代次数
	static const double iter_eps; // 小于此值时视为收敛
	static const double out_eps; // 反射点向外平移距离

public:

	RandomGen random_gen;
	int num_beizer;
	RawBeizer* beizer;
	Vector3 move; // 中心轴：(move.x, move.y) 
	double rot_ang; // 旋转的角度（与贴图有关）
	bool has_img;
	Image img;
	double img_up, img_down;
	double border_up, border_down, radius, radius2; // up:包围盒顶面 down:包围盒底面

	RotatingBody(string s_file, int _num_beizer, Vector3 _move, double _rot_ang, 
	Material _compose, Color _color, Color _emit);
	void setImg(Image _img, double _up, double _down);
	virtual bool inside(const Point3 &p) override;
	bool insertBox(Ray3 ray, double &ins0, double &ins1); // 求与圆柱包围盒交点
	Array3 envalue(Point2 val, double sin_the, double cos_the); // 求曲线在一点处的值
	virtual InsertInfo insertLight(Ray3 ray, bool pre_inside) override;
	virtual pair<Color, Color> getColorAndEmit(Point3 p) override;
};

#endif // __BEIZER__