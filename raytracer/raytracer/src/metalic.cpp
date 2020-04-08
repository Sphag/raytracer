#include "rtpch.h"
#include "metalic.h"
#include "math_utils.h"


bool Metalic::Scatter(const Ray& rayIn, const HitInfo& hitInfo, FRGBA& attenuation, Ray& scattered) const
{
   glm::vec3 reflected = glm::reflect(hitInfo.hitPoint, hitInfo.normal);
   scattered = Ray(hitInfo.hitPoint, reflected + m_Fuzziness * RandomUnitVector());
   attenuation = m_Albedo;

   return glm::dot(scattered.Direction(), hitInfo.normal) > 0.0f;
}
