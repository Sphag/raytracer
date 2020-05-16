#include "rtpch.h"
#include "objects/box.h"


bool Box::Hit(const Ray& ray, float minDist, float maxDist, HitInfo& hitInfo) const
{
   glm::vec3 min = m_Center - m_Dim;
   glm::vec3 max = m_Center + m_Dim;
   float t0x = glm::min(min.x - ray.Origin().x, max.x - ray.Origin().x / ray.Direction().x);
   float t1x = glm::max(min.x - ray.Origin().x, max.x - ray.Origin().x / ray.Direction().x);
   if (t1x < t0x)
      return false;

   float t0y = glm::min(min.y - ray.Origin().y, max.y - ray.Origin().y / ray.Direction().y);
   float t1y = glm::max(min.y - ray.Origin().y, max.y - ray.Origin().y / ray.Direction().y);
   if (t1y < t0y)
      return false;

   float t0z = glm::min(min.z - ray.Origin().z, max.z - ray.Origin().z / ray.Direction().z);
   float t1z = glm::max(min.z - ray.Origin().z, max.z - ray.Origin().z / ray.Direction().z);
   if (t1z < t0z)
      return false;

   if (glm::compMax(glm::vec3(t1x, t1y, t1z)) < 0.0f)
      return false;

   // NOT DONE!
   if      (t0x <= t0y && t0x <= t0z && t0x >= 0.0f) return t0x;
   else if (t0y <= t0z && t0y >= 0.0f)               return t0y;
   else if (t0z >= 0.0f)                             return t0z;
   if (t1x <= t1y && t1x <= t1z && t1x >= 0.0f)      return t1x;
   else if (t1y <= t1z && t1y >= 0.0f)               return t1y;
   else if (t1z >= 0.0f)                             return t1z;

   return true;
}
