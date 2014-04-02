/*--
 Open3DMotion
 Copyright (c) 2004-2014.
 All rights reserved.
 See LICENSE.txt for more information.
 --*/

#include "Open3DMotion/OpenORM/Leaves/TreeFloat64.h"
#include "Open3DMotion/OpenORM/Leaves/TreeInt32.h"
#include "Open3DMotion/OpenORM/Leaves/TreeString.h"
#include "Open3DMotion/OpenORM/Leaves/TreeBinary.h"
#include "Open3DMotion/OpenORM/Leaves/TreeBool.h"
#include "Open3DMotion/OpenORM/Branches/TreeList.h"
#include "Open3DMotion/OpenORM/Mappings/MapInt32.h"
#include "Open3DMotion/OpenORM/Mappings/MapFloat64.h"
#include "Open3DMotion/OpenORM/Mappings/MapBool.h"
#include "Open3DMotion/OpenORM/Mappings/MapArrayCompound.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinMemFactoryDefault.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONTimestampHolder.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONObjectIdHolder.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONInputStreamSTL.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONOutputStreamSTL.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONReader.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONWriter.h"

#include <cppunit/extensions/HelperMacros.h>
#include <istream>

using namespace Open3DMotion;

/* Summary
 Integration test using BSONWriter with readback from BSONReader.
 */
class TestBSONWriteRead : public CppUnit::TestFixture
{
public:
	// Test object
	static class ObjectA : public MapCompound
	{
	public:
		ObjectA()
		{
			REGISTER_MEMBER(MyNumber);
			REGISTER_MEMBER(MyString);
			REGISTER_MEMBER(MyScalar);
		}

	public:
		MapInt32 MyNumber;
		MapString MyString;
		MapFloat64 MyScalar;
	};

	// Another test object with compound array of ObjectA
	static class ObjectB : public MapCompound
	{
	public:
		ObjectB() :
			MyList("Something")
		{
			REGISTER_MEMBER(IsItTrue);
			REGISTER_MEMBER(MyList);
		}

	public:
		MapBool IsItTrue;
		MapArrayCompound<ObjectA> MyList;
	};

public:
	CPPUNIT_TEST_SUITE( TestBSONWriteRead );
	CPPUNIT_TEST( testCompound );
	CPPUNIT_TEST( testCompoundWithList );
	CPPUNIT_TEST_SUITE_END();
  
public:
  
	TestBSONWriteRead()
	{
	}
  
public:
  
	void testCompound()
	{
    std::stringstream output(std::ios::binary | std::ios::out | std::ios::in);

    {
			ObjectA a;
			a.MyNumber = 582;
			a.MyString = "WhatIsThis??";
			a.MyScalar = -82.2;
			std::auto_ptr<TreeValue> tree(a.ToTree());
      BSONOutputStreamSTL stream_output(output);
      BSONWriter writer(stream_output);
			writer.WriteDocument(*static_cast<TreeCompound*>(tree.get()));
    }
    
		output.seekp(0, std::ios::beg);
		BSONInputStreamSTL stream_input(output);
		BinMemFactoryDefault memfactory;
		BSONReader reader(stream_input, memfactory);
		TreeCompound tree_result;
		reader.ReadDocument(tree_result);
		ObjectA a_result;
		a_result.FromTree(&tree_result);
		CPPUNIT_ASSERT_EQUAL(Int32(582), a_result.MyNumber.Value());
		CPPUNIT_ASSERT_EQUAL(std::string("WhatIsThis??"), a_result.MyString.Value());
		CPPUNIT_ASSERT_EQUAL(-82.2, a_result.MyScalar.Value());
	}

	void testCompoundWithList()
	{
    std::stringstream output(std::ios::binary | std::ios::out | std::ios::in);

    {
			ObjectA a0;
			a0.MyNumber = 582;
			a0.MyString = "WhatIsThis??";
			a0.MyScalar = -82.2;
			
			ObjectA a1;
			a1.MyNumber = -2999;
			a1.MyString = "NothingHere";
			a1.MyScalar = 122.222;

			ObjectB b;
			b.IsItTrue = true;
			b.MyList.Add(a0);
			b.MyList.Add(a1);
			
			std::auto_ptr<TreeValue> tree(b.ToTree());
      BSONOutputStreamSTL stream_output(output);
      BSONWriter writer(stream_output);
			writer.WriteDocument(*static_cast<TreeCompound*>(tree.get()));
    }
    
		output.seekp(0, std::ios::beg);
		BSONInputStreamSTL stream_input(output);
		BinMemFactoryDefault memfactory;
		BSONReader reader(stream_input, memfactory);
		TreeCompound tree_result;
		reader.ReadDocument(tree_result);
		ObjectB b_result;
		b_result.FromTree(&tree_result);
		CPPUNIT_ASSERT_EQUAL(true, b_result.IsItTrue.Value());
		CPPUNIT_ASSERT_EQUAL(size_t(2), b_result.MyList.NumElements());
		CPPUNIT_ASSERT_EQUAL(Int32(582), b_result.MyList[0].MyNumber.Value());
		CPPUNIT_ASSERT_EQUAL(std::string("WhatIsThis??"), b_result.MyList[0].MyString.Value());
		CPPUNIT_ASSERT_EQUAL(-82.2, b_result.MyList[0].MyScalar.Value());
		CPPUNIT_ASSERT_EQUAL(Int32(-2999), b_result.MyList[1].MyNumber.Value());
		CPPUNIT_ASSERT_EQUAL(std::string("NothingHere"), b_result.MyList[1].MyString.Value());
		CPPUNIT_ASSERT_EQUAL(122.222, b_result.MyList[1].MyScalar.Value());
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestBSONWriteRead );
