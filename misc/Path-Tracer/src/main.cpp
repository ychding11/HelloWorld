/*
*  Simple monte carlo path tracer in C++
*
*  Compilation:
*      $ mkdir build
*      $ cd build
*      $ cmake ..
*      $ make
*
*  Usage:
*      $ ./pathtracer <number of samples>
*/

#include <stdio.h>
#include <stdlib.h>
#include "time.h"

#include "geometry.h"
#include "material.h"
#include "objects.h"
#include "camera.h"
#include "scene.h"
#include "renderer.h"

#include "pathtracer.h"

void printDuration(time_t start, time_t stop)
{
    double diff = difftime(stop, start);
    int hrs = (int)diff/3600;
    int mins = ((int)diff/60)-(hrs*60);
    int secs = (int)diff-(hrs*3600)-(mins*60);
    printf("- Rendering Complete!\tTime Taken: %i hrs, %i mins, %i secs.\n", hrs, mins, secs);
}

/*! \brief simple demo. construct scenes and render. */
int main(int argc, char *argv[])
{
	google::InitGoogleLogging(argv[0]);

    time_t start, stop;
    time(&start);               // Start execution timer
    int samples = 4;            // Default samples per pixel

    if (argc == 2) samples = atoi(argv[1]); // parse samples/pixel from command lines.

    Camera camera = Camera(Point3f(0, -5, 2.5), Point3f(0,0,1), 640, 360);     // Create camera
    Scene scene = Scene();                                              // Create scene
    scene.add( dynamic_cast<Object*>(new Sphere(Point3f(0,0,-1000), 1000, new Material())) );
    scene.add( dynamic_cast<Object*>(new Sphere(Point3f(-1004,0,0), 1000, new Material(DIFF, Vector3f(0.85,0.4,0.4)))) );
    scene.add( dynamic_cast<Object*>(new Sphere(Point3f(1004,0,0), 1000, new Material(DIFF, Vector3f(0.4,0.4,0.85)))) );
    scene.add( dynamic_cast<Object*>(new Sphere(Point3f(0,1006,0), 1000, new Material())) );
    scene.add( dynamic_cast<Object*>(new Sphere(Point3f(0,0,110), 100, new Material(EMIT, Vector3f(1,1,1), Vector3f(2.2,2.2,2.2)))) );
    scene.add( dynamic_cast<Object*>(new Mesh(Point3f(), "../models/dragon2.obj", new Material(DIFF, Vector3f(0.9, 0.9, 0.9)))) );

    Renderer renderer = Renderer(&scene, &camera);  // Create renderer with our scene and camera
    renderer.render(samples);                       // Render image to pixel buffer
    renderer.saveImage("dragon-in-box.png");              // Save image

    time(&stop);
    printDuration(start, stop);
    return 0;
}
