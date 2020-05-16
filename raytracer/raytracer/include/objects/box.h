#ifndef __BOX_H_INCLUDED
#define __BOX_H_INCLUDED

#include "objects/base_object.h"


class Box : public BaseObject
{
   friend class IntersectMng;
public:
   Box(
      glm::vec3 center,
      glm::vec3 dim,
      std::shared_ptr<BaseMaterial> material
   ) : m_Center(center), m_Dim(dim) 
   {
      m_Material = material;
      m_BoundingBox = { center, dim };
   }

   glm::vec3 GetCenter() const { return m_Center; }
   glm::vec3 GetDim() const { return m_Dim; }

   void GetUV(const glm::vec3& point, float& u, float& v) const override { u = 0.0f; v = 0.0f; }

   bool Hit(const Ray& ray, float minDist, float maxDist, HitInfo& hitInfo) const override;

   void ApplyTransform() override
   {
      m_Center = glm::vec3(m_Transform.GetMatrix() * glm::vec4(m_Center, 1.0f));
      m_BoundingBox.SetCenter(m_Center);
   }
   
private:
   glm::vec3 m_Center;
   glm::vec3 m_Dim;
};

#endif
