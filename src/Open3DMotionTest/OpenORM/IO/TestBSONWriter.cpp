/*--
 Open3DMotion
 Copyright (c) 2004-2014.
 All rights reserved.
 See LICENSE.txt for more information.
 --*/

#include "Open3DMotion/OpenORM/Leaves/TreeFloat64.h"
#include "Open3DMotion/OpenORM/Leaves/TreeInt32.h"
#include "Open3DMotion/OpenORM/Leaves/TreeString.h"
#include "Open3DMotion/OpenORM/Leaves/TreeBinary.h"
#include "Open3DMotion/OpenORM/Leaves/TreeBool.h"
#include "Open3DMotion/OpenORM/Branches/TreeList.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinMemFactoryDefault.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONOutputStreamSTL.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONOutputStreamGZ.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONWriter.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONWriterMOBL.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONTimestampHolder.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONObjectIdHolder.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONObjectIdList.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONDateHolder.h"

#include <cppunit/extensions/HelperMacros.h>
#include <istream>

using namespace Open3DMotion;

namespace Open3DMotion
{
	// helper to test functions not dependent on streams
	// (those functions could be static but that would prevent inheritance)
	class BSONOutputStreamNull : public BSONOutputStream
	{
	public:
    BSONOutputStreamNull()
		{
		}
    
		virtual ~BSONOutputStreamNull()
		{
		}
    
		virtual void WriteBinary(const void* /*binary*/, UInt32 /*size*/)  throw(BSONWriteException)
		{ throw BSONWriteException(__FILE__ ": unexpected call to BSONOutputStreamNull::WriteBinary"); }
	};
}

/* Summary
 Unit test fixture for BSON conversion from tree.
 */
class TestBSONWriter : public CppUnit::TestFixture
{
  
public:
	CPPUNIT_TEST_SUITE( TestBSONWriter );

	CPPUNIT_TEST( testStreamWriteBinary );
	CPPUNIT_TEST( testSizeCString );
	CPPUNIT_TEST( testSizeString );
	CPPUNIT_TEST( testSizeBSONArrayIndices );
	CPPUNIT_TEST( testSizeElementBool );
	CPPUNIT_TEST( testSizeElementInt32 );
	CPPUNIT_TEST( testSizeElementFloat64 );
	CPPUNIT_TEST( testSizeElementString );
	CPPUNIT_TEST( testSizeElementBinary );
	CPPUNIT_TEST( testSizeBSONTimestamp );
	CPPUNIT_TEST( testSizeBSONObjectId );
	CPPUNIT_TEST( testSizeBSONDate );
	CPPUNIT_TEST( testSizeDocumentElement );
	CPPUNIT_TEST( testSizeListElement );
	CPPUNIT_TEST( testWriteCString );
	CPPUNIT_TEST( testWriteString );
	CPPUNIT_TEST( testWriteElementBool );
	CPPUNIT_TEST( testWriteElementInt32 );
	CPPUNIT_TEST( testWriteElementFloat64 );
	CPPUNIT_TEST( testWriteElementString );
	CPPUNIT_TEST( testWriteElementBinary );
	CPPUNIT_TEST( testWriteBSONTimestamp );
	CPPUNIT_TEST( testWriteBSONObjectId );
	CPPUNIT_TEST( testWriteDocumentElement );
	CPPUNIT_TEST( testWriteListElement );
	CPPUNIT_TEST( testWriteDocument );
	CPPUNIT_TEST( testWriteList );
	CPPUNIT_TEST( testSizeElementBinaryMOBLCompatible );
	CPPUNIT_TEST( testWriteElementBinaryMOBLCompatible );
	CPPUNIT_TEST( testWriteBSONObjectIdList );
	CPPUNIT_TEST( testWriteBSONDate );

	/* MOBL compatibility not yet implemented
  CPPUNIT_TEST( testGZStreamWriteBinary );
	*/
	
	CPPUNIT_TEST_SUITE_END();
  
public:
  
	TestBSONWriter()
	{
	}
  
public:
  
	void testStreamWriteBinary()
	{
    std::stringstream output(std::ios::binary | std::ios::out | std::ios::in);

    {
      UInt8 data[5] = { 23, 2, 255, 1, 89 };
      BSONOutputStreamSTL stream(output);
      BSONWriter writer(stream);
      writer.WriteBinary(data, 5);
    }
    
		output.seekp(0, std::ios::beg);
    CPPUNIT_ASSERT_EQUAL( 23, output.get());
    CPPUNIT_ASSERT_EQUAL(  2, output.get());
    CPPUNIT_ASSERT_EQUAL(255, output.get());
    CPPUNIT_ASSERT_EQUAL(  1, output.get());
    CPPUNIT_ASSERT_EQUAL( 89, output.get());
	}
  
