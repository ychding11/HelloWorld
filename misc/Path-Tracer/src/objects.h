#ifndef OBJECTS_H
#define OBJECTS_H

#include "material.h"
#include "geometry.h"
#include "kdtreeaccel.h"

#include <algorithm>

struct SurfaceInteraction
{
	bool hit;	// If there was an intersection
	Float u;	// Distance to intersection along ray
	Vector3f n;		// Normal of intersected face
	Material m;	// Material of intersected face

	SurfaceInteraction(bool hit = false, Float u = 0.0, Vector3f n= Vector3f(), Material m = Material())
    : hit(hit) , u(u) , n(n) , m(m) {}
};

class Primitive
{
public:
	// Primitive Interface
	virtual ~Primitive() {}

	virtual Bounds3f WorldBound() const = 0;
	virtual bool Intersect(const Ray &r, SurfaceInteraction *) const = 0;
	virtual bool IntersectP(const Ray &r) const = 0;
	virtual const Material *GetMaterial() const = 0;
};

class Triangle : public Primitive
{
public:
	Point3f v0, v1, v2;     // Vertex world space coordinates
	Point3f t0, t1, t2;  // Triangle normal and texture coordinates
	Vector3f e1, e2, n;         // Edge 1 and edge 2 of triangle
	Material *m;        // Pointer to material

	Triangle(Point3f v0_, Point3f v1_, Point3f v2_,
		     Point3f t0_ = Point3f(), Point3f t1_ = Point3f(), Point3f t2_ = Point3f(), Material *m_ = NULL)
		:v0(v0_), v1(v1_), v2(v2_)
		,t0(t0_), t1(t1_), t2(t2_)
	{
		e1 = v1 - v0; e2 = v2 - v0;
		n = Normalize(Cross(e1, e2));
		m = m_;
	}

	Bounds3f WorldBound() const override
	{
		Point3f  pl = Point3f(
			std::min(std::min(v0.x, v1.x), v2.x),
			std::min(std::min(v0.y, v1.y), v2.y),
			std::min(std::min(v0.z, v1.z), v2.z));
		Point3f  p2 = Point3f(
			std::max(std::max(v0.x, v1.x), v2.x),
			std::max(std::max(v0.y, v1.y), v2.y),
			std::max(std::max(v0.z, v1.z), v2.z));

		return Bounds3f(pl, p2);
	}
	virtual bool Intersect(const Ray &r, SurfaceInteraction * intersect) const override
	{

		return false;
	}
	bool IntersectP(const Ray &r) const override
	{
		return false;
	}
	const Material *GetMaterial() const override
	{
		return m;
	}

	// Returns axis aligned bounding box that contains the triangle
	//AABBox get_bounding_box() const
	void get_bounding_box() const
	{
	}

	// Returns the midpoint of the triangle
	//Vec get_midpoint() const
	Point3f get_midpoint() const
	{
		return (v0 + v1 + v2) / 3;
	}

	// Checks if ray intersects with triangle. Returns true/false.
	bool intersect(Ray ray, double &t, double tmin, Vector3f &norm) const
	{
		Float u, v, t_temp = 0.0;
		Vector3f pvec = Cross(ray.d, e2);
		Float det = Dot(e1, pvec);
		if (det == 0) return false;
		Float invDet = 1. / det;
		Vector3f tvec = ray.o - v0;
		u = Dot(tvec, pvec) * invDet;
		if (u < 0 || u > 1) return false;
		Vector3f qvec = Cross(tvec, e1);
		v = Dot(ray.d, qvec)  * invDet;
		if (v < 0 || u + v > 1) return false;
		t_temp = Dot(e2, qvec) * invDet; // Set distance along ray to intersection
		if (t_temp < tmin)
		{
			if (t_temp > 1e-9)  // Fairly arbritarily small value, scared to change
			{
				t = t_temp;         // it as it works.
				norm = n;
				return true;
			}
		}
		return false;
	}

	// Returns barycentric coordinates of point p on the triangle
	Point3f barycentric(Point3f p) const
	{
		Vector3f v2_ = p - v0;
		double d00 = Dot(e1, e1);
		double d01 = Dot(e1, e2);
		double d11 = Dot(e2, e2);
		double d20 = Dot(v2_, e1);
		double d21 = Dot(v2_, e2);
		double d = d00*d11 - d01*d01;
		double v = (d11*d20 - d01*d21) / d;
		double w = (d00*d21 - d01*d20) / d;
		double u = 1 - v - w;
		return Point3f(u, v, w);
	}

	// Returns the colour at point p on the triangle
	Vector3f get_colour_at(Point3f p) const
	{
		if (m == NULL) return Vector3f(1, 0, 1);

		Point3f b = barycentric(p);
		Point3f c;
		c = c + (t0 * b.x);
		c = c + (t1 * b.y);
		c = c + (t2 * b.z);

		return m->get_colour_at(c.x, c.y);
	}
};

class Object
{
protected:
	Point3f mCenter; // World Position
	Material mMaterial;	// Material
public:
	explicit Object(Point3f center) : mCenter(center), mMaterial(){ }
	Object(Point3f center, Material material) : mCenter(center), mMaterial(material){ }
	Object() : mCenter(0, 0, 0), mMaterial() { }

	virtual bool getIntersection(const Ray &r, SurfaceInteraction &intersection) const = 0;
	Material getMaterial() const { return mMaterial; }
};

class Sphere : public Object
{
private:
	Float mRadius;	// Radius

public:
	Sphere(Point3f p, double radius, Material m)	
    : mRadius(radius) , Object(p, m) { }

    Float getRadius() const { return mRadius; }

	bool getIntersection(const Ray &r, SurfaceInteraction &intersection) const override;
};

class Mesh : public Object
{
private:
	
    std::vector<Triangle*> mTriangles;
	KdTreeAccel *mKDtree = nullptr;

public:
	Mesh(Point3f p_, const char* file_path, Material m_);

	bool getIntersection(const Ray &r, SurfaceInteraction &intersection) const  override;
};



#endif // OBJECTS_H
