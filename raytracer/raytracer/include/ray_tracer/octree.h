#ifndef __OCTREE_H_INCLUDED
#define __OCTREE_H_INCLUDED

#include "ray_tracer/aabb.h"
#include "objects/triangle.h"

constexpr int MAX_OBJECTS_IN_ONE_NODE = 5;

class Octree
{
public:
   Octree(const AABB& initialBox = AABB()) : m_InitialBox(initialBox), m_Data(), m_Root(nullptr) {}

   void Clear();

   ~Octree() { Clear(); }

   void Construct(const std::vector<std::shared_ptr<Triangle>>& objects);

private:
   struct OctreeNode
   {
      AABB box;                         // box
      OctreeNode* childNodes[8];        // child nodes
      std::vector<int> objectsIndices;  // indices to objects, AABB of which is contained in that node
   };

   void ConstructImpl(OctreeNode* node, const std::vector<std::shared_ptr<Triangle>>& objects);
   void SetUpBoxes(OctreeNode *parent) const;
   std::vector<AABB> GetAABB(const std::vector<int>& indices);
   void ClearImpl(OctreeNode* node);
private:
   AABB m_InitialBox;
   std::vector<std::shared_ptr<Triangle>> m_Data;
   OctreeNode* m_Root;
};

#endif