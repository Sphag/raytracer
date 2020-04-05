#include <iostream>

#include "image.h"
#include "string_utils.h"

#include <glm/glm.hpp>

#include "ray_tracer.h"


int main()
{
   RayTracer::Init(200, 100);
   ImageURGBA img(400, 200);
   img(0, 0).r = 255;
   img(0, 0).g = 0;
   img(0, 0).b = 0;
   img(0, 0).a = 255;
   //auto image = RayTracer::Render();
   img.Write("render.png");
   return 0;
}