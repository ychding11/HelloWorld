#ifndef TEXTURE_H
#define TEXTURE_H
#include <vector>
#include "geometry.h"

class Texture {
private:
    unsigned width;
    unsigned height;
    bool loaded = false;
    std::vector<unsigned char> image;
public:
    Texture(const char* filename);
    Texture(){};
    Vector3f get_pixel(unsigned x, unsigned y) const;
    Vector3f get_pixel(double u, double v) const;
    bool is_loaded() const;
};

#endif // TEXTURE_H