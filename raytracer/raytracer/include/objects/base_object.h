#ifndef __BASE_OBJECT_H_INCLUDED
#define __BASE_OBJECT_H_INCLUDED

#include <memory>

#include "materials/base_material.h"

#include "ray_tracer/ray.h"
#include "ray_tracer/aabb.h"


class BaseObject
{
public:
   BaseObject() : m_BoundingBox(), m_Material() {}
   virtual ~BaseObject() {}
   virtual bool Hit(const Ray& ray, float minDist, float maxDist, HitInfo& hitInfo) const = 0;

   AABB GetAABB() const { return m_BoundingBox; }

   virtual void GetUV(const glm::vec3& point, float &u, float&v) const = 0;
protected:
   AABB m_BoundingBox;
   std::shared_ptr<BaseMaterial> m_Material;
};

#endif
