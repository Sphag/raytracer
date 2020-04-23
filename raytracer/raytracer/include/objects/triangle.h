#ifndef __TRIANGLE_H_INCLUDED
#define __TRIANGLE_H_INCLUDED

#include <array>

#include <glm/glm.hpp>

#include "objects/base_object.h"
#include "materials/base_material.h"
#include "objects/plane.h"


class Triangle : public BaseObject
{
   friend class IntersectMng;
public:
   Triangle(
      const glm::vec3& A = glm::vec3(0.0f),
      const glm::vec3& B = glm::vec3(0.0f),
      const glm::vec3& C = glm::vec3(0.0f),
      std::shared_ptr<BaseMaterial> material = nullptr) :
      BaseObject(),
      m_A(A),
      m_B(B),
      m_C(C)
   {
      m_Material = material;
      m_AB = m_B - m_A;
      m_AC = m_C - m_A;
      m_Normal = glm::cross(m_AB, m_AC);
      glm::vec3 min = glm::min(glm::min(A, B), C);
      glm::vec3 max = glm::max(glm::max(A, B), C);
      m_BoundingBox = { 0.5f * (max + min), max - 0.5f * (max + min) };
   }

   bool Hit(const Ray& ray, float minDist, float maxDist, HitInfo& hitInfo) const override;
   Plane GetPlane() const { return Plane(m_Normal, m_A, m_Material); }
   
   glm::vec3 GetNormal() const { return m_Normal; }
   glm::vec3 A() const { return m_A; }
   glm::vec3 B() const { return m_B; }
   glm::vec3 C() const { return m_C; }
   void GetUV(const glm::vec3& point, float &u, float&v) const override { }
private:
   glm::vec3 m_A, m_B, m_C;
   glm::vec3 m_AB, m_AC;
   glm::vec3 m_Normal;
   std::shared_ptr<BaseMaterial> m_Material;
};

#endif
