#include "rtpch.h"
#include "textures/image_texture.h"


FRGBA ImageTexture::Value(float u, float v, const glm::vec3& hitPoint) const
{
   int i = u * m_Image.Width();
   int j = (1 - v) * m_Image.Height();

   i = glm::clamp(i, 0, m_Image.Width() - 1);
   j = glm::clamp(j, 0, m_Image.Height() - 1);

   return m_Image(j, i);
}

