#include "rtpch.h"
#include "core/image.h"


#ifdef UTILS_USE_DEFAULT_PIXEL_TYPES

ImageURGBA ToURGBA(const ImageFRGBA& image)
{
   ImageURGBA out(image.Width(), image.Height());
   for (int i = 0; i < image.Height(); i++) {
      for (int j = 0; j < image.Width(); j++) {
         out(i, j).r = (uint8_t)(glm::clamp(image(i, j).r, 0.0f, 0.999f) * 256);
         out(i, j).g = (uint8_t)(glm::clamp(image(i, j).g, 0.0f, 0.999f) * 256);
         out(i, j).b = (uint8_t)(glm::clamp(image(i, j).b, 0.0f, 0.999f) * 256);
         out(i, j).a = (uint8_t)(glm::clamp(image(i, j).a, 0.0f, 0.999f) * 256);
      }
   }

   return out;
}

ImageFRGBA ToFRGBA(const ImageURGBA& image)
{
   ImageFRGBA out(image.Width(), image.Height());
   for (int i = 0; i < image.Height(); i++) {
      for (int j = 0; j < image.Width(); j++) {
         FRGBA fcol = {
            glm::clamp(float(image(i, j).r) / float(255), 0.0f, 1.0f),
            glm::clamp(float(image(i, j).g) / float(255), 0.0f, 1.0f),
            glm::clamp(float(image(i, j).b) / float(255), 0.0f, 1.0f),
            1.0f
         };
         out(i, j) = fcol;
      }
   }

   return out;
}

#endif

void GammaCorrect(ImageFRGBA& image, float gamma)
{
   for (int i = 0; i < image.Height(); i++) {
      for (int j = 0; j < image.Width(); j++) {
         image(i, j) = glm::pow(image(i, j), FRGBA(glm::vec3(1 / gamma), 1.0f));
      }
   }
}
