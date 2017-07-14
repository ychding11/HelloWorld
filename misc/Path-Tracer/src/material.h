#ifndef MATERIAL_H
#define MATERIAL_H

#include "../lib/rand48/erand48.h"
//#include "vector.h"
//#include "ray.h"
#include "texture.h"
#include "geometry.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

enum MaterialType { DIFF, SPEC, EMIT };

class Material
{

private:
	MaterialType m_type;
	Vector3f m_colour;
	Vector3f m_emission;
    Texture  m_texture;

public:
	Material(MaterialType t = DIFF, const Vector3f &c = Vector3f(), const Vector3f &e = Vector3f(0, 0, 0), const Texture &tex = Texture())
		: m_type(t) , m_colour(c), m_emission(e), m_texture(tex) {}


	MaterialType getType() const { return m_type; }
	Vector3f getColour()  const { return m_colour; } 
	Vector3f get_emission() const { return m_emission; } 

	Vector3f get_colour_at(double u, double v) const;

};

#endif // MATERIAL_H
