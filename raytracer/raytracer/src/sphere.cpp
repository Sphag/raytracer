#include "rtpch.h"

#include "sphere.h"


bool Sphere::Hit(const Ray& ray, float& t) const
{
   glm::vec3 oc = ray.Origin() - m_Center;
   float a = glm::dot(ray.Direction(), ray.Direction());
   float b = 2.0f * glm::dot(ray.Direction(), oc);
   float c = glm::dot(oc, oc) - m_Radius * m_Radius;

   float discriminant = b*b - 4.0f * a * c;

   if (discriminant < 0.0f) {
      return false;
   }

   t = (-b - glm::sqrt(discriminant)) / (2.0f * a);

   return true;
}
