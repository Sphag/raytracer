#include "rtpch.h"
#include "objects/scene.h"
#include "ray_tracer/ray.h"
#include "objects/sphere.h"
#include "ray_tracer/intersect_mng.h"

bool Scene::Hit(const Ray& ray, float minDist, float maxDist, HitInfo& hitInfo) const
{
   HitInfo tempHitInfo;
   bool isHitOccurred = false;

   if (m_BVH.FindIntersectingVolume(ray, minDist, maxDist, tempHitInfo)) {
      hitInfo = tempHitInfo;
      isHitOccurred = true;
   }

   return isHitOccurred;
}
