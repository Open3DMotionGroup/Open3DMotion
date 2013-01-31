/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
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

#include "Open3DMotion/OpenORM/IO/BSON/BSONInputStreamSTL.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONInputStreamGZ.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONTimestampHolder.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONObjectIdHolder.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONReader.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONReaderMOBL.h"

#include <cppunit/extensions/HelperMacros.h>
#include <istream>


using namespace Open3DMotion;

/* Summary
   Unit test fixture for BSON conversion to/from tree.
                                                                 */
class TestBSON : public CppUnit::TestFixture
{

public:
	CPPUNIT_TEST_SUITE( TestBSON );
	CPPUNIT_TEST( testStreamReadBinary );
	CPPUNIT_TEST( testStreamSkipBytes );
	CPPUNIT_TEST( testGZStreamReadBinary );
	CPPUNIT_TEST( testGZStreamSkipBytes );
	CPPUNIT_TEST( testReadCString );
	CPPUNIT_TEST( testReadString );
	CPPUNIT_TEST( testReadElementBool );
	CPPUNIT_TEST( testReadElementInt32 );
	CPPUNIT_TEST( testReadElementFloat64 );
	CPPUNIT_TEST( testReadElementInt64 );
	CPPUNIT_TEST( testReadElementString );
	CPPUNIT_TEST( testReadElementBinary );
	CPPUNIT_TEST( testReadElementBinaryMOBLCompatible );
	CPPUNIT_TEST( testReadBSONTimestamp ); 
	CPPUNIT_TEST( testBSONObjectIdHolder ); 
	CPPUNIT_TEST( testReadBSONObjectId ); 
	CPPUNIT_TEST( testReadDocument );
	CPPUNIT_TEST( testReadList );
	CPPUNIT_TEST( testReadDocumentElement );
	CPPUNIT_TEST( testReadListElement );
	CPPUNIT_TEST_SUITE_END();

public:

	TestBSON()
	{
	}

	/** Helper to initialise a read-write binary STL stream with data */
	class BinaryStream : public std::stringstream
	{
	public:
    BinaryStream(const void* s, std::size_t n) :
				std::stringstream(std::ios::binary | std::ios::out | std::ios::in)
    {
       write((const char*)s, n);
    }
	};


public:

	void testStreamReadBinary()
	{
		const UInt8 data[] = { 0x05, 0x02, 0xFE, 0x1A, 0x3F, 0xF1 };
		BinaryStream input(data, sizeof(data));
		
		BSONInputStreamSTL stream(input);
		BinMemFactoryDefault memfactory;
		BSONReader reader(stream, memfactory);

		UInt8 result1[2];
		reader.ReadBinary(result1, 2);
		CPPUNIT_ASSERT_EQUAL(0x05, (int)result1[0]);
		CPPUNIT_ASSERT_EQUAL(0x02, (int)result1[1]);

		UInt8 result2[3];
		reader.ReadBinary(result2, 3);
		CPPUNIT_ASSERT_EQUAL(0xFE, (int)result2[0]);
		CPPUNIT_ASSERT_EQUAL(0x1A, (int)result2[1]);
		CPPUNIT_ASSERT_EQUAL(0x3F, (int)result2[2]);

		UInt8 result3[9];
		try
		{
			reader.ReadBinary(result3, 9);
		}
		catch (const BSONReadException&)
		{
			return;
		}
		CPPUNIT_FAIL("expected BSONReadException when reading past end of data");
	}

	void testStreamSkipBytes()
	{
		const UInt8 data[] = { 0x05, 0x02, 0xFE, 0x1A, 0x3F, 0xF1 };
		std::stringstream input(std::ios::binary | std::ios::in | std::ios::out);
		input.write((char*)data, sizeof(data));
		input.seekg(0);
		
		BSONInputStreamSTL stream(input);
		BinMemFactoryDefault memfactory;
		BSONReader reader(stream, memfactory);

		UInt8 result1[2];
		reader.ReadBinary(result1, 2);
		CPPUNIT_ASSERT_EQUAL(0x05, (int)result1[0]);
		CPPUNIT_ASSERT_EQUAL(0x02, (int)result1[1]);

		reader.SkipBytes(2);

		UInt8 result2(0);
		reader.ReadBinary(&result2, 1);
		CPPUNIT_ASSERT_EQUAL(0x3F, (int)result2);

		try
		{
			reader.SkipBytes(3);
		}
		catch (const BSONReadException&)
		{
			return;
		}
		CPPUNIT_FAIL("expected BSONReadException when skipping past end of data");
	}

