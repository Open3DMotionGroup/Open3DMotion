/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Branches/TreeList.h"
#include "Open3DMotion/OpenORM/Leaves/TreeInt32.h"
#include "Open3DMotion/OpenORM/Leaves/TreeBinary.h"
#include "Open3DMotion/OpenORM/Leaves/MemoryHandlerBasic.h"

#include <cppunit/extensions/HelperMacros.h>

using namespace Open3DMotion;
using namespace std;

class TestTreeList  : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestTreeList );
	CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testNewBlank );
	CPPUNIT_TEST( testCopyFrom );
	CPPUNIT_TEST( testAdd );
	CPPUNIT_TEST( testClear );
	CPPUNIT_TEST_SUITE_END();

public:
	void testConstructor()
	{
		TreeList* tl = new TreeList("ThisMightBeMyName");
		CPPUNIT_ASSERT_EQUAL(string("ThisMightBeMyName"), tl->ElementName());
		CPPUNIT_ASSERT_EQUAL(0, strcmp("ThisMightBeMyName", tl->ElementName().c_str()));
		delete tl;
	}

	void testNewBlank()
	{
		TreeList a("Yes");
		a.Add(new TreeInt32(-29));
		CPPUNIT_ASSERT_EQUAL(size_t(1), a.NumElements());

		TreeList* b = a.NewBlank();
		CPPUNIT_ASSERT_EQUAL(string("Yes"), b->ElementName());
		CPPUNIT_ASSERT_EQUAL(size_t(0), b->NumElements());
		delete b;
	}

	void testCopyFrom()
	{
		TreeList a("Yes");
		a.Add(new TreeInt32(-29));
		a.Add(new TreeInt32(-30));
		a.Add(new TreeInt32(30290));
		CPPUNIT_ASSERT_EQUAL(size_t(3), a.NumElements());

		TreeList b("NoName");
		b.Add(new TreeInt32(52));
		
		b.CopyFrom(&a);
		CPPUNIT_ASSERT_EQUAL(string("Yes"), b.ElementName());
		CPPUNIT_ASSERT_EQUAL(size_t(3), b.NumElements());
		CPPUNIT_ASSERT_EQUAL(Int32(-29), TreeValueCast<TreeInt32>( b.ElementArray()[0] )->Value() );
		CPPUNIT_ASSERT_EQUAL(Int32(-30), TreeValueCast<TreeInt32>( b.ElementArray()[1] )->Value() );
		CPPUNIT_ASSERT_EQUAL(Int32(30290), TreeValueCast<TreeInt32>( b.ElementArray()[2] )->Value() );
	}

	void testAdd()
	{
		TreeList* tl = new TreeList("Bob");
		CPPUNIT_ASSERT_EQUAL(string("Bob"), tl->ElementName());
	
		// add three elements
		tl->Add(new TreeInt32(82));
		tl->Add(new TreeInt32(-3));
		tl->Add(new TreeInt32(999));

		// verify length and values
		CPPUNIT_ASSERT_EQUAL(size_t(3), tl->NumElements());
		CPPUNIT_ASSERT_EQUAL(Int32(82), TreeValueCast<TreeInt32>( tl->ElementArray()[0] )->Value() );
		CPPUNIT_ASSERT_EQUAL(Int32(-3), TreeValueCast<TreeInt32>( tl->ElementArray()[1] )->Value() );
		CPPUNIT_ASSERT_EQUAL(Int32(999), TreeValueCast<TreeInt32>( tl->ElementArray()[2] )->Value() );

		// use a binary blob as example of another type
		// and also a convenient way to check that the item is referenced/dereferenced to avoid memory leaks
		MemoryHandlerBasic m(5);
		CPPUNIT_ASSERT_EQUAL(size_t(1), m.RefCount());
		tl->Add(new TreeBinary(m.Clone()));

		// check it's on there ok
		CPPUNIT_ASSERT_EQUAL(size_t(4), tl->NumElements());
		CPPUNIT_ASSERT_EQUAL(size_t(5), TreeValueCast<TreeBinary>( tl->ElementArray()[3] )->SizeBytes());

		// re-check other elements
		CPPUNIT_ASSERT_EQUAL(Int32(82), TreeValueCast<TreeInt32>( tl->ElementArray()[0] )->Value() );
		CPPUNIT_ASSERT_EQUAL(Int32(-3), TreeValueCast<TreeInt32>( tl->ElementArray()[1] )->Value() );
		CPPUNIT_ASSERT_EQUAL(Int32(999), TreeValueCast<TreeInt32>( tl->ElementArray()[2] )->Value() );

		// should now have 2 references as it's referenced by a tree binary object on the tree...
		CPPUNIT_ASSERT_EQUAL(size_t(2), m.RefCount());

		// ...but if we delete the list it should go back to one reference again
		delete tl;
		CPPUNIT_ASSERT_EQUAL(size_t(1), m.RefCount());
	}

	void testClear()
	{
		TreeList* tl = new TreeList("Bob");
		CPPUNIT_ASSERT_EQUAL(string("Bob"), tl->ElementName());
	
		// add two integer elements
		tl->Add(new TreeInt32(82));
		tl->Add(new TreeInt32(-3));

		// and binary handler
		MemoryHandlerBasic m(5);
		tl->Add(new TreeBinary(m.Clone()));

		CPPUNIT_ASSERT_EQUAL(size_t(3), tl->NumElements());
		CPPUNIT_ASSERT_EQUAL(size_t(2), m.RefCount());

		tl->Clear();
		CPPUNIT_ASSERT_EQUAL(size_t(0), tl->NumElements());
		CPPUNIT_ASSERT_EQUAL(size_t(0), tl->ElementArray().size());
		CPPUNIT_ASSERT_EQUAL(size_t(1), m.RefCount());

		delete tl;
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestTreeList );
