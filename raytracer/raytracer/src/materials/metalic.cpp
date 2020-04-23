#include "rtpch.h"
#include "materials/metalic.h"


bool Metalic::Scatter(const Ray& rayIn, const HitInfo& hitInfo, FRGBA& attenuation, std::vector<Ray>& scattered) const
{
   RT_ASSERT(scattered.size() == 0);
   glm::vec3 reflected = glm::reflect(hitInfo.hitPoint, hitInfo.normal);
   scattered.emplace_back(hitInfo.hitPoint, reflected + m_Fuzziness * RandomUnitVector());
   attenuation = m_Albedo->Value(hitInfo.u, hitInfo.v, hitInfo.hitPoint);

   return glm::dot(scattered[0].Direction(), hitInfo.normal) > 0.0f;
}

MATERIAL_TYPE Metalic::s_MaterialType = MATERIAL_TYPE::METALIC;
