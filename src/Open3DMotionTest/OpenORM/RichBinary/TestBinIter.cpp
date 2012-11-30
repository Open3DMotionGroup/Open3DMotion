/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinIter.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinMemFactoryDefault.h"

#include <cppunit/extensions/HelperMacros.h>

using namespace Open3DMotion;
using namespace std;


class TestBinIter  : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestBinIter );
	CPPUNIT_TEST( testIter1 );
	CPPUNIT_TEST( testIter2 );
	CPPUNIT_TEST( testIter3 );
	CPPUNIT_TEST( testInsufficientDimension );
	CPPUNIT_TEST( testWrongType );
	CPPUNIT_TEST_SUITE_END();

    static const UInt32 testframes = 10;
	static const Int32 testdata_32bit[testframes];
	static const UInt8 testdata_8bit[testframes];
    

public:

	void testIter1()
	{
		RichBinary bin("TestStructure");
		MakeTestData(bin);

		size_t index(0);
		BinIter1<Int32> iwrite(bin, "number", 2);
		CPPUNIT_ASSERT_EQUAL(testframes, (UInt32)iwrite.NumFrames());
		for ( ; iwrite.HasFrame(); iwrite.Next(), index++)
		{
			CPPUNIT_ASSERT_EQUAL(index, iwrite.FrameIndex());
			*iwrite.ValuePtr0() = testdata_32bit[index];
		}

		iwrite.SeekTo(1);
		CPPUNIT_ASSERT_EQUAL(testdata_32bit[1], *iwrite.ValuePtr0());

		iwrite.NextN(3);
		CPPUNIT_ASSERT_EQUAL(testdata_32bit[4], *iwrite.ValuePtr0());

		iwrite.PrevN(2);
		CPPUNIT_ASSERT_EQUAL(testdata_32bit[2], *iwrite.ValuePtr0());

		iwrite.Prev();
		CPPUNIT_ASSERT_EQUAL(testdata_32bit[1], *iwrite.ValuePtr0());

		index = 0;
		for (BinConstIter1<Int32> iread(bin, "number", 2); iread.HasFrame(); iread.Next(), index++)
		{
			CPPUNIT_ASSERT_EQUAL(testdata_32bit[index], *iread.ValuePtr0());
			CPPUNIT_ASSERT_EQUAL(index, iread.FrameIndex());
		}
	}

	void testIter2()
	{
		RichBinary bin("TestStructure");
		MakeTestData(bin);

		// write both fields
		size_t index(0);
		BinIter2<Int32, UInt8> iwrite(bin, "number", 2, "letter", 1);
		CPPUNIT_ASSERT_EQUAL(testframes, (UInt32)iwrite.NumFrames());
		for ( ; iwrite.HasFrame(); iwrite.Next(), index++)
		{
			CPPUNIT_ASSERT_EQUAL(index, iwrite.FrameIndex());
			*iwrite.ValuePtr0() = testdata_32bit[index];
			*iwrite.ValuePtr1() = testdata_8bit[index];
		}

		iwrite.SeekTo(9);
		CPPUNIT_ASSERT_EQUAL(testdata_32bit[9], *iwrite.ValuePtr0());
		CPPUNIT_ASSERT_EQUAL(testdata_8bit[9], *iwrite.ValuePtr1());

		iwrite.PrevN(4);
		CPPUNIT_ASSERT_EQUAL(testdata_32bit[5], *iwrite.ValuePtr0());
		CPPUNIT_ASSERT_EQUAL(testdata_8bit[5], *iwrite.ValuePtr1());

		iwrite.NextN(2);
		CPPUNIT_ASSERT_EQUAL(testdata_32bit[7], *iwrite.ValuePtr0());
		CPPUNIT_ASSERT_EQUAL(testdata_8bit[7], *iwrite.ValuePtr1());

		iwrite.Prev();
		CPPUNIT_ASSERT_EQUAL(testdata_32bit[6], *iwrite.ValuePtr0());
		CPPUNIT_ASSERT_EQUAL(testdata_8bit[6], *iwrite.ValuePtr1());

		// read single field to check consistent with single iterator
		index = 0;
		for (BinConstIter1<Int32> iread1(bin, "number", 2); iread1.HasFrame(); iread1.Next(), index++)
		{
			CPPUNIT_ASSERT_EQUAL(testdata_32bit[index], *iread1.ValuePtr0());
			CPPUNIT_ASSERT_EQUAL(index, iread1.FrameIndex());
		}		

		// deliberately read fields in different order to write
		index = 0;
		for (BinConstIter2<UInt8, Int32> iread2(bin, "letter", 1, "number", 2); iread2.HasFrame(); iread2.Next(), index++)
		{
			CPPUNIT_ASSERT_EQUAL(testdata_8bit[index], *iread2.ValuePtr0());
			CPPUNIT_ASSERT_EQUAL(testdata_32bit[index], *iread2.ValuePtr1());
			CPPUNIT_ASSERT_EQUAL(index, iread2.FrameIndex());
		}		
	}

	void testIter3()
	{
		RichBinary bin("TestStructure");
		MakeTestData(bin);

		// write both fields
		size_t index(0);
		BinIter3<Int32, UInt8, double> iwrite(bin, "number", 2, "letter", 1, "measurement", 1);
		CPPUNIT_ASSERT_EQUAL(testframes, (UInt32)iwrite.NumFrames());
		for ( ; iwrite.HasFrame(); iwrite.Next(), index++)
		{
			CPPUNIT_ASSERT_EQUAL(index, iwrite.FrameIndex());
			*iwrite.ValuePtr0() = testdata_32bit[index];
			*iwrite.ValuePtr1() = testdata_8bit[index];
			*iwrite.ValuePtr2() = (double)testdata_32bit[TestBinIter::testframes-1-index];
		}

		iwrite.SeekTo(9);
		CPPUNIT_ASSERT_EQUAL(testdata_32bit[9], *iwrite.ValuePtr0());
		CPPUNIT_ASSERT_EQUAL(testdata_8bit[9], *iwrite.ValuePtr1());
		CPPUNIT_ASSERT_DOUBLES_EQUAL(testdata_32bit[TestBinIter::testframes-1-9], *iwrite.ValuePtr2(), 1E-12);

		iwrite.PrevN(4);
		CPPUNIT_ASSERT_EQUAL(testdata_32bit[5], *iwrite.ValuePtr0());
		CPPUNIT_ASSERT_EQUAL(testdata_8bit[5], *iwrite.ValuePtr1());
		CPPUNIT_ASSERT_DOUBLES_EQUAL(testdata_32bit[TestBinIter::testframes-1-5], *iwrite.ValuePtr2(), 1E-12);

		iwrite.NextN(2);
		CPPUNIT_ASSERT_EQUAL(testdata_32bit[7], *iwrite.ValuePtr0());
		CPPUNIT_ASSERT_EQUAL(testdata_8bit[7], *iwrite.ValuePtr1());
		CPPUNIT_ASSERT_DOUBLES_EQUAL(testdata_32bit[TestBinIter::testframes-1-7], *iwrite.ValuePtr2(), 1E-12);

		iwrite.Prev();
		CPPUNIT_ASSERT_EQUAL(testdata_32bit[6], *iwrite.ValuePtr0());
		CPPUNIT_ASSERT_EQUAL(testdata_8bit[6], *iwrite.ValuePtr1());
		CPPUNIT_ASSERT_DOUBLES_EQUAL(testdata_32bit[TestBinIter::testframes-1-6], *iwrite.ValuePtr2(), 1E-12);

		// read single field to check consistent with single iterator
		index = 0;
		for (BinConstIter1<Int32> iread1(bin, "number", 2); iread1.HasFrame(); iread1.Next(), index++)
		{
			CPPUNIT_ASSERT_EQUAL(testdata_32bit[index], *iread1.ValuePtr0());
			CPPUNIT_ASSERT_EQUAL(index, iread1.FrameIndex());
		}		

		// deliberately read fields in different order to write
		index = 0;
		for (BinConstIter3<double, UInt8, Int32> iread2(bin, "measurement", 1, "letter", 1, "number", 2); iread2.HasFrame(); iread2.Next(), index++)
		{
			CPPUNIT_ASSERT_DOUBLES_EQUAL((double)testdata_32bit[TestBinIter::testframes-1-index], *iread2.ValuePtr0(), 1E-12);
			CPPUNIT_ASSERT_EQUAL(testdata_8bit[index], *iread2.ValuePtr1());
			CPPUNIT_ASSERT_EQUAL(testdata_32bit[index], *iread2.ValuePtr2());
			CPPUNIT_ASSERT_EQUAL(index, iread2.FrameIndex());
		}		
	}

	void testInsufficientDimension()
	{
		RichBinary bin("TestStructure");
		MakeTestData(bin);

		try
		{
			BinConstIter2<UInt8, Int32> iread2(bin, "letter", 1, "number", 5);
		}
		catch (const NoSuchFieldException& e)
		{
			CPPUNIT_ASSERT_EQUAL(string("number"), e.FieldName());
			CPPUNIT_ASSERT_EQUAL(NoSuchFieldException::wrongdimension, e.Error());
			return;
		}

		CPPUNIT_FAIL("Expected NoSuchFieldException(\"number\", wrongdimension)");
	}

	void testWrongType()
	{
		RichBinary bin("TestStructure");
		MakeTestData(bin);

		try
		{
			BinConstIter2<Int32, double> iread2(bin, "number", 1, "letter", 1);
		}
		catch (const NoSuchFieldException& e)
		{
			CPPUNIT_ASSERT_EQUAL(string("letter"), e.FieldName());
			CPPUNIT_ASSERT_EQUAL(NoSuchFieldException::wrongtype, e.Error());
			return;
		}

		CPPUNIT_FAIL("Expected NoSuchFieldException(\"letter\", wrongtype)");
	}

protected:
	void MakeTestData(RichBinary& bin)
	{
		vector<BinaryFieldSpec> layout;
		layout.push_back(BinaryFieldSpec::FromType<Int32>("number", 3L));
		layout.push_back(BinaryFieldSpec::FromType<UInt8>("letter", 1L));
		layout.push_back(BinaryFieldSpec::FromType<double>("measurement", 1L));
		BinMemFactoryDefault memfactory;
		bin.Allocate(layout, testframes, memfactory);
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION( TestBinIter );

#ifdef __GNUC__
// Need explcit implementation of these in GNU
// though constant value is given in header
const UInt32 TestBinIter::testframes;
#endif

const Int32 TestBinIter::testdata_32bit[TestBinIter::testframes] = { 9, 2, 7, 0, 3, 4, 1, 5, 8, 2 };
const UInt8 TestBinIter::testdata_8bit[TestBinIter::testframes] = { 'H', 'e', 'l', 'l', 'o', 'H', 'o', 'r', 's', 'e' };
