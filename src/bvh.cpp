#include "bvh.h"
#include "meshobj.h"

#include <cstring>
#include <queue>

namespace OpenPT
{
    BoundingBox::BoundingBox()
        : vmin(Vector3f::O), vmax(Vector3f::O)
    {
    }

    BoundingBox::BoundingBox(const Vector3f &vmin_, const Vector3f &vmax_)
        : vmin(vmin_), vmax(vmax_)
    {
    }

    const bool BoundingBox::Intersect(const Ray &ray, float &t_min)
    {
        float tmin = (vmin.x - ray.src.x) / ray.direction.x;
        float tmax = (vmax.x - ray.src.x) / ray.direction.x;

        if (tmin > tmax)
        {
            std::swap(tmin, tmax);
        }

        float tymin = (vmin.y - ray.src.y) / ray.direction.y;
        float tymax = (vmax.y - ray.src.y) / ray.direction.y;

        if (tymin > tymax)
        {
            std::swap(tymin, tymax);
        }

        if ((tmin > tymax) || (tymin > tmax))
        {
            return false;
        }

        if (tymin > tmin)
            tmin = tymin;

        if (tymax < tmax)
            tmax = tymax;

        float tzmin = (vmin.z - ray.src.z) / ray.direction.z;
        float tzmax = (vmax.z - ray.src.z) / ray.direction.z;

        if (tzmin > tzmax)
        {
            std::swap(tzmin, tzmax);
        }

        if ((tmin > tzmax) || (tzmin > tmax))
        {
            return false;
        }

        if (tzmin > tmin)
        {
            tmin = tzmin;
        }

        if (tzmax < tmax)
        {
            tmax = tzmax;
        }
        
        t_min = tmin;

        return true;
    }

    void BoundingBox::ExtendBy(const BoundingBox &bbox)
    {
        vmin = {std::min(vmin.x, bbox.vmin.x),
                std::min(vmin.y, bbox.vmin.y),
                std::min(vmin.z, bbox.vmin.z)};
        vmax = {std::max(vmax.x, bbox.vmax.x),
                std::max(vmax.y, bbox.vmax.y),
                std::max(vmax.z, bbox.vmax.z)};
    }

    const Vector3f BoundingBox::Centroid() const
    {
        return 0.5f * (vmin + vmax);
    }

    OctreeNode::OctreeNode()
        : is_leaf(true)
    {
        for (uint8_t i = 0; i < 8; ++i)
        {
            child[i] = nullptr;
        }
    }

    OctreeNode::~OctreeNode()
    {
        // Do nothing.
        // Memory managed by Octree.
    }

    Octree::Octree(const BoundingBox &tree_bbox_)
        : tree_bbox(tree_bbox_)
    {
        root = new OctreeNode;
    }

    void Octree::Insert(const BoundingBox *bbox_insert)
    {
        Insert(root, bbox_insert, tree_bbox, 0);
    }

    void Octree::Build()
    {
        Build(root, tree_bbox);
    }

    Octree::~Octree()
    {
        RecursiveDelete(root);
    }

    void Octree::Insert(OctreeNode *&node, const BoundingBox *bbox_insert, const BoundingBox &bbox, int depth)
    {
        if (node->is_leaf)
        {
            if (node->node_bbox_list.size() == 0 || depth == 16)
            {
                node->node_bbox_list.push_back(bbox_insert);
            }
            else
            {
                node->is_leaf = false;
                while (node->node_bbox_list.size())
                {
                    Insert(node, node->node_bbox_list.back(), bbox, depth);
                    node->node_bbox_list.pop_back();
                }
                Insert(node, bbox_insert, bbox, depth);
            }
        }
        else
        {
            Vector3f bbox_insert_centroid = bbox_insert->Centroid();
            Vector3f node_centroid = bbox.Centroid();
            BoundingBox child_bbox;
            uint8_t child_index = 0;

            if (bbox_insert_centroid.x > node_centroid.x)
            {
                child_index = 4;
                child_bbox.vmin.x = node_centroid.x;
                child_bbox.vmax.x = bbox.vmax.x;
            }
            else
            {
                child_bbox.vmin.x = bbox.vmin.x;
                child_bbox.vmax.x = node_centroid.x;
            }

            if (bbox_insert_centroid.y > node_centroid.y)
            {
                child_index += 2;
                child_bbox.vmin.y = node_centroid.y;
                child_bbox.vmax.y = bbox.vmax.y;
            }
            else
            {
                child_bbox.vmin.y = bbox.vmin.y;
                child_bbox.vmax.y = node_centroid.y;
            }

            if (bbox_insert_centroid.z > node_centroid.z)
            {
                child_index += 1;
                child_bbox.vmin.z = node_centroid.z;
                child_bbox.vmax.z = bbox.vmax.z;
            }
            else
            {
                child_bbox.vmin.z = bbox.vmin.z;
                child_bbox.vmax.z = node_centroid.z;
            }

            if (node->child[child_index] == nullptr)
            {
                node->child[child_index] = new OctreeNode;
            }
            Insert(node->child[child_index], bbox_insert, child_bbox, depth + 1);
        }
    }

