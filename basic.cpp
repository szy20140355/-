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
Point3 Point3::operator - (const Vector3 &v) const 
{
    return Point3(x - v.x, y - v.y, z - v.z);
}
Vector3 Point3::operator - (const Point3 &p) const
{
    return Vector3(x - p.x, y - p.y, z - p.z);
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