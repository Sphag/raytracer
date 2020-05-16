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


std::array<std::shared_ptr<Quad>, 6> GetBox(glm::vec3 center, glm::vec3 dim, std::shared_ptr<BaseMaterial> material)
{
   std::array<std::shared_ptr<Quad>, 6> box;
   glm::vec3 min = center - dim, max = center + dim;
   dim *= 2.0f;
   glm::vec3 x = { 1, 0, 0 }, y = { 0, 1, 0 }, z = { 0, 0, 1 };

   // floor
   box[0] = std::make_shared<Quad>(
      min,
      min + dim.z * z,
      min + dim.z * z + dim.x * x,
      min + dim.x * x,
      material
   );

   // ceiling
   box[1] = std::make_shared<Quad>(
      min + dim.y * y,
      min + dim.y * y + dim.z * z,
      min + dim.y * y + dim.z * z + dim.x * x,
      min + dim.y * y + dim.x * x,
      material
   );

   // back wall
   box[2] = std::make_shared<Quad>(
      min,
      min + dim.y * y,
      min + dim.x * x + dim.y * y,
      min + dim.x * x,
      material
   );

   // front wall
   box[3] = std::make_shared<Quad>(
      min + dim.z * z,
      min + dim.z * z + dim.x * x,
      min + dim.z * z + dim.x * x + dim.y * y,
      min + dim.z * z + dim.y * y,
      material
   );

   // left wall
   box[4] = std::make_shared<Quad>(
      min,
      min + dim.z * z,
      min + dim.z * z + dim.y * y,
      min + dim.y * y,
      material
   );

   // right wall
   box[5] = std::make_shared<Quad>(
      min + dim.x * x,
      min + dim.x * x + dim.y * y,
      min + dim.x * x + dim.z * z + dim.y * y,
      min + dim.x * x + dim.z * z,
      material
   );

   return box;
}

