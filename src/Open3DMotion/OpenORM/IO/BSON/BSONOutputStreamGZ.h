/*--
 Open3DMotion
 Copyright (c) 2004-2014.
 All rights reserved.
 See LICENSE.txt for more information.
 --*/

#ifndef _ORMPP_BSON_OUTPUT_STREAM_GZ_H_
#define _ORMPP_BSON_OUTPUT_STREAM_GZ_H_

#include "Open3DMotion/OpenORM/Types.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONWriteException.h"
#include <ostream>

namespace Open3DMotion
{
	/** Abstract base class for writing to binary streams */
	class BSONOutputStreamGZ
	{
	public:
    BSONOutputStreamGZ(std::ostream& _output);
    
		virtual ~BSONOutputStreamGZ();
    
		virtual void WriteBinary(const void* binary, UInt32 size)  throw(BSONWriteException);
    
  protected:
    std::ostream& output;
    
	};
  
}

#endif
