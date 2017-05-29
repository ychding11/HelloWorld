#ifndef KDTREE_H
#define KDTREE_H

#include <vector>

#include "vector.h"
#include "ray.h"
#include "triangle.h"

// dynamic memory manage need to rewrite.
class KDNode
{
public:
    AABBox box;
    KDNode* left;
    KDNode* right;
    std::vector<Triangle*> triangles;
    bool leaf;

    KDNode(){};
    KDNode* build(std::vector<Triangle*> &tris, int depth);
    bool hit (KDNode* node, const Ray &ray, double &t, double &tmin, Vec &normal, Vec &c);
};

#endif // KDTREE_H