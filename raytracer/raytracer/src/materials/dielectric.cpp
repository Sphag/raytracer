#include "rtpch.h"
#include "materials/dielectric.h"


bool Dielectric::Scatter(const Ray& rayIn, const HitInfo& hitInfo, FRGBA& attenuation, std::vector<Ray>& scattered) const
{
   RT_ASSERT(scattered.size() == 0);

   attenuation = m_Albedo->Value(hitInfo.u, hitInfo.v, hitInfo.hitPoint);
   float ratio = hitInfo.isFrontFace ? 1.0f / m_RefractionIdx : m_RefractionIdx;

   glm::vec3 unitDirection = glm::normalize(rayIn.Direction());
   float cosEta = glm::min(glm::dot(-unitDirection, hitInfo.normal), 1.0f);
   float sinEta = glm::sqrt(1.0f - cosEta * cosEta);
   glm::vec3 direction = glm::refract(unitDirection, hitInfo.normal, ratio);
   float reflectProbe = Schlick(cosEta, ratio);
   if (direction != glm::vec3(0.0f) &&  RT_RandomFloat() < reflectProbe || direction == glm::vec3(0.0f)) {
      direction = glm::reflect(unitDirection, hitInfo.normal);
   }
   scattered.emplace_back(hitInfo.hitPoint, direction);
   return true;
}

float Dielectric::Schlick(float cosine, float ratio) const 
{
   float r0 = (1 - ratio) / (1 + ratio);
   r0 = r0*r0;
   return r0 + (1 - r0)*glm::pow((1 - cosine), 5);
}

