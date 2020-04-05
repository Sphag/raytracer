#include "string_utils.h"


std::string GetExtension(const std::string& fileName)
{
   size_t dotPos = fileName.find_first_of('.');
   std::string extension(fileName.begin() + dotPos + 1, fileName.end());

   return extension;
}
