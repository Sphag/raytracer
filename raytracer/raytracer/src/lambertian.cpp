#include "rtpch.h"
#include "lambertian.h"
#include "math_utils.h"

bool Lambertian::Scatter(const Ray& rayIn, const HitInfo& hitInfo, FRGBA& attenuation, Ray& scattered) const
{
   glm::vec3 scatterDir = hitInfo.normal + RandomUnitHemisphere(hitInfo.normal);
   scattered = Ray(hitInfo.hitPoint, scatterDir);
   attenuation = m_Albedo;

   return true;
}
