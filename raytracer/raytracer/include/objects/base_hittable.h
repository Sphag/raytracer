#ifndef __BASE_HITTABLE_H_INCLUDED
#define __BASE_HITTABLE_H_INCLUDED

#include "core/ray.h"


class BaseHittable
{
public:
   virtual ~BaseHittable() {}
   virtual bool Hit(const Ray& ray, float minDist, float maxDist, HitInfo& hitInfo) const = 0;
private:
};

#endif
