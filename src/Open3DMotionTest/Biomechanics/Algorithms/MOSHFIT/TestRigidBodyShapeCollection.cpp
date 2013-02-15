#include "Open3DMotion/Biomechanics/Algorithms/MOSHFIT/RigidBodyShapeCollection.h"
#include "Open3DMotion/Maths/RigidTransform3.h"
#include "Open3DMotion/Biomechanics/Trial/TSFactory.h"
#include <cppunit/extensions/HelperMacros.h>
#include <math.h>
#include <vector>
#include <time.h>
#include <list>

using namespace Open3DMotion;
using namespace std;

// basic vis matrix with all codes for 4 markers
static UInt8 testvismatrix1[16*4] = 
{
0,0,0,0,	
0,0,0,1,
0,0,1,0,
0,0,1,1,
0,1,0,0,
0,1,0,1,
0,1,1,0,
0,1,1,1,	//  7
1,0,0,0,
1,0,0,1,
1,0,1,0,
1,0,1,1,	// 11
1,1,0,0,
1,1,0,1,	// 13
1,1,1,0,	// 14
1,1,1,1	
};

// same as testvismatrix1 but with 3 all-visible fields inserted before other codes
static UInt8 testvismatrix2[64*4] = 
{
1,1,1,1,
1,1,1,1,
1,1,1,1,
0,0,0,0,
1,1,1,1,
1,1,1,1,
1,1,1,1,
0,0,0,1,
1,1,1,1,
1,1,1,1,
1,1,1,1,
0,0,1,0,
1,1,1,1,
1,1,1,1,
1,1,1,1,
0,0,1,1,
1,1,1,1,
1,1,1,1,
1,1,1,1,
0,1,0,0,
1,1,1,1,
1,1,1,1,
1,1,1,1,
0,1,0,1,
1,1,1,1,
1,1,1,1,
1,1,1,1,
0,1,1,0,
1,1,1,1,
1,1,1,1,
1,1,1,1,
0,1,1,1,
1,1,1,1,
1,1,1,1,
1,1,1,1,
1,0,0,0,
1,1,1,1,
1,1,1,1,
1,1,1,1,
1,0,0,1,
1,1,1,1,
1,1,1,1,
1,1,1,1,
1,0,1,0,
1,1,1,1,
1,1,1,1,
1,1,1,1,
1,0,1,1,
1,1,1,1,
1,1,1,1,
1,1,1,1,
1,1,0,0,
1,1,1,1,
1,1,1,1,
1,1,1,1,
1,1,0,1,
1,1,1,1,
1,1,1,1,
1,1,1,1,
1,1,1,0,
1,1,1,1,
1,1,1,1,
1,1,1,1,
1,1,1,1,
};

// testvismatrix2 with out-of-view dilated +- 1 sample
static UInt8 testvismatrix2_dilate1[64*4] = 
{
1,1,1,1,
1,1,1,1,
0,0,0,0,
0,0,0,0,
0,0,0,0,
1,1,1,1,
0,0,0,1,
0,0,0,1,
0,0,0,1,
1,1,1,1,
0,0,1,0,
0,0,1,0,
0,0,1,0,
1,1,1,1,
0,0,1,1,
0,0,1,1,
0,0,1,1,
1,1,1,1,
0,1,0,0,
0,1,0,0,
0,1,0,0,
1,1,1,1,
0,1,0,1,
0,1,0,1,
0,1,0,1,
1,1,1,1,
0,1,1,0,
0,1,1,0,
0,1,1,0,
1,1,1,1,
0,1,1,1,
0,1,1,1,
0,1,1,1,
1,1,1,1,
1,0,0,0,
1,0,0,0,
1,0,0,0,
1,1,1,1,
1,0,0,1,
1,0,0,1,
1,0,0,1,
1,1,1,1,
1,0,1,0,
1,0,1,0,
1,0,1,0,
1,1,1,1,
1,0,1,1,
1,0,1,1,
1,0,1,1,
1,1,1,1,
1,1,0,0,
1,1,0,0,
1,1,0,0,
1,1,1,1,
1,1,0,1,
1,1,0,1,
1,1,0,1,
1,1,1,1,
1,1,1,0,
1,1,1,0,
1,1,1,0,
1,1,1,1,
1,1,1,1,
1,1,1,1,
};

class TestRigidBodyShapeCollection : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestRigidBodyShapeCollection );
	CPPUNIT_TEST( testSelfPopulateTimeSequences );
	CPPUNIT_TEST( testEmptyConstructor );
	CPPUNIT_TEST( testCopyFromTimeSequences );
	CPPUNIT_TEST( testDilateVisibility );
	CPPUNIT_TEST( testVisibilityCanon );
	CPPUNIT_TEST( testConnectedVisibilityCanon );
	CPPUNIT_TEST( testComputeApproxMeanShape );
	CPPUNIT_TEST( testComputeMeanShape );
	CPPUNIT_TEST( testMOSHFIT );
	CPPUNIT_TEST_SUITE_END();

public:

	static void PopulateTimeSequences(std::vector<const TimeSequence*>& input, const UInt8* vismatrix, size_t num_markers, size_t num_frames)
	{
		TimeRange tr;
		tr.Frames = num_frames;
		tr.Rate = 1.0;
		for (size_t imarker = 0; imarker < num_markers; imarker++)
		{
			TimeSequence* ts = TSFactoryOccValue(3).New(tr, BinMemFactoryDefault());
			size_t iframe = 0;
			for (TSOccVector3Iter iter(*ts); iter.HasFrame(); iter.Next(), iframe++)
			{
				iter.Value()[0] = (double)iframe;
				iter.Value()[1] = 0.0;
				iter.Value()[2] = -(double)iframe;
				iter.Occluded() = vismatrix[iter.FrameIndex()*num_markers+imarker] ? 0 : 1;
			}
			input.push_back( ts );
		}
	}

