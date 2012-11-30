/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Branches/TreeCompound.h"
#include "Open3DMotion/OpenORM/Leaves/TreeInt32.h"
#include "Open3DMotion/OpenORM/Leaves/TreeBinary.h"
#include "Open3DMotion/OpenORM/Leaves/MemoryHandlerBasic.h"

#include <cppunit/extensions/HelperMacros.h>

using namespace Open3DMotion;
using namespace std;

class TestTreeCompound  : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestTreeCompound );
	CPPUNIT_TEST( testNode );
	CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testSetGetNode );
	CPPUNIT_TEST( testRemove );
	CPPUNIT_TEST( testClear );
	CPPUNIT_TEST( testNewBlank );
	CPPUNIT_TEST( testCopyFrom );
	CPPUNIT_TEST_SUITE_END();

public:
	void testConstructor()
	{
		TreeCompound* tc = new TreeCompound();
		CPPUNIT_ASSERT_EQUAL(size_t(0), tc->NumElements());
		delete tc;
	}

	void testNode()
	{
		// test empty constructor
		TreeCompoundNode a;
		CPPUNIT_ASSERT_EQUAL(string(""), a.Name());
		CPPUNIT_ASSERT_EQUAL((TreeValue*)NULL, a.Value());

		// test set
		MemoryHandlerBasic m(321);
		TreeCompoundNode* b = new TreeCompoundNode("Juggle", new TreeBinary(&m));
		CPPUNIT_ASSERT_EQUAL(size_t(2), m.RefCount());
		CPPUNIT_ASSERT_EQUAL(string("Juggle"), b->Name());
		CPPUNIT_ASSERT_EQUAL(size_t(321), TreeValueCast<TreeBinary>( b->Value() )->SizeBytes() );

		// test deletion of value item
		delete b;
		CPPUNIT_ASSERT_EQUAL(size_t(1), m.RefCount());

		// test modification
		TreeCompoundNode* c = new TreeCompoundNode("Juggle", new TreeBinary(&m));
		const TreeCompoundNode* c_const = c;
		CPPUNIT_ASSERT_EQUAL(size_t(2), m.RefCount());
		c->ChangeValue(new TreeInt32(876));
		CPPUNIT_ASSERT_EQUAL(size_t(1), m.RefCount());
		CPPUNIT_ASSERT_EQUAL(Int32(876), TreeValueCast<TreeInt32>( c->Value() )->Value() );
		CPPUNIT_ASSERT_EQUAL(Int32(876), TreeValueCast<TreeInt32>( c_const->Value() )->Value() );
		delete c;
	}

	void testSetGetNode()
	{
		TreeCompound* tc = new TreeCompound();
		const TreeCompound* const_tc = tc;
		
		// getting non-existent should return null
		CPPUNIT_ASSERT_EQUAL((TreeValue*) NULL, tc->Get("Nothing"));

		tc->Set("z", new TreeInt32(123));
		CPPUNIT_ASSERT_EQUAL(size_t(1), tc->NumElements());
		
		tc->Set("a", new TreeInt32(-789));
		
		// use modifiable version
		CPPUNIT_ASSERT_EQUAL(size_t(2), tc->NumElements());		
		CPPUNIT_ASSERT_EQUAL(Int32(123), TreeValueCast<TreeInt32>(tc->Node(0)->Value())->Value());
		CPPUNIT_ASSERT_EQUAL(Int32(-789), TreeValueCast<TreeInt32>(tc->Node(1)->Value())->Value());
		CPPUNIT_ASSERT_EQUAL(-Int32(789), TreeValueCast<TreeInt32>(tc->Get("a"))->Value());
		CPPUNIT_ASSERT_EQUAL( Int32(123), TreeValueCast<TreeInt32>(tc->Get("z"))->Value());

		// use const version
		CPPUNIT_ASSERT_EQUAL(size_t(2), const_tc->NumElements());		
		CPPUNIT_ASSERT_EQUAL(Int32(123), TreeValueCast<TreeInt32>(const_tc->Node(0)->Value())->Value());
		CPPUNIT_ASSERT_EQUAL(Int32(-789), TreeValueCast<TreeInt32>(const_tc->Node(1)->Value())->Value());
		CPPUNIT_ASSERT_EQUAL(Int32(-789), TreeValueCast<TreeInt32>(const_tc->Get("a"))->Value());
		CPPUNIT_ASSERT_EQUAL( Int32(123), TreeValueCast<TreeInt32>(const_tc->Get("z"))->Value());

		// getting non-existent should still return null
		CPPUNIT_ASSERT_EQUAL((TreeValue*) NULL, tc->Get("MoreOfNothing"));
		CPPUNIT_ASSERT_EQUAL((const TreeValue*) NULL, const_tc->Get("AndYetMoreNothing"));

		// add something
		tc->Set("c", new TreeInt32(1000));
		CPPUNIT_ASSERT_EQUAL(size_t(3), tc->NumElements());		
		CPPUNIT_ASSERT_EQUAL( Int32(1000), TreeValueCast<TreeInt32>(tc->Node(2)->Value())->Value());
		CPPUNIT_ASSERT_EQUAL( Int32(1000), TreeValueCast<TreeInt32>(tc->Get("c"))->Value());

		// make a mod in place
		*TreeValueCast<TreeInt32>(tc->Get("a")) = 42;
		CPPUNIT_ASSERT_EQUAL(Int32(42), TreeValueCast<TreeInt32>(tc->Node(1)->Value())->Value());
		CPPUNIT_ASSERT_EQUAL(Int32(42), TreeValueCast<TreeInt32>(const_tc->Node(1)->Value())->Value());

		// do a replace mod
		tc->Set("z", new TreeInt32(43));

		// everything should be ordered as before but just with the z value changed
		CPPUNIT_ASSERT_EQUAL(size_t(3), tc->NumElements());		
		CPPUNIT_ASSERT_EQUAL(Int32(43), TreeValueCast<TreeInt32>(tc->Node(0)->Value())->Value());
		CPPUNIT_ASSERT_EQUAL(Int32(42), TreeValueCast<TreeInt32>(tc->Node(1)->Value())->Value());
		CPPUNIT_ASSERT_EQUAL(Int32(1000), TreeValueCast<TreeInt32>(tc->Node(2)->Value())->Value());
		CPPUNIT_ASSERT_EQUAL(Int32(43), TreeValueCast<TreeInt32>(tc->Get("z"))->Value());
		CPPUNIT_ASSERT_EQUAL(Int32(42), TreeValueCast<TreeInt32>(tc->Get("a"))->Value());
		CPPUNIT_ASSERT_EQUAL(Int32(1000), TreeValueCast<TreeInt32>(tc->Get("c"))->Value());
		CPPUNIT_ASSERT_EQUAL(size_t(3), const_tc->NumElements());		
		CPPUNIT_ASSERT_EQUAL(Int32(43), TreeValueCast<TreeInt32>(const_tc->Node(0)->Value())->Value());
		CPPUNIT_ASSERT_EQUAL(Int32(42), TreeValueCast<TreeInt32>(const_tc->Node(1)->Value())->Value());
		CPPUNIT_ASSERT_EQUAL(Int32(1000), TreeValueCast<TreeInt32>(const_tc->Node(2)->Value())->Value());
		CPPUNIT_ASSERT_EQUAL(Int32(43), TreeValueCast<TreeInt32>(const_tc->Get("z"))->Value());
		CPPUNIT_ASSERT_EQUAL(Int32(42), TreeValueCast<TreeInt32>(const_tc->Get("a"))->Value());
		CPPUNIT_ASSERT_EQUAL(Int32(1000), TreeValueCast<TreeInt32>(const_tc->Get("c"))->Value());

	}

	void testRemove()
	{
		TreeCompound* tc = new TreeCompound;
	
		// add two integer elements
		tc->Set("A", new TreeInt32(82));
		tc->Set("B", new TreeInt32(-3));

		// and binary handler useful for reference counting
		MemoryHandlerBasic m(5);
		tc->Set("C", new TreeBinary(&m));

		// check num elements etc as expected
		CPPUNIT_ASSERT_EQUAL(size_t(3), tc->NumElements());

		// ref count for memory should be 1 for raw input object
		// and 1 for reference in the TreeCompound
		CPPUNIT_ASSERT_EQUAL(size_t(2), m.RefCount());

		// removing something non-existent shouldn't matter
		tc->Remove("NotThereAnyway");
		CPPUNIT_ASSERT_EQUAL(size_t(3), tc->NumElements());
		CPPUNIT_ASSERT_EQUAL(Int32(82), TreeValueCast<TreeInt32>(tc->Get("A"))->Value());
		CPPUNIT_ASSERT_EQUAL(Int32(-3), TreeValueCast<TreeInt32>(tc->Get("B"))->Value());
		CPPUNIT_ASSERT_EQUAL(size_t(5), TreeValueCast<TreeBinary>(tc->Get("C"))->SizeBytes());
		CPPUNIT_ASSERT_EQUAL(Int32(82), TreeValueCast<TreeInt32>(tc->Node(0)->Value())->Value());
		CPPUNIT_ASSERT_EQUAL(Int32(-3), TreeValueCast<TreeInt32>(tc->Node(1)->Value())->Value());
		CPPUNIT_ASSERT_EQUAL(size_t(5), TreeValueCast<TreeBinary>(tc->Node(2)->Value())->SizeBytes());
		CPPUNIT_ASSERT_EQUAL(size_t(2), m.RefCount());

		// removing just A should leave others intact and indices shift up one
		tc->Remove("A");
		CPPUNIT_ASSERT_EQUAL(size_t(2), tc->NumElements());
		CPPUNIT_ASSERT_EQUAL((TreeValue*)NULL, tc->Get("A"));
		CPPUNIT_ASSERT_EQUAL(Int32(-3), TreeValueCast<TreeInt32>(tc->Get("B"))->Value());
		CPPUNIT_ASSERT_EQUAL(size_t(5), TreeValueCast<TreeBinary>(tc->Get("C"))->SizeBytes());
		CPPUNIT_ASSERT_EQUAL(Int32(-3), TreeValueCast<TreeInt32>(tc->Node(0)->Value())->Value());
		CPPUNIT_ASSERT_EQUAL(size_t(5), TreeValueCast<TreeBinary>(tc->Node(1)->Value())->SizeBytes());

		// unchanged ref count to binary
		CPPUNIT_ASSERT_EQUAL(size_t(2), m.RefCount());

		// but removing binary should decrease ref count
		tc->Remove("C");
		CPPUNIT_ASSERT_EQUAL(size_t(1), tc->NumElements());
		CPPUNIT_ASSERT_EQUAL(size_t(1), m.RefCount());

		delete tc;
	}

	void testNewBlank()
	{
		TreeCompound a;
		a.Set("Who?", new TreeInt32(-29));
		CPPUNIT_ASSERT_EQUAL(size_t(1), a.NumElements());

		TreeValue* b = a.NewBlank();
		CPPUNIT_ASSERT_EQUAL(size_t(0), ((TreeCompound*)b)->NumElements());
		delete b;
	}

	void testCopyFrom()
	{
		TreeCompound a;
		a.Set("Bananas", new TreeInt32(-29));
		a.Set("Apples", new TreeInt32(-30));
		a.Set("Supernovas", new TreeInt32(30290));
		CPPUNIT_ASSERT_EQUAL(size_t(3), a.NumElements());

		TreeCompound b;
		b.Set("NotBothered", new TreeInt32(20000));

		b.CopyFrom(&a);
		
		// check total
		CPPUNIT_ASSERT_EQUAL(size_t(3), b.NumElements());

		// check retrieval
		CPPUNIT_ASSERT_EQUAL(Int32(30290), TreeValueCast<TreeInt32>( b.Get("Supernovas") )->Value() );
		CPPUNIT_ASSERT_EQUAL(Int32(-30), TreeValueCast<TreeInt32>( b.Get("Apples") )->Value() );
		CPPUNIT_ASSERT_EQUAL(Int32(-29), TreeValueCast<TreeInt32>( b.Get("Bananas") )->Value() );

		// check ordering
		CPPUNIT_ASSERT_EQUAL(Int32(-29), TreeValueCast<TreeInt32>( b.Node(0)->Value() )->Value() );
		CPPUNIT_ASSERT_EQUAL(Int32(-30), TreeValueCast<TreeInt32>( b.Node(1)->Value() )->Value() );
		CPPUNIT_ASSERT_EQUAL(Int32(30290), TreeValueCast<TreeInt32>( b.Node(2)->Value() )->Value() );
	}


	void testClear()
	{
		TreeCompound* tc = new TreeCompound;
	
		// add two integer elements
		tc->Set("A", new TreeInt32(82));
		tc->Set("B", new TreeInt32(-3));

		// and binary handler
		MemoryHandlerBasic m(5);
		tc->Set("C", new TreeBinary(&m));

		CPPUNIT_ASSERT_EQUAL(size_t(3), tc->NumElements());
		CPPUNIT_ASSERT_EQUAL(size_t(2), m.RefCount());

		tc->Clear();
		CPPUNIT_ASSERT_EQUAL(size_t(0), tc->NumElements());
		CPPUNIT_ASSERT_EQUAL(size_t(1), m.RefCount());

		delete tc;
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestTreeCompound );
