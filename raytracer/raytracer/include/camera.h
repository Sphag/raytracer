#ifndef __CAMERA_H_INCLUDED
#define __CAMERA_H_INCLUDED

#include <glm/glm.hpp>

#include "ray.h"


class Camera
{
public:
   Camera(glm::vec3 origin = glm::vec3(0.0f),
      glm::vec3 lowerLeft = glm::vec3(-2.0f, -1.0f, -1.0f),
      glm::vec3 horizontal = glm::vec3(4.0f, 0.0f, 0.0f),
      glm::vec3 vertical = glm::vec3(0.0f, 2.0f, 0.0f)) : m_Origin(origin),
      m_LowerLeft(lowerLeft),
      m_Horizontal(horizontal),
      m_Vertical(vertical) {}

   Ray GetRay(float u, float v) const
   {
      return Ray(m_Origin, m_LowerLeft + u * m_Horizontal + v * m_Vertical - m_Origin);
   }
private:
   glm::vec3 m_Origin;
   glm::vec3 m_LowerLeft;
   glm::vec3 m_Horizontal;
   glm::vec3 m_Vertical;
};

#endif
