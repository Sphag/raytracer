#include "rtpch.h"
#include "ray_tracer/intersect_mng.h"



#define X 0

#define Y 1

#define Z 2



#define CROSS(dest,v1,v2) \
dest[0] = v1[1] * v2[2] - v1[2] * v2[1]; \
dest[1] = v1[2] * v2[0] - v1[0] * v2[2]; \
dest[2] = v1[0] * v2[1] - v1[1] * v2[0];



#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])



#define SUB(dest,v1,v2) \
dest[0] = v1[0] - v2[0]; \
dest[1] = v1[1] - v2[1]; \
dest[2] = v1[2] - v2[2];



#define FINDMINMAX(x0,x1,x2,min,max) \
min = max = x0;   \
if (x1 < min) min = x1; \
   if (x1 > max) max = x1; \
      if (x2 < min) min = x2; \
         if (x2 > max) max = x2;



int planeBoxOverlap(float normal[3], float vert[3], const float maxbox[3])	// -NJMP-

{

   int q;

   float vmin[3], vmax[3], v;

   for (q = X; q <= Z; q++)

   {

      v = vert[q];					// -NJMP-

      if (normal[q] > 0.0f)

      {

         vmin[q] = -maxbox[q] - v;	// -NJMP-

         vmax[q] = maxbox[q] - v;	// -NJMP-

      }

      else

      {

         vmin[q] = maxbox[q] - v;	// -NJMP-

         vmax[q] = -maxbox[q] - v;	// -NJMP-

      }

   }

   if (DOT(normal, vmin) > 0.0f) return 0;	// -NJMP-

   if (DOT(normal, vmax) > 0.0f) return 1;	// -NJMP-



   return 0;

}





/*======================== X-tests ========================*/

#define AXISTEST_X01(a, b, fa, fb)			   \
p0 = a * v0[Y] - b * v0[Z];			       	   \
p2 = a * v2[Y] - b * v2[Z];			       	   \
if (p0 < p2) { min = p0; max = p2; }else { min = p2; max = p0; } \
rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z];   \
if (min > rad || max < -rad) return 0;



#define AXISTEST_X2(a, b, fa, fb)			   \
p0 = a * v0[Y] - b * v0[Z];			           \
p1 = a * v1[Y] - b * v1[Z];			       	   \
if (p0 < p1) { min = p0; max = p1; }else { min = p1; max = p0; } \
rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z];   \
if (min > rad || max < -rad) return 0;



/*======================== Y-tests ========================*/

#define AXISTEST_Y02(a, b, fa, fb)			   \
p0 = -a * v0[X] + b * v0[Z];		      	   \
p2 = -a * v2[X] + b * v2[Z];	       	       	   \
if (p0 < p2) { min = p0; max = p2; }else { min = p2; max = p0; } \
rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z];   \
if (min > rad || max < -rad) return 0;



#define AXISTEST_Y1(a, b, fa, fb)			   \
p0 = -a * v0[X] + b * v0[Z];		      	   \
p1 = -a * v1[X] + b * v1[Z];	     	       	   \
if (p0 < p1) { min = p0; max = p1; }else { min = p1; max = p0; } \
rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z];   \
if (min > rad || max < -rad) return 0;



/*======================== Z-tests ========================*/



#define AXISTEST_Z12(a, b, fa, fb)			   \
p1 = a * v1[X] - b * v1[Y];			           \
p2 = a * v2[X] - b * v2[Y];			       	   \
if (p2 < p1) { min = p2; max = p1; }else { min = p1; max = p2; } \
rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y];   \
if (min > rad || max < -rad) return 0;



#define AXISTEST_Z0(a, b, fa, fb)			   \
p0 = a * v0[X] - b * v0[Y];				   \
p1 = a * v1[X] - b * v1[Y];			           \
if (p0 < p1) { min = p0; max = p1; }\
else { min = p1; max = p0; } \
rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y];   \
if (min > rad || max < -rad) return 0;



int triBoxOverlap(const float boxcenter[3], const float boxhalfsize[3], const float triverts[3][3])

