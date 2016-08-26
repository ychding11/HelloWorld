#include "World.h"
#include "AAScene.h"

int main()
{
    AAScene scene;
    World &world = scene; 
    world.build();
    scene.set_pixel_samples(4);
    //world.render_scene();
    world.render_orthographic();
}
