/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

/* TestCODAText.cpp
   
	 Summary
	 Unit tests for Codamotion TXT (CODAText) file format

 */

#include "Open3DMotion/MotionFile/MotionFileHandler.h"
#include "Open3DMotion/MotionFile/MotionFileFormat.h"
#include "Open3DMotion/Biomechanics/Trial/TSFactory.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinMemFactoryDefault.h"
#include "Open3DMotionTest/Data/CODAText/CartWheel.h"
#include "Open3DMotion/MotionFile/Formats/CODAText/FileFormatOptionsCODAText.h"
#include <cppunit/extensions/HelperMacros.h>
#include <math.h>

/* Summary
   Unit test fixture for Codamotion TXT (CODAText) files.
   
   Description
   Currently tests loading of CartWheel.txt against
	 ground-truth and verifies that resave-load also works.
                                                                 */
class TestCODAText : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE( TestCODAText );
	CPPUNIT_TEST( testCartWheel );
	CPPUNIT_TEST( testReWriteCartWheel );
	CPPUNIT_TEST_SUITE_END();

public:
	TestCODAText() :
			handler("TestCODAText", "UNVERSIONED")
	{
	}

public:
	/*
		Summary
		Use Motion File Library to load CartWheel.txt and compare with ground truth in TestData::CartWheel
		(Calls testCartWheelFile("Data/TXT/CartWheel.txt"))

		See Also
		testReWriteCartWheel, testCartWheel
		*/
	void testCartWheel()
	{ testCartWheelFile("Open3DMotionTest/Data/CODAText/CartWheel.txt"); }

	/*
		Summary
		Use Motion File Library to load CartWheel.txt, then uses Motion File library to resave it.
		Uses testReWriteCartWheel to compare results of the rewrite with ground truth in TestData::CartWheel

		See Also
		testReWriteCartWheel, testCartWheel
		*/
	void testReWriteCartWheel();

protected:
	/*
		Summary
		Use Motion File Library to load the specified save of CartWheel.txt and compare with ground truth in TestData::CartWheel
		*/
	void testCartWheelFile(const char* filename);

protected:
	Open3DMotion::MotionFileHandler handler;
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestCODAText );

void TestCODAText::testCartWheelFile(const char* filename)
{
	Open3DMotion::Trial trial;

	try
	{
		// read
		std::unique_ptr<Open3DMotion::TreeValue> tree ( handler.Read(filename) );

		// parse as trial object
		trial.FromTree(tree.get());
	}
	catch(const Open3DMotion::MotionFileException& error)
	{
		CPPUNIT_FAIL(error.message);
	}

	// check comment
	CPPUNIT_ASSERT_EQUAL(std::string(TestData::CartWheel_comment), trial.UserInput.Comment.Value());

	// retrieve sequences (analog & marker)
	std::vector<const Open3DMotion::TimeSequence*> marker;
	trial.Acq.GetTSGroup(marker, Open3DMotion::TrialSectionAcq::TSGroupMarker);

	// check expected marker count
	CPPUNIT_ASSERT_EQUAL(TestData::CartWheel_nummarkers, (int)marker.size());

	int i;
	char detail[1024];

	// check correct marker names and ID's
	for (i = 0; i < TestData::CartWheel_nummarkers; i++)
	{
		const std::string& channel = marker[i]->Channel;
		Open3DMotion::Int32 hwid = marker[i]->HardwareID;
		CPPUNIT_ASSERT_EQUAL(std::string(TestData::CartWheel_markername[i]), channel);
		CPPUNIT_ASSERT_EQUAL(TestData::CartWheel_markerid[i], (int)hwid);
	}

	// check marker data
	for (i = 0; i < TestData::CartWheel_nummarkers; i++)
	{
		// must have expected length
		CPPUNIT_ASSERT_EQUAL(TestData::CartWheel_numframes, (int)marker[i]->NumFrames());

		// must have expected start time
		CPPUNIT_ASSERT_EQUAL(TestData::CartWheel_starttime, marker[i]->Start.Value());

		// check array data itself
		Open3DMotion::TSOccVector3ConstIter iter_ts(*marker[i]);
		for (int j = 0; j < TestData::CartWheel_numframes; j++, iter_ts.Next())
		{
			// frame read by file library
			const double* actual_pos = iter_ts.Value();
			bool actual_occ = iter_ts.Occluded() ? true : false;

			// expected frame
			const double* expected = TestData::CartWheel + 4*(j*TestData::CartWheel_nummarkers+i);

			// check occlusion (zero means occluded in TXT file)
			if (actual_occ != (expected[3] < 1E-12))
			{
				sprintf(detail, "testCartWheel: [marker=%s][frame=%d][dimension=occlusion]\n"
					              "Expected occlusion %s Actual occlusion %s",
					TestData::CartWheel_markername[i], j,
					(expected[3] < 1E-12) ? "true" : "false", 
					 actual_occ ? "true" : "false");
				CPPUNIT_FAIL(detail);
			}

			// check 3D coords
			for (int k = 0; k < 3; k++)
			{
				if (fabs(actual_pos[k]-expected[k]) > 1E-12)
				{
					sprintf(detail, "testCartWheel: [marker=%s][frame=%d][dimension=%d]\n"
						              "Expected %lf Actual %lf",
						TestData::CartWheel_markername[i], j, k, expected[k], actual_pos[k]);
					CPPUNIT_FAIL(detail);
				}
			}
		}
	}
}

void TestCODAText::testReWriteCartWheel()
{
	// load & rewrite file
	try
	{
		// read in
		std::unique_ptr<Open3DMotion::TreeValue> tree( handler.Read("Open3DMotionTest/Data/CODAText/CartWheel.txt") );

		// write as text
		std::unique_ptr<Open3DMotion::TreeValue> options( Open3DMotion::FileFormatOptionsCODAText().ToTree() );
		handler.Write("Open3DMotionTest/Data/Temp/CartWheel_rewrite_CODAtext.txt", tree.get(), options.get());
	}
	catch(Open3DMotion::MotionFileException& error)
	{
		CPPUNIT_FAIL(error.message);
	}

	// test the re-written data
	testCartWheelFile("Open3DMotionTest/Data/Temp/CartWheel_rewrite_CODAtext.txt");
}


