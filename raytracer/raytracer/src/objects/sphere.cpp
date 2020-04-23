#include "rtpch.h"

#include "objects/sphere.h"


bool Sphere::Hit(const Ray& ray, float minDist, float maxDist, HitInfo& hitInfo) const
{
   glm::vec3 oc = ray.Origin() - m_Center;
   float a = glm::dot(ray.Direction(), ray.Direction());
   float b = 2.0f * glm::dot(ray.Direction(), oc);
   float c = glm::dot(oc, oc) - m_Radius * m_Radius;

   float discriminant = b*b - 4.0f * a * c;

   if (discriminant < 0.0f) {
      return false;
   }

   float t1 = (-b - glm::sqrt(discriminant)) / (2.0f * a);
   RT_ASSERT(m_Material);
   if (t1 >= minDist && t1 <= maxDist) {
      hitInfo.hitPoint = ray.At(t1);
      hitInfo.t = t1;
      hitInfo.SetFaceNormal(ray, glm::normalize(hitInfo.hitPoint - m_Center));
      hitInfo.material = m_Material;
      GetUV((hitInfo.hitPoint - m_Center) / m_Radius, hitInfo.u, hitInfo.v);
      return true;
   }

   float t2 = (-b + glm::sqrt(discriminant)) / (2.0f * a);
   if (t2 >= minDist && t2 <= maxDist) {
      hitInfo.hitPoint = ray.At(t2);
      hitInfo.t = t2;
      hitInfo.SetFaceNormal(ray, glm::normalize(hitInfo.hitPoint - m_Center));
      hitInfo.material = m_Material;
      GetUV((hitInfo.hitPoint - m_Center) / m_Radius, hitInfo.u, hitInfo.v);
      return true;
   }

   return false;
}

void Sphere::GetUV(const glm::vec3& point, float &u, float&v) const
{
   float phi = glm::atan(point.z, point.x);
   float theta = glm::asin(point.y);
   u = 1 - (phi + RT_FloatPi) / (2 * RT_FloatPi);
   v = (theta + RT_FloatPi / 2) / RT_FloatPi;
}
