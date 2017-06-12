#include <math.h>
#include <iostream>
#include <stdio.h>

#include "vector.h"
#include "ray.h"
#include "material.h"
#include "objects.h"
//#include "../lib/fastbvh/BVH.h"

#include "pathtracer.h"

// Check if ray intersects with sphere. Returns ObjectIntersection data structure
ObjectIntersection Sphere::get_intersection(const Ray &ray)
{
	// Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
	bool hit = false;
	double distance = 0;
	Vec n = Vec();

	Vec op = m_p - ray.origin;
	double t, eps=1e-4, b=op.dot(ray.direction), det=b*b-op.dot(op)+m_r*m_r;
	if (det<0) return ObjectIntersection(hit, distance, n, m_m); 
	else det=sqrt(det);
	distance = (t=b-det)>eps ? t : ((t=b+det)>eps ? t : 0);
	if (distance != 0)
		hit = true, 
		n = ((ray.origin + ray.direction * distance) - m_p).norm();

	return ObjectIntersection(hit, distance, n, m_m);
}

//constructor
// https://syoyo.github.io/tinyobjloader/
// https://en.wikipedia.org/wiki/Wavefront_.obj_file
Mesh::Mesh(Vec p_, const char* file_path, Material m_)
{
	CHECK(file_path);
	m_p=p_, m_m=m_;

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
            materials.push_back( Material(DIFF, Vec(1,1,1), Vec(), texture_path.c_str()) );
        }
        else
        {
            materials.push_back( Material(DIFF, Vec(1,1,1), Vec()) );
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
            Vec v0_(positions[indices[3 * f] * 3 ],
					positions[indices[3 * f] * 3 + 1],
					positions[indices[3 * f] * 3 + 2] );

            Vec v1_(positions[indices[3 * f + 1] * 3 ],
					positions[indices[3 * f + 1] * 3 + 1],
					positions[indices[3 * f + 1] * 3 + 2] );

            Vec v2_(positions[indices[3 * f + 2] * 3 ],
					positions[indices[3 * f + 2] * 3 + 1],
					positions[indices[3 * f + 2] * 3 + 2] );
			v0_ += m_p, v1_ += m_p, v2_ += m_p;

            Vec t0_, t1_, t2_;

            //Attempt to load triangle texture coordinates
            if (indices[3 * f + 2] * 2 + 1 < texcoords.size())
            {
                t0_ = Vec(texcoords[indices[3 * f] * 2],
						  texcoords[indices[3 * f] * 2 + 1], 0.);
                t1_ = Vec(texcoords[indices[3 * f + 1] * 2],
						  texcoords[indices[3 * f + 1] * 2 + 1], 0.);
                t2_ = Vec(texcoords[indices[3 * f + 2] * 2],
						  texcoords[indices[3 * f + 2] * 2 + 1], 0.);
            }

            if (m_shapes[i].mesh.material_ids[ f ] < materials.size())
                tris.push_back(new Triangle(v0_, v1_, v2_, t0_, t1_, t2_, &materials[ m_shapes[i].mesh.material_ids[ f ] ]));
            else
                tris.push_back(new Triangle(v0_, v1_, v2_, t0_, t1_, t2_, &m_m));
        }
    }

	LOG(INFO) << "- Total triangles " << tris.size();

    // Clean up
    m_shapes.clear();
    m_materials.clear();
    
	printf(" - Building k-d tree...\n\n");
    // build these triangles in mesh into 
    // a kdTree.
    node = KDNode().build(tris, 0);
    printf("\n");
	//bvh = BVH(&tris);
}

// Check if ray intersects with mesh. Returns ObjectIntersection data structure
ObjectIntersection Mesh::get_intersection(const Ray &ray)
{
    double t=0, tmin=INFINITY;
    Vec normal = Vec();
    Vec colour = Vec();
    bool hit = node->hit(node, ray, t, tmin, normal, colour);
    //bool hit = bvh.getIntersection(ray, t, tmin, normal);
    return ObjectIntersection(hit, tmin, normal, Material(DIFF, colour, Vec()));

}
