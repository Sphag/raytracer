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
#include "cds/container/msqueue.h"




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
   
  // std::shared_ptr<HittableList> list = std::make_shared<HittableList>();
   Model model;
   //model.SetMaterial(std::make_shared<Lambertian>(FRGBA(0.23f, 0.23f, 0.63f, 1.0f)));
   model.Load("bunny.obj");
   /*
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