#pragma once
#ifndef __BASE_TEXTURE_H_INCLUDED
#define __BASE_TEXTURE_H_INCLUDED

#include <glm/glm.hpp>
#include <core/core.h>


class BaseTexture
{
public:
   virtual ~BaseTexture() {}
   virtual FRGBA Value(float u, float v, const glm::vec3& hitPoint) const = 0;
};


#endif