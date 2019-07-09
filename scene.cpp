#include "scene.h"

Point3 CanvasPlane3::getPos(int x, int y)
{
	return x_dir.start + x_dir.dir * (double)(x + 0.5) * x_interval  
					   + y_dir.dir * (double)(y + 0.5) * y_interval;
}
void CanvasPlane3::setColor(int x, int y, Color c)
{
	p_color[x][y] = c;
}
void CanvasPlane3::print()
{
	ofstream ofs(output_path, ios::out | ios::binary);
	ofs << "P6\n" << w_pixel << " " << h_pixel << "\n255\n";
	/* 
	for(int i = h_pixel - 1; i >= 0; i--)
		for(int j = 0; j < w_pixel; j++) */
	for(int i = 0; i < h_pixel; i++)
		for(int j = w_pixel - 1; j >= 0; j--)
		{
			double t = max(max(p_color[j][i].r, p_color[j][i].g), p_color[j][i].b);
			double factor = t > 1.0 ? 1.0 / t : 1.0;
			ofs << (unsigned char)(p_color[j][i].r * factor * color_range) <<
				   (unsigned char)(p_color[j][i].g * factor * color_range) <<
				   (unsigned char)(p_color[j][i].b * factor * color_range);
		}
	ofs.close();
	//ofstream ofs1(debug_path, ios::out);
	/*
	for(int i = h_pixel - 1; i >= 0; i--)
	{
		ofs << i << " " << 200 << " : ";
		ofs1 << (int)(min(1.0, p_color[200][i].r) * color_range) << " " <<
		(int)(min(1.0, p_color[200][i].g) * color_range) << " " <<
		(int)(min(1.0, p_color[200][i].b) * color_range) << endl;
	}
	/*for(int i = 0; i < h_pixel; i++)
		for(int j = 0; j < w_pixel; j++)
		{
			ofs1 << (int)(min(1.0, p_color[j][i].r) * color_range) << " " <<
				    (int)(min(1.0, p_color[j][i].g) * color_range) << " " <<
				    (int)(min(1.0, p_color[j][i].b) * color_range) << endl;
		}*/
	
	//ofs1.close();
	
}

SceneManager::SceneManager(Camera _camera, CanvasPlane3* _canvas) :
camera(_camera), canvas(_canvas)
{
    #ifdef DEPTH_OF_FIELD
    focal_plane.normal.start = camera.position + (canvas -> normal).dir * focal_length;
	focal_plane.normal.dir = (canvas -> normal).dir; 
    #endif // DEPTH_OF_FIELD
}

void SceneManager::addObject(Object3* object)
{
	objects.push_back(object);
}
int SceneManager::getRayNum(int depth, double prop)
{
	return max(1, (int)(trace_time * prop * pow(layer_inc, depth - 1)));
	//return 1;
}
Color SceneManager::trace(Ray3 ray, int depth, double prop, bool pre_inside)
{
	InsertInfo info(false);
	Object3* ins_obj = NULL;
	for(int i = 0; i < objects.size(); i++)
	{
		InsertInfo t_info = objects[i] -> insertLight(ray, pre_inside);
		if(t_info.insert)
		{
			if(!info.insert || info.distance2(ray.start) > t_info.distance2(ray.start))
			{
				info = t_info;
				ins_obj = objects[i];
			}
		}
	}
	if(!ins_obj)
	{
		return background;
	}
	if(depth == max_depth || prop < min_prop)
	{
		localLightProc t(ins_obj, info, ray, 0, true);
		return t.emit * t.emit_prop;
	}
	int ray_num = getRayNum(depth, prop);
	localLightProc t(ins_obj, info, ray, ray_num);

	Color ret = black, reflect = black, refract = black;
	Point3 p_up = info.normal.start + info.normal.dir * bias, 
		   p_down = info.normal.start - info.normal.dir * bias;
	reflect = trace(Ray3(p_up, t.reflect_dir), depth + 1, prop * t.reflect_prop, pre_inside);
	if(t.refract_prop) 
		refract = trace(Ray3(p_down, t.refract_dir), depth + 1, prop * t.refract_prop, !pre_inside);
	
	ret = ret + reflect * t.reflect_prop;
	if(info.inside)
		ret = ret + refract * t.refract_prop;
	else ret = ret + refract * t.refract_prop * t.color;

	for(int i = 0; i < ray_num; i++)
	{
		Color diff = trace(Ray3(p_up, t.diff_dir[i]), depth + 1, prop * t.frac[i], pre_inside);
		ret = ret + diff * t.frac[i] * t.color;
	}
	ret = ret + t.emit * t.emit_prop;
	return ret;
}
void SceneManager::calcRayCasting()
{
#ifdef DEPTH_OF_FIELD
	static RandomGen random_gen;
#endif // DEPTH_OF_FIELD

	int start_tim = clock();
	int tot_block = canvas -> w_pixel * canvas -> h_pixel;
	int curr_block = 0;

	Point3 pos = canvas -> getPos(canvas -> w_pixel / 2, 10);
	Color c = trace(Ray3(pos, camera.position - pos), 1, 1.0, false);	

	for(int i = 0; i < canvas -> w_pixel; i++)
	{
		#pragma omp parallel for
		for(int j = 0; j < canvas -> h_pixel; j++)
		{

		#ifdef PATH_TRACING
			Point3 pos = canvas -> getPos(i, j);
		
			#ifdef DEPTH_OF_FIELD
				Color c;
				for(int i = 0; i < ray_num; i++)
				{
					double tx, ty;
					do
					{
						tx = random_gen.getRand() * 2.0 - 1.0;
						ty = random_gen.getRand() * 2.0 - 1.0;
					}while(tx * tx + ty * ty > 1.0);
					tx *= lens_diameter * 0.5;
					ty *= lens_diameter * 0.5;
					Point3 tp = camera.position + (canvas -> x_dir).dir * tx + (canvas -> y_dir).dir * ty;
					InsertInfo t_info = focal_plane.insertRay(Ray3(pos, camera.position - pos));
					Point3 tp1 = t_info.normal.start;
					c = c + trace(Ray3(tp, tp1 - tp), 1, 1.0, false) * (1.0 / ray_num);
				}
			#else
				Color c = trace(Ray3(pos, camera.position - pos), 1, 1.0, false);
				//Color c = trace(Ray3(camera.position, pos - camera.position), 1, 1.0);
			#endif // DEPTH_OF_FIELD

			canvas -> setColor(i, j, c);
			cout << i << " " << j << endl;
			fflush(stdout);
			fflush(stderr);
			
		#endif // PATH_TRACING
		
			curr_block ++;
			cout << "expect time : " << (clock() - start_tim) / curr_block * (tot_block - curr_block) / 1000 << endl; 
			fflush(stdout);
		}
		cout << "total time : " << (clock() - start_tim) / 1000;
		fflush(stdout);
	}
	canvas -> print();
}
