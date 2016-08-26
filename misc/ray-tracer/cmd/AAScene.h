#ifndef __AASCENE__
#define __AASCENE__

#include "ViewPlane.h"
#include "RGBColor.h"
#include "Tracer.h"
#include "GeometricObject.h"
#include "Sphere.h"
#include "Ray.h"
#include "SingleSphere.h"

#include "Camera.h"
#include "Light.h"
#include "Ambient.h"
#include "World.h"

#include <vector>

using namespace std;


class AAScene : public World
{	
	public:
	
		AAScene(void)
        { }												
		
		~AAScene()
        { }
								
		void 					
		build(void) override;

        void 												
        render_orthographic(void);

        void
        set_pixel_samples(int num);
};

#endif
