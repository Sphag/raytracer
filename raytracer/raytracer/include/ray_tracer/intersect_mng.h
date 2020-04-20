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





class IntersectMng
{
public:
   static bool Intersects(const Ray&, const AABB&);
   static bool Intersects(const AABB&, const Ray&);

   static bool Intersects(const Ray&, const Plane&);
   static bool Intersects(const Plane&, const Ray&);

   static bool Intersects(const Ray&, const Box&);
   static bool Intersects(const Box&, const Ray&);

   static bool Intersects(const Ray&, const Model&);
   static bool Intersects(const Model&, const Ray&);

   static bool Intersects(const Ray&, const Scene&);
   static bool Intersects(const Scene&, const Ray&);

   static bool Intersects(const Ray&, const Sphere&);
   static bool Intersects(const Sphere&, const Ray&);

   static bool Intersects(const Ray&, const Triangle&);
   static bool Intersects(const Triangle&, const Ray&);

   static bool Intersects(const Plane&, const AABB&);
   static bool Intersects(const AABB&, const Plane&);

   static bool Intersects(const Triangle&, const AABB&);
   static bool Intersects(const AABB&, const Triangle&);
};


#endif
