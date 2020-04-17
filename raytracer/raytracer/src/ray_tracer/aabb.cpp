#include "rtpch.h"
#include "ray_tracer/aabb.h"

AABB GetCommonAABB(const std::vector<AABB> boxes)
{
   glm::vec3 min = {  INFINITY,  INFINITY,  INFINITY };
   glm::vec3 max = { -INFINITY, -INFINITY, -INFINITY };

   for (const auto& box : boxes) {
      glm::vec3 bmin = box.GetCenter() - box.GetDim();
      glm::vec3 bmax = box.GetCenter() + box.GetDim();
      if (glm::any(glm::lessThan(bmin, min))) {
         min = bmin;
      } else if (glm::any(glm::greaterThan(bmax, max))) {
         max = bmax;
      }
   }

   return AABB(0.5f * (min + max), max - 0.5f * (min + max));
}
