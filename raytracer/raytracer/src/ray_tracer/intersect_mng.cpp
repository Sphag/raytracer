#include "rtpch.h"
#include "ray_tracer/intersect_mng.h"


#include <cmath>
#include <glm/glm.hpp>

inline void findMinMax(float x0, float x1, float x2, float &min, float &max) {
   min = max = x0;
   if (x1 < min)
      min = x1;
   if (x1 > max)
      max = x1;
   if (x2 < min)
      min = x2;
   if (x2 > max)
      max = x2;
}

inline bool planeBoxOverlap(glm::vec3 normal, glm::vec3 vert, glm::vec3 maxbox) {
   glm::vec3 vmin, vmax;
   float v;
   for (size_t q = 0; q < 3; q++) {
      v = vert[q];
      if (normal[q] > 0.0f) {
         vmin[q] = -maxbox[q] - v;
         vmax[q] = maxbox[q] - v;
      }
      else {
         vmin[q] = maxbox[q] - v;
         vmax[q] = -maxbox[q] - v;
      }
   }
   if (glm::dot(normal, vmin) > 0.0f)
      return false;
   if (glm::dot(normal, vmax) >= 0.0f)
      return true;

   return false;
}

/*======================== X-tests ========================*/

inline bool axisTestX01(float a, float b, float fa, float fb, const glm::vec3 &v0,
   const glm::vec3 &v2, const glm::vec3 &boxhalfsize, float &rad, float &min,
   float &max, float &p0, float &p2) {
   p0 = a * v0.y - b * v0.z;
   p2 = a * v2.y - b * v2.z;
   if (p0 < p2) {
      min = p0;
      max = p2;
   }
   else {
      min = p2;
      max = p0;
   }
   rad = fa * boxhalfsize.y + fb * boxhalfsize.z;
   if (min > rad || max < -rad)
      return false;
   return true;
}
inline bool axisTestX2(float a, float b, float fa, float fb, const glm::vec3 &v0,
   const glm::vec3 &v1, const glm::vec3 &boxhalfsize, float &rad, float &min,
   float &max, float &p0, float &p1) {
   p0 = a * v0.y - b * v0.z;
   p1 = a * v1.y - b * v1.z;
   if (p0 < p1) {
      min = p0;
      max = p1;
   }
   else {
      min = p1;
      max = p0;
   }
   rad = fa * boxhalfsize.y + fb * boxhalfsize.z;
   if (min > rad || max < -rad)
      return false;
   return true;
}

/*======================== Y-tests ========================*/

inline bool axisTestY02(float a, float b, float fa, float fb, const glm::vec3 &v0,
   const glm::vec3 &v2, const glm::vec3 &boxhalfsize, float &rad, float &min,
   float &max, float &p0, float &p2) {
   p0 = -a * v0.x + b * v0.z;
   p2 = -a * v2.x + b * v2.z;
   if (p0 < p2) {
      min = p0;
      max = p2;
   }
   else {
      min = p2;
      max = p0;
   }
   rad = fa * boxhalfsize.x + fb * boxhalfsize.z;
   if (min > rad || max < -rad)
      return false;
   return true;
}

inline bool axisTestY1(float a, float b, float fa, float fb, const glm::vec3 &v0,
   const glm::vec3 &v1, const glm::vec3 &boxhalfsize, float &rad, float &min,
   float &max, float &p0, float &p1) {
   p0 = -a * v0.x + b * v0.z;
   p1 = -a * v1.x + b * v1.z;
   if (p0 < p1) {
      min = p0;
      max = p1;
   }
   else {
      min = p1;
      max = p0;
   }
   rad = fa * boxhalfsize.x + fb * boxhalfsize.z;
   if (min > rad || max < -rad)
      return false;
   return true;
}

/*======================== Z-tests ========================*/
inline bool axisTestZ12(float a, float b, float fa, float fb, const glm::vec3 &v1,
   const glm::vec3 &v2, const glm::vec3 &boxhalfsize, float &rad, float &min,
   float &max, float &p1, float &p2) {
   p1 = a * v1.x - b * v1.y;
   p2 = a * v2.x - b * v2.y;
   if (p1 < p2) {
      min = p1;
      max = p2;
   }
   else {
      min = p2;
      max = p1;
   }
   rad = fa * boxhalfsize.x + fb * boxhalfsize.y;
   if (min > rad || max < -rad)
      return false;
   return true;
}

inline bool axisTestZ0(float a, float b, float fa, float fb, const glm::vec3 &v0,
   const glm::vec3 &v1, const glm::vec3 &boxhalfsize, float &rad, float &min,
   float &max, float &p0, float &p1) {
   p0 = a * v0.x - b * v0.y;
   p1 = a * v1.x - b * v1.y;
   if (p0 < p1) {
      min = p0;
      max = p1;
   }
   else {
      min = p1;
      max = p0;
   }
   rad = fa * boxhalfsize.x + fb * boxhalfsize.y;
   if (min > rad || max < -rad)
      return false;
   return true;
}

