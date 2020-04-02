#pragma once
#ifndef __RAY_TRACER_H_INCLUDED
#define __RAY_TRACER_H_INCLUDED

#include <utils/Image.h>
#include <utils/pixel_traits.h>


class RayTracer
{
public:
   
   static void Init();
   static void SetDefaultParameters();
   static ImageFRGBA Render();
private:
   static struct
   {
   } m_Parameters;
};

#endif