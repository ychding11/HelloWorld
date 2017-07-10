
#ifndef _ACCELERATORS_KDTREEACCEL_H
#define _ACCELERATORS_KDTREEACCEL_H

// accelerators/kdtreeaccel.h*
//#include "primitive.h"

#include "pathtracer.h"
#include "geometry.h"
#include "objects.h"
#include <memory>

//namespace pbrt {

// KdTreeAccel Declarations
struct KdAccelNode;
struct BoundEdge;

class Primitive;
class SurfaceInteraction;

class KdTreeAccel 
{
  public:

    // KdTreeAccel Public Methods
    KdTreeAccel(const std::vector<std::shared_ptr<Primitive>> &p,
                int isectCost = 80, int traversalCost = 1,
                Float emptyBonus = 0.5, int maxPrims = 1, int maxDepth = -1);

    Bounds3f WorldBound() const { return bounds; }

    ~KdTreeAccel();
    bool Intersect(const Ray &ray, SurfaceInteraction *isect) const;
    bool IntersectP(const Ray &ray) const;

  private:

    // KdTreeAccel Private Methods
    void buildTree(int nodeNum, const Bounds3f &bounds,
                   const std::vector<Bounds3f> &primBounds, int *primNums,
                   int nprims, int depth,
                   const std::unique_ptr<BoundEdge[]> edges[3], int *prims0,
                   int *prims1, int badRefines = 0);

    // KdTreeAccel Private Data
    const int isectCost, traversalCost, maxPrims;
    const Float emptyBonus;
    std::vector<std::shared_ptr<Primitive>> primitives;
    std::vector<int> primitiveIndices;
    KdAccelNode *nodes;
    int nAllocedNodes, nextFreeNode;
    Bounds3f bounds;
};

struct KdToDo
{
    const KdAccelNode *node;
    Float tMin, tMax;
};

#if 0
std::shared_ptr<KdTreeAccel> CreateKdTreeAccelerator(
    const std::vector<std::shared_ptr<Primitive>> &prims, const ParamSet &ps);
#endif

//}  // namespace pbrt

#endif  // PBRT_ACCELERATORS_KDTREEACCEL_H
