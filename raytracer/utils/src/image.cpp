#include "image.h"


#ifdef UTILS_USE_DEFAULT_PIXEL_TYPES

ImageURGBA ToURGBA(const ImageFRGBA& image)
{
   ImageURGBA out(image.Width(), image.Height());

   for (int i = 0; i < image.Height(); i++) {
      for (int j = 0; j < image.Width(); j++) {
         out(i, j).r = image(i, j).r * 255.999;
         out(i, j).g = image(i, j).g * 255.999;
         out(i, j).b = image(i, j).b * 255.999;
         out(i, j).a = image(i, j).a * 255.999;
      }
   }

   return out;
}

#endif