/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinMemFactoryDefault.h"
#include "Open3DMotion/Biomechanics/Trial/TimeSequence.h"
#include "Open3DMotion/Biomechanics/Trial/TimeRange.h"
#include "Open3DMotion/Biomechanics/Trial/EventGroup.h"
#include "Open3DMotion/Biomechanics/Trial/ForcePlate.h"

#include <cppunit/extensions/HelperMacros.h>

using namespace Open3DMotion;
using namespace std;


class TestTrialStructures  : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestTrialStructures );
	CPPUNIT_TEST( testTimeSequence );
	CPPUNIT_TEST( testEventGroup );
	CPPUNIT_TEST( testForcePlate );
	CPPUNIT_TEST_SUITE_END();

public:

	void testTimeSequence()
	{

		BinMemFactoryDefault mem;

		// tree to populate
		TreeValue* t = NULL; 

		{
			//  blank ts
			TimeSequence ts0;

			// allocate
			vector<BinaryFieldSpec> layout;
			layout.push_back(BinaryFieldSpec::FromType<float>("Position", 3L));
			TimeRange tr;
			tr.Start = 0.777;
			tr.Frames = 100;
			tr.Rate = 103.5;
			ts0.Allocate(layout, tr, mem);

			// verify some basic properties
			CPPUNIT_ASSERT_EQUAL(size_t(1), ts0.DataStructure().GetLayout().NumElements());
			CPPUNIT_ASSERT_EQUAL(string("Position"), ts0.DataStructure().GetLayout()[0].Name.Value());
			CPPUNIT_ASSERT_EQUAL(size_t(12*100), ts0.DataSizeBytes());
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.777, ts0.Start.Value(), 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(103.5, ts0.Rate.Value(), 1E-12);
			CPPUNIT_ASSERT_EQUAL(Int32(100), ts0.NumFrames());

			// set some dummy data
			memcpy(ts0.Data(), "Hello", 6);

			// attributes
			ts0.Group = "My Gang";
			ts0.Channel = "Tuned In";
			ts0.HardwareID = 999L;
			ts0.Units = "BIG";
			ts0.Scale = 1000000.1;
			ts0.Offset = 2.222;
			ts0.OffsetStdDev = 0.00003;		

			// send to tree
			t = ts0.ToTree();
		}

		{
			// round-trip back from tree
			TimeSequence ts1;
			ts1.FromTree(t);

			// re-verify basic properties
			CPPUNIT_ASSERT_EQUAL(size_t(1), ts1.DataStructure().GetLayout().NumElements());
			CPPUNIT_ASSERT_EQUAL(string("Position"), ts1.DataStructure().GetLayout()[0].Name.Value());
			CPPUNIT_ASSERT_EQUAL(size_t(12*100), ts1.DataSizeBytes());
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.777, ts1.Start.Value(), 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(103.5, ts1.Rate.Value(), 1E-12);
			CPPUNIT_ASSERT_EQUAL(Int32(100), ts1.NumFrames());

			// verify dummy data
			CPPUNIT_ASSERT_EQUAL(string("Hello"), string((const char*)ts1.Data()));

			// verify attributes
			CPPUNIT_ASSERT_EQUAL(string("My Gang"), ts1.Group.Value());
			CPPUNIT_ASSERT_EQUAL(string("Tuned In"), ts1.Channel.Value());
			CPPUNIT_ASSERT_EQUAL(Int32(999), ts1.HardwareID.Value());
			CPPUNIT_ASSERT_EQUAL(string("BIG"), ts1.Units.Value());
			CPPUNIT_ASSERT_DOUBLES_EQUAL(1000000.1, ts1.Scale.Value(), 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(2.222, ts1.Offset.Value(), 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.00003, ts1.OffsetStdDev.Value(), 1E-12);
		}

		// done with tree
		delete t;
	}

	void testEventGroup()
	{
		// Empty group with name
		EventGroup e0;
		e0.Name = "Animal Events";

		// populate
		{
			EventArray a0;
			a0.AddEvent(2.0, "Elephant");
			a0.AddEvent(1.5, "Ostrich");
			a0.AddEvent(3.8, "Elephant");
			a0.AddEvent(4.5, "Manatee");
			BinMemFactoryDefault mem;
			e0.SetEvents(a0,	mem);
		}

		// check round-trip from-to EventArray class
		{
			EventArray a1;
			e0.GetEvents(a1);

			CPPUNIT_ASSERT_EQUAL(size_t(4), a1.NumEvents());
			CPPUNIT_ASSERT_DOUBLES_EQUAL(1.5, a1.EventTime(0), 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, a1.EventTime(1), 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(3.8, a1.EventTime(2), 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(4.5, a1.EventTime(3), 1E-12);
			CPPUNIT_ASSERT_EQUAL(string("Ostrich"), a1.EventName(0));
			CPPUNIT_ASSERT_EQUAL(string("Elephant"), a1.EventName(1));
			CPPUNIT_ASSERT_EQUAL(string("Elephant"), a1.EventName(2));
			CPPUNIT_ASSERT_EQUAL(string("Manatee"), a1.EventName(3));
		}

		// check round-trip to tree
		{
			// do round-trip
			TreeValue* t = e0.ToTree();
			EventGroup e1;
			e1.FromTree(t);

			// name
			CPPUNIT_ASSERT_EQUAL(string("Animal Events"), e1.Name.Value());

			// events
			EventArray a2;
			e1.GetEvents(a2);
			CPPUNIT_ASSERT_EQUAL(size_t(4), a2.NumEvents());
			CPPUNIT_ASSERT_DOUBLES_EQUAL(1.5, a2.EventTime(0), 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, a2.EventTime(1), 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(3.8, a2.EventTime(2), 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(4.5, a2.EventTime(3), 1E-12);
			CPPUNIT_ASSERT_EQUAL(string("Ostrich"), a2.EventName(0));
			CPPUNIT_ASSERT_EQUAL(string("Elephant"), a2.EventName(1));
			CPPUNIT_ASSERT_EQUAL(string("Elephant"), a2.EventName(2));
			CPPUNIT_ASSERT_EQUAL(string("Manatee"), a2.EventName(3));
		}
	}

	void testForcePlate()
	{
		ForcePlate f;
		// ForcePlate g;
		// g = f;

		vector<ForcePlate> fparray;
		fparray.push_back(f);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestTrialStructures );


