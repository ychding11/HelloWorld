//#include "wxraytracer.h"
#include "World.h"
#include "Constants.h"

// geometric objects
#include "Plane.h"
#include "Sphere.h"
//#include "Box.h"

// tracers
#include "SingleSphere.h"
#include "MultipleObjects.h"
#include "RayCast.h"

// cameras
#include "Pinhole.h"
#include "FishEye.h"
#include "ThinLens.h"

#include "MultiJittered.h"

// lights
#include "Directional.h"

// materials
#include "Matte.h"

// utilities
#include "Vector3D.h"
#include "Point3D.h"
#include "Normal.h"
#include "ShadeRec.h"
#include "Maths.h"


#include <cstdio> 




// -------------------------------------------------------------------- default constructor

// tracer_ptr is set to NULL because the build functions will always construct the appropriate tracer
// ambient_ptr is set to a default ambient light because this will do for most scenes
// camera_ptr is set to NULL because the build functions will always have to construct a camera
// and set its parameters

World::World(void)
	    :background_color(black)
		,tracer_ptr(NULL)
		,ambient_ptr(new Ambient)
		,camera_ptr(NULL)
        ,vp(200, 150)
{}


//------------------------------------------------------------------ destructor

World::~World(void)
{

	delete tracer_ptr;
	tracer_ptr = NULL;
		
	delete ambient_ptr;
	ambient_ptr = NULL;
		
	delete_objects();	
	delete_lights();				
    delete_cameras();
	camera_ptr = NULL;
}


void
World::set_pixel_samples(int num)
{
    vp.set_samples(num);
}

//------------------------------------------------------------------ render_scene
void 												
World::render_scene(void) const
{
    if (camera_ptr)
        camera_ptr->render_scene(*this);
}


//------------------------------------------------------------------ render_scene
// This uses orthographic viewing along the zw axis
void 												
World::render_orthographic(void)
{

	RGBColor	pixel_color;	 	
	Ray			ray;					
	int 		hres 	= vp.hres;
	int 		vres 	= vp.vres;
	float		s		= vp.s;
	float		zw		= 200.0;				// hardwired in

	ray.d = Vector3D(0, 0, -1);
    fprintf(stdout, "- Render parameter. w=%d h=%d samples=%d.\n", vp.hres, vp.vres, 1);	
	for (int r = 0; r < vres; r++)
    {
        fprintf(stdout, "\r - Orthographic Camera Rendering... %.2f%%.", 100. * float(r) / float(vres - 1));
		for (int c = 0; c < hres; c++)
        {
			ray.o = Point3D(s * (c - hres / 2.0 + 0.5), s * (r - vres / 2.0 + 0.5), zw);
			pixel_color = tracer_ptr->trace_ray(ray);
            vp.write_to_buffer(vres - r - 1, c, pixel_color);
		}	
    }
    fprintf(stdout, "\n- Orthographic Camera Rendering... OK.\n");
    vp.save_to_ppm("orthographic.ppm");
}  


// ------------------------------------------------------------------ clamp

RGBColor
World::max_to_one(const RGBColor& c) const
{
	float max_value = max(c.r, max(c.g, c.b));
	
	if (max_value > 1.0)
		return (c / max_value);
	else
		return (c);
}


// ------------------------------------------------------------------ clamp_to_color
// Set color to red if any component is greater than one

RGBColor
World::clamp_to_color(const RGBColor& raw_color) const
{
	RGBColor c(raw_color);
	
	if (raw_color.r > 1.0 || raw_color.g > 1.0 || raw_color.b > 1.0)
	{
		c.r = 1.0; c.g = 0.0; c.b = 0.0;
	}
		
	return (c);
}


// ---------------------------------------------------------------------------display_pixel
// raw_color is the pixel color computed by the ray tracer
// its RGB floating point components can be arbitrarily large
// mapped_color has all components in the range [0, 1], but still floating point
// display color has integer components for computer display
// the Mac's components are in the range [0, 65535]
// a PC's components will probably be in the range [0, 255]
// the system-dependent code is in the function convert_to_display_color
// the function SetCPixel is a Mac OS function


void
World::display_pixel(const int row, const int column, const RGBColor& raw_color) const
{
	RGBColor mapped_color;

	if (vp.show_out_of_gamut)
		mapped_color = clamp_to_color(raw_color);
	else
		mapped_color = max_to_one(raw_color);
	
	if (vp.gamma != 1.0)
		mapped_color = mapped_color.powc(vp.inv_gamma);
	
   //have to start from max y coordinate to convert to screen coordinates
   int x = column;
   int y = vp.vres - row - 1;
/*
   paintArea->setPixel(x, y, (int)(mapped_color.r * 255),
                             (int)(mapped_color.g * 255),
                             (int)(mapped_color.b * 255));
   */
}

RGBColor 
World::convert_color(const RGBColor& raw_color) const
{
	RGBColor mapped_color;

	if (vp.show_out_of_gamut)
		mapped_color = clamp_to_color(raw_color);
	else
		mapped_color = max_to_one(raw_color);
	
	if (vp.gamma != 1.0)
		mapped_color = mapped_color.powc(vp.inv_gamma);

   mapped_color.r = (int)(mapped_color.r * 255);
   mapped_color.g = (int)(mapped_color.g * 255);
   mapped_color.b = (int)(mapped_color.b * 255);
   
   return mapped_color;
}
// ----------------------------------------------------------------------------- hit_objects

