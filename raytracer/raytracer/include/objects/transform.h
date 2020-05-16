#pragma once
#ifndef __TRANSFORM_H_INCLUDED
#define __TRANSFORM_H_INCLUDED

#include <core/core.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


class Transform
{
public:
   Transform(glm::vec3 translate = glm::vec3(0.0f),
      glm::vec3 rotation = glm::vec3(0.0f),
      glm::vec3 scale = glm::vec3(1.0f)) : m_Translation(translate), m_Rotation(rotation), m_Scale(scale) 
   {}

   glm::mat4 GetMatrix() const 
   {
      return glm::translate(m_Translation) * m_RotationMatrix *
         glm::rotate(m_Rotation.x, glm::vec3({ 1.0f, 0.0f, 0.0f })) *
         glm::rotate(m_Rotation.y, glm::vec3({ 0.0f, 1.0f, 0.0f })) *
         glm::rotate(m_Rotation.z, glm::vec3({ 0.0f, 0.0f, 1.0f })) *
         glm::scale(m_Scale);

   }

   void SetTranslation(glm::vec3 translation) { m_Translation = translation; }
   void SetRotation(glm::vec3 rotation) { m_Rotation= rotation; }
   void SetScale(glm::vec3 scale) { m_Scale = scale; }
   void SetLocalRotationMatrix(glm::mat4 rm) { m_RotationMatrix = rm; }
private:
   glm::vec3 m_Translation;
   glm::vec3 m_Rotation;
   glm::vec3 m_Scale;
   glm::mat4 m_RotationMatrix = glm::mat4(1.0f); // for local rotation against axis
};

#endif