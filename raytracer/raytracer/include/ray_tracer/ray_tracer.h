#ifndef __RAY_TRACER_H_INCLUDED
#define __RAY_TRACER_H_INCLUDED

#include "rtpch.h"

#include "core/image.h"
#include "objects/sphere.h"
#include "ray_tracer/ray.h"
#include "objects/scene.h"
#include "ray_tracer/camera.h"


class RayTracer
{
public:
   static void Init(int width, int height);

   static void SetScene(std::shared_ptr<Scene> hittableList);
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
   static std::shared_ptr<Scene> s_HittableList;
   static int                           s_SPP;
   static std::shared_ptr<Camera>       s_Camera;
   static float                         s_Gamma;
   static uint32_t                      s_Depth;
};

#endif
