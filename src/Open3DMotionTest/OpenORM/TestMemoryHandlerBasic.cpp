/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Leaves/MemoryHandlerBasic.h"
#include <cppunit/extensions/HelperMacros.h>

using namespace Open3DMotion;

class TestMemoryHandlerBasic  : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestMemoryHandlerBasic );
	CPPUNIT_TEST( testCreate );
	CPPUNIT_TEST( testClone );
	CPPUNIT_TEST( testReference );
	CPPUNIT_TEST_SUITE_END();

public:

	void testCreate()
	{
		MemoryHandler* m = new MemoryHandlerBasic(1000);
		CPPUNIT_ASSERT_EQUAL(size_t(1), static_cast<MemoryHandlerBasic*> (m)->RefCount());
		CPPUNIT_ASSERT_EQUAL(size_t(1000), m->SizeBytes());
		CPPUNIT_ASSERT(m->Data() != NULL);
		m->Data()[27] = 62;
		m->Data()[997] = 65;
		CPPUNIT_ASSERT_EQUAL((UInt8)(62), m->Data()[27]);
		CPPUNIT_ASSERT_EQUAL((UInt8)(65), m->Data()[997]);
		CPPUNIT_ASSERT_EQUAL(size_t(1), static_cast<MemoryHandlerBasic*> (m)->RefCount());
		delete m;
	}

	void testClone()
	{
		// make object
		MemoryHandler* m = new MemoryHandlerBasic(1000);
		CPPUNIT_ASSERT_EQUAL(size_t(1000), m->SizeBytes());		
		CPPUNIT_ASSERT_EQUAL(size_t(1), static_cast<MemoryHandlerBasic*> (m)->RefCount());

		// make zero with two non-zero elements
		memset(m->Data(), 0, 1000);
		CPPUNIT_ASSERT_EQUAL((UInt8)(0), m->Data()[0]);
		CPPUNIT_ASSERT_EQUAL((UInt8)(0), m->Data()[999]);
		m->Data()[27] = 62;
		m->Data()[997] = 65;

		// clone
		MemoryHandler* q = m->Clone();

		// elements should match
		CPPUNIT_ASSERT_EQUAL(size_t(1000), q->SizeBytes());
		CPPUNIT_ASSERT_EQUAL((UInt8)(62), q->Data()[27]);
		CPPUNIT_ASSERT_EQUAL((UInt8)(65), q->Data()[997]);

		// clone is supposed to be shallow copy only (so pointer will be identical)
		CPPUNIT_ASSERT(m->Data() == q->Data());

		// ref counter should now say 2 for both
		CPPUNIT_ASSERT_EQUAL(size_t(2), static_cast<MemoryHandlerBasic*> (m)->RefCount());
		CPPUNIT_ASSERT_EQUAL(size_t(2), static_cast<MemoryHandlerBasic*> (q)->RefCount());

		// modifying one should affect the other
		m->Data()[997] = 254;
		CPPUNIT_ASSERT_EQUAL((UInt8)(254), q->Data()[997]);

		// should happily be able to delete one and the other should still work
		delete m;
		CPPUNIT_ASSERT_EQUAL((UInt8)(62), q->Data()[27]);
		CPPUNIT_ASSERT_EQUAL((UInt8)(254), q->Data()[997]);

		// ref counter should now say 1 again
		CPPUNIT_ASSERT_EQUAL(size_t(1), static_cast<MemoryHandlerBasic*> (q)->RefCount());

		// this should remove all
		delete q;
	}

	void testReference()
	{
		// make object
		MemoryHandler* m = new MemoryHandlerBasic(1000);
		CPPUNIT_ASSERT_EQUAL(size_t(1000), m->SizeBytes());		
		CPPUNIT_ASSERT_EQUAL(size_t(1), static_cast<MemoryHandlerBasic*> (m)->RefCount());

		// make zero with two non-zero elements
		memset(m->Data(), 0, 1000);
		CPPUNIT_ASSERT_EQUAL((UInt8)(0), m->Data()[0]);
		CPPUNIT_ASSERT_EQUAL((UInt8)(0), m->Data()[999]);
		m->Data()[27] = 62;
		m->Data()[997] = 65;

		// clone
		MemoryHandler* q = m->Clone();

		// now make a reference to clone
		MemoryHandlerBasic r(static_cast<MemoryHandlerBasic&>(*q));
		
		// currently 3 items referencing
		CPPUNIT_ASSERT_EQUAL(size_t(3), r.RefCount());

		// should be able to remove q and m and still work
		delete m;
		delete q;

		// elements should match
		CPPUNIT_ASSERT_EQUAL(size_t(1000), r.SizeBytes());
		CPPUNIT_ASSERT_EQUAL((UInt8)(62), r.Data()[27]);
		CPPUNIT_ASSERT_EQUAL((UInt8)(65), r.Data()[997]);
		CPPUNIT_ASSERT_EQUAL(size_t(1), r.RefCount());
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION( TestMemoryHandlerBasic );
