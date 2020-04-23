#pragma once
#ifndef __SCENE_H_INCLUDED
#define __SCENE_H_INCLUDED

#include <vector>
#include <memory>

#include "objects/base_object.h"
#include "ray_tracer/bvh.h"


class Scene : public BaseObject
{
   friend class IntersectMng;
public:
   Scene() : BaseObject(), m_Objects() 
   {
      m_BoundingBox = { {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
      m_Material = nullptr;
   }
   
   void Clear() { m_Objects.clear(); }
   void Add(std::shared_ptr<BaseObject> object) { m_Objects.push_back(object); }
   void Construct() 
   { 
      m_BVH.Construct(m_Objects);
      std::vector<int> ind; for (int i = 0; i < m_Objects.size(); i++) ind.push_back(i);
      bool res = m_BVH.GetRootAABB(m_BoundingBox);
      RT_ASSERT(res);
   }

   size_t Count() const { return m_Objects.size(); }

   bool Hit(const Ray& ray, float minDist, float maxDist, HitInfo& hitInfo) const override;

   void GetUV(const glm::vec3& point, float &u, float&v) const override {}
private:
   std::vector<std::shared_ptr<BaseObject>> m_Objects;
   BVH m_BVH;
};

#endif