#include "scene.h"
#include "objects.h"
#include "pathtracer.h"

void Scene::add(Object *object)
{
    mObjects.push_back( object ); 
}

SurfaceInteraction Scene::intersect(const Ray &ray)
{
	SurfaceInteraction isct;
    int n = mObjects.size();
    for (int i = 0; i < n; i++)
    {
		SurfaceInteraction temp;
        mObjects[i]->getIntersection(ray, temp);
        if (temp.hit && (isct.u == 0.0 || temp.u < isct.u)) isct = temp;
    }
    return isct;
}

Vector3f Scene::trace_ray(const Ray &ray, int depth, unsigned short *Xi)
{

	SurfaceInteraction isct = intersect(ray);
	if (!isct.hit) return mBackground;
    /*if (!isct.hit){
        double u, v;
        v = (acos(Vec(0,0,1).dot(ray.direction))/M_PI);
        u = (acos(ray.direction.y)/ M_PI);
        return bg.get_pixel(fabs(u), fabs(v))*1.2;
    }*/

	// light source, termination
    if (isct.m.get_type() == EMIT) return isct.m.get_emission();

    Vector3f colour = isct.m.get_colour();
    //return colour * isct.n.dot((Vec(1,-3,8)-x).norm());

    // Calculate max reflection
    //double p = colour.x > colour.y && colour.x > colour.z ? colour.x : colour.y>colour.z ? colour.y : colour.z;
    Float p = std::max(std::max(colour.x, colour.y), colour.z); 

    // Russian roulette termination.
    // If random number between 0 and 1 is > p, terminate and return hit object's emmission
    double rnd = erand48(Xi);
    if (++depth > 5)
    {
        if (rnd < p*0.9)
        { // Multiply by 0.9 to avoid infinite loop with colours of 1.0
            colour=colour*(0.9/p);
        }
        else
        {
            return isct.m.get_emission();
        }
    }

    Point3f isctPoint = ray.o + ray.d * isct.u;
    Ray reflected = isct.m.get_reflected_ray(ray, isctPoint, isct.n, Xi);

    return colour * trace_ray(reflected, depth, Xi);
}
