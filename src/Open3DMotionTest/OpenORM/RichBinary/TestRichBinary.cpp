/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinMemFactoryDefault.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/RichBinary.h"
#include <cppunit/extensions/HelperMacros.h>

using namespace Open3DMotion;
using namespace std;

// An invented new type of 11 bytes to test extension of field type strings
typedef char WobblyNumber[11];

// Test code to extend available field types to use this invented new type
namespace Open3DMotion
{
  template<> const char* BinaryFieldTypeString<WobblyNumber>() { return "WobblyNumber"; }
}

class TestRichBinary  : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestRichBinary );
	CPPUNIT_TEST( testBinaryFieldSpec );
	CPPUNIT_TEST( testBinaryStructure );
	CPPUNIT_TEST( testDeepCopy );
	CPPUNIT_TEST_SUITE_END();

public:

	void testBinaryFieldSpec()
	{
		// Blank constructor
		BinaryFieldSpec s0;		

		// Checl that blank still populates tree
		TreeCompound* t0 = TreeValueCast<TreeCompound>( s0.ToTree() );
		CPPUNIT_ASSERT(t0 != NULL);
		CPPUNIT_ASSERT(t0->Get("Name") != NULL);
		CPPUNIT_ASSERT(t0->Get("Type") != NULL);
		CPPUNIT_ASSERT(t0->Get("Bytes") != NULL);
		CPPUNIT_ASSERT(t0->Get("Dimension") != NULL);

		// Check that parameter constructor fills all members
		BinaryFieldSpec s1(BinaryFieldSpec::FromType<Int32>( "Bob", 7L ));
		CPPUNIT_ASSERT_EQUAL(string("Bob"), s1.Name.Value());
		CPPUNIT_ASSERT_EQUAL(string("long"), s1.Type.Value());
		CPPUNIT_ASSERT_EQUAL(Int32(28), s1.Bytes.Value());
		CPPUNIT_ASSERT_EQUAL(Int32(7), s1.Dimension.Value());

		// Check round-trip to tree from constructed item
		TreeValue* t1 = s1.ToTree();
		BinaryFieldSpec s2;
		s2.FromTree(t1);
		CPPUNIT_ASSERT_EQUAL(string("Bob"), s2.Name.Value());
		CPPUNIT_ASSERT_EQUAL(string("long"), s2.Type.Value());
		CPPUNIT_ASSERT_EQUAL(Int32(28), s2.Bytes.Value());
		CPPUNIT_ASSERT_EQUAL(Int32(7), s2.Dimension.Value());

		// Check copy constructor
		BinaryFieldSpec s3(s1);
		CPPUNIT_ASSERT_EQUAL(string("Bob"), s3.Name.Value());
		CPPUNIT_ASSERT_EQUAL(string("long"), s3.Type.Value());
		CPPUNIT_ASSERT_EQUAL(Int32(28), s3.Bytes.Value());
		CPPUNIT_ASSERT_EQUAL(Int32(7), s3.Dimension.Value());

		// Check round-trip to tree from copied item
		TreeValue* t2 = s3.ToTree();
		BinaryFieldSpec s4;
		s4.FromTree(t2);
		CPPUNIT_ASSERT_EQUAL(string("Bob"), s4.Name.Value());
		CPPUNIT_ASSERT_EQUAL(string("long"), s4.Type.Value());
		CPPUNIT_ASSERT_EQUAL(Int32(28), s4.Bytes.Value());
		CPPUNIT_ASSERT_EQUAL(Int32(7), s4.Dimension.Value());
	}

	void testBinaryStructure()
	{
		BinaryStructure a;
		CPPUNIT_ASSERT_EQUAL(size_t(0), a.GetLayout().NumElements());
		CPPUNIT_ASSERT_EQUAL(size_t(0), a.TotalBytes());

		vector<BinaryFieldSpec> layout;
		layout.push_back(BinaryFieldSpec::FromType<float>("Position", 3L));
		layout.push_back(BinaryFieldSpec::FromType<UInt8>("Flag", 1L));
		layout.push_back(BinaryFieldSpec::FromType<WobblyNumber>("CrazyItem", 5L));

		BinaryStructure b;
		b.SetLayout(layout);
		CPPUNIT_ASSERT_EQUAL(size_t(3), b.GetLayout().NumElements());
		CPPUNIT_ASSERT_EQUAL(size_t(68), b.TotalBytes());

		TreeValue* t = b.ToTree();
		BinaryStructure c;
		c.FromTree(t);

		CPPUNIT_ASSERT_EQUAL(size_t(3), c.GetLayout().NumElements());
		CPPUNIT_ASSERT_EQUAL(size_t(68), c.TotalBytes());
		CPPUNIT_ASSERT_EQUAL(string("Position"), c.GetLayout()[0].Name.Value());
		CPPUNIT_ASSERT_EQUAL(string("float"), c.GetLayout()[0].Type.Value());
		CPPUNIT_ASSERT_EQUAL(Int32(12), c.GetLayout()[0].Bytes.Value());
		CPPUNIT_ASSERT_EQUAL(Int32(3), c.GetLayout()[0].Dimension.Value());
		CPPUNIT_ASSERT_EQUAL(string("Flag"), c.GetLayout()[1].Name.Value());
		CPPUNIT_ASSERT_EQUAL(string("byte"), c.GetLayout()[1].Type.Value());
		CPPUNIT_ASSERT_EQUAL(Int32(1), c.GetLayout()[1].Bytes.Value());
		CPPUNIT_ASSERT_EQUAL(Int32(1), c.GetLayout()[1].Dimension.Value());
		CPPUNIT_ASSERT_EQUAL(string("CrazyItem"), c.GetLayout()[2].Name.Value());
		CPPUNIT_ASSERT_EQUAL(string("WobblyNumber"), c.GetLayout()[2].Type.Value());
		CPPUNIT_ASSERT_EQUAL(Int32(55), c.GetLayout()[2].Bytes.Value());
		CPPUNIT_ASSERT_EQUAL(Int32(5), c.GetLayout()[2].Dimension.Value());

		const BinaryFieldSpec* spec(NULL);
		size_t offset(0);
		c.GetFieldOffset(spec, offset, "Position");
		CPPUNIT_ASSERT_EQUAL(string("Position"), spec->Name.Value());
		CPPUNIT_ASSERT_EQUAL(size_t(0), offset);
		
		c.GetFieldOffset(spec, offset, "Flag");
		CPPUNIT_ASSERT_EQUAL(string("Flag"), spec->Name.Value());
		CPPUNIT_ASSERT_EQUAL(size_t(12), offset);

		c.GetFieldOffset(spec, offset, "CrazyItem");
		CPPUNIT_ASSERT_EQUAL(string("CrazyItem"), spec->Name.Value());
		CPPUNIT_ASSERT_EQUAL(size_t(13), offset);

		try
		{
			size_t badoffset(0);
			c.GetFieldOffset(spec, badoffset, "WhoAreYou?");
		}
		catch (const NoSuchFieldException& e)
		{
			CPPUNIT_ASSERT_EQUAL(string("WhoAreYou?"), e.FieldName());
			CPPUNIT_ASSERT_EQUAL(NoSuchFieldException::missing, e.Error());
			return;
		}

		CPPUNIT_FAIL("Expected NoSuchFieldException(\"WhoAreYou?\", missing)");
	}

	class DerivedRichBinary : public RichBinary
	{
	public:
		DerivedRichBinary() :
				RichBinary("MyStructure")
		{
			REGISTER_MEMBER(MyInfo);
		}

	public:
		MapString MyInfo;
	};

	void testDeepCopy()
	{
		BinMemFactoryDefault memfactory;
		std::vector<BinaryFieldSpec> layout;
		layout.push_back(BinaryFieldSpec::FromType<UInt8>("Flags", 3L));
		
		DerivedRichBinary drb;
		drb.Allocate(layout, 23, memfactory);
		drb.MyInfo = "Hello";
		memset(drb.Data(), 0, 23*3);
		drb.Data()[7] = 121;

		DerivedRichBinary copy_shallow;
		std::auto_ptr<TreeValue> tree_copy(drb.ToTree());
		copy_shallow.FromTree(tree_copy.get());

		DerivedRichBinary copy_deep;
		copy_deep.DeepCopyFrom(drb, memfactory);

		// check that deep, shallow, and initial input all match
		CPPUNIT_ASSERT_EQUAL(size_t(23*3), drb.DataSizeBytes());
		CPPUNIT_ASSERT_EQUAL(UInt8(0), drb.Data()[0]);
		CPPUNIT_ASSERT_EQUAL(UInt8(121), drb.Data()[7]);
		CPPUNIT_ASSERT_EQUAL(std::string("Hello"), drb.MyInfo.Value());
		CPPUNIT_ASSERT_EQUAL(size_t(23*3), copy_shallow.DataSizeBytes());
		CPPUNIT_ASSERT_EQUAL(UInt8(0), copy_shallow.Data()[0]);
		CPPUNIT_ASSERT_EQUAL(UInt8(121), copy_shallow.Data()[7]);
		CPPUNIT_ASSERT_EQUAL(std::string("Hello"), copy_shallow.MyInfo.Value());
		CPPUNIT_ASSERT_EQUAL(size_t(23*3), copy_deep.DataSizeBytes());
		CPPUNIT_ASSERT_EQUAL(UInt8(0), copy_deep.Data()[0]);
		CPPUNIT_ASSERT_EQUAL(UInt8(121), copy_deep.Data()[7]);
		CPPUNIT_ASSERT_EQUAL(std::string("Hello"), copy_deep.MyInfo.Value());

		// modify the base data
		drb.Data()[0] = 2;
		drb.Data()[7] = 13;
		drb.MyInfo = "What?";

		// shallow should have changed data but not meta-data
		CPPUNIT_ASSERT_EQUAL(size_t(23*3), copy_shallow.DataSizeBytes());
		CPPUNIT_ASSERT_EQUAL(UInt8(2), copy_shallow.Data()[0]);
		CPPUNIT_ASSERT_EQUAL(UInt8(13), copy_shallow.Data()[7]);
		CPPUNIT_ASSERT_EQUAL(std::string("Hello"), copy_shallow.MyInfo.Value());

		// deep should not have changed anything
		CPPUNIT_ASSERT_EQUAL(size_t(23*3), copy_deep.DataSizeBytes());
		CPPUNIT_ASSERT_EQUAL(UInt8(0), copy_deep.Data()[0]);
		CPPUNIT_ASSERT_EQUAL(UInt8(121), copy_deep.Data()[7]);
		CPPUNIT_ASSERT_EQUAL(std::string("Hello"), copy_deep.MyInfo.Value());
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION( TestRichBinary );
