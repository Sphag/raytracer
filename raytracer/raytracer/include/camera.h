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
      float fov = 100) : m_Origin(origin),
      m_Eye(eye),
      m_Up(up),
      m_AspectRatio(aspectRatio),
      m_FOV(fov)
   {
      glm::vec3 u, v, w;
      auto halfHeight = glm::tan(glm::radians(fov) / 2);
      auto halfWidth = aspectRatio * halfHeight;
      w = glm::normalize(origin - eye);
      auto a = glm::cross(up, w);
      u = glm::normalize(glm::cross(up, w));
      v = glm::cross(w, u);

      lowerLeft = origin - halfWidth*u - halfHeight*v - w;

      horizontal = 2 * halfWidth*u;
      vertical = 2 * halfHeight*v;
   }

   Ray GetRay(float u, float v) const
   {
      return Ray(m_Origin, lowerLeft + u * horizontal + v * vertical - m_Origin);
   }

   glm::vec3 GetOrigin()       const { return m_Origin; }
   glm::vec3 GetEye()          const { return m_Eye;    }
   glm::vec3 GetUp()           const { return m_Up;     }
   glm::vec3 GetRight()        const { return m_Right;  }

   float     GetAspectRatio()  const { return m_AspectRatio; }
   float     GetFOVInRadians() const { return glm::radians(m_FOV); }
   float     GetFOVInDegrees() const { return m_FOV; }

   void      SetOrigin      (glm::vec3 origin)  { m_Origin = origin; }
   void      SetEye         (glm::vec3 eye)     { m_Eye = eye; }
   void      SetUp          (glm::vec3 up)      { m_Up = up; }
   void      SetRight       (glm::vec3 right)   { m_Right = right; }

   void      SetAspectRatio (float aspectRatio) { m_AspectRatio = aspectRatio; }
   void      SetFOV         (float fov)         { m_FOV = fov; }


private:
   glm::vec3 m_Origin;
   glm::vec3 m_Eye;
   glm::vec3 m_Up;
   glm::vec3 lowerLeft;
   glm::vec3 horizontal;
   glm::vec3 vertical;
   float m_AspectRatio;
   float m_FOV;
   glm::vec3 m_Right;
};

#endif
