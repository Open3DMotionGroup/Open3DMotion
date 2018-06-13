#include "BSONObjectIdList.h"

namespace Open3DMotion
{
  const char* BSONObjectIdList::ItemName = "BSONObjectIdListItem";

  BSONObjectIdList::BSONObjectIdList() :
    TreeList(BSONObjectIdList::ItemName)
  {
  }
}
