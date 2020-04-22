#ifndef __BVH_H_INCLUDED
#define __BVH_H_INCLUDED

#include <vector>
#include <memory>

#include "objects/base_object.h"
#include "ray_tracer/aabb.h"


struct BVHNode
{
   AABB aabb;
   BVHNode* first;
   BVHNode* second;
   std::vector<int> objectIndices;
};

constexpr int MAX_OBJECTS_IN_VOLUME = 20;

class BVH
{
public:
   BVH(const AABB& aabb = AABB(), const std::vector<std::shared_ptr<BaseObject>>& objects = std::vector<std::shared_ptr<BaseObject>>()) :
      m_Root(nullptr), m_AABB(aabb)
   {
   }

   ~BVH() { Clear(); }

   void Clear() { ClearImpl(&m_Root); }

   bool GetRootAABB(AABB& outAABB) const { outAABB = m_Root->aabb; return m_Root; }

   void Construct(const std::vector<std::shared_ptr<BaseObject>>& objects);
   std::vector<AABB> GetAABB(const std::vector<int>& indices);
   std::shared_ptr<BaseObject> GetObjectById(int idx) const { return m_Objects[idx]; }

   bool FindIntersectingVolume(const Ray& ray, float minDist, float maxDist, HitInfo& hitInfo) const;
private:
   bool FindIntersectingVolumeImpl(const Ray& ray, BVHNode* inNode, float minDist, float maxDist, HitInfo& hitInfo) const;
   void ClearImpl(BVHNode** node);
   void ConstructImpl(BVHNode** node, const std::vector<int>& indices);
   bool cmp(int lhs, int rhs) {
      return glm::distance(m_Root->aabb.GetCenter() - m_Root->aabb.GetDim(), m_Objects[lhs]->GetAABB().GetCenter() - m_Objects[lhs]->GetAABB().GetDim()) <
         glm::distance(m_Root->aabb.GetCenter() - m_Root->aabb.GetDim(), m_Objects[rhs]->GetAABB().GetCenter() - m_Objects[rhs]->GetAABB().GetDim());
   }
private:
   std::vector<std::shared_ptr<BaseObject>> m_Objects;
   BVHNode* m_Root;
   AABB     m_AABB;
};

#endif
