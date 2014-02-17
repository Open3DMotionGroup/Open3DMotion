/*--
 Open3DMotion
 Copyright (c) 2004-2014.
 All rights reserved.
 See LICENSE.txt for more information.
 --*/

#ifndef _ORMPP_BSON_WRITE_EXCEPTION_H_
#define _ORMPP_BSON_WRITE_EXCEPTION_H_

#include "Open3DMotion/OpenORM/IO/ORMIOException.h"

namespace Open3DMotion
{
  /** Errors produced when writing BSON to stream */
  class BSONWriteException : public ORMIOException
  {
  public:
    /** construct specified error */
    BSONWriteException(const char* msg) :
      ORMIOException(msg)
    {
    }
  };
  
}

#endif
