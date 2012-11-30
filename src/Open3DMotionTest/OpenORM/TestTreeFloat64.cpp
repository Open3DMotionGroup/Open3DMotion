/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Leaves/TreeFloat64.h"
#include <cppunit/extensions/HelperMacros.h>

using namespace Open3DMotion;

class TestTreeFloat64  : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestTreeFloat64 );
	CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testNewBlank );
	CPPUNIT_TEST( testAssignEquals );
	CPPUNIT_TEST( testCopyFrom );
	CPPUNIT_TEST_SUITE_END();

public:
	void testConstructor()
	{
		TreeFloat64 a;
		CPPUNIT_ASSERT_EQUAL(0.0, a.Value());

		TreeFloat64 b(2.067);
		CPPUNIT_ASSERT_EQUAL(2.067, b.Value());

		TreeFloat64 c(-29.8888);
		CPPUNIT_ASSERT_EQUAL(-29.8888, c.Value());
	}

	void testNewBlank()
	{
		TreeFloat64 a(576.76);
		TreeFloat64* ptr = a.NewBlank();
		CPPUNIT_ASSERT_EQUAL(0.0, ptr->Value());
		CPPUNIT_ASSERT_EQUAL(576.76, a.Value());
		delete ptr;
	}

	void testAssignEquals()
	{
		TreeFloat64 a(-888.888);
		TreeFloat64 b(999.999);

		CPPUNIT_ASSERT_EQUAL(999.999, b.Value());
		b = a;
		CPPUNIT_ASSERT_EQUAL(-888.888, b.Value());
		b = -9.3;
		CPPUNIT_ASSERT_EQUAL(-9.3, b.Value());
	}

	void testCopyFrom()
	{
		TreeFloat64 a(288.1);
		TreeFloat64 b(3043.0);
		CPPUNIT_ASSERT_EQUAL(3043.0, b.Value());
		b.CopyFrom(&a);
		CPPUNIT_ASSERT_EQUAL(288.1, b.Value());
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION( TestTreeFloat64 );
