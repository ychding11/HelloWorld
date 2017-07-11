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
