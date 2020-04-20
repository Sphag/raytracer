#include "rtpch.h"
#include "ray_tracer/bvh.h"
#include "ray_tracer/intersect_mng.h"


void BVH::Construct(const std::vector<std::shared_ptr<BaseObject>>& objects)
{
   if (m_Root) {
      Clear();
   }

   m_Objects = objects;
   std::vector<int> indices; for (int i = 0; i < objects.size(); i++) indices.push_back(i);
   ConstructImpl(&m_Root, indices);
}

bool BVH::FindIntersectingVolumeImpl(const Ray& ray, BVHNode* inNode, BVHNode** outNode) const
{
   if (!inNode) {
      return false;
   }

   if (!IntersectMng::Intersects(m_AABB, ray)) {
      return false;
   }

   if (FindIntersectingVolumeImpl(ray, inNode->first, outNode)) {
      return true;
   }

   if (FindIntersectingVolumeImpl(ray, inNode->second, outNode)) {
      return true;
   }

   *outNode = inNode;
   return true;
}

void BVH::ClearImpl(BVHNode** node)
{
   if (node && *node) {
      ClearImpl(&((*node)->first));
      ClearImpl(&((*node)->second));
      free(*node);
   }
}

void BVH::ConstructImpl(BVHNode** node, const std::vector<int>& indices)
{
   *node = (BVHNode*)malloc(sizeof(BVHNode));
   memset(*node, 0, sizeof(BVHNode));

   (*node)->objectIndices = indices;
   (*node)->aabb = GetCommonAABB(GetAABB((*node)->objectIndices));
   if ((*node)->objectIndices.size() > MAX_OBJECTS_IN_VOLUME) {
      std::vector<int> objIndices = indices;
      std::sort(objIndices.begin(), objIndices.end(), [this](int lhs, int rhs) {
         return cmp(lhs, rhs);
      });

      std::vector<int> left(objIndices.begin(), objIndices.begin() + objIndices.size() / 2);
      std::vector<int> right(objIndices.begin() + objIndices.size() / 2, objIndices.end());

      ConstructImpl(&(*node)->first, left);
      ConstructImpl(&(*node)->second, right);
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

bool BVH::FindIntersectingVolume(const Ray& ray, BVHNode** outNode) const
{
   return FindIntersectingVolumeImpl(ray, m_Root, outNode);
}
