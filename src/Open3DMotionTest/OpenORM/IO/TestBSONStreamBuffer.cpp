/*--
Open3DMotion
Copyright (c) 2004-2018.
All rights reserved.
See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/IO/BSON/BSONInputStreamBuffer.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONOutputStreamBuffer.h"

#include <cppunit/extensions/HelperMacros.h>
#include <istream>

using namespace Open3DMotion;

/* Summary
   Test streams that are intended for reading and writing of binary from/to memory buffers.
*/
class TestBSONStreamBuffer : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE(TestBSONStreamBuffer);
	CPPUNIT_TEST(testRead);
	CPPUNIT_TEST(testReadBeyondEnd);
	CPPUNIT_TEST(testReadSkip);
	CPPUNIT_TEST(testReadSkipBeyondEnd);
	CPPUNIT_TEST(testWrite);
	CPPUNIT_TEST(testWriteBeyondEnd);
	CPPUNIT_TEST_SUITE_END();

public:

	TestBSONStreamBuffer()
	{
	}

public:

	void testRead()
	{
		UInt8 testdata[5] = { 0x01, 0x02, 0x07, 0x08, 0x11 };
		BSONInputStreamBuffer stream(testdata, 5);
		UInt8 read_a[3] = { 0x00, 0x00, 0x00 };
		UInt8 read_b[2] = { 0x00, 0x00 };
		stream.ReadBinary(read_a, 3);
		stream.ReadBinary(read_b, 2);
		CPPUNIT_ASSERT_EQUAL(UInt8(0x01), read_a[0]);
		CPPUNIT_ASSERT_EQUAL(UInt8(0x02), read_a[1]);
		CPPUNIT_ASSERT_EQUAL(UInt8(0x07), read_a[2]);
		CPPUNIT_ASSERT_EQUAL(UInt8(0x08), read_b[0]);
		CPPUNIT_ASSERT_EQUAL(UInt8(0x11), read_b[1]);
	}

	void testReadBeyondEnd()
	{
		UInt8 testdata[5] = { 0xCC, 0xAA, 0xBB, 0xDD };
		BSONInputStreamBuffer stream(testdata, 4);
		UInt8 read_a[3] = { 0x00, 0x00, 0x00 };
		UInt8 read_b[2] = { 0x00, 0x00 };
		stream.ReadBinary(read_a, 3);
		CPPUNIT_ASSERT_THROW(stream.ReadBinary(read_b, 2), BSONReadException);
		CPPUNIT_ASSERT_EQUAL(UInt8(0xCC), read_a[0]);
		CPPUNIT_ASSERT_EQUAL(UInt8(0xAA), read_a[1]);
		CPPUNIT_ASSERT_EQUAL(UInt8(0xBB), read_a[2]);
		CPPUNIT_ASSERT_EQUAL(UInt8(0x00), read_b[0]);
		CPPUNIT_ASSERT_EQUAL(UInt8(0x00), read_b[1]);
	}

	void testReadSkip()
	{
		UInt8 testdata[5] = { 0x01, 0x02, 0x07, 0x08, 0x11 };
		BSONInputStreamBuffer stream(testdata, 5);
		UInt8 read_a[2] = { 0x00, 0x00 };
		UInt8 read_b[1] = { 0x00 };
		stream.ReadBinary(read_a, 2);
		stream.SkipBytes(2);
		stream.ReadBinary(read_b, 1);
		CPPUNIT_ASSERT_EQUAL(UInt8(0x01), read_a[0]);
		CPPUNIT_ASSERT_EQUAL(UInt8(0x02), read_a[1]);
		CPPUNIT_ASSERT_EQUAL(UInt8(0x11), read_b[0]);
	}

	void testReadSkipBeyondEnd()
	{
		UInt8 testdata[5] = { 0x01, 0x02, 0x07, 0x08, 0x11 };
		BSONInputStreamBuffer stream(testdata, 5);
		UInt8 read_a[2] = { 0x00, 0x00 };
		stream.ReadBinary(read_a, 2);
		CPPUNIT_ASSERT_THROW(stream.SkipBytes(4), BSONReadException);
		CPPUNIT_ASSERT_EQUAL(UInt8(0x01), read_a[0]);
		CPPUNIT_ASSERT_EQUAL(UInt8(0x02), read_a[1]);
	}

	void testWrite()
	{
		UInt8 testdata[3] = { 0x33, 0x55, 0x44 };
		UInt8 buffer[5] = { 0x00, 0x00, 0x00, 0x00, 0x00 };
		BSONOutputStreamBuffer stream(buffer, 5);
		stream.WriteBinary(testdata, 2);
		stream.WriteBinary(testdata, 1);
		CPPUNIT_ASSERT_EQUAL(UInt8(0x33), buffer[0]);
		CPPUNIT_ASSERT_EQUAL(UInt8(0x55), buffer[1]);
		CPPUNIT_ASSERT_EQUAL(UInt8(0x33), buffer[2]);
		CPPUNIT_ASSERT_EQUAL(UInt8(0x00), buffer[3]);
		CPPUNIT_ASSERT_EQUAL(UInt8(0x00), buffer[4]);
	}

	void testWriteBeyondEnd()
	{
		UInt8 testdata[3] = { 0x33, 0x55, 0x44 };
		UInt8 buffer[5] = { 0x00, 0x00, 0x00, 0x00, 0x00 };
		BSONOutputStreamBuffer stream(buffer, 5);
		stream.WriteBinary(testdata, 3);
		CPPUNIT_ASSERT_THROW(stream.WriteBinary(testdata, 3), BSONWriteException);
		CPPUNIT_ASSERT_EQUAL(UInt8(0x33), buffer[0]);
		CPPUNIT_ASSERT_EQUAL(UInt8(0x55), buffer[1]);
		CPPUNIT_ASSERT_EQUAL(UInt8(0x44), buffer[2]);
		CPPUNIT_ASSERT_EQUAL(UInt8(0x00), buffer[3]);
		CPPUNIT_ASSERT_EQUAL(UInt8(0x00), buffer[4]);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestBSONStreamBuffer);