{



   /*    use separating axis theorem to test overlap between triangle and box */

   /*    need to test for overlap in these directions: */

   /*    1) the {x,y,z}-directions (actually, since we use the AABB of the triangle */

   /*       we do not even need to test these) */

   /*    2) normal of the triangle */

   /*    3) crossproduct(edge from tri, {x,y,z}-directin) */

   /*       this gives 3x3=9 more tests */

   float v0[3], v1[3], v2[3];

   //   float axis[3];

   float min, max, p0, p1, p2, rad, fex, fey, fez;		// -NJMP- "d" local variable removed

   float normal[3], e0[3], e1[3], e2[3];



   /* This is the fastest branch on Sun */

   /* move everything so that the boxcenter is in (0,0,0) */

   SUB(v0, triverts[0], boxcenter);

   SUB(v1, triverts[1], boxcenter);

   SUB(v2, triverts[2], boxcenter);



   /* compute triangle edges */

   SUB(e0, v1, v0);      /* tri edge 0 */

   SUB(e1, v2, v1);      /* tri edge 1 */

   SUB(e2, v0, v2);      /* tri edge 2 */



   /* Bullet 3:  */

   /*  test the 9 tests first (this was faster) */

   fex = fabsf(e0[X]);

   fey = fabsf(e0[Y]);

   fez = fabsf(e0[Z]);

   AXISTEST_X01(e0[Z], e0[Y], fez, fey);

   AXISTEST_Y02(e0[Z], e0[X], fez, fex);

   AXISTEST_Z12(e0[Y], e0[X], fey, fex);



   fex = fabsf(e1[X]);

   fey = fabsf(e1[Y]);

   fez = fabsf(e1[Z]);

   AXISTEST_X01(e1[Z], e1[Y], fez, fey);

   AXISTEST_Y02(e1[Z], e1[X], fez, fex);

   AXISTEST_Z0(e1[Y], e1[X], fey, fex);



   fex = fabsf(e2[X]);

   fey = fabsf(e2[Y]);

   fez = fabsf(e2[Z]);

   AXISTEST_X2(e2[Z], e2[Y], fez, fey);

   AXISTEST_Y1(e2[Z], e2[X], fez, fex);

   AXISTEST_Z12(e2[Y], e2[X], fey, fex);



   /* Bullet 1: */

   /*  first test overlap in the {x,y,z}-directions */

   /*  find min, max of the triangle each direction, and test for overlap in */

   /*  that direction -- this is equivalent to testing a minimal AABB around */

   /*  the triangle against the AABB */



   /* test in X-direction */

   FINDMINMAX(v0[X], v1[X], v2[X], min, max);

   if (min > boxhalfsize[X] || max < -boxhalfsize[X]) return 0;



   /* test in Y-direction */

   FINDMINMAX(v0[Y], v1[Y], v2[Y], min, max);

   if (min > boxhalfsize[Y] || max < -boxhalfsize[Y]) return 0;



   /* test in Z-direction */

   FINDMINMAX(v0[Z], v1[Z], v2[Z], min, max);

   if (min > boxhalfsize[Z] || max < -boxhalfsize[Z]) return 0;



   /* Bullet 2: */

   /*  test if the box intersects the plane of the triangle */

   /*  compute plane equation of triangle: normal*x+d=0 */

   CROSS(normal, e0, e1);

   // -NJMP- (line removed here)

   if (!planeBoxOverlap(normal, v0, boxhalfsize)) return 0;	// -NJMP-

}



bool IntersectMng::Intersects(const AABB& lhs, const Triangle& rhs)
{
   float verts[3][3] = {
      rhs.A().x, rhs.A().y, rhs.A().z,
      rhs.B().x, rhs.B().y, rhs.B().z,
      rhs.C().x, rhs.C().y, rhs.C().z,
   };
   return triBoxOverlap(glm::value_ptr(lhs.GetCenter()), glm::value_ptr(lhs.GetCenter()), verts);
   glm::vec3 min = lhs.GetCenter() - lhs.GetDim();
   glm::vec3 max = lhs.GetCenter() + lhs.GetDim();
   if (glm::all(glm::greaterThanEqual(rhs.A(), min)) && glm::all(glm::lessThanEqual(rhs.A(), max))) return true;
   if (glm::all(glm::greaterThanEqual(rhs.B(), min)) && glm::all(glm::lessThanEqual(rhs.B(), max))) return true;
   if (glm::all(glm::greaterThanEqual(rhs.C(), min)) && glm::all(glm::lessThanEqual(rhs.C(), max))) return true;


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
   float tmin = (min.x - rhs.Origin().x) / rhs.Direction().x;
   float tmax = (max.x - rhs.Origin().x) / rhs.Direction().x;

   if (tmin > tmax) std::swap(tmin, tmax);

   float tymin = (min.y - rhs.Origin().y) / rhs.Direction().y;
   float tymax = (max.y - rhs.Origin().y) / rhs.Direction().y;

   if (tymin > tymax) std::swap(tymin, tymax);

   if ((tmin > tymax) || (tymin > tmax))
      return false;

   if (tymin > tmin)
      tmin = tymin;

   if (tymax < tmax)
      tmax = tymax;

   float tzmin = (min.z - rhs.Origin().z) / rhs.Direction().z;
   float tzmax = (max.z - rhs.Origin().z) / rhs.Direction().z;

   if (tzmin > tzmax) std::swap(tzmin, tzmax);

   if ((tmin > tzmax) || (tzmin > tmax))
      return false;

   if (tzmin > tmin)
      tmin = tzmin;

   if (tzmax < tmax)
      tmax = tzmax;

   return true;
}

bool IntersectMng::Intersects(const Ray& lhs, const AABB& rhs)
{
   return IntersectMng::Intersects(rhs, lhs);
}
