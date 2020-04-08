#include "rtpch.h"

#include "image.h"

#include "ray_tracer.h"
#include "ray.h"
#include "sphere.h"

// RayTracer static members
int                           RayTracer::m_Width;
int                           RayTracer::m_Height;
std::shared_ptr<HittableList> RayTracer::m_HittableList;
int                           RayTracer::m_SPP;
std::shared_ptr<Camera>       RayTracer::m_Camera;


void RayTracer::Init(int width, int height)
{
   m_Width = width;
   m_Height = height;
}

void RayTracer::SetScene(std::shared_ptr<HittableList> hittableList)
{
   m_HittableList = hittableList;
}

void RayTracer::SetCamera(std::shared_ptr<Camera> camera)
{
   m_Camera = camera;
}

void RayTracer::SetSSRate(int samplesPerPixel)
{
   RT_ASSERT(samplesPerPixel != 0);
   m_SPP = samplesPerPixel;
}

ImageURGBA RayTracer::Render()
{
   RT_ASSERT(RT_FloatEquals((float)m_Width / m_Height, m_Camera.GetAspectRatio()));
   ImageFRGBA image(m_Width, m_Height);

   for (int i = 0; i < image.Height(); i++) {
      for (int j = 0; j < image.Width(); j++) {
         FRGBA color(0.0f);
         for (int k = 0; k < m_SPP; k++) {
            float v = glm::lerp(-1.0f, 1.0f, float(i + RT_RandomFloat()) / image.Height());
            float u = glm::lerp(-1.0f, 1.0f, float(j + RT_RandomFloat()) / image.Width());
            Ray ray = m_Camera->GetRay(u, v);
            color += ColorRay(ray);
         }

         color /= (float)m_SPP;
         image(i, j) = color;
      }
   }

   return ToURGBA(image);
}

FRGBA RayTracer::ColorRay(const Ray& ray)
{
   HitInfo hitInfo;
   if (m_HittableList->Hit(ray, RT_FloatEpsilon, RT_FloatInfinity, hitInfo)) {
      return FRGBA(0.5f * (hitInfo.normal + glm::vec3(1, 1, 1)), 1.0f);
   }
   glm::vec3 unitDirection = glm::normalize(ray.Direction());
   float t = 0.5f * (unitDirection.y + 1.0f);
   return glm::lerp(FRGBA(1.0f), FRGBA(0.5f, 0.7f, 1.0f, 1.0f), t);
}
