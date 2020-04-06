#include "rtpch.h"

#include "image.h"

#include "ray_tracer.h"
#include "ray.h"
#include "sphere.h"

int RayTracer::m_Width;
int RayTracer::m_Height;
std::shared_ptr<HittableList> RayTracer::m_HittableList;

void RayTracer::Init(int width, int height)
{
   m_Width = width;
   m_Height = height;
}

void RayTracer::SetScene(std::shared_ptr<HittableList> hittableList)
{
   m_HittableList = hittableList;
}

ImageURGBA RayTracer::Render()
{
   ImageFRGBA image(m_Width, m_Height);

   glm::vec3 lower_left_corner(-2.0, -1.0, -1.0);
   glm::vec3 origin(0.0f, 0.0f, 0.0f);
   glm::vec3 horizontal(4.0f, 0.0f, 0.0f);
   glm::vec3 vertical(0.0f, 2.0f, 0.0f);


   for (int i = 0; i < image.Height(); i++) {
      for (int j = 0; j < image.Width(); j++) {
         float u = float(i) / image.Height();
         float v = float(j) / image.Width();
         Ray ray(origin, lower_left_corner + u * vertical + v * horizontal);
         image(i, j) = ColorRay(ray);
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
