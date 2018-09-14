/*--
Open3DMotion
Copyright (c) 2004-2018.
All rights reserved.
See LICENSE.txt for more information.
--*/

#include "Open3DMotion/Biomechanics/Trial/TimeSequence.h"
#include "Open3DMotion/Biomechanics/Trial/TSFactory.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinMemFactoryDefault.h"

#include <cppunit/extensions/HelperMacros.h>

using namespace Open3DMotion;
using namespace std;


class TestTimeSequence : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE(TestTimeSequence);
	CPPUNIT_TEST(testConstructor);
	CPPUNIT_TEST(testAllocate);
	CPPUNIT_TEST(testFactoryAllocate);
	CPPUNIT_TEST(testFactoryAllocateSameTimeRangeAs);
	CPPUNIT_TEST(testGetTimeRange);
	CPPUNIT_TEST(testTimeRangeIsSameAs);
	CPPUNIT_TEST(testToFromTree);
	CPPUNIT_TEST_SUITE_END();

public:

	void testConstructor()
	{
		TimeSequence ts;
		CPPUNIT_ASSERT_EQUAL(size_t(0), ts.NumFrames());
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, ts.Start.Value(), DBL_MIN);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, ts.Rate.Value(), DBL_MIN);
	}

	void testAllocate()
	{
		TimeSequence ts;
		TimeRange range;
		range.Frames = 23;
		range.Rate = 800.0;
		range.Start = 0.123;
		TSFactoryOccValue factory(3);
		ts.Allocate(factory.Layout(), range, BinMemFactoryDefault());
		CPPUNIT_ASSERT_EQUAL(size_t(23), ts.NumFrames());
		CPPUNIT_ASSERT_DOUBLES_EQUAL(800.0, ts.Rate.Value(), DBL_MIN);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.123, ts.Start.Value(), DBL_MIN);
		CPPUNIT_ASSERT_EQUAL(size_t(23 * (3 * sizeof(double) + 1)), ts.DataSizeBytes());
	}

	void testFactoryAllocate()
	{
		TimeSequence ts;
		TimeRange range;
		range.Frames = 23;
		range.Rate = 800.0;
		range.Start = 0.123;
		TSFactoryOccValue(3).Allocate(ts, range, BinMemFactoryDefault());
		CPPUNIT_ASSERT_EQUAL(size_t(23), ts.NumFrames());
		CPPUNIT_ASSERT_DOUBLES_EQUAL(800.0, ts.Rate.Value(), DBL_MIN);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.123, ts.Start.Value(), DBL_MIN);
		CPPUNIT_ASSERT_EQUAL(size_t(23 * (3 * sizeof(double) + 1)), ts.DataSizeBytes());
	}

	void testFactoryAllocateSameTimeRangeAs()
	{
		TimeSequence ts;

		{
			// make predicate sequence
			TimeSequence predicate;
			TimeRange range;
			range.Frames = 87;
			range.Rate = 400.0;
			range.Start = 0.175;
			TSFactoryValue(1).Allocate(predicate, range, BinMemFactoryDefault());

			// create new one with same time range (but different frame layout)
			TSFactoryOccValue(2).AllocateSameTimeRange(ts, predicate, BinMemFactoryDefault());
		}

		CPPUNIT_ASSERT_EQUAL(size_t(87), ts.NumFrames());
		CPPUNIT_ASSERT_DOUBLES_EQUAL(400.0, ts.Rate.Value(), DBL_MIN);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.175, ts.Start.Value(), DBL_MIN);
		CPPUNIT_ASSERT_EQUAL(size_t(87 * (2 * sizeof(double) + 1)), ts.DataSizeBytes());
	}

	void testGetTimeRange()
	{
		TimeRange range;
		range.Frames = 23;
		range.Rate = 800.0;
		range.Start = 0.123;

		TimeSequence ts;
		TSFactoryOccValue factory(3);
		ts.Allocate(factory.Layout(), range, BinMemFactoryDefault());

		TimeRange retrieval;
		ts.GetTimeRange(retrieval);
		CPPUNIT_ASSERT_EQUAL(Int32(23), retrieval.Frames.Value());
		CPPUNIT_ASSERT_DOUBLES_EQUAL(800.0, retrieval.Rate.Value(), DBL_MIN);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.123, retrieval.Start.Value(), DBL_MIN);
	}

	void testTimeRangeIsSameAs()
	{
		TimeRange a, b;

		a.Frames = 1001;
		a.Rate = 200.0;
		a.Start = 7.7;

		b.Frames = 1002;
		b.Rate = 200.0;
		b.Start = 7.7;

		TSFactoryOccValue factory_vector(3);
		TSFactoryOccValue factory_scalar(1);

		TimeSequence ts0;
		ts0.Allocate(factory_vector.Layout(), a, BinMemFactoryDefault());

		TimeSequence ts1;
		ts1.Allocate(factory_scalar.Layout(), a, BinMemFactoryDefault());

		TimeSequence ts2;
		ts2.Allocate(factory_vector.Layout(), b, BinMemFactoryDefault());

		CPPUNIT_ASSERT_EQUAL(true, ts0.TimeRangeIsSameAs(ts1));
		CPPUNIT_ASSERT_EQUAL(false , ts0.TimeRangeIsSameAs(ts2));
		CPPUNIT_ASSERT_EQUAL(false, ts2.TimeRangeIsSameAs(ts0));
	}

	void testToFromTree()
	{
		std::auto_ptr<TreeValue> tree;

		{
			// sample data
			TimeSequence tsinput;
			TimeRange range;
			range.Frames = 23;
			range.Rate = 800.0;
			range.Start = 0.123;
			TSFactoryOccValue(1).Allocate(tsinput, range, BinMemFactoryDefault());
			memset(tsinput.Data(), 0, tsinput.DataSizeBytes());
			TSOccScalarIter iter_input(tsinput);
			iter_input.Value() = 3.3;
			iter_input.Next();
			iter_input.Value() = 2.2;
			iter_input.Next();
			iter_input.Value() = -1.1;
			iter_input.Next();
			iter_input.Occluded() = 1;
			iter_input.NextN(19);
			iter_input.Value() = -7.7;

			// to tree
			tree.reset(tsinput.ToTree());
		}

		// back from tree
		TimeSequence tsresult;
		tsresult.FromTree(tree.get());

		// Check meta data
		CPPUNIT_ASSERT_EQUAL(size_t(23), tsresult.NumFrames());
		CPPUNIT_ASSERT_DOUBLES_EQUAL(800.0, tsresult.Rate.Value(), DBL_MIN);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.123, tsresult.Start.Value(), DBL_MIN);
		CPPUNIT_ASSERT_EQUAL(size_t(23) * (sizeof(double) + sizeof(UInt8)), tsresult.DataSizeBytes());

		// Quick check of data itself
		TSOccScalarConstIter iter_result(tsresult);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 3.3, iter_result.Value(), DBL_MIN);
		CPPUNIT_ASSERT_EQUAL(UInt8(0), iter_result.Occluded());
		iter_result.Next();
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.2, iter_result.Value(), DBL_MIN);
		CPPUNIT_ASSERT_EQUAL(UInt8(0), iter_result.Occluded());
		iter_result.Next();
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.1, iter_result.Value(), DBL_MIN);
		CPPUNIT_ASSERT_EQUAL(UInt8(0), iter_result.Occluded());
		iter_result.Next();
		CPPUNIT_ASSERT_EQUAL(UInt8(1), iter_result.Occluded());
		iter_result.NextN(19);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-7.7, iter_result.Value(), DBL_MIN);
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestTimeSequence);
