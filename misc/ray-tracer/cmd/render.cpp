#include "World.h"
#include "AAScene.h"
#include <cstdlib>


int main(int argc, char **argv)
{
    int samples = 4;
    if (argc == 2)
    {
        int temp = atoi(argv[1]);
        if (temp > 0) samples = temp;
    }

    AAScene scene;
    World &world = scene; 
    world.build();
    world.set_pixel_samples(samples);
    world.render_scene();
    //world.render_orthographic();
}
