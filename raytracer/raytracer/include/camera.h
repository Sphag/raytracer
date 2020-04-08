#ifndef __CAMERA_H_INCLUDED
#define __CAMERA_H_INCLUDED

#include <glm/glm.hpp>

#include "ray.h"


class Camera
{
public:
   Camera(glm::vec3 origin = glm::vec3(0.0f),
      glm::vec3 eye = glm::vec3(0.0f, 0.0f, -1.0f),
      glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
      float aspectRatio = (float)16 / 9,
      float fov = 110) : m_Origin(origin),
      m_Eye(eye),
      m_Up(up),
      m_AspectRatio(aspectRatio),
      m_FOV(fov)
   {
      m_Right = glm::normalize(glm::cross(m_Eye, m_Up));
      float halfHorizontalLength = glm::tan(glm::radians(fov / 2.0f));
      float halfVerticalLength = halfHorizontalLength * aspectRatio;

      m_Up *= halfHorizontalLength;
      m_Right *= halfVerticalLength;
   }

   Ray GetRay(float u, float v) const
   {
      return Ray(m_Origin, m_Eye + u * m_Right + v * m_Up - m_Origin);
   }

   float GetAspectRatio()  const { return m_AspectRatio; }
   float GetFOVInRadians() const { return glm::radians(m_FOV); }
   float GetFOVInDegrees() const { return m_FOV; }
private:
   glm::vec3 m_Origin;
   glm::vec3 m_Eye;
   glm::vec3 m_Up;
   float m_AspectRatio;
   float m_FOV;
   glm::vec3 m_Right;
};

#endif