	void testGZStreamReadBinary()
	{
		// open input data (originally made using MakeGZTestData.java)
		std::ifstream input("Open3DMotionTest/Data/GZ/GZTestData.gz", std::ios::binary);
		CPPUNIT_ASSERT(input.good());

		// try to read it, use tiny buffer to make sure we deal with wrap-around ok
		BSONInputStreamGZ stream(input, 8);
		BinMemFactoryDefault memfactory;
		BSONReader reader(stream, memfactory);
		char result1[36];
		reader.ReadBinary(result1, 36);

		// see what happened
		CPPUNIT_ASSERT_EQUAL(std::string("Hello, I hope this uncompresses ok."), std::string(result1));
	}

	void testGZStreamSkipBytes()
	{
		// open input data (originally made using MakeGZTestData.java)
		std::ifstream input("Open3DMotionTest/Data/GZ/GZTestData.gz", std::ios::binary);
		CPPUNIT_ASSERT(input.good());

		// try to read it, use tiny buffer to make sure we deal with wrap-around ok
		BSONInputStreamGZ stream(input, 8);
		BinMemFactoryDefault memfactory;
		BSONReader reader(stream, memfactory);
		char result1[36];
		reader.ReadBinary(result1, 7);
		reader.SkipBytes(25);
		reader.ReadBinary(result1+7, 4);

		// see what happened
		// should have skipped the middle section which originally said "I hope this uncompresses"
		CPPUNIT_ASSERT_EQUAL(std::string("Hello, ok."), std::string(result1));
	}

	void testReadCString()
	{
		char data[] = "Bob";
		BinaryStream input(data, sizeof(data));
		BSONInputStreamSTL stream(input);
		BinMemFactoryDefault memfactory;
		BSONReader reader(stream, memfactory);
		std::string result;
		reader.ReadCString(result);
		CPPUNIT_ASSERT_EQUAL(size_t(3), result.size());
		CPPUNIT_ASSERT_EQUAL(std::string("Bob"), result);
	}


	void testReadString()
	{
		char data[] = { 0x05, 0x00, 0x00, 0x00,  'B', 'o', 'b', '2', '\0' };
		BinaryStream input(data, sizeof(data));
		BSONInputStreamSTL stream(input);
		BinMemFactoryDefault memfactory;
		BSONReader reader(stream, memfactory);
		std::string result;
		reader.ReadString(result);
		CPPUNIT_ASSERT_EQUAL(size_t(4), result.size());
		CPPUNIT_ASSERT_EQUAL(std::string("Bob2"), result);
	}

	void testReadElementBool()
	{
		char data[] = { 0x08, 'T', 'e', 'd', '\0', 0x01, 0x08, 'B', 'i', 'l', 'l', '\0', 0x00 };
		BinaryStream input(data, sizeof(data));
		BSONInputStreamSTL stream(input);
		BinMemFactoryDefault memfactory;
		BSONReader reader(stream, memfactory);

		std::string name;
		TreeValue* value(NULL);

		CPPUNIT_ASSERT( reader.ReadElement(name, value) );
		CPPUNIT_ASSERT_EQUAL(std::string("Ted"), name);
		CPPUNIT_ASSERT( value != NULL );
		CPPUNIT_ASSERT( value->ClassNameMatches(TreeBool::classname) );
		CPPUNIT_ASSERT_EQUAL(true, ((TreeBool*)value)->Value());

		CPPUNIT_ASSERT( reader.ReadElement(name, value) );
		CPPUNIT_ASSERT_EQUAL(std::string("Bill"), name);
		CPPUNIT_ASSERT( value != NULL );
		CPPUNIT_ASSERT( value->ClassNameMatches(TreeBool::classname) );
		CPPUNIT_ASSERT_EQUAL(false, ((TreeBool*)value)->Value());
	}

