#ifndef __CAMERA_H_INCLUDED
#define __CAMERA_H_INCLUDED

#include <glm/glm.hpp>

#include "ray_tracer/ray.h"


class Camera
{
public:
   Camera(glm::vec3 origin = glm::vec3(0.0f),
      glm::vec3 eye = glm::vec3(0.0f, 0.0f, -1.0f),
      glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
      float aspectRatio = (float)16 / 9,
      float fov = 100,
      float aperture = 0.5f,
      float focusDist = 1.0f) : m_Origin(origin),
      m_Eye(eye),
      m_Up(up),
      m_AspectRatio(aspectRatio),
      m_FOV(fov),
      m_Aperture(aperture),
      m_FocusDistance(focusDist)
   {
      glm::vec3 u, v, w;
      m_LensRadius = aperture / 2;
      auto halfHeight = glm::tan(glm::radians(fov) / 2);
      auto halfWidth = aspectRatio * halfHeight;
      w = glm::normalize(origin - eye);
      auto a = glm::cross(up, w);
      m_U = glm::normalize(glm::cross(up, w));
      m_V = glm::cross(w, m_U);

      lowerLeft = origin - m_FocusDistance * halfWidth*m_U - m_FocusDistance * halfHeight*m_V - m_FocusDistance * w;

      horizontal = 2 * m_FocusDistance * halfWidth*m_U;
      vertical = 2 * m_FocusDistance * halfHeight*m_V;
   }

   Ray GetRay(float u, float v) const
   {
      glm::vec3 randomInDisk = m_LensRadius * glm::vec3(glm::circularRand(1.0f), 0.0f);
      glm::vec3 offset = m_U * randomInDisk.x + m_V * randomInDisk.y;
      return Ray(m_Origin + offset, lowerLeft + u * horizontal + v * vertical - m_Origin);
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
   glm::vec3 m_U, m_V;
   glm::vec3 lowerLeft;
   glm::vec3 horizontal;
   glm::vec3 vertical;
   float m_AspectRatio;
   float m_FOV;
   float m_Aperture;
   float m_FocusDistance;
   float m_LensRadius;
   glm::vec3 m_Right;
};

#endif
