#include "rtpch.h"

#include "image.h"

#include "ray_tracer.h"


int main()
{
   RayTracer::Init(200, 100);
   auto image = RayTracer::Render();
   image.Write("render.png");
   return 0;
}