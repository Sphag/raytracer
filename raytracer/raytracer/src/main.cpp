#include <iostream>
#include <stb_image_write.h>
#include <glm/glm.hpp>

int main()
{
   void* data = malloc(400 * 400);
   memset(data, 0, 400 * 400);
   stbi_write_png("black.png", 400, 400, 1, data, 400);
   std::cout << "Hello, World!";
   return 0;
}