	void testReadElementInt32()
	{
		// 0xFFF0BDB8 = -1000008
		UInt8 data[] = { 0x10, 'T', 'e', 'd', '\0',    0xB8, 0xBD, 0xF0, 0xFF  };
		BinaryStream input(data, sizeof(data));
		BSONInputStreamSTL stream(input);
		BinMemFactoryDefault memfactory;
		BSONReader reader(stream, memfactory);

		std::string name;
		TreeValue* value(NULL);

		CPPUNIT_ASSERT( reader.ReadElement(name, value) );
		CPPUNIT_ASSERT_EQUAL(std::string("Ted"), name);
		CPPUNIT_ASSERT( value != NULL );
		CPPUNIT_ASSERT( value->ClassNameMatches(TreeInt32::classname) );
		CPPUNIT_ASSERT_EQUAL(Int32(-1000008), ((TreeInt32*)value)->Value());

	}

	void testReadElementInt64()
	{
		// 0xFFF0BDB8 = -1000008
		// 0x0000000D = 13
		// should truncate the rest
		UInt8 data[] = { 0x12, 'T', 'e', 'd', '\0',       0xB8, 0xBD, 0xF0, 0xFF, 0x01, 0x02, 0x03, 0x04,  
		                 0x12, 'B', 'i', 'l', 'l', '\0',  0x0D, 0x00, 0x00, 0x00, 0xA1, 0xB2, 0xC3, 0xD4};
		BinaryStream input(data, sizeof(data));
		BSONInputStreamSTL stream(input);
		BinMemFactoryDefault memfactory;
		BSONReader reader(stream, memfactory);

		std::string name;
		TreeValue* value(NULL);

		CPPUNIT_ASSERT( reader.ReadElement(name, value) );
		CPPUNIT_ASSERT_EQUAL(std::string("Ted"), name);
		CPPUNIT_ASSERT( value != NULL );
		CPPUNIT_ASSERT( value->ClassNameMatches(TreeInt32::classname) );
		CPPUNIT_ASSERT_EQUAL(Int32(-1000008), ((TreeInt32*)value)->Value());

		CPPUNIT_ASSERT( reader.ReadElement(name, value) );
		CPPUNIT_ASSERT_EQUAL(std::string("Bill"), name);
		CPPUNIT_ASSERT( value != NULL );
		CPPUNIT_ASSERT( value->ClassNameMatches(TreeInt32::classname) );
		CPPUNIT_ASSERT_EQUAL(Int32(13), ((TreeInt32*)value)->Value());

	}

	void testReadElementFloat64()
	{
		char data[1+4+8];
		data[0] = 0x01;
		strcpy(&data[1], "Ted");
		double d(-23.99993);
		memcpy(&data[5], &d, 8);

		BinaryStream input(data, sizeof(data));
		BSONInputStreamSTL stream(input);
		BinMemFactoryDefault memfactory;
		BSONReader reader(stream, memfactory);

		TreeValue* value(NULL);
		std::string name;
		CPPUNIT_ASSERT( reader.ReadElement(name, value) );
		CPPUNIT_ASSERT_EQUAL(std::string("Ted"), name);
		CPPUNIT_ASSERT( value != NULL );
		CPPUNIT_ASSERT( value->ClassNameMatches(TreeFloat64::classname) );
		CPPUNIT_ASSERT_EQUAL(-23.99993, ((TreeFloat64*)value)->Value());

	}

	void testReadElementBinary()
	{
		UInt8 data[] = { 
			0x05, 
			'T', 'e', 'd', '\0', 
			0x07, 0x00, 0x00, 0x00, 
			0x00,
			0x03, 0x04, 0x05, 0xA1, 0xB1, 0xAA, 0x11 };
		BinaryStream input(data, sizeof(data));
		BSONInputStreamSTL stream(input);
		BinMemFactoryDefault memfactory;
		BSONReader reader(stream, memfactory);
		TreeValue* value(NULL);
		std::string name;
		CPPUNIT_ASSERT( reader.ReadElement(name, value) );
		CPPUNIT_ASSERT_EQUAL(std::string("Ted"), name);
		CPPUNIT_ASSERT( value != NULL );
		CPPUNIT_ASSERT( value->ClassNameMatches(TreeBinary::classname) );
		TreeBinary* bin = (TreeBinary*)value;
		CPPUNIT_ASSERT_EQUAL(size_t(7), bin->SizeBytes());
		CPPUNIT_ASSERT_EQUAL(0x03, (int) bin->Data()[0]);
		CPPUNIT_ASSERT_EQUAL(0x04, (int) bin->Data()[1]);
		CPPUNIT_ASSERT_EQUAL(0x05, (int) bin->Data()[2]);
		CPPUNIT_ASSERT_EQUAL(0xA1, (int) bin->Data()[3]);
		CPPUNIT_ASSERT_EQUAL(0xB1, (int) bin->Data()[4]);
		CPPUNIT_ASSERT_EQUAL(0xAA, (int) bin->Data()[5]);
		CPPUNIT_ASSERT_EQUAL(0x11, (int) bin->Data()[6]);
	}

