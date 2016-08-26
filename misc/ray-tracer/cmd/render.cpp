#include "Camera.h"
#include "World.h"

int main()
{
    World world;
    world.build();
    //world.render_scene();
    world.render_orthographic();
}
