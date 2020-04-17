#include "rtpch.h"
#include "ray_tracer/bvh.h"


void BVH::Construct()
{
   if (m_Root) {
      Clear();
   }

   std::vector<int> indices; for (int i = 0; i < m_Objects.size(); i++) indices.push_back(i);
   ConstructImpl(m_Root, indices);
}

void BVH::ClearImpl(BVHNode* node)
{
   if (node) {
      free(node);
      ClearImpl(node->first);
      ClearImpl(node->second);
   }
}

void BVH::ConstructImpl(BVHNode* node, const std::vector<int>& indices)
{
   node = (BVHNode*)malloc(sizeof(BVHNode));
   memset(node, 0, sizeof(BVHNode));

   node->objectIndices = indices;
   node->aabb = GetCommonAABB(GetAABB(node->objectIndices));
   if (node->objectIndices.size() > MAX_OBJECTS_IN_VOLUME) {
      std::vector<int> objIndices = indices;
      std::sort(objIndices.begin(), objIndices.end(), [this](int lhs, int rhs) {
         return cmp(lhs, rhs);
      });

      std::vector<int> left(objIndices.begin(), objIndices.begin() + objIndices.size() / 2);
      std::vector<int> right(objIndices.begin() + objIndices.size() / 2, objIndices.end());

      ConstructImpl(node->first, left);
      ConstructImpl(node->second, right);
   }
}

std::vector<AABB> BVH::GetAABB(const std::vector<int>& indices)
{
   std::vector<AABB> aabbs;
   for (int i = 0; i < indices.size(); i++) {
      aabbs.push_back(m_Objects[indices[i]]->GetAABB());
   }

   return aabbs;
}