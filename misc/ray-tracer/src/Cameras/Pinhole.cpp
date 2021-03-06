// This file contains the definition of the Pinhole class

#include "Constants.h" 
#include "Point3D.h"
#include "Vector3D.h"
#include "Pinhole.h"
#include <math.h>
#include <stdio.h>

// ----------------------------------------------------------------------------- default constructor
Pinhole::Pinhole(void)		
	:	Camera(),
		d(500),
		zoom(1.0)
{}

// ----------------------------------------------------------------------------- copy constructor
Pinhole::Pinhole(const Pinhole& c)   		
	: 	Camera(c),
		d(c.d),
		zoom(c.zoom)
{}

// ----------------------------------------------------------------------------- clone
Camera* 
Pinhole::clone(void) const
{
	return (new Pinhole(*this));
}

// ----------------------------------------------------------------------------- assignment operator
Pinhole& 
Pinhole::operator= (const Pinhole& rhs)
{
	if (this == &rhs) return (*this);
		
	Camera::operator= (rhs);

	d 		= rhs.d;
	zoom	= rhs.zoom;

	return (*this);
}

// ----------------------------------------------------------------------------- destructor
Pinhole::~Pinhole(void) {}	

// ----------------------------------------------------------------------------- get_direction
Vector3D
Pinhole::get_direction(const Point2D& p) const
{
	Vector3D dir = p.x * u + p.y * v - d * w;
	dir.normalize();
	
	return(dir);
}

// ----------------------------------------------------------------------------- render_scene
void 												
Pinhole::render_scene(const World& w)
{
	RGBColor	L;
	ViewPlane	vp(w.vp);	 								
	Ray			ray;
	int 		depth = 0;  
	Point2D 	pp;		// sample point on a pixel
	int n = (int)sqrt((float)vp.num_samples);
		
	vp.s /= zoom;
	ray.o = eye;
    
    fprintf(stderr, "\r- Pinhole Camera. w=%d, h=%d, n=%d\n", vp.hres, vp.vres, vp.num_samples);
	for (int r = 0; r < vp.vres; r++)
    {
		fprintf(stderr, "\r- Rendering... %lf%%.", 100. * float(r) /  float(vp.vres - 1));
		for (int c = 0; c < vp.hres; c++)
        {
			L = black; 
			for (int p = 0; p < n; p++)
            {
				for (int q = 0; q < n; q++)
                {
					pp.x = vp.s * (c - 0.5 * vp.hres + (q + 0.5) / n); 
					pp.y = vp.s * (r - 0.5 * vp.vres + (p + 0.5) / n);
					ray.d = get_direction(pp);
					L += w.tracer_ptr->trace_ray(ray, depth);
				}	
			}								
			L /= vp.num_samples;
			L *= exposure_time;
            vp.write_to_buffer(vp.vres - r - 1, c, L);
		} 
    }
	fprintf(stderr, "\n- Rendering OK.\n");
    vp.save_to_ppm("pinhole.ppm");
}
