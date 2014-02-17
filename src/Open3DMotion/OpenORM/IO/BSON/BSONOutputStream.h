/*--
 Open3DMotion
 Copyright (c) 2004-2014.
 All rights reserved.
 See LICENSE.txt for more information.
 --*/

#ifndef _ORMPP_BSON_OUTPUT_STREAM_H_
#define _ORMPP_BSON_OUTPUT_STREAM_H_

#include "Open3DMotion/OpenORM/Types.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONWriteException.h"

namespace Open3DMotion
{
	/** Abstract base class for writing to binary streams */
	class BSONOutputStream
	{
	public:
		virtual ~BSONOutputStream()
    {
    }
    
		/** Write a specified amount of data
     @param binary pointer to buffer of data to write
     @param size the amount of data to write, in bytes
     */
		virtual void WriteBinary(const void* binary, UInt32 size)  throw(BSONWriteException) = 0;
    
	};
  
}

#endif
