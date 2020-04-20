#include "rtpch.h"

#include "core/image.h"

#include "ray_tracer/ray_tracer.h"
#include "objects/scene.h"
#include "materials/lambertian.h"
#include "materials/metalic.h"
#include "materials/dielectric.h"
#include "objects/triangle.h"
#include "objects/plane.h"
#include "objects/model.h"
#include "ray_tracer/intersect_mng.h"



int main()
{
  
   std::shared_ptr<Scene> scene = std::make_shared<Scene>();
   //Sphere sphere(glm::vec3(0, 0, -1), 0.5);
   /*hittableList->Add(std::make_shared<Sphere>(glm::vec3(0, 0, -1), 0.5f, std::make_shared<Dielectric>(1.5f)));
   hittableList->Add(std::make_shared<Sphere>(glm::vec3(0, -100.5, -1), 100.0f, std::make_shared<Lambertian>(FRGBA(0.7, 0.3, 0.3, 1.0))));*/
   //hittableList->Add(std::make_shared<Sphere>(glm::vec3(0, 0, -1), 0.5f, std::make_shared<Metalic>(FRGBA(0.8, 0.6, 0.2, 1.0f), 0.7f)));
   
   //hittableList->Add(std::make_shared<Sphere>(glm::vec3(1, 0, -1), 0.5f, std::make_shared<Metalic>(FRGBA(0.8, 0.6, 0.2, 1.0f), 0.1f)));
   //hittableList->Add(std::make_shared<Sphere>(glm::vec3(-1, 0, -1), 0.5f, std::make_shared<Metalic>(FRGBA(0.8, 0.8, 0.8, 1.0f), 1.0f)));
   //hittableList->Add(std::make_shared<Sphere>(glm::vec3(-1, 0, -1), 0.5f, std::make_shared<Dielectric>(1.66f)));
   //hittableList->Add(std::make_shared<Sphere>(glm::vec3(-1, 0, -1), 0.45f, std::make_shared<Dielectric>(1.66f)));
   //std::shared_ptr<Model> model = std::make_shared<Model>("cube.obj");
   std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(glm::vec3(0, 0, -1), 0.5f, std::make_shared<Lambertian>(FRGBA(0.1, 0.2, 0.8, 1.0f)));
   scene->Add(sphere);
   scene->Add(std::make_shared<Sphere>(glm::vec3(1.5,0, -1), 0.25, std::make_shared<Lambertian>(FRGBA(0.7, 0.3, 0.3, 1.0))));
   scene->Construct();
   RayTracer::Init(960, 540);
   RayTracer::SetScene(scene);
   RayTracer::SetSSRate(50);
   RayTracer::SetBounceDepth(20);
   std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 16.f/9, 90.0f);
   RayTracer::SetCamera(camera);
   auto image = RayTracer::Render();
   image.Write("cube.png");
   
   bool res = IntersectMng::Intersects(AABB({ 0.25, 0.25, 0.25 }, { 0.25, 0.25, 0.25 }), Triangle({ 0,1,0 }, { 1,1,1 }, { 1,1,0 } ));
   return 0;
}