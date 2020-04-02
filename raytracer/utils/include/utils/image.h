#pragma once
#ifndef __IMAGE_H_INCLUDED
#define __IMAGE_H_INCLUDED

#include <glm/glm.hpp>
#include <stb_image.h>
#include <stb_image_write.h>
#include <stb_image_resize.h>

#include "string_utils.h"


////////////////////////////////////////////////////////////////////////////////
//Pixel types

//Floating pixel types
using FG    = float;       // Grey
using FGA   = glm::vec2;   // Grey, Alpha
using FRGB  = glm::vec3;   // Red, Green, Blue
using FRGBA = glm::vec4;   // Red, Green, Blue, Alpha

//Unsigned pixel types
using UG    = uint32_t;                              // Grey
using UGA   = glm::vec<2, uint8_t, glm::defaultp>;   // Grey, Alpha
using URGB  = glm::vec<3, uint8_t, glm::defaultp>;   // Red, Green, Blue
using URGBA = glm::vec<4, uint8_t, glm::defaultp>;   // Red, Green, Blue, Alpha


template<class T>
class PixelTrait
{
   static int            Size() { return sizeof(T); }
   static stbir_datatype DataType() { return STBIR_MAX_TYPES; }
   static int            NumChannels() { return 0; }
};

template<>
class PixelTrait<FG>
{
   static int            Size() { return sizeof(FG); }
   static stbir_datatype DataType() { return STBIR_TYPE_FLOAT; }
   static int            NumChannels() { return 1; }
};

template<>
class PixelTrait<FGA>
{
   static int            Size() { return sizeof(FGA); }
   static stbir_datatype DataType() { return STBIR_TYPE_FLOAT; }
   static int            NumChannels() { return 2; }
};

template<>
class PixelTrait<FRGB>
{
   static int            Size() { return sizeof(FRGB); }
   static stbir_datatype DataType() { return STBIR_TYPE_FLOAT; }
   static int            NumChannels() { return 3; }
};

template<>
class PixelTrait<FRGBA>
{
   static int            Size() { return sizeof(FRGBA); }
   static stbir_datatype DataType() { return STBIR_TYPE_FLOAT; }
   static int            NumChannels() { return 4; }
};

template<>
class PixelTrait<UG>
{
   static int            Size() { return sizeof(UG); }
   static stbir_datatype DataType() { return STBIR_TYPE_UINT8; }
   static int            NumChannels() { return 1; }
};

template<>
class PixelTrait<UGA>
{
   static int            Size() { return sizeof(UGA); }
   static stbir_datatype DataType() { return STBIR_TYPE_UINT8; }
   static int            NumChannels() { return 2; }
};

template<>
class PixelTrait<URGB>
{
   static int            Size() { return sizeof(URGB); }
   static stbir_datatype DataType() { return STBIR_TYPE_UINT8; }
   static int            NumChannels() { return 3; }
};

template<>
class PixelTrait<URGBA>
{
   static int            Size() { return sizeof(URGBA); }
   static stbir_datatype DataType() { return STBIR_TYPE_UINT8; }
   static int            NumChannels() { return 4; }
};


////////////////////////////////////////////////////////////////////////////////
//Image type
template<class Pixel>
class Image
{
public:
   Image(int width = 0, int height = 0) :
      m_Width(width),
      m_Height(height),
      m_Data(nullptr)
   {
      stbi_set_flip_vertically_on_load(1);
      stbi_flip_vertically_on_write(1);
   }

   Image(const std::string& fileName)
   {
      Load(fileName);
   }

   ~Image()
   {
      stbi_image_free(m_Data);
   }


   Image(const Image& other)
   {
      m_Width = other.m_Width;
      m_Height = other.m_Height;

      m_Data = STBI_MALLOC(other.DataSize());

      for (int i = 0; i < other.PixelCount()) {
         m_Data[i] = other.m_Data[i];
      }
   }

   Image(Image&& other) :
      m_Width(std::move(other.m_Width)),
      m_Height(std::move(other.m_Height)),
      m_Data(std::move(other.m_Data))
   {}


   Image& operator=(const Image& other)
   {
      if (this != &other) {
         m_Width = other.m_Width;
         m_Height = other.m_Height;
         m_Data = STBI_MALLOC(other.DataSize());

         for (int i = 0; i < other.PixelCount(); i++) {
            m_Data[i] = other.m_Data[i];
         }
      }

      return *this;
   }

   Image& operator=(Image&& other)
   {
      if (this != &other) {
         m_Width = std::move(other.m_Width);
         m_Height = std::move(other.m_Height);
         m_Data = std::move(other.m_Data);
      }

      return *this;
   }


   Pixel& operator[](int ind)       { return data[ind]; }
   Pixel  operator[](int ind) const { return data[ind]; }

   Pixel& operator()(int i, int j)       { return data[m_Width * i + j]; }
   Pixel  operator()(int i, int j) const { return data[m_Width * i + j]; }

   bool Load(const std::string fileName)
   {
      int dummyPixelSize = 0;
      m_Data = stbi_load(fileName.c_str(), &m_Width, &m_Height, dummyPixelSize, 0);
      
      return true;
   }

   bool Write(const std::string fileName) const
   {
      bool isWrittenSuccessfully = true;
      std::string extension = GetExtension(fileName);
      switch (extension)
      {
         case "png":
         {
            stbi_write_png(fileName.c_str(), m_Width, m_Height, sizeof(Pixel), m_Data, m_Width * sizeof(Pixel));
            break;
         }
         case "bmp":
         {
            stbi_write_bmp(fileName.c_str(), m_Width, m_Height, sizeof(Pixel), m_Data);
            break;
         }
         case "tga":
         {
            stbi_write_tga(fileName.c_str(), m_Width, m_Height, sizeof(Pixel), m_Data);
            break;
         }
         case "jpg":
         {
            stbi_write_jpg(fileName.c_str(), m_Width, m_Height, sizeof(Pixel), m_Data);
            break;
         }
         case "hdr":
         {
            stbi_write_hdr(fileName.c_str(), m_Width, m_Height, sizeof(Pixel), m_Data);
            break;
         }
         default:
         {
            isWrittenSuccessfully = false;
         }
      }

      return isWrittenSuccessfully;
   }

   bool Resize(int newWidth, int newHeight)
   {
      unsigned char * newData = nullptr;
      bool isSuccessful = stbir_resize(m_Data, m_Width, m_Height, m_Width * PixelTrait<Pixel>::Size(),
         newData, newWidth, newHeight, newWidth * PixelTrait<Pixel>::Size(),
         PixelTrait<Pixel>::DataType(), PixelTrait<Pixel>::NumChannels(), -1, 0,
         STBIR_EDGE_CLAMP, STBIR_EDGE_CLAMP, STBIR_FILTER_DEFAULT, STBIR_FILTER_DEFAULT, STBIR_COLORSPACE_LINEAR, nullptr);

      return isSuccessful;
   }

   int Width()      const { return m_Width; }
   int Height()     const { return m_Height; }
   int PixelCount() const { return m_Width * m_Height; }
   int DataSize()   const { return m_Width * m_Height * m_pixelSize; }
private:
   int   m_Width;
   int   m_Height;
   Pixel*   m_Data;
};

using ImageFG = Image<FG>;
using ImageFGA = Image<FGA>;
using ImageFRGB = Image<FRGB>;
using ImageFRGBA = Image<FRGBA>;

using ImageUG = Image<UG>;
using ImageUGA = Image<UGA>;
using ImageURGB = Image<URGB>;
using ImageURGBA = Image<URGBA>;

#endif
