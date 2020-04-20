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
   }
   
   void Clear() { m_Objects.clear(); }
   void Add(std::shared_ptr<BaseObject> object) { m_Objects.push_back(object); }
   void Construct() 
   { 
      m_BVH.Construct(m_Objects);
      std::vector<int> ind; for (int i = 0; i < ind.size(); i++) ind.push_back(i);
      m_BoundingBox = GetCommonAABB(m_BVH.GetAABB(ind));
   }

   std::shared_ptr<BaseObject>& operator[] (int idx)       { return m_Objects[idx]; }
   std::shared_ptr<BaseObject>  operator[] (int idx) const { return m_Objects[idx]; }

   size_t Count() const { return m_Objects.size(); }

   std::vector<std::shared_ptr<BaseObject>>::iterator begin() { return m_Objects.begin(); }
   std::vector<std::shared_ptr<BaseObject>>::iterator end() { return m_Objects.end(); }

   std::vector<std::shared_ptr<BaseObject>>::const_iterator begin() const { return m_Objects.begin(); }
   std::vector<std::shared_ptr<BaseObject>>::const_iterator end() const { return m_Objects.end(); }

   bool Hit(const Ray& ray, float minDist, float maxDist, HitInfo& hitInfo) const override;
private:
   std::vector<std::shared_ptr<BaseObject>> m_Objects;
   BVH m_BVH;
};

#endif