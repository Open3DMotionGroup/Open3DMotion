/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "BSONReader.h"

#include "Open3DMotion/OpenORM/Leaves/TreeFloat64.h"
#include "Open3DMotion/OpenORM/Leaves/TreeInt32.h"
#include "Open3DMotion/OpenORM/Leaves/TreeString.h"
#include "Open3DMotion/OpenORM/Leaves/TreeBinary.h"
#include "Open3DMotion/OpenORM/Leaves/TreeBool.h"
#include "Open3DMotion/OpenORM/Leaves/MemoryHandlerBasic.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinMemFactory.h"

#include "BSONObjectIdHolder.h"
#include "BSONObjectIdList.h"
#include "BSONTimestampHolder.h"
#include "BSONDateHolder.h"

#include <memory>

namespace Open3DMotion
{
	BSONReader::BSONReader(BSONInputStream& _input, const BinMemFactory& _memfactory) :
		input(_input),
		memfactory(_memfactory)
	{
	}

	BSONReader::~BSONReader()
	{
	}

	void BSONReader::ReadDocument(TreeCompound& compound)  throw(BSONReadException)
	{
		UInt32 size(0);
		ReadBinary(&size, 4);

		TreeValue* value = NULL;
		std::string name;
		while (ReadElement(name, value))
		{
			if (value != NULL)
			{
				if (name.size())
				{
					compound.Set(name.c_str(), value);
				}
				else
				{
					delete value;
				}
			}
		}
	}

	TreeList* BSONReader::ReadList()  throw(BSONReadException)
	{
    // Get list size
	  UInt32 size(0);
		ReadBinary(&size, 4);

    // Attempt read first element
  	std::string readname;
    TreeValue* readvalue(NULL);
		std::unique_ptr<TreeValue> value;
    {
	  	ReadElement(readname, readvalue);
      value.reset(readvalue);
    }

    // First element could be a string (in which case it is a label for list items)
    // Or special case for list of ObjectIds that have no item label
    // as used in the ODIN schema for ancestry lists
		std::unique_ptr<TreeList> lst;
		if (value.get() != NULL)
		{
      if (value->ClassNameMatches(TreeString::classname))
      {
        // Usual behaviour is just to make a new list where the first
        // BSON entry is taken to be the label used for list items
  			lst.reset(new TreeList(((TreeString*)value.get())->Value().c_str()));
      }
      else if (value->ClassNameMatches(TreeCompound::classname) &&
               TreeValueCast<TreeCompound>(value.get())->GetType<TreeString>(MEMBER_NAME(BSONObjectIdHolder::BSONObjectId)) != NULL)
      {
        // But if an Object Id is encountered instead then assume
        // it's a list of ObjectIds
        lst.reset(new BSONObjectIdList());
        lst->Add(value.release());
      }
		}

		// read all elements and add
		while (ReadElement(readname, readvalue))
		{
      value.reset(readvalue);
			if (lst.get())
			{
				lst->Add(value.release());
			}
		}

    return lst.release();
	}

	bool BSONReader::ReadElement(std::string& name, TreeValue*& value)  throw(BSONReadException)
	{
		// init value to NULL & empty name
		name.clear();
		value = NULL;

		// read code
		UInt8 elementcode(0);
		ReadBinary(&elementcode, 1);
			
		// return now if zero, means end of elements
		if (elementcode == 0x00)
		{
			return false;
		}

		// read name
		ReadCString(name);

		// read value
		value = ReadElementValue(elementcode);

		// value read ok (will have throw exception if not)
		return true;
	}

	TreeValue* BSONReader::ReadElementValue(UInt8 elementcode) throw (BSONReadException)
	{
		// read element data
		// take care that any allocated memory is either freed or left pointed to by value
		// so that it gets freed in the catch block below
		switch (elementcode)
		{
			case 0x01:	// double (float64)
			{
				std::unique_ptr<TreeFloat64> f( new TreeFloat64 );
				ReadBinary(&f->Value(), 8);
				return f.release();
			}
			
			case 0x02:	// string
			case 0x0D:	// javascript code
			case 0x0E:	// symbol (deprecated)
			{
				std::unique_ptr<TreeString> s( new TreeString );
				ReadString(s->Value());
				return s.release();
			}

			case 0x03:	// compound document
			{
				std::unique_ptr<TreeCompound> c( new TreeCompound );
				ReadDocument(*c);
				return c.release();
			}

			case 0x04:	// array document
			{
				return ReadList();
			}

			case 0x05:  // binary
			{
				UInt32 numbytes(0);
				UInt8 subtype(0);
				ReadBinary(&numbytes, 4);
				ReadBinary(&subtype, 1);
				std::unique_ptr<MemoryHandler> memory( memfactory.Allocate(numbytes) );
				ReadBinary(memory->Data(), numbytes);
				return new TreeBinary(memory.release());
			}

			case 0x08:	// boolean
			{
				UInt8 b(0);
				ReadBinary(&b, 1);
				return new TreeBool(b ? true : false);
			}

			case 0x10:	// Int32
			{
				std::unique_ptr<TreeInt32> n( new TreeInt32 );
				ReadBinary(&n->Value(), 4);
				return n.release();
			}

			case 0x12:	// Int64
			{
				Int64 x;
				ReadBinary(&x, 8);
				return new TreeInt32((Int32)x);
			}

			case 0x06:	// undefined
			case 0x0A:	// null
			case 0x7F:  // min key
			case 0xFF:  // max key
			{
				break;
			}

			case 0x07:  // object id (byte*12)
			{
				BSONObjectIdBinary binary;
				ReadBinary(binary, BSONObjectIdBytes);
				BSONObjectIdHolder holder;
				holder.FromBinary(binary);
				return holder.ToTree();
			}

			case 0x11:	// Timestamp (Int32 increment, Int32 time)
			{
				BSONTimestampHolder holder;
				ReadBinary(&holder.BSONTimestamp.Increment.Value(), 4);
				ReadBinary(&holder.BSONTimestamp.Seconds.Value(), 4);
				return holder.ToTree();
			}

			case 0x09: // 64-bit UTC date/time
			{
				BSONDateHolder holder;
				ReadBinary(&holder.BSONDate.LSB.Value(), 4);
				ReadBinary(&holder.BSONDate.MSB.Value(), 4);
				return holder.ToTree();
			}

			case 0x0B: // regular exp (cstring, cstring)
			{
				std::string s1, s2;
				ReadCString(s1);
				ReadCString(s2);
				break;
			}

			case 0x0C: // DBPointer (deprecated) (cstring, (byte*12))
			{
				std::string s1;
				ReadCString(s1);
				SkipBytes(12);
				break;
			}

			case 0x0F: // Code-with-scope (int32, string, document)
			{
				UInt32 size = 0;
				ReadBinary(&size, 4);
				SkipBytes(size-4);
				break;
			}

			default:
			{
				throw BSONReadException("unrecognised BSON element");
			}
		}

		return NULL;
	}

	void BSONReader::ReadCString(std::string& s) throw(BSONReadException)
	{
		while (true)
		{
			UInt8 b(0);
			ReadBinary(&b, 1);
			if (b == 0)
				break;
			s.push_back(b);
		}
	}

	void BSONReader::ReadString(std::string& s) throw(BSONReadException)
	{
		// size including compulsory null-terminator
		UInt32 size(0);
		ReadBinary(&size, 4);

		// read including null-terminator
		s.resize(size, '\0');
		ReadBinary(&s[0], size);
		s.resize(size-1);
	}
}
