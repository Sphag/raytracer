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

#include <chrono>



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
   std::shared_ptr<Model> model = std::make_shared<Model>("bunny.obj");
   model->SetMaterial(std::make_shared<Lambertian>(FRGBA(0.6, 0.8, 0.8, 1.0f)));
   /*std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(glm::vec3(-0.25, 0, -3), 0.25f, std::make_shared<Lambertian>(FRGBA(0.0, 0.0, 0.8, 1.0f)));

   scene->Add(std::make_shared<Sphere>(glm::vec3(-1.25, 0, -3), 0.25, std::make_shared<Lambertian>(FRGBA(0.9, 0.3, 0.3, 1.0))));
   scene->Add(std::make_shared<Sphere>(glm::vec3(-0.75, 0, -3), 0.25, std::make_shared<Lambertian>(FRGBA(0.8, 0.3, 0.3, 1.0))));
   scene->Add(sphere);
   scene->Add(std::make_shared<Sphere>(glm::vec3(0.25, 0, -3), 0.25, std::make_shared<Lambertian>(FRGBA(0.0, 0.9, 0.0, 1.0))));
   scene->Add(std::make_shared<Sphere>(glm::vec3(0.75, 0, -3), 0.25, std::make_shared<Lambertian>(FRGBA(0.0, 0.9, 0.0, 1.0))));
   scene->Add(std::make_shared<Sphere>(glm::vec3(1.25, 0, -3), 0.25, std::make_shared<Lambertian>(FRGBA(0.0, 0.9, 0.0, 1.0))));*/
   scene->Add(model);
   scene->Construct();
   RayTracer::Init(240, 135);
   RayTracer::SetScene(scene);
   RayTracer::SetSSRate(10);
   RayTracer::SetBounceDepth(5);
   //std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 16.f/9, 90.0f);
   std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(-0.4f, 1.3f, 0.7f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(-0.3f, 1.0f, 0.0f), 16.0f / 9, 90.0f);
   RayTracer::SetCamera(camera);
   auto start = std::chrono::high_resolution_clock::now();
   auto image = RayTracer::Render();
   auto end = std::chrono::high_resolution_clock::now();
   image.Write("cube.png");
   std::chrono::duration<float> time = end - start;
   std::cout << std::endl << std::endl << time.count() << " sec.";
   return 0;
}