#include "World.h"
#include "AAScene.h"
#include <cstdlib>


void testCamera(int num)
{
    World world;
    world.build();
    world.set_pixel_samples(num);
    world.render_scene();
}

void testAA(int num)
{
    AAScene scene;
    World &world = scene; 
    world.build();
    world.set_pixel_samples(num);
    world.render_orthographic();
}

int main(int argc, char **argv)
{
    int samples = 4;
    if (argc == 2)
    {
        int temp = atoi(argv[1]);
        if (temp > 0) samples = temp;
    }

    //testAA(samples);
    testCamera(samples);
}
