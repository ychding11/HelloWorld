#include <math.h>
#include <iostream>
#include <stdio.h>

#include "objects.h"
#include "../lib/tiny_obj_loader/tiny_obj_loader.h"

#include "pathtracer.h"

// override
bool Sphere::getIntersection(const Ray &ray, SurfaceInteraction &intersection) const
{
	Vector3f oc = ray.o - mCenter;
	Float det = Dot(oc, ray.d) * Dot(oc, ray.d) - (Dot(oc, oc) - mRadius);
	if (det < 0.0) return false;
	det = sqrt(det);
	Float t, epslon = 1e-4, minusB = -1 * Dot(ray.d, oc);
	Float dist = (t = minusB - det) > epslon ? t : ((t = minusB + det) > epslon ?  t : 0.0);
	if (dist == 0.0) return false;
	Vector3f  normal = Normalize( (ray.o + ray.d * dist) - mCenter);
	intersection.hit = true;
	intersection.n = normal;
	intersection.u = dist;
	intersection.m = mMaterial;
	return true;
}
//constructor
// https://syoyo.github.io/tinyobjloader/
// https://en.wikipedia.org/wiki/Wavefront_.obj_file
Mesh::Mesh(Point3f center, const char* file_path, Material defaultMaterial)
	: Object(center, defaultMaterial)
{
	CHECK(file_path);

	std::vector<tinyobj::shape_t> m_shapes;
	std::vector<tinyobj::material_t> m_materials;
	std::vector<Material> materials;
    std::string inputfile(file_path);
    auto pos = inputfile.find_last_of("/");
    std::string mtlbasepath = inputfile.substr(0, pos+1);

	LOG(INFO) << "- Loading model from file: " << file_path;
	std::string err = tinyobj::LoadObj(m_shapes, m_materials, inputfile.c_str(), mtlbasepath.c_str());
	if (!err.empty())
    {
		std::cerr << err << std::endl;
		exit(1);
	}

    size_t shapes_size = m_shapes.size();
    size_t materials_size = m_materials.size();

	LOG(INFO) << "- Building material list...";
	LOG(INFO) << "- Material num " << materials_size;
    for (size_t i=0; i < materials_size; i++)
    {
        std::string texture_path = "";
        if (!m_materials[i].diffuse_texname.empty())
        {
            if (m_materials[i].diffuse_texname[0] == '/') texture_path = m_materials[i].diffuse_texname;
            texture_path = mtlbasepath + m_materials[i].diffuse_texname;
            materials.push_back( Material(DIFF, Vector3f(1,1,1), Vector3f(), texture_path.c_str()) );
        }
        else
        {
            materials.push_back( Material(DIFF, Vector3f(1,1,1), Vector3f()) );
        }
    }

	LOG(INFO) << "- Building triangle list...";
    for (size_t i = 0; i < shapes_size; i++)
    {
		CHECK( m_shapes[i].mesh.indices.size() % 3 == 0 );
        size_t triangles = m_shapes[i].mesh.indices.size() / 3;
		const auto &positions = m_shapes[i].mesh.positions;
		const auto &indices  = m_shapes[i].mesh.indices;
		const auto &texcoords = m_shapes[i].mesh.texcoords;
		DLOG(INFO) << "- Begin handle shape " << i << ", has triangle num " << triangles;
        for (size_t f = 0; f < triangles; f++)
        {
            // Triangle vertex coordinates
            Point3f v0_(positions[indices[3 * f] * 3 ],
					positions[indices[3 * f] * 3 + 1],
					positions[indices[3 * f] * 3 + 2] );

            Point3f v1_(positions[indices[3 * f + 1] * 3 ],
					positions[indices[3 * f + 1] * 3 + 1],
					positions[indices[3 * f + 1] * 3 + 2] );

            Point3f v2_(positions[indices[3 * f + 2] * 3 ],
					positions[indices[3 * f + 2] * 3 + 1],
					positions[indices[3 * f + 2] * 3 + 2] );
			v0_ += mCenter; v1_ += mCenter; v2_ += mCenter; // World position

            Point3f t0_, t1_, t2_;

            //Attempt to load triangle texture coordinates
            if (indices[3 * f + 2] * 2 + 1 < texcoords.size())
            {
                t0_ = Point3f(texcoords[indices[3 * f] * 2],
							texcoords[indices[3 * f] * 2 + 1], 0.);
                t1_ = Point3f(texcoords[indices[3 * f + 1] * 2],
						   texcoords[indices[3 * f + 1] * 2 + 1], 0.);
                t2_ = Point3f(texcoords[indices[3 * f + 2] * 2],
						   texcoords[indices[3 * f + 2] * 2 + 1], 0.);
            }

            if (m_shapes[i].mesh.material_ids[ f ] < materials.size())
                mTriangles.push_back(new Triangle(v0_, v1_, v2_, t0_, t1_, t2_, &materials[ m_shapes[i].mesh.material_ids[ f ] ]));
            else
                mTriangles.push_back(new Triangle(v0_, v1_, v2_, t0_, t1_, t2_, &mMaterial));
        }
    }

	LOG(INFO) << "- Total triangles " << mTriangles.size();

    // Clean up
    m_shapes.clear();
    m_materials.clear();
	materials.clear();
    
	printf(" - Building k-d tree...\n\n");
    //node = KDNode().build(tris, 0);
    printf("\n");
}

// override
bool Mesh::getIntersection(const Ray &ray, SurfaceInteraction &intersection) const
{
	Float t = 0, tmin = INFINITY;
	Vector3f normal, colour;
	bool hit = false;
	if (mKDtree) mKDtree->Intersect(ray, &intersection);
	else
	{
		for (auto a : mTriangles)
		{

		}

	}
	return hit;
}