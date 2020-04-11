#include "rtpch.h"
#include "objects/hittable_list.h"
#include "core/ray.h"
#include "objects/sphere.h"

bool HittableList::Hit(const Ray& ray, float minDist, float maxDist, HitInfo& hitInfo) const
{
   HitInfo tempHitInfo;
   bool isHitOccurred = false;
   float closest = maxDist;

   for (const auto& object : m_Objects) {
      if (object->Hit(ray, minDist, maxDist, tempHitInfo) && tempHitInfo.t < closest) {
         isHitOccurred = true;
         closest = tempHitInfo.t;
         hitInfo = tempHitInfo;
      }
   }

   return isHitOccurred;
}