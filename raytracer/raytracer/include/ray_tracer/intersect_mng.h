#ifndef __INTERSECT_MNG_INCLUDED
#define __INTERSECT_MNG_INCLUDED

#include "ray_tracer/ray.h"
#include "ray_tracer/aabb.h"

#include "objects/box.h"
#include "objects/plane.h"
#include "objects/model.h"
#include "objects/scene.h"
#include "objects/sphere.h"
#include "objects/triangle.h"


#define __INTERSECTS_DECL(Type1, Type2) bool Intersects(const Type1& lhs, const Type2& rhs)
#define __INTERSECTS_IMPL(Type1, Type2) bool IntersectMng::Intersects(const Type1& lhs, const Type2& rhs) 
#define __STATIC_INTERSECTS_DECL(Type1, Type2) static __INTERSECTS_DECL(Type1, Type2)
#define INTERSECTS_DECL(Type1, Type2) __STATIC_INTERSECTS_DECL(Type1, Type2);\
                                 __STATIC_INTERSECTS_DECL(Type2, Type1)

#define INTERSECTS_IMPL(Type1, Type2, code) __INTERSECTS_IMPL(Type1, Type2)\
 code \
 __INTERSECTS_IMPL(Type2, Type1)\
 { return Intersects(rhs, lhs);}

class IntersectMng
{
public:
   INTERSECTS_DECL(Ray, AABB);
   INTERSECTS_DECL(Ray, Box);
   INTERSECTS_DECL(Ray, Plane);
   INTERSECTS_DECL(Ray, Model);
   INTERSECTS_DECL(Ray, Scene);
   INTERSECTS_DECL(Ray, Sphere);
   INTERSECTS_DECL(Ray, Triangle);

   INTERSECTS_DECL(AABB, Plane);
   INTERSECTS_DECL(AABB, Triangle);

   void a(Scene s)
   {
      
   }
};


#endif
