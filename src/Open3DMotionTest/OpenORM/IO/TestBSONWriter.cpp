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
#include "Open3DMotion/OpenORM/IO/BSON/BSONTimestampHolder.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONObjectIdHolder.h"


#include <cppunit/extensions/HelperMacros.h>
#include <istream>


using namespace Open3DMotion;

namespace Open3DMotion
{
	// helper to test functions not dependent on streams
	// (those functions could be static but that would disallow inheritance)
	class BSONOutputStreamNull : public BSONOutputStream
	{
	public:
    BSONOutputStreamNull()
		{
		}
    
		virtual ~BSONOutputStreamNull()
		{
		}
    
		virtual void WriteBinary(const void* binary, UInt32 size)  throw(BSONWriteException)
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
  CPPUNIT_TEST( testGZStreamWriteBinary );

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

	CPPUNIT_TEST( testWriteElementBinaryMOBLCompatible );

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
		MemoryHandlerBasic binarydata(650);
		std::string name("Info");
		TreeBinary value(&binarydata);
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
		std::auto_ptr<TreeValue> value( holder.ToTree() );
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
		std::auto_ptr<TreeValue> value( holder.ToTree() );
		UInt32 s = writer.SizeElement(name, *value);

		// 1 byte type ID, 8 byte name, 1 byte name term, 12 byte time object id
		CPPUNIT_ASSERT_EQUAL(UInt32(22), s);
	}
	
	void testSizeDocumentElement()
	{ 
		BSONOutputStreamNull nullstream;
		BSONWriter writer(nullstream);
		std::auto_ptr<TreeCompound> tree( new TreeCompound );
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
		std::auto_ptr<TreeList> items(new TreeList("Bob"));
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

	void testGZStreamWriteBinary()
	{
    CPPUNIT_FAIL("Not implemented");
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
		CPPUNIT_ASSERT_EQUAL(0x10, output.get());
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
		char data[1+4+8];
		data[0] = 0x01;
		strcpy(&data[1], "Ted");
		double d(-23.99993);
		memcpy(&data[5], &d, 8);
    
    CPPUNIT_FAIL("Not implemented");
    
	}
  
	void testWriteElementBinary()
	{
		UInt8 data[] = {
			0x05,
			'T', 'e', 'd', '\0',
			0x07, 0x00, 0x00, 0x00,
			0x00,
			0x03, 0x04, 0x05, 0xA1, 0xB1, 0xAA, 0x11 };
    CPPUNIT_FAIL("Not implemented");
	}
  
	void testWriteElementBinaryMOBLCompatible()
	{
		UInt8 data[] = {
			0x05,
			'T', 'e', 'd', '\0',
			0x0B, 0x00, 0x00, 0x00,
			0x00,
			0x07, 0x00, 0x00, 0x00,	// the size of data not including this element - this is extra stuff added by MOBL writer
			0x03, 0x04, 0x05, 0xA1, 0xB1, 0xAA, 0x11 };
    CPPUNIT_FAIL("Not implemented");
	}
  
	void testWriteElementString()
	{
		UInt8 data[] = {
			0x02,
			'T', 'e', 'd', '\0',
			0x07, 0x00, 0x00, 0x00,
			'P', 'o', 't', 'a', 't', 'o', '\0'
		};
    CPPUNIT_FAIL("Not implemented");
	}
	
	void testWriteBSONTimestamp()
	{
		UInt8 data[] = {
			0x11,
			'T', 'e', 'd', '\0',
			0x25, 0x00, 0x00, 0x00,
			0x09, 0x00, 0x00, 0x00
		};
    CPPUNIT_FAIL("Not implemented");
	}
  
 
	void testWriteBSONObjectId()
	{
    CPPUNIT_FAIL("Not implemented");
	}
  
	void testWriteDocument()
	{
		UInt8 data[] = {
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
    
    CPPUNIT_FAIL("Not implemented");
	}
  
	void testWriteDocumentElement()
	{
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
    
    CPPUNIT_FAIL("Not implemented");
	}
  
	void testWriteList()
	{
		UInt8 data[] = {
			0x35, 0x00, 0x00, 0x00,
			0x02,
			'1', '5', '6', '\0',
			0x05, 0x00, 0x00, 0x00,
			'F', 'o', 'o', 'd', '\0',
			0x02,
			'1', '5', '9', '\0',
			0x07, 0x00, 0x00, 0x00,
			'P', 'o', 't', 'a', 't', 'o', '\0',
			0x02,
			'3', '0', '9', '2', '\0',
			0x08, 0x00, 0x00, 0x00,
			'B', 'a', 'n', 'a', 'n', 'a', 's', '\0',
			0x00
		};
    
    CPPUNIT_FAIL("Not implemented");
	}
  
	void testWriteListElement()
	{
		UInt8 data[] = {
			0x04,
			'S', 't', 'u', 'f', 'f', '\0',
			0x35, 0x00, 0x00, 0x00,
			0x02,
			'1', '5', '6', '\0',
			0x05, 0x00, 0x00, 0x00,
			'F', 'o', 'o', 'd', '\0',
			0x02,
			'1', '5', '9', '\0',
			0x07, 0x00, 0x00, 0x00,
			'P', 'o', 't', 'a', 't', 'o', '\0',
			0x02,
			'3', '0', '9', '2', '\0',
			0x08, 0x00, 0x00, 0x00,
			'B', 'a', 'n', 'a', 'n', 'a', 's', '\0',
			0x00
		};
    
    CPPUNIT_FAIL("Not implemented");
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestBSONWriter );
