/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Mappings/MapCompound.h"
#include "Open3DMotion/OpenORM/Mappings/MapBool.h"
#include "Open3DMotion/OpenORM/Mappings/MapInt32.h"
#include "Open3DMotion/OpenORM/Mappings/MapFloat64.h"
#include "Open3DMotion/OpenORM/Mappings/MapString.h"
#include <cppunit/extensions/HelperMacros.h>

using namespace Open3DMotion;
using namespace std;

class TestMapCompound  : public CppUnit::TestCase
{

	class OneLayer : public MapCompound
	{
	public:
		MapInt32 AnApple;
		MapFloat64 ABanana;
		MapString SomeNonsense;
		MapBool YesOrNo;

	public:
		OneLayer()
		{
			REGISTER_MEMBER(AnApple);
			REGISTER_MEMBER(ABanana);
			REGISTER_MEMBER(SomeNonsense);
			REGISTER_MEMBER(YesOrNo);
		}
	};

	class TwoLayers : public MapCompound
	{
	public:
		MapString SomeBasicInfo;
		OneLayer MyLayer;

	public:
		TwoLayers()
		{
			REGISTER_MEMBER(SomeBasicInfo);
			REGISTER_MEMBER(MyLayer);
		}
	};


public:
	CPPUNIT_TEST_SUITE( TestMapCompound );
	CPPUNIT_TEST( testToTreeOneLayer );
	CPPUNIT_TEST( testToTreeTwoLayers );
	CPPUNIT_TEST( testFromTreeEmpty );
	CPPUNIT_TEST( testFromTreeTwoLayers );
	CPPUNIT_TEST_SUITE_END();

public:

	void testToTreeOneLayer()
	{
		OneLayer m;
		TreeCompound* t1 = (TreeCompound*)m.ToTree();
		CPPUNIT_ASSERT_EQUAL(size_t(4), t1->NumElements());
		CPPUNIT_ASSERT(t1->Get("AnApple") != NULL); 
		CPPUNIT_ASSERT(t1->Get("ABanana") != NULL); 
		CPPUNIT_ASSERT(t1->Get("SomeNonsense") != NULL); 
		CPPUNIT_ASSERT(t1->Get("YesOrNo") != NULL); 
		CPPUNIT_ASSERT(t1->Get("ShouldntBeHere") == NULL); 

		m.AnApple = 321L;
		m.ABanana = -3.42;
		m.SomeNonsense = "GrannySmith";
		m.YesOrNo = true;
		TreeCompound* t2 = (TreeCompound*)m.ToTree();
		CPPUNIT_ASSERT_EQUAL(Int32(321), TreeValueCast<TreeInt32>( t2->Get("AnApple") )->Value() );
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.42, TreeValueCast<TreeFloat64>( t2->Get("ABanana") )->Value(), 1E-12 );
		CPPUNIT_ASSERT_EQUAL(string("GrannySmith"), TreeValueCast<TreeString>( t2->Get("SomeNonsense") )->Value() );
		CPPUNIT_ASSERT_EQUAL(true, TreeValueCast<TreeBool>( t2->Get("YesOrNo") )->Value() );

		delete t1;
		delete t2;
	}

	void testToTreeTwoLayers()
	{
		TwoLayers m;
		m.SomeBasicInfo = "Bob";
		m.MyLayer.ABanana = 789.123;
		m.MyLayer.SomeNonsense = "Haaaa!!?";

		TreeCompound* t = (TreeCompound*)m.ToTree();
		CPPUNIT_ASSERT_EQUAL(size_t(2), t->NumElements());
		CPPUNIT_ASSERT_EQUAL(string("Bob"), TreeValueCast<TreeString>( t->Get("SomeBasicInfo") )->Value() );
		TreeCompound* sub_t = TreeValueCast<TreeCompound>( t->Get("MyLayer") );
		CPPUNIT_ASSERT(sub_t != NULL);
		CPPUNIT_ASSERT_EQUAL(size_t(4), sub_t->NumElements());
		CPPUNIT_ASSERT_EQUAL(Int32(0), TreeValueCast<TreeInt32>( sub_t->Get("AnApple") )->Value() );	// left at default zero
		CPPUNIT_ASSERT_DOUBLES_EQUAL(789.123, TreeValueCast<TreeFloat64>( sub_t->Get("ABanana") )->Value(), 1E-12 );
		CPPUNIT_ASSERT_EQUAL(string("Haaaa!!?"), TreeValueCast<TreeString>( sub_t->Get("SomeNonsense") )->Value() );
		CPPUNIT_ASSERT_EQUAL(false, TreeValueCast<TreeBool>( sub_t->Get("YesOrNo") )->Value() );
		delete t;
	}

	void testFromTreeEmpty()
	{
		TreeCompound t;
		TwoLayers m;
		m.FromTree(&t);
		CPPUNIT_ASSERT_EQUAL(string(""), m.SomeBasicInfo.Value());
		CPPUNIT_ASSERT_EQUAL(Int32(0), m.MyLayer.AnApple.Value());
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, m.MyLayer.ABanana, 1E-12);
		CPPUNIT_ASSERT_EQUAL(string(""), m.MyLayer.SomeNonsense.Value());
	}

	void testFromTreeTwoLayers()
	{
		TreeCompound t;
		TreeCompound* sub_t = new TreeCompound;
		t.Set("SomeBasicInfo", new TreeString("Welcome"));
		t.Set("MyLayer", sub_t);
		sub_t->Set("SomeNonsense", new TreeString("TotalRubbish"));

		TwoLayers m;
		m.FromTree(&t);
		CPPUNIT_ASSERT_EQUAL(string("Welcome"), m.SomeBasicInfo.Value());
		CPPUNIT_ASSERT_EQUAL(Int32(0), m.MyLayer.AnApple.Value());
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, m.MyLayer.ABanana, 1E-12);
		CPPUNIT_ASSERT_EQUAL(string("TotalRubbish"), m.MyLayer.SomeNonsense.Value());
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestMapCompound );
