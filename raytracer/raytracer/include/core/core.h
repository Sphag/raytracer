#ifndef __CORE_H_INCLUDED
#define __CORE_H_INCLUDED

#include "core/pixel_traits.h"

#define NOMINMAX

#define RT_RandomFloatRange(x,y) glm::linearRand(x, y)
#define RT_RandomFloat()         RT_RandomFloatRange(0.0f, 1.0f)
#define RT_RandomBool()          glm::linearRand(0, 1)
#define RT_RandomUnitSphere()    glm::sphericalRand(1.0f)
#define RT_FloatEquals(x,y)      (glm::abs((x) - (y)) < glm::epsilon<float>())
#define RT_FloatEpsilon          glm::epsilon<float>()
#define RT_FloatInfinity         INFINITY
#define RT_FloatPi               glm::pi<float>()
#define RT_GetNormal(x, y)       glm::normalize(glm::cross(x, y))

#define RT_Float3UnitX           glm::vec3(1.0f, 0.0f, 0.0f)
#define RT_Float3UnitY           glm::vec3(0.0f, 1.0f, 0.0f)
#define RT_Float3UnitZ           glm::vec3(0.0f, 0.0f, 1.0f)
#define RT_Float3Zero            glm::vec3(0.0f)
#define RT_Float3One             glm::vec3(1.0f)

#ifdef UTILS_USE_DEFAULT_PIXEL_TYPES

#define RT_FBLACK        FRGBA(0.0f, 0.0f, 0.0f, 1.0f)
#define RT_FWHITE        FRGBA(1.0f, 1.0f, 1.0f, 1.0f)
#define RT_FRED          FRGBA(1.0f, 0.0f, 0.0f, 1.0f)
#define RT_FGREEN        FRGBA(0.0f, 1.0f, 0.0f, 1.0f)
#define RT_FBLUE         FRGBA(0.0f, 0.0f, 1.0f, 1.0f)
#define RT_FINVALIDCOLOR FRGBA(-1.0f, -1.0f, -1.0f, -1.0f)

#else

#define RT_FBLACK
#define RT_FWHITE
#define RT_FRED  
#define RT_FGREEN
#define RT_FBLUE 

#endif

#ifdef RT_DEBUG

#define RT_ASSERT(cond) while(!(cond)){ __debugbreak(); assert((cond)); }

#else

#define RT_ASSERT(cond) 

#endif


#endif