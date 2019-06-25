#include "const.h"
#include "basic.h"
#include "entity.h"
#include "scene.h"
#include "mesh.h"

SceneManager* manager;
Image img; 


namespace RoomScene
{
	void work()
	{
		freopen("error.out", "w", stderr);

		cerr << "###begin" << endl;
		fflush(stderr);

		Camera camera(Point3(0, 0, 0));
		CanvasPlane3 canvas(Material(), Color(), Color(), Ray3(Point3(0,5,0), Vector3(0,-1,0)),
							800, 800, 20.0, 20.0, Ray3(Point3(-10, 5, -10), Vector3(1,0,0)));
			
		manager = new SceneManager(camera, &canvas);
		Material wall(0.7, 0.0, 0.7);
		Material wall_rough(0.9, 0.0, 0.8);
		Material ball(0.2, 1.15, 1.0);
		Material ball_reflect(0.0, 0.0, 1.0);
		Material mesh(0.2, 1.3, 0.9);

		cerr << "###finish0" << endl;
		fflush(stderr);

		img.readImage(image_path);

		Plane3 wall_l(wall, red, black, Ray3(Point3(-15,0,0), Vector3(1,0,0)));
		Plane3 wall_r(wall, blue, black, Ray3(Point3(15,0,0), Vector3(-1,0,0)));
		Plane3 wall_front(wall, white, white, Ray3(Point3(0,40,0), Vector3(0,-1	,0)));
		Plane3 wall_down(wall, red + blue + green *0.2, black, Ray3(Point3(0,0,-15), Vector3(0,0,1)));
		Square3 wall_up(wall_rough, black, black, Ray3(Point3(0,0,15),Vector3(0,0,1))
						,img.w,img.h,20.0,20.0,
						Ray3(Point3(-10,10,15), Vector3(1,0,0)));
		
		cerr << "###finish1" << endl;
		fflush(stderr);

		for(int i = 0; i < img.w; i++)
			for(int j = 0; j < img.h; j++)
			{
				wall_up.setEmit(i,j, img.pix[j][i]);
				//cerr << img.pix[j][i] << endl;
				wall_up.setColor(i,j, black);
			}

		cerr << "###finish2" << endl;
		fflush(stderr);

		manager -> addObject(&wall_l);
		manager -> addObject(&wall_r);
		manager -> addObject(&wall_front);
		manager -> addObject(&wall_down);
		manager -> addObject(&wall_up);


		//Sphere ball_1(ball, red + blue, black, Point3(-7.5,17.5,7.5),5);
		//manager -> addObject(&ball_1);

		Sphere ball_2(ball, white, black, Point3(2.5,10,-2),3.5);
		manager -> addObject(&ball_2);

		Sphere ball_3(ball_reflect, white, black, Point3(-4,14,-7),3);
		manager -> addObject(&ball_3);

		//Sphere ball_3(ball, blue, black, Point3(0,25,0),7);
		//manager -> addObject(&ball_3);

		//Sphere ball_3(ball, blue, black, Point3(0,25,0),7);
		//manager -> addObject(&ball_3);

		Mesh cat(mesh_path, 24956, 49912, Vector3(0, 23, -12.5), Vector3(0.3, 0.3, 0.3),  PI / 2.0, 0, 0,
		mesh, (red + green + blue * 0.6), black);
		manager -> addObject(&cat);

		//Mesh test(mesh_path, 6, 8, Vector3(0, 20, 0), Vector3(2, 2, 2), mesh, red + green + blue * 0.5, black);
		//manager -> addObject(&test);

		manager -> calcRayCasting();
	}
};

