#include "BSONWriterMOBL.h"
#include "Open3DMotion/OpenORM/Leaves/TreeBinary.h"

namespace Open3DMotion
{
	bool BSONWriterMOBL::WriteElement(const std::string& name, const TreeValue& value)  throw(BSONWriteException)
	{
		if (value.ClassNameMatches(TreeBinary::classname))
		{
			// 4 byte data size value, 1 byte BSON subtype indicator, 
			// then additional size value as required for MOBL, then data
			UInt8 bson_type = 0x05;
			const TreeBinary& bin = static_cast<const TreeBinary&>(value);
			UInt32 internal_data_size = (UInt32)bin.SizeBytes();	// the actual data size we have
			UInt32 bson_data_size = 4UL + internal_data_size;			// additional 4-byte data size descriptor as required
			UInt8 sub_type = 0;
			WriteBinary(&bson_type, 1);
			WriteCString(name);
			WriteBinary(&bson_data_size, 4);
			WriteBinary(&sub_type, 1);
			WriteBinary(&internal_data_size, 4);
			WriteBinary(bin.Data(), internal_data_size);
			return true;
		}
		else
		{
			return BSONWriter::WriteElement(name, value);
		}
	}

	bool BSONWriterMOBL::SizeElementValue(UInt32& size, const TreeValue& value)
	{
		if (value.ClassNameMatches(TreeBinary::classname))
		{
			// 4 byte data size value, 1 byte BSON subtype indicator, 
			// then internal 4-byte size descriptor, then data
			size = 9 + static_cast<const TreeBinary&>(value).SizeBytes();
			return true;
		}
		else
		{
			return BSONWriter::SizeElementValue(size, value);
		}
	}
}