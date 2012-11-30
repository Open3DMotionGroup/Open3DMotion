/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/Biomechanics/Trial/EventArray.h"
#include <cppunit/extensions/HelperMacros.h>

using namespace Open3DMotion;

class TestEventArray : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestEventArray );
	CPPUNIT_TEST( testConstructAddClear );
	CPPUNIT_TEST( testManualAdd );
	CPPUNIT_TEST_SUITE_END();

public:
	void testConstructAddClear()
	{
		EventArray a;
		CPPUNIT_ASSERT_EQUAL(size_t(0), a.NumEvents());

		a.AddEvent(-27.8, "Unga");
		CPPUNIT_ASSERT_EQUAL(size_t(1), a.NumEvents());

		a.AddEvent(9.0, "Bunga");
		CPPUNIT_ASSERT_EQUAL(size_t(2), a.NumEvents());

		a.AddEvent(2.1, "Bongo");
		CPPUNIT_ASSERT_EQUAL(size_t(3), a.NumEvents());

		CPPUNIT_ASSERT_DOUBLES_EQUAL(-27.8, a.EventTime(0), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(  2.1, a.EventTime(1), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(  9.0, a.EventTime(2), 1E-12);

		CPPUNIT_ASSERT_EQUAL(std::string("Unga"), a.EventName(0));
		CPPUNIT_ASSERT_EQUAL(std::string("Bongo"), a.EventName(1));
		CPPUNIT_ASSERT_EQUAL(std::string("Bunga"), a.EventName(2));

		CPPUNIT_ASSERT_EQUAL(Int32(1), a.EventID(0));
		CPPUNIT_ASSERT_EQUAL(Int32(3), a.EventID(1));
		CPPUNIT_ASSERT_EQUAL(Int32(2), a.EventID(2));

		a.Clear();
		CPPUNIT_ASSERT_EQUAL(size_t(0), a.NumEvents());
		CPPUNIT_ASSERT_EQUAL(size_t(0), a.NameMap().size());
	}

	void testManualAdd()
	{
		EventArray a;

		// manually set 3 names
		a.SetIDName(17L, "Unga");
		a.SetIDName(31L, "Bunga");
		a.SetIDName(23L, "Bongo");

		CPPUNIT_ASSERT_EQUAL(true, a.AddEvent(-27.8, 17L));
		CPPUNIT_ASSERT_EQUAL(size_t(1), a.NumEvents());

		CPPUNIT_ASSERT_EQUAL(true, a.AddEvent(9.0, 31L));
		CPPUNIT_ASSERT_EQUAL(size_t(2), a.NumEvents());

		CPPUNIT_ASSERT_EQUAL(true, a.AddEvent(2.1, 23L));
		CPPUNIT_ASSERT_EQUAL(size_t(3), a.NumEvents());

		// adding an event for non-existent ID should do nothing and return false
		CPPUNIT_ASSERT_EQUAL(false, a.AddEvent(10.0, 1L));
		CPPUNIT_ASSERT_EQUAL(size_t(3), a.NumEvents());

		CPPUNIT_ASSERT_DOUBLES_EQUAL(-27.8, a.EventTime(0), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(  2.1, a.EventTime(1), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(  9.0, a.EventTime(2), 1E-12);

		CPPUNIT_ASSERT_EQUAL(std::string("Unga"), a.EventName(0));
		CPPUNIT_ASSERT_EQUAL(std::string("Bongo"), a.EventName(1));
		CPPUNIT_ASSERT_EQUAL(std::string("Bunga"), a.EventName(2));

		CPPUNIT_ASSERT_EQUAL(Int32(17), a.EventID(0));
		CPPUNIT_ASSERT_EQUAL(Int32(23), a.EventID(1));
		CPPUNIT_ASSERT_EQUAL(Int32(31), a.EventID(2));

		a.Clear();
		CPPUNIT_ASSERT_EQUAL(size_t(0), a.NumEvents());
		CPPUNIT_ASSERT_EQUAL(size_t(0), a.NameMap().size());
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestEventArray );
