/*--
 Open3DMotion
 Copyright (c) 2004-2014.
 All rights reserved.
 See LICENSE.txt for more information.
 --*/

#ifndef _ORMPP_BSON_WRITER_MOBL_H_
#define _ORMPP_BSON_WRITER_MOBL_H_

#include "Open3DMotion/OpenORM/IO/BSON/BSONWriter.h"

namespace Open3DMotion
{
	/** Write OpenORM++ objects to stream using BSON encoding,
			with modification to binary writing for MOBL support */
	class BSONWriterMOBL : public BSONWriter
	{
	public:
    
		/** Construct for given stream */
		BSONWriterMOBL(BSONOutputStream& _output) :
				BSONWriter(_output)
		{
		}
    
    
	public:
      

		/** Write any element from tree if supported, with override for MOBL binary
     
     @param name name of element to write
     @param value value to write
     @return true if value class was supported, false otherwise
     */
		virtual bool WriteElement(const std::string& name, const TreeValue& value)  throw(BSONWriteException);
    

	protected:

		/* Override of size evaluation to deal with MOBL binary */
		virtual bool SizeElementValue(UInt32& size, const TreeValue& value);

	};
}

#endif