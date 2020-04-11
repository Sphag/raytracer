#ifndef __BASE_MATERIAL_H_INCLUDED
#define __BASE_MATERIAL_H_INCLUDED

#include <vector>

#include <glm/glm.hpp>

#include "core/ray.h"


enum class MATERIAL_TYPE : int8_t
{
   UNKNOWN = -1,
   LAMBERTIAN,
   METALIC,
   DIELECTRIC
};

class BaseMaterial
{
public:
   virtual ~BaseMaterial() {}
   virtual bool  Scatter(const Ray& rayIn, const HitInfo& hitInfo, FRGBA& attenuation, std::vector<Ray>& scattered) const = 0;
   virtual MATERIAL_TYPE GetMaterialType() const = 0;
};

#endif