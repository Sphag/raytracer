#ifndef __AABB_H_INCLUDED
#define __AABB_H_INCLUDED

#include "ray_tracer/ray.h"

#include "objects/plane.h"
#include "objects/triangle.h"


class AABB
{
public:
   AABB(const glm::vec3& min = glm::vec3(0.0f), const glm::vec3& max = glm::vec3(1.0f)) : m_MinVert(min), m_MaxVert(max) {}

   glm::vec3 MinVert() const { return m_MinVert; }
   glm::vec3 MaxVert() const { return m_MaxVert; }

   void SetMinVert(const glm::vec3& min) { m_MinVert = min; }
   void SetMaxVert(const glm::vec3& max) { m_MaxVert = max; }

   bool Hit(const Ray& ray, float minDist, float maxDist) const;
   bool Intersects(const Plane& plane) const;
   bool Intersects(const Triangle& triangle) const;
private:
   glm::vec3 m_MinVert; // vertex of BB, that contains the smallest x, y, z
   glm::vec3 m_MaxVert; // vertex of BB, that contains the largest x, y, z
};

#endif