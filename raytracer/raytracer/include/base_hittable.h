#ifndef __BASE_HITTABLE_H_INCLUDED
#define __BASE_HITTABLE_H_INCLUDED

#include "ray.h"


class BaseHittable
{
public:
   virtual ~BaseHittable() {}
   virtual bool Hit(const Ray& ray, float& t) const = 0;
};

#endif
