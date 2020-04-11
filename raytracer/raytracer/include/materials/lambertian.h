#ifndef __LAMBERTIAN_H_INCLUDED
#define __LAMBERTIAN_H_INCLUDED

#include <glm/glm.hpp>

#include "base_material.h"

class Lambertian : public BaseMaterial
{
public:
   Lambertian(const FRGBA& albedo) : m_Albedo(albedo) { s_MaterialType = MATERIAL_TYPE::LAMBERTIAN; }
   bool  Scatter(const Ray& rayIn, const HitInfo& hitInfo, FRGBA& attenuation, std::vector<Ray>& scattered) const override;
   MATERIAL_TYPE GetMaterialType() const override { return s_MaterialType; }
private:
   static MATERIAL_TYPE s_MaterialType;
   FRGBA m_Albedo;
};

#endif