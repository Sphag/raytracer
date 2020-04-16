#include "rtpch.h"
#include "objects/model.h"
#include "tiny_obj_loader.h"


bool Model::Hit(const Ray& ray, float minDist, float maxDist, HitInfo& hitInfo) const
{
   HitInfo tempHitInfo;
   bool isHitOccurred = false;
   float closest = maxDist;

   for (const auto& tri : m_Mesh) {
      if (tri.Hit(ray, minDist, maxDist, tempHitInfo) && tempHitInfo.t < closest) {
         isHitOccurred = true;
         closest = tempHitInfo.t;
         hitInfo = tempHitInfo;
         hitInfo.material = m_Material;
      }
   }

   return isHitOccurred;
}

bool Model::Load(const std::string filePath)
{
   tinyobj::attrib_t attrib;
   std::vector<tinyobj::shape_t>shapes;
   std::vector<tinyobj::material_t>materials;
   std::string error;
   std::string warning;
   bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warning, &error, filePath.c_str());
   if (!ret) {
#ifdef RT_DEBUG
      std::cerr << "Failed to load model " << filePath << std::endl;
#endif
      return false;
   }
#ifdef RT_DEBUG
   if (!warning.empty()) {
      std::cerr << "WARNINGS:\n" << warning;
   }
   if (!error.empty()) {
      std::cerr << "ERRORS:\n" << error << std::endl;
      return false;
   }
#endif
   RT_ASSERT(attrib.vertices.size() % 3 == 0);
   RT_ASSERT(shapes[0].mesh.indices.size() % 3 == 0);

   glm::vec3* verts = reinterpret_cast<glm::vec3*>(attrib.vertices.data());
   for (int i = 0; i < shapes[0].mesh.indices.size(); i+=3) {
      m_Mesh.emplace_back(
         verts[shapes[0].mesh.indices[i + 0].vertex_index],
         verts[shapes[0].mesh.indices[i + 1].vertex_index],
         verts[shapes[0].mesh.indices[i + 2].vertex_index]
      );
   }

   return true;
}
