#include "rtpch.h"
#include "ray_tracer/octree.h"
#include "ray_tracer/intersect_mng.h"


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

   m_Root->box = initialBB;

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

   glm::vec3 midPoint = glm::vec3(0.5f) /** (node->box.MinVert() + node->box.MaxVert())*/;

   SetUpBoxes(node, midPoint);

   for (int i = 0; i < 8; i++) {
      for (int j = 0; j < objects.size(); j++) {
         if (IntersectMng::Intersects(node->childNodes[i]->box, objects[j]->GetPlane())) {
            node->childNodes[i]->objectsIndices.push_back(j);
         }
      }
   }

   for (int i = 0; i < 8; i++) {
      ConstructImpl(node->childNodes[i], objects);
   }
}

void Octree::SetUpBoxes(OctreeNode *parent, const glm::vec3& midPoint) const
{
   /*glm::vec3 diag = midPoint - parent->box.MinVert();
   // 1
   parent->childNodes[0]->box.SetMinVert(parent->box.MinVert());
   parent->childNodes[0]->box.SetMaxVert(midPoint);

   // 2
   parent->childNodes[1]->box.SetMinVert(parent->box.MinVert() + glm::vec3(0.0f, diag.y, 0.0f));
   parent->childNodes[1]->box.SetMaxVert(midPoint + glm::vec3(0.0f, diag.y, 0.0f));

   // 3
   parent->childNodes[2]->box.SetMinVert(parent->box.MinVert() + glm::vec3(0.0f, 0.0f, diag.z));
   parent->childNodes[2]->box.SetMaxVert(midPoint + glm::vec3(0.0f, 0.0f, diag.z));

   // 4
   parent->childNodes[3]->box.SetMinVert(parent->box.MinVert() + glm::vec3(0.0f, diag.y, diag.z));
   parent->childNodes[3]->box.SetMaxVert(midPoint + glm::vec3(0.0f, diag.y, diag.z));

   // 5
   parent->childNodes[4]->box.SetMinVert(parent->box.MinVert() + glm::vec3(diag.x, 0.0f, 0.0f));
   parent->childNodes[4]->box.SetMaxVert(midPoint + glm::vec3(diag.x, 0.0f, 0.0f));

   // 6
   parent->childNodes[5]->box.SetMinVert(parent->box.MinVert() + glm::vec3(diag.x, diag.y, 0.0f));
   parent->childNodes[5]->box.SetMaxVert(midPoint + glm::vec3(diag.x, diag.y, 0.0f));

   // 7
   parent->childNodes[6]->box.SetMinVert(parent->box.MinVert() + glm::vec3(diag.x, 0.0f, diag.z));
   parent->childNodes[6]->box.SetMaxVert(midPoint + glm::vec3(diag.x, 0.0f, diag.z));

   // 8
   parent->childNodes[7]->box.SetMinVert(midPoint);
   parent->childNodes[7]->box.SetMaxVert(parent->box.MaxVert());*/
}