ShadeRec									
World::hit_objects(const Ray& ray)
{

	ShadeRec	sr(*this);
	double		t;
	Normal normal;
	Point3D local_hit_point;
	float		tmin 			= kHugeValue;
	int 		num_objects 	= objects.size();
	
	for (int j = 0; j < num_objects; j++)
		if (objects[j]->hit(ray, t, sr) && (t < tmin))
		{
			sr.hit_an_object	= true;
			tmin 				= t;
			sr.material_ptr     = objects[j]->get_material();
			sr.hit_point 		= ray.o + t * ray.d;
			normal 				= sr.normal;
			local_hit_point	 	= sr.local_hit_point;
		}
  
	if(sr.hit_an_object)
	{
		sr.t = tmin;
		sr.normal = normal;
		sr.local_hit_point = local_hit_point;
	}
		
	return(sr);   
}


void
World::delete_cameras(void)
{
	int num_cameras = cameras.size();
	
	for (int j = 0; j < num_cameras; j++)
	{
		delete cameras[j];
		cameras[j] = NULL;
	}	
	cameras.erase (cameras.begin(), cameras.end());
}

//------------------------------------------------------------------ delete_objects

// Deletes the objects in the objects array, and erases the array.
// The objects array still exists, because it's an automatic variable, but it's empty 

void
World::delete_objects(void)
{
	int num_objects = objects.size();
	
	for (int j = 0; j < num_objects; j++)
	{
		delete objects[j];
		objects[j] = NULL;
	}	
	
	objects.erase (objects.begin(), objects.end());
}


//------------------------------------------------------------------ delete_lights

void
World::delete_lights(void)
{
	int num_lights = lights.size();
	
	for (int j = 0; j < num_lights; j++)
	{
		delete lights[j];
		lights[j] = NULL;
	}	
	
	lights.erase (lights.begin(), lights.end());
}


void 												
World::build(void)
{
	int num_samples = 16;
	vp.set_samples(num_samples);
	vp.set_pixel_size(.5);
	
	tracer_ptr = new RayCast(this);
	float a = 0.75;
	background_color = RGBColor(0.0, 0.3 * a, 0.25 * a);  // torquise
			
	Ambient* ambient_ptr = new Ambient;
	ambient_ptr->scale_radiance(0.5);
	set_ambient_light(ambient_ptr);
	
	Directional* light_ptr = new Directional;
	light_ptr->set_direction(15, 15, 2.5); 
	light_ptr->scale_radiance(6.0);	
	add_light(light_ptr);

	Matte* matte_ptr1 = new Matte;			
	matte_ptr1->set_ka(0.25);
	matte_ptr1->set_kd(0.75);
	matte_ptr1->set_cd(0.75, 0.75, 0);    	// dark yellow
	
	Matte* matte_ptr2 = new Matte;			
	matte_ptr2->set_ka(0.45); 
	matte_ptr2->set_kd(0.75);
	matte_ptr2->set_cd(0.75, 0.25, 0);  	 // orange
	
	Matte* matte_ptr3 = new Matte;			
	matte_ptr3->set_ka(0.4); 
	matte_ptr3->set_kd(0.75);
	matte_ptr3->set_cd(1, 0.5, 1);  		// mauve
	
	Matte* matte_ptr4 = new Matte;			
	matte_ptr4->set_ka(0.15);
	matte_ptr4->set_kd(0.5);
	matte_ptr4->set_cd(0.75, 1.0, 0.75);   	// light green
	
	Matte* matte_ptr5 = new Matte;			
	matte_ptr5->set_ka(0.20); 
	matte_ptr5->set_kd(0.97);	
	matte_ptr5->set_cd(0.25, 0.25, 1.0);  
	
	Pinhole* pinhole_ptr = new Pinhole;
	pinhole_ptr->set_eye(0, 0, 10);
	pinhole_ptr->set_lookat(0, 0, -10);
	pinhole_ptr->set_view_distance(5);
	pinhole_ptr->compute_uvw(); 
	add_camera(pinhole_ptr);

	FishEye* fisheye_ptr = new FishEye;
	fisheye_ptr->set_eye(0, 0, 5); 
	fisheye_ptr->set_lookat(0, 0, -2);  
	fisheye_ptr->compute_uvw(); 
	add_camera(fisheye_ptr);
		
    ThinLens *thin_len_ptr = new ThinLens;
    thin_len_ptr->set_sampler(new MultiJittered(num_samples));
    thin_len_ptr->set_eye(0, 0, 10);
    thin_len_ptr->set_lookat(0, 0, -10);
	thin_len_ptr->set_view_distance(5);
	thin_len_ptr->set_focal_distance(20);
	thin_len_ptr->set_lens_radius(1);
	thin_len_ptr->compute_uvw(); 
	add_camera(thin_len_ptr);

	set_camera(fisheye_ptr);

	// spheres
	Sphere* sphere_ptr1 = new Sphere(Point3D(10, 10, -15), 7);
	sphere_ptr1->set_material(matte_ptr1);
	add_object(sphere_ptr1);
	
	Sphere* sphere_ptr2 = new Sphere(Point3D(-10, 10, -20), 10);
	sphere_ptr2->set_material(matte_ptr2);     
	add_object(sphere_ptr2);

#if 0
	// cylinder
	float bottom 	= 0.0;
	float top 		= 8.5;   
	float radius	= 2.2;
	SolidCylinder* cylinder_ptr = new SolidCylinder(bottom, top, radius);
	cylinder_ptr->set_material(matte_ptr3);
	add_object(cylinder_ptr);
	
	// box
	Box* box_ptr = new Box(Point3D(-3.5, 0, -11), Point3D(-2.5, 6, 6.5));
	box_ptr->set_material(matte_ptr4);
	add_object(box_ptr);
#endif
	
	// ground plane
	Plane* plane_ptr = new Plane(Point3D(0), Normal(0, 1, 0));
	plane_ptr->set_material(matte_ptr5);
	add_object(plane_ptr);
}


