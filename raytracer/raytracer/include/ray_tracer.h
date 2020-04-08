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
   static void SetGamma(float gamma);
   static void SetBounceDepth(uint32_t bounceDepth);

   static ImageURGBA Render();
private:
   static FRGBA ColorRay(const Ray& ray, uint32_t depth);
private:
   static int                           s_Width;
   static int                           s_Height;
   static std::shared_ptr<HittableList> s_HittableList;
   static int                           s_SPP;
   static std::shared_ptr<Camera>       s_Camera;
   static float                         s_Gamma;
   static uint32_t                      s_Depth;
};

#endif
