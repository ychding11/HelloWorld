#include "World.h"
#include "AAScene.h"

int main()
{
    AAScene scene;
    World &world = scene; 
    world.build();
    //world.render_scene();
    world.render_orthographic();
}
