#include "basic.h"

double& Unit3::getDim(int c)
{
    if(c == 0) return x;
    if(c == 1) return y;
    return z;
}

Point3 Point3::operator + (const Vector3 &v) const
{
    return Point3(x + v.x, y + v.y, z + v.z);
}
Point3 Point3::operator + (const Point3 &p) const
{
    return Point3(x + p.x, y + p.y, z + p.z);
}
Point3 Point3::operator - (const Vector3 &v) const 
{
    return Point3(x - v.x, y - v.y, z - v.z);
}
Vector3 Point3::operator - (const Point3 &p) const
{
    return Vector3(x - p.x, y - p.y, z - p.z);
}
Point3 Point3::operator * (const double c) const
{
    return Point3(x * c, y * c, z * c);
}
Point3 Point3::operator / (const double c) const
{
    return Point3(x / c, y / c, z / c);
}
double Point3::dot(const Vector3 &v)
{
    return x * v.x + y * v.y + z * v.z;
}
ostream& operator << (ostream& out, const Point3& p)
{
    out << "x:"<< p.x << " y:" << p.y << " z:" << p.z << endl;
    return out;
}

Vector3 Vector3::operator + (const Vector3 &v) const
{
    return Vector3(x + v.x, y + v.y, z + v.z);
}
Vector3 Vector3::operator - (const Vector3 &v) const
{
    return Vector3(x - v.x, y - v.y, z - v.z);
}
Vector3 Vector3::operator * (const double c) const
{
    return Vector3(x * c, y * c, z * c);    
}
Vector3 Vector3::operator * (const Vector3 &v) const // 点积
{
    return Vector3(x * v.x, y * v.y, z * v.z);
}
Vector3 Vector3::operator ^ (const Vector3 &v) const // 叉积
{
    return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}
Vector3 Vector3::operator - () const
{
    return Vector3(-x, -y, -z);
}
double Vector3::dot(const Vector3& v)
{
    return x * v.x + y * v.y + z * v.z;
}
double Vector3::dot(const Point3 &p)
{
    return x * p.x + y * p.y + z * p.z;
}
double Vector3::length2()
{
    return x * x + y * y + z * z;
}
double Vector3::length()
{
    return sqrt(length2());    
}
Vector3& Vector3::normalize()
{
    double nor2 = length2();
    if(nor2 > 0)
    {
        double invNor = 1.0 / sqrt(nor2);
        x *= invNor, y *= invNor, z *= invNor;
    }
    return *this;
}
ostream& operator << (ostream& out, const Vector3& v)
{
    out << "x:"<< v.x << " y:" << v.y << " z:" << v.z << endl;
    return out;
}

Ray3& Ray3::normalize()
{
    dir.normalize();
    return *this;
}
Ray3 Ray3::fullReflect(Vector3 incomming, const Ray3 &normal)
{
    Ray3 ret;
    ret.start = normal.start;
    ret.dir = incomming - normal.dir * 2 * incomming.dot(normal.dir);
    return ret;
}

Color Color::operator + (const Color &c) const
{
    return Color(r + c.r, g + c.g, b + c.b);
}
Color Color::operator * (const double c) const
{
    return Color(c * r, c * g, c * b);
}
Color Color::operator * (const Color &c) const
{
    return Color(r * c.r, g * c.g, b * c.b);
}
bool operator == (const Color &c1, const Color &c2)
{
    return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
}
bool operator != (const Color &c1, const Color &c2)
{
    return !(c1 == c2);
}
ostream& operator << (ostream& out, const Color& c)
{
    out << "r:"<< c.r << " g:" << c.g << " b:" << c.b << endl;
    return out;
}

double InsertInfo::distance2(const Point3 &p)
{
    return (p - normal.start).length2();
}
double InsertInfo::distance(const Point3 &p)
{
    return sqrt(distance2(p));
}

