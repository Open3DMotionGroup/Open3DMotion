/*--
 Open3DMotion
 Copyright (c) 2004-2014.
 All rights reserved.
 See LICENSE.txt for more information.
 --*/

#include "BSONWriter.h"
#include "BSONTimestampHolder.h"
#include "BSONObjectIdHolder.h"
#include "Open3DMotion/OpenORM/Leaves/TreeFloat64.h"
#include "Open3DMotion/OpenORM/Leaves/TreeInt32.h"
#include "Open3DMotion/OpenORM/Leaves/TreeString.h"
#include "Open3DMotion/OpenORM/Leaves/TreeBinary.h"
#include "Open3DMotion/OpenORM/Leaves/TreeBool.h"
#include <sstream>

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
			supported = true;
			
			// get compound object
			const TreeCompound& compound = static_cast<const TreeCompound&>(value);
			
			// override behaviour for certain known structures
			if (compound.GetType<TreeString>(MEMBER_NAME(BSONObjectIdHolder::BSONObjectId)))
			{
				size = BSONObjectIdBytes;
			}
			else if (compound.GetType<TreeCompound>(MEMBER_NAME(BSONTimestampHolder::BSONTimestamp)))
			{
				size = 8;
			}
			else
			{
				// normal behaviour
				size = SizeBSONDocument(compound);
			}
		}
		else if (value.ClassNameMatches(TreeList::classname))
		{
			supported = true;
			size = SizeBSONArray(static_cast<const TreeList&>(value));
		}

		return supported;
	}

	void BSONWriter::WriteCString(const std::string& s) throw(BSONWriteException)
	{
		WriteBinary(&s[0], s.size());
		UInt8 term(0);
		WriteBinary(&term, 1);
	}
    
	void BSONWriter::WriteString(const std::string& s) throw(BSONWriteException)
	{
		Int32 bytes = s.size() + 1;
		WriteBinary(&bytes, 4);
		WriteBinary(&s[0], s.size());
		UInt8 term(0);
		WriteBinary(&term, 1);
	}

	void BSONWriter::WriteDocument(const TreeCompound& compound)  throw(BSONWriteException)
	{
		UInt32 size = SizeBSONDocument(compound);
		WriteBinary(&size, 4);
		for (size_t index = 0; index < compound.NumElements(); index++)
		{
			const TreeCompoundNode* node = compound.Node(index);
			WriteElement(node->Name(), *node->Value());
		}
		UInt8 term(0);
		WriteBinary(&term, 1);
	}

	void BSONWriter::WriteList(const TreeList& tlist)  throw(BSONWriteException)
	{
		// compute and write size
		UInt32 size = SizeBSONArray(tlist);
		WriteBinary(&size, 4);
		
		// write element name as first element
		std::string str_index_first("0");
		UInt8 name_type_id(2);
		WriteBinary(&name_type_id, 1);
		WriteCString(str_index_first);
		WriteString(tlist.ElementName());
		
		// write elements with string indices "1", "2", "3", ...
		for (size_t index = 0; index < tlist.NumElements(); index++)
		{
      std::ostringstream index_string_stream;
      index_string_stream << (index+1);
      std::string index_string = index_string_stream.str();
			WriteElement(index_string, *tlist.ElementArray()[index]);
		}

		// array document terminator
		UInt8 term(0);
		WriteBinary(&term, 1);
	}

	bool BSONWriter::WriteElement(const std::string& name, const TreeValue& value)  throw(BSONWriteException)
	{
		bool supported = false;
		UInt8 bson_type = 0;

		// introspection to determine size of info to write
		if (value.ClassNameMatches(TreeBool::classname))
		{
			supported = true;
			bson_type = 0x08;
			const TreeBool& boolval = static_cast<const TreeBool&>(value);
			UInt8 value = (boolval.Value() != 0) ? 0x01 : 0x00;
			WriteBinary(&bson_type, 1);
			WriteCString(name);
			WriteBinary(&value, 1);
		}
		else if (value.ClassNameMatches(TreeInt32::classname))
		{
			supported = true;
			bson_type = 0x12;	// write as 64-bit integer
			Int32 value_lsb = static_cast<const TreeInt32&>(value).Value();
			Int32 value_msb = 0;
			WriteBinary(&bson_type, 1);
			WriteCString(name);
			WriteBinary(&value_lsb, 4);
			WriteBinary(&value_msb, 4);
		}
		else if (value.ClassNameMatches(TreeFloat64::classname))
		{
			supported = true;
			bson_type = 0x01;
			const TreeFloat64& floatval = static_cast<const TreeFloat64&>(value);
			output.WriteBinary(&bson_type, 1);
			WriteCString(name);
			WriteBinary(&floatval.Value(), 8);
		}
		else if (value.ClassNameMatches(TreeString::classname))
		{
			supported = true;
			bson_type = 0x02;
			output.WriteBinary(&bson_type, 1);
			WriteCString(name);
			WriteString(static_cast<const TreeString&>(value).Value());
		}
		else if (value.ClassNameMatches(TreeBinary::classname))
		{
			// 4 byte data size value, 1 byte BSON subtype indicator, then data
			supported = true;
			bson_type = 0x05;
			const TreeBinary& bin = static_cast<const TreeBinary&>(value);
			UInt32 data_size = (UInt32)bin.SizeBytes();
			UInt8 sub_type = 0;
			WriteBinary(&bson_type, 1);
			WriteCString(name);
			WriteBinary(&data_size, 4);
			WriteBinary(&sub_type, 1);
			WriteBinary(bin.Data(), data_size);
		}
		else if (value.ClassNameMatches(TreeCompound::classname))
		{
			supported = true;
			const TreeCompound& compound = static_cast<const TreeCompound&>(value);

			// override behaviour for certain known structures
			if (compound.GetType<TreeString>(MEMBER_NAME(BSONObjectIdHolder::BSONObjectId)))
			{
				bson_type = 0x07;
				WriteBinary(&bson_type, 1);
				WriteCString(name);
				BSONObjectIdHolder holder;
				holder.FromTree(&compound);
				BSONObjectIdBinary bin;
				holder.ToBinary(bin);
				WriteBinary(bin, BSONObjectIdBytes);
			}
			else if (compound.GetType<TreeCompound>(MEMBER_NAME(BSONTimestampHolder::BSONTimestamp)))
			{
				bson_type = 0x11;
				WriteBinary(&bson_type, 1);
				WriteCString(name);
				BSONTimestampHolder holder;
				holder.FromTree(&compound);
				WriteBinary(&holder.BSONTimestamp.Increment.Value(), 4);
				WriteBinary(&holder.BSONTimestamp.Seconds.Value(), 4);			}
			else
			{
				// normal behaviour
				bson_type = 0x03;
				WriteBinary(&bson_type, 1);
				WriteCString(name);
				WriteDocument(compound);
			}
		}
		else if (value.ClassNameMatches(TreeList::classname))
		{
			supported = true;
			bson_type = 0x04;
			const TreeList& compound = static_cast<const TreeList&>(value);
			WriteBinary(&bson_type, 1);
			WriteCString(name);
			WriteList(compound);
		}

		return supported;
	}
}
