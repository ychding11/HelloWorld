#include "Pinhole.h"
#include "FishEye.h"
#include "Camera.h"
#include "World.h"

int main()
{
    World world;
    world.build();
    world.render_scene();
    //Pinhole camera;
    //FishEye camera;
    //Camera *camera_ptr = world.get_camera();
    //camera_ptr->render_scene(world);
}