	void testReadElementBinaryMOBLCompatible()
	{
		UInt8 data[] = { 
			0x05, 
			'T', 'e', 'd', '\0', 
			0x0B, 0x00, 0x00, 0x00, 
			0x00,
			0x07, 0x00, 0x00, 0x00,	// the size of data not including this element - this is extra stuff added by MOBL writer
			0x03, 0x04, 0x05, 0xA1, 0xB1, 0xAA, 0x11 };
		BinaryStream input(data, sizeof(data));
		BSONInputStreamSTL stream(input);
		BinMemFactoryDefault memfactory;
		BSONReaderMOBL reader(stream, memfactory);
		TreeValue* value(NULL);
		std::string name;
		CPPUNIT_ASSERT( reader.ReadElement(name, value) );
		CPPUNIT_ASSERT_EQUAL(std::string("Ted"), name);
		CPPUNIT_ASSERT( value != NULL );
		CPPUNIT_ASSERT( value->ClassNameMatches(TreeBinary::classname) );
		TreeBinary* bin = (TreeBinary*)value;
		CPPUNIT_ASSERT_EQUAL(size_t(7), bin->SizeBytes());
		CPPUNIT_ASSERT_EQUAL(0x03, (int) bin->Data()[0]);
		CPPUNIT_ASSERT_EQUAL(0x04, (int) bin->Data()[1]);
		CPPUNIT_ASSERT_EQUAL(0x05, (int) bin->Data()[2]);
		CPPUNIT_ASSERT_EQUAL(0xA1, (int) bin->Data()[3]);
		CPPUNIT_ASSERT_EQUAL(0xB1, (int) bin->Data()[4]);
		CPPUNIT_ASSERT_EQUAL(0xAA, (int) bin->Data()[5]);
		CPPUNIT_ASSERT_EQUAL(0x11, (int) bin->Data()[6]);
	}

	void testReadElementString()
	{
		UInt8 data[] = { 
			0x02, 
			'T', 'e', 'd', '\0', 
			0x07, 0x00, 0x00, 0x00,
			'P', 'o', 't', 'a', 't', 'o', '\0'
		};
		BinaryStream input(data, sizeof(data));
		BSONInputStreamSTL stream(input);
		BinMemFactoryDefault memfactory;
		BSONReader reader(stream, memfactory);
		TreeValue* value(NULL);
		std::string name;
		CPPUNIT_ASSERT( reader.ReadElement(name, value) );
		CPPUNIT_ASSERT_EQUAL(std::string("Ted"), name);
		CPPUNIT_ASSERT( value != NULL );
		CPPUNIT_ASSERT( value->ClassNameMatches(TreeString::classname) );
		CPPUNIT_ASSERT_EQUAL(size_t(6), ((TreeString*)value)->Value().size());
		CPPUNIT_ASSERT_EQUAL(std::string("Potato"), ((TreeString*)value)->Value());
	}
	
	void testReadBSONTimestamp()
	{
		UInt8 data[] = { 
			0x11, 
			'T', 'e', 'd', '\0', 
			0x25, 0x00, 0x00, 0x00,
			0x09, 0x00, 0x00, 0x00
		};
		BinaryStream input(data, sizeof(data));
		BSONInputStreamSTL stream(input);
		BinMemFactoryDefault memfactory;
		BSONReader reader(stream, memfactory);
		TreeValue* value(NULL);
		std::string name;
		CPPUNIT_ASSERT( reader.ReadElement(name, value) );
		CPPUNIT_ASSERT_EQUAL(std::string("Ted"), name);
		CPPUNIT_ASSERT( value != NULL );
		CPPUNIT_ASSERT( value->ClassNameMatches(TreeCompound::classname) );
		BSONTimestampHolder holder;
		holder.FromTree(value);
		CPPUNIT_ASSERT_EQUAL(Int32(37), holder.BSONTimestamp.Increment.Value());
		CPPUNIT_ASSERT_EQUAL(Int32(9), holder.BSONTimestamp.Seconds.Value());
	}

