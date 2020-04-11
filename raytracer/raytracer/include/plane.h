#ifndef __PLANE_H_INCLUDED
#define __PLANE_H_INCLUDED

#include <array>

#include <glm/glm.hpp>

#include "base_hittable.h"
#include "base_material.h"


class Plane : public BaseHittable
{
public:
   Plane(
      const glm::vec3& normal = glm::vec3(1.0f, 0.0f, 0.0f),
      const glm::vec3& offset = glm::vec3(0.0f, 0.0f, 0.0f),
      std::shared_ptr<BaseMaterial> material = nullptr) :
      m_Normal(normal),
      m_Offset(offset),
      m_Material(material)
   {
      RT_ASSERT(m_Normal != glm::vec3(0.0f));
   }

   bool Hit(const Ray& ray, float minDist, float maxDist, HitInfo& hitInfo) const override;

private:
   glm::vec3 m_Normal;
   glm::vec3 m_Offset;
   std::shared_ptr<BaseMaterial> m_Material;
};

#endif