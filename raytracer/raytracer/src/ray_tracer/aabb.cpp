#include "rtpch.h"
#include "ray_tracer/aabb.h"


bool AABB::Hit(const Ray& ray, float minDist, float maxDist) const
{
   glm::vec3 t0 = glm::min(
      (m_MinVert - ray.Origin()) / ray.Direction(),
      (m_MaxVert - ray.Origin()) / ray.Direction()
   );

   glm::vec3 t1 = glm::max(
      (m_MinVert - ray.Origin()) / ray.Direction(),
      (m_MaxVert - ray.Origin()) / ray.Direction()
   );

   glm::vec3 tMin = glm::max(t0, glm::vec3(minDist));
   glm::vec3 tMax = glm::min(t1, glm::vec3(maxDist));

   if (glm::any(glm::lessThanEqual(tMax, tMin))) {
      return false;
   }

   return true;
}

bool AABB::Intersects(const Plane& plane) const
{
   glm::vec3 mid = (m_MaxVert + m_MinVert) * 0.5f;
   glm::vec3 ext = m_MaxVert - mid;


   float r = glm::abs(glm::dot(ext, plane.GetNormal()));
   float s = glm::dot(plane.GetNormal(), mid) - glm::dot(plane.GetOffset(), plane.GetNormal());

   return glm::abs(s) <= glm::abs(r);
}

bool AABB::Intersects(const Triangle& triangle) const
{
   glm::vec3 mid = (m_MaxVert + m_MinVert) * 0.5f;
   glm::vec3 h = m_MaxVert - mid;

   glm::vec3 f[3] = {
      triangle.B() - triangle.A(),
      triangle.C() - triangle.B(),
      triangle.A() - triangle.C()
   };

   glm::vec3 v[3] = {
      triangle.A() - mid,
      triangle.B() - mid,
      triangle.C() - mid
   };
   glm::mat3 vv(v[0], v[1], v[2]);

   glm::vec3 axis[13];
   for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
         axis[i * 3 + j] = glm::cross(UnitVector(i), f[j]);
      }
   }
   axis[9] = RT_Float3UnitX;
   axis[10] = RT_Float3UnitY;
   axis[11] = RT_Float3UnitZ;
   axis[12] = triangle.GetNormal();

   for (int i = 0; i < 13; i++) {
      float p0 = glm::dot(axis[i], v[0]);
      float p1 = glm::dot(axis[i], v[1]);
      float p2 = glm::dot(axis[i], v[2]);

      glm::vec3 p = axis[i] * vv;

      float r = glm::dot(h, glm::abs(axis[i]));

      if (glm::compMin(p) > r || glm::compMax(p) < -r) return false;
   }

   return true;
}