	void testBSONObjectIdHolder()
	{
		BSONObjectIdHolder h1;
		BSONObjectIdBinary a = 
		{ 
			0x03, 0x11, 0xA1, 0xBB, 
			0x23, 0xD3, 0xC7, 0x27,
			0x87, 0x7A, 0xFF, 0x20
		};
		h1.FromBinary(a);
		CPPUNIT_ASSERT_EQUAL(std::string("0311a1bb23d3c727877aff20"), h1.BSONObjectId.Value());

		BSONObjectIdHolder h2;
		h2.BSONObjectId.Value() = "abcdef010203040506f7e8d9";
		BSONObjectIdBinary b;
		h2.ToBinary(b);
		CPPUNIT_ASSERT_EQUAL(0xAB, (int)b[ 0]);
		CPPUNIT_ASSERT_EQUAL(0xCD, (int)b[ 1]);
		CPPUNIT_ASSERT_EQUAL(0xEF, (int)b[ 2]);
		CPPUNIT_ASSERT_EQUAL(0x01, (int)b[ 3]);
		CPPUNIT_ASSERT_EQUAL(0x02, (int)b[ 4]);
		CPPUNIT_ASSERT_EQUAL(0x03, (int)b[ 5]);
		CPPUNIT_ASSERT_EQUAL(0x04, (int)b[ 6]);
		CPPUNIT_ASSERT_EQUAL(0x05, (int)b[ 7]);
		CPPUNIT_ASSERT_EQUAL(0x06, (int)b[ 8]);
		CPPUNIT_ASSERT_EQUAL(0xF7, (int)b[ 9]);
		CPPUNIT_ASSERT_EQUAL(0xE8, (int)b[10]);
		CPPUNIT_ASSERT_EQUAL(0xD9, (int)b[11]);
	}

	void testReadBSONObjectId()
	{
		UInt8 data[] = { 
			0x7, 
			'T', 'e', 'd', '\0', 
			0x12, 0xAB, 0xCD, 0x34, 0xFE, 0x56, 0x78, 0x91, 0xA3, 0xB4, 0xC7, 0xD3
		};
		BinaryStream input(data, sizeof(data));
		BSONInputStreamSTL stream(input);
		BinMemFactoryDefault memfactory;
		BSONReader reader(stream, memfactory);
		TreeValue* value(NULL);
		std::string name;
		CPPUNIT_ASSERT( reader.ReadElement(name, value) );
		CPPUNIT_ASSERT_EQUAL(std::string("Ted"), name);
		CPPUNIT_ASSERT( value != NULL );
		CPPUNIT_ASSERT( value->ClassNameMatches(TreeCompound::classname) );
		BSONObjectIdHolder holder;
		holder.FromTree(value);
		CPPUNIT_ASSERT_EQUAL(std::string("12abcd34fe567891a3b4c7d3"), holder.BSONObjectId.Value());
	}

	void testReadDocument()
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

		*(double*)(&data[29]) = 374231.23916;

		// internal consistency on doc size
		CPPUNIT_ASSERT(*(Int32*)&data[0] == (Int32)sizeof(data));

		BinaryStream input(data, sizeof(data));
		BSONInputStreamSTL stream(input);
		BinMemFactoryDefault memfactory;
		BSONReader reader(stream, memfactory);

		TreeCompound c;
		reader.ReadDocument(c);

