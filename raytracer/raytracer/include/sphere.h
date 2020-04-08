#ifndef __SPHERE__H_INCLUDED
#define __SPHERE__H_INCLUDED

#include "base_hittable.h"


class Sphere : public BaseHittable
{
public:
   Sphere(glm::vec3 center = glm::vec3(0.0f), float radius = 1.0f) : m_Center(center), m_Radius(radius) {}
   Sphere(const Sphere& other) = default;
   Sphere(Sphere&& other) = default;

   Sphere& operator=(const Sphere& other) = default;
   Sphere& operator=(Sphere&& other) = default;

   bool Hit(const Ray& ray, float minDist, float maxDist, HitInfo& hitInfo) const override;

   glm::vec3 GetCenter() const { return m_Center; }
   float     GetRadius() const { return m_Radius; }

   void SetCenter(const glm::vec3& center) { m_Center = center; }
   void SetRadius(float radius) { RT_ASSERT(radius > 0.0f); m_Radius = radius; }

private:
   glm::vec3 m_Center;
   float     m_Radius;
};

#endif