public:

	void testSelfPopulateTimeSequences()
	{
		UInt8 vismatrix[6] = 
		{
			0, 1, 1,
			1, 0, 1
		};

		std::vector<const TimeSequence*> ts;
		PopulateTimeSequences(ts, vismatrix, 3, 2);
		CPPUNIT_ASSERT_EQUAL(size_t(3), ts.size());

		TSOccVector3ConstIter i0( *ts[0] );
		TSOccVector3ConstIter i1( *ts[1] );
		TSOccVector3ConstIter i2( *ts[2] );
		CPPUNIT_ASSERT_EQUAL(size_t(2), i0.NumFrames());
		CPPUNIT_ASSERT_EQUAL(size_t(2), i1.NumFrames());
		CPPUNIT_ASSERT_EQUAL(size_t(2), i2.NumFrames());
		CPPUNIT_ASSERT_EQUAL(UInt8(1), i0.Occluded());
		CPPUNIT_ASSERT_EQUAL(UInt8(0), i1.Occluded());
		CPPUNIT_ASSERT_EQUAL(UInt8(0), i2.Occluded());
		i0.Next();
		i1.Next();
		i2.Next();
		CPPUNIT_ASSERT_EQUAL(UInt8(0), i0.Occluded());
		CPPUNIT_ASSERT_EQUAL(UInt8(1), i1.Occluded());
		CPPUNIT_ASSERT_EQUAL(UInt8(0), i2.Occluded());
	}

	void testEmptyConstructor()
	{
		RigidBodyShapeCollection c;
		CPPUNIT_ASSERT_EQUAL(size_t(0), c.NumShapes());
	}

	void testCopyFromTimeSequences()
	{
		// make input data with vis codes as above
		vector<const TimeSequence*> input;
		PopulateTimeSequences(input, testvismatrix1, 4, 16);

		// mismatched input
		vector<const TimeSequence*> wronginput;
		wronginput.push_back( input[0] );
		TimeRange tr_wrong;
		tr_wrong.Frames = 7;
		tr_wrong.Rate = 1.0;
		wronginput.push_back( TSFactoryOccValue(3).New(tr_wrong, BinMemFactoryDefault()) );
		
		// check get error
		RigidBodyShapeCollection c_wrong;
		UInt32 wrong_result = c_wrong.CopyFromTimeSequences(wronginput);
		CPPUNIT_ASSERT_EQUAL(RigidBodyResult::timesequence_mismatch, wrong_result);

		RigidBodyShapeCollection c;
		c.CopyFromTimeSequences( input );
	
		CPPUNIT_ASSERT_EQUAL(size_t(16), c.NumShapes());
		for (size_t iframe = 0; iframe < 16; iframe++)
		{
			const RigidBodyShape& s = c.Shape(iframe);
			CPPUNIT_ASSERT_EQUAL(size_t(4), s.NumMarkers());
			for (size_t imarker = 0; imarker < 4; imarker++)
			{
				UInt8 v = s.Marker(imarker).visible;
				if (v != testvismatrix1[4*iframe+imarker])
				{
					char msg[256];
					sprintf(msg, "marker % d frame %d expected %d got %d",
						(int)imarker, (int)iframe, (int)testvismatrix1[4*iframe+imarker], (int)v);
					CPPUNIT_FAIL(msg);
				}
			}
		}
	
	}

	void testDilateVisibility() 
	{
		// make input data with vis codes as above
		vector<const TimeSequence*> input;
		PopulateTimeSequences(input, testvismatrix2, 4, 64);

		// copy input
		RigidBodyShapeCollection c;
		c.CopyFromTimeSequences(input);

		// dilate by 1
		c.DilateVisiblity(1);

		CPPUNIT_ASSERT_EQUAL(size_t(64), c.NumShapes());
		for (size_t iframe = 0; iframe < 64; iframe++)
		{
			const RigidBodyShape& s = c.Shape(iframe);
			CPPUNIT_ASSERT_EQUAL(size_t(4), s.NumMarkers());
			for (size_t imarker = 0; imarker < 4; imarker++)
			{
				UInt8 v = s.Marker(imarker).visible;
				if (v != testvismatrix2_dilate1[4*iframe+imarker])
				{
					char msg[256];
					sprintf(msg, "marker % d frame %d expected %d got %d",
						(int)imarker, (int)iframe, (int)testvismatrix2_dilate1[4*iframe+imarker], (int)v);
					CPPUNIT_FAIL(msg);
				}
			}
		}
	}

	void testVisibilityCanon() 
	{
		// run all codes for 4 markers - should give just 1,1,1,1 as canon
		std::vector<const TimeSequence*> input_allcodes;
		PopulateTimeSequences(input_allcodes, testvismatrix1, 4, 16);
		RigidBodyShapeCollection allcodes;
		allcodes.CopyFromTimeSequences(input_allcodes);

		// compute canon
		RigidBodyVisibilityCanon canon_all;
		allcodes.ComputeVisiblityCanon(canon_all);

		CPPUNIT_ASSERT_EQUAL(size_t(1), canon_all.NumShapes());
		CPPUNIT_ASSERT_EQUAL(size_t(4), canon_all.Shape(0).NumMarkers());
		CPPUNIT_ASSERT_EQUAL(UInt8(1), canon_all.Shape(0).Marker(0).visible);
		CPPUNIT_ASSERT_EQUAL(UInt8(1), canon_all.Shape(0).Marker(1).visible);
		CPPUNIT_ASSERT_EQUAL(UInt8(1), canon_all.Shape(0).Marker(2).visible);
		CPPUNIT_ASSERT_EQUAL(UInt8(1), canon_all.Shape(0).Marker(3).visible);

		// run all codes for 4 markers excluding 1,1,1,1
		// - should give the 4 disjoint codes:
		// 0,1,1,1 ; 1,0,1,1 ; 1,1,0,1 ; 1,1,1,0
		// - these are at the following indices in testvismatrix1:
		//  7      ; 11      ; 13      ; 14
		std::vector<const TimeSequence*> input_disjoint;
		PopulateTimeSequences(input_disjoint, testvismatrix1, 4, 15);
		RigidBodyShapeCollection disjoint;
		disjoint.CopyFromTimeSequences(input_disjoint);
		RigidBodyVisibilityCanon canon_disjoint;
		disjoint.ComputeVisiblityCanon(canon_disjoint);

		CPPUNIT_ASSERT_EQUAL(size_t(4), canon_disjoint.NumShapes());
		CPPUNIT_ASSERT_EQUAL(size_t(4), canon_disjoint.Shape(0).NumMarkers());
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 7.0, canon_disjoint.Shape(0).Marker(0).position[0], 1E-12);
		CPPUNIT_ASSERT_EQUAL(UInt8(0), canon_disjoint.Shape(0).Marker(0).visible);
		CPPUNIT_ASSERT_EQUAL(UInt8(1), canon_disjoint.Shape(0).Marker(1).visible);
		CPPUNIT_ASSERT_EQUAL(UInt8(1), canon_disjoint.Shape(0).Marker(2).visible);
		CPPUNIT_ASSERT_EQUAL(UInt8(1), canon_disjoint.Shape(0).Marker(3).visible);
		CPPUNIT_ASSERT_EQUAL(size_t(4), canon_disjoint.Shape(1).NumMarkers());
		CPPUNIT_ASSERT_DOUBLES_EQUAL(11.0, canon_disjoint.Shape(1).Marker(0).position[0], 1E-12);
		CPPUNIT_ASSERT_EQUAL(UInt8(1), canon_disjoint.Shape(1).Marker(0).visible);
		CPPUNIT_ASSERT_EQUAL(UInt8(0), canon_disjoint.Shape(1).Marker(1).visible);
		CPPUNIT_ASSERT_EQUAL(UInt8(1), canon_disjoint.Shape(1).Marker(2).visible);
		CPPUNIT_ASSERT_EQUAL(UInt8(1), canon_disjoint.Shape(1).Marker(3).visible);
		CPPUNIT_ASSERT_EQUAL(size_t(4), canon_disjoint.Shape(2).NumMarkers());
		CPPUNIT_ASSERT_DOUBLES_EQUAL(13.0, canon_disjoint.Shape(2).Marker(0).position[0], 1E-12);
		CPPUNIT_ASSERT_EQUAL(UInt8(1), canon_disjoint.Shape(2).Marker(0).visible);
		CPPUNIT_ASSERT_EQUAL(UInt8(1), canon_disjoint.Shape(2).Marker(1).visible);
		CPPUNIT_ASSERT_EQUAL(UInt8(0), canon_disjoint.Shape(2).Marker(2).visible);
		CPPUNIT_ASSERT_EQUAL(UInt8(1), canon_disjoint.Shape(2).Marker(3).visible);
		CPPUNIT_ASSERT_EQUAL(size_t(4), canon_disjoint.Shape(3).NumMarkers());
		CPPUNIT_ASSERT_DOUBLES_EQUAL(14.0, canon_disjoint.Shape(3).Marker(0).position[0], 1E-12);
		CPPUNIT_ASSERT_EQUAL(UInt8(1), canon_disjoint.Shape(3).Marker(0).visible);
		CPPUNIT_ASSERT_EQUAL(UInt8(1), canon_disjoint.Shape(3).Marker(1).visible);
		CPPUNIT_ASSERT_EQUAL(UInt8(1), canon_disjoint.Shape(3).Marker(2).visible);
		CPPUNIT_ASSERT_EQUAL(UInt8(0), canon_disjoint.Shape(3).Marker(3).visible);
	}

	void testConnectedVisibilityCanon()
	{
		std::vector<const TimeSequence*> input_allcodes;
		PopulateTimeSequences(input_allcodes, testvismatrix1, 4, 16);
		RigidBodyShapeCollection allcodes;
		allcodes.CopyFromTimeSequences(input_allcodes);
		RigidBodyVisibilityCanon canon_all;
		allcodes.ComputeVisiblityCanon(canon_all);

		// connectivity matrix for single canon should just be 1 element of value 0
		RigidBodyConnectedVisibilityCanon connect_all;
		canon_all.ComputeConnectedVisibilityCanon(connect_all, 1.0);
		CPPUNIT_ASSERT_EQUAL(size_t(1), connect_all.NumShapes());
		CPPUNIT_ASSERT_EQUAL(size_t(4), connect_all.Shape(0).NumMarkers());
		CPPUNIT_ASSERT_DOUBLES_EQUAL(15.0, connect_all.Shape(0).Marker(0).position[0], 1E-12);
		CPPUNIT_ASSERT_EQUAL(UInt8(1), connect_all.Shape(0).Marker(0).visible);
		CPPUNIT_ASSERT_EQUAL(UInt8(1), connect_all.Shape(0).Marker(1).visible);
		CPPUNIT_ASSERT_EQUAL(UInt8(1), connect_all.Shape(0).Marker(2).visible);
		CPPUNIT_ASSERT_EQUAL(UInt8(1), connect_all.Shape(0).Marker(3).visible);

		// there is no connectivity between codes which have less than 2 markers in common
		// should have root element equal to first code with 3 markers in view and then nothing else
		std::vector<const TimeSequence*> input_disjoint;
		PopulateTimeSequences(input_disjoint, testvismatrix1, 4, 15);
		RigidBodyShapeCollection disjoint;
		disjoint.CopyFromTimeSequences(input_disjoint);
		RigidBodyVisibilityCanon canon_disjoint;
		disjoint.ComputeVisiblityCanon(canon_disjoint);

		RigidBodyConnectedVisibilityCanon connect_disjoint;
		canon_disjoint.ComputeConnectedVisibilityCanon(connect_disjoint, 1.0);
		CPPUNIT_ASSERT_EQUAL(size_t(1), connect_disjoint.NumShapes());
		CPPUNIT_ASSERT_EQUAL(size_t(4), connect_disjoint.Shape(0).NumMarkers());
		CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, connect_disjoint.Shape(0).Marker(0).position[0], 1E-12);

		// some other tests

		// code 0 is connected to code 2
		// code 1 is not connected to either
		UInt8 testconn1[3*6] = 
		{
			0,1,1,1,1,0,
			1,1,0,0,0,1,
			1,0,1,1,1,0,
		};

		// make data
		std::vector<const TimeSequence*> tsconn1;
		PopulateTimeSequences(tsconn1, testconn1, 6, 3);

		TSOccVector3Iter ic2( const_cast<TimeSequence&>( *tsconn1[2] ) );
		TSOccVector3Iter ic3( const_cast<TimeSequence&>( *tsconn1[3] ) );
		TSOccVector3Iter ic4( const_cast<TimeSequence&>( *tsconn1[4] ) );

		// a triangle along roughly orthogonal axes
		ic2.Value()[0] =  -3.0;
		ic2.Value()[1] =   2.0;
		ic2.Value()[2] =   7.2;
		ic3.Value()[0] = 101.0;
		ic3.Value()[1] =  12.0;
		ic3.Value()[2] =  7.1;
		ic4.Value()[0] = 100.0;
		ic4.Value()[1] = 113.0;
		ic4.Value()[2] =  7.3;

		// ignore middle row of frames
		// and advance to last row of frames
		ic2.NextN(2);
		ic3.NextN(2);
		ic4.NextN(2);

		// same thing but offset a bit and slightly distorted
		ic2.Value()[0] =   7.7;
		ic2.Value()[1] =  12.8;
		ic2.Value()[2] =  17.2;
		ic3.Value()[0] = 111.5;
		ic3.Value()[1] =  22.6;
		ic3.Value()[2] =  17.1;
		ic4.Value()[0] = 110.9;
		ic4.Value()[1] = 123.2;
		ic4.Value()[2] =  17.3;

		RigidBodyShapeCollection collection_conn1;
		collection_conn1.CopyFromTimeSequences(tsconn1);
		RigidBodyVisibilityCanon canon_conn1;
		collection_conn1.ComputeVisiblityCanon(canon_conn1);

		// get result
		RigidBodyConnectedVisibilityCanon result_conn1;
		UInt32 status_conn1 = canon_conn1.ComputeConnectedVisibilityCanon(result_conn1, 1.0);

		// should say there was a mismatch, since 2nd code not connected
		CPPUNIT_ASSERT_EQUAL(RigidBodyResult::timesequence_mismatch, status_conn1);

		// code 0 in level 0, code 1 in no level, code 2 in level 1
		CPPUNIT_ASSERT_EQUAL(size_t(2), result_conn1.NumShapes());
		CPPUNIT_ASSERT_EQUAL(size_t(6), result_conn1.Shape(0).NumMarkers());
		CPPUNIT_ASSERT_EQUAL(size_t(6), result_conn1.Shape(1).NumMarkers());
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.0, result_conn1.Shape(0).Marker(2).position[0], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 7.7, result_conn1.Shape(1).Marker(2).position[0], 1E-12);
	}

	void testComputeApproxMeanShape()
	{
		TimeRange tr;
		tr.Frames = 3;
		tr.Start = 0.0;
		tr.Rate = 1.0;

		double testshape[12] =
		{
			0.0, 0.0, 0.0,
			4.0, 0.0, 0.0,
			4.0, 3.0, 0.0,
			0.0, 3.0, 0.0
		};

		double testscale[3] =
		{
			1.1,
			0.9,
			1.0
		};

		bool occluded_frame_marker[12] = 
		{ 
			true, false, false, false,
			true, false, false, false,
			false, false, false, false
		};

		vector<const TimeSequence*> tsarray;
		for (UInt32 imarker = 0; imarker < 4; imarker++)
		{
			TimeSequence* ts = TSFactoryOccValue(3).New(tr, BinMemFactoryDefault());
			TSOccVector3Iter iter( *ts );
			for (UInt32 iframe = 0; iframe < 3; iframe++)
			{
				iter.Occluded() = occluded_frame_marker[iframe*4 + imarker];
				if (iter.Occluded())
				{
					iter.Value()[0] = 0.0;
					iter.Value()[1] = 0.0;
					iter.Value()[2] = 0.0;
				}
				else
				{
					iter.Value()[0] = testscale[iframe]*testshape[3*imarker+0];
					iter.Value()[1] = testscale[iframe]*testshape[3*imarker+1];
					iter.Value()[2] = testscale[iframe]*testshape[3*imarker+2];
				}
				iter.Next();
			}
			tsarray.push_back(ts);
		}

		RigidBodyShapeCollection collection;
		collection.CopyFromTimeSequences(tsarray);

		// minimal set of visiblity codes
		RigidBodyVisibilityCanon canon;
		collection.ComputeVisiblityCanon(canon);

		//  re-order codes to ensure maximal ones go first
		RigidBodyConnectedVisibilityCanon connected_canon;
		UInt32 canon_result = canon.ComputeConnectedVisibilityCanon(connected_canon, 0.01);
		CPPUNIT_ASSERT_EQUAL(RigidBodyResult::success, canon_result);

		// should get mean shape output
		RigidBodyShape mean;
		connected_canon.ComputeApproxMeanShape(mean);

		// verify result
		CPPUNIT_ASSERT_EQUAL(size_t(4), mean.NumMarkers());
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mean.Marker(0).position[0], 0.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mean.Marker(0).position[1], 0.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mean.Marker(0).position[2], 0.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, mean.Marker(1).position[0], 0.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mean.Marker(1).position[1], 0.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mean.Marker(1).position[2], 0.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, mean.Marker(2).position[0], 0.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, mean.Marker(2).position[1], 0.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mean.Marker(2).position[2], 0.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mean.Marker(3).position[0], 0.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, mean.Marker(3).position[1], 0.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mean.Marker(3).position[2], 0.0001);
	}

	void testComputeMeanShape()
	{
		TimeRange tr;
		tr.Frames = 10000;
		tr.Start = 0.0;
		tr.Rate = 1.0;

		double testshape[12] =
		{
			0.0, 0.0, 0.0,
			4.0, 0.0, 0.0,
			4.0, 3.0, 0.0,
			0.0, 3.0, 0.0
		};

		vector<const TimeSequence*> tsarray;
		for (UInt32 imarker = 0; imarker < 4; imarker++)
		{
			TimeSequence* ts = TSFactoryOccValue(3).New(tr, BinMemFactoryDefault());
			TSOccVector3Iter iter( *ts );
			for (UInt32 iframe = 0; iframe < (UInt32)tr.Frames; iframe++)
			{
				iter.Value()[0] = testshape[3*imarker+0] + 0.2*(-0.5 + ((double)rand() / (double)RAND_MAX));
				iter.Value()[1] = testshape[3*imarker+1] + 0.2*(-0.5 + ((double)rand() / (double)RAND_MAX));
				iter.Value()[2] = testshape[3*imarker+2] + 0.2*(-0.5 + ((double)rand() / (double)RAND_MAX));
				iter.Occluded() = 0;
				iter.Next();
			}
			tsarray.push_back(ts);
		}

		// put input
		RigidBodyShapeCollection collection;
		collection.CopyFromTimeSequences(tsarray);

		// do the sums
		clock_t time0 = clock();
		RigidBodyShape mean;
		UInt32 status = collection.ComputeMeanShape(mean, 0.1, 0.000001);
		clock_t time_taken = clock() - time0;
		fprintf(stderr, "RigidBodyShapeCollectionTest::testComputeMeanShape time taken: %.3lfs/%d frames/%d markers\n",
			(double)time_taken / (double)CLK_TCK, (int)collection.NumShapes(), (int)collection.Shape(0).NumMarkers());
    CPPUNIT_ASSERT_EQUAL(RigidBodyResult::success, status);

		// the result is only correct up to rigid transform
		// - transform back so we can check it's ok
		RigidBodyShape templ_shape;
		templ_shape.AddMarker(&testshape[0], true);
		templ_shape.AddMarker(&testshape[3], true);
		templ_shape.AddMarker(&testshape[6], true);
		templ_shape.AddMarker(&testshape[9], true);
		RigidTransform3 T;
		mean.ComputeFitTo(T, templ_shape);
		for (size_t marker_index = 0; marker_index < 4; marker_index++)
		{
			Vector3 Tx;
			RigidTransform3::MulVec(Tx, T, templ_shape.Marker(marker_index).position);
			mean.Marker(marker_index).position = Tx;
		}

		// verify result
		CPPUNIT_ASSERT_EQUAL(size_t(4), mean.NumMarkers());
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mean.Marker(0).position[0], 0.1);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mean.Marker(0).position[1], 0.1);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mean.Marker(0).position[2], 0.1);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, mean.Marker(1).position[0], 0.1);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mean.Marker(1).position[1], 0.1);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mean.Marker(1).position[2], 0.1);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, mean.Marker(2).position[0], 0.1);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, mean.Marker(2).position[1], 0.1);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mean.Marker(2).position[2], 0.1);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mean.Marker(3).position[0], 0.1);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, mean.Marker(3).position[1], 0.1);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mean.Marker(3).position[2], 0.1);
	}

	class NinePointCode
	{
	public:
		NinePointCode()
		{
			for (UInt8 i = 0; i < 9; i++)
				occluded[i] = 0;
		}

	public:
		UInt8 occluded[9];
	};

	static void ProjectionAngles(double* a, const double* R)
	{
		const double radians_to_degrees = 57.295779513082320876798154814105;
		a[0] = radians_to_degrees * acos(R[0]);
		a[1] = radians_to_degrees * acos(R[4]);
		a[2] = radians_to_degrees * acos(R[8]);
	}

	static void ExponentialMap(Matrix3x3& R,const Vector3& a)
	{
		double ang = a.Modulus();
		double c(0), s(0), v(0);
		Vector3 na(0.0);
		if (ang > 1E-12)
		{
				na[0] = a[0] / ang;
				na[1] = a[1] / ang;
				na[2] = a[2] / ang;
				c = cos(ang);
				s = sin(ang);
				v = 1 - c;
		}
		else
		{
			na = a;
			c = 1   - 0.50000000*ang*ang;
			v = 0.5 - 0.04166667*ang*ang;
			s = 1.0 - 0.16666667*ang*ang;
		}

		R[0] = v*na[0]*na[0] + c;
		R[1] = v*na[1]*na[0] - na[2]*s;
		R[2] = v*na[2]*na[0] + na[1]*s;

		R[3] = v*na[0]*na[1] + na[2]*s;
		R[4] = v*na[1]*na[1] + c;
		R[5] = v*na[2]*na[1] - na[0]*s;

		R[6] = v*na[0]*na[2] - na[1]*s;
		R[7] = v*na[1]*na[2] + na[0]*s;
		R[8] = v*na[2]*na[2] + c;

	}

	static double RandomUnit()
	{
		return (1.0 + (double)rand()) / (1.0 + (double)RAND_MAX);
	}

	static double RandomGauss()
	{
		double u1 = RandomUnit();
		double u2 = RandomUnit();
		double g = sqrt(-2.0*log(u1))*cos(6.283185307179586476925286766559*u2);
		CPPUNIT_ASSERT(_finite(g));
		return g;
	}

	void testMOSHFIT()
	{
		// Number of repeats of experiment
		// - set this to 1000 to recreate the results of MOSHFIT paper
		const UInt32 num_reps = 5;
		// const UInt32 num_reps = 1000;

		// Shape of gaussian noise
		const Vector3 noise(2.0, 2.0, 2.0); // spherical 2mm
		// const Vector3 noise(1.0, 8.0, 1.0); // non-spherical (1mm, 8mm, 1mm)

		// template shape
		double nine_point_shape[3*9] =
		{
			-80.0,  25.0,  50.0,
			-75.0,   0.0,   0.0,
			-25.0,  50.0,   0.0,
				0.0,  50.0, -25.0,
				0.0, -25.0,  50.0,
			 25.0,   0.0,   0.0,
			 75.0,   0.0,   0.0,
			 80.0, -50.0,  25.0,
			 85.0,   0.0, -50.0
		};

		// ground truth shape - used to set coordinate system for angle comparison
		RigidBodyShape shape_truth;
		UInt32 imarker;
		for (UInt32 imarker = 0; imarker < 9; imarker++)
		{
			shape_truth.AddMarker(nine_point_shape+3*imarker, true);
		}

		double sum_clock = 0.0;
		double rms_min = DBL_MAX;
		double rms_max = 0.0;
		double angle_rms_min = DBL_MAX;
		double angle_rms_max = 0.0;
		double proc_rms_min = DBL_MAX;
		double proc_rms_max = 0.0;
		double proc_angle_rms_min = DBL_MAX;
		double proc_angle_rms_max = 0.0;
		for (UInt32 irep = 0; irep < num_reps; irep++)
		{
			fprintf(stderr, "RigidBodyShapeCollectionTest::testMoshFit experiment %u\n", irep);

			// make vector of all visibility codes, repeated a number of times
			std::list<NinePointCode> viscode;
			for (UInt8 i0 = 0; i0 < 5; i0++)
			{
				for (UInt8 i1 = i0+1; i1 < 6; i1++)
				{
					for (UInt8 i2 = i1+1; i2 < 7; i2++)
					{
						for (UInt8 i3 = i2+1; i3 < 8; i3++)
						{
							for (UInt8 i4 = i3+1; i4 < 9; i4++)
							{
								NinePointCode n;
								n.occluded[i0] = 1;
								n.occluded[i1] = 1;
								n.occluded[i2] = 1;
								n.occluded[i3] = 1;
								n.occluded[i4] = 1;
								for (UInt8 repcount = 0; repcount < 10; repcount++)
								{
									viscode.push_back(n);
								}
							}		
						}		
					}
				}
			}

			// build empty TS array
			UInt32 iframe;
			vector<TimeSequence*> tstruth;
			vector<TimeSequence*> tsinput;
			TimeRange tr;
			tr.Frames = viscode.size();
			tr.Start = 0.0;
			tr.Rate = 1.0;
			for (imarker = 0; imarker < 9; imarker++)
			{
				tsinput.push_back( TSFactoryOccValue(3).New(tr, BinMemFactoryDefault()) );
				tstruth.push_back( TSFactoryOccValue(3).New(tr, BinMemFactoryDefault()) );
			}

			// generate data and also calculate ground-truth for angles for comparison
			Vector3 a(0.0);
			Vector3 t(0.0);
			std::auto_ptr<TimeSequence> ts_angle_truth( TSFactoryOccValue(3).New(tr, BinMemFactoryDefault()) );
			TSOccVector3Iter iter_ts_angle_truth(*ts_angle_truth);
			for (iframe = 0; iframe < (UInt32)tr.Frames; iframe++)
			{
				// choose and pop vis code
				size_t viscodechoice = (size_t)((((double)rand() / (double)RAND_MAX) - 1E-12) * viscode.size());
				std::list<NinePointCode>::iterator viscodeiter = viscode.begin();
				for (size_t ichoice = 0; ichoice < viscodechoice; ichoice++)
					viscodeiter++;
				NinePointCode n = *viscodeiter;
				viscode.erase(viscodeiter);

				// random walk in pose space
				t[0] += -1.0 + 2*RandomUnit();
				t[1] += -1.0 + 2*RandomUnit();
				t[2] += -1.0 + 2*RandomUnit();
				a[0] += -0.1 + 0.2*RandomUnit();
				a[1] += -0.1 + 0.2*RandomUnit();
				a[2] += -0.1 + 0.2*RandomUnit();

				// use matrix form of pose
				Matrix3x3 R;
				ExponentialMap(R, a);

				// generate simulated marker data
				for (imarker = 0; imarker < 9; imarker++)
				{
					// get pointer to this marker
					TSOccVector3Iter iter_input( *tsinput[imarker] );
					TSOccVector3Iter iter_truth( *tstruth[imarker] );
					iter_input.NextN(iframe);
					iter_truth.NextN(iframe);

					// prepare
					if (n.occluded[imarker])
					{
						// set occluded
						iter_input.Value()[0] = 0.0;
						iter_input.Value()[1] = 0.0;
						iter_input.Value()[2] = 0.0;				
						iter_input.Occluded() = 1;
						iter_truth.Value()[0] = 0.0;
						iter_truth.Value()[1] = 0.0;
						iter_truth.Value()[2] = 0.0;				
						iter_truth.Occluded() = 1;
					}
					else
					{
						// take untransformed shape
						Vector3 x = (nine_point_shape + 3*imarker);

						// transform
						Vector3 Tx(0.0);
						Matrix3x3::MulVec(Tx, R, x);
						Tx += t;

						// store ground truth without noise
						iter_truth.Value()[0] = Tx[0];
						iter_truth.Value()[1] = Tx[1];
						iter_truth.Value()[2] = Tx[2];				
						iter_truth.Occluded() = 0;

						// add noise
						Tx[0] +=  noise[0]*RandomGauss();
						Tx[1] +=  noise[1]*RandomGauss();
						Tx[2] +=  noise[2]*RandomGauss();

						// use as input
						iter_input.Value()[0] = Tx[0];
						iter_input.Value()[1] = Tx[1];
						iter_input.Value()[2] = Tx[2];				
						iter_input.Occluded() = 0;
					}
				}

				// store in angles for angle comparison
				ProjectionAngles(iter_ts_angle_truth.Value(), R);
				iter_ts_angle_truth.Next();
			}

			// put data into 
			// put input
			RigidBodyShapeCollection collection;
			collection.CopyFromTimeSequences((vector<const TimeSequence*>&)tsinput);

			// do the sums
			clock_t time0 = clock();
			std::vector<TimeSequence*> tsfit;
			UInt32 status = collection.MOSHFIT(tsfit, 200.0, 6.0, 0.001);
			clock_t time_taken = clock() - time0;
			double time_taken_seconds = (double)time_taken / (double)CLK_TCK;
			sum_clock += time_taken_seconds;
		
			// evaluate results
			CPPUNIT_ASSERT_EQUAL(RigidBodyResult::success, status);
			CPPUNIT_ASSERT_EQUAL(size_t(9), tsfit.size());
			double sum_sq_diff(0.0);
			size_t count_diff(0);
			for (imarker = 0; imarker < 9; imarker++)
			{
				const TimeSequence& ts_truth = *tstruth[imarker];
				const TimeSequence& ts_out = *tsfit[imarker];
				CPPUNIT_ASSERT_EQUAL(size_t(1260), ts_out.NumFrames());
				CPPUNIT_ASSERT_EQUAL(0.0, ts_out.Start.Value());
				CPPUNIT_ASSERT_EQUAL(200.0, ts_out.Rate.Value());
				TSOccVector3ConstIter iter_truth(ts_truth);
				TSOccVector3ConstIter iter_out(ts_out);
				for (iframe = 0; iframe < 1260; iframe++, iter_truth.Next(), iter_out.Next())
				{
					if (iter_truth.Occluded() == 0)
					{
						Vector3 d;
						Vector3::Sub(d, iter_truth.Value(), iter_out.Value());
						sum_sq_diff += d.Modulus2();
						// fprintf(stderr, "%u: %lf\n", iframe, d.Modulus2());
						count_diff++;
					}
				}
			}

			// compute difference
			double rms_diff = sqrt(sum_sq_diff / count_diff);

			// find range
			if (rms_diff < rms_min)
				rms_min = rms_diff;
			if (rms_diff > rms_max)
				rms_max = rms_diff;

			// look at angles computed per-frame and also compare with procrustes fit of ground-truth
			iter_ts_angle_truth.SeekTo(0);
			double sum_sq_angle_diff(0.0);
			double proc_sum_sq_diff(0.0);
			double proc_sum_sq_angle_diff(0.0);
			for (iframe = 0; iframe < 1260; iframe++)
			{
				// populate shape classes of simulated input and MOSHFIT output
				RigidBodyShape shape_in;
				RigidBodyShape shape_out;
				for (imarker = 0; imarker < 9; imarker++)
				{
					TSOccVector3ConstIter iter_in(*tsinput[imarker]);
					TSOccVector3ConstIter iter_out(*tsfit[imarker]);
					iter_in.NextN(iframe);
					iter_out.NextN(iframe);
					shape_in.AddMarker(iter_in.Value(), iter_in.Occluded() ? false : true);
					shape_out.AddMarker(iter_out.Value(), iter_out.Occluded() ? false : true);
				}

				// compute procrustes fit to ground truth
				RigidTransform3 proc_T;
				UInt32 proc_fit_result = shape_truth.ComputeFitTo(proc_T, shape_in);
				CPPUNIT_ASSERT_EQUAL(RigidBodyResult::success, proc_fit_result);

				// compute procrustes fit adjusted to match mean coord system
				RigidTransform3 T;
				UInt32 fit_result = shape_truth.ComputeFitTo(T, shape_out);
				CPPUNIT_ASSERT_EQUAL(RigidBodyResult::success, fit_result);

				// compute RMS position using procrustes fit to idealised calibration data
				for (imarker = 0; imarker < 9; imarker++)
				{
					// compare positions and compute RMS (count should match that of MOSHFIT comparisons)
					Vector3 m;
					RigidTransform3::MulVec(m, proc_T, shape_truth.Marker(imarker).position);
					if (shape_in.Marker(imarker).visible)
					{
						Vector3 dm;
						Vector3::Sub(dm, shape_in.Marker(imarker).position, m);
						proc_sum_sq_diff += dm.Modulus2();
						// fprintf(stderr, "%u: %lf\n", iframe, sqrt(dm.Modulus2()));
					}
				}

				// angle using procrustes fit
				Vector3 proc_angle(0.0);
				ProjectionAngles(proc_angle, proc_T.R);

				// deal with wrap-around
				for (UInt32 icomp = 0; icomp < 3; icomp++)
				{
					if (fabs(proc_angle[icomp] - iter_ts_angle_truth.Value()[icomp]) > 180.0)
					{
						if (proc_angle[icomp] < 0.0)
							proc_angle[icomp] += 360.0;
						else
							proc_angle[icomp] -= 360.0;
					}
				}

				// compute difference with ground truth
				Vector3 proc_angle_diff(0.0);
				Vector3::Sub(proc_angle_diff, proc_angle, iter_ts_angle_truth.Value());
				double proc_sq_angle_diff = proc_angle_diff.Modulus2();
				proc_sum_sq_angle_diff += proc_sq_angle_diff;

				// angles from MOSHFIT output
				Vector3 angle_out(0.0);
				ProjectionAngles(angle_out, T.R);

				// deal with wrap-around
				for (UInt32 icomp = 0; icomp < 3; icomp++)
				{
					if (fabs(angle_out[icomp] - iter_ts_angle_truth.Value()[icomp]) > 180.0)
					{
						if (angle_out[icomp] < 0.0)
							angle_out[icomp] += 360.0;
						else
							angle_out[icomp] -= 360.0;
					}
				}

				// compute difference with ground truth
				Vector3 angle_diff(0.0);
				Vector3::Sub(angle_diff, angle_out, iter_ts_angle_truth.Value());
				double sq_angle_diff = angle_diff.Modulus2();
				// fprintf(stderr, "Angle: %.02lf %.02lf %.02lf -> %.02lf %.02lf %.02lf\n",
				//		iter_ts_angle_truth.Value()[0], iter_ts_angle_truth.Value()[1], iter_ts_angle_truth.Value()[2],
				//		angle_out[0], angle_out[1], angle_out[2]);
				sum_sq_angle_diff += sq_angle_diff;

				iter_ts_angle_truth.Next();
			}

			// compute angle difference
			double angle_rms_diff = sqrt(sum_sq_angle_diff / (3*1260));
			double proc_angle_rms_diff = sqrt(proc_sum_sq_angle_diff / (3*1260));

			// find angle difference range
			if (angle_rms_diff < angle_rms_min)
				angle_rms_min = angle_rms_diff;
			if (angle_rms_diff > angle_rms_max)
				angle_rms_max = angle_rms_diff;

			// compute procrustes position difference
			double proc_rms_diff = sqrt(proc_sum_sq_diff / count_diff);

			// find procrustes position difference range
			if (proc_rms_diff < proc_rms_min)
				proc_rms_min = proc_rms_diff;
			if (proc_rms_diff > proc_rms_max)
				proc_rms_max = proc_rms_diff;

			// find procrustes angle difference range
			if (proc_angle_rms_diff < proc_angle_rms_min)
				proc_angle_rms_min = proc_angle_rms_diff;
			if (proc_angle_rms_diff > proc_angle_rms_max)
				proc_angle_rms_max = proc_angle_rms_diff;

			// done with markers for this experiment
			for (imarker = 0; imarker < 9; imarker++)
			{
				delete const_cast<TimeSequence*>( tstruth[imarker] );
				delete const_cast<TimeSequence*>( tsinput[imarker] );
				delete const_cast<TimeSequence*>( tsfit[imarker] );
			}
		}

		double mean_time = sum_clock / num_reps;
		fprintf(stderr, "RigidBodyShapeCollection::testMoshFit repetitions %u\n" \
										 "  mean processing time: %lf\n" \
			               "  RMS min: %lf\n" \
										 "  RMS max: %lf\n" \
										 "  angle RMS min: %lf\n" \
										 "  angle RMS max: %lf\n" \
			               "  procrustes RMS min: %lf\n" \
										 "  procrustes RMS max: %lf\n" \
										 "  procrustes angle RMS min: %lf\n" \
										 "  procrustes angle RMS max: %lf\n",
			num_reps, mean_time, 
			rms_min, rms_max, 
			angle_rms_min, angle_rms_max,
			proc_rms_min, proc_rms_max, 
			proc_angle_rms_min, proc_angle_rms_max);

		CPPUNIT_ASSERT(rms_max < 3.0);
		CPPUNIT_ASSERT(angle_rms_max < 2.0);
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION( TestRigidBodyShapeCollection );
