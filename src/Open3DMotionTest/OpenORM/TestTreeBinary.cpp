/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Leaves/TreeBinary.h"
#include "Open3DMotion/OpenORM/Leaves/MemoryHandlerBasic.h"
#include <cppunit/extensions/HelperMacros.h>

using namespace Open3DMotion;
using namespace std;

class TestTreeBinary  : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestTreeBinary );
	CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testSetData );
	CPPUNIT_TEST( testNewBlank );
	CPPUNIT_TEST( testCopyFrom );
	CPPUNIT_TEST_SUITE_END();

public:
	void testConstructor()
	{
		TreeBinary a;
		CPPUNIT_ASSERT_EQUAL(size_t(0), a.SizeBytes());
		CPPUNIT_ASSERT_EQUAL((UInt8*)NULL, a.Data());

		MemoryHandlerBasic mb(5);
		CPPUNIT_ASSERT_EQUAL(size_t(1), mb.RefCount());
		memcpy(mb.Data(), (UInt8*)"Bob!", 5);
		TreeBinary b(mb.Clone());
		CPPUNIT_ASSERT_EQUAL(size_t(5), b.SizeBytes());
		CPPUNIT_ASSERT_EQUAL(string("Bob!"), string((const char*)b.Data()));
		CPPUNIT_ASSERT_EQUAL(size_t(2), mb.RefCount());
		CPPUNIT_ASSERT(b.BinMemory()->Data() == mb.Data());
	}

	void testSetData()
	{
		// raw data 1
		MemoryHandlerBasic m1(5);
		memcpy(m1.Data(), (UInt8*)"Bob!", 5);
		CPPUNIT_ASSERT_EQUAL(size_t(1), m1.RefCount());

		// raw data 2
		MemoryHandlerBasic m2(4);
		memcpy(m2.Data(), (UInt8*)"No!", 4);
		CPPUNIT_ASSERT_EQUAL(size_t(1), m2.RefCount());

		// empty binary object
		TreeBinary a;
		CPPUNIT_ASSERT_EQUAL(size_t(0), a.SizeBytes());
		CPPUNIT_ASSERT_EQUAL((UInt8*)NULL, a.Data());

		// set first
		a.SetData(m1.Clone());
		CPPUNIT_ASSERT_EQUAL(size_t(5), a.SizeBytes());
		CPPUNIT_ASSERT_EQUAL(string("Bob!"), string((const char*)a.Data()));
		CPPUNIT_ASSERT_EQUAL(size_t(2), m1.RefCount());
		CPPUNIT_ASSERT_EQUAL(size_t(1), m2.RefCount());

		// now change to second
		a.SetData(m2.Clone());
		CPPUNIT_ASSERT_EQUAL(size_t(4), a.SizeBytes());
		CPPUNIT_ASSERT_EQUAL(string("No!"), string((const char*)a.Data()));
		CPPUNIT_ASSERT_EQUAL(size_t(1), m1.RefCount());
		CPPUNIT_ASSERT_EQUAL(size_t(2), m2.RefCount());

	}

	void testNewBlank()
	{
		MemoryHandlerBasic ma(6);
		memcpy(ma.Data(), (UInt8*)"Who?!", 6);
		CPPUNIT_ASSERT_EQUAL(size_t(1), ma.RefCount());

		TreeBinary a(ma.Clone());
		CPPUNIT_ASSERT_EQUAL(size_t(6), a.SizeBytes());
		CPPUNIT_ASSERT_EQUAL(string("Who?!"), string((const char*)a.Data()));
		CPPUNIT_ASSERT_EQUAL(size_t(2), ma.RefCount());

		TreeBinary* ptr = a.NewBlank();
		CPPUNIT_ASSERT_EQUAL(size_t(0), ptr->SizeBytes());
		CPPUNIT_ASSERT_EQUAL((UInt8*)NULL, ptr->Data());
		CPPUNIT_ASSERT_EQUAL(size_t(2), ma.RefCount());
		delete ptr;
		CPPUNIT_ASSERT_EQUAL(size_t(2), ma.RefCount());
	}

	void testCopyFrom()
	{
		TreeBinary a, b;

		MemoryHandlerBasic m(5);
		CPPUNIT_ASSERT_EQUAL(size_t(1), m.RefCount());
		memcpy(m.Data(), (UInt8*)"Bob!", 5);
		
		// make first item
		a.SetData(m.Clone());
		CPPUNIT_ASSERT_EQUAL(size_t(2), m.RefCount());

		// check b is empty
		CPPUNIT_ASSERT_EQUAL(size_t(0), b.SizeBytes());
		CPPUNIT_ASSERT_EQUAL((UInt8*)NULL, b.Data());

		// use copy-from
		b.CopyFrom(&a);

		// b should no longer be empty
		CPPUNIT_ASSERT_EQUAL(size_t(5), b.SizeBytes());
		CPPUNIT_ASSERT_EQUAL(string("Bob!"), string((const char*)b.Data()));
		
		// should be referenced from local variable m, plus TreeBinary a, and TreeBinary b
		CPPUNIT_ASSERT_EQUAL(size_t(3), m.RefCount());
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestTreeBinary );
