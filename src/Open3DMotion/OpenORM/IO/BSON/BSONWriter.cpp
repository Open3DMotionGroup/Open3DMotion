/*--
 Open3DMotion
 Copyright (c) 2004-2014.
 All rights reserved.
 See LICENSE.txt for more information.
 --*/

#include "BSONWriter.h"
#include "Open3DMotion/OpenORM/Leaves/TreeFloat64.h"
#include "Open3DMotion/OpenORM/Leaves/TreeInt32.h"
#include "Open3DMotion/OpenORM/Leaves/TreeString.h"
#include "Open3DMotion/OpenORM/Leaves/TreeBinary.h"
#include "Open3DMotion/OpenORM/Leaves/TreeBool.h"

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

	UInt32 BSONWriter::SizeCString(const std::string& s)
	{
		// size of string and its null term
		return (UInt32)s.size() + 1UL;
	}

	UInt32 BSONWriter::SizeString(const std::string& s)
	{
		// 4 bytes to hold string size, then content, then null term
		return 4UL + (UInt32)s.size() + 1UL;
	}

	UInt32 BSONWriter::SizeBSONArrayIndices(size_t num_elements)
	{
		size_t digits = 1;
		size_t ten_to_power = 10;
		UInt32 size = 1;	// initial element occupied with array element name
		while (num_elements >= ten_to_power)
		{
			size_t elements_with_these_digits = ten_to_power - (ten_to_power/10);
			size += elements_with_these_digits * digits;
			ten_to_power *= 10;
			digits++;
		}
		size_t remaining_elements = num_elements - (ten_to_power/10) + 1;
		size += remaining_elements * digits;
		return size;
	}

	UInt32 BSONWriter::SizeBSONArray(const TreeList& tlist)
	{
		UInt32 size = 0;
		size += 4;	// document size
		size += SizeString(tlist.ElementName());	// element name as first item
		size_t num_supported_elements = 0;
		for (size_t index = 0; index < tlist.NumElements(); index++)
		{
			UInt32 elsize(0);
			if (SizeElementValue(elsize, *tlist.ElementArray()[index]))
			{
				size += elsize;
				num_supported_elements++;
			}
		}
		size += 1 + (UInt32)num_supported_elements;						// type id for each element and for element name
		size += SizeBSONArrayIndices(num_supported_elements);	// storage of array indices as strings (will include element name index)
		size += 1 + (UInt32)num_supported_elements;						// null terminators for those strings
		size += 1;	// document terminator
		return size;
	}

	UInt32 BSONWriter::SizeBSONDocument(const TreeCompound& tobj)
	{
		UInt32 size = 0;
		size += 4;	// doc size
		for (size_t index = 0; index < tobj.NumElements(); index++)
		{
			const TreeCompoundNode* node = tobj.Node(index);
			size += SizeElement(node->Name(), *node->Value());
		}
		size += 1;	// doc null term
		return size;
	}

	UInt32 BSONWriter::SizeElement(const std::string& name, const TreeValue& value)
	{
		UInt32 size(0);
		if (SizeElementValue(size, value))
		{
			size++;											// type ID prefix byte
			size += SizeCString(name);	// element name byte
		}
		return size;
	}

	bool BSONWriter::SizeElementValue(UInt32& size, const TreeValue& value)
	{
		size = 0;
		bool supported = false;

		// introspection to determine size of info to write
		if (value.ClassNameMatches(TreeBool::classname))
		{
			size = 1;
			supported = true;
		}
		else if (value.ClassNameMatches(TreeInt32::classname))
		{
			size = 8;
			supported = true;
		}
		else if (value.ClassNameMatches(TreeFloat64::classname))
		{
			size = 8;
			supported = true;
		}
		else if (value.ClassNameMatches(TreeString::classname))
		{
			size = SizeString(static_cast<const TreeString&>(value).Value());
			supported = true;
		}
		else if (value.ClassNameMatches(TreeBinary::classname))
		{
			// 4 byte data size value, 1 byte BSON subtype indicator, then data
			size = 5 + static_cast<const TreeBinary&>(value).SizeBytes();
			supported = true;
		}
		else if (value.ClassNameMatches(TreeCompound::classname))
		{
			size = SizeBSONDocument(static_cast<const TreeCompound&>(value));
			supported = true;
		}
		else if (value.ClassNameMatches(TreeList::classname))
		{
			size = SizeBSONArray(static_cast<const TreeList&>(value));
			supported = true;
		}

		return supported;
	}

	void BSONWriter::WriteCString(const std::string& s) throw(BSONWriteException)
	{
	}
    
	void BSONWriter::WriteString(const std::string& s) throw(BSONWriteException)
	{
	}

	void BSONWriter::WriteDocument(const TreeCompound& compound)  throw(BSONWriteException)
	{
	}

	void BSONWriter::WriteList(const TreeList& tlist)  throw(BSONWriteException)
	{
	}

	bool BSONWriter::WriteElement(const std::string& name, const TreeValue& value)  throw(BSONWriteException)
	{
		bool supported = false;
		UInt8 bson_type = 0;

		// introspection to determine size of info to write
		if (value.ClassNameMatches(TreeBool::classname))
		{
			bson_type = 0x08;
			output.WriteBinary(&bson_type, 1);			
			supported = true;
		}
		else if (value.ClassNameMatches(TreeInt32::classname))
		{
			supported = true;
		}
		else if (value.ClassNameMatches(TreeFloat64::classname))
		{
			supported = true;
		}
		else if (value.ClassNameMatches(TreeString::classname))
		{
			supported = true;
		}
		else if (value.ClassNameMatches(TreeBinary::classname))
		{
			// 4 byte data size value, 1 byte BSON subtype indicator, then data
			supported = true;
		}
		else if (value.ClassNameMatches(TreeCompound::classname))
		{
			supported = true;
		}
		else if (value.ClassNameMatches(TreeList::classname))
		{
			supported = true;
		}

		return supported;
	}
}