	void testSizeCString()
	{
		BSONOutputStreamNull nullstream;
		BSONWriter writer(nullstream);
    std::string hello("hello");
		UInt32 s = writer.SizeCString(hello);
		CPPUNIT_ASSERT_EQUAL(UInt32(6), s);	// 5 bytes of string; 1 byte terminator
	}

	void testSizeString()
	{
		BSONOutputStreamNull nullstream;
		BSONWriter writer(nullstream);
    std::string teststring("Crazy, man");
		UInt32 s = writer.SizeString(teststring);
		CPPUNIT_ASSERT_EQUAL(UInt32(15), s);	// 4 bytes holding string size; 10 byte string; 1 byte terminator
	}

	void testSizeBSONArrayIndices()
	{
		// zero elements should have size 1 for element name
		CPPUNIT_ASSERT_EQUAL(UInt32(1), BSONWriter::SizeBSONArrayIndices(0));

		// 7 elements - each has one digit - so 7 bytes + 1 byte for element name
		CPPUNIT_ASSERT_EQUAL(UInt32(8), BSONWriter::SizeBSONArrayIndices(7));

		// 93 elements, 1-9 have one digit, 10-93 have two digits
		// 1 byte + 9 byte + (2*84 byte) = 178
		CPPUNIT_ASSERT_EQUAL(UInt32(178), BSONWriter::SizeBSONArrayIndices(93));

		// 20463 elements:
		//   1-9 have 1 digit
		//   10-99 have 2 digits
		//   100-999 have 3 digits
		//   1000-9999 have 4 digits
		//   10000-20463 have 5 digits
		// = 1 + 9 + 90*2 + 900*3 + 9000*4 + 10464*5 = 91210
		CPPUNIT_ASSERT_EQUAL(UInt32(91210), BSONWriter::SizeBSONArrayIndices(20463));

	}

	void testSizeElementBool()
	{ 
		BSONOutputStreamNull nullstream;
		BSONWriter writer(nullstream);
		std::string name("a");
		TreeBool value(true);
		UInt32 s = writer.SizeElement(name, value);
		CPPUNIT_ASSERT_EQUAL(UInt32(4), s);	// 1 byte type ID, 2 byte name with terminator, 1 byte boolean value
	}

	void testSizeElementInt32()
	{ 
		// written as 64-bit
		BSONOutputStreamNull nullstream;
		BSONWriter writer(nullstream);
		std::string name("a");
		TreeInt32 value(27);
		UInt32 s = writer.SizeElement(name, value);
		CPPUNIT_ASSERT_EQUAL(UInt32(11), s);	// 1 byte type ID, 2 byte name with terminator, 8 byte value
	}
	
	void testSizeElementFloat64()
	{ 
		BSONOutputStreamNull nullstream;
		BSONWriter writer(nullstream);
		std::string name("a");
		TreeFloat64 value(123.9);
		UInt32 s = writer.SizeElement(name, value);
		CPPUNIT_ASSERT_EQUAL(UInt32(11), s);	// 1 byte type ID, 2 byte name with terminator, 8 byte value
	}
	

	void testSizeElementString()
	{ 
		BSONOutputStreamNull nullstream;
		BSONWriter writer(nullstream);
		std::string name("What?");
		TreeString value("Oh, hi!");
		UInt32 s = writer.SizeElement(name, value);
		// 1 byte type ID, 5 byte name, 1 byte name term, 
		// 4 byte string length, 7 byte string chars, 1 byte null term
		CPPUNIT_ASSERT_EQUAL(UInt32(19), s);	
	}
	
	void testSizeElementBinary()
	{ 
		BSONOutputStreamNull nullstream;
		BSONWriter writer(nullstream);
		std::unique_ptr<MemoryHandlerBasic> binarydata(new MemoryHandlerBasic(650));
		std::string name("Info");
		TreeBinary value(binarydata.release());
		UInt32 s = writer.SizeElement(name, value);
		// 1 byte type ID, 4 byte name, 1 byte name term,
		// 4 byte binary length, 1 byte binary sub-type, 650 bytes binary data
		CPPUNIT_ASSERT_EQUAL(UInt32(661), s);
	}
	
