#include <stdlib.h>

#include "geometry.h"
#include "material.h"
#include "pathtracer.h"

// Get colour at UV coordinates u,v
Vector3f Material::get_colour_at(double u, double v) const
{
    if (m_texture.is_loaded()) return m_texture.get_pixel(u, v);
    return m_colour;
}

