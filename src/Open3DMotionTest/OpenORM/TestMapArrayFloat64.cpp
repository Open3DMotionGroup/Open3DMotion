/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Mappings/MapArrayFloat64.h"
#include <cppunit/extensions/HelperMacros.h>

using namespace Open3DMotion;
using namespace std;

// some useful test data
namespace TestPoints
{
	const int N = 10;
	const double resolution = 1E-8;
	const double data[N] =
	{
		939761.64633320,
		488999.52036317,
		966398.13015860,
		852646.96890825,
		-613738.93069702,
		551205.20764220,
		138616.43691909,
		-64355.36975981,
		237527.88119615,
		-166741.89884243
	};
};

class TestMapArrayFloat64  : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestMapArrayFloat64 );
	CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testAddClear );
	CPPUNIT_TEST( testToTree );
	CPPUNIT_TEST( testFromTree );
	CPPUNIT_TEST( testFromTree_SingleItem );
	CPPUNIT_TEST_SUITE_END();

public:
	void testConstructor()
	{
		MapArrayFloat64* m = new MapArrayFloat64("MaybeMyElementName");
		CPPUNIT_ASSERT_EQUAL(string("MaybeMyElementName"), m->ElementName());
		delete m;
	}

	void testAddClear()
	{
		// make an array with 3 elements
		MapArrayFloat64 a("Yes");
		a.Add(27890.33);
		CPPUNIT_ASSERT_EQUAL(size_t(1), a.NumElements());
		a.Add(123.56);
		CPPUNIT_ASSERT_EQUAL(size_t(2), a.NumElements());
		a.Add(-689.111);
		CPPUNIT_ASSERT_EQUAL(size_t(3), a.NumElements());

		// retrieval block
		{
			double& a0 = a[0];
			double& a1 = a[1];
			double& a2 = a[2];
			CPPUNIT_ASSERT_DOUBLES_EQUAL(27890.33, a0, 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(123.56, a1, 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-689.111, a2, 1E-12);
		}

		// block with const retrieval to test const functions
		{
			const double& ca0 = a[0];
			const double& ca1 = a[1];
			const double& ca2 = a[2];
			CPPUNIT_ASSERT_DOUBLES_EQUAL(27890.33, ca0, 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(123.56, ca1, 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-689.111, ca2, 1E-12);
		}

		// now clear
		a.Clear();
		CPPUNIT_ASSERT_EQUAL(size_t(0), a.NumElements());
	}

	void testToTree()
	{
		// build test data
		int i = 0;
		MapArrayFloat64 a("SomeName");
		for (i = 0; i < TestPoints::N; i++)
		{
			a.Add(TestPoints::data[i]);
		}

		// verify tree
		TreeList* t = TreeValueCast<TreeList>(a.ToTree());
		CPPUNIT_ASSERT(t != NULL);
		CPPUNIT_ASSERT_EQUAL(string("SomeName"), t->ElementName());
		CPPUNIT_ASSERT_EQUAL(size_t(TestPoints::N), t->NumElements());
		for (i = 0; i < TestPoints::N; i++)
		{
			const TreeFloat64* v = TreeValueCast<TreeFloat64>( t->ElementArray()[i] );
			CPPUNIT_ASSERT(v != NULL);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(TestPoints::data[i], v->Value(), TestPoints::resolution);
		}
	}

	void testFromTree()
	{
		// build test data
		int i = 0;
		TreeList t("Grrr!!");
		for (i = 0; i < TestPoints::N; i++)
		{
			t.ElementArray().push_back(new TreeFloat64(TestPoints::data[i]));
		}

		// conversion to array of different element name should just clear
		{
			MapArrayFloat64 a("Hmph");
			a.Add(3.14);
			a.FromTree(&t);
			CPPUNIT_ASSERT_EQUAL(string("Hmph"), a.ElementName());
			CPPUNIT_ASSERT_EQUAL(size_t(0), a.NumElements());
		}

		// but when element names match should work ok
		{
			MapArrayFloat64 b("Grrr!!");
			b.Add(3.14);
			b.Add(2.59);
			b.FromTree(&t);
			CPPUNIT_ASSERT_EQUAL(string("Grrr!!"), b.ElementName());
			CPPUNIT_ASSERT_EQUAL(size_t(10), b.NumElements());
			for (i = 0; i < TestPoints::N; i++)
			{
				CPPUNIT_ASSERT_DOUBLES_EQUAL(TestPoints::data[i], b[i], TestPoints::resolution);
			}
		}
	}

	void testFromTree_SingleItem()
	{
		TreeCompound t;
		TreeCompound* sub_t = new TreeCompound;
		t.Set("SomeNumber", new TreeFloat64(-253.5));

		MapArrayFloat64 result("SomeNumber");
		result.FromTree(&t);

		CPPUNIT_ASSERT_EQUAL(size_t(1), result.NumElements());
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-253.5, result[0], 1E-12);
	}



};

CPPUNIT_TEST_SUITE_REGISTRATION( TestMapArrayFloat64 );
