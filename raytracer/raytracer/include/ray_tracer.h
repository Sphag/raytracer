#ifndef __RAY_TRACER_H_INCLUDED
#define __RAY_TRACER_H_INCLUDED

#include "image.h"
#include <glm/glm.hpp>
#include "sphere.h"

#include "ray.h"


class RayTracer
{
public:
   static void Init(int width, int height);
   static ImageURGBA Render();
private:
   static FRGBA ColorRay(const Ray& ray);
   static int m_Width;
   static int m_Height;
   static Sphere m_Sphere;
};

#endif
