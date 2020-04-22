#include "rtpch.h"
#include "ray_tracer/octree.h"
#include "ray_tracer/intersect_mng.h"


void Octree::Clear()
{
   ClearImpl(&m_Root);
}

void Octree::ClearImpl(OctreeNode** node)
{
   if (!node || !*node) {
      return;
   }

   // clear child nodes
   for (int i = 0; i < 8; i++) {
      ClearImpl(&(*node)->childNodes[i]);
   }

   free(*node);
   *node = nullptr;
}

bool Octree::FindIntersectedNodeImpl(const Ray& ray, OctreeNode* inNode, std::vector<OctreeNode*>& outNodes) const
{
   if (!inNode) {
      return false;
   }

   if (!IntersectMng::Intersects(ray, inNode->box)) {
      return false;
   }

   bool hasNotNullChild = false;
   bool isFound = false;
   for (int i = 0; i < 8; i++) {
      if (inNode->childNodes[i]) {
         hasNotNullChild = true;
         bool isFoundInChild = FindIntersectedNodeImpl(ray, inNode->childNodes[i], outNodes);
         isFound = isFound || isFoundInChild;
      }
   }

   if (!hasNotNullChild) {
      outNodes.push_back(inNode);
      isFound = true;
   }

   return isFound;
}

void Octree::Construct(const std::vector<std::shared_ptr<Triangle>>& objects)
{
   m_Root = (OctreeNode*)malloc(sizeof(OctreeNode));
   memset(m_Root, 0, sizeof(OctreeNode));


   m_Root->objectsIndices.resize(objects.size());
   for (int i = 0; i < objects.size(); i++) {
      m_Root->objectsIndices[i] = i;
   }
   m_Data = objects;
   m_Root->box = GetCommonAABB(GetAABB(m_Root->objectsIndices));
   m_TotalNodes = 1;
   ConstructImpl(&m_Root, objects);
   int a = 5;
}

void Octree::ConstructImpl(OctreeNode** node, const std::vector<std::shared_ptr<Triangle>>& objects)
{
   if (!node || !*node || (*node)->objectsIndices.size() <= MAX_OBJECTS_IN_ONE_NODE) {
      return;
   }

   // allocate child nodes
   for (int i = 0; i < 8; i++) {
      (*node)->childNodes[i] = (OctreeNode*)malloc(sizeof(OctreeNode));
      memset((*node)->childNodes[i], 0, sizeof(OctreeNode));
   }
   m_TotalNodes += 8;
   SetUpBoxes((*node));

   for (int i = 0; i < 8; i++) {
      for (int j = 0; j < (*node)->objectsIndices.size(); j++) {
         if (IntersectMng::Intersects((*node)->childNodes[i]->box, *objects[(*node)->objectsIndices[j]])) {
            (*node)->childNodes[i]->objectsIndices.push_back((*node)->objectsIndices[j]);
         }
      }

      if (node != &m_Root && (*node)->childNodes[i]->objectsIndices.size() >= (*node)->objectsIndices.size()) {
         // if child has at least as many objects as parent, that means that it wont be decreasing 
         // with constructing the tree deeper down, so free all the child nodes and return
         for (int k = 0; k < 8; k++) {
            free((*node)->childNodes[k]);
            (*node)->childNodes[k] = nullptr;
         }

         m_TotalNodes -= 8;

         return;
      }
   }

   for (int i = 0; i < 8; i++) {
      if ((*node)->childNodes[i]->objectsIndices.size() == 0) {
         free((*node)->childNodes[i]);
         (*node)->childNodes[i] = nullptr;
         m_TotalNodes -= 1;
      }
      ConstructImpl(&(*node)->childNodes[i], objects);
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
   parent->childNodes[1]->box.SetCenter(parent->box.GetCenter() - newDim + glm::vec3(parent->box.GetDim().x, 0.0f, 0.0f));
   parent->childNodes[1]->box.SetDim(newDim);

   // 3
   parent->childNodes[2]->box.SetCenter(parent->box.GetCenter() - newDim + glm::vec3(0.0f, parent->box.GetDim().y, 0.0f));
   parent->childNodes[2]->box.SetDim(newDim);

   // 4
   parent->childNodes[3]->box.SetCenter(parent->box.GetCenter() - newDim + glm::vec3(parent->box.GetDim().x,  parent->box.GetDim().y, 0.0f));
   parent->childNodes[3]->box.SetDim(newDim);

   // 5
   parent->childNodes[4]->box.SetCenter(parent->box.GetCenter() + newDim - glm::vec3(parent->box.GetDim().x,  parent->box.GetDim().y, 0.0f));
   parent->childNodes[4]->box.SetDim(newDim);

   // 6
   parent->childNodes[5]->box.SetCenter(parent->box.GetCenter() + newDim - glm::vec3(0.0f, parent->box.GetDim().y, 0.0f));
   parent->childNodes[5]->box.SetDim(newDim);

   // 7
   parent->childNodes[6]->box.SetCenter(parent->box.GetCenter() + newDim - glm::vec3(parent->box.GetDim().x, 0.0f, 0.0f));
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

bool Octree::FindIntersectedNode(const Ray& ray, std::vector<OctreeNode*>& outNodes) const
{
   return FindIntersectedNodeImpl(ray, m_Root, outNodes);
}
