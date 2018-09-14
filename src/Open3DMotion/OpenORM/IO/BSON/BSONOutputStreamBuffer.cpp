/*--
  Open3DMotion 
  Copyright (c) 2004-2018.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "BSONOutputStreamBuffer.h"

namespace Open3DMotion
{
		BSONOutputStreamBuffer::BSONOutputStreamBuffer(UInt8* ndata, UInt32 nlength) :
      data(ndata), length(nlength), offset(0)
    {

    }  

		void BSONOutputStreamBuffer::WriteBinary(const void* binary, UInt32 size)  throw(BSONWriteException)
    {
      if (offset + size > length)
        throw BSONWriteException("exceeded buffer size");
      memcpy(data + offset, binary, size);
      offset += size;
    }
}