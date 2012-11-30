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

class TestMapOptional  : public CppUnit::TestCase
{

	// Test class with two mandatory elements (for use as optional sub-element)
	class People : public MapCompound
	{
	public:
		MapString RockStar;
		MapString Geek;

	public:
		People()
		{
			REGISTER_MEMBER(RockStar);
			REGISTER_MEMBER(Geek);
		}
	};

	// Test class with multiple optional elements
	class Vehicles : public MapCompound
	{
	public:
		MapString Bicycle;
		MapOptionalString LightAircraft;
		MapOptionalInt32 Pedestrians;
		MapOptionalFloat64 Distance;
		MapOptionalBool AllWell;
		MapOptional<People> MyPeople;

	public:
		Vehicles()
		{
			REGISTER_MEMBER(Bicycle);
			REGISTER_MEMBER(LightAircraft);
			REGISTER_MEMBER(Pedestrians);
			REGISTER_MEMBER(Distance);
			REGISTER_MEMBER(AllWell);
			REGISTER_MEMBER(MyPeople);
		}
	};

public:
	CPPUNIT_TEST_SUITE( TestMapOptional );
	CPPUNIT_TEST( testToTree );
	CPPUNIT_TEST( testFromTree );
	CPPUNIT_TEST_SUITE_END();

public:
	void testToTree()
	{
		Vehicles v;

		// create one where only the non-optional item is set - all others should not appear in tree
		v.Bicycle = "Raleigh";
		TreeCompound* t1 = (TreeCompound*)v.ToTree();
		CPPUNIT_ASSERT_EQUAL(size_t(1), t1->NumElements());
		CPPUNIT_ASSERT_EQUAL(string("Raleigh"), TreeValueCast<TreeString>( t1->Get("Bicycle") )->Value() );
		delete t1;
		t1 = NULL;

		// set some things in inner compound ref
		v.MyPeople.RockStar = "Joel";
		TreeCompound* t2 = (TreeCompound*)v.ToTree();
		CPPUNIT_ASSERT_EQUAL(size_t(2), t2->NumElements());
		CPPUNIT_ASSERT_EQUAL(string("Raleigh"),TreeValueCast<TreeString>( t2->Get("Bicycle") )->Value() );
		TreeCompound* sub_t2 = TreeValueCast<TreeCompound>( t2->Get("MyPeople") );
		CPPUNIT_ASSERT(sub_t2 != NULL);
		CPPUNIT_ASSERT_EQUAL(size_t(2), sub_t2->NumElements());
		CPPUNIT_ASSERT_EQUAL(string("Joel"), TreeValueCast<TreeString>( sub_t2->Get("RockStar") )->Value() );
		CPPUNIT_ASSERT_EQUAL(string(""), TreeValueCast<TreeString>( sub_t2->Get("Geek") )->Value() );
		delete t2;
		t2 = NULL;

		// now set the others
		v.LightAircraft = "Cessna170";
		v.Pedestrians = 27L;
		v.Distance = 29.777;
		v.AllWell = true;
		v.MyPeople.Geek = "Bob";
		TreeCompound* t3 = (TreeCompound*)v.ToTree();
		CPPUNIT_ASSERT_EQUAL(size_t(6), t3->NumElements());
		CPPUNIT_ASSERT_EQUAL(string("Raleigh"), TreeValueCast<TreeString>( t3->Get("Bicycle") )->Value() );
		CPPUNIT_ASSERT_EQUAL(string("Cessna170"), TreeValueCast<TreeString>( t3->Get("LightAircraft") )->Value() );
		CPPUNIT_ASSERT_EQUAL(Int32(27), TreeValueCast<TreeInt32>( t3->Get("Pedestrians") )->Value() );
		CPPUNIT_ASSERT_DOUBLES_EQUAL(29.777, TreeValueCast<TreeFloat64>( t3->Get("Distance") )->Value(), 1E-12 );
		CPPUNIT_ASSERT_EQUAL(true, TreeValueCast<TreeBool>( t3->Get("AllWell") )->Value() );
		
		TreeCompound* sub_t3 = TreeValueCast<TreeCompound>( t3->Get("MyPeople") );
		CPPUNIT_ASSERT(sub_t3 != NULL);
		CPPUNIT_ASSERT_EQUAL(size_t(2), sub_t3->NumElements());
		CPPUNIT_ASSERT_EQUAL(string("Joel"), TreeValueCast<TreeString>( sub_t3->Get("RockStar") )->Value() );
		CPPUNIT_ASSERT_EQUAL(string("Bob"), TreeValueCast<TreeString>( sub_t3->Get("Geek") )->Value() );

		delete t3;
		t3 = NULL;
	}

