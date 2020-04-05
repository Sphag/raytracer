#include "rtpch.h"
#include "image.h"


#ifdef UTILS_USE_DEFAULT_PIXEL_TYPES

ImageURGBA ToURGBA(const ImageFRGBA& image)
{
   ImageURGBA out(image.Width(), image.Height());

   for (int i = 0; i < image.Height(); i++) {
      for (int j = 0; j < image.Width(); j++) {
         out(i, j).r = (uint8_t)(image(i, j).r * 255.999);
         out(i, j).g = (uint8_t)(image(i, j).g * 255.999);
         out(i, j).b = (uint8_t)(image(i, j).b * 255.999);
         out(i, j).a = (uint8_t)(image(i, j).a * 255.999);
      }
   }

   return out;
}

#endif