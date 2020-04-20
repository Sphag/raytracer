#include "rtpch.h"
#include "objects/scene.h"
#include "ray_tracer/ray.h"
#include "objects/sphere.h"
#include "ray_tracer/intersect_mng.h"

bool Scene::Hit(const Ray& ray, float minDist, float maxDist, HitInfo& hitInfo) const
{
   HitInfo tempHitInfo;
   bool isHitOccurred = false;
   float closest = maxDist;

   BVHNode* node = nullptr;
   if (!m_BVH.FindIntersectingVolume(ray, &node)) {
      return false;
   }

   RT_ASSERT(node);
   for (int i = 0; i < node->objectIndices.size(); i++) {
      if (m_BVH.GetObjectById(i)->Hit(ray, minDist, maxDist, tempHitInfo) && tempHitInfo.t < closest) {
         isHitOccurred = true;
         closest = tempHitInfo.t;
         hitInfo = tempHitInfo;
      }
   }

   return isHitOccurred;
}
