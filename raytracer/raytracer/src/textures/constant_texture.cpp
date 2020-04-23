#include "rtpch.h"

#include "textures/constant_texture.h"


FRGBA ConstantTexture::Value(float u, float v, const glm::vec3& hitPoint) const
{
   return m_Color;
}

