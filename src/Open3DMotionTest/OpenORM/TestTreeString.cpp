/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Leaves/TreeString.h"
#include <cppunit/extensions/HelperMacros.h>

using namespace Open3DMotion;
using namespace std;

class TestTreeString  : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestTreeString );
	CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testSet );
	CPPUNIT_TEST( testNewBlank );
	CPPUNIT_TEST( testAssignEquals );
	CPPUNIT_TEST( testCopyFrom );
	CPPUNIT_TEST_SUITE_END();

public:
	void testConstructor()
	{
		TreeString a;
		CPPUNIT_ASSERT_EQUAL(string(""), a.Value());

		TreeString b("Bob");
		CPPUNIT_ASSERT_EQUAL(string("Bob"), b.Value());

		TreeString c("Bobson");
		CPPUNIT_ASSERT_EQUAL(string("Bobson"), c.Value());
	}

	void testSet()
	{
		TreeString a;
		CPPUNIT_ASSERT_EQUAL(string(""), a.Value());
		a.Value() = "Hello";
		CPPUNIT_ASSERT_EQUAL(string("Hello"), a.Value());
		a.Value() = "Bye";
		CPPUNIT_ASSERT_EQUAL(string("Bye"), a.Value());
	}

	void testNewBlank()
	{
		TreeString a("WhatDoYouMean?");
		TreeString* ptr = a.NewBlank();
		CPPUNIT_ASSERT_EQUAL(string(""), ptr->Value());
		CPPUNIT_ASSERT_EQUAL(string("WhatDoYouMean?"), a.Value());
		delete ptr;
	}

	void testAssignEquals()
	{
		TreeString a("OneString");
		TreeString b("LongAsPieceOfString");

		CPPUNIT_ASSERT_EQUAL(string("LongAsPieceOfString"), b.Value());
		b = a;
		CPPUNIT_ASSERT_EQUAL(string("OneString"), b.Value());
		b = "NothingToSeeHereMoveAlongNowPlease";
		CPPUNIT_ASSERT_EQUAL(string("NothingToSeeHereMoveAlongNowPlease"), b.Value());
	}

	void testCopyFrom()
	{
		TreeString a("Obviously");
		TreeString b("NotMe");
		CPPUNIT_ASSERT_EQUAL(string("NotMe"), b.Value());
		b.CopyFrom(&a);
		CPPUNIT_ASSERT_EQUAL(string("Obviously"), b.Value());
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestTreeString );
