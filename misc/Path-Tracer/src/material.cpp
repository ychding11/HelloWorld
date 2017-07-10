#include <stdlib.h>

#include "vector.h"
#include "ray.h"
#include "material.h"
#include "pathtracer.h"

// Get colour at UV coordinates u,v
Vector3f Material::get_colour_at(double u, double v) const
{
    if (m_texture.is_loaded()) return m_texture.get_pixel(u, v);
    return m_colour;
}

Ray Material::get_reflected_ray(const Ray &r, Point3f &p, const Vector3f &n, unsigned short *Xi) const
{
	if (m_type == SPEC)
    {
        const double roughness = 0.8;
		Vector3f reflected = r.d - Dot( n, r.d) * 2 * n;
        reflected = Vector3f(
						 reflected.x + (erand48(Xi)-0.5)*roughness,
						 reflected.y + (erand48(Xi)-0.5)*roughness,
						 reflected.z + (erand48(Xi)-0.5)*roughness );

        return Ray(p, Normalize(reflected));
	}
	else if (m_type == DIFF)
    {
		Vector3f nl = Dot(n, r.d) < 0 ? -n : n;
		double r1 = 2 * M_PI * erand48(Xi),
			   r2 = erand48(Xi),
			   r2s = sqrt(r2);
        Vector3f w = nl;
		Vector3f u = Normalize(fabs(w.x) > .1 ? Vector3f(0, 1, 0) : Cross(Vector3f(1, 0, 0), w));
		Vector3f v = Cross(w, u);
        Vector3f d = Normalize(u*cos(r1)*r2s + v*sin(r1)*r2s + w*sqrt(1-r2));

        return Ray(p, d);
	}
	else
	{
		// invalid branch
		CHECK(m_type == SPEC || m_type == DIFF);
	}
}