bool triBoxOverlap(glm::vec3 boxcenter, glm::vec3 boxhalfsize, glm::vec3 tv0, glm::vec3 tv1,
   glm::vec3 tv2) {
   /*    use separating axis theorem to test overlap between triangle and box */
   /*    need to test for overlap in these directions: */
   /*    1) the {x,y,z}-directions (actually, since we use the AABB of the triangle */
   /*       we do not even need to test these) */
   /*    2) normal of the triangle */
   /*    3) crossproduct(edge from tri, {x,y,z}-directin) */
   /*       this gives 3x3=9 more tests */
   glm::vec3 v0, v1, v2;
   float min, max, p0, p1, p2, rad, fex, fey, fez;
   glm::vec3 normal, e0, e1, e2;

   /* This is the fastest branch on Sun */
   /* move everything so that the boxcenter is in (0,0,0) */
   v0 = tv0 - boxcenter;
   v1 = tv1 - boxcenter;
   v2 = tv2 - boxcenter;

   /* compute triangle edges */
   e0 = v1 - v0;
   e1 = v2 - v1;
   e2 = v0 - v2;

   /* Bullet 3:  */
   /*  test the 9 tests first (this was faster) */
   fex = fabsf(e0.x);
   fey = fabsf(e0.y);
   fez = fabsf(e0.z);

   if (!axisTestX01(e0.z, e0.y, fez, fey, v0, v2, boxhalfsize, rad, min, max, p0, p2))
      return false;
   if (!axisTestY02(e0.z, e0.x, fez, fex, v0, v2, boxhalfsize, rad, min, max, p0, p2))
      return false;
   if (!axisTestZ12(e0.y, e0.x, fey, fex, v1, v2, boxhalfsize, rad, min, max, p1, p2))
      return false;

   fex = fabsf(e1.x);
   fey = fabsf(e1.y);
   fez = fabsf(e1.z);

   if (!axisTestX01(e1.z, e1.y, fez, fey, v0, v2, boxhalfsize, rad, min, max, p0, p2))
      return false;
   if (!axisTestY02(e1.z, e1.x, fez, fex, v0, v2, boxhalfsize, rad, min, max, p0, p2))
      return false;
   if (!axisTestZ0(e1.y, e1.x, fey, fex, v0, v1, boxhalfsize, rad, min, max, p0, p1))
      return false;

   fex = fabsf(e2.x);
   fey = fabsf(e2.y);
   fez = fabsf(e2.z);
   if (!axisTestX2(e2.z, e2.y, fez, fey, v0, v1, boxhalfsize, rad, min, max, p0, p1))
      return false;
   if (!axisTestY1(e2.z, e2.x, fez, fex, v0, v1, boxhalfsize, rad, min, max, p0, p1))
      return false;
   if (!axisTestZ12(e2.y, e2.x, fey, fex, v1, v2, boxhalfsize, rad, min, max, p1, p2))
      return false;

   /* Bullet 1: */
   /*  first test overlap in the {x,y,z}-directions */
   /*  find min, max of the triangle each direction, and test for overlap in */
   /*  that direction -- this is equivalent to testing a minimal AABB around */
   /*  the triangle against the AABB */

   /* test in X-direction */
   findMinMax(v0.x, v1.x, v2.x, min, max);
   if (min > boxhalfsize.x || max < -boxhalfsize.x)
      return false;

   /* test in Y-direction */
   findMinMax(v0.y, v1.y, v2.y, min, max);
   if (min > boxhalfsize.y || max < -boxhalfsize.y)
      return false;

   /* test in Z-direction */
   findMinMax(v0.z, v1.z, v2.z, min, max);
   if (min > boxhalfsize.z || max < -boxhalfsize.z)
      return false;

   /* Bullet 2: */
   /*  test if the box intersects the plane of the triangle */
   /*  compute plane equation of triangle: normal*x+d=0 */
   normal = glm::cross(e0, e1);
   if (!planeBoxOverlap(normal, v0, boxhalfsize))
      return false;

   return true; /* box and triangle overlaps */
}




