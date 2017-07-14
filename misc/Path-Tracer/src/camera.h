#ifndef CAMERA_H
#define CAMERA_H

#include "../lib/rand48/erand48.h"
#include "geometry.h"

class Camera
{

private:
    int mFilmWidth;
    int mFilmHeight;
    Point3f  mEyePosition;
    Vector3f mDx;
	Vector3f mDy;
    Vector3f mDz;
    Point3f mFilmCenter; // Film position
public:
    Camera(Point3f position, Point3f target, int width, int height);

	// How about copy constructor and assign operator?
	// The default one is sufficient? 

    int get_width() const  { return mFilmWidth; }
    int get_height() const { return mFilmHeight; }

    Ray get_ray(int x, int y, bool jitter, unsigned short *Xi);
};

#endif //CAMERA_H
