#ifndef __BEIZER__
#define __BEIZER__

#include "basic.h"
#include "entit.h"

class RawBeizer
{  
public:
	int n; // 控制点数
	Point2* p; // 控制点坐标
	double* vx, vx1;
	double* bn, bn1; // C_ni, C_ni-1
	RawBeizer() {}
	RawBeizer(int _n, FILE* input); 
};

class RotatingBody : public Object3
{
public:
	int num_beizer;
	RawBeizer* beizer;
	Transform trans;
	bool* has_img;
	Image* img;
	double* img_up, img_down;
	
	RotatingBody(string s_file, int num_beizer, Vector3 move, Vector3 scale, 
	double rotate_x, double rotate_y, double rotate_z, Material _compose, Color _color, Color _emit);
	void setImg(Image _img, double _up, double _down, int beizer_id = 0);
	void 
};

#endif // __BEIZER__