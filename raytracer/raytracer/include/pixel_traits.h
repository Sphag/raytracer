#pragma once
#ifndef __PIXEL_TRAITS_H_INCLUDED
#define  __PIXEL_TRAITS_H_INCLUDED

#include <glm/glm.hpp>
#include <stb_image_resize.h>


////////////////////////////////////////////////////////////////////////////////
//Pixel types

enum class PIXEL_DATA_TYPE : int8_t
{
   UNKNOWN = -1,
   UINT8   = STBIR_TYPE_UINT8,
   UINT16  = STBIR_TYPE_UINT16,
   UINT32  = STBIR_TYPE_UINT32,
   FLOAT   = STBIR_TYPE_FLOAT
};

////////////////////////////////////////////////////////////////////////////////
// Base pixel traits.
//
// Serves as example, you must register your own type, if you want to use 
// your own type of pixel, this will creates traits for your type

template<class T>
class PixelTraits
{
public:
   static  int             Size()        { return sizeof(T); }
   static  PIXEL_DATA_TYPE DataType()    { return PIXEL_DATA_TYPE::UNKNOWN; }
   static  int             NumChannels() { return 0; }
   typedef T               PixelType;
};


////////////////////////////////////////////////////////////////////////////////
// Auxiliary stuff for finding pixel type by info

template<size_t Size, PIXEL_DATA_TYPE DataType, size_t NumChannels>
class PixelTypeHolder {};

////////////////////////////////////////////////////////////////////////////////
// Registration of pixel types

#define REGISTER_PIXEL_TYPE(name, type, dataType, numChannels)     \
typedef type name;                                                 \
template<>                                                         \
class PixelTraits<name>                                            \
{                                                                  \
public:                                                            \
   static  int             Size() { return sizeof(type); }         \
   static  PIXEL_DATA_TYPE DataType() { return dataType; }         \
   static  int             NumChannels() { return numChannels; }   \
   typedef name            PixelType;                              \
};                                                                 \
template<>                                                         \
class PixelTypeHolder<sizeof(type), dataType, numChannels>         \
{                                                                  \
public:                                                            \
   typedef name PixelType;                                         \
};

#ifdef UTILS_USE_DEFAULT_PIXEL_TYPES

REGISTER_PIXEL_TYPE(FRGBA, glm::vec4,   PIXEL_DATA_TYPE::FLOAT, 4);
REGISTER_PIXEL_TYPE(URGBA, glm::u8vec4, PIXEL_DATA_TYPE::UINT8, 4);

#endif

#endif
