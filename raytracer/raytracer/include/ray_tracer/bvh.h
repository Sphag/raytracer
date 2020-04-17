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

constexpr int MAX_OBJECTS_IN_VOLUME = 5;

class BVH
{
public:
   BVH(const AABB& aabb = AABB(), const std::vector<std::shared_ptr<BaseObject>>& objects = std::vector<std::shared_ptr<BaseObject>>()) :
      m_Objects(objects), m_Root(nullptr), m_AABB(aabb)
   {
      if (!m_Objects.empty()) {
         Construct();
      }
   }

   ~BVH() { Clear(); }

   void Clear() { ClearImpl(m_Root); }

   void Construct();
private:
   void ClearImpl(BVHNode* node);
   void ConstructImpl(BVHNode* node, const std::vector<int>& indices);
   std::vector<AABB> GetAABB(const std::vector<int>& indices);
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
