#ifndef __METALIC_H_INCLUDED
#define __METALIC_H_INCLUDED

#include "base_material.h"


class Metalic : public BaseMaterial
{
public:
   Metalic(const FRGBA& albedo, float fuzziness = 0.0f) : m_Albedo(albedo), m_Fuzziness(fuzziness) {}
   bool Scatter(const Ray& rayIn, const HitInfo& hitInfo, FRGBA& attenuation, Ray& scattered) const override;
private:
   FRGBA m_Albedo;
   float m_Fuzziness;
};

#endif
