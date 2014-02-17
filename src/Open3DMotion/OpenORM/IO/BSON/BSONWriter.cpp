/*--
 Open3DMotion
 Copyright (c) 2004-2014.
 All rights reserved.
 See LICENSE.txt for more information.
 --*/

#include "BSONWriter.h"

namespace Open3DMotion
{
  /** Construct for given stream */
  BSONWriter::BSONWriter(BSONOutputStream& _output) :
    output(_output)
  {
  }
  
  /** Virtual destructor */
  BSONWriter::~BSONWriter()
  {
  }
}