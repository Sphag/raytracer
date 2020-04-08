#include "rtpch.h"

#include "image.h"

#include "ray_tracer.h"
#include "hittable_list.h"
#include "lambertian.h"
#include "metalic.h"

int main()
{
   std::shared_ptr<HittableList> hittableList = std::make_shared<HittableList>();
   Sphere sphere(glm::vec3(0, 0, -1), 0.5);
   /*hittableList->Add(std::make_shared<Sphere>(glm::vec3(0, 0, -1), 0.5f));
   hittableList->Add(std::make_shared<Sphere>(glm::vec3(0, -100.5, -1), 100.0f));*/
   hittableList->Add(std::make_shared<Sphere>(glm::vec3(0, 0, -1), 0.5f, std::make_shared<Lambertian>(FRGBA(0.7, 0.3, 0.3, 1.0))));
   hittableList->Add(std::make_shared<Sphere>(glm::vec3(0, -100.5, -1), 100.0f, std::make_shared<Lambertian>(FRGBA(0.8, 0.8, 0.0, 1.0f))));
   hittableList->Add(std::make_shared<Sphere>(glm::vec3(1, 0, -1), 0.5f, std::make_shared<Metalic>(FRGBA(0.8, 0.6, 0.2, 1.0f))));
   hittableList->Add(std::make_shared<Sphere>(glm::vec3(-1, 0, -1), 0.5f, std::make_shared<Metalic>(FRGBA(0.8, 0.8, 0.8, 1.0f))));
   RayTracer::Init(1920, 1080);
   RayTracer::SetScene(hittableList);
   RayTracer::SetSSRate(300);
   std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 16.0f / 9, 90.0f);
   RayTracer::SetCamera(camera);
   auto image = RayTracer::Render();
   image.Write("render.png");
   return 0;
}