#ifndef __OCTREE_H_INCLUDED
#define __OCTREE_H_INCLUDED

#include "ray_tracer/aabb.h"
#include "objects/triangle.h"


struct OctreeNode
{
   AABB box;                         // box
   OctreeNode* childNodes[8];        // child nodes
   std::vector<int> objectsIndices;  // indices to objects, AABB of which is contained in that node
};



class Octree
{
public:
   int MAX_OBJECTS_IN_ONE_NODE = 50000;
   Octree(const AABB& initialBox = AABB()) : m_InitialBox(initialBox), m_Data(), m_Root(nullptr), m_TotalNodes(0) {}

   void Clear();

   ~Octree() { Clear(); }

   void Construct(const std::vector<std::shared_ptr<Triangle>>& objects);
   OctreeNode* GetRoot() const { return m_Root; }
   std::vector<AABB> GetAABB(const std::vector<int>& indices);

   bool FindIntersectedNode(const Ray& ray, float minDist, float maxDist, HitInfo& hitInfo) const;

   std::shared_ptr<Triangle> GetTriangleById(int idx) const { return m_Data[idx]; }

private:
   void ConstructImpl(OctreeNode** node, const std::vector<std::shared_ptr<Triangle>>& objects);
   void SetUpBoxes(OctreeNode *parent) const;
   void ClearImpl(OctreeNode** node);
   bool FindIntersectedNodeImpl(const Ray& ray, OctreeNode* inNode, float minDist, float maxDist, HitInfo& hitInfo) const;
private:
   AABB m_InitialBox;
   std::vector<std::shared_ptr<Triangle>> m_Data;
   OctreeNode* m_Root;
   int m_TotalNodes;
};

#endif