#ifndef __PLANE_H_INCLUDED
#define __PLANE_H_INCLUDED

#include <array>

#include <glm/glm.hpp>

#include "ray_tracer/aabb.h"
#include "objects/base_object.h"
#include "materials/base_material.h"


class Plane : public BaseObject
{
   friend class IntersectMng;
public:
   Plane(
      const glm::vec3& normal = glm::vec3(0.0f, 1.0f, 0.0f),
      const glm::vec3& offset = glm::vec3(0.0f, 0.0f, 0.0f),
      std::shared_ptr<BaseMaterial> material = nullptr) :
      BaseObject()
   {
      m_Material = material;
      m_Normal = normal;
      m_Offset = offset;
      m_BoundingBox = { glm::vec3(0.0f), {INFINITY, INFINITY, INFINITY} };
      RT_ASSERT(m_Normal != glm::vec3(0.0f));
   }

   bool Hit(const Ray& ray, float minDist, float maxDist, HitInfo& hitInfo) const override;

   glm::vec3 GetNormal() const { return m_Normal; }
   glm::vec3 GetOffset() const { return m_Offset; }

   void GetUV(const glm::vec3& point, float& u, float& v) const override;

private:
   glm::vec3 m_Normal;
   glm::vec3 m_Offset;
};

#endif
