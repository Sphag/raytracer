#ifndef __AABB_H_INCLUDED
#define __AABB_H_INCLUDED

#include "ray_tracer/ray.h"


class AABB
{
public:
   AABB(const glm::vec3& center = glm::vec3(0.0f), const glm::vec3& dim = glm::vec3(1.0f)) : m_Center(center), m_Dim(dim) {}

   glm::vec3 GetCenter() const { return m_Center; }
   glm::vec3 GetDim() const { return m_Dim; }

   void SetCenter(const glm::vec3& center) { m_Center = center; }
   void SetDim(const glm::vec3& dim) { m_Dim = dim; }
private:
   glm::vec3 m_Center;
   glm::vec3 m_Dim;
};


AABB GetCommonAABB(const std::vector<AABB> boxes);

#endif