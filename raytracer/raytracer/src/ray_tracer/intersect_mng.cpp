#include "rtpch.h"
#include "ray_tracer/intersect_mng.h"


INTERSECTS_IMPL(Ray, AABB, {
   return true; 
});

INTERSECTS_IMPL(Ray, Box, {
   return true;
});

INTERSECTS_IMPL(Ray, Plane, {
   HitInfo dummy;
   return rhs.Hit(lhs, 0.0f, INFINITY, dummy);
});

INTERSECTS_IMPL(Ray, Scene, {
   HitInfo dummy;
   return rhs.Hit(lhs, 0.0f, INFINITY, dummy);
});

INTERSECTS_IMPL(Ray, Model, {
   HitInfo dummy;
   return rhs.Hit(lhs, 0.0f, INFINITY, dummy);
});

INTERSECTS_IMPL(Ray, Sphere, {
   HitInfo dummy;
   return rhs.Hit(lhs, 0.0f, INFINITY, dummy);
});

INTERSECTS_IMPL(Ray, Triangle, {
   HitInfo dummy;
   return rhs.Hit(lhs, 0.0f, INFINITY, dummy);
});

INTERSECTS_IMPL(AABB, Plane, {
   return true;
});

INTERSECTS_IMPL(AABB, Triangle, {
   return true;
});
