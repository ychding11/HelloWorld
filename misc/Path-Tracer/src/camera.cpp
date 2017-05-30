#include "ray.h"
#include "camera.h"
#include "stats.h"

//constructor
Camera::Camera(Vec position, Vec target, int width, int height)
	:m_width(width)
	,m_height(height)
	,m_position(position)
{
    m_width_recp = 1./m_width;
    m_height_recp = 1./m_height;
    m_ratio = (double)m_width/m_height;

	// Construct camera space basis vector.
    m_direction = (target - m_position).norm();
    m_x_direction = Vec(0, 0, 1).cross(m_direction * -1).norm();
    m_y_direction = m_x_direction.cross(m_direction).norm();

    m_x_spacing = (2.0 * m_ratio)/(double)m_width;
    m_y_spacing = (double)2.0/(double)m_height;
    m_x_spacing_half = m_x_spacing * 0.5;
    m_y_spacing_half = m_y_spacing * 0.5;
}

STAT_COUNTER("Camera/rays", rays);

// Generate a ray from camera origin through pixel(x,y)
Ray Camera::get_ray(int x, int y, bool jitter, unsigned short *Xi)
{
    double x_jitter, y_jitter;

    // If jitter == true, jitter point for anti-aliasing
    if (jitter)
    {
        x_jitter = (erand48(Xi) * m_x_spacing) - m_x_spacing_half;
        y_jitter = (erand48(Xi) * m_y_spacing) - m_y_spacing_half;
    }
    else
    {
        x_jitter = 0;
        y_jitter = 0;
    }

    Vec pixel = m_position + m_direction * 2; // Film position is fixed.
    pixel = pixel - m_x_direction*m_ratio + m_x_direction*((x * 2 * m_ratio)*m_width_recp) + x_jitter;
    pixel = pixel + m_y_direction - m_y_direction*((y * 2.0)*m_height_recp + y_jitter);
	rays++;
    return Ray(m_position, (pixel-m_position).norm());
}
