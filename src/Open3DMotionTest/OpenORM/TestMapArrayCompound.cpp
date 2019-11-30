/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Mappings/MapArrayCompound.h"
#include "Open3DMotion/OpenORM/Mappings/MapInt32.h"
#include "Open3DMotion/OpenORM/Mappings/MapFloat64.h"
#include "Open3DMotion/OpenORM/Mappings/MapString.h"
#include "Open3DMotion/OpenORM/Mappings/MapBinary.h"
#include "Open3DMotion/OpenORM/Leaves/MemoryHandlerBasic.h"
#include <cppunit/extensions/HelperMacros.h>

using namespace Open3DMotion;
using namespace std;

class TestMapArrayCompound  : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestMapArrayCompound );
	CPPUNIT_TEST( testEmptyConstructor );
	CPPUNIT_TEST( testAllocatingConstructor );
	CPPUNIT_TEST( testAddClear );
	CPPUNIT_TEST( testToTree );
	CPPUNIT_TEST( testFromTree );
	CPPUNIT_TEST_SUITE_END();

public:

	// test class with 2 compulsory and 1 optional elements
	class Wood : public MapCompound
	{
	public:
		MapInt32 Trees;
		MapOptionalFloat64 Acres;
		MapString Notes;
		MapOptional<MapBinary> RingData;

	public:
		Wood()
		{
			RegisterMembers();
		}


		Wood(Int32 trees, const char* notes)
		{
			RegisterMembers();
			Trees = trees;
			Notes = notes;
		}

		Wood(Int32 trees, double acres, const char* notes)
		{
			RegisterMembers();
			Trees = trees;
			Notes = notes;
			this->Acres = acres;
		}

		Wood(Int32 trees, double acres, const char* notes, size_t ringdatasize)
		{
			RegisterMembers();
			Trees = trees;
			Notes = notes;
			this->Acres = acres;
			this->RingData.SetData(new MemoryHandlerBasic(ringdatasize));
		}

	private:
		void RegisterMembers()
		{
			REGISTER_MEMBER(Trees);
			REGISTER_MEMBER(Acres);
			REGISTER_MEMBER(Notes);
			REGISTER_MEMBER(RingData);
		}
	};

