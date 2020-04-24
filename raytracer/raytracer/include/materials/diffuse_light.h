#ifndef __DIFFUSE_LIGHT_H_INCLUDED
#define __DIFFUSE_LIGHT_H_INCLUDED

#include <memory>

#include "materials/base_material.h"
#include "textures/base_texture.h"

class DiffuseLight : public BaseMaterial
{
public:
   DiffuseLight(std::shared_ptr<BaseTexture> tex, float intensity) : m_Texture(tex), m_Intensity(intensity) { RT_ASSERT(intensity >= 0.0f); }

   bool Scatter(const Ray& rayIn, const HitInfo& hitInfo, FRGBA& attenuation, std::vector<Ray>& scattered) const override { return false; }
   FRGBA Emitted(float u, float v, const glm::vec3& p) const override { return m_Texture->Value(u, v, p) * m_Intensity; }
private:
   std::shared_ptr<BaseTexture> m_Texture;
   float m_Intensity;
};

#endif
