#pragma once
#ifndef __HITTABLE_LIST_H_INCLUDED
#define __HITTABLE_LIST_H_INCLUDED

#include "base_hittable.h"


class HittableList : public BaseHittable
{
public:
   HittableList() : m_Objects() {}
   
   void Clear() { m_Objects.clear(); }
   void Add(std::shared_ptr<BaseHittable> object) { m_Objects.push_back(object); }

   std::shared_ptr<BaseHittable>& operator[] (int idx)       { return m_Objects[idx]; }
   std::shared_ptr<BaseHittable>  operator[] (int idx) const { return m_Objects[idx]; }

   size_t Count() const { return m_Objects.size(); }

   std::vector<std::shared_ptr<BaseHittable>>::iterator begin() { return m_Objects.begin(); }
   std::vector<std::shared_ptr<BaseHittable>>::iterator end() { return m_Objects.end(); }

   std::vector<std::shared_ptr<BaseHittable>>::const_iterator begin() const { return m_Objects.begin(); }
   std::vector<std::shared_ptr<BaseHittable>>::const_iterator end() const { return m_Objects.end(); }

   bool Hit(const Ray& ray, float minDist, float maxDist, HitInfo& hitInfo) const override;
private:
   std::vector<std::shared_ptr<BaseHittable>> m_Objects;
};

#endif