	void testSizeBSONTimestamp()
	{ 
		BSONOutputStreamNull nullstream;
		BSONWriter writer(nullstream);
		MemoryHandlerBasic binarydata(650);
		std::string name("MyTime");
		BSONTimestampHolder holder;
		holder.BSONTimestamp.Increment = 25;
		holder.BSONTimestamp.Seconds = 227;
		std::unique_ptr<TreeValue> value( holder.ToTree() );
		UInt32 s = writer.SizeElement(name, *value);
		// 1 byte type ID, 6 byte name, 1 byte name term, 8 byte time stamp
		CPPUNIT_ASSERT_EQUAL(UInt32(16), s);
	}
	
	void testSizeBSONObjectId()
	{ 
		BSONOutputStreamNull nullstream;
		BSONWriter writer(nullstream);

		std::string name("MyObject");
		MemoryHandlerBasic binarydata(650);
		BSONObjectIdBinary test_id;
		memset(&test_id[0], 0, sizeof(test_id));
		BSONObjectIdHolder holder;
		holder.FromBinary(test_id);
		std::unique_ptr<TreeValue> value( holder.ToTree() );
		UInt32 s = writer.SizeElement(name, *value);

		// 1 byte type ID, 8 byte name, 1 byte name term, 12 byte time object id
		CPPUNIT_ASSERT_EQUAL(UInt32(22), s);
	}

	void testSizeBSONDate()
	{
		BSONOutputStreamNull nullstream;
		BSONWriter writer(nullstream);
		std::string name("SomeDate");
		BSONDateHolder holder;
		holder.BSONDate.MSB = 0x00000000;
		holder.BSONDate.LSB = 0xABCDEF12;
		std::unique_ptr<TreeValue> value(holder.ToTree());
		UInt32 s = writer.SizeElement(name, *value);
		// 1 byte type ID, 8 byte name, 1 byte name term, 8 byte date
		CPPUNIT_ASSERT_EQUAL(UInt32(18), s);
	}

	void testSizeDocumentElement()
	{ 
		BSONOutputStreamNull nullstream;
		BSONWriter writer(nullstream);
		std::unique_ptr<TreeCompound> tree( new TreeCompound );
		tree->Set("One", new TreeInt32(583));
		tree->Set("Other", new TreeString("Really?"));
		UInt32 s = writer.SizeElement("Doc", *tree);

		// 1 byte type id
		// 3 byte element name ("Doc")
		// 1 byte element name term
		// 4 byte document size
		// -- 1 byte type id
		//    3 byte element name ("One")
		//    1 byte element name term
		//    8 byte value
		// -- 1 byte type id
		//    5 byte element name ("Other")
		//    1 byte element name term
		//    4 byte string length
		//    7 byte string ("Really?")
		//    1 byte string term
		// 1 byte document term
		// ==
		// 42

		CPPUNIT_ASSERT_EQUAL(UInt32(42), s);
	}
	
	void testSizeListElement() 
	{ 
		BSONOutputStreamNull nullstream;
		BSONWriter writer(nullstream);
		std::unique_ptr<TreeList> items(new TreeList("Bob"));
		items->Add(new TreeInt32(10));
		items->Add(new TreeInt32(50));
		items->Add(new TreeInt32(987));
		UInt32 s = writer.SizeElement("MyList", *items);		

		// 1 byte type id
		// 6 byte element name ("MyList")
		// 1 byte element name term
		// 4 byte document size
		// -- 1 byte type id
		//    1 byte element name ("0")
		//    1 byte element name term
		//    4 byte list item name length
		//    3 byte list item name ("Bob")
		//    1 byte list item name term
		// -- 1 byte type id
		//    1 byte element name ("1")
		//    1 byte element name term
		//    8 byte value (10)
		// -- 1 byte type id
		//    1 byte element name ("2")
		//    1 byte element name term
		//    8 byte value (50)
		// -- 1 byte type id
		//    1 byte element name ("3")
		//    1 byte element name term
		//    8 byte value (587)
		// 1 byte document term
		// ==
		// 57
		CPPUNIT_ASSERT_EQUAL(UInt32(57), s);
	}
  
