#include "rtpch.h"
#include "ray_tracer/aabb.h"

AABB GetCommonAABB(const std::vector<AABB> boxes)
{
   glm::vec3 min = {  INFINITY,  INFINITY,  INFINITY };
   glm::vec3 max = { -INFINITY, -INFINITY, -INFINITY };

   for (const auto& box : boxes) {
      glm::vec3 bmin = box.GetCenter() - box.GetDim();
      glm::vec3 bmax = box.GetCenter() + box.GetDim();
      if (bmin.x < min.x) min.x = bmin.x;
      if (bmin.y < min.y) min.y = bmin.y;
      if (bmin.z < min.z) min.z = bmin.z;

      if (bmax.x > max.x) max.x = bmax.x;
      if (bmax.y > max.y) max.y = bmax.y;
      if (bmax.z > max.z) max.z = bmax.z;

   }

   return AABB(0.5f * (min + max), max - 0.5f * (min + max));
}
