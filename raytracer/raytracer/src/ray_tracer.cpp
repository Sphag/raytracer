#include "rtpch.h"

#include "image.h"

#include "ray_tracer.h"
#include "ray.h"
#include "sphere.h"

// RayTracer static members
int                           RayTracer::s_Width = 200;
int                           RayTracer::s_Height = 100;
std::shared_ptr<HittableList> RayTracer::s_HittableList = nullptr;
int                           RayTracer::s_SPP = 1;
std::shared_ptr<Camera>       RayTracer::s_Camera = std::make_shared<Camera>();
float                         RayTracer::s_Gamma = 2.0f;
uint32_t                      RayTracer::s_Depth = 50;


void RayTracer::Init(int width, int height)
{
   s_Width = width;
   s_Height = height;
}

void RayTracer::SetScene(std::shared_ptr<HittableList> hittableList)
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

ImageURGBA RayTracer::Render()
{
   RT_ASSERT(RT_FloatEquals((float)s_Width / s_Height, s_Camera->GetAspectRatio()));
   ImageFRGBA image(s_Width, s_Height);
   uint32_t totalPixels = image.PixelCount();
   uint32_t p = 0;

   for (int i = 0; i < image.Height(); i++) {
      for (int j = 0; j < image.Width(); j++) {
         FRGBA color(0.0f);
         for (int k = 0; k < s_SPP; k++) {
            float v = float(i + RT_RandomFloat()) / image.Height();
            float u = float(j + RT_RandomFloat()) / image.Width();
            Ray ray = s_Camera->GetRay(u, v);
            color += ColorRay(ray, s_Depth);
         }

         color /= (float)s_SPP;
         image(i, j) = color;

         p++;
         if (p % 10000 == 0) {
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
      if (hitInfo.material->Scatter(ray, hitInfo, attenuation, scattered)) {
         RT_ASSERT(scattered.size() != 0);
               return attenuation * ColorRay(scattered[0], depth - 1);
         volatile MATERIAL_TYPE materialType = hitInfo.material->GetMaterialType();
         switch (materialType) {
            case MATERIAL_TYPE::LAMBERTIAN:
            case MATERIAL_TYPE::METALIC:
               return attenuation * ColorRay(scattered[0], depth - 1);
            case MATERIAL_TYPE::DIELECTRIC:
               return attenuation * ColorRay(scattered[0], depth - 1);
#if 0
            {
               if (scattered.size() > 1) {
                  FRGBA refractedColor = ColorRay(scattered[1], depth - 1);
                  FRGBA reflectedColor = ColorRay(scattered[0], depth - 1);
                  return attenuation * (refractedColor + reflectedColor) / 2.0f;
               } else {
                  return attenuation * ColorRay(scattered[0], depth - 1);
               }
            }
#endif
            case MATERIAL_TYPE::UNKNOWN:
            {
               MATERIAL_TYPE m = hitInfo.material->GetMaterialType();
               RT_ASSERT(false);
               return RT_FBLACK;
            }
         }
      } else {
         return RT_FBLACK;
      }
   }

   glm::vec3 unitDirection = glm::normalize(ray.Direction());
   float t = 0.5f * (unitDirection.y + 1.0f);
   return glm::lerp(FRGBA(1.0f), FRGBA(0.5f, 0.7f, 1.0f, 1.0f), t);
}
