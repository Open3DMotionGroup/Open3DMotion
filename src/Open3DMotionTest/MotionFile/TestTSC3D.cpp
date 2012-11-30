/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/MotionFile/Formats/C3D/C3DTSFactory.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinMemFactoryDefault.h"
#include <cppunit/extensions/HelperMacros.h>

using namespace Open3DMotion;
using namespace std;

class TestTSC3D  : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestTSC3D );
	CPPUNIT_TEST( testBuild );
	CPPUNIT_TEST( testIterSetGet );
	CPPUNIT_TEST( testIterDefaults );
	CPPUNIT_TEST_SUITE_END();


public:

	void testBuild()
	{
		TSFactoryC3D factory;
		BinMemFactoryDefault memfactory;

		TimeRange tr;
		tr.Frames = 10;
		tr.Rate = 1.0;
		TimeSequence* ts = factory.New(tr, memfactory);
		CPPUNIT_ASSERT_EQUAL(size_t(4), ts->DataStructure().GetLayout().NumElements());
		
		const BinaryFieldSpec* spec(NULL);
		size_t offset;
		ts->DataStructure().GetFieldOffset(spec, offset, TSFactoryValue::fieldname_value);
		CPPUNIT_ASSERT_EQUAL(size_t(0), offset);
		ts->DataStructure().GetFieldOffset(spec, offset, TSFactoryOccValue::fieldname_occluded);
		CPPUNIT_ASSERT_EQUAL(size_t(3*sizeof(double)), offset);
		ts->DataStructure().GetFieldOffset(spec, offset, TSFactoryC3D::fieldname_confidence);
		CPPUNIT_ASSERT_EQUAL(size_t(3*sizeof(double)+1), offset);
		ts->DataStructure().GetFieldOffset(spec, offset, TSFactoryC3D::fieldname_c3dflag);
		CPPUNIT_ASSERT_EQUAL(size_t(3*sizeof(double)+1+3*sizeof(double)), offset);

		delete ts;
	}

	void testIterSetGet()
	{
		// make sequence with all fields
		TimeRange tr;
		tr.Frames = 10;
		tr.Rate = 1.0;
        BinMemFactoryDefault memfactory;
		TimeSequence* ts = TSFactoryC3D().New(tr, memfactory);

		// access to set
		{
			TSC3DIter i( *ts ); 

			i.Next();
			i.NextN(7);

			i.Value()[0] = 1.2;
			i.Value()[1] = 2.4;
			i.Value()[2] = -8.8;
			i.Occluded() = 0;
			i.Confidence()[0] = 2.2;
			i.Confidence()[1] = 3.3;
			i.Confidence()[2] = 4.4;
			i.CameraFlag() = UInt8(111);

			i.Next();

			i.Value()[0] = -2.0;
			i.Value()[1] = -3.0;
			i.Value()[2] = -4.0;
			i.Occluded() = 1;
			i.Confidence()[0] = 5.5;
			i.Confidence()[1] = 6.6;
			i.Confidence()[2] = 7.7;
			i.CameraFlag() = UInt8(222);
		}

		// access to get & check result
		{
			TSC3DConstIter ic( *ts );
			CPPUNIT_ASSERT_EQUAL(size_t(3), ic.ConfidenceDimension());
			CPPUNIT_ASSERT_EQUAL(true, ic.HasCameraFlag());
			ic.NextN(8);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(1.2, ic.Value()[0], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(2.4, ic.Value()[1], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-8.8, ic.Value()[2], 1E-12);
			CPPUNIT_ASSERT_EQUAL(UInt8(0), ic.Occluded());
			CPPUNIT_ASSERT_DOUBLES_EQUAL(2.2, ic.Confidence()[0], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(3.3, ic.Confidence()[1], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(4.4, ic.Confidence()[2], 1E-12);
			CPPUNIT_ASSERT_EQUAL(UInt8(111), ic.CameraFlag());
			ic.Next();
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.0, ic.Value()[0], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.0, ic.Value()[1], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-4.0, ic.Value()[2], 1E-12);
			CPPUNIT_ASSERT_EQUAL(UInt8(1), ic.Occluded());
			CPPUNIT_ASSERT_DOUBLES_EQUAL(5.5, ic.Confidence()[0], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(6.6, ic.Confidence()[1], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(7.7, ic.Confidence()[2], 1E-12);
			CPPUNIT_ASSERT_EQUAL(UInt8(222), ic.CameraFlag());

		}
	}

	void testIterDefaults()
	{
		// make sequence with only 3D value and occlusion fields
		TimeRange tr;
		tr.Frames = 10;
		tr.Rate = 1.0;
        BinMemFactoryDefault memfactory;
		TimeSequence* ts = TSFactoryOccValue(3).New(tr, memfactory);

		// access to set
		{
			TSOccVector3Iter i( *ts ); 

			i.Next();
			i.NextN(7);

			i.Value()[0] = 1.2;
			i.Value()[1] = 2.4;
			i.Value()[2] = -8.8;
			i.Occluded() = 0;

			i.Next();

			i.Value()[0] = -2.0;
			i.Value()[1] = -3.0;
			i.Value()[2] = -4.0;
			i.Occluded() = 1;
		}

		// access to get & check results of default items
		{
			TSC3DConstIter ic( *ts );
			CPPUNIT_ASSERT_EQUAL(size_t(0), ic.ConfidenceDimension());
			CPPUNIT_ASSERT_EQUAL(false, ic.HasCameraFlag());
			ic.NextN(8);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(1.2, ic.Value()[0], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(2.4, ic.Value()[1], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-8.8, ic.Value()[2], 1E-12);
			CPPUNIT_ASSERT_EQUAL(UInt8(0), ic.Occluded());
			ic.Next();
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.0, ic.Value()[0], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.0, ic.Value()[1], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-4.0, ic.Value()[2], 1E-12);
			CPPUNIT_ASSERT_EQUAL(UInt8(1), ic.Occluded());
		}
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION( TestTSC3D );
