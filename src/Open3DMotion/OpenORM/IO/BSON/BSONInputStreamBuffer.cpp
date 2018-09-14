/*--
  Open3DMotion 
  Copyright (c) 2004-2018.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "BSONInputStreamBuffer.h"

namespace Open3DMotion
{
  BSONInputStreamBuffer::BSONInputStreamBuffer(const UInt8* ndata, UInt32 nlength) :
    data(ndata), length(nlength), offset(0)
  {
  }
  
  void BSONInputStreamBuffer::SkipBytes(UInt32 count)  throw(BSONReadException)
  {
    if ((offset + count) > length)
      throw BSONReadException("attempted skip beyond end of buffer");
    offset += count;
  }

  void BSONInputStreamBuffer::ReadBinary(void* binary, UInt32 size)  throw(BSONReadException)
  {
    if ((offset + size) > length)
      throw BSONReadException("attempted read beyond end of buffer");
    memcpy(binary, data + offset, size);
    offset += size;
  }

  bool BSONInputStreamBuffer::HaveMore()
  { 
    return (offset < length); 
  }
}
