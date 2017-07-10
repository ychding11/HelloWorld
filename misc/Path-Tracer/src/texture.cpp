#include <iostream>
#include <stdio.h>
#include <cmath>
#include <stdexcept>

#include "texture.h"
#include "../lib/lodepng/lodepng.h"
#include "geometry.h"

// constructor
Texture::Texture(const char *filename)
{
    image = std::vector<unsigned char>();
    // try catch to ceck input.
    unsigned error = lodepng::decode(image, width, height, filename);
    //if there's an error, display it
    printf("Loading texture: %s\n", filename);
    if(error)
    {
        std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl << std::endl;
        return;
    }
    loaded = true;
}

// Get pixel at texture coordinates x,y
Vector3f Texture::get_pixel(unsigned x, unsigned y) const
{

    if (!loaded) return (Vector3f(1,0,1));

    double r, g, b;
    r = (double)image.at(y*width*4 + x    )/255.;
    g = (double)image.at(y*width*4 + x + 1)/255.;
    b = (double)image.at(y*width*4 + x + 2)/255.;
    return Vector3f(r, g, b);
}

// Get pixel at normalised texture coordinates u,v
Vector3f Texture::get_pixel(double u, double v) const
{
    if (!loaded)
        return (Vector3f(1,0,1));

    int x = (fmod(fabs(u), 1.0)) * (width-1);
    int y = (1.-fmod(fabs(v), 1.0)) * (height-1);
    //printf("%f, %f\n", u, v);
    double r, g, b;
    try
	{
        r = (double)image.at(y*width*4 + x*4    )/255.;
        g = (double)image.at(y*width*4 + x*4 + 1)/255.;
        b = (double)image.at(y*width*4 + x*4 + 2)/255.;
        return Vector3f(r, g, b);
    }
    catch (const std::out_of_range& e)
	{
        printf("error with uv, yx: %lf, %lf - %i, %i (width, height: %i, %i) \n", u, v, x, y, width, height);
        return Vector3f(0,1,0);
    }
}

// Check if texture is loaded
bool Texture::is_loaded() const
{
    return loaded;
}
