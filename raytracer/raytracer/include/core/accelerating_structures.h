#ifndef __ACCELERATING_STRUCTURES_H
#define __ACCELERATING_STRUCTURES_H

#include <glm/glm.hpp>
#include "ray.h"

class AABB
{
public:
   AABB(const glm::vec3& min = glm::vec3(0.0f), const glm::vec3& max = glm::vec3(1.0f)) : m_MinVert(min), m_MaxVert(max) {}

   glm::vec3 MinVert() const { return m_MinVert; }
   glm::vec3 MaxVert() const { return m_MaxVert; }

   bool Hit(const Ray& ray, float minDist, float maxDist) const;
private:
   glm::vec3 m_MinVert; // vertex of BB, that contains the smallest x, y, z
   glm::vec3 m_MaxVert; // vertex of BB, that contains the largest x, y, z
};

#endif