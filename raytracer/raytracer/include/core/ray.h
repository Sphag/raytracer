#pragma once
#ifndef __RAY_H_INCLUDED
#define __RAY_H_INCLUDED

#include <glm/glm.hpp>

class BaseMaterial;


class Ray
{
public:
   Ray() {}
   Ray(const glm::vec3& origin, const glm::vec3& direction) : m_Origin(origin), m_Direction(direction) {}

   glm::vec3 Origin() const { return m_Origin; }
   glm::vec3 Direction() const { return m_Direction; }

   glm::vec3 At(float t) const { return m_Origin + t * m_Direction; }
private:
   glm::vec3 m_Origin;
   glm::vec3 m_Direction;
};


struct HitInfo
{
   glm::vec3 hitPoint;
   glm::vec3 normal;
   std::shared_ptr<BaseMaterial> material;
   float t;
   bool isFrontFace;

   inline void SetFaceNormal(const Ray& ray, const glm::vec3& outwardNormal)
   {
      isFrontFace = glm::dot(ray.Direction(), outwardNormal) < 0;
      normal = isFrontFace ? outwardNormal : -outwardNormal;
   }
};

#endif