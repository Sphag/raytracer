#ifndef __CHECKER_TEXTURE_H_INCLUDED
#define __CHECKER_TEXTURE_H_INCLUDED

#include <memory>

#include "textures/base_texture.h"


class CheckerTexture : public BaseTexture
{
public:
   CheckerTexture(
      std::shared_ptr<BaseTexture> tex0,
      std::shared_ptr<BaseTexture> tex1
   ) : m_Tex0(tex0), m_Tex1(tex1) {}

   FRGBA Value(float u, float v, const glm::vec3& hitPoint) const override;
private:
   std::shared_ptr<BaseTexture> m_Tex0;
   std::shared_ptr<BaseTexture> m_Tex1;
};

#endif