#include "rtpch.h"

#include "core/image.h"

#include "ray_tracer/ray_tracer.h"
#include "ray_tracer/ray.h"
#include "objects/sphere.h"

// RayTracer static members
int                           RayTracer::s_Width = 200;
int                           RayTracer::s_Height = 100;
std::shared_ptr<Scene> RayTracer::s_HittableList = nullptr;
int                           RayTracer::s_SPP = 1;
std::shared_ptr<Camera>       RayTracer::s_Camera = std::make_shared<Camera>();
float                         RayTracer::s_Gamma = 2.0f;
uint32_t                      RayTracer::s_Depth = 50;
FRGBA                         RayTracer::s_AmbientLight = RT_FBLACK;


void RayTracer::Init(int width, int height)
{
   s_Width = width;
   s_Height = height;
}

void RayTracer::SetScene(std::shared_ptr<Scene> hittableList)
{
   s_HittableList = hittableList;
}

void RayTracer::SetCamera(std::shared_ptr<Camera> camera)
{
   s_Camera = camera;
}

void RayTracer::SetSSRate(int samplesPerPixel)
{
   RT_ASSERT(samplesPerPixel != 0);
   s_SPP = samplesPerPixel;
}

void RayTracer::SetGamma(float gamma)
{
   s_Gamma = gamma;
}

void RayTracer::SetBounceDepth(uint32_t bounceDepth)
{
   s_Depth = bounceDepth;
}

void RayTracer::SetAmbientLight(FRGBA ambientLight)
{
   s_AmbientLight = ambientLight;
}

ImageURGBA RayTracer::Render()
{
   RT_ASSERT(RT_FloatEquals((float)s_Width / s_Height, s_Camera->GetAspectRatio()));
   ImageFRGBA image(s_Width, s_Height);
   uint32_t totalPixels = image.PixelCount();
   uint32_t p = 0;

   for (int i = 0; i < image.Height(); i++) {
      for (int j = 0; j < image.Width(); j++) {
         FRGBA color(0.0f);
         if (i == image.Height() / 2 && j == image.Width() / 2) 
            volatile int aa = 5;
         for (int k = 0; k < s_SPP; k++) {
            float v = float(i + RT_RandomFloat()) / image.Height();
            float u = float(j + RT_RandomFloat()) / image.Width();
            Ray ray = s_Camera->GetRay(u, v);
            color += ColorRay(ray, s_Depth);
         }

         color /= (float)s_SPP;
         image(i, j) = color;

         p++;
         if (p % 1 == 0) {
            std::cout << p * 100.0f / totalPixels << "% Done" << std::endl;
         }
      }
   }

   GammaCorrect(image, s_Gamma);

   return ToURGBA(image);
}

FRGBA RayTracer::ColorRay(const Ray& ray, uint32_t depth)
{
   HitInfo hitInfo;
   if (depth == 0) {
      return RT_FBLACK;
   }

   if (s_HittableList->Hit(ray, RT_FloatEpsilon, RT_FloatInfinity, hitInfo)) {
      std::vector<Ray> scattered;
      FRGBA attenuation;
      FRGBA emitted = hitInfo.material->Emitted(hitInfo.u, hitInfo.v, hitInfo.hitPoint);
      if (hitInfo.material->Scatter(ray, hitInfo, attenuation, scattered)) {
            RT_ASSERT(scattered.size() != 0);
            return emitted + attenuation * ColorRay(scattered[0], depth - 1);
      } else {
         return emitted;
      }
   }

   return s_AmbientLight;


   /*glm::vec3 unitDirection = glm::normalize(ray.Direction());
   float t = 0.5f * (unitDirection.y + 1.0f);
   return glm::lerp(FRGBA(1.0f), FRGBA(0.5f, 0.7f, 1.0f, 1.0f), t);*/
}
