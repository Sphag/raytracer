#include "rtpch.h"

#include "image.h"

#include "ray_tracer.h"
#include "hittable_list.h"
#include "lambertian.h"
#include "metalic.h"
#include "dielectric.h"
#include "triangle.h"
#include "model.h"

int main()
{
   // std::shared_ptr<HittableList> hittableList = std::make_shared<HittableList>();
   // Sphere sphere(glm::vec3(0, 0, -1), 0.5);
   // /*hittableList->Add(std::make_shared<Sphere>(glm::vec3(0, 0, -1), 0.5f, std::make_shared<Dielectric>(1.5f)));
   // hittableList->Add(std::make_shared<Sphere>(glm::vec3(0, -100.5, -1), 100.0f, std::make_shared<Lambertian>(FRGBA(0.7, 0.3, 0.3, 1.0))));*/
   // hittableList->Add(std::make_shared<Sphere>(glm::vec3(0, 0, -1), 0.5f, std::make_shared<Metalic>(FRGBA(0.8, 0.6, 0.2, 1.0f), 0.7f)));
   // hittableList->Add(std::make_shared<Sphere>(glm::vec3(0, -100.5, -1), 100.0f, std::make_shared<Lambertian>(FRGBA(0.8, 0.8, 0.0, 1.0f))));
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
   
   Model model;
   model.SetMaterial(std::make_shared<Lambertian>(FRGBA(0.23f, 0.23f, 0.63f, 1.0f)));
   model.Load("bunny.obj");

   std::shared_ptr<HittableList> list = std::make_shared<HittableList>();
   list->Add(std::make_shared<Model>(model));
   RayTracer::Init(1920, 1080);
   RayTracer::SetScene(list);
   RayTracer::SetSSRate(50);
   RayTracer::SetBounceDepth(100);
   std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(-0.4f, 1.3f, 0.7f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(-0.3f, 1.0f, 0.0f), 16.0f/9, 90.0f);
   RayTracer::SetCamera(camera);
   auto image = RayTracer::Render();
   image.Write("bunny.png");
   return 0;
}