		CPPUNIT_ASSERT_EQUAL(size_t(2), c.NumElements());
		CPPUNIT_ASSERT_EQUAL(std::string("One"), c.Node(0)->Name());
		CPPUNIT_ASSERT( c.Node(0)->Value()->ClassNameMatches(TreeString::classname) );
		CPPUNIT_ASSERT_EQUAL( std::string("Potato"), ((TreeString*)c.Node(0)->Value())->Value());
		CPPUNIT_ASSERT_EQUAL(std::string("Bigness"), c.Node(1)->Name());
		CPPUNIT_ASSERT( c.Node(1)->Value()->ClassNameMatches(TreeFloat64::classname) );
		CPPUNIT_ASSERT_EQUAL( 374231.23916, ((TreeFloat64*)c.Node(1)->Value())->Value());
	}

	void testReadDocumentElement()
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

		*(double*)(&data[36]) = 374231.23916;

		// internal consistency on doc size
		CPPUNIT_ASSERT(*(Int32*)&data[7] == ((Int32)sizeof(data) - 7));

		BinaryStream input(data, sizeof(data));
		BSONInputStreamSTL stream(input);
		BinMemFactoryDefault memfactory;
		BSONReader reader(stream, memfactory);

		TreeValue* value(NULL);
		std::string name;
		CPPUNIT_ASSERT( reader.ReadElement(name, value) );
		CPPUNIT_ASSERT_EQUAL(std::string("Stuff"), name);
		CPPUNIT_ASSERT( value != NULL );
		CPPUNIT_ASSERT( value->ClassNameMatches(TreeCompound::classname) );
		const TreeCompound& c = (TreeCompound&)(*value);
		CPPUNIT_ASSERT_EQUAL(size_t(2), c.NumElements());
		CPPUNIT_ASSERT_EQUAL(std::string("One"), c.Node(0)->Name());
		CPPUNIT_ASSERT( c.Node(0)->Value()->ClassNameMatches(TreeString::classname) );
		CPPUNIT_ASSERT_EQUAL( std::string("Potato"), ((TreeString*)c.Node(0)->Value())->Value());
		CPPUNIT_ASSERT_EQUAL(std::string("Bigness"), c.Node(1)->Name());
		CPPUNIT_ASSERT( c.Node(1)->Value()->ClassNameMatches(TreeFloat64::classname) );
		CPPUNIT_ASSERT_EQUAL( 374231.23916, ((TreeFloat64*)c.Node(1)->Value())->Value());
	}

	void testReadList()
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

		// internal consistency on doc size
		CPPUNIT_ASSERT(*(Int32*)&data[0] == (Int32)sizeof(data));

		BinaryStream input(data, sizeof(data));
		BSONInputStreamSTL stream(input);
		BinMemFactoryDefault memfactory;
		BSONReader reader(stream, memfactory);

		std::auto_ptr<TreeList> lst( reader.ReadList() );
		CPPUNIT_ASSERT( lst.get() != NULL );
		CPPUNIT_ASSERT_EQUAL(std::string("Food"), lst->ElementName());
		CPPUNIT_ASSERT_EQUAL(size_t(2), lst->NumElements());
		CPPUNIT_ASSERT( lst->ElementArray()[0]->ClassNameMatches(TreeString::classname) );
		CPPUNIT_ASSERT_EQUAL(std::string("Potato"), ((TreeString*)lst->ElementArray()[0])->Value());
		CPPUNIT_ASSERT( lst->ElementArray()[1]->ClassNameMatches(TreeString::classname) );
		CPPUNIT_ASSERT_EQUAL(std::string("Bananas"), ((TreeString*)lst->ElementArray()[1])->Value());
	}

	void testReadListElement()
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

		// internal consistency on doc size
		CPPUNIT_ASSERT(*(Int32*)&data[7] == ((Int32)sizeof(data)-7));

		BinaryStream input(data, sizeof(data));
		BSONInputStreamSTL stream(input);
		BinMemFactoryDefault memfactory;
		BSONReader reader(stream, memfactory);

		TreeValue* value(NULL);
		std::string name;
		CPPUNIT_ASSERT( reader.ReadElement(name, value) );
		CPPUNIT_ASSERT_EQUAL(std::string("Stuff"), name);
		CPPUNIT_ASSERT( value != NULL );
		CPPUNIT_ASSERT( value->ClassNameMatches(TreeList::classname) );
		const TreeList& lst = (TreeList&)(*value);
		CPPUNIT_ASSERT_EQUAL(std::string("Food"), lst.ElementName());
		CPPUNIT_ASSERT_EQUAL(size_t(2), lst.NumElements());
		CPPUNIT_ASSERT( lst.ElementArray()[0]->ClassNameMatches(TreeString::classname) );
		CPPUNIT_ASSERT_EQUAL(std::string("Potato"), ((TreeString*)lst.ElementArray()[0])->Value());
		CPPUNIT_ASSERT( lst.ElementArray()[1]->ClassNameMatches(TreeString::classname) );
		CPPUNIT_ASSERT_EQUAL(std::string("Bananas"), ((TreeString*)lst.ElementArray()[1])->Value());
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestBSON );
