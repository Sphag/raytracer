#ifndef __TRIANGLE_H_INCLUDED
#define __TRIANGLE_H_INCLUDED

#include <array>

#include <glm/glm.hpp>

#include "objects/base_hittable.h"
#include "materials/base_material.h"


class Triangle : public BaseHittable
{
public:
   Triangle(
      const glm::vec3& A = glm::vec3(0.0f),
      const glm::vec3& B = glm::vec3(0.0f),
      const glm::vec3& C = glm::vec3(0.0f),
      std::shared_ptr<BaseMaterial> material = nullptr) :
   m_A(A),
   m_B(B),
   m_C(C),
   m_Material(material)
   {
      m_AB = m_B - m_A;
      m_AC = m_C - m_A;
      m_Normal = glm::cross(m_AB, m_AC);
   }

   bool Hit(const Ray& ray, float minDist, float maxDist, HitInfo& hitInfo) const override;
private:
   glm::vec3 m_A, m_B, m_C;
   glm::vec3 m_AB, m_AC;
   glm::vec3 m_Normal;
   std::shared_ptr<BaseMaterial> m_Material;
};

#endif