std::shared_ptr<Scene> GetCornell()
{
   std::shared_ptr<Scene> scene = std::make_shared<Scene>();

   float d = 0.55f;
   float h = 0.75f;
   float radius = 0.2f * d;

   glm::vec3 x = { 1.0f, 0.0f, 0.0f }, y = { 0.0f, 1.0f, 0.0f }, z = { 0.0f, 0.0f, 1.0f };
   glm::vec3 start = { -d/2, -h/2, -d/2 };
   glm::vec3 middle = start + 0.5f * d * x + 0.5f * d * z + 0.5f * h * y;

   ImageURGBA tex("earthmap.png");
   ImageFRGBA ftex = ToFRGBA(tex);

   std::shared_ptr<BaseTexture> tGray = std::make_shared<ConstantTexture>(FRGBA(0.5f, 0.5f, 0.5f, 1.0f));
   std::shared_ptr<BaseTexture> tEarth = std::make_shared<ImageTexture>(ftex);
   std::shared_ptr<BaseTexture> tLightBlue = std::make_shared<ConstantTexture>(FRGBA(0.7f, 0.8f, 0.9f, 1.0f));
   std::shared_ptr<BaseTexture> tLGray = std::make_shared<ConstantTexture>(FRGBA(0.7f, 0.7f, 0.7f, 1.0f));
   std::shared_ptr<BaseTexture> tBlack = std::make_shared<ConstantTexture>(FRGBA(0.1f, 0.1f, 0.1f, 1.0f));
   std::shared_ptr<BaseTexture> tWhite = std::make_shared<ConstantTexture>(FRGBA(0.9f, 0.9f, 0.9f, 1.0f));
   std::shared_ptr<BaseTexture> tGreen = std::make_shared<ConstantTexture>(FRGBA(0.0f, 1.0f, 0.0f, 1.0f));
   std::shared_ptr<BaseTexture> tBlue = std::make_shared<ConstantTexture>(FRGBA(0.0f, 0.0f, 1.0f, 1.0f));
   std::shared_ptr<BaseTexture> tRed = std::make_shared<ConstantTexture>(FRGBA(1.0f, 0.0f, 0.0f, 1.0f));

   std::shared_ptr<BaseMaterial> mGray = std::make_shared<Lambertian>(tGray);
   std::shared_ptr<BaseMaterial> mEarth = std::make_shared<Lambertian>(tEarth);
   std::shared_ptr<BaseMaterial> mBunny = std::make_shared<Lambertian>(tLightBlue);
   std::shared_ptr<BaseMaterial> mLGray = std::make_shared<Lambertian>(tLGray);
   std::shared_ptr<BaseMaterial> mBlack = std::make_shared<Lambertian>(tBlack);
   std::shared_ptr<BaseMaterial> mWhite = std::make_shared<Lambertian>(tWhite);
   std::shared_ptr<BaseMaterial> mGreen = std::make_shared<Lambertian>(tGreen);
   std::shared_ptr<BaseMaterial> mRed = std::make_shared<Lambertian>(tRed);
   std::shared_ptr<BaseMaterial> mMetal = std::make_shared<Metalic>(tWhite, 0.0f);
   std::shared_ptr<BaseMaterial> mGlass = std::make_shared<Dielectric>(tWhite, 1.6f);

   std::shared_ptr<BaseMaterial> lWhite = std::make_shared<DiffuseLight>(tWhite, 60.0f);
   
   std::shared_ptr<Model> bunny = std::make_shared<Model>("new_bunny.obj");
   bunny->SetMaterial(mBunny);
   glm::vec3 b2c = start + 0.4f * d * x + 0.5f * d * z + radius * y;
   glm::vec3 b1c = start + 0.65f * d * x + d * z + 0.5f * radius * y;
   bunny->SetTransform(glm::vec3(0.09f, -0.21f, 0.26f), glm::vec3(0.0f), glm::vec3(0.7f));
   //scene->Add(bunny);

   auto box1 = GetBox(glm::vec3(0.0f), glm::vec3(0.75f * radius, 0.5f * radius, 0.75f * radius), mLGray);
   for (auto& q : box1) {
      q->SetTransform(start + 0.65f * d * x + d * z + 0.5f * radius * y, glm::vec3(0.0f, glm::radians(-25.0f), 0.0f), glm::vec3(1.0f));
      scene->Add(q);
   }

   auto box2 = GetBox(glm::vec3(0.0f), glm::vec3(0.75f * radius, radius, 0.75f * radius), mLGray);
   for (auto& q : box2) {
      q->SetTransform(start + 0.4f * d * x + 0.5f * d * z + radius * y, glm::vec3(0.0f, glm::radians(15.0f), 0.0f), glm::vec3(1.0f));
      scene->Add(q);
   }

   std::shared_ptr<Quad> floor = std::make_shared<Quad>(
      start,
      start + (d + 1) * z,
      start + (d + 1) * z + d * x,
      start + d * x,
      mGray
   );
   scene->Add(floor);

   std::shared_ptr<Quad> ceiling = std::make_shared<Quad>(
      start + h * y,
      start + h * y + d * x,
      start + h * y + (d + 1) * z + d * x,
      start + h * y + (d + 1) * z,
      mGray
   );
   scene->Add(ceiling);

   std::shared_ptr<Quad> rWall = std::make_shared<Quad>(
      start + d * x,
      start + d * x + (d + 1) * z,
      start + d * x + (d + 1) * z + h * y,
      start + d * x + h * y,
      mGreen
   );
   scene->Add(rWall);

   std::shared_ptr<Quad> bWall = std::make_shared<Quad>(
      start,
      start + d * x,
      start + d * x + h * y,
      start + h * y,
      mGray
   );
   scene->Add(bWall);

   std::shared_ptr<Quad> lWall = std::make_shared<Quad>(
      start,
      start + h * y,
      start + h * y + (d + 1) * z,
      start + (d + 1) * z,
      mRed
   );
   scene->Add(lWall);

   float dd = d * 0.35f;
   glm::vec3 mid = start + (h - 0.01f)*y + 0.5f * d * x + 0.5f * d * z;
   std::shared_ptr<Quad> light = std::make_shared<Quad>(
      mid - 0.5f * dd * x - 0.5f * dd * z,
      mid - 0.5f * dd * x + 0.5f * dd * z,
      mid + 0.5f * dd * x + 0.5f * dd * z,
      mid + 0.5f * dd * x - 0.5f * dd * z,
      lWhite
   );
   scene->Add(light);

   std::shared_ptr<Quad> frontWall = std::make_shared<Quad>(
      start + (d + 1) * z,
      start + (d + 1) * z + h * y,
      start + (d + 1) * z + d * x + h * y,
      start + (d + 1) * z + d * x,
      mGray
   );
   scene->Add(frontWall);


   std::shared_ptr<Sphere> metalSphere = std::make_shared<Sphere>(
      b2c + 2.0f * radius * y,
      radius,
      mMetal
   );
   scene->Add(metalSphere);

   float gRadius = 0.5f * radius;
   std::shared_ptr<Sphere> glassSphere = std::make_shared<Sphere>(
      start + 0.3f * d * x + (d + 0.1f) * z + radius * y,
      gRadius,
      mGlass
   );
   scene->Add(glassSphere);

   std::shared_ptr<Sphere> earthSphere = std::make_shared<Sphere>(
      b1c + (0.5f * radius + gRadius) * y,
      gRadius,
      mEarth
   );
   scene->Add(earthSphere);

   scene->Construct();

   return scene;
}

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
   std::shared_ptr<BaseTexture> ct3 = std::make_shared<ConstantTexture>(FRGBA(0.7, 0.7, 0.7, 1.0));
   std::shared_ptr<BaseTexture> cct = std::make_shared<ConstantTexture>(FRGBA(0.0, 0.3, 0.0, 1.0));
   std::shared_ptr<BaseTexture> cw = std::make_shared<ConstantTexture>(RT_FWHITE);
   std::shared_ptr<BaseTexture> ct4 = std::make_shared<ConstantTexture>(RT_FBLUE);
   std::shared_ptr<BaseTexture> ct6 = std::make_shared<ConstantTexture>(RT_FRED);
   std::shared_ptr<BaseTexture> ct5 = std::make_shared<ConstantTexture>(RT_FGREEN);
   std::shared_ptr<BaseTexture> ct2 = std::make_shared<ConstantTexture>(FRGBA(1.0f, 1.0f, 0.0f, 1.0f));
   std::shared_ptr<BaseTexture> cht = std::make_shared<CheckerTexture>(ct1, ct2);
   ImageURGBA tex("earthmap.jpg");
   ImageFRGBA ftex = ToFRGBA(tex);
   std::shared_ptr<BaseTexture> it = std::make_shared<ImageTexture>(ftex);
   std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(glm::vec3(0, 0, -1), 0.5f, std::make_shared<Dielectric>(cw, 1.6));
   std::shared_ptr<Sphere> sphere1 = std::make_shared<Sphere>(glm::vec3(-1, 0, -1), 0.5f, std::make_shared<Metalic>(ct3, 0.0));
   std::shared_ptr<Sphere> sphere2 = std::make_shared<Sphere>(glm::vec3(0.5, 0, -1), 0.5f, std::make_shared<Metalic>(ct3, 0.0));
   std::shared_ptr<Sphere> osphere = std::make_shared<Sphere>(glm::vec3(0, 0, 0), 7.f, std::make_shared<Lambertian>(cht));
   //scene->Add(osphere);
   //scene->Add(sphere);
   scene->Add(sphere1);
   scene->Add(sphere2);
   //scene->Add(std::make_shared<Plane>(glm::vec3(0, 1, 0), glm::vec3(0, -0.5, 0), std::make_shared<Lambertian>(ct4)));
   //scene->Add(std::make_shared<Plane>(glm::vec3(0, 1, 0), glm::vec3(0, -0.5, 0), std::make_shared<Lambertian>(cct)));
   scene->Add(std::make_shared<Quad>(
      glm::vec3(-3, -0.5, -3),
      glm::vec3(-3, -0.5, 3),
      glm::vec3(3, -0.5, 3),
      glm::vec3(3, -0.5, -3),
      std::make_shared<Lambertian>(cct)));
   //scene->Add(std::make_shared<Plane>(glm::vec3(0, 0, 1), glm::vec3(0, 0, -1), std::make_shared<Lambertian>(ct4)));

   auto difflight = std::make_shared<DiffuseLight>(std::make_shared<ConstantTexture>(FRGBA(0.9, 0.6, 0.9, 1.0)), 10.0f);
   //scene->Add(std::make_shared<Sphere>(glm::vec3(-0.5, 1.2, -1), 0.7, difflight));
   float d = 0.5f;
   float x = -0.25, y = 0.7, z = -0.3;
   //scene->Add(std::make_shared<Quad>( glm::vec3(x, y, z), glm::vec3(x + d, y, z),
    //  glm::vec3(x + d, y, z + d), glm::vec3(x, y, z + d), difflight));
   HitInfo h;
   //auto quad = std::make_shared<Quad>(glm::vec3(-0.1, -0.1, -1.0), glm::vec3(0.1, -0.1, -1.0), glm::vec3(0.1, 0.1, -1.0), glm::vec3(-0.1, 0.1, -1.0), std::make_shared<Lambertian>(it));
   //quad->Hit({ { 0, 0, 0 }, { 0, 0, -1 } }, 0, INFINITY, h);
   //scene->Add(quad);
   //scene->Construct();
   RayTracer::Init(300, 400);
   model->Clear();
   model->Load("new_bunny.obj");
   //scene->Add(model);
   scene->Construct();
   //RayTracer::SetScene(scene);
   RayTracer::SetScene(GetCornell()); // 500 50
   RayTracer::SetSSRate(500);
   RayTracer::SetBounceDepth(50);
   RayTracer::SetGamma(2);
   RayTracer::SetAmbientLight(FRGBA(0.7, 0.7, 0.7, 1.0));
   
   std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 0.8f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.75, 90.0f, 0.00, glm::length(sphere->GetCenter()));
   //std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(0.0f, 1.f, 0.f), glm::vec3(.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 0.75, 90.0f, 0.00, glm::length(sphere->GetCenter()));
   //std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 2.f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.75, 90.0f, 0.00, glm::length(sphere->GetCenter()));
   //std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(-0.4f, 1.3f, 0.7f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(-0.3f, 1.0f, 0.0f), 16.0f / 9, 90.0f);
   RayTracer::SetCamera(camera);
   auto start = std::chrono::high_resolution_clock::now();
   auto image = RayTracer::Render();
   auto end = std::chrono::high_resolution_clock::now();

   image.Write("finalsss1.png");
   std::chrono::duration<float> time = end - start;
   std::cout << time.count() << " sec." << std::endl;
   
   //image.Write("cube.png");
   return 0;
}