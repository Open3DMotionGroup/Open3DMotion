/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinIter.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinMemFactoryDefault.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/RichBinaryConvertFloat.h"
#include "Open3DMotion/OpenORM/Leaves/MemoryHandlerBasic.h"

#include <cppunit/extensions/HelperMacros.h>

using namespace Open3DMotion;
using namespace std;


class TestRichBinaryConvertFloat  : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestRichBinaryConvertFloat );
	CPPUNIT_TEST( test32To64NotRichBinary );
	CPPUNIT_TEST( test64To32NotRichBinary );
	CPPUNIT_TEST( test32To64 );
	CPPUNIT_TEST( test64To32 );
	CPPUNIT_TEST_SUITE_END();

public:

	void test32To64NotRichBinary()
	{
		std::auto_ptr<TreeCompound> tree ( new TreeCompound );
		MemoryHandlerBasic junk(2003);
		tree->Set(MEMBER_NAME(RichBinary::Data),  new TreeBinary(&junk) );
		BinMemFactoryDefault memfactory;
		TreeValue* result = RichBinaryConvertFloat32To64(tree.get(), "MyStructure", memfactory);
		CPPUNIT_ASSERT(result == NULL);
	}

	void test64To32NotRichBinary()
	{
		std::auto_ptr<TreeCompound> tree ( new TreeCompound );
		MemoryHandlerBasic junk(2003);
		tree->Set(MEMBER_NAME(RichBinary::Data),  new TreeBinary(&junk) );
		BinMemFactoryDefault memfactory;
		TreeValue* result = RichBinaryConvertFloat32To64(tree.get(), "MyStructure", memfactory);
		CPPUNIT_ASSERT(result == NULL);
	}

	void test32To64()
	{
		try
		{
			auto_ptr<TreeValue> result_tree;
			{
				BinMemFactoryDefault memfactory;

				vector<BinaryFieldSpec> layout;
				layout.push_back(BinaryFieldSpec::FromType<UInt8>("Flag", 1L));
				layout.push_back(BinaryFieldSpec::FromType<float>("Position", 3L));
				layout.push_back(BinaryFieldSpec::FromType<Int32>("Number", 2L));
		
				RichBinary rb("MyStructure");
				rb.Allocate(layout, 5, memfactory);

				// invented data
				BinIter3<UInt8, float, Int32> iter_input(rb, "Flag", 1, "Position", 3, "Number", 2);
				memset(rb.Data(), 0, rb.DataSizeBytes());
				iter_input.ValuePtr0()[0] = 2;
				iter_input.ValuePtr1()[0] = -22.9f;
				iter_input.ValuePtr1()[1] =  33.2f;
				iter_input.ValuePtr1()[2] = 189.9f;
				iter_input.ValuePtr2()[0] = 9999;
				iter_input.ValuePtr2()[1] = 2222;
				iter_input.Next();
				iter_input.Next();
				iter_input.Next();
				iter_input.Next();
				iter_input.ValuePtr0()[0] = 3;
				iter_input.ValuePtr1()[0] = -23.8f;
				iter_input.ValuePtr1()[1] =  17.1f;
				iter_input.ValuePtr1()[2] = 200.1f;
				iter_input.ValuePtr2()[0] = 12;
				iter_input.ValuePtr2()[1] = 9;

				// convert to tree
				auto_ptr<TreeValue> rb_tree( rb.ToTree() );
		
				// invented meta-data on rich binary object
				TreeValueCast<TreeCompound>( rb_tree.get() )->Set("MyInfo", new TreeString("Some Text"));

				// convert
				result_tree = auto_ptr<TreeValue>( RichBinaryConvertFloat32To64(rb_tree.get(), "MyStructure", memfactory) );
			}

			CPPUNIT_ASSERT(result_tree.get() != NULL);

			RichBinary result("MyStructure");
			result.FromTree(result_tree.get());

			// check size of result
			CPPUNIT_ASSERT_EQUAL(size_t(33), result.DataStructure().TotalBytes());
			CPPUNIT_ASSERT_EQUAL(size_t(5*33), result.DataSizeBytes());

			// check result content
			BinConstIter3<UInt8, double, Int32> iter_result(result, "Flag", 1, "Position", 3, "Number", 2);
			CPPUNIT_ASSERT_EQUAL(int(2), int( iter_result.ValuePtr0()[0]));
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-22.9, iter_result.ValuePtr1()[0], 0.0001);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(33.2, iter_result.ValuePtr1()[1], 0.0001);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(189.9, iter_result.ValuePtr1()[2], 0.0001);
			CPPUNIT_ASSERT_EQUAL(Int32(9999), iter_result.ValuePtr2()[0]);
			CPPUNIT_ASSERT_EQUAL(Int32(2222), iter_result.ValuePtr2()[1]);
			iter_result.Next();
			iter_result.Next();
			iter_result.Next();
			iter_result.Next();
			CPPUNIT_ASSERT_EQUAL(int(3), int(iter_result.ValuePtr0()[0]));
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-23.8, iter_result.ValuePtr1()[0], 0.0001);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(17.1, iter_result.ValuePtr1()[1], 0.0001);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(200.1, iter_result.ValuePtr1()[2], 0.0001);
			CPPUNIT_ASSERT_EQUAL(Int32(12), iter_result.ValuePtr2()[0]);
			CPPUNIT_ASSERT_EQUAL(Int32(9), iter_result.ValuePtr2()[1]);

			// check info still attached
			const TreeValue* result_info = TreeValueCast<const TreeCompound>( result_tree.get() )->Get("MyInfo");
			CPPUNIT_ASSERT(result_info != NULL);
			CPPUNIT_ASSERT(result_info->ClassNameMatches(TreeString::classname));
			CPPUNIT_ASSERT_EQUAL(std::string("Some Text"), TreeValueCast<const TreeString>( result_info )->Value());
		}
		catch (const NoSuchFieldException& e)
		{
			CPPUNIT_FAIL(e.FieldName());
		}
	}

	void test64To32()
	{
		try
		{
			auto_ptr<TreeValue> result_tree;
			{
				BinMemFactoryDefault memfactory;

				vector<BinaryFieldSpec> layout;
				layout.push_back(BinaryFieldSpec::FromType<UInt8>("Flag", 1L));
				layout.push_back(BinaryFieldSpec::FromType<double>("Position", 3L));
				layout.push_back(BinaryFieldSpec::FromType<Int32>("Number", 2L));
		
				RichBinary rb("MyStructure");
				rb.Allocate(layout, 5, memfactory);

				// invented data
				BinIter3<UInt8, double, Int32> iter_input(rb, "Flag", 1, "Position", 3, "Number", 2);
				memset(rb.Data(), 0, rb.DataSizeBytes());
				iter_input.ValuePtr0()[0] = 2;
				iter_input.ValuePtr1()[0] = -22.9;
				iter_input.ValuePtr1()[1] =  33.2;
				iter_input.ValuePtr1()[2] = 189.9;
				iter_input.ValuePtr2()[0] = 9999;
				iter_input.ValuePtr2()[1] = 2222;
				iter_input.Next();
				iter_input.Next();
				iter_input.Next();
				iter_input.Next();
				iter_input.ValuePtr0()[0] = 3;
				iter_input.ValuePtr1()[0] = -23.8;
				iter_input.ValuePtr1()[1] =  17.1;
				iter_input.ValuePtr1()[2] = 200.1;
				iter_input.ValuePtr2()[0] = 12;
				iter_input.ValuePtr2()[1] = 9;

				// convert to tree
				auto_ptr<TreeValue> rb_tree = auto_ptr<TreeValue> ( rb.ToTree() );
		
				// invented meta-data on rich binary object
				TreeValueCast<TreeCompound>( rb_tree.get() )->Set("MyInfo", new TreeString("Some Text"));

				// convert
				result_tree  = auto_ptr<TreeValue>( RichBinaryConvertFloat64To32(rb_tree.get(), "MyStructure", memfactory) );
			}

		
			CPPUNIT_ASSERT(result_tree.get() != NULL);

			RichBinary result("MyStructure");
			result.FromTree(result_tree.get());

			// check size of result
			CPPUNIT_ASSERT_EQUAL(size_t(21), result.DataStructure().TotalBytes());
			CPPUNIT_ASSERT_EQUAL(size_t(5*21), result.DataSizeBytes());

			// check result content
			BinConstIter3<UInt8, float, Int32> iter_result(result, "Flag", 1, "Position", 3, "Number", 2);
			CPPUNIT_ASSERT_EQUAL(int(2), int(iter_result.ValuePtr0()[0]));
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-22.9, iter_result.ValuePtr1()[0], 0.0001);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(33.2, iter_result.ValuePtr1()[1], 0.0001);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(189.9, iter_result.ValuePtr1()[2], 0.0001);
			CPPUNIT_ASSERT_EQUAL(Int32(9999), iter_result.ValuePtr2()[0]);
			CPPUNIT_ASSERT_EQUAL(Int32(2222), iter_result.ValuePtr2()[1]);
			iter_result.Next();
			iter_result.Next();
			iter_result.Next();
			iter_result.Next();
			CPPUNIT_ASSERT_EQUAL(int(3), int(iter_result.ValuePtr0()[0]));
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-23.8, iter_result.ValuePtr1()[0], 0.0001);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(17.1, iter_result.ValuePtr1()[1], 0.0001);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(200.1, iter_result.ValuePtr1()[2], 0.0001);
			CPPUNIT_ASSERT_EQUAL(Int32(12), iter_result.ValuePtr2()[0]);
			CPPUNIT_ASSERT_EQUAL(Int32(9), iter_result.ValuePtr2()[1]);

			// check info still attached
			const TreeValue* result_info = TreeValueCast<const TreeCompound>( result_tree.get() )->Get("MyInfo");
			CPPUNIT_ASSERT(result_info != NULL);
			CPPUNIT_ASSERT(result_info->ClassNameMatches(TreeString::classname));
			CPPUNIT_ASSERT_EQUAL(std::string("Some Text"), TreeValueCast<const TreeString>( result_info )->Value());
		}
		catch (const NoSuchFieldException& e)
		{
			CPPUNIT_FAIL(e.FieldName());
		}
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION( TestRichBinaryConvertFloat );
