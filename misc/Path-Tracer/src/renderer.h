#ifndef RENDERER_H
#define RENDERER_H

#include "geometry.h"
#include "scene.h"
#include "camera.h"

class Renderer
{

private:
    Scene      *mScene;
    Camera     *mCamera;
    Vector3f   *mPixelBuffer;

public:
    Renderer(Scene *scene, Camera *camera);

    void render(int samples = 4);
    void saveImage(const char * file_path = "result.png");

private:
	Vector3f traceRay(const Ray &ray, int depth, unsigned short*Xi) const;
	Ray getReflectedRay(const Ray &r, const Point3f &p, const Vector3f &n, MaterialType type, unsigned short *Xi) const;
};

#endif //RENDERER_H
