/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Leaves/MemoryHandlerNull.h"
#include <cppunit/extensions/HelperMacros.h>

using namespace Open3DMotion;

class TestMemoryHandlerNull  : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestMemoryHandlerNull );
	CPPUNIT_TEST( testCreate );
	CPPUNIT_TEST( testClone );
	CPPUNIT_TEST_SUITE_END();

public:

	void testCreate()
	{
		MemoryHandler* m = new MemoryHandlerNull();
		CPPUNIT_ASSERT_EQUAL(size_t(0), m->SizeBytes());
		CPPUNIT_ASSERT_EQUAL((UInt8*)(NULL), m->Data());
		delete m;
	}

	void testClone()
	{
		MemoryHandler* m = new MemoryHandlerNull();
		MemoryHandler* q = m->Clone();
		CPPUNIT_ASSERT_EQUAL(size_t(0), q->SizeBytes());
		CPPUNIT_ASSERT_EQUAL((UInt8*)(NULL), q->Data());
		delete m;
		delete q;
	}


};

CPPUNIT_TEST_SUITE_REGISTRATION( TestMemoryHandlerNull );
