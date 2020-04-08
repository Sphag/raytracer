#include "rtpch.h"

#include "image.h"

#include "ray_tracer.h"
#include "ray.h"
#include "sphere.h"

int RayTracer::m_Width;
int RayTracer::m_Height;
std::shared_ptr<HittableList> RayTracer::m_HittableList;
int RayTracer::m_SPP;
Camera RayTracer::m_Camera;

void RayTracer::Init(int width, int height)
{
   m_Width = width;
   m_Height = height;
}

void RayTracer::SetScene(std::shared_ptr<HittableList> hittableList)
{
   m_HittableList = hittableList;
}

void RayTracer::SetCamera(const Camera& camera)
{
   m_Camera = camera;
}

void RayTracer::SetSSRate(int samplesPerPixel)
{
   m_SPP = samplesPerPixel;
}

ImageURGBA RayTracer::Render()
{
   ImageFRGBA image(m_Width, m_Height);

   for (int i = 0; i < image.Height(); i++) {
      for (int j = 0; j < image.Width(); j++) {
         FRGBA color(0.0f);
         for (int k = 0; k < m_SPP; k++) {
            float v = float(i + RandomFloat()) / image.Height();
            float u = float(j + RandomFloat()) / image.Width();
            Ray ray = m_Camera.GetRay(u, v);
            color += ColorRay(ray);
         }

         color /= m_SPP;
         image(i, j) = color;
      }
   }

   return ToURGBA(image);
}

FRGBA RayTracer::ColorRay(const Ray& ray)
{
   HitInfo hitInfo;
   if (m_HittableList->Hit(ray, 0, INFINITY, hitInfo)) {
      return FRGBA(0.5f * (hitInfo.normal + glm::vec3(1, 1, 1)), 1.0f);
   }
   glm::vec3 unitDirection = glm::normalize(ray.Direction());
   float t = 0.5f * (unitDirection.y + 1.0f);
   return glm::lerp(FRGBA(1.0f), FRGBA(0.5f, 0.7f, 1.0f, 1.0f), t);
}
