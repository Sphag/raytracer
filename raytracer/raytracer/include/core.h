#ifndef __CORE_H_INCLUDED
#define __CORE_H_INCLUDED

#ifdef RT_DEBUG

#define RT_ASSERT(cond) do { __debugbreak(); assert((cond)); }while(0)

#else

#define RT_ASSERT(cond) 

#endif


#endif