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
#include "Open3DMotion/OpenORM/Mappings/MapBinary.h"
#include "Open3DMotion/OpenORM/Mappings/MapArrayCompound.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinMemFactoryDefault.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONTimestampHolder.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONObjectIdHolder.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONInputStreamSTL.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONOutputStreamSTL.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONReader.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONWriter.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONReaderMOBL.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONWriterMOBL.h"
#include "Open3DMotion/OpenORM/Leaves/MemoryHandlerBasic.h"

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
	class ObjectA : public MapCompound
	{
	public:
		ObjectA()
		{
			REGISTER_MEMBER(MyNumber);
			REGISTER_MEMBER(MyString);
			REGISTER_MEMBER(MyScalar);
			REGISTER_MEMBER(MyQuestion);
			REGISTER_MEMBER(MyData);
		}

	public:
		MapInt32 MyNumber;
		MapString MyString;
		MapFloat64 MyScalar;
		MapBool MyQuestion;
		MapBinary MyData;
	};

	// Another test object with compound array of ObjectA
	class ObjectB : public MapCompound
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
	CPPUNIT_TEST( testCompoundWithListMOBL );
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
			a.MyQuestion = true;
			MemoryHandlerBasic mem(3);
			mem.Data()[0] = 20;
			mem.Data()[1] = 30;
			mem.Data()[2] = 44;
			a.MyData.SetData(mem.Clone());
			std::unique_ptr<TreeValue> tree(a.ToTree());
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
		CPPUNIT_ASSERT_EQUAL(true, a_result.MyQuestion.Value());
		CPPUNIT_ASSERT_EQUAL(size_t(3), a_result.MyData.SizeBytes());
		CPPUNIT_ASSERT_EQUAL(20, (int)a_result.MyData.Data()[0]);
		CPPUNIT_ASSERT_EQUAL(30, (int)a_result.MyData.Data()[1]);
		CPPUNIT_ASSERT_EQUAL(44, (int)a_result.MyData.Data()[2]);
	}

	void testCompoundWithList()
	{
    std::stringstream output(std::ios::binary | std::ios::out | std::ios::in);

    {
			MemoryHandlerBasic mem1(3);
			mem1.Data()[0] = 55;
			mem1.Data()[1] = 254;
			mem1.Data()[2] = 32;

			ObjectA a0;
			a0.MyNumber = 582;
			a0.MyString = "WhatIsThis??";
			a0.MyScalar = -82.2;
			a0.MyQuestion = false;
			
			ObjectA a1;
			a1.MyNumber = -2999;
			a1.MyString = "NothingHere";
			a1.MyScalar = 122.222;
			a1.MyQuestion = true;
			a1.MyData.SetData(mem1.Clone());

			ObjectB b;
			b.IsItTrue = true;
			b.MyList.Add(a0);
			b.MyList.Add(a1);
			
			std::unique_ptr<TreeValue> tree(b.ToTree());
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
		CPPUNIT_ASSERT_EQUAL(false, b_result.MyList[0].MyQuestion.Value());
		CPPUNIT_ASSERT_EQUAL(size_t(0), b_result.MyList[0].MyData.SizeBytes());
		CPPUNIT_ASSERT_EQUAL(Int32(-2999), b_result.MyList[1].MyNumber.Value());
		CPPUNIT_ASSERT_EQUAL(std::string("NothingHere"), b_result.MyList[1].MyString.Value());
		CPPUNIT_ASSERT_EQUAL(122.222, b_result.MyList[1].MyScalar.Value());
		CPPUNIT_ASSERT_EQUAL(true, b_result.MyList[1].MyQuestion.Value());
		CPPUNIT_ASSERT_EQUAL(size_t(3), b_result.MyList[1].MyData.SizeBytes());
		CPPUNIT_ASSERT_EQUAL(55, (int)b_result.MyList[1].MyData.Data()[0]);
		CPPUNIT_ASSERT_EQUAL(254, (int)b_result.MyList[1].MyData.Data()[1]);
		CPPUNIT_ASSERT_EQUAL(32, (int)b_result.MyList[1].MyData.Data()[2]);
	}

	void testCompoundWithListMOBL()
	{
    std::stringstream output(std::ios::binary | std::ios::out | std::ios::in);

    {
			MemoryHandlerBasic mem1(3);
			mem1.Data()[0] = 55;
			mem1.Data()[1] = 254;
			mem1.Data()[2] = 32;

			ObjectA a0;
			a0.MyNumber = 582;
			a0.MyString = "WhatIsThis??";
			a0.MyScalar = -82.2;
			a0.MyQuestion = false;
			
			ObjectA a1;
			a1.MyNumber = -2999;
			a1.MyString = "NothingHere";
			a1.MyScalar = 122.222;
			a1.MyQuestion = true;
			a1.MyData.SetData(mem1.Clone());

			ObjectB b;
			b.IsItTrue = true;
			b.MyList.Add(a0);
			b.MyList.Add(a1);
			
			std::unique_ptr<TreeValue> tree(b.ToTree());
      BSONOutputStreamSTL stream_output(output);
      BSONWriterMOBL writer(stream_output);
			writer.WriteDocument(*static_cast<TreeCompound*>(tree.get()));
    }
    
		output.seekp(0, std::ios::beg);
		BSONInputStreamSTL stream_input(output);
		BinMemFactoryDefault memfactory;
		BSONReaderMOBL reader(stream_input, memfactory);
		TreeCompound tree_result;
		reader.ReadDocument(tree_result);
		ObjectB b_result;
		b_result.FromTree(&tree_result);
		CPPUNIT_ASSERT_EQUAL(true, b_result.IsItTrue.Value());
		CPPUNIT_ASSERT_EQUAL(size_t(2), b_result.MyList.NumElements());
		CPPUNIT_ASSERT_EQUAL(Int32(582), b_result.MyList[0].MyNumber.Value());
		CPPUNIT_ASSERT_EQUAL(std::string("WhatIsThis??"), b_result.MyList[0].MyString.Value());
		CPPUNIT_ASSERT_EQUAL(-82.2, b_result.MyList[0].MyScalar.Value());
		CPPUNIT_ASSERT_EQUAL(false, b_result.MyList[0].MyQuestion.Value());
		CPPUNIT_ASSERT_EQUAL(size_t(0), b_result.MyList[0].MyData.SizeBytes());
		CPPUNIT_ASSERT_EQUAL(Int32(-2999), b_result.MyList[1].MyNumber.Value());
		CPPUNIT_ASSERT_EQUAL(std::string("NothingHere"), b_result.MyList[1].MyString.Value());
		CPPUNIT_ASSERT_EQUAL(122.222, b_result.MyList[1].MyScalar.Value());
		CPPUNIT_ASSERT_EQUAL(true, b_result.MyList[1].MyQuestion.Value());
		CPPUNIT_ASSERT_EQUAL(size_t(3), b_result.MyList[1].MyData.SizeBytes());
		CPPUNIT_ASSERT_EQUAL(55, (int)b_result.MyList[1].MyData.Data()[0]);
		CPPUNIT_ASSERT_EQUAL(254, (int)b_result.MyList[1].MyData.Data()[1]);
		CPPUNIT_ASSERT_EQUAL(32, (int)b_result.MyList[1].MyData.Data()[2]);
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION( TestBSONWriteRead );
