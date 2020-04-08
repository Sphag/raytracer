#ifndef __CORE_H_INCLUDED
#define __CORE_H_INCLUDED

#define RandomFloat() glm::linearRand(0.0f, 1.0f)

#ifdef RT_DEBUG

#define RT_ASSERT(cond) do { __debugbreak(); assert((cond)); }while(0)

#else

#define RT_ASSERT(cond) 

#endif


#endif