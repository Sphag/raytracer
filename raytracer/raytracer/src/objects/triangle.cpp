#include "rtpch.h"
#include "objects/triangle.h"


bool Triangle::Hit(const Ray& ray, float minDist, float maxDist, HitInfo& hitInfo) const
{
   float det = -glm::dot(ray.Direction(), m_Normal);
   float invdet = 1.0f / det;
   glm::vec3 AO = ray.Origin() - m_A;
   glm::vec3 DAO = glm::cross(AO, ray.Direction());
   float u =  glm::dot(m_AC, DAO)     * invdet;
   float v = -glm::dot(m_AB, DAO)    * invdet;
   float t =  glm::dot(AO, m_Normal) * invdet;
   if (det >= RT_FloatEpsilon && t >= minDist && t <= maxDist && u >= 0.0 && v >= 0.0 && (u + v) <= 1.0) {
      hitInfo.hitPoint = ray.At(t);
      hitInfo.t = t;
      hitInfo.SetFaceNormal(ray, glm::normalize(m_Normal));
      hitInfo.material = m_Material;
      return true;
   }

   return false;
}

void Triangle::ApplyTransform()
{
   auto as = m_Transform.GetMatrix();
   m_A = glm::vec3(m_Transform.GetMatrix() * glm::vec4(m_A, 1.0f));
   m_B = glm::vec3(m_Transform.GetMatrix() * glm::vec4(m_B, 1.0f));
   m_C = glm::vec3(m_Transform.GetMatrix() * glm::vec4(m_C, 1.0f));
   CalculateMisc();
}

