#include "rtpch.h"
#include "textures/checker_texture.h"


FRGBA CheckerTexture::Value(float u, float v, const glm::vec3& hitPoint) const
{
   float sines = glm::compMul(glm::sin(10.0f * hitPoint));
   if (sines < 0) {
      return m_Tex1->Value(u, v, hitPoint);
   } else {
      return m_Tex0->Value(u, v, hitPoint);
   }
}

