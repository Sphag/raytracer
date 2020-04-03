#include <iostream>

#include <utils/image.h>
#include <utils/string_utils.h>

#include <glm/glm.hpp>

#include "ray_tracer.h"


int main()
{
   RayTracer::Init(200, 100);
   auto image = RayTracer::Render();
   image.Write("render.png");
   return 0;
}