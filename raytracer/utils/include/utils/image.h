#pragma once
#ifndef __IMAGE_H_INCLUDED
#define __IMAGE_H_INCLUDED

#include <glm/glm.hpp>
#include <stb_image.h>
#include <stb_image_write.h>
#include <stb_image_resize.h>

#include "pixel_traits.h"
#include "string_utils.h"


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
      m_Data = malloc(m_Width * m_Height * PixelTraits<Pixel>::Size());
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

      m_Data = reinterpret_cast<unsigned char*>((other.DataSize()));

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
         m_Data = malloc(other.DataSize());

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


   Pixel& operator[](int ind)       { return m_Data[ind]; }
   Pixel  operator[](int ind) const { return m_Data[ind]; }

   Pixel& operator()(int i, int j)       { return m_Data[m_Width * i + j]; }
   Pixel  operator()(int i, int j) const { return m_Data[m_Width * i + j]; }

   bool Load(const std::string fileName)
   {
      int dummyPixelSize = 0;
      m_Data = stbi_load(fileName.c_str(), &m_Width, &m_Height, &dummyPixelSize, 0);
      
      return true;
   }

   bool Write(const std::string fileName, bool isHdr = false) const
   {
      bool isWrittenSuccessfully = false;
      std::string extension = GetExtension(fileName);
      if (isHdr && PixelTraits<Pixel>::DataType() == STBIR_TYPE_FLOAT) {
         stbi_write_hdr(fileName.c_str(), m_Width, m_Height, sizeof(Pixel), reinterpret_cast<float*>(m_Data));
         isWrittenSuccessfully = true;
      } else if (extension == "png") {
         stbi_write_png(fileName.c_str(), m_Width, m_Height, sizeof(Pixel), m_Data, m_Width * sizeof(Pixel));
         isWrittenSuccessfully = true;
      } else if (extension == "bmp") {
         stbi_write_bmp(fileName.c_str(), m_Width, m_Height, sizeof(Pixel), m_Data);
         isWrittenSuccessfully = true;
      } else if (extension == "tga") {
         stbi_write_tga(fileName.c_str(), m_Width, m_Height, sizeof(Pixel), m_Data);
         isWrittenSuccessfully = true;
      } else if (extension == "jpg") {
         stbi_write_jpg(fileName.c_str(), m_Width, m_Height, sizeof(Pixel), m_Data, 100);
         isWrittenSuccessfully = true;
      }

      return isWrittenSuccessfully;
   }

   int Resize(int newWidth, int newHeight)
   {
      unsigned char * newData = reinterpret_cast<unsigned char*>(malloc(newWidth * newHeight * PixelTraits<Pixel>::Size()));
      int success = 0;
      if (PixelTraits<Pixel>::DataType() == STBIR_TYPE_UINT8) {
         success = stbir_resize_uint8(m_Data, m_Width, m_Height, 0,
            newData, newWidth, newHeight, 0,
            PixelTraits<Pixel>::NumChannels());
      } else if (PixelTraits<Pixel>::DataType() == STBIR_TYPE_FLOAT) {
         success = stbir_resize_float(reinterpret_cast<float*>(m_Data), m_Width, m_Height, 0,
            reinterpret_cast<float*>(newData), newWidth, newHeight, 0,
            PixelTraits<Pixel>::NumChannels());
      }

      if (success) {
         m_Data = newData;
         m_Width = newWidth;
         m_Height = newHeight;
      }

      return success;
   }

   void SetVerticalFlipOnLoad  (bool isFlip) const { stbi_set_flip_vertically_on_load(isFlip); }
   void SetVerticalFlipOnWrite (bool isFlip) const { stbi_flip_vertically_on_write(isFlip); }

   int Width()      const { return m_Width; }
   int Height()     const { return m_Height; }
   int PixelCount() const { return m_Width * m_Height; }
   int DataSize()   const { return m_Width * m_Height * m_pixelSize; }
private:
   int              m_Width;
   int              m_Height;
   unsigned char*   m_Data;
};

#ifdef UTILS_USE_DEFAULT_PIXEL_TYPES

using ImageFG = Image<FG>;
using ImageFGA = Image<FGA>;
using ImageFRGB = Image<FRGB>;
using ImageFRGBA = Image<FRGBA>;

using ImageUG = Image<UG>;
using ImageUGA = Image<UGA>;
using ImageURGB = Image<URGB>;
using ImageURGBA = Image<URGBA>;

#endif

#endif
