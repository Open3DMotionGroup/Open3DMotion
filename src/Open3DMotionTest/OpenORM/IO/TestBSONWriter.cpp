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

/* Summary
 Unit test fixture for BSON conversion from tree.
 */
class TestBSONWriter : public CppUnit::TestFixture
{
  
public:
	CPPUNIT_TEST_SUITE( TestBSONWriter );
	CPPUNIT_TEST( testStreamWriteBinary );
  CPPUNIT_TEST( testGZStreamWriteBinary );
	CPPUNIT_TEST( testWriteCString );
	CPPUNIT_TEST( testWriteString );
	CPPUNIT_TEST( testWriteElementBool );
	CPPUNIT_TEST( testWriteElementInt32 );
	CPPUNIT_TEST( testWriteElementFloat64 );
	CPPUNIT_TEST( testWriteElementInt64 );
	CPPUNIT_TEST( testWriteElementString );
	CPPUNIT_TEST( testWriteElementBinary );
	CPPUNIT_TEST( testWriteElementBinaryMOBLCompatible );
	CPPUNIT_TEST( testWriteBSONTimestamp );
	CPPUNIT_TEST( testWriteBSONObjectId );
	CPPUNIT_TEST( testWriteDocument );
	CPPUNIT_TEST( testWriteList );
	CPPUNIT_TEST( testWriteDocumentElement );
	CPPUNIT_TEST( testWriteListElement );
	CPPUNIT_TEST_SUITE_END();
  
public:
  
	TestBSONWriter()
	{
	}
  
public:
  
	void testStreamWriteBinary()
	{
    std::ostringstream output(std::ios::binary | std::ios::out | std::ios::in);

    {
      UInt8 data[5] = { 23, 2, 255, 1, 89 };
      BSONOutputStreamSTL stream(output);
      BSONWriter writer(stream);
      writer.WriteBinary(data, 5);
    }
    
    CPPUNIT_ASSERT_EQUAL(size_t(5), static_cast<size_t>(output.tellg()));
    output.seekg(0, std::ios::beg);
    CPPUNIT_ASSERT_EQUAL( 23, output.get());
    CPPUNIT_ASSERT_EQUAL(  2, output.get());
    CPPUNIT_ASSERT_EQUAL(255, output.get());
    CPPUNIT_ASSERT_EQUAL(  1, output.get());
    CPPUNIT_ASSERT_EQUAL( 89, output.get());
	}
  
	void testGZStreamWriteBinary()
	{
    CPPUNIT_FAIL("Not implemented");
	}
  
	void testWriteCString()
	{
    CPPUNIT_FAIL("Not implemented");
	}
  
	void testWriteString()
	{
		char data[] = { 0x05, 0x00, 0x00, 0x00,  'B', 'o', 'b', '2', '\0' };
    CPPUNIT_FAIL("Not implemented");
	}
  
	void testWriteElementBool()
	{
		char data[] = { 0x08, 'T', 'e', 'd', '\0', 0x01, 0x08, 'B', 'i', 'l', 'l', '\0', 0x00 };
    CPPUNIT_FAIL("Not implemented");
	}
  
	void testWriteElementInt32()
	{
		// 0xFFF0BDB8 = -1000008
		UInt8 data[] = { 0x10, 'T', 'e', 'd', '\0',    0xB8, 0xBD, 0xF0, 0xFF  };
    CPPUNIT_FAIL("Not implemented");
	}
  
	void testWriteElementInt64()
	{
		// 0xFFF0BDB8 = -1000008
		// 0x0000000D = 13
		// should truncate the rest
		UInt8 data[] = { 0x12, 'T', 'e', 'd', '\0',       0xB8, 0xBD, 0xF0, 0xFF, 0x01, 0x02, 0x03, 0x04,
      0x12, 'B', 'i', 'l', 'l', '\0',  0x0D, 0x00, 0x00, 0x00, 0xA1, 0xB2, 0xC3, 0xD4};
    CPPUNIT_FAIL("Not implemented");
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
