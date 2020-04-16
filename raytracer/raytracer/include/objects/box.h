#ifndef __BOX_H_INCLUDED
#define __BOX_H_INCLUDED

#include "objects/base_object.h"


class Box : public BaseObject
{
   friend class IntersectMng;
public:
   Box(
      glm::vec3 center,
      glm::vec3 dim,
      std::shared_ptr<BaseMaterial> material
   ) : m_Center(center) {}
   
private:
   glm::vec3 m_Center;
   glm::vec3 m_Dim;
};

#endif
