#include "camera.h"
#include "stats.h"

//constructor
Camera::Camera(Point3f position, Point3f target, int width, int height)
	:m_width(width)
	,m_height(height)
	,m_position(position)
{
    m_width_inv = 1./m_width;
    m_height_inv = 1./m_height;
    m_ratio = (double)m_width/m_height;
	
	// Construct camera space basis vector.
    m_direction = Normalize(target - m_position);

	m_x_direction = Normalize(Cross( Vector3f(0, 0, 1), (m_direction * -1.0)));
	m_y_direction = Normalize(Cross(m_x_direction, m_direction));

	m_x_spacing = 1.0 * m_width_inv * m_ratio;
	m_y_spacing = 1.0 * m_height_inv;
    m_x_spacing_half = m_x_spacing * 0.5;
    m_y_spacing_half = m_y_spacing * 0.5;
}

STAT_COUNTER("Camera/rays", rays);

// Generate a ray from camera origin through pixel(x,y)
Ray Camera::get_ray(int x, int y, bool jitter, unsigned short *Xi)
{
    double x_jitter = .0, y_jitter = .0;

    // If jitter == true, jitter point for anti-aliasing
    if (jitter)
    {
        x_jitter = (erand48(Xi) * m_x_spacing) - m_x_spacing_half;
        y_jitter = (erand48(Xi) * m_y_spacing) - m_y_spacing_half;
    }

    Point3f pixel = m_position + m_direction * 2; // Film position is fixed.
	double unitY = (y + .5) * 2.0 * m_height_inv - 1.0 + y_jitter;
	double unitX = (x + .5) * 2.0 * m_width_inv * m_ratio - 1.0 + x_jitter;
	pixel = pixel + m_x_direction * unitX + m_y_direction * unitY;
	rays++;
    return Ray(m_position, Normalize(pixel - m_position));
}
