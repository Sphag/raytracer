#ifndef __METALIC_H_INCLUDED
#define __METALIC_H_INCLUDED

#include "base_material.h"


class Metalic : public BaseMaterial
{
public:
   Metalic(const FRGBA& albedo, float fuzziness = 0.0f) : m_Albedo(albedo), m_Fuzziness(fuzziness) { s_MaterialType = MATERIAL_TYPE::METALIC; }
   bool  Scatter(const Ray& rayIn, const HitInfo& hitInfo, FRGBA& attenuation, std::vector<Ray>& scattered) const override;
   MATERIAL_TYPE GetMaterialType() const override { return Metalic::s_MaterialType; }
private:
   static MATERIAL_TYPE s_MaterialType;
   FRGBA m_Albedo;
   float m_Fuzziness;
};

#endif