	void testWriteCString()
	{
		std::stringstream output(std::ios::binary | std::ios::out | std::ios::in);

		{
			BSONOutputStreamSTL stream(output);
			BSONWriter writer(stream);
			writer.WriteCString("What?");
		}

		output.seekp(0, std::ios::beg);
		CPPUNIT_ASSERT_EQUAL((int)'W', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'h', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'a', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'t', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'?', output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
	}
  
	void testWriteString()
	{
		std::stringstream output(std::ios::binary | std::ios::out | std::ios::in);

		{
			BSONOutputStreamSTL stream(output);
			BSONWriter writer(stream);
			writer.WriteString("Bob2");
		}

		output.seekp(0, std::ios::beg);
		CPPUNIT_ASSERT_EQUAL(5, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL((int)'B', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'o', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'b', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'2', output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
	}
  
	void testWriteElementBool()
	{
		// char data[] = { 0x08, 'T', 'e', 'd', '\0', 0x01, 0x08, 'B', 'i', 'l', 'l', '\0', 0x00 };

		std::stringstream output(std::ios::binary | std::ios::out | std::ios::in);

		{
			TreeBool value_true(true);
			TreeBool value_false(false);
			BSONOutputStreamSTL stream(output);
			BSONWriter writer(stream);
			writer.WriteElement("Ted", value_true);
			writer.WriteElement("Bill", value_false);
		}

		output.seekp(0, std::ios::beg);
		CPPUNIT_ASSERT_EQUAL(0x08, output.get());
		CPPUNIT_ASSERT_EQUAL((int)'T', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'e', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'d', output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0x01, output.get());
		CPPUNIT_ASSERT_EQUAL(0x08, output.get());
		CPPUNIT_ASSERT_EQUAL((int)'B', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'i', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'l', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'l', output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0x00, output.get());
	}
  
	void testWriteElementInt32()
	{
		// 0xFFF0BDB8 = -1000008
		// UInt8 data[] = { 0x10, 'T', 'e', 'd', '\0',    0xB8, 0xBD, 0xF0, 0xFF  };

		std::stringstream output(std::ios::binary | std::ios::out | std::ios::in);

		{
			TreeInt32 value(-1000008);
			BSONOutputStreamSTL stream(output);
			BSONWriter writer(stream);
			writer.WriteElement("Ted", value);
		}

		output.seekp(0, std::ios::beg);
		CPPUNIT_ASSERT_EQUAL(0x12, output.get());
		CPPUNIT_ASSERT_EQUAL((int)'T', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'e', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'d', output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0xB8, output.get());
		CPPUNIT_ASSERT_EQUAL(0xBD, output.get());
		CPPUNIT_ASSERT_EQUAL(0xF0, output.get());
		CPPUNIT_ASSERT_EQUAL(0xFF, output.get());
	}
  
	void testWriteElementFloat64()
	{
		std::stringstream output(std::ios::binary | std::ios::out | std::ios::in);

		{
			TreeFloat64 value(-138.2765);
			BSONOutputStreamSTL stream(output);
			BSONWriter writer(stream);
			writer.WriteElement("Ted", value);
		}

		output.seekp(0, std::ios::beg);
		CPPUNIT_ASSERT_EQUAL(0x01, output.get());
		CPPUNIT_ASSERT_EQUAL((int)'T', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'e', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'d', output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		double readback(0.0);
		output.read((char*)&readback, sizeof(readback));
		CPPUNIT_ASSERT_EQUAL(-138.2765, readback);
	}
  
	void testWriteElementBinary()
	{
		std::stringstream output(std::ios::binary | std::ios::out | std::ios::in);

		{
			MemoryHandlerBasic mem(7);
			mem.Data()[0] = 0xAA;
			mem.Data()[1] = 0x04;
			mem.Data()[2] = 0x05;
			mem.Data()[3] = 0xA1;
			mem.Data()[4] = 0xB1;
			mem.Data()[5] = 0xAA;
			mem.Data()[6] = 0x11;
			TreeBinary value(mem.Clone());
			BSONOutputStreamSTL stream(output);
			BSONWriter writer(stream);
			writer.WriteElement("Ted", value);
		}

		output.seekp(0, std::ios::beg);
		CPPUNIT_ASSERT_EQUAL(0x05, output.get());	// binary type
		CPPUNIT_ASSERT_EQUAL((int)'T', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'e', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'d', output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(7, output.get());	// data size
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());	// binary sub-type
		CPPUNIT_ASSERT_EQUAL(0xAA, output.get());	// data
		CPPUNIT_ASSERT_EQUAL(0x04, output.get());
		CPPUNIT_ASSERT_EQUAL(0x05, output.get());
		CPPUNIT_ASSERT_EQUAL(0xA1, output.get());
		CPPUNIT_ASSERT_EQUAL(0xB1, output.get());
		CPPUNIT_ASSERT_EQUAL(0xAA, output.get());
		CPPUNIT_ASSERT_EQUAL(0x11, output.get());
	}
  
	void testWriteElementString()
	{
		std::stringstream output(std::ios::binary | std::ios::out | std::ios::in);

		{
			TreeString value("OK!");
			BSONOutputStreamSTL stream(output);
			BSONWriter writer(stream);
			writer.WriteElement("Good", value);
		}

		output.seekp(0, std::ios::beg);
		CPPUNIT_ASSERT_EQUAL(0x02, output.get());
		CPPUNIT_ASSERT_EQUAL((int)'G', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'o', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'o', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'d', output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(4, output.get());	// string size incl null
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL((int)'O', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'K', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'!', output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
	}
	
	void testWriteBSONTimestamp()
	{
		std::stringstream output(std::ios::binary | std::ios::out | std::ios::in);

		{
			BSONTimestampHolder holder;
			holder.BSONTimestamp.Increment = 23;
			holder.BSONTimestamp.Seconds = 54;
			std::unique_ptr<TreeValue> value(holder.ToTree());
			BSONOutputStreamSTL stream(output);
			BSONWriter writer(stream);
			writer.WriteElement("t", *value);
		}

		output.seekp(0, std::ios::beg);
		CPPUNIT_ASSERT_EQUAL(0x11, output.get());
		CPPUNIT_ASSERT_EQUAL((int)'t', output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(23, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(54, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
	}
  
 
	void testWriteBSONObjectId()
	{
		std::stringstream output(std::ios::binary | std::ios::out | std::ios::in);

		{
			BSONObjectIdBinary data = { 8, 9, 7, 6, 50, 0, 70, 230, 55, 165, 255, 9 };
			BSONObjectIdHolder holder;
			holder.FromBinary(data);
			std::unique_ptr<TreeValue> value(holder.ToTree());
			BSONOutputStreamSTL stream(output);
			BSONWriter writer(stream);
			writer.WriteElement("MyID", *value);
		}

		output.seekp(0, std::ios::beg);
		CPPUNIT_ASSERT_EQUAL(0x07, output.get());
		CPPUNIT_ASSERT_EQUAL((int)'M', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'y', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'I', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'D', output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(8, output.get());
		CPPUNIT_ASSERT_EQUAL(9, output.get());
		CPPUNIT_ASSERT_EQUAL(7, output.get());
		CPPUNIT_ASSERT_EQUAL(6, output.get());
		CPPUNIT_ASSERT_EQUAL(50, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(70, output.get());
		CPPUNIT_ASSERT_EQUAL(230, output.get());
		CPPUNIT_ASSERT_EQUAL(55, output.get());
		CPPUNIT_ASSERT_EQUAL(165, output.get());
		CPPUNIT_ASSERT_EQUAL(255, output.get());
		CPPUNIT_ASSERT_EQUAL(9, output.get());
	}
  
	void testWriteDocument()
	{
		std::stringstream output(std::ios::binary | std::ios::out | std::ios::in);

		{
			std::unique_ptr<TreeCompound> tree(new TreeCompound);
			tree->Set("One", new TreeString("Potato"));
			tree->Set("Bigness", new TreeFloat64(0.0));
			BSONOutputStreamSTL stream(output);
			BSONWriter writer(stream);
			writer.WriteDocument(*tree);
		}
  
 		output.seekp(0, std::ios::beg);
		CPPUNIT_ASSERT_EQUAL(38, output.get());	// document size
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(2, output.get());	// string type id
		CPPUNIT_ASSERT_EQUAL((int)'O', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'n', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'e', output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(7, output.get());	// string size
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL((int)'P', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'o', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'t', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'a', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'t', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'o', output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(1, output.get());	// 64-bit float type id
		CPPUNIT_ASSERT_EQUAL((int)'B', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'i', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'g', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'n', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'e', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'s', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'s', output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());	// 64-bit float value (0.0)
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());	// document terminator
	}
  
	void testWriteDocumentElement()
	{
		std::stringstream output(std::ios::binary | std::ios::out | std::ios::in);

		{
			std::string name("Stuff");
			std::unique_ptr<TreeCompound> tree(new TreeCompound);
			tree->Set("One", new TreeString("Potato"));
			tree->Set("Bigness", new TreeFloat64(0.0));
			BSONOutputStreamSTL stream(output);
			BSONWriter writer(stream);
			writer.WriteElement(name, *tree);
		}

		/*
		UInt8 data[] = {
			0x03,
			'S', 't', 'u', 'f', 'f', '\0',
			0x26, 0x00, 0x00, 0x00,
			0x02,
			'O', 'n', 'e', '\0',
			0x07, 0x00, 0x00, 0x00,
			'P', 'o', 't', 'a', 't', 'o', '\0',
			0x01,
			'B', 'i', 'g', 'n', 'e', 's', 's', '\0',
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00
		};
		*/

 		output.seekp(0, std::ios::beg);
		CPPUNIT_ASSERT_EQUAL(3, output.get());	// document type id
		CPPUNIT_ASSERT_EQUAL((int)'S', output.get());	// document element name
		CPPUNIT_ASSERT_EQUAL((int)'t', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'u', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'f', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'f', output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());	// document element name terminator
		CPPUNIT_ASSERT_EQUAL(38, output.get());	// document size
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(2, output.get());	// string type id
		CPPUNIT_ASSERT_EQUAL((int)'O', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'n', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'e', output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(7, output.get());	// string size
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL((int)'P', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'o', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'t', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'a', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'t', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'o', output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(1, output.get());	// 64-bit float type id
		CPPUNIT_ASSERT_EQUAL((int)'B', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'i', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'g', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'n', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'e', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'s', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'s', output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());	// 64-bit float value (0.0)
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());	// document terminator	
	}
  
	void testWriteList()
	{
		std::stringstream output(std::ios::binary | std::ios::out | std::ios::in);

		{
			std::unique_ptr<TreeList> items(new TreeList("Aircraft"));
			items->Add(new TreeString("Plane"));
			items->Add(new TreeString("Rocket"));
			items->Add(new TreeString("Balloon"));
			BSONOutputStreamSTL stream(output);
			BSONWriter writer(stream);
			writer.WriteList(*items);
		}

 		output.seekp(0, std::ios::beg);
		CPPUNIT_ASSERT_EQUAL(63, output.get());	// array size (bytes)
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(2, output.get());	// string type id
		CPPUNIT_ASSERT_EQUAL((int)'0', output.get());	// index
		CPPUNIT_ASSERT_EQUAL(0, output.get());				// index terminator
		CPPUNIT_ASSERT_EQUAL(9, output.get());				// string size
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL((int)'A', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'i', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'r', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'c', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'r', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'a', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'f', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'t', output.get());	// index
		CPPUNIT_ASSERT_EQUAL(0, output.get());				// string terminator
		CPPUNIT_ASSERT_EQUAL(2, output.get());	// string type id
		CPPUNIT_ASSERT_EQUAL((int)'1', output.get());	// index
		CPPUNIT_ASSERT_EQUAL(0, output.get());				// index terminator
		CPPUNIT_ASSERT_EQUAL(6, output.get());				// string size
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL((int)'P', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'l', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'a', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'n', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'e', output.get());	// index
		CPPUNIT_ASSERT_EQUAL(0, output.get());				// string terminator
		CPPUNIT_ASSERT_EQUAL(2, output.get());	// string type id
		CPPUNIT_ASSERT_EQUAL((int)'2', output.get());	// index
		CPPUNIT_ASSERT_EQUAL(0, output.get());				// index terminator
		CPPUNIT_ASSERT_EQUAL(7, output.get());				// string size
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL((int)'R', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'o', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'c', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'k', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'e', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'t', output.get());	// index
		CPPUNIT_ASSERT_EQUAL(0, output.get());				// string terminator
		CPPUNIT_ASSERT_EQUAL(2, output.get());	// string type id
		CPPUNIT_ASSERT_EQUAL((int)'3', output.get());	// index
		CPPUNIT_ASSERT_EQUAL(0, output.get());				// index terminator
		CPPUNIT_ASSERT_EQUAL(8, output.get());				// string size
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL((int)'B', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'a', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'l', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'l', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'o', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'o', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'n', output.get());	// index
		CPPUNIT_ASSERT_EQUAL(0, output.get());				// string terminator
		CPPUNIT_ASSERT_EQUAL(0, output.get());				// array terminator
	}
  
	void testWriteListElement()
	{
		std::stringstream output(std::ios::binary | std::ios::out | std::ios::in);

		{
			std::unique_ptr<TreeList> items(new TreeList("Aircraft"));
			items->Add(new TreeString("Plane"));
			items->Add(new TreeString("Rocket"));
			items->Add(new TreeString("Balloon"));
			BSONOutputStreamSTL stream(output);
			BSONWriter writer(stream);
			writer.WriteElement("Stuff", *items);
		}

 		output.seekp(0, std::ios::beg);
		CPPUNIT_ASSERT_EQUAL(4, output.get());				// array type id
		CPPUNIT_ASSERT_EQUAL((int)'S', output.get());	// array name
		CPPUNIT_ASSERT_EQUAL((int)'t', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'u', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'f', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'f', output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());	// array name terminator
		CPPUNIT_ASSERT_EQUAL(63, output.get());	// array size (bytes)
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(2, output.get());	// string type id
		CPPUNIT_ASSERT_EQUAL((int)'0', output.get());	// index
		CPPUNIT_ASSERT_EQUAL(0, output.get());				// index terminator
		CPPUNIT_ASSERT_EQUAL(9, output.get());				// string size
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL((int)'A', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'i', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'r', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'c', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'r', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'a', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'f', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'t', output.get());	// index
		CPPUNIT_ASSERT_EQUAL(0, output.get());				// string terminator
		CPPUNIT_ASSERT_EQUAL(2, output.get());	// string type id
		CPPUNIT_ASSERT_EQUAL((int)'1', output.get());	// index
		CPPUNIT_ASSERT_EQUAL(0, output.get());				// index terminator
		CPPUNIT_ASSERT_EQUAL(6, output.get());				// string size
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL((int)'P', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'l', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'a', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'n', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'e', output.get());	// index
		CPPUNIT_ASSERT_EQUAL(0, output.get());				// string terminator
		CPPUNIT_ASSERT_EQUAL(2, output.get());	// string type id
		CPPUNIT_ASSERT_EQUAL((int)'2', output.get());	// index
		CPPUNIT_ASSERT_EQUAL(0, output.get());				// index terminator
		CPPUNIT_ASSERT_EQUAL(7, output.get());				// string size
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL((int)'R', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'o', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'c', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'k', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'e', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'t', output.get());	// index
		CPPUNIT_ASSERT_EQUAL(0, output.get());				// string terminator
		CPPUNIT_ASSERT_EQUAL(2, output.get());	// string type id
		CPPUNIT_ASSERT_EQUAL((int)'3', output.get());	// index
		CPPUNIT_ASSERT_EQUAL(0, output.get());				// index terminator
		CPPUNIT_ASSERT_EQUAL(8, output.get());				// string size
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL((int)'B', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'a', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'l', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'l', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'o', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'o', output.get());	// index
		CPPUNIT_ASSERT_EQUAL((int)'n', output.get());	// index
		CPPUNIT_ASSERT_EQUAL(0, output.get());				// string terminator
		CPPUNIT_ASSERT_EQUAL(0, output.get());				// array terminator
	}

	void testSizeElementBinaryMOBLCompatible()
	{ 
		BSONOutputStreamNull nullstream;
		BSONWriterMOBL writer(nullstream);
		std::unique_ptr<MemoryHandlerBasic> binarydata(new MemoryHandlerBasic(623));
		std::string name("Info");
		TreeBinary value(binarydata.release());
		UInt32 s = writer.SizeElement(name, value);
		// 1 byte type ID, 4 byte name, 1 byte name term,
		// 4 byte binary length, 1 byte binary sub-type, 
		// 4 byte internal binary length, 623 bytes binary data
		CPPUNIT_ASSERT_EQUAL(UInt32(638), s);
	}

	void testWriteElementBinaryMOBLCompatible()
	{
		std::stringstream output(std::ios::binary | std::ios::out | std::ios::in);

		{
			std::unique_ptr<MemoryHandlerBasic> mem(new MemoryHandlerBasic(7));
			mem->Data()[0] = 0xAA;
			mem->Data()[1] = 0x04;
			mem->Data()[2] = 0x05;
			mem->Data()[3] = 0xA1;
			mem->Data()[4] = 0xB1;
			mem->Data()[5] = 0xAA;
			mem->Data()[6] = 0x11;
			TreeBinary value(mem.release());
			BSONOutputStreamSTL stream(output);
			BSONWriterMOBL writer(stream);
			writer.WriteElement("Ted", value);
		}

		output.seekp(0, std::ios::beg);
		CPPUNIT_ASSERT_EQUAL(0x05, output.get());	// binary type
		CPPUNIT_ASSERT_EQUAL((int)'T', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'e', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'d', output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(11, output.get());		// 11 = 7 bytes data size plus 4-byte internal size descriptor
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(2, output.get());		// binary sub-type is two in MOBL files (the old indicator for binary)
		CPPUNIT_ASSERT_EQUAL(7, output.get());		// internal data size descriptor
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0xAA, output.get());	// data
		CPPUNIT_ASSERT_EQUAL(0x04, output.get());
		CPPUNIT_ASSERT_EQUAL(0x05, output.get());
		CPPUNIT_ASSERT_EQUAL(0xA1, output.get());
		CPPUNIT_ASSERT_EQUAL(0xB1, output.get());
		CPPUNIT_ASSERT_EQUAL(0xAA, output.get());
		CPPUNIT_ASSERT_EQUAL(0x11, output.get());
	}

	void testWriteBSONObjectIdList()
	{
		std::stringstream output(std::ios::binary | std::ios::out | std::ios::in);

		{
			BSONObjectIdHolder holder0, holder1;
			holder0.BSONObjectId = "12abcd34fe567891a3b4c7d3";
			holder1.BSONObjectId = "234a2daa11223399b3e104dd";

			std::unique_ptr<BSONObjectIdList> idlist(new BSONObjectIdList());
			idlist->Add(holder0.ToTree());
			idlist->Add(holder1.ToTree());

			TreeCompound testdata;
			testdata.Set("JustOne", new TreeInt32(1));
			testdata.Set("Stuff", idlist.release());

			BSONOutputStreamSTL stream(output);
			BSONWriterMOBL writer(stream);
			writer.WriteDocument(testdata);
		}

		const UInt8 expected[] = {
			0x40, 0x00, 0x00, 0x00,					                // int32 document size
			0x12,                                           // 64-bit number coming up
			'J', 'u', 's', 't', 'O', 'n', 'e', '\0',        // name of item
			0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // item
			0x04,                                           // list coming up
			'S', 't', 'u', 'f', 'f',  '\0',                 // name of list
			0x23, 0x00, 0x00, 0x00,					                // int32 list size
			0x07,
			'0', '\0',
			0x12, 0xAB, 0xCD, 0x34, 0xFE, 0x56, 0x78, 0x91, 0xA3, 0xB4, 0xC7, 0xD3,
			0x07,
			'1', '\0',
			0x23, 0x4A, 0x2D, 0xAA, 0x11, 0x22, 0x33, 0x99, 0xB3, 0xE1, 0x04, 0xDD,
			0x00,
			0x00
		};

		UInt8 result[64];
		output.seekp(0, std::ios::beg);
		output.read((char*)result, 64);
		CPPUNIT_ASSERT_EQUAL(0, memcmp(result, expected, 64));
	}

	void testWriteBSONDate()
	{
		// 14/09/3001 15:39:09 is 32557390742000 milliseconds since 01/01/1970 00:00:00
		// And this is 1D9C5BB5C1F0 in hex 
		BSONDateHolder holder;
		holder.BSONDate.MSB.Value() = (Int32)0x00001D9CUL;
		holder.BSONDate.LSB.Value() = (Int32)0x5BB5C1F0UL;

		// write to stream
		std::stringstream output(std::ios::binary | std::ios::out | std::ios::in);
		{
			std::unique_ptr<TreeValue> value(holder.ToTree());
			BSONOutputStreamSTL stream(output);
			BSONWriter writer(stream);
			writer.WriteElement("SomeTime", *value);
		}

		// Should give BSON date representation
		output.seekp(0, std::ios::beg);
		CPPUNIT_ASSERT_EQUAL(0x09, output.get());
		CPPUNIT_ASSERT_EQUAL((int)'S', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'o', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'m', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'e', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'T', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'i', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'m', output.get());
		CPPUNIT_ASSERT_EQUAL((int)'e', output.get());
		CPPUNIT_ASSERT_EQUAL(0, output.get());
		CPPUNIT_ASSERT_EQUAL(0xF0, output.get());  // 1D9C5BB5C1F0 backwards for little-endian
		CPPUNIT_ASSERT_EQUAL(0xC1, output.get());
		CPPUNIT_ASSERT_EQUAL(0xB5, output.get());
		CPPUNIT_ASSERT_EQUAL(0x5B, output.get());
		CPPUNIT_ASSERT_EQUAL(0x9C, output.get());
		CPPUNIT_ASSERT_EQUAL(0x1D, output.get());
		CPPUNIT_ASSERT_EQUAL(0x00, output.get());
		CPPUNIT_ASSERT_EQUAL(0x00, output.get());
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestBSONWriter );
