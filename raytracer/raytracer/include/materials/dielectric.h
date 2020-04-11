#ifndef __DIELECTIC_H_INCLUDED
#define __DIELECTIC_H_INCLUDED

#include "base_material.h"


class Dielectric : public BaseMaterial
{
public:
   Dielectric(float refractionIdx) : m_RefractionIdx(refractionIdx) { s_MaterialType = MATERIAL_TYPE::DIELECTRIC; }
   bool Scatter(const Ray& rayIn, const HitInfo& hitInfo, FRGBA& attenuation, std::vector<Ray>& scattered) const override;
   MATERIAL_TYPE GetMaterialType() const override { return s_MaterialType; }
private:
   float Schlick(float cosine, float ratio) const;
   MATERIAL_TYPE s_MaterialType;
   float m_RefractionIdx;
};

#endif
