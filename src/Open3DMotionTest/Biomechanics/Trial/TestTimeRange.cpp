/*--
Open3DMotion
Copyright (c) 2004-2018.
All rights reserved.
See LICENSE.txt for more information.
--*/

#include "Open3DMotion/Biomechanics/Trial/TimeRange.h"
#include <cppunit/extensions/HelperMacros.h>

using namespace Open3DMotion;
using namespace std;


class TestTimeRange : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE(TestTimeRange);
	CPPUNIT_TEST(testConstructor);
	CPPUNIT_TEST(testIsSameAs);
	CPPUNIT_TEST_SUITE_END();

public:

	void testConstructor()
	{
		TimeRange t;
		CPPUNIT_ASSERT_EQUAL(Int32(0), t.Frames.Value());
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, t.Start.Value(), DBL_MIN);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, t.Rate.Value(), DBL_MIN);
	}

	void testIsSameAs()
	{
		TimeRange a, b, c, d, e;
		
		a.Frames = 1001;
		a.Rate = 200.0;
		a.Start = 7.7;

		b.Frames = 1001;
		b.Rate = 200.0;
		b.Start = 7.7;

		c.Frames = 1001;
		c.Rate = 200.001;
		c.Start = 7.7;

		d.Frames = 1001;
		d.Rate = 200.0;
		d.Start = 7.701;

		e.Frames = 1002;
		e.Rate = 200.0;
		e.Start = 7.7;

		CPPUNIT_ASSERT_EQUAL(true, a.IsSameAs(b));
		CPPUNIT_ASSERT_EQUAL(true, b.IsSameAs(a));
		CPPUNIT_ASSERT_EQUAL(false, a.IsSameAs(c));
		CPPUNIT_ASSERT_EQUAL(false, c.IsSameAs(a));
		CPPUNIT_ASSERT_EQUAL(false, a.IsSameAs(d));
		CPPUNIT_ASSERT_EQUAL(false, d.IsSameAs(a));
		CPPUNIT_ASSERT_EQUAL(false, a.IsSameAs(e));
		CPPUNIT_ASSERT_EQUAL(false, e.IsSameAs(a));
		CPPUNIT_ASSERT_EQUAL(true, a.IsSameAs(c, 1E-9, 1E-2));
		CPPUNIT_ASSERT_EQUAL(true, c.IsSameAs(a, 1E-9, 1E-2));
		CPPUNIT_ASSERT_EQUAL(true, a.IsSameAs(d, 1E-2, 1E-9));
		CPPUNIT_ASSERT_EQUAL(true, d.IsSameAs(a, 1E-2, 1E-9));
		CPPUNIT_ASSERT_EQUAL(false, a.IsSameAs(e, 1E-2, 1E-2));
		CPPUNIT_ASSERT_EQUAL(false, e.IsSameAs(a, 1E-2, 1E-2));
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestTimeRange);
