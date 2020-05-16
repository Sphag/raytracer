#ifndef __QUAD_H_INCLUDED
#define __QUAD_H_INCLUDED

#include "objects/base_object.h"
#include "objects/triangle.h"
#include "materials/base_material.h"


class Quad : public BaseObject
{
public:
   Quad(const glm::vec3& x0 = { 0,0,0 },
      const glm::vec3& x1 = { 1,0,0 },
      const glm::vec3& x2 = { 1, 1, 0 },
      const glm::vec3& x3 = { 0,1,0 },
      std::shared_ptr<BaseMaterial> material = nullptr) :
      m_Material(material),
      m_Tri1(x0, x1, x2),
      m_Tri2(x0, x2, x3)
   {
      m_BoundingBox = GetCommonAABB({ m_Tri1.GetAABB(), m_Tri2.GetAABB() });
   }

   bool Hit(const Ray& ray, float minDist, float maxDist, HitInfo& hitInfo) const override;
   void ApplyTransform() override;

   void GetUV(const glm::vec3& point, float &u, float&v) const override;
private:
   std::shared_ptr<BaseMaterial> m_Material;
   Triangle m_Tri1, m_Tri2; // verts of quad in anticlockwise order start from min vert
};

#endif
