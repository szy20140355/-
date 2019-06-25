#ifndef __SCENE__
#define __SCENE__

#include "entity.h"

class Camera
{
public:
    Point3 position;
    Camera() {}
    Camera(Point3 _position) : position(_position) {}
};

class CanvasPlane3 : public Square3
{
public:
    using Square3::Square3; // normal 指向相机
    Point3 getPos(int x, int y); // 获取坐标 x,y 像素的中心位置
    void setColor(int x, int y, Color c);
    void print();
};

class SceneManager
{
public:
    vector<Object3*> objects;
    Camera camera;
    CanvasPlane3* canvas;
    
    #ifdef DEPTH_OF_FIELD
    RawPlane3 focal_plane; 
    #endif // DEPTH_OF_FIELD

    SceneManager(Camera _camera, CanvasPlane3* _canvas);
    void addObject(Object3* object);
    int getRayNum(int depth, double prop);
    Color trace(Ray3 ray, int depth, double prop, bool pre_inside); // 光线追踪部分，prop为当前光线占总光线中比重
    void calcRayCasting();
};

#endif // __SCENE__