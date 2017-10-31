#include <vector>
#include <cstdio>
#include <iostream>

#include "../lib/lodepng/lodepng.h"
#include "renderer.h"
#include "parallel.h"
#include "stats.h"
#include "pathtracer.h"
#include "progressreporter.h"


//#define MULTI_THREAD

// Clamp double to min/max of 0/1
inline double clamp(double x){ return x<0 ? 0 : x>1 ? 1 : x; }
// Clamp to between 0-255
inline int toInt(double x){ return int(clamp(x)*255+.5); }

Renderer::Renderer(Scene *scene, Camera *camera)
: mScene(scene)
, mCamera(camera)
{
    try
    {
		CHECK(mScene && mCamera);
		int size = mCamera->get_width() * mCamera->get_height();
		CHECK(size > 0);
        mPixelBuffer = new Vector3f[size];
    }
    catch(...)
    {
        throw;
    }
}

void Renderer::render(int samples)
{
    int width  = mCamera->get_width();
    int height = mCamera->get_height();
    double invSamples = 1./samples;

    // Main Loop
#ifndef MULTI_THREAD
	// Single thread for debug only
	//memset();
	for (int a = 0; a < samples; a++)
	{
        #pragma omp parallel for schedule(dynamic, 1)       // OpenMP
		for (int y = 0; y < height; y++)
		{
			unsigned short Xi[3] = { 0, 0, y*y*y };   // Stores seed for erand48

			fprintf(stderr, "\rRendering (%i samples): %d, %.2f%% ", samples, a, (double)y / height * 100);  // report progress

			for (int x = 0; x < width; x++)
			{
				Ray ray = mCamera->get_ray(x, y, a < 0, Xi);
				Vector3f color =  traceRay(ray, 0, Xi);
				mPixelBuffer[y * width + x] += color * invSamples;
			}
		}
		char filename[64];
		sprintf(filename, "reulst-%d.png", a);
		saveImage(filename);
	}
#else
    parallelInit();
{
    ProgressReporter reporter(width * height, "Rendering");
    ParallelFor([&](int64_t y) {
        unsigned short Xi[3] = { 0, 0, y*y*y };
         for (int x = 0; x < width; x++)
         {
            Vector3f color;
            for (int a = 0; a < samples; ++a)
            {
                Ray ray = mCamera->get_ray(x, y, a > 0, Xi);
                color = color + traceRay(ray,0,Xi);
            }
            mPixelBuffer[y * width + x] = color * invSamples;
            reporter.update();
         }
    }, height, 32);
    reporter.done();
}
	MergeWorkerThreadStats();
	ReportThreadStats();
	PrintStats(stdout);
	ClearStats();
    parallelCleanup();
#endif
}

void Renderer::saveImage(const char *filepath)
{
    int width  = mCamera->get_width();
    int height = mCamera->get_height();
    int pixel_count = width * height;
    std::vector<unsigned char> pixel_buffer;

    for (int i = 0; i < pixel_count; i++)
    {
        pixel_buffer.push_back(toInt(mPixelBuffer[i].x));
        pixel_buffer.push_back(toInt(mPixelBuffer[i].y));
        pixel_buffer.push_back(toInt(mPixelBuffer[i].z));
        pixel_buffer.push_back(255);
    }

    //Encode the image
	CHECK(filepath);
    unsigned error = lodepng::encode(filepath, pixel_buffer, width, height);
    if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
} 

#if 0
Vector3f Renderer::traceRay(const Ray &ray, int depth, unsigned short *Xi) const
{

	SurfaceInteraction isct = mScene->intersect(ray);
	if (!isct.hit) return mScene->mBackground;
	CHECK(isct.m);
	// light source, termination
	if (isct.m->getType() == EMIT) return isct.m->get_emission();

	Vector3f colour = isct.m->getColour();
	Float p = std::max(std::max(colour.x, colour.y), colour.z);

	// Russian roulette termination.
	// If random number between 0 and 1 is > p, terminate and return hit object's emmission
	if (depth > 3)
	{
		double rnd = erand48(Xi);
		if (rnd < p * 0.9 && depth < 5)
		{ 
			colour = colour * (0.9 / p);
		}
		else
		{
			//std::cout << "ray bouns.\n";
			return isct.m->get_emission();
		}
	}

	Point3f isctPoint = ray.o + ray.d * isct.u;
	Ray reflected = getReflectedRay(ray, isctPoint, isct.n, isct.m->getType(), Xi);
	Vector3f colourReflect = traceRay(reflected, depth + 1, Xi);
	Vector3f result = colour * colourReflect;
	return result;
}
#endif

Ray Renderer::getReflectedRay(const Ray &r, const Point3f &p, const Vector3f &n, MaterialType type, unsigned short *Xi) const
{
	if (type == SPEC)
    {
        const double roughness = 0.8;
		Vector3f reflected = r.d - 2 * Dot( n, r.d) * n;
        reflected = Vector3f(
						 reflected.x + (erand48(Xi)-0.5)*roughness,
						 reflected.y + (erand48(Xi)-0.5)*roughness,
						 reflected.z + (erand48(Xi)-0.5)*roughness );

        return Ray(p, Normalize(reflected));
	}
	else if (type == DIFF)
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
		CHECK(type == SPEC || type == DIFF);
	}
}

Vector3f Renderer::traceRay(const Ray &ray, int depth, unsigned short *Xi) const
{

	SurfaceInteraction isct = mScene->intersect(ray);
	if (!isct.hit) return mScene->mBackground;
	// light source, termination
	if (isct.m->getType() == EMIT) return isct.m->get_emission();

	Vector3f colour = isct.getColour();
	return colour;
}

Vector3f Renderer::castRay(const Ray &ray) const
{
	SurfaceInteraction isct = mScene->intersect(ray);
	if (!isct.hit) return mScene->mBackground;
	// light source, termination
	if (isct.m->getType() == EMIT) return isct.m->get_emission();

	Vector3f colour = isct.getColour();
	return colour;
}