	void testFromTree()
	{
		TreeCompound* t = new TreeCompound();
		t->Set("Pedestrians", new TreeInt32(3999L));

		// first go with not much there
		Vehicles v;
		
		// use this to ensure we use const version of Value()
		const Vehicles& cv = v;

		v.FromTree(t);
		CPPUNIT_ASSERT_EQUAL(string(""), cv.Bicycle.Value());
		CPPUNIT_ASSERT( !cv.LightAircraft.IsSet() );
		CPPUNIT_ASSERT(  cv.Pedestrians.IsSet() );
		CPPUNIT_ASSERT( !cv.Distance.IsSet() );
		CPPUNIT_ASSERT( !cv.AllWell.IsSet() );
		CPPUNIT_ASSERT( !cv.MyPeople.IsSet() );
		CPPUNIT_ASSERT_EQUAL(string(""), cv.LightAircraft.Value());
		CPPUNIT_ASSERT_EQUAL(Int32(3999), cv.Pedestrians.Value());
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cv.Distance.Value(), 1E-12);
		CPPUNIT_ASSERT_EQUAL(false, cv.AllWell.Value());
		CPPUNIT_ASSERT_EQUAL(string(""), cv.MyPeople.RockStar.Value());
		CPPUNIT_ASSERT_EQUAL(string(""), cv.MyPeople.Geek.Value());

		// set additional sub-item so sub-tree should appear set
		TreeCompound* sub_t = new TreeCompound();
		t->Set("MyPeople", sub_t);
		sub_t->Set("Geek", new TreeString("Sheila"));
		v.FromTree(t);
		CPPUNIT_ASSERT_EQUAL(string(""), cv.Bicycle.Value());
		CPPUNIT_ASSERT( !cv.LightAircraft.IsSet() );
		CPPUNIT_ASSERT(  cv.Pedestrians.IsSet() );
		CPPUNIT_ASSERT( !cv.Distance.IsSet() );
		CPPUNIT_ASSERT( !cv.AllWell.IsSet() );
		CPPUNIT_ASSERT(  cv.MyPeople.IsSet() );
		CPPUNIT_ASSERT_EQUAL(string(""), cv.LightAircraft.Value());
		CPPUNIT_ASSERT_EQUAL(Int32(3999), cv.Pedestrians.Value());
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cv.Distance.Value(), 1E-12);
		CPPUNIT_ASSERT_EQUAL(false, cv.AllWell.Value());
		CPPUNIT_ASSERT_EQUAL(string(""), cv.MyPeople.RockStar.Value());
		CPPUNIT_ASSERT_EQUAL(string("Sheila"), cv.MyPeople.Geek.Value());

		// set all items
		t->Set("Bicycle", new TreeString("Raleigh"));
		t->Set("LightAircraft", new TreeString("747"));
		t->Set("Distance", new TreeFloat64(-32.999));
		t->Set("AllWell", new TreeBool(true));
		sub_t->Set("RockStar", new TreeString("Bob"));
		v.FromTree(t);
		CPPUNIT_ASSERT_EQUAL(string("Raleigh"), cv.Bicycle.Value());
		CPPUNIT_ASSERT(  cv.LightAircraft.IsSet() );
		CPPUNIT_ASSERT(  cv.Pedestrians.IsSet() );
		CPPUNIT_ASSERT(  cv.Distance.IsSet() );
		CPPUNIT_ASSERT(  cv.AllWell.IsSet() );
		CPPUNIT_ASSERT(  cv.MyPeople.IsSet() );
		CPPUNIT_ASSERT_EQUAL(string("747"), cv.LightAircraft.Value());
		CPPUNIT_ASSERT_EQUAL(Int32(3999), cv.Pedestrians.Value());
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-32.999, cv.Distance.Value(), 1E-12);
		CPPUNIT_ASSERT_EQUAL(true, cv.AllWell.Value());
		CPPUNIT_ASSERT_EQUAL(string("Bob"), cv.MyPeople.RockStar.Value());
		CPPUNIT_ASSERT_EQUAL(string("Sheila"), cv.MyPeople.Geek.Value());

		delete t;
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION( TestMapOptional );
