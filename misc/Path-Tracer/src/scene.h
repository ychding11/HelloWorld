#ifndef SCENE_H
#define SCENE_H

#include "geometry.h"
#include "objects.h"

class Scene
{
private:
    std::vector<Object*> mObjects;
	Vector3f mBackground;

public:
    Scene() :mBackground(0,0,0) {};

    void add(Object *object);
	SurfaceInteraction intersect(const Ray &ray);
    Vector3f trace_ray(const Ray &ray, int depth, unsigned short*Xi);
};

#endif //SCENE_H