public:
	void testEmptyConstructor()
	{
		MapArrayCompound<Wood>* m = new MapArrayCompound<Wood>("Arbre");
		CPPUNIT_ASSERT_EQUAL(string("Arbre"), m->ElementName());
		CPPUNIT_ASSERT_EQUAL(size_t(0), m->NumElements());
		delete m;
	}

	void testAllocatingConstructor()
	{
		MapArrayCompound<Wood>* m = new MapArrayCompound<Wood>("Arbre", 10);
		CPPUNIT_ASSERT_EQUAL(string("Arbre"), m->ElementName());
		CPPUNIT_ASSERT_EQUAL(size_t(10), m->NumElements());
		CPPUNIT_ASSERT_EQUAL(Int32(0), (*m)[7].Trees.Value());
		CPPUNIT_ASSERT( !(*m)[7].Acres.IsSet() );
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, (*m)[7].Acres.Value(), 1E-12);
		CPPUNIT_ASSERT_EQUAL(string(""), (*m)[7].Notes.Value());
		delete m;
	}

	void testAddClear()
	{
		// test item
		MapArrayCompound<Wood> m("Arbre");

		// Some dummy binary blobs useful for checking reference counts
		MemoryHandlerBasic m0(234);
		MemoryHandlerBasic m2(701);

		{
			// copy this onto array
			Wood w0;
			w0.Trees = 1002;
			w0.Acres = 100.0;
			w0.Notes = "One Hundred Acre Wood";
			w0.RingData.SetData(m0.Clone());
			m.Add(w0);

			// copy this onto array
			Wood w1;
			w1.Trees = -1;
			w1.Notes = "An Imaginary Forest";
			m.Add(w1);

			// use the pointer directly on array
			Wood* w2 = new Wood;
			w2->Trees = 3;
			w2->Acres = 0.0001;
			w2->Notes = "Pointless";
			w2->RingData.SetData(m2.Clone());
			m.Add(w2);
		}

		// memory should be referenced twice at this time
		CPPUNIT_ASSERT_EQUAL(size_t(2), m0.RefCount());
		CPPUNIT_ASSERT_EQUAL(size_t(2), m2.RefCount());

		// retrieval block
		{
			Wood& t0 = m[0];
			Wood& t1 = m[1];
			Wood& t2 = m[2];

			CPPUNIT_ASSERT_EQUAL(Int32(1002), t0.Trees.Value());
			CPPUNIT_ASSERT(t0.Acres.IsSet());
			CPPUNIT_ASSERT_DOUBLES_EQUAL(100.0, t0.Acres.Value(), 1E-12);
			CPPUNIT_ASSERT_EQUAL(string("One Hundred Acre Wood"), t0.Notes.Value());
			CPPUNIT_ASSERT(t0.RingData.IsSet());
			CPPUNIT_ASSERT_EQUAL(size_t(234), t0.RingData.SizeBytes());
			
			CPPUNIT_ASSERT_EQUAL(Int32(-1), t1.Trees.Value());
			CPPUNIT_ASSERT(!t1.Acres.IsSet());
			CPPUNIT_ASSERT_EQUAL(string("An Imaginary Forest"), t1.Notes.Value());
			CPPUNIT_ASSERT(!t1.RingData.IsSet());

			CPPUNIT_ASSERT_EQUAL(Int32(3), t2.Trees.Value());
			CPPUNIT_ASSERT(t2.Acres.IsSet());
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0001, t2.Acres.Value(), 1E-12);
			CPPUNIT_ASSERT_EQUAL(string("Pointless"), t2.Notes.Value());
			CPPUNIT_ASSERT(t2.RingData.IsSet());
			CPPUNIT_ASSERT_EQUAL(size_t(701), t2.RingData.SizeBytes());

		}

		// block with const retrieval to test const functions
		{
			const Wood& ct0 = m[0];
			const Wood& ct1 = m[1];
			const Wood& ct2 = m[2];

			CPPUNIT_ASSERT_EQUAL(Int32(1002), ct0.Trees.Value());
			CPPUNIT_ASSERT(ct0.Acres.IsSet());
			CPPUNIT_ASSERT_DOUBLES_EQUAL(100.0, ct0.Acres.Value(), 1E-12);
			CPPUNIT_ASSERT_EQUAL(string("One Hundred Acre Wood"), ct0.Notes.Value());
			CPPUNIT_ASSERT(ct0.RingData.IsSet());
			CPPUNIT_ASSERT_EQUAL(size_t(234), ct0.RingData.SizeBytes());
			
			CPPUNIT_ASSERT_EQUAL(Int32(-1), ct1.Trees.Value());
			CPPUNIT_ASSERT(!ct1.Acres.IsSet());
			CPPUNIT_ASSERT_EQUAL(string("An Imaginary Forest"), ct1.Notes.Value());
			CPPUNIT_ASSERT(!ct1.RingData.IsSet());

			CPPUNIT_ASSERT_EQUAL(Int32(3), ct2.Trees.Value());
			CPPUNIT_ASSERT(ct2.Acres.IsSet());
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0001, ct2.Acres.Value(), 1E-12);
			CPPUNIT_ASSERT_EQUAL(string("Pointless"), ct2.Notes.Value());
			CPPUNIT_ASSERT(ct2.RingData.IsSet());
			CPPUNIT_ASSERT_EQUAL(size_t(701), ct2.RingData.SizeBytes());		
		}

		// memory should *still* be referenced twice at this time
		CPPUNIT_ASSERT_EQUAL(size_t(2), m0.RefCount());
		CPPUNIT_ASSERT_EQUAL(size_t(2), m2.RefCount());

		// now clear
		m.Clear();

		// should have zero elements
		CPPUNIT_ASSERT_EQUAL(size_t(0), m.NumElements());

		// memory should have just the one local reference remaining
		CPPUNIT_ASSERT_EQUAL(size_t(1), m0.RefCount());
		CPPUNIT_ASSERT_EQUAL(size_t(1), m2.RefCount());
	}

	void testToTree()
	{
		MapArrayCompound<Wood> m("Arbre");
		m.Add(Wood(32, 2.33, "Bob", 4));
		m.Add(Wood(-23, "Nothing"));
		m.Add(Wood(987, 2999.9, "What?", 976));
		TreeList* t = TreeValueCast<TreeList> (m.ToTree());
		CPPUNIT_ASSERT(t != NULL);
		CPPUNIT_ASSERT_EQUAL(size_t(3), t->NumElements());
		CPPUNIT_ASSERT_EQUAL(string("Arbre"), t->ElementName());

		{
			TreeCompound* t0 = TreeValueCast<TreeCompound>(t->ElementArray()[0]);
			CPPUNIT_ASSERT_EQUAL(Int32(32), TreeValueCast<TreeInt32>( t0->Get("Trees") )->Value() );
			CPPUNIT_ASSERT_DOUBLES_EQUAL(2.33, TreeValueCast<TreeFloat64>( t0->Get("Acres") )->Value(), 1E-12 );
			CPPUNIT_ASSERT_EQUAL(string("Bob"), TreeValueCast<TreeString>( t0->Get("Notes") )->Value() );
			CPPUNIT_ASSERT_EQUAL(size_t(4), TreeValueCast<TreeBinary>( t0->Get("RingData") )->SizeBytes() );
		}

		{
			TreeCompound* t1 = TreeValueCast<TreeCompound>(t->ElementArray()[1]);
			CPPUNIT_ASSERT_EQUAL(Int32(-23), TreeValueCast<TreeInt32>( t1->Get("Trees") )->Value() );
			CPPUNIT_ASSERT_EQUAL(string("Nothing"), TreeValueCast<TreeString>( t1->Get("Notes") )->Value() );
			CPPUNIT_ASSERT(t1->Get("Acres") == NULL);
			CPPUNIT_ASSERT(t1->Get("RingData") == NULL);
		}

		{
			TreeCompound* t2 = TreeValueCast<TreeCompound>(t->ElementArray()[2]);
			CPPUNIT_ASSERT_EQUAL(Int32(987), TreeValueCast<TreeInt32>( t2->Get("Trees") )->Value() );
			CPPUNIT_ASSERT_DOUBLES_EQUAL(2999.9, TreeValueCast<TreeFloat64>( t2->Get("Acres") )->Value(), 1E-12 );
			CPPUNIT_ASSERT_EQUAL(string("What?"), TreeValueCast<TreeString>( t2->Get("Notes") )->Value() );
			CPPUNIT_ASSERT_EQUAL(size_t(976), TreeValueCast<TreeBinary>( t2->Get("RingData") )->SizeBytes() );
		}
	}

	void testFromTree()
	{
		TreeList t("MyElement");

		{
			TreeCompound* t0 = new TreeCompound();
			t0->Set("Trees", new TreeInt32(33));
			t.Add(t0);
		}

		{
			TreeCompound* t1 = new TreeCompound();
			t1->Set("Trees", new TreeInt32(3333));
			t1->Set("Acres", new TreeFloat64(1000.11));
			t1->Set("Notes", new TreeString("Some notes"));
			std::unique_ptr<MemoryHandlerBasic> m(new MemoryHandlerBasic(888));
			t1->Set("RingData", new TreeBinary(m.release()));
			t.Add(t1);
		}

		// reading into an array with different element name should just clear it and leave name unchanged
		MapArrayCompound<Wood> wrongname("WrongName", 20);
		CPPUNIT_ASSERT_EQUAL(size_t(20), wrongname.NumElements());
		wrongname.FromTree(&t);
		CPPUNIT_ASSERT_EQUAL(size_t(0), wrongname.NumElements());
		CPPUNIT_ASSERT_EQUAL(string("WrongName"), wrongname.ElementName());

		MapArrayCompound<Wood> m("MyElement");
		m.FromTree(&t);
		CPPUNIT_ASSERT_EQUAL(size_t(2), m.NumElements());
		CPPUNIT_ASSERT_EQUAL(string("MyElement"), m.ElementName());

		{
			const Wood& ct0 = m[0];
			CPPUNIT_ASSERT_EQUAL(Int32(33), ct0.Trees.Value());
			CPPUNIT_ASSERT(!ct0.Acres.IsSet());
			CPPUNIT_ASSERT_EQUAL(string(""), ct0.Notes.Value());
			CPPUNIT_ASSERT(!ct0.RingData.IsSet());
		}

		{
			const Wood& ct1 = m[1];
			CPPUNIT_ASSERT_EQUAL(Int32(3333), ct1.Trees.Value());
			CPPUNIT_ASSERT(ct1.Acres.IsSet());
			CPPUNIT_ASSERT_DOUBLES_EQUAL(1000.11, ct1.Acres.Value(), 1E-12);
			CPPUNIT_ASSERT_EQUAL(string("Some notes"), ct1.Notes.Value());
			CPPUNIT_ASSERT(ct1.RingData.IsSet());
			CPPUNIT_ASSERT_EQUAL(size_t(888), ct1.RingData.SizeBytes());		
		}
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestMapArrayCompound );
