#ifndef __MODEL_H_INCLUDED
#define __MODEL_H_INCLUDED

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "objects/base_object.h"
#include "ray_tracer/octree.h"
#include "ray_tracer/bvh.h"
#include "materials/base_material.h"
#include "objects/triangle.h"


class Model : public BaseObject
{
   friend class IntersectMng;
public:
   Model(
      const std::vector<std::shared_ptr<Triangle>>& mesh = std::vector<std::shared_ptr<Triangle>>(),
      std::shared_ptr<BaseMaterial> material = nullptr
   ) : m_Mesh(mesh), m_Material(material) {}

   Model(const std::string& filePath) { Load(filePath); }

   bool Hit(const Ray& ray, float minDist, float maxDist, HitInfo& hitInfo) const override;
   void ApplyTransform() override;
   bool Load(const std::string filePath);
   void SetMaterial(std::shared_ptr<BaseMaterial> material) { m_Material = material; }
   void GetUV(const glm::vec3& point, float& u, float& v) const override { u = 0.0f; v = 0.0f; }
   Octree& GetTree() { return m_Tree; }
   void Clear() { m_Tree.Clear(); m_Mesh.clear(); m_VertexNormals.clear(); }
private:
   std::vector<std::shared_ptr<Triangle>> m_Mesh;
   std::vector<glm::vec3> m_VertexNormals;
   std::shared_ptr<BaseMaterial> m_Material;
   Octree m_Tree;
};

#endif
