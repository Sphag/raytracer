#ifndef __RAY_TRACER_H_INCLUDED
#define __RAY_TRACER_H_INCLUDED

#include "rtpch.h"

#include "image.h"
#include "sphere.h"
#include "ray.h"
#include "hittable_list.h"
#include "camera.h"


class RayTracer
{
public:
   static void Init(int width, int height);

   static void SetScene(std::shared_ptr<HittableList> hittableList);
   static void SetCamera(std::shared_ptr<Camera> camera);
   static void SetSSRate(int samplesPerPixel);

   static ImageURGBA Render();
private:
   static FRGBA ColorRay(const Ray& ray);
private:
   static int                           m_Width;
   static int                           m_Height;
   static std::shared_ptr<HittableList> m_HittableList;
   static int                           m_SPP;
   static std::shared_ptr<Camera>       m_Camera;
};

#endif
