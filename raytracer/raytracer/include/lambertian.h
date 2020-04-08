#ifndef __LAMBERTIAN_H_INCLUDED
#define __LAMBERTIAN_H_INCLUDED

#include <glm/glm.hpp>

#include "base_material.h"

class Lambertian : public BaseMaterial
{
public:
   Lambertian(const FRGBA& albedo) : m_Albedo(albedo) {}
   bool Scatter(const Ray& rayIn, const HitInfo& hitInfo, FRGBA& attenuation, Ray& scattered) const override;
private:
   FRGBA m_Albedo;
};

#endif