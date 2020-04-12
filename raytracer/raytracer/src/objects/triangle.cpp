#include "rtpch.h"
#include "objects/triangle.h"


bool Triangle::Hit(const Ray& ray, float minDist, float maxDist, HitInfo& hitInfo) const
{
   float det = -glm::dot(ray.Direction(), m_Normal);
   float invdet = 1.0 / det;
   glm::vec3 AO = ray.Origin() - m_A;
   glm::vec3 DAO = glm::cross(AO, ray.Direction());
   float u =  glm::dot(m_AC, DAO)     * invdet;
   float v = -glm::dot(m_AB, DAO)    * invdet;
   float t =  glm::dot(AO, m_Normal) * invdet;
   if (det >= RT_FloatEpsilon && t >= minDist && t <= maxDist && u >= 0.0 && v >= 0.0 && (u + v) <= 1.0) {
      hitInfo.hitPoint = ray.At(t);
      hitInfo.SetFaceNormal(ray, glm::normalize(m_Normal));
      hitInfo.material = m_Material;
      return true;
   }

   return false;
}

