#include "rtpch.h"
#include <fstream>
#include "core/image.h"

#include "ray_tracer/ray_tracer.h"
#include "objects/scene.h"
#include "materials/lambertian.h"
#include "materials/metalic.h"
#include "materials/dielectric.h"
#include "objects/triangle.h"
#include "objects/plane.h"
#include "objects/quad.h"
#include "objects/model.h"
#include "ray_tracer/intersect_mng.h"

#include <chrono>
#include <textures/constant_texture.h>
#include <textures/checker_texture.h>
#include <textures/image_texture.h>
#include <materials/diffuse_light.h>


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
   std::shared_ptr<Model> model = std::make_shared<Model>();
   model->SetMaterial(std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(FRGBA(0.6, 0.8, 0.8, 1.0f))));
   /*
   scene->Add(std::make_shared<Sphere>(glm::vec3(-1.25, 0, -3), 0.25, std::make_shared<Lambertian>(FRGBA(0.9, 0.3, 0.3, 1.0))));
   scene->Add(std::make_shared<Sphere>(glm::vec3(-0.75, 0, -3), 0.25, std::make_shared<Lambertian>(FRGBA(0.8, 0.3, 0.3, 1.0))));
   scene->Add(sphere);
   scene->Add(std::make_shared<Sphere>(glm::vec3(0.25, 0, -3), 0.25, std::make_shared<Lambertian>(FRGBA(0.0, 0.9, 0.0, 1.0))));
   scene->Add(std::make_shared<Sphere>(glm::vec3(0.75, 0, -3), 0.25, std::make_shared<Lambertian>(FRGBA(0.0, 0.9, 0.0, 1.0))));
   scene->Add(std::make_shared<Sphere>(glm::vec3(1.25, 0, -3), 0.25, std::make_shared<Lambertian>(FRGBA(0.0, 0.9, 0.0, 1.0))));*/
   
   std::shared_ptr<BaseTexture> ct1 = std::make_shared<ConstantTexture>(FRGBA(1.0f, 0.0f, 1.0f, 1.0f));
   std::shared_ptr<BaseTexture> ct2 = std::make_shared<ConstantTexture>(FRGBA(1.0f, 1.0f, 0.0f, 1.0f));
   std::shared_ptr<BaseTexture> cht = std::make_shared<CheckerTexture>(ct1, ct2);
   ImageURGBA tex("earthmap.jpg");
   ImageFRGBA ftex = ToFRGBA(tex);
   std::shared_ptr<BaseTexture> it = std::make_shared<ImageTexture>(ftex);
   std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(glm::vec3(0, 0, -1), 0.5f, std::make_shared<Dielectric>(1.5));
   std::shared_ptr<Sphere> osphere = std::make_shared<Sphere>(glm::vec3(0, 0, 0), 7.f, std::make_shared<Lambertian>(cht));
   scene->Add(osphere);
   scene->Add(sphere);
   scene->Add(std::make_shared<Plane>(glm::vec3(0, 1, 0), glm::vec3(0, -0.5, 0), std::make_shared<Lambertian>(ct1)));
   //scene->Add(std::make_shared<Plane>(glm::vec3(0, 0, 1), glm::vec3(0, 0, -1), std::make_shared<Lambertian>(ct1)));

   auto difflight = std::make_shared<DiffuseLight>(std::make_shared<ConstantTexture>(FRGBA(0.9, 0.6, 0.9, 1.0)), 10.0f);
   scene->Add(std::make_shared<Sphere>(glm::vec3(-0.5, 1.2, -1), 0.7, difflight));
   scene->Add(std::make_shared<Quad>( glm::vec3(0.5, 0, -1.25), glm::vec3(0.5,0, -0.75), glm::vec3(0.5, 0.7, -0.75), glm::vec3(0.5, 0.7, -1.25), difflight));
   HitInfo h;
   //auto quad = std::make_shared<Quad>(glm::vec3(-0.1, -0.1, -1.0), glm::vec3(0.1, -0.1, -1.0), glm::vec3(0.1, 0.1, -1.0), glm::vec3(-0.1, 0.1, -1.0), std::make_shared<Lambertian>(it));
   //quad->Hit({ { 0, 0, 0 }, { 0, 0, -1 } }, 0, INFINITY, h);
   //scene->Add(quad);
   //scene->Construct();
   RayTracer::Init(400, 200);
   RayTracer::SetScene(scene);
   RayTracer::SetSSRate(500);
   RayTracer::SetBounceDepth(3000);
   RayTracer::SetGamma(2);
   RayTracer::SetAmbientLight(FRGBA(0.5, 0.5, 0.5, 1.0));
   
   std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 2, 90.0f, 0.05f, glm::length(sphere->GetCenter()));
   //std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(-0.4f, 1.3f, 0.7f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(-0.3f, 1.0f, 0.0f), 16.0f / 9, 90.0f);
   RayTracer::SetCamera(camera);
   model->Clear();
   model->Load("bunny.obj");
   //scene->Add(model);
   scene->Construct();
   auto start = std::chrono::high_resolution_clock::now();
   auto image = RayTracer::Render();
   auto end = std::chrono::high_resolution_clock::now();

   image.Write("bunny.png");
   std::chrono::duration<float> time = end - start;
   std::cout << time.count() << " sec." << std::endl;
   
   //image.Write("cube.png");
   return 0;
}