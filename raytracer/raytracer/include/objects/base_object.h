#ifndef __BASE_OBJECT_H_INCLUDED
#define __BASE_OBJECT_H_INCLUDED

#include "ray_tracer/ray.h"


class BaseObject
{
public:
   virtual ~BaseObject() {}
   virtual bool Hit(const Ray& ray, float minDist, float maxDist, HitInfo& hitInfo) const = 0;
private:
};

#endif
