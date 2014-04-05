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
  
	/** Write OpenORM++ objects to stream using BSON encoding */
	class BSONWriter
	{
	public:
    
		/** Construct for given stream */
		BSONWriter(BSONOutputStream& _output);
    
		/** Virtual destructor */
		virtual ~BSONWriter();
    
	public:
      

		/** Evaluate size of C-string (ASCII subset of UTF-8) */
		UInt32 SizeCString(const std::string& s);

		/** Evaluate size of C-string (ASCII subset of UTF-8) */
		UInt32 SizeString(const std::string& s);

		/** Evaluate size of encoding of list object as BSON array - including element name written as first item */
		UInt32 SizeBSONArray(const TreeList& tlist);

		/** Evaluate size of BSON encoding of compound to a BSON document */
		UInt32 SizeBSONDocument(const TreeCompound& tobj);

		/** Evaluate size of BSON representation of element if supported
			  @param value value to evaluate 
				@return size in bytes which WriteElement will use or 0 if not supported */
		UInt32 SizeElement(const std::string& name, const TreeValue& value);

		/** Helper to evaluate the storage required for array indices in a BSON array of specified size,
		    assuming element name as first item */
		static UInt32 SizeBSONArrayIndices(size_t num_elements);

		/** Write a C-string (ASCII subset of UTF-8) */
		void WriteCString(const std::string& s) throw(BSONWriteException);
    
		/** Write generic UTF-8 string */
		void WriteString(const std::string& s) throw(BSONWriteException);

		/** Write document (used for top level without name) */
		void WriteDocument(const TreeCompound& compound)  throw(BSONWriteException);

		/** Encode a list object as top level - element name is written as first item in BSON array */
		void WriteList(const TreeList& tlist)  throw(BSONWriteException);

		/** Write any element from tree if supported
     
     @param name name of element to write
     @param value value to write
     @return true if value class was supported, false otherwise
     */
		virtual bool WriteElement(const std::string& name, const TreeValue& value)  throw(BSONWriteException);
    
	public:
    
		/** Write a specified amount of data
     @param pointer to buffer of data to write
     @param size the amount of data to write, in bytes
     */
		void WriteBinary(const void* binary, UInt32 size)  throw(BSONWriteException)
		{ output.WriteBinary(binary, size); }

	protected:

		virtual bool SizeElementValue(UInt32& size, const TreeValue& value);

	private:
		BSONOutputStream& output;
	};
}

#endif