
#include "AAScene.h"
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

// sampler
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

//------------------------------------------------------------------ render_orthographic
void 												
AAScene::render_orthographic(void)
{

	RGBColor	pixel_color;	 	
	Ray			ray;					
	int 		hres 	= vp.hres;
	int 		vres 	= vp.vres;
	float		s		= vp.s;
	float		zw		= 200.0;

	ray.d = Vector3D(0, 0, -1);
    fprintf(stdout, "- Render parameter. w=%d h=%d samples=%d.\n", vp.hres, vp.vres, vp.num_samples);	
	for (int r = 0; r < vres; r++)
    {
        fprintf(stdout, "\r - Orthographic Camera Rendering... %.2f%%.", 100. * float(r) / float(vres - 1));
		for (int c = 0; c < hres; c++)
        {
			RGBColor L = black;
			for (int j = 0; j < vp.num_samples; j++)
            {
				Point2D pp, sp = vp.sampler_ptr->sample_unit_square();
				pp.x = s * (c - 0.5 * hres + sp.x);
				pp.y = s * (r - 0.5 * vres + sp.y);
			    ray.o = Point3D(pp.x, pp.y, zw);

				L += tracer_ptr->trace_ray(ray);
			}

			L /= vp.num_samples;
            vp.write_to_buffer(vres - r - 1, c, L);
		}
    }
    fprintf(stdout, "\n- Orthographic Camera Rendering OK.\n");
    vp.save_to_ppm("orthographic.ppm");
}  

void
AAScene::set_pixel_samples(int num)
{
    vp.set_samples(num);
}

void 												
AAScene::build(void)
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
	set_camera(pinhole_ptr);

	FishEye* fisheye_ptr = new FishEye;
	fisheye_ptr->set_eye(0, 0, 5); 
	fisheye_ptr->set_lookat(0, 0, -2);  
	fisheye_ptr->compute_uvw(); 
	//set_camera(fisheye_ptr);
		
    ThinLens *thin_len_ptr = new ThinLens;
    thin_len_ptr->set_sampler(new MultiJittered(num_samples));
    thin_len_ptr->set_eye(0, 0, 10);
    thin_len_ptr->set_lookat(0, 0, -10);
	thin_len_ptr->set_view_distance(5);
	thin_len_ptr->set_focal_distance(20);
	thin_len_ptr->set_lens_radius(1);
	thin_len_ptr->compute_uvw(); 
	set_camera(thin_len_ptr);

	// spheres
	Sphere* sphere_ptr1 = new Sphere(Point3D(10, 10, -15), 7);
	sphere_ptr1->set_material(matte_ptr1);
	add_object(sphere_ptr1);
	
	Sphere* sphere_ptr2 = new Sphere(Point3D(-10, 10, -20), 10);
	sphere_ptr2->set_material(matte_ptr2);     
	add_object(sphere_ptr2);
	
	// ground plane
	Plane* plane_ptr = new Plane(Point3D(0), Normal(0, 1, 0));
	plane_ptr->set_material(matte_ptr5);
	add_object(plane_ptr);
}

