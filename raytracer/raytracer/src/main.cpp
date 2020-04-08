#include "rtpch.h"

#include "image.h"

#include "ray_tracer.h"
#include "hittable_list.h"

int main()
{
   std::shared_ptr<HittableList> hittableList = std::make_shared<HittableList>();
   Sphere sphere(glm::vec3(0, 0, -1), 0.5);
   hittableList->Add(std::make_shared<Sphere>(glm::vec3(0, 0, -1), 0.5f));
   hittableList->Add(std::make_shared<Sphere>(glm::vec3(0, -100.5, -1), 100));
   RayTracer::Init(200, 100);
   RayTracer::SetScene(hittableList);
   Camera cam;
   RayTracer::SetSSRate(100);
   RayTracer::SetCamera(cam);
   auto image = RayTracer::Render();
   image.Write("render.png");
   return 0;
}