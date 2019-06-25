#include "beizer.h"

RawBeizer(int _n, FILE* input) : n(_n) // n:控制点数
{
    p = new Point2[n];
    vx = new double[n];
    vx1 = new double[n];
    bn = new int[n];
    bn1 = new int[n];

    int** C = new int*[n];
    for(int i = 0; i < n; i++) C[i] = new int[n](0);

    for(int i = 0; i < n; i++) C[i][0] = 1;
    for(int i = 1; i < n; i++)
        for(int j = 1; j <= i; j++)
            C[i][j] = C[i - 1][j] + C[i - 1][j - 1];

    for(int i = 0; i < n; i++) bn[i] = C[n - 1][i];
    for(int i = 0; i < n - 1; i++) bn1[i] = C[n - 2][i];

    for(int i = 0; i < n; i++) delete(C[i]);
    delete C;

    for(int i = 0; i < n; i++)
        fscanf(input, "%lf%lf", &p[i].x, &p[i].y); // x坐标与z坐标
}


RotatingBody::RotatingBody(string s_file, int _num_beizer, Vector3 move, Vector3 scale, 
double rotate_x, double rotate_y, double rotate_z, Material _compose, Color _color, Color _emit) :
num_beizer(_num_beizer), Object3(_compose, _color, _emit)
{
    beizer = new RawBeizer[num_beizer];
    has_image = new bool[num_beizer];
    img = new Image[num_beizer];
    img_up = new double[num_beizer];
    img_down = new double[num_beizer];
    trans.move(move);
    trans.scale(scale);
    trans.rotateZ(rotate_z);
    trans.rotateY(rotate_y);
    trans.rotateX(rotate_x);

    FILE* file = fopen(s_file.cstr(), "r");
    for(int i = 0, tn = 0; i < num_beizer; i++)
    {
        fscanf(file, "%d", &tn);
        beizer[i] = RawBeizer(tn, file);
    }
    fclose(file);
}
	
void setImg(Image _img, double _up, double _down, int beizer_id = 0)
{
    has_image[beizer_id] = true;
    img[beizer_id] = _img;
    img_up[beizer_id] = _up;
    img_down[beizer_id] = _down;
}

void 
