/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Leaves/TreeInt32.h"
#include <cppunit/extensions/HelperMacros.h>

using namespace Open3DMotion;

class TestTreeInt32  : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestTreeInt32 );
	CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testNewBlank );
	CPPUNIT_TEST( testAssignEquals );
	CPPUNIT_TEST( testCopyFrom );
	CPPUNIT_TEST_SUITE_END();

public:
	void testConstructor()
	{
		TreeInt32 a;
		CPPUNIT_ASSERT_EQUAL(Int32(0L), a.Value());

		TreeInt32 b(2);
		CPPUNIT_ASSERT_EQUAL(Int32(2L), b.Value());

		TreeInt32 c(-29);
		CPPUNIT_ASSERT_EQUAL(Int32(-29L), c.Value());
	}

	void testNewBlank()
	{
		TreeInt32 a(576);
		TreeInt32* ptr = a.NewBlank();
		CPPUNIT_ASSERT_EQUAL(Int32(0L), ptr->Value());
		CPPUNIT_ASSERT_EQUAL(Int32(576L), a.Value());
		delete ptr;
	}

	void testAssignEquals()
	{
		TreeInt32 a(-888);
		TreeInt32 b(999);

		CPPUNIT_ASSERT_EQUAL(Int32(999L), b.Value());
		b = a;
		CPPUNIT_ASSERT_EQUAL(Int32(-888L), b.Value());
		b = -9;
		CPPUNIT_ASSERT_EQUAL(Int32(-9L), b.Value());
	}

	void testCopyFrom()
	{
		TreeInt32 a(288);
		TreeInt32 b(3043);
		CPPUNIT_ASSERT_EQUAL(Int32(3043L), b.Value());
		b.CopyFrom(&a);
		CPPUNIT_ASSERT_EQUAL(Int32(288L), b.Value());
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION( TestTreeInt32 );
