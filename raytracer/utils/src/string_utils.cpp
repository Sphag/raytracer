#include "string_utils.h"

std::string GetExtension(const std::string& fileName)
{
   auto dot = std::find(fileName.begin(), fileName.end(), '.');
   std::string extension;
   if (dot != fileName.end()) {
      extension = std::string(dot + 1, fileName.end());
   }

   return extension;
}