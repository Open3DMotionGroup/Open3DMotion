#ifndef _ORMPP_BSON_OBJECT_ID_LIST_H_
#define _ORMPP_BSON_OBJECT_ID_LIST_H_

#include "Open3DMotion/OpenORM/Branches/TreeList.h"

namespace Open3DMotion
{
  class BSONObjectIdList : public TreeList
  {
  public:
    static const char* ItemName;

  public:
    BSONObjectIdList();
  };
}

#endif