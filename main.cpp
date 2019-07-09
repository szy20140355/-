#include "entity.h"
#include "const.h"
#include "basic.h"
#include "scene.h"
#include "mesh.h"
#include "beizer.h"

SceneManager* manager;
Image img; 
Image earth;
Image back_ground;
Image vase_img;

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

namespace DepthOfFieldScene1
{
	void work()
	{
		Camera camera(Point3(0, -10, 0));
		CanvasPlane3 canvas(Material(), Color(), Color(), Ray3(Point3(0,0,0), Vector3(0,1,0)),
							1000, 750, 20.0, 15.0, Ray3(Point3(-10, -15, -10), Vector3(1,0,0)), true);
			
		manager = new SceneManager(camera, &canvas);
		Material wall(0.7, 0.0, 0.7);
		Material wall_rough(1.0, 0.0, 0.8);
		Material floor(1.0, 0.0, 0.6);
		Material ball(0.2, 1.15, 1.0);
		Material ball_rough(0.95, 0.0, 1.0);
		Material ball_reflect(0.0, 0.0, 1.0);
		Material mesh(0.2, 1.3, 0.9);
		Material vase(0.8, 0.0, 0.9);

		img.readImage(image_path);
		earth.readImage(earth_path);
		back_ground.readImage(background_path);

		Plane3 wall_r(wall, blue, black, Ray3(Point3(35,0,0), Vector3(-1,0,0)));
		//Plane3 wall_front(wall_rough, white, black, Ray3(Point3(0,25,0), Vector3(0,-1,0)));
		Square3 wall_front(wall_rough, red * 0.8 + blue * 0.8 + green, green * 0.18 + red * 0.15 + blue * 0.15, Ray3(Point3(0,35,0), Vector3(0,-1,0))
				   ,back_ground.w, back_ground.h, 36.0, 28.5,
				    Ray3(Point3(-30, 35, 10), Vector3(1, 0, 0)));
		Plane3 wall_down(floor, (red + blue * 0.7 + green * 0.8) * 0.7, black, Ray3(Point3(0,0,-15), Vector3(0,0,1)));
		Square3 wall_up(wall_rough, white, black, Ray3(Point3(0,0,60),Vector3(0,0,1))
						,1,1,50.0,40.0,
						Ray3(Point3(-35,0,60), Vector3(1,0,0)));

		wall_up.setEmit(0, 0, white * 8);

		for(int i = 0; i < back_ground.w; i++)
			for(int j = 0; j < back_ground.h; j++)
			{
				wall_front.setColor(i,j, back_ground.pix[back_ground.h - j - 1][i]);
			}

		manager -> addObject(&wall_r);
		manager -> addObject(&wall_front);
		manager -> addObject(&wall_down);
		manager -> addObject(&wall_up);	



		//Sphere ball_1(vase, red, black, Point3(0,14,-6),2.5);
		//manager -> addObject(&ball_1);

		//RotatingBody vase_1(beizer_path, 1, Vector3(0, 12, 3), 0.0, vase, red, black);
		//RotatingBody vase_1(raindrop_path, 1, Vector3(9, 23, -5), - PI / 2.0, vase, red, black);
		//vase_1.setImg(earth, 18.6, 0);
		//manager -> addObject(&vase_1);

		//RotatingBody vase_2(raindrop_path, 1, Vector3(-30, 28, 2), 0.0, vase, red, black);
		//manager -> addObject(&vase_2);

		Mesh cat1(mesh_path, 24956, 49912, Vector3(-10, 25, -12.5), Vector3(0.3, 0.3, 0.3),  PI / 2.0, 0, 0,
		mesh, (red + green + blue * 0.3), black);
		manager -> addObject(&cat1);

		Mesh cat2(mesh_path, 24956, 49912, Vector3( 8, 15, -12.5), Vector3(0.25, 0.25, 0.25),  PI / 2.0, 0, -PI / 6.0,
		mesh, (red + green + blue * 0.3), black);
		manager -> addObject(&cat2);

	//	RotatingBody vase_1(vase_path, 1, Vector3(15, 27, -15), 0.0, vase, red, black);
	//	manager -> addObject(&vase_1);

		Sphere ball_2(ball_rough, red * 0.6 + green * 0.6 + blue, black, Point3(-40,28,-7),7);
		manager -> addObject(&ball_2);

		Sphere ball_3(ball, red + green * 0.3 + blue * 0.3, black, Point3(-23,16,-9),6);
		manager -> addObject(&ball_3);

	//
		Sphere ball_4(ball_reflect, red + green + blue * 0.3, black, Point3(27,23,24),8);
		manager -> addObject(&ball_4);

		manager -> calcRayCasting();
	}
};

