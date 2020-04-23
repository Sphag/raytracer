#ifndef __IMAGE_TEXTURE_H
#define __IMAGE_TEXTURE_H

#include "textures/base_texture.h"

class ImageTexture : public BaseTexture
{
public:
   ImageTexture(const ImageFRGBA& image) : m_Image(image) {}
   ImageTexture(ImageFRGBA&& image) : m_Image(image) {}
   ImageTexture(const std::string& fs) : m_Image(ToFRGBA(ImageURGBA(fs))) {}

   FRGBA Value(float u, float v, const glm::vec3& hitPoint) const override;
private:
   ImageFRGBA m_Image;
};

#endif
