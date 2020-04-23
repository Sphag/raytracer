#include "rtpch.h"
#include "materials/lambertian.h"


bool Lambertian::Scatter(const Ray& rayIn, const HitInfo& hitInfo, FRGBA& attenuation, std::vector<Ray>& scattered) const
{
   RT_ASSERT(scattered.size() == 0);

   glm::vec3 scatterDir = glm::normalize(glm::normalize(hitInfo.normal) + RandomUnitVector());
   scattered.emplace_back(hitInfo.hitPoint, scatterDir);
   auto fff = m_Albedo->Value(hitInfo.u, hitInfo.v, hitInfo.hitPoint);
   attenuation = fff;

   return true;
}

MATERIAL_TYPE Lambertian::s_MaterialType = MATERIAL_TYPE::LAMBERTIAN;
