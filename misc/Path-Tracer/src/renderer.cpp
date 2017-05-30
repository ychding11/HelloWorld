
#include <vector>
#include <stdio.h>
#include <iostream>

#include "renderer.h"
#include "../lib/lodepng/lodepng.h"
#include "parallel.h"
#include "stats.h"
#include "progressreporter.h"

#define MULTI_THREAD

// Clamp double to min/max of 0/1
inline double clamp(double x){ return x<0 ? 0 : x>1 ? 1 : x; }
// Clamp to between 0-255
inline int toInt(double x){ return int(clamp(x)*255+.5); }

Renderer::Renderer(Scene *scene, Camera *camera)
: m_scene(scene), m_camera(camera)
{
    try
    {
        m_pixel_buffer = new Vec[m_camera->get_width()*m_camera->get_height()];
    }
    catch(...)
    {
        throw;
    }
}

void Renderer::render(int samples)
{
    int width  = m_camera->get_width();
    int height = m_camera->get_height();
    double invSamples = 1./samples;

#ifndef MULTI_THREAD
    // Main Loop
    #pragma omp parallel for schedule(dynamic, 1)       // OpenMP
    for (int y = 0; y < height; y++)
    {
        unsigned short Xi[3] = {0, 0, y*y*y};   // Stores seed for erand48

        fprintf(stderr, "\rRendering (%i samples): %.2f%% ",  samples, (double)y/height*100);  // report progress

        for (int x = 0; x < width; x++)
        {
            Vec color = Vec();
            for (int a = 0; a < samples; a++)
            {
                Ray ray = m_camera->get_ray(x, y, a > 0, Xi);
                color = color + m_scene->trace_ray(ray,0,Xi);
            }
            m_pixel_buffer[(y)*width + x] = color * invSamples;
        }
    }
#else
    parallelInit();
    ProgressReporter reporter(width * height, "Rendering");
    ParallelFor([&](int64_t y) {
        unsigned short Xi[3] = { 0, 0, y*y*y };
         for (int x = 0; x < width; x++)
         {
            Vec color = Vec();
            for (int a = 0; a < samples; a++)
            {
                Ray ray = m_camera->get_ray(x, y, a > 0, Xi);
                color = color + m_scene->trace_ray(ray,0,Xi);
            }
            m_pixel_buffer[y * width + x] = color * invSamples;
            reporter.update();
         }
    }, height, 32);
    reporter.done();
	MergeWorkerThreadStats();
	ReportThreadStats();
	PrintStats(stdout);
	ClearStats();
    parallelCleanup();
#endif
}

void Renderer::save_image(const char *file_path)
{
    int width  = m_camera->get_width();
    int height = m_camera->get_height();
    int pixel_count = width*height;
    std::vector<unsigned char> pixel_buffer;

    for (int i=0; i<pixel_count; i++)
    {
        pixel_buffer.push_back(toInt(m_pixel_buffer[i].x));
        pixel_buffer.push_back(toInt(m_pixel_buffer[i].y));
        pixel_buffer.push_back(toInt(m_pixel_buffer[i].z));
        pixel_buffer.push_back(255);
    }

    //Encode the image
    unsigned error = lodepng::encode(file_path, pixel_buffer, width, height);
    if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;

    pixel_buffer.clear();
}
