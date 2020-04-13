#ifndef __MODEL_H_INCLUDED
#define __MODEL_H_INCLUDED

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "objects/base_object.h"
#include "materials/base_material.h"
#include "triangle.h"


class Model : public BaseObject
{
public:
   Model(
      const std::vector<Triangle>& mesh = std::vector<Triangle>(),
      std::shared_ptr<BaseMaterial> material = nullptr
   ) : m_Mesh(mesh), m_Material(material) {}

   Model(const std::string& filePath) { Load(filePath); }

   bool Hit(const Ray& ray, float minDist, float maxDist, HitInfo& hitInfo) const override;
   bool Load(const std::string filePath);
   void SetMaterial(std::shared_ptr<BaseMaterial> material) { m_Material = material; }
private:
   bool LoadRaw(const std::string filePath, std::vector<glm::vec3>& verticies, std::vector<glm::i32vec3>& faces);
   bool ProcessChar(const char ch);
private:
   enum class LOAD_LINE_STATE
   {
      BEGIN_LINE,
      VERTEX,
      TEXT_COORD,
      PARAMETER_VERTEX,
      NORMAL,
      FACE,
      LINE,
      COMMENT,
   } m_LoadLineState;
   enum class LOAD_TOKEN_STATE
   {
      BEGIN_TOKEN,
      CONTINUE_TOKEN
   } m_LoadTokenState;
   enum class LOAD_TOKEN_TYPE
   {
      LINE_TYPE,
      FLOAT,
      INT
   } m_LoadTokenType;
private:
   std::vector<Triangle> m_Mesh;
   std::vector<glm::vec3> m_VertexNormals;
   std::shared_ptr<BaseMaterial> m_Material;
};

#endif
