#include "rtpch.h"
#include "hittable_list.h"
#include "ray.h"
#include "sphere.h"

bool HittableList::Hit(const Ray& ray, float minDist, float maxDist, HitInfo& hitInfo) const
{
   HitInfo tempHitInfo;
   bool isHitOccurred = false;
   float closest = maxDist;

   for (const auto& object : m_Objects) {
      if (object->Hit(ray, minDist, maxDist, tempHitInfo) && tempHitInfo.t < closest) {
         isHitOccurred = true;
         volatile float rad = ((Sphere*)object.get())->GetRadius();
         closest = tempHitInfo.t;
         hitInfo = tempHitInfo;
      }
   }

   return isHitOccurred;
}