    void Octree::Build(OctreeNode *&node, const BoundingBox &bbox)
    {
        if (node->is_leaf)
        {
            for (const auto &e : node->node_bbox_list)
            {
                node->bbox.ExtendBy(*e);
            }
        }
        else
        {
            for (uint8_t i = 0; i < 8; ++i)
            {
                if(node->child[i])
                {
                    BoundingBox child_bbox;
                    Vector3f centroid = bbox.Centroid();

                    child_bbox.vmin.x = (i&4)?centroid.x : bbox.vmin.x;
                    child_bbox.vmax.x = (i&4)?bbox.vmax.x : centroid.x;

                    child_bbox.vmin.y = (i&2)?centroid.y : bbox.vmin.y;
                    child_bbox.vmax.y = (i&2)?bbox.vmax.y : centroid.y;

                    child_bbox.vmin.z = (i&1)?centroid.z : bbox.vmin.z;
                    child_bbox.vmax.z = (i&1)?bbox.vmax.z : centroid.z;
                    Build(node->child[i], child_bbox);
                    node->bbox.ExtendBy(node->child[i]->bbox);
                }
            }
        }
    }

    void Octree::RecursiveDelete(OctreeNode *&node)
    {
        for(uint8_t i =0;i<8;++i){
            if(node->child[i] != nullptr){
                RecursiveDelete(node->child[i]);
            }
        }
        delete node;
    }

    BVH::BVH(std::vector<const Triangle *> &models)
    {
        BoundingBox tree_bbox;
        bbox_list.reserve(models.size());
        for(int i=0;i<models.size();++i){
            bbox_list[i] = models[i]->BBox();
            tree_bbox.ExtendBy(bbox_list[i]);
            bbox_list[i].object = models[i];
        }

        octree = new Octree(tree_bbox);
        
        for(int i=0;i<models.size();++i){
            octree->Insert(&bbox_list[i]);
        }

        octree->Build();
    }

    const Triangle *BVH::Intersect(const Ray &ray, float &t, float &u, float &v) const
    {
        t = INF;
        const Triangle *intersected = nullptr;
        float placeholder;
        if(!octree->root->bbox.Intersect(ray, placeholder))
        {
            return nullptr;
        }

        std::priority_queue<Octree::QueueElement> queue;
        queue.push(Octree::QueueElement(octree->root, 0));

        while(!queue.empty() && queue.top().t < t)
        {
            auto node = queue.top().node;
            queue.pop();
            if(node->is_leaf)
            {
                for(const auto &e : node->node_bbox_list)
                {
                    float it,iu,iv;
                    if(e->object->Intersect(ray, it, iu, iv)){
                        if(it < t){
                            t = it;
                            u = iu;
                            v = iv;
                            intersected = e->object;
                        }
                    }
                }
            }
            else
            {
                for(uint8_t i=0;i<8;++i){
                    if(node->child[i]!=nullptr){
                        float t_bbox;
                        if(node->child[i]->bbox.Intersect(ray, t_bbox))
                        {
                            queue.push(Octree::QueueElement(node->child[i], t_bbox));
                        }
                    }
                }
            }
        }

        return intersected;
    }

    BVH::~BVH()
    {
        delete octree;
    }
}