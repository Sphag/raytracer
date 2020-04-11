#ifndef __ACCELERATING_STRUCTURES_H
#define __ACCELERATING_STRUCTURES_H

#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include "core/ray.h"
#include "objects/triangle.h"
#include "objects/plane.h"


struct Rect
{
   glm::vec3 minVert;
   glm::vec3 maxVert;
};

class AABB
{
public:
   AABB(const glm::vec3& min = glm::vec3(0.0f), const glm::vec3& max = glm::vec3(1.0f)) : m_MinVert(min), m_MaxVert(max) {}

   glm::vec3 MinVert() const { return m_MinVert; }
   glm::vec3 MaxVert() const { return m_MaxVert; }

   void SetMinVert(const glm::vec3& min) { m_MinVert = min; }
   void SetMaxVert(const glm::vec3& max) { m_MaxVert = max; }

   bool Hit(const Ray& ray, float minDist, float maxDist) const;
   bool Intersects(const Plane& plane) const;
private:
   glm::vec3 m_MinVert; // vertex of BB, that contains the smallest x, y, z
   glm::vec3 m_MaxVert; // vertex of BB, that contains the largest x, y, z
};


constexpr int MAX_OBJECTS_IN_ONE_NODE = 5;

class Octree
{
public:
   Octree(const AABB& initialBox = AABB()) : m_InitialBox(initialBox), m_Data(), m_Root(nullptr) {}

   void Clear();

   ~Octree() { Clear(); }

   void Construct(const AABB& initialBB, const std::vector<std::shared_ptr<Triangle>>& objects);

private:
   struct OctreeNode
   {
      AABB rect;                        // rectangle
      OctreeNode* childNodes[8];        // child nodes
      std::vector<int> objectsIndices;  // indices to objects, AABB of which is contained in that node
   };

   void ConstructImpl(OctreeNode* node, const std::vector<std::shared_ptr<Triangle>>& objects);
   void SetUpRects(OctreeNode *parent, const glm::vec3& midPoint) const;

   void ClearImpl(OctreeNode* node);
private:
   AABB m_InitialBox;
   std::vector<std::shared_ptr<Triangle>> m_Data;
   OctreeNode* m_Root;
};

#endif