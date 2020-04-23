#ifndef __CONSTANT_TEXTURE_H_INCLUDED
#define __CONSTANT_TEXTURE_H_INCLUDED

#include "textures/base_texture.h"


class ConstantTexture : public BaseTexture
{
public:
   ConstantTexture(FRGBA color) : m_Color(color) {}

   FRGBA Value(float u, float v, const glm::vec3& hitPoint) const override;
private:
   FRGBA m_Color;
};

#endif