namespace BeizerRotatingBody
{
	void work()
	{
		Camera camera(Point3(0, 0, 0));
		CanvasPlane3 canvas(Material(), Color(), Color(), Ray3(Point3(0,0,0), Vector3(0,1,0)),
							1000, 750, 20.0, 15.0, Ray3(Point3(-10, -5, -10), Vector3(1,0,0)), true);
			
		manager = new SceneManager(camera, &canvas);
		Material wall(0.7, 0.0, 0.7);
		Material wall_rough(1.0, 0.0, 0.8);
		Material floor(1.0, 0.0, 0.6);
		Material ball(0.2, 1.15, 1.0);
		Material ball_reflect(0.0, 0.0, 1.0);
		Material mesh(0.2, 1.3, 0.9);
		Material vase(0.8, 0.0, 0.9);

		img.readImage(image_path);
		earth.readImage(earth_path);
		back_ground.readImage(background_path);
		vase_img.readImage(texture_path);


		/*
		Plane3 wall_l(wall, red, black, Ray3(Point3(-25,0,0), Vector3(1,0,0)));
		Plane3 wall_r(wall, blue, black, Ray3(Point3(25,0,0), Vector3(-1,0,0)));
		Plane3 wall_front(wall_rough, white, white, Ray3(Point3(0,40,0), Vector3(0,-1,0)));
		Plane3 wall_down(wall_rough, red + blue + green *0.8, black, Ray3(Point3(0,0,-15), Vector3(0,0,1)));
		Square3 wall_up(wall_rough, white, black, Ray3(Point3(0,0,20),Vector3(0,0,1))
						,1,1,10.0,10.0,
						Ray3(Point3(-5,20,20), Vector3(1,0,0)));

		wall_up.setEmit(0, 0, white);

		manager -> addObject(&wall_l);
		manager -> addObject(&wall_r);
		manager -> addObject(&wall_front);
		manager -> addObject(&wall_down);
		manager -> addObject(&wall_up);	*/



		Plane3 wall_r(wall, blue, black, Ray3(Point3(35,0,0), Vector3(-1,0,0)));
		//Plane3 wall_front(wall_rough, white, black, Ray3(Point3(0,25,0), Vector3(0,-1,0)));
		Square3 wall_front(wall_rough, red * 0.8 + blue * 0.8 + green, green * 0.18 + red * 0.15 + blue * 0.15, Ray3(Point3(0,35,0), Vector3(0,-1,0))
				   ,back_ground.w, back_ground.h, 36.0, 28.5,
				    Ray3(Point3(-30, 35, 5), Vector3(1, 0, 0)));
		Plane3 wall_down(floor, (red + blue * 0.7 + green * 0.8) * 0.7, black, Ray3(Point3(0,0,-15), Vector3(0,0,1)));
		Square3 wall_up(wall_rough, white, black, Ray3(Point3(0,0,55),Vector3(0,0,1))
						,1,1,50.0,40.0,
						Ray3(Point3(-35,0,55), Vector3(1,0,0)));

		wall_up.setEmit(0, 0, white * 8);

		for(int i = 0; i < back_ground.w; i++)
			for(int j = 0; j < back_ground.h; j++)
			{
				wall_front.setColor(i,j, back_ground.pix[back_ground.h - j - 1][i]);
			}

		manager -> addObject(&wall_r);
		manager -> addObject(&wall_front);
		manager -> addObject(&wall_down);
		manager -> addObject(&wall_up);	

		//Sphere ball_1(vase, red, black, Point3(0,14,-6),2.5);
		//manager -> addObject(&ball_1);

		//RotatingBody vase_1(beizer_path, 1, Vector3(0, 12, 3), 0.0, vase, red, black);
		//RotatingBody vase_1(raindrop_path, 1, Vector3(9, 23, -5), - PI / 2.0, vase, red, black);
		//vase_1.setImg(earth, 18.6, 0);
		//manager -> addObject(&vase_1);

		//RotatingBody vase_2(raindrop_path, 1, Vector3(-30, 28, 2), 0.0, vase, red, black);
		//manager -> addObject(&vase_2);

		//RotatingBody vase_1(vase_path, 1, Vector3(10, 27, -15), 0.0, vase, red, black);
		//manager -> addObject(&vase_1);

		RotatingBody vase_2(vase_path, 1, Vector3(10, 27, -15), 0.0, vase, red, black);
		vase_2.setImg(vase_img, 21.9, 0);
		manager -> addObject(&vase_2);

		//Mesh dragon(dragon_path, 104855, 209227, Vector3(-3, 22, -12.5), Vector3(0.3, 0.3, 0.3),  PI / 2.0, 0, 0,
		//mesh, (red * 0.6 + green + blue * 0.8), black);
		//manager -> addObject(&dragon);

		//Mesh cat2(mesh_path, 24956, 49912, Vector3( -5, 16, -12.5), Vector3(0.12, 0.12, 0.12),  PI / 2.0, 0, -PI / 6.0,
		//mesh, (red + green * 0.7 + blue * 0.6), black);
		//manager -> addObject(&cat2);

		//Sphere ball_1(ball, green * 0.3 + blue, black, Point3(10,25,-15),5);
		//manager -> addObject(&ball_1);

		Sphere ball_2(ball_reflect, red + green + blue * 0.3, black, Point3(-8,28,-10),7);
		manager -> addObject(&ball_2);
	//
		//Sphere ball_3(ball_reflect, red + green + blue * 0.3, black, Point3(-11,27,-15),7);
		//manager -> addObject(&ball_3);

		manager -> calcRayCasting();
	}
};