namespace TestScene
{
	void work()
	{
		freopen("error.out", "w", stderr);

		cerr << "###begin" << endl;
		fflush(stderr);

		Camera camera(Point3(0, 0, 0));
		CanvasPlane3 canvas(Material(), Color(), Color(), Ray3(Point3(0,5,0), Vector3(0,-1,0)),
							200, 200, 20.0, 20.0, Ray3(Point3(-10, 5, -10), Vector3(1,0,0)));
			
		manager = new SceneManager(camera, &canvas);
		Material wall(0.7, 0.0, 0.7);
		Material wall_rough(0.9, 0.0, 0.8);
		Material ball(0.2, 1.15, 1.0);
		Material ball_reflect(0.0, 0.0, 1.0);
		Material mesh(0.2, 1.3, 0.9);

		cerr << "###finish0" << endl;
		fflush(stderr);

		img.readImage(image_path);

		Plane3 wall_down(wall, red + blue + green *0.2, black, Ray3(Point3(0,0,-15), Vector3(0,0,1)));
		Square3 wall_up(wall_rough, black, black, Ray3(Point3(0,0,15),Vector3(0,0,1))
						,img.w,img.h,20.0,20.0,
						Ray3(Point3(-10,10,15), Vector3(1,0,0)));
		
		cerr << "###finish1" << endl;
		fflush(stderr);

		for(int i = 0; i < img.w; i++)
			for(int j = 0; j < img.h; j++)
			{
				wall_up.setEmit(i,j, img.pix[j][i] * 10.0);
				//cerr << img.pix[j][i] << endl;
				wall_up.setColor(i,j, black);
			}

		cerr << "###finish2" << endl;
		fflush(stderr);

		manager -> addObject(&wall_down);
		manager -> addObject(&wall_up);


		//Sphere ball_1(ball, red + blue, black, Point3(-7.5,17.5,7.5),5);
		//manager -> addObject(&ball_1);

		Sphere ball_2(ball, white, black, Point3(2.5,10,-2),3.5);
		manager -> addObject(&ball_2);

		Sphere ball_3(ball_reflect, white, black, Point3(-4,14,-7),3);
		manager -> addObject(&ball_3);

		//Sphere ball_3(ball, blue, black, Point3(0,25,0),7);
		//manager -> addObject(&ball_3);

		//Sphere ball_3(ball, blue, black, Point3(0,25,0),7);
		//manager -> addObject(&ball_3);

		Mesh cat(mesh_path, 24956, 49912, Vector3(0, 23, -12.5), Vector3(0.3, 0.3, 0.3),  PI / 2.0, 0, 0,
		mesh, (red + green + blue * 0.6), black);
		manager -> addObject(&cat);

		//Mesh test(mesh_path, 6, 8, Vector3(0, 20, 0), Vector3(2, 2, 2), mesh, red + green + blue * 0.5, black);
		//manager -> addObject(&test);

		manager -> calcRayCasting();
	}
};

namespace DepthOfFieldScene
{
	void work()
	{
		Camera camera(Point3(0, 5, 0));
		CanvasPlane3 canvas(Material(), Color(), Color(), Ray3(Point3(0,0,0), Vector3(0,1,0)),
							1000, 1000, 20.0, 20.0, Ray3(Point3(-10, 0, -10), Vector3(1,0,0)), true);
			
		manager = new SceneManager(camera, &canvas);
		Material wall(0.7, 0.0, 0.7);
		Material wall_rough(1.0, 0.0, 0.8);
		Material ball(0.2, 1.15, 1.0);
		Material ball_reflect(0.0, 0.0, 1.0);
		Material mesh(0.2, 1.3, 0.9);

		img.readImage(image_path);

		Plane3 wall_l(wall_rough, red, black, Ray3(Point3(-25,0,0), Vector3(1,0,0)));
		Plane3 wall_r(wall_rough, blue, black, Ray3(Point3(25,0,0), Vector3(-1,0,0)));
		Plane3 wall_front(wall_rough, white, white * 0.6, Ray3(Point3(0,40,0), Vector3(0,-1	,0)));
		Plane3 wall_down(wall_rough, red + blue + green *0.8, black, Ray3(Point3(0,0,-15), Vector3(0,0,1)));
		Square3 wall_up(wall_rough, white * 0.3, black, Ray3(Point3(0,0,20),Vector3(0,0,1))
						,img.w,img.h,20.0,20.0,
						Ray3(Point3(-10,15,20), Vector3(1,0,0)));

		for(int i = 0; i < img.w; i++)
			for(int j = 0; j < img.h; j++)
			{
				wall_up.setEmit(i,j, img.pix[j][i] * 8.0);
				//cerr << img.pix[j][i] << endl;
				wall_up.setColor(i,j, black);
			}

		manager -> addObject(&wall_l);
		manager -> addObject(&wall_r);
		manager -> addObject(&wall_front);
		manager -> addObject(&wall_down);	
		manager -> addObject(&wall_up);	

/* 
		Sphere ball_1(ball, red, black, Point3(4,11,0),3);
		manager -> addObject(&ball_1);

		Sphere ball_2(ball, green, black, Point3(0,13,0),4);
		manager -> addObject(&ball_2);

		Sphere ball_3(ball, blue, black, Point3(-4,16,0),5);
		manager -> addObject(&ball_3);*/

		Mesh cat1(mesh_path, 24956, 49912, Vector3(-5, 30, -12.5), Vector3(0.3, 0.3, 0.3),  PI / 2.0, 0, 0,
		mesh, (red + green + blue * 0.6), black);
		manager -> addObject(&cat1);

		Mesh cat2(mesh_path, 24956, 49912, Vector3( 8, 20, -12.5), Vector3(0.2, 0.2, 0.2),  PI / 2.0, 0, -PI / 6.0,
		mesh, (red + green + blue * 0.6), black);
		manager -> addObject(&cat2);

		manager -> calcRayCasting();
	}
};

int main()
{
	srand(time(NULL));

	DepthOfFieldScene::work();

	return 0;
}
