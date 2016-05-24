#ifndef RAY_H
#define RAY_H

#include "vector.h"

struct Ray
{
    Vec origin, direction, direction_inv;

    Ray(Vec orig, Vec dir)
    : origin(orig), direction(dir)
    {
        direction_inv = Vec( 1./direction.x, 1./direction.y, 1./direction.z);
    }
};

#endif // RAY_H
