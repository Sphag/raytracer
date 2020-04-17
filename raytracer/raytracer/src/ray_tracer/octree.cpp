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

void Octree::Construct(const std::vector<std::shared_ptr<Triangle>>& objects)
{
   m_Root = (OctreeNode*)malloc(sizeof(OctreeNode));
   memset(m_Root, 0, sizeof(m_Root));


   m_Root->objectsIndices.resize(objects.size());
   for (int i = 0; i < objects.size(); i++) {
      m_Root->objectsIndices[i] = i;
   }

   m_Root->box = GetCommonAABB(GetAABB(m_Root->objectsIndices));
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

   SetUpBoxes(node);

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

void Octree::SetUpBoxes(OctreeNode *parent) const
{
   glm::vec3 newDim = 0.5f * parent->box.GetDim();
   glm::vec3 max = parent->box.GetCenter() + parent->box.GetDim();
   // 1
   parent->childNodes[0]->box.SetCenter(parent->box.GetCenter() - newDim);
   parent->childNodes[0]->box.SetDim(newDim);

   // 2
   parent->childNodes[1]->box.SetCenter(parent->box.GetCenter() - newDim + parent->box.GetDim().x);
   parent->childNodes[1]->box.SetDim(newDim);

   // 3
   parent->childNodes[2]->box.SetCenter(parent->box.GetCenter() - newDim + parent->box.GetDim().y);
   parent->childNodes[2]->box.SetDim(newDim);

   // 4
   parent->childNodes[3]->box.SetCenter(parent->box.GetCenter() - newDim + parent->box.GetDim().x + parent->box.GetDim().y);
   parent->childNodes[3]->box.SetDim(newDim);

   // 5
   parent->childNodes[4]->box.SetCenter(parent->box.GetCenter() + newDim - parent->box.GetDim().x - parent->box.GetDim().y);
   parent->childNodes[4]->box.SetDim(newDim);

   // 6
   parent->childNodes[5]->box.SetCenter(parent->box.GetCenter() + newDim - parent->box.GetDim().y);
   parent->childNodes[5]->box.SetDim(newDim);

   // 7
   parent->childNodes[6]->box.SetCenter(parent->box.GetCenter() + newDim - parent->box.GetDim().x);
   parent->childNodes[6]->box.SetDim(newDim);

   // 8
   parent->childNodes[7]->box.SetCenter(parent->box.GetCenter() + newDim);
   parent->childNodes[7]->box.SetDim(newDim);
}

std::vector<AABB> Octree::GetAABB(const std::vector<int>& indices)
{
   std::vector<AABB> aabbs;
   for (int i = 0; i < indices.size(); i++) {
      aabbs.push_back(m_Data[indices[i]]->GetAABB());
   }

   return aabbs;
}
