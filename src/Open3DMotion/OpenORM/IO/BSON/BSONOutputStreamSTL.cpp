/*--
 Open3DMotion
 Copyright (c) 2004-2014.
 All rights reserved.
 See LICENSE.txt for more information.
 --*/

#include "BSONOutputStreamSTL.h"

namespace Open3DMotion
{
  BSONOutputStreamSTL::BSONOutputStreamSTL(std::ostream& _output) :
    output(_output)
  {
  }
  
  BSONOutputStreamSTL::~BSONOutputStreamSTL()
  {
  }
  
  void BSONOutputStreamSTL::WriteBinary(const void* binary, UInt32 size)  throw(BSONWriteException)
  {
    output.write((char*)binary, size);
  }
}
