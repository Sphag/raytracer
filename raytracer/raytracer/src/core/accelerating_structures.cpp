#include "rtpch.h"

#include "core/accelerating_structures.h"


bool AABB::Hit(const Ray& ray, float minDist, float maxDist) const
{
   glm::vec3 t0 = glm::min(
      (m_MinVert - ray.Origin()) / ray.Direction(),
      (m_MaxVert - ray.Origin()) / ray.Direction()
   );

   glm::vec3 t1 = glm::max(
      (m_MinVert - ray.Origin()) / ray.Direction(),
      (m_MaxVert - ray.Origin()) / ray.Direction()
   );

   glm::vec3 tMin = glm::max(t0, glm::vec3(minDist));
   glm::vec3 tMax = glm::min(t1, glm::vec3(maxDist));
   
   if (glm::any(glm::lessThanEqual(tMax, tMin))) {
      return false;
   }

   return true;
}

bool AABB::Intersects(const Plane& plane) const
{
   glm::vec3 verts[8] = {
      m_MinVert,
      m_MinVert + glm::vec3(m_MaxVert.x, 0.0f, 0.0f),
      m_MinVert + glm::vec3(m_MaxVert.x, m_MaxVert.y, 0.0f),
      m_MinVert + glm::vec3(0.0f, m_MaxVert.y, 0.0f),
      m_MinVert + glm::vec3(0.0f, 0.0f, m_MaxVert.z),
      m_MinVert + glm::vec3(m_MaxVert.x, 0.0f, m_MaxVert.z),
      m_MinVert + glm::vec3(0.0f, m_MaxVert.y, m_MaxVert.z),
      m_MaxVert
   };

   glm::vec4 planeCoeff(plane.GetNormal(), glm::dot(plane.GetNormal(), plane.GetOffset()));
   int sign = 0;
   for (int i = 0; i < 8; i++) {
      float val = glm::dot(planeCoeff, glm::vec4(verts[i], 1.0f));
      if (sign == 0) {
         sign = glm::sign(val);
      } else if (glm::sign(val) != sign) {
         return true;
      }
   }

   return sign == 0;
}

void Octree::Clear()
{
   ClearImpl(m_Root);
}

void Octree::ClearImpl(OctreeNode* node)
{
   if (!node) {
      return;
   }

   // clear child nodes
   for (int i = 0; i < 8; i++) {
      ClearImpl(m_Root->childNodes[i]);
   }

   free(node);
   node = nullptr;
}

void Octree::Construct(const AABB& initialBB, const std::vector<std::shared_ptr<Triangle>>& objects)
{
   m_Root = (OctreeNode*)malloc(sizeof(OctreeNode));
   memset(m_Root, 0, sizeof(m_Root));

   m_Root->rect = initialBB;

   m_Root->objectsIndices.resize(objects.size());
   for (int i = 0; i < objects.size(); i++) {
      m_Root->objectsIndices[i] = i;
   }

   ConstructImpl(m_Root, objects);
}

void Octree::ConstructImpl(OctreeNode* node, const std::vector<std::shared_ptr<Triangle>>& objects)
{
   if (objects.size() <= MAX_OBJECTS_IN_ONE_NODE) {
      return;
   }

   // allocate child nodes
   for (int i = 0; i < 8; i++) {
      node->childNodes[i] = (OctreeNode*)malloc(sizeof(OctreeNode));
      memset(node->childNodes[i], 0, sizeof(OctreeNode));
   }

   glm::vec3 midPoint = 0.5f * (node->rect.MinVert() + node->rect.MaxVert());

   SetUpRects(node, midPoint);

   for (int i = 0; i < 8; i++) {
      for (int j = 0; j < objects.size(); j++) {
         if (node->childNodes[i]->rect.Intersects(objects[j]->GetPlane())) {
            node->childNodes[i]->objectsIndices.push_back(j);
         }
      }
   }

   for (int i = 0; i < 8; i++) {
      ConstructImpl(node->childNodes[i], objects);
   }
}

void Octree::SetUpRects(OctreeNode *parent, const glm::vec3& midPoint) const
{
   glm::vec3 diag = midPoint - parent->rect.MinVert();
   // 1
   parent->childNodes[0]->rect.SetMinVert(parent->rect.MinVert());
   parent->childNodes[0]->rect.SetMaxVert(midPoint);

   // 2
   parent->childNodes[1]->rect.SetMinVert(parent->rect.MinVert() + glm::vec3(0.0f, diag.y, 0.0f));
   parent->childNodes[1]->rect.SetMaxVert(midPoint + glm::vec3(0.0f, diag.y, 0.0f));

   // 3
   parent->childNodes[2]->rect.SetMinVert(parent->rect.MinVert() + glm::vec3(0.0f, 0.0f, diag.z));
   parent->childNodes[2]->rect.SetMaxVert(midPoint + glm::vec3(0.0f, 0.0f, diag.z));

   // 4
   parent->childNodes[3]->rect.SetMinVert(parent->rect.MinVert() + glm::vec3(0.0f, diag.y, diag.z));
   parent->childNodes[3]->rect.SetMaxVert(midPoint + glm::vec3(0.0f, diag.y, diag.z));

   // 5
   parent->childNodes[4]->rect.SetMinVert(parent->rect.MinVert() + glm::vec3(diag.x, 0.0f, 0.0f));
   parent->childNodes[4]->rect.SetMaxVert(midPoint + glm::vec3(diag.x, 0.0f, 0.0f));

   // 6
   parent->childNodes[5]->rect.SetMinVert(parent->rect.MinVert() + glm::vec3(diag.x, diag.y, 0.0f));
   parent->childNodes[5]->rect.SetMaxVert(midPoint + glm::vec3(diag.x, diag.y, 0.0f));

   // 7
   parent->childNodes[6]->rect.SetMinVert(parent->rect.MinVert() + glm::vec3(diag.x, 0.0f, diag.z));
   parent->childNodes[6]->rect.SetMaxVert(midPoint + glm::vec3(diag.x, 0.0f, diag.z));

   // 8
   parent->childNodes[7]->rect.SetMinVert(midPoint);
   parent->childNodes[7]->rect.SetMaxVert(parent->rect.MaxVert());
}
