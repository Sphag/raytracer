#include "rtpch.h"
#include "ray_tracer/intersect_mng.h"


INTERSECTS_IMPL(Ray, AABB, {
   glm::vec3 t0 = (rhs.GetCenter() - rhs.GetDim() - lhs.Origin()) / lhs.Direction();
   glm::vec3 t1 = (rhs.GetCenter() + rhs.GetDim() - lhs.Origin()) / lhs.Direction();

   if (t0.x > t1.x) std::swap(t0.x, t1.x);
   if (t0.y > t1.y) std::swap(t0.y, t1.y);
   if (t0.z > t1.z) std::swap(t0.z, t1.z);

   float tmin = glm::compMax(t0);
   float tmax = glm::compMin(t1);

   return tmin <= tmax;
});

INTERSECTS_IMPL(Ray, Box, {
   glm::vec3 t0 = (rhs.GetCenter() - rhs.GetDim() - lhs.Origin()) / lhs.Direction();
   glm::vec3 t1 = (rhs.GetCenter() + rhs.GetDim() - lhs.Origin()) / lhs.Direction();

   if (t0.x > t1.x) std::swap(t0.x, t1.x);
   if (t0.y > t1.y) std::swap(t0.y, t1.y);
   if (t0.z > t1.z) std::swap(t0.z, t1.z);

   float tmin = glm::compMax(t0);
   float tmax = glm::compMin(t1);

   return tmin <= tmax;
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
    float r = glm::abs(glm::dot(lhs.GetDim(), rhs.GetNormal()));
    float s = glm::dot(rhs.GetNormal(), lhs.GetCenter()) - glm::dot(rhs.GetOffset(), rhs.GetNormal());

    return glm::abs(s) <= glm::abs(r);
});

INTERSECTS_IMPL(AABB, Triangle, {
   glm::vec3 f[3];
   f[0] = rhs.B() - rhs.A();
   f[1] = rhs.C() - rhs.B();
   f[2] = rhs.A() - rhs.C();

   glm::vec3 v[3];
   v[0] = rhs.A() - lhs.GetCenter();
   v[1] = rhs.B() - lhs.GetCenter();
   v[2] = rhs.C() - lhs.GetCenter();
   glm::mat3 vv(v[0], v[1], v[2]);

   glm::vec3 axis[13];
   for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) axis[i * 3 + j] = glm::cross(UnitVector(i), f[j]);

   axis[9] = RT_Float3UnitX;
   axis[10] = RT_Float3UnitY;
   axis[11] = RT_Float3UnitZ;
   axis[12] = rhs.GetNormal();

   for (int i = 0; i < 13; i++) {
      float p0 = glm::dot(axis[i], v[0]);
      float p1 = glm::dot(axis[i], v[1]);
      float p2 = glm::dot(axis[i], v[2]);

      glm::vec3 p = axis[i] * vv;

      float r = glm::dot(lhs.GetDim(), glm::abs(axis[i]));

      if (glm::compMin(p) > r || glm::compMax(p) < -r) return false;
   }

   return true; 
});
