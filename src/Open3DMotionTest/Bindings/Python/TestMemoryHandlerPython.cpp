/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/Bindings/Python/MemoryHandlerPython.h"
#include "Open3DMotion/Bindings/Python/BinMemFactoryPython.h"
#include <cppunit/extensions/HelperMacros.h>
#include "PythonTotalRefCount.h"

using namespace Open3DMotion;

class TestMemoryHandlerPython  : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestMemoryHandlerPython );
	CPPUNIT_TEST( testCreate );
	CPPUNIT_TEST( testClone );
	CPPUNIT_TEST( testReference );
	CPPUNIT_TEST( testBinMemFactory );
	CPPUNIT_TEST_SUITE_END();

public:

	void setUp()
	{
		Py_Initialize();
	}

	void tearDown()
	{
		Py_Finalize();
	}

	void testCreate()
	{
		size_t refcount0 = PythonTotalRefCount();
		MemoryHandler* m = new MemoryHandlerPython(1000);
		CPPUNIT_ASSERT_EQUAL(size_t(1), static_cast<MemoryHandlerPython*> (m)->RefCount());
		CPPUNIT_ASSERT_EQUAL(size_t(1000), m->SizeBytes());
		CPPUNIT_ASSERT(m->Data() != NULL);
		m->Data()[27] = 62;
		m->Data()[997] = 65;
		CPPUNIT_ASSERT_EQUAL((UInt8)(62), m->Data()[27]);
		CPPUNIT_ASSERT_EQUAL((UInt8)(65), m->Data()[997]);
		CPPUNIT_ASSERT_EQUAL(size_t(1), static_cast<MemoryHandlerPython*> (m)->RefCount());
		delete m;
		size_t refcount1 = PythonTotalRefCount();
		CPPUNIT_ASSERT(refcount1 == refcount0);
	}

	void testClone()
	{
		size_t refcount0 = PythonTotalRefCount();

		// make object
		MemoryHandler* m = new MemoryHandlerPython(1000);
		CPPUNIT_ASSERT_EQUAL(size_t(1000), m->SizeBytes());		
		CPPUNIT_ASSERT_EQUAL(size_t(1), static_cast<MemoryHandlerPython*> (m)->RefCount());

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
		CPPUNIT_ASSERT_EQUAL(size_t(2), static_cast<MemoryHandlerPython*> (m)->RefCount());
		CPPUNIT_ASSERT_EQUAL(size_t(2), static_cast<MemoryHandlerPython*> (q)->RefCount());

		// modifying one should affect the other
		m->Data()[997] = 254;
		CPPUNIT_ASSERT_EQUAL((UInt8)(254), q->Data()[997]);

		// should happily be able to delete one and the other should still work
		delete m;
		CPPUNIT_ASSERT_EQUAL((UInt8)(62), q->Data()[27]);
		CPPUNIT_ASSERT_EQUAL((UInt8)(254), q->Data()[997]);

		// ref counter should now say 1 again
		CPPUNIT_ASSERT_EQUAL(size_t(1), static_cast<MemoryHandlerPython*> (q)->RefCount());

		// this should remove all
		delete q;
		size_t refcount1 = PythonTotalRefCount();
		CPPUNIT_ASSERT(refcount1 == refcount0);
	}

	void testReference()
	{
		size_t refcount0 = PythonTotalRefCount();

		// make object
		MemoryHandler* m = new MemoryHandlerPython(1000);
		CPPUNIT_ASSERT_EQUAL(size_t(1000), m->SizeBytes());		
		CPPUNIT_ASSERT_EQUAL(size_t(1), static_cast<MemoryHandlerPython*> (m)->RefCount());

		// make zero with two non-zero elements
		memset(m->Data(), 0, 1000);
		CPPUNIT_ASSERT_EQUAL((UInt8)(0), m->Data()[0]);
		CPPUNIT_ASSERT_EQUAL((UInt8)(0), m->Data()[999]);
		m->Data()[27] = 62;
		m->Data()[997] = 65;

		// clone
		MemoryHandler* q = m->Clone();

		{
			// now make a reference to clone
			MemoryHandlerPython r(static_cast<MemoryHandlerPython&>(*q));
		
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

		// static reference r should get destroyed and end of scope here
		}

		// all should now be back to start
		size_t refcount1 = PythonTotalRefCount();
		CPPUNIT_ASSERT(refcount1 == refcount0);
	}

	void testBinMemFactory()
	{
		size_t refcount0 = PythonTotalRefCount();
		{
			BinMemFactoryPython factory;
			std::auto_ptr<MemoryHandler> mem( factory.Allocate(37) );
			CPPUNIT_ASSERT(mem.get() != NULL);
			CPPUNIT_ASSERT_EQUAL(size_t(37), mem->SizeBytes());
			MemoryHandlerPython* mem_python = NamedClassCast<MemoryHandler, MemoryHandlerPython> ( mem.get() );
			CPPUNIT_ASSERT(mem_python != NULL);
		}
		size_t refcount1 = PythonTotalRefCount();
		CPPUNIT_ASSERT(refcount1 == refcount0);
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION( TestMemoryHandlerPython );
