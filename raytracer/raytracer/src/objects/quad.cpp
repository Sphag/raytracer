#include "rtpch.h"
#include "objects/quad.h"

bool Quad::Hit(const Ray& ray, float minDist, float maxDist, HitInfo& hitInfo) const
{
   if (m_Tri1.Hit(ray, minDist, maxDist, hitInfo) || m_Tri2.Hit(ray, minDist, maxDist, hitInfo)) {
      hitInfo.material = m_Material;
      GetUV(hitInfo.hitPoint, hitInfo.u, hitInfo.v);
      return true;
   }

   return false;
}

void Quad::ApplyTransform()
{
   m_Tri1.SetTransform(m_Transform);
   m_Tri2.SetTransform(m_Transform);
   m_Tri1.ApplyTransform();
   m_Tri2.ApplyTransform();
}

void Quad::GetUV(const glm::vec3& point, float &u, float&v) const
{
   glm::vec3 diag = m_Tri1.C() - m_Tri1.A();
   u = glm::abs(m_Tri1.B().x - point.x) / diag.x;
   v = glm::abs(m_Tri1.B().y - point.y) / diag.y;
}
