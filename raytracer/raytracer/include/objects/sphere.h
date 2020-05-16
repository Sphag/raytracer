#ifndef __SPHERE__H_INCLUDED
#define __SPHERE__H_INCLUDED

#include "objects/base_object.h"
#include "materials/base_material.h"


class Sphere : public BaseObject
{
   friend class IntersectMng;
public:
   Sphere(glm::vec3 center = glm::vec3(0.0f), float radius = 1.0f, std::shared_ptr<BaseMaterial> material = nullptr) : m_Center(center), 
      m_Radius(radius),
      m_Material(material)
   {
      m_BoundingBox = { m_Center, {radius, radius, radius} };
   }
   Sphere(const Sphere& other) = default;
   Sphere(Sphere&& other) = default;

   Sphere& operator=(const Sphere& other) = default;
   Sphere& operator=(Sphere&& other) = default;

   bool Hit(const Ray& ray, float minDist, float maxDist, HitInfo& hitInfo) const override;
   void ApplyTransform() override 
   { 
      m_Center = glm::vec3(m_Transform.GetMatrix() * glm::vec4(m_Center, 1.0f));
      m_BoundingBox = { m_Center, {m_Radius, m_Radius, m_Radius} };
   }

   glm::vec3 GetCenter() const { return m_Center; }
   float     GetRadius() const { return m_Radius; }

   void SetCenter(const glm::vec3& center) { m_Center = center; }
   void SetRadius(float radius) { RT_ASSERT(radius > 0.0f); m_Radius = radius; }
   void GetUV(const glm::vec3& point, float &u, float&v) const override;
private:
   glm::vec3                     m_Center;
   float                         m_Radius;
   std::shared_ptr<BaseMaterial> m_Material;
};

#endif
