#include "const.h"
#include "basic.h"
#include "entity.h"
#include "scene.h"

SceneManager* manager;

int main()
{
	freopen("error.out", "w", stderr);

	Camera camera(Point3(0, 0, 0));
	CanvasPlane3 canvas(Material(), Color(), Color(), Ray3(Point3(0,5,0), Vector3(0,-1,0)),
	 					800, 800, 20.0, 20.0, Ray3(Point3(-10, 5, -10), Vector3(1,0,0)));
		
	manager = new SceneManager(camera, &canvas);
	Material wall(0.7, 0.0, 0.9);
	Material ball(0.0, 1.3, 0.9);
	Plane3 wall_l(wall, red, black, Ray3(Point3(-15,0,0), Vector3(1,0,0)));
	Plane3 wall_r(wall, blue, black, Ray3(Point3(15,0,0), Vector3(-1,0,0)));
	Plane3 wall_front(wall, white, white, Ray3(Point3(0,40,0), Vector3(0,-1	,0)));
	Plane3 wall_down(wall, white, black, Ray3(Point3(0,0,-15), Vector3(0,0,1)));
	Square3 wall_up(wall, (red+green)*0.5, black, Ray3(Point3(0,0,15),Vector3(0,0,1)),1,1,10.0,10.0,
					Ray3(Point3(-5,10,15), Vector3(1,0,0)));
	wall_up.setEmit(0,0,white * 5.0);
	wall_up.setColor(0,0,black);
	manager -> addObject(&wall_l);
	manager -> addObject(&wall_r);
	manager -> addObject(&wall_front);
	manager -> addObject(&wall_down);
	manager -> addObject(&wall_up);

	Sphere ball_1(ball, red + blue, black, Point3(-7.5,17.5,7.5),5);
	manager -> addObject(&ball_1);

	Sphere ball_2(ball, (red + green + blue * 0.5), black, Point3(5,12.5,0),6);
	manager -> addObject(&ball_2);

	Sphere ball_3(ball, blue, black, Point3(0,25,0),7);
	manager -> addObject(&ball_3);

	manager -> calcRayCasting();
	return 0;
}
