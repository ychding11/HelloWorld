#include "camera.h"
#include "stats.h"

//constructor
Camera::Camera(Point3f position, Point3f target, int width, int height)
	:mFilmWidth(width)
	,mFilmHeight(height)
	,mEyePosition(position)
{
	mDz = Normalize(target - mEyePosition);
	mDx = Normalize(Cross(mDz, Vector3f(0, 1, 0)));
	mDy = Normalize(Cross(mDx, mDz ));
	mFilmCenter = mEyePosition + mDz * 1.3;
}

STAT_COUNTER("Camera/rays", rays);

// Generate a ray from camera origin through pixel(x,y)
Ray Camera::get_ray(int x, int y, bool jitter, unsigned short *Xi)
{
    double xJitter = .0, yJitter = .0;
    if (jitter)
    {
		xJitter = erand48(Xi) / (double)mFilmHeight;
		yJitter = erand48(Xi) / (double)mFilmHeight;
    }
	const double aspect = (double)mFilmHeight / (double)mFilmWidth;
	const double W = (double)mFilmWidth / 2.0, H = (double)mFilmWidth / 2.0;

	double unitX = ((double)x + 0.5 - W) / W  + xJitter;
	double unitY = ((double)y + 0.5 * aspect - H) / H + yJitter;
	Point3f pixelPos = mFilmCenter + mDx * unitX + mDy * unitY;
	rays++;
    return Ray(mEyePosition, Normalize(pixelPos - mEyePosition));
}
