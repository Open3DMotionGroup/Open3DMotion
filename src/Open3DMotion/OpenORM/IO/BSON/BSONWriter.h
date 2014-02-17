/*--
 Open3DMotion
 Copyright (c) 2004-2014.
 All rights reserved.
 See LICENSE.txt for more information.
 --*/

#ifndef _ORMPP_BSON_WRITER_H_
#define _ORMPP_BSON_WRITER_H_

#include "Open3DMotion/OpenORM/TreeValue.h"
#include "Open3DMotion/OpenORM/Branches/TreeCompound.h"
#include "Open3DMotion/OpenORM/Branches/TreeList.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONWriteException.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONOutputStream.h"

namespace Open3DMotion
{
	class BinMemFactory;
  
	/** Read from a stream of BSON-encoded data and translate into OpenORM++ objects */
	class BSONWriter
	{
	public:
    
		/** Construct for given stream */
		BSONWriter(BSONOutputStream& _output);
    
		/** Virtual destructor */
		virtual ~BSONWriter();
    
	public:
    
		/** Write whole document - any unsupported fields are skipped */
		void WriteDocument(const TreeCompound& compound)  throw(BSONWriteException);
    
		/** Read document as a list (require first item to be a string containing element name, returns NULL if not a string there) */
		void WriteList(const TreeList& tlist)  throw(BSONWriteException);
    
		/** Write next BSON element in stream
     
     @param name name of element to write
     @param value value to write
     @return true if value class was supported, false otherwise
     */
		bool WriteElement(const std::string& name, const TreeValue& value)  throw(BSONWriteException);
    
		/** Read a C-string (ASCII subset of UTF-8) */
		void WriteCString(const std::string& s) throw(BSONWriteException);
    
		/** Read generic UTF-8 string */
		void WriteString(const std::string& s) throw(BSONWriteException);
    
	public:
    
		/** Write a specified amount of data
     @param pointer to buffer of data to write
     @param size the amount of data to write, in bytes
     */
		void WriteBinary(const void* binary, UInt32 size)  throw(BSONWriteException)
		{ output.WriteBinary(binary, size); }
    
	private:
		BSONOutputStream& output;
	};

#endif