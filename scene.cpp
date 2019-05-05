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
	for(int i = h_pixel - 1; i >= 0; i--)
		for(int j = 0; j < w_pixel; j++)
		{
			ofs << (unsigned char)(min(1.0, p_color[j][i].r) * color_range) <<
				   (unsigned char)(min(1.0, p_color[j][i].g) * color_range) <<
				   (unsigned char)(min(1.0, p_color[j][i].b) * color_range);
		}
	ofs.close();
	ofstream ofs1(debug_path, ios::out);
	for(int i = 0; i < h_pixel; i++)
		for(int j = 0; j < w_pixel; j++)
		{
			ofs1 << (int)(min(1.0, p_color[j][i].r) * color_range) << " " <<
				    (int)(min(1.0, p_color[j][i].g) * color_range) << " " <<
				    (int)(min(1.0, p_color[j][i].b) * color_range) << endl;
		}
	ofs1.close();
	
}

SceneManager::SceneManager(Camera _camera, CanvasPlane3* _canvas) :
camera(_camera), canvas(_canvas){}
void SceneManager::addObject(Object3* object)
{
	objects.push_back(object);
}
int SceneManager::getRayNum(int depth, double prop)
{
	return max(1, (int)(prop * pow(layer_inc, depth - 1)));
}
Color SceneManager::trace(Ray3 ray, int depth, double prop)
{
	InsertInfo info(false);
	Object3* ins_obj = NULL;
	for(int i = 0; i < objects.size(); i++)
	{
		InsertInfo t_info = objects[i] -> insertLight(ray);
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
	reflect = trace(Ray3(p_up, t.reflect_dir), depth + 1, prop * t.reflect_prop);
	if(t.refract_prop) refract = trace(Ray3(p_down, t.refract_dir), depth + 1, prop * t.refract_prop);
	
	ret = ret + reflect * t.reflect_prop;
	if(info.inside)
		ret = ret + refract * t.refract_prop;
	else ret = ret + refract * t.refract_prop * t.color;

	for(int i = 0; i < ray_num; i++)
	{
		Color diff = trace(Ray3(p_up, t.diff_dir[i]), depth + 1, prop * t.frac[i]);
		ret = ret + diff * t.frac[i] * t.color;
	}
	ret = ret + t.emit * t.emit_prop;
	return ret;
}
void SceneManager::calcRayCasting()
{
#ifdef DEPTH_OF_FIELD
#else // DEPTH_OF_FIELD
	
	for(int i = 0; i < canvas -> w_pixel; i++)
		for(int j = 0; j < canvas -> h_pixel; j++)
		{
		#ifdef PATH_TRACING
		
			Point3 pos = canvas -> getPos(i, j);
			Color c = trace(Ray3(camera.position, pos - camera.position), 1, 1.0);
			canvas -> setColor(i, j, c);
			cout << i << " " << j << endl;
			
		#endif // PATH_TRACING
		}
#endif // DEPTH_OF_FIELD

	canvas -> print();
}
