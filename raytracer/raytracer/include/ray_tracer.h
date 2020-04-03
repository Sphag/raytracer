#ifndef __RAY_TRACER_H_INCLUDED
#define __RAY_TRACER_H_INCLUDED

#include <utils/image.h>
#include <glm/glm.hpp>

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
};

#endif