bool IntersectMng::Intersects(const AABB& lhs, const Triangle& rhs)
{
   glm::vec3 min = lhs.GetCenter() - lhs.GetDim();
   glm::vec3 max = lhs.GetCenter() + lhs.GetDim();
   if (glm::all(glm::greaterThanEqual(rhs.A(), min)) && glm::all(glm::lessThanEqual(rhs.A(), max))) return true;
   if (glm::all(glm::greaterThanEqual(rhs.B(), min)) && glm::all(glm::lessThanEqual(rhs.B(), max))) return true;
   if (glm::all(glm::greaterThanEqual(rhs.C(), min)) && glm::all(glm::lessThanEqual(rhs.C(), max))) return true;
   return triBoxOverlap(lhs.GetCenter(), lhs.GetDim(), rhs.A(), rhs.B(), rhs.C());
}

bool IntersectMng::Intersects(const Triangle& lhs, const AABB& rhs)
{
   return IntersectMng::Intersects(rhs, lhs);
}

bool IntersectMng::Intersects(const AABB& lhs, const Plane& rhs)
{
   return false;
}

bool IntersectMng::Intersects(const Plane& lhs, const AABB& rhs)
{
   return IntersectMng::Intersects(rhs, lhs);
}

bool IntersectMng::Intersects(const Triangle& lhs, const Ray& rhs)
{
   HitInfo dummy;
   return lhs.Hit(rhs, 0, INFINITY, dummy);
}

bool IntersectMng::Intersects(const Ray& lhs, const Triangle& rhs)
{
   return IntersectMng::Intersects(rhs, lhs);
}

bool IntersectMng::Intersects(const Sphere& lhs, const Ray& rhs)
{
   HitInfo dummy;
   return lhs.Hit(rhs, 0, INFINITY, dummy);
}

bool IntersectMng::Intersects(const Ray& lhs, const Sphere& rhs)
{
   return IntersectMng::Intersects(rhs, lhs);
}

bool IntersectMng::Intersects(const Scene& lhs, const Ray& rhs)
{
   HitInfo dummy;
   return lhs.Hit(rhs, 0, INFINITY, dummy);
}

bool IntersectMng::Intersects(const Ray& lhs, const Scene& rhs)
{
   return IntersectMng::Intersects(rhs, lhs);
}

bool IntersectMng::Intersects(const Model& lhs, const Ray& rhs)
{
   HitInfo dummy;
   return lhs.Hit(rhs, 0, INFINITY, dummy);
}

bool IntersectMng::Intersects(const Ray& lhs, const Model& rhs)
{
   return IntersectMng::Intersects(rhs, lhs);
}

bool IntersectMng::Intersects(const Box& lhs, const Ray& rhs)
{
   glm::vec3 t0 = (lhs.GetCenter() - lhs.GetDim() - rhs.Origin()) / rhs.Direction();
   glm::vec3 t1 = (lhs.GetCenter() + lhs.GetDim() - rhs.Origin()) / rhs.Direction();

   if (t0.x > t1.x) std::swap(t0.x, t1.x);
   if (t0.y > t1.y) std::swap(t0.y, t1.y);
   if (t0.z > t1.z) std::swap(t0.z, t1.z);

   float tmin = glm::compMax(t0);
   float tmax = glm::compMin(t1);

   return tmin <= tmax;
}

bool IntersectMng::Intersects(const Ray& lhs, const Box& rhs)
{
   return IntersectMng::Intersects(rhs, lhs);
}

bool IntersectMng::Intersects(const Plane& lhs, const Ray& rhs)
{
   HitInfo dummy;
   return lhs.Hit(rhs, 0.0f, INFINITY, dummy);
}

bool IntersectMng::Intersects(const Ray& lhs, const Plane& rhs)
{
   return IntersectMng::Intersects(rhs, lhs);
}

bool IntersectMng::Intersects(const AABB& lhs, const Ray& rhs)
{
   glm::vec3 min = lhs.GetCenter() - lhs.GetDim();
   glm::vec3 max = lhs.GetCenter() + lhs.GetDim();
   float t0 = glm::min(min.x - rhs.Origin().x, max.x - rhs.Origin().x / rhs.Direction().x);
   float t1 = glm::max(min.x - rhs.Origin().x, max.x - rhs.Origin().x / rhs.Direction().x);
   if (t1 < t0)
      return false;

   t0 = glm::min(min.y - rhs.Origin().y, max.y - rhs.Origin().y / rhs.Direction().y);
   t1 = glm::max(min.y - rhs.Origin().y, max.y - rhs.Origin().y / rhs.Direction().y);
   if (t1 < t0)
      return false;

   t0 = glm::min(min.z - rhs.Origin().z, max.z - rhs.Origin().z / rhs.Direction().z);
   t1 = glm::max(min.z - rhs.Origin().z, max.z - rhs.Origin().z / rhs.Direction().z);
   if (t1 < t0)
      return false;
   
   return true;
}

bool IntersectMng::Intersects(const Ray& lhs, const AABB& rhs)
{
   return IntersectMng::Intersects(rhs, lhs);
}
