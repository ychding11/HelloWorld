#ifndef CAMERA_H
#define CAMERA_H

#include "../lib/rand48/erand48.h"
#include "geometry.h"

class Camera
{

private:
    int m_width;
    int m_height;
    double m_width_inv;
    double m_height_inv;
    double m_ratio;
    double m_x_spacing;
    double m_x_spacing_half;
    double m_y_spacing;
    double m_y_spacing_half;
    Point3f  m_position;
    Vector3f m_direction;
    Vector3f m_x_direction;
    Vector3f m_y_direction;

public:
    Camera(Point3f position, Point3f target, int width, int height);

	// How about copy constructor and assign operator?
	// The default one is sufficient? 

    int get_width() const { return m_width; }
    int get_height() const { return m_height; }

    Ray get_ray(int x, int y, bool jitter, unsigned short *Xi);
};

#endif //CAMERA_H
