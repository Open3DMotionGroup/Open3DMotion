/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Leaves/TreeBool.h"
#include <cppunit/extensions/HelperMacros.h>

using namespace Open3DMotion;

class TestTreeBool  : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestTreeBool );
	CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testNewBlank );
	CPPUNIT_TEST( testAssignEquals );
	CPPUNIT_TEST( testCopyFrom );
	CPPUNIT_TEST_SUITE_END();

public:
	void testConstructor()
	{
		TreeBool a;
		CPPUNIT_ASSERT_EQUAL(false, a.Value());

		TreeBool b(false);
		CPPUNIT_ASSERT_EQUAL(false, b.Value());

		TreeBool c(true);
		CPPUNIT_ASSERT_EQUAL(true, c.Value());
	}

	void testNewBlank()
	{
		TreeBool a(true);
		TreeBool* ptr = a.NewBlank();
		CPPUNIT_ASSERT_EQUAL(false, ptr->Value());
		CPPUNIT_ASSERT_EQUAL(true, a.Value());
		delete ptr;
	}

	void testAssignEquals()
	{
		TreeBool a(true);
		TreeBool b(false);

		CPPUNIT_ASSERT_EQUAL(false, b.Value());
		b = a;
		CPPUNIT_ASSERT_EQUAL(true, b.Value());
		b = false;
		CPPUNIT_ASSERT_EQUAL(false, b.Value());
	}

	void testCopyFrom()
	{
		TreeBool a(true);
		TreeBool b(false);
		CPPUNIT_ASSERT_EQUAL(false, b.Value());
		b.CopyFrom(&a);
		CPPUNIT_ASSERT_EQUAL(true, b.Value());
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestTreeBool );
