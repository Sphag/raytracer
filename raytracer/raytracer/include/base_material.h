#ifndef __BASE_MATERIAL_H_INCLUDED
#define __BASE_MATERIAL_H_INCLUDED

#include <glm/glm.hpp>

#include "ray.h"


class BaseMaterial
{
public:
   virtual ~BaseMaterial() {}
   virtual bool Scatter(const Ray& rayIn, const HitInfo& hitInfo, FRGBA& attenuation, Ray& scattered) const = 0;
};

#endif