namespace BeizerRotatingBody1
{
	void work()
	{
		Camera camera(Point3(0, -10, 0));
		CanvasPlane3 canvas(Material(), Color(), Color(), Ray3(Point3(0,0,0), Vector3(0,1,0)),
							200, 150, 20.0, 15.0, Ray3(Point3(-10, -15, -10), Vector3(1,0,0)), true);
			
		manager = new SceneManager(camera, &canvas);
		Material wall(0.7, 0.0, 0.7);
		Material wall_rough(1.0, 0.0, 0.8);
		Material floor(1.0, 0.0, 0.6);
		Material ball(0.2, 1.15, 1.0);
		Material ball_rough(0.95, 0.0, 1.0);
		Material ball_reflect(0.0, 0.0, 1.0);
		Material mesh(0.2, 1.3, 0.9);
		Material vase(0.8, 0.0, 0.9);

		img.readImage(image_path);
		earth.readImage(earth_path);
		back_ground.readImage(background_path);
		vase_img.readImage(texture_path);

		Plane3 wall_r(wall, blue, black, Ray3(Point3(35,0,0), Vector3(-1,0,0)));
		//Plane3 wall_front(wall_rough, white, black, Ray3(Point3(0,25,0), Vector3(0,-1,0)));
		Square3 wall_front(wall_rough, red * 0.8 + blue * 0.8 + green, green * 0.18 + red * 0.15 + blue * 0.15, Ray3(Point3(0,35,0), Vector3(0,-1,0))
				   ,back_ground.w, back_ground.h, 36.0, 28.5,
				    Ray3(Point3(-30, 35, 10), Vector3(1, 0, 0)));
		Plane3 wall_down(floor, (red + blue * 0.7 + green * 0.8) * 0.7, black, Ray3(Point3(0,0,-15), Vector3(0,0,1)));
		Square3 wall_up(wall_rough, white, black, Ray3(Point3(0,0,60),Vector3(0,0,1))
						,1,1,50.0,40.0,
						Ray3(Point3(-35,0,60), Vector3(1,0,0)));

		wall_up.setEmit(0, 0, white * 8);

		for(int i = 0; i < back_ground.w; i++)
			for(int j = 0; j < back_ground.h; j++)
			{
				wall_front.setColor(i,j, back_ground.pix[back_ground.h - j - 1][i]);
			}

		manager -> addObject(&wall_r);
		manager -> addObject(&wall_front);
		manager -> addObject(&wall_down);
		manager -> addObject(&wall_up);	

		//Sphere ball_1(vase, red, black, Point3(0,14,-6),2.5);
		//manager -> addObject(&ball_1);

		//RotatingBody vase_1(beizer_path, 1, Vector3(0, 12, 3), 0.0, vase, red, black);
		//RotatingBody vase_1(raindrop_path, 1, Vector3(9, 23, -5), - PI / 2.0, vase, red, black);
		//vase_1.setImg(earth, 18.6, 0);
		//manager -> addObject(&vase_1);

		RotatingBody vase_2(vase_path, 1, Vector3(12, 28, -15), 0.0, vase, red, black);
		vase_2.setImg(vase_img, 21.9, 0);
		manager -> addObject(&vase_2);

		Mesh cat1(mesh_path, 24956, 49912, Vector3(-10, 25, -12.5), Vector3(0.3, 0.3, 0.3),  PI / 2.0, 0, 0,
		mesh, (red + green + blue * 0.3), black);
		manager -> addObject(&cat1);

	//	RotatingBody vase_1(vase_path, 1, Vector3(15, 27, -15), 0.0, vase, red, black);
	//	manager -> addObject(&vase_1);

		Sphere ball_2(ball_rough, red * 0.6 + green * 0.6 + blue, black, Point3(-40,28,-7),7);
		manager -> addObject(&ball_2);

		Sphere ball_3(ball, red + green * 0.3 + blue * 0.3, black, Point3(-23,16,-9),6);
		manager -> addObject(&ball_3);

	//
	//	Sphere ball_4(ball_reflect, red + green + blue * 0.3, black, Point3(27,23,24),8);
	//	manager -> addObject(&ball_4);

		manager -> calcRayCasting();
	}
};

namespace Scene1
{
	void work()
	{
		Camera camera(Point3(0, 5, 0));
		CanvasPlane3 canvas(Material(), Color(), Color(), Ray3(Point3(0,0,0), Vector3(0,1,0)),
							400, 400, 20.0, 20.0, Ray3(Point3(-10, 0, -10), Vector3(1,0,0)), true);
			
		manager = new SceneManager(camera, &canvas);
		Material wall(0.7, 0.0, 0.7);
		Material wall_rough(1.0, 0.0, 0.8);
		Material ball(0.2, 1.15, 1.0);
		Material ball_reflect(0.0, 0.0, 1.0);
		Material mesh(0.2, 1.3, 0.9);
		Material vase(0.6, 0.0, 0.9);

		//img.readImage(image_path);
		//earth.readImage(earth_path);

		Plane3 wall_r(wall, blue, black, Ray3(Point3(25,0,0), Vector3(-1,0,0)));
		Plane3 wall_front(wall_rough, white, black, Ray3(Point3(0,25,0), Vector3(0,-1,0)));
		Plane3 wall_down(wall_rough, red + blue + green *0.8, black, Ray3(Point3(0,0,-20), Vector3(0,0,1)));
		//Plane3 wall_up(wall_rough, white, white * 3, Ray3(Point3(0,0,35), Vector3(0,0,1)));
		Square3 wall_up(wall_rough, white, black, Ray3(Point3(0,0,45),Vector3(0,0,1))
						,1,1,40.0,40.0,
						Ray3(Point3(-20,0,45), Vector3(1,0,0)));

		wall_up.setEmit(0, 0, white * 5);

		manager -> addObject(&wall_r);
		manager -> addObject(&wall_front);
		manager -> addObject(&wall_down);
		manager -> addObject(&wall_up);	

		//Sphere ball_1(vase, red, black, Point3(0,10,0),3);
		//manager -> addObject(&ball_1);

		//RotatingBody vase_1(raindrop_path, 1, Vector3(0, 10, -3), 0.0, vase, red, black);
		//manager -> addObject(&vase_1);
		
		//RotatingBody vase_1(raindrop_path, 1, Vector3(0, 8, -2), 0.0, vase, red, black);
		//vase_1.setImg(earth, 4, 0);
		//manager -> addObject(&vase_1);

		RotatingBody vase_1(vase_path, 1, Vector3(0, 12, -4), 0.0, vase, red, black);
		//vase_1.setImg(earth, 4, 0);
		manager -> addObject(&vase_1);

		manager -> calcRayCasting();
	}
};

namespace Scene2
{
	void work()
	{
		Camera camera(Point3(0, 5, 0));
		CanvasPlane3 canvas(Material(), Color(), Color(), Ray3(Point3(0,0,0), Vector3(0,1,0)),
							400, 400, 20.0, 20.0, Ray3(Point3(-10, 0, -10), Vector3(1,0,0)), true);
			
		manager = new SceneManager(camera, &canvas);
		Material wall(0.7, 0.0, 0.7);
		Material wall_rough(1.0, 0.0, 0.8);
		Material ball(0.2, 1.15, 1.0);
		Material ball_reflect(0.0, 0.0, 1.0);
		Material mesh(0.2, 1.3, 0.9);
		Material vase(0.6, 0.0, 0.9);

		img.readImage(image_path);
		earth.readImage(earth_path);
		back_ground.readImage(background_path);

		Plane3 wall_r(wall, blue, black, Ray3(Point3(25,0,0), Vector3(-1,0,0)));
		//Plane3 wall_front(wall_rough, white, black, Ray3(Point3(0,25,0), Vector3(0,-1,0)));
		Square3 wall_front(wall_rough, white, black, Ray3(Point3(0,25,0), Vector3(0,-1,0))
						   ,back_ground.w, back_ground.h, 24.0, 19.0,
						    Ray3(Point3(-10, 25, 8), Vector3(0, -1, 0)));
		Plane3 wall_down(wall_rough, red + blue + green * 0.8, black, Ray3(Point3(0,0,-20), Vector3(0,0,1)));
		//Plane3 wall_up(wall_rough, white, white * 3, Ray3(Point3(0,0,35), Vector3(0,0,1)));
		Square3 wall_up(wall_rough, white, black, Ray3(Point3(0,0,45),Vector3(0,0,1))
						,1,1,40.0,40.0,
						Ray3(Point3(-20,0,45), Vector3(1,0,0)));

		wall_up.setEmit(0, 0, white * 5);

		for(int i = 0; i < back_ground.w; i++)
			for(int j = 0; j < back_ground.h; j++)
			{
				wall_front.setColor(i,j, back_ground.pix[j][i]);
			}

		manager -> addObject(&wall_r);
		manager -> addObject(&wall_front);
		manager -> addObject(&wall_down);
		manager -> addObject(&wall_up);	

		//Sphere ball_1(vase, red, black, Point3(0,10,0),3);
		//manager -> addObject(&ball_1);

		//RotatingBody vase_1(raindrop_path, 1, Vector3(0, 10, -3), 0.0, vase, red, black);
		//manager -> addObject(&vase_1);
		
		//RotatingBody vase_1(raindrop_path, 1, Vector3(0, 8, -2), 0.0, vase, red, black);
		//vase_1.setImg(earth, 4, 0);
		//manager -> addObject(&vase_1);

		RotatingBody vase_1(vase_path, 1, Vector3(0, 12, -4), 0.0, vase, red, black);
		//vase_1.setImg(earth, 4, 0);
		manager -> addObject(&vase_1);

		manager -> calcRayCasting();
	}
};

int main()
{
	srand(time(NULL));

	BeizerRotatingBody::work();

	return 0;
}
