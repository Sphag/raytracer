#include "rtpch.h"

#include "core/image.h"

#include "ray_tracer/ray_tracer.h"
#include "objects/hittable_list.h"
#include "materials/lambertian.h"
#include "materials/metalic.h"
#include "materials/dielectric.h"
#include "objects/triangle.h"
#include "objects/plane.h"
#include "core/accelerating_structures.h"
#include "objects/model.h"
#include "cds/container/msqueue.h"

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



int planeBoxOverlap(float normal[3], float vert[3], float maxbox[3])	// -NJMP-

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

   if (DOT(normal, vmax) >= 0.0f) return 1;	// -NJMP-



   return 0;

}





/*======================== X-tests ========================*/

#define AXISTEST_X01(a, b, fa, fb)			   \
p0 = a * v0[Y] - b * v0[Z];			       	   \
p2 = a * v2[Y] - b * v2[Z];			       	   \
if (p0 < p2) { min = p0; max = p2; }\
else { min = p2; max = p0; } \
rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z];   \
if (min > rad || max < -rad) return 0;



#define AXISTEST_X2(a, b, fa, fb)			   \
p0 = a * v0[Y] - b * v0[Z];			           \
p1 = a * v1[Y] - b * v1[Z];			       	   \
if (p0 < p1) { min = p0; max = p1; }\
else { min = p1; max = p0; } \
rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z];   \
if (min > rad || max < -rad) return 0;



/*======================== Y-tests ========================*/

#define AXISTEST_Y02(a, b, fa, fb)			   \
p0 = -a * v0[X] + b * v0[Z];		      	   \
p2 = -a * v2[X] + b * v2[Z];	       	       	   \
if (p0 < p2) { min = p0; max = p2; }\
else { min = p2; max = p0; } \
rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z];   \
if (min > rad || max < -rad) return 0;



#define AXISTEST_Y1(a, b, fa, fb)			   \
p0 = -a * v0[X] + b * v0[Z];		      	   \
p1 = -a * v1[X] + b * v1[Z];	     	       	   \
if (p0 < p1) { min = p0; max = p1; }\
else { min = p1; max = p0; } \
rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z];   \
if (min > rad || max < -rad) return 0;



/*======================== Z-tests ========================*/



#define AXISTEST_Z12(a, b, fa, fb)			   \
p1 = a * v1[X] - b * v1[Y];			           \
p2 = a * v2[X] - b * v2[Y];			       	   \
if (p2 < p1) { min = p2; max = p1; }\
else { min = p1; max = p2; } \
rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y];   \
if (min > rad || max < -rad) return 0;



#define AXISTEST_Z0(a, b, fa, fb)			   \
p0 = a * v0[X] - b * v0[Y];				   \
p1 = a * v1[X] - b * v1[Y];			           \
if (p0 < p1) { min = p0; max = p1; }\
else { min = p1; max = p0; } \
rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y];   \
if (min > rad || max < -rad) return 0;



int triBoxOverlap(float boxcenter[3], float boxhalfsize[3], float triverts[3][3])
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



   return 1;   /* box and triangle overlaps */

}


int main()
{
  // cds::Initialize();
 //  cds::gc::HP hpGC;
   // If main thread uses lock-free containers
   // the main thread should be attached to libcds infrastructure
 //  cds::threading::Manager::attachThread();
   // std::shared_ptr<HittableList> hittableList = std::make_shared<HittableList>();
   // Sphere sphere(glm::vec3(0, 0, -1), 0.5);
   // /*hittableList->Add(std::make_shared<Sphere>(glm::vec3(0, 0, -1), 0.5f, std::make_shared<Dielectric>(1.5f)));
   // hittableList->Add(std::make_shared<Sphere>(glm::vec3(0, -100.5, -1), 100.0f, std::make_shared<Lambertian>(FRGBA(0.7, 0.3, 0.3, 1.0))));*/
   // hittableList->Add(std::make_shared<Sphere>(glm::vec3(0, 0, -1), 0.5f, std::make_shared<Metalic>(FRGBA(0.8, 0.6, 0.2, 1.0f), 0.7f)));
   // 
   // hittableList->Add(std::make_shared<Sphere>(glm::vec3(1, 0, -1), 0.5f, std::make_shared<Metalic>(FRGBA(0.8, 0.6, 0.2, 1.0f), 0.1f)));
   // //hittableList->Add(std::make_shared<Sphere>(glm::vec3(-1, 0, -1), 0.5f, std::make_shared<Metalic>(FRGBA(0.8, 0.8, 0.8, 1.0f), 1.0f)));
   // hittableList->Add(std::make_shared<Sphere>(glm::vec3(-1, 0, -1), 0.5f, std::make_shared<Dielectric>(1.66f)));
   // hittableList->Add(std::make_shared<Sphere>(glm::vec3(-1, 0, -1), 0.45f, std::make_shared<Dielectric>(1.66f)));
   // RayTracer::Init(960, 540);
   // RayTracer::SetScene(hittableList);
   // RayTracer::SetSSRate(50);
   // RayTracer::SetBounceDepth(20);
   // std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 16.f/9, 90.0f);
   // RayTracer::SetCamera(camera);
   // auto image = RayTracer::Render();
   // image.Write("render.png");
   
  /* std::shared_ptr<HittableList> list = std::make_shared<HittableList>();
   Model model;
   model.SetMaterial(std::make_shared<Lambertian>(FRGBA(0.23f, 0.23f, 0.63f, 1.0f)));
   model.Load("bunny.obj");
   
   //list->Add(std::make_shared<Model>(model));
   list->Add(std::make_shared<Plane>(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -2.0f, 0.0f), std::make_shared<Lambertian>(FRGBA(0.1f, 0.2f, 0.1f, 1.0f))));
   RayTracer::Init(960, 540);
   RayTracer::SetScene(list);
   RayTracer::SetSSRate(50);
   RayTracer::SetBounceDepth(100);
   //std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(-0.4f, 1.3f, 0.7f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(-0.3f, 1.0f, 0.0f), 16.0f/9, 90.0f);
   std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 16.0f/9, 90.0f);
   RayTracer::SetCamera(camera);
  // auto image = RayTracer::Render();
   //image.Write("render.png");
   cds::container::MSQueue<cds::gc::HP, Triangle> queue;
   queue.enqueue(Triangle(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f)));
   Triangle t;
   queue.dequeue(t);
   cds::Terminate();*/
   return 0;
}