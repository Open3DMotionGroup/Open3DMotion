/*--
 Open3DMotion
 Copyright (c) 2004-2014.
 All rights reserved.
 See LICENSE.txt for more information.
 --*/

#ifndef _ORMPP_BSON_OUTPUT_STREAM_STL_H_
#define _ORMPP_BSON_OUTPUT_STREAM_STL_H_

#include "Open3DMotion/OpenORM/IO/BSON/BSONOutputStream.h"
#include <ostream>

namespace Open3DMotion
{
	/** Implementation for writing to binary streams using STL stream */
	class BSONOutputStreamSTL : public BSONOutputStream
	{
	public:
    BSONOutputStreamSTL(std::ostream& _output);
    
		virtual ~BSONOutputStreamSTL();
    
		virtual void WriteBinary(const void* binary, UInt32 size)  throw(BSONWriteException);
    
  protected:
    std::ostream& output;
    
	};
  
}

#endif