Transform::Transform()
{
	memset(mat, 0, sizeof(mat));
	mat[0][0] = mat[1][1] = mat[2][2] = 1.0;
}
void Transform::move(Vector3 v)
{
    p0 = p0 + v;
}
void Transform::scale(Vector3 v)
{
	mat[0][0] *= v.x; mat[0][1] *= v.x; mat[0][2] *= v.x;
	mat[1][0] *= v.y; mat[1][1] *= v.y; mat[1][2] *= v.y;
	mat[2][0] *= v.z; mat[2][1] *= v.z; mat[2][2] *= v.z;
}
void Transform::rotateZ(double phi)
{
	double temp[3][3], cp = cos(phi), sp = sin(phi);
	
	temp[0][0] = mat[0][0] * cp + mat[0][1] * sp;
	temp[1][0] = mat[1][0] * cp + mat[1][1] * sp;
	temp[2][0] = mat[2][0] * cp + mat[2][1] * sp;

	temp[0][1] = mat[0][0] * (-sp) + mat[0][1] * cp;
	temp[1][1] = mat[1][0] * (-sp) + mat[1][1] * cp;
	temp[2][1] = mat[2][0] * (-sp) + mat[2][1] * cp;

	temp[0][2] = mat[0][2];
	temp[1][2] = mat[1][2];
	temp[2][2] = mat[2][2];
	
	memcpy(mat, temp, sizeof(mat));
}
void Transform::rotateY(double phi)
{
	double temp[3][3], cp = cos(phi), sp = sin(phi);
	
	temp[0][0] = mat[0][0] * cp + mat[0][2] * sp;
	temp[1][0] = mat[1][0] * cp + mat[1][2] * sp;
	temp[2][0] = mat[2][0] * cp + mat[2][2] * sp;

	temp[0][1] = mat[0][1];
	temp[1][1] = mat[1][1];
	temp[2][1] = mat[2][1];

	temp[0][2] = mat[0][0] * (-sp) + mat[0][2] * cp;
	temp[1][2] = mat[1][0] * (-sp) + mat[1][2] * cp;
	temp[2][2] = mat[2][0] * (-sp) + mat[2][2] * cp;

	memcpy(mat, temp, sizeof(mat));
}
void Transform::rotateX(double phi)
{
	double temp[3][3], cp = cos(phi), sp = sin(phi);

	temp[0][0] = mat[0][0];
	temp[1][0] = mat[1][0];
	temp[2][0] = mat[2][0];
	
	temp[0][1] = mat[0][1] * cp + mat[0][2] * sp;
	temp[1][1] = mat[1][1] * cp + mat[1][2] * sp;
	temp[2][1] = mat[2][1] * cp + mat[2][2] * sp;

	temp[0][2] = mat[0][1] * (-sp) + mat[0][2] * cp;
	temp[1][2] = mat[1][1] * (-sp) + mat[1][2] * cp;
	temp[2][2] = mat[2][1] * (-sp) + mat[2][2] * cp;

	memcpy(mat, temp, sizeof(mat));	
}
Point3 Transform::trans(Point3 p)
{
	Point3 ret;
	ret.x = mat[0][0] * p.x + mat[0][1] * p.y + mat[0][2] * p.z;
	ret.y = mat[1][0] * p.x + mat[1][1] * p.y + mat[1][2] * p.z;
	ret.z = mat[2][0] * p.x + mat[2][1] * p.y + mat[2][2] * p.z;
	ret = ret + p0;
	return ret;
}

double Matrix3::algMinor(int x, int y)
{
    int u[2], w[2];
    for(int i = 0, t = 0; i < 3; i++) if(i != x) u[t++] = i;
    for(int i = 0, t = 0; i < 3; i++) if(i != y) w[t++] = i;
    double ans = a[u[0]][w[0]] * a[u[1]][w[1]] - a[u[1]][w[0]] * a[u[0]][w[1]];
    if((x + y) & 1) return -ans;
    return ans;
}
double Matrix3::det()
{
    return a[0][0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1]) 
         - a[0][1] * (a[1][0] * a[2][2] - a[1][2] * a[2][0])
         + a[0][2] * (a[1][0] * a[2][1] - a[1][1] * a[2][0]);
}
Matrix3 Matrix3::inv(double det)
{
    Matrix3 ret;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            ret.a[i][j] = algMinor(j, i) / det;
    return ret;
}
Matrix3 Matrix3::operator + (const Matrix3& p)
{
    Matrix3 ret;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            ret.a[i][j] = a[i][j] + p.a[i][j];
    return ret;
}
Matrix3 Matrix3::operator - (const Matrix3& p)
{
    Matrix3 ret;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            ret.a[i][j] = a[i][j] - p.a[i][j];
    return ret;
}
Matrix3& Matrix3::operator += (const Matrix3& p)
{
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            a[i][j] += p.a[i][j];
    return *this;
}

Matrix3 Array3::mul(const Array3& v)
{
    Matrix3 ret;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
        ret.a[i][j] = a[i] * v.a[j];
    return ret;
}
double Array3::dot(const Array3& v)
{
    double ret;
    for(int i = 0; i < 3; i++)
        ret += a[i] * v.a[i];
    return ret;
}
Array3 Array3::operator - (const Array3& v)
{
    Array3 ret;
    for(int i = 0; i < 3; i++)
        ret.a[i] = a[i] - v.a[i];
    return ret;
}
Array3 Array3::operator + (const Array3& v)
{
    Array3 ret;
    for(int i = 0; i < 3; i++)
        ret.a[i] = a[i] + v.a[i];
    return ret;
}
Array3 Array3::operator * (double c)
{
    Array3 ret;
    for(int i = 0; i < 3; i++)
        ret.a[i] = a[i] * c;
    return ret;
}
Array3 operator * (const Matrix3& p, const Array3& v)
{
    Array3 ret;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            ret.a[i] += p.a[i][j] * v.a[j];
    return ret;
}

void Image::readImage(string path)
{
	FILE* file = fopen("image.out", "r");
	fscanf(file, "%d%d", &h, &w);
	
    pix = new Color*[h];
    for(int i = 0; i < h; i++)
        pix[i] = new Color[w];

	cerr << "## h : " << h << "  w : " << w << endl;
	fflush(stderr);

	for(int i = 0; i < h; i++)
		for(int j = 0; j < w; j++)
		{
			fscanf(file, "%lf%lf%lf", &pix[i][j].r, &pix[i][j].g, &pix[i][j].b);
			//cerr << pix[i][j] << endl;
			pix[i][j].r /= color_range;
			pix[i][j].g /= color_range;
			pix[i][j].b /= color_range;
		}
	fclose(file);
}
