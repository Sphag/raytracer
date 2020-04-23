#include "rtpch.h"
#include "objects/plane.h"


bool Plane::Hit(const Ray& ray, float minDist, float maxDist, HitInfo& hitInfo) const
{
   float denominator = glm::dot(m_Normal, ray.Direction());
   if (denominator == 0.0f) {
      return false;
   }

   float t = glm::dot(ray.Origin() + m_Offset, m_Normal) / denominator;
   if (t >= minDist && t <= maxDist) {
      hitInfo.t = t;
      hitInfo.SetFaceNormal(ray, m_Normal);
      hitInfo.hitPoint = ray.At(t);
      hitInfo.material = m_Material;
      return true;
   }

   return false;
}

void Plane::GetUV(const glm::vec3& point, float& u, float& v) const
{
   
}

