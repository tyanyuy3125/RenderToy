#ifndef BVH_H
#define BVH_H

#include <vector>

#include "geoobj.h"
#include "mathfunc.h"
#include "ray.h"

namespace OpenPT
{
    class Triangle;

    struct BoundingBox
    {
        Vector3f vmin, vmax;
        BoundingBox();
        BoundingBox(const Vector3f &vmin_, const Vector3f &vmax_);

        const bool Intersect(const Ray &ray, float &t_min, float &t_max);
        /// @brief Extends by another bbox.
        /// @param bbox
        void ExtendBy(const BoundingBox &bbox);
        const Vector3f Centroid() const;

        const Triangle *object{nullptr};
    };

    struct OctreeNode
    {
        OctreeNode *child[8];
        std::vector<const BoundingBox *> node_bbox_list;
        BoundingBox bbox;
        bool is_leaf;
        OctreeNode();
        ~OctreeNode();
    };

    struct Octree
    {
        Octree(const BoundingBox &tree_bbox_);
        void Insert(const BoundingBox *bbox_insert);
        void Build();
        ~Octree();
        struct QueueElement
        {
            const OctreeNode *node;
            float t;
            QueueElement(const OctreeNode *n, float t_) : node(n), t(t_) {}
            friend bool operator<(const QueueElement &a, const QueueElement &b)
            {
                return a.t > b.t;
            }
        };
        BoundingBox tree_bbox;
        OctreeNode *root;

    private:
        void Insert(OctreeNode *&node, const BoundingBox *bbox_insert, const BoundingBox &bbox, int depth);
        void Build(OctreeNode *&node, const BoundingBox &bbox);
        void RecursiveDelete(OctreeNode *&node);
    };

    class BVH
    {
    public:
        Octree *octree;
        std::vector<BoundingBox> bbox_list;
        BVH(std::vector<const Triangle *> &models);
        const Triangle *Intersect(const Ray &ray, float &t, float &u, float &v) const;
        const Triangle *Intersect(const Ray &ray, Vector3f &position) const;
        ~BVH();
    };
}

#endif // BVH_H