#include "rtpch.h"
#include "core/math_utils.h"


glm::vec3 RandomUnitVector()
{
   float a = RT_RandomFloatRange(0.0f, 2 * RT_FloatPi);
   float z = RT_RandomFloatRange(-1.0f, 1.0f);
   float r = glm::sqrt(1 - z*z);
   return glm::vec3(r*glm::cos(a), r*glm::sin(a), z);
}

glm::vec3 RandomUnitHemisphere(const glm::vec3& normal)
{
   glm::vec3 inUnitSphere = RandomUnitVector();
   if (glm::dot(inUnitSphere, normal) > 0.0f) {
      return inUnitSphere;
   }
   else {
      return -inUnitSphere;
   }
}
