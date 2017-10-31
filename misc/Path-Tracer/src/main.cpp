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
    int samples = 2;            // Default samples per pixel

    if (argc == 2) samples = atoi(argv[1]); // parse samples/pixel from command lines.

    Camera camera(Point3f(2.828,2.828, 0), Point3f(0,0,0), 32, 32);     // Create camera
    Scene scene;                                              // Create scene
#if 1
    scene.add(new Sphere(Point3f(0, 0, 0), 1, new Material(DIFF, Vector3f(0.85,0.4,0.4)))); //Left
    //scene.add(new Sphere(Point3f(-104, 0, 0), 100, new Material(DIFF, Vector3f(0.85,0.4,0.4)))); //Right
    //scene.add(new Sphere(Point3f(104, 0, 0), 100, new Material(DIFF, Vector3f(0.85,0.4,0.4)))); //Back
    //scene.add(new Sphere(Point3f(0, 106, 0), 100, new Material(DIFF))); //Front
    //scene.add(new Sphere(Point3f(0, 0, 11), 10, new Material(DIFF, Vector3f(0.85,0.4,0.4)))); //Bottom
//    scene.add(new Sphere(Point3f(50, -1e5 + 81.6, 81.6), 1e5, new Material(DIFF, Vector3f(0.85,0.4,0.4)))); //Top
 //   scene.add(new Sphere(Point3f(50, 681.6 - .27, 81.60), 600, new Material(EMIT, Vector3f(), Vector3f(12,12,12)))); //light source
  //  scene.add(new Sphere(Point3f(10, 3, 10), 1, new Material(EMIT, Vector3f(), Vector3f(12,12,12)))); //light source
  //  scene.add(new Mesh(Point3f(), "../models/dragon2.obj", new Material(DIFF, Vector3f(0.9, 0.9, 0.9))));
#endif

    Renderer renderer = Renderer(&scene, &camera);  // Create renderer with our scene and camera
    renderer.render(samples);                       // Render image to pixel buffer
    renderer.saveImage("dragon-in-box.png");        // Save image

    time(&stop);
    printDuration(start, stop);
    return 0;
}
