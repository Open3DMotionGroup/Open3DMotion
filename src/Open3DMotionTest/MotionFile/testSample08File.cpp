/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "testSample08File.h"

#include "Open3DMotion/Biomechanics/Trial/Trial.h"
#include "Open3DMotion/Biomechanics/Trial/TSFactory.h"
#include "Open3DMotionTest/Data/C3D/sample08/EB01PI.h"
#include <cppunit/extensions/HelperMacros.h>
#include <math.h>

using namespace std;
using namespace Open3DMotion;

void testSample08File(MotionFileHandler& handler, const char* filename, double markertol, const double* analogtol, bool allow_analog_reorder, bool allow_analog_removal, bool exclude_force)
{
	// message
	const char* msg = filename;

	// read
	std::auto_ptr<TreeValue> trialcontents;
	try
	{
		trialcontents = std::auto_ptr<TreeValue>( handler.Read(filename) );
	}
	catch(MotionFileException& e)
	{
		CPPUNIT_FAIL(e.message);
	}

	// build trial object
	std::auto_ptr<Trial> trial(new Trial);
	trial->FromTree(trialcontents.get());

	// retrieve marker sequences
	std::vector<const TimeSequence*> marker;
	trial->Acq.GetTSGroup(marker, TrialSectionAcq::TSGroupMarker);

	int i;
	char detail[1024];

	// check expected marker count
	CPPUNIT_ASSERT_EQUAL_MESSAGE(msg, TestData::EB01PI_nummarkers, (int)marker.size());

	// check correct marker names
	for (i = 0; i < TestData::EB01PI_nummarkers; i++)
	{
		CPPUNIT_ASSERT_EQUAL_MESSAGE(msg, std::string(TestData::EB01PI_markername[i]), marker[i]->Channel.Value());
	}

	// check marker data
	for (i = 0; i < TestData::EB01PI_nummarkers; i++)
	{
		// must have expected length
		CPPUNIT_ASSERT_EQUAL_MESSAGE(msg, (size_t)TestData::EB01PI_numframes, marker[i]->NumFrames());
		TSOccVector3ConstIter iter_ts(*marker[i]);
		for (int j = 0; j < TestData::EB01PI_numframes; j++, iter_ts.Next())
		{
			const double* expected = TestData::EB01PI + j*TestData::EB01PI_valuesperframe + 4*i;
			if (iter_ts.Occluded() != (expected[3] < 0.0))
			{
				sprintf(detail, "%s: [marker=%s][frame=%d][dimension=occlusion]\n"
					              "Expected occlusion %s Actual occlusion %s",
					msg, TestData::EB01PI_markername[i], j,
					(expected[3] < 0.0) ? "true" : "false", 
					 iter_ts.Occluded() ? "true" : "false");
				CPPUNIT_FAIL(detail);
			}

			for (int k = 0; k < 3; k++)
			{
				if (fabs(iter_ts.Value()[k]-expected[k]) > markertol)
				{
					sprintf(detail, "%s: [marker=%s][frame=%d][dimension=%d]\n"
						              "Expected %lf Actual %lf",
						msg, TestData::EB01PI_markername[i], j, k, expected[k], iter_ts.Value()[k]);
					CPPUNIT_FAIL(detail);
				}
			}
		}
	}

	// read calibrated analog info
	std::vector<const TimeSequence*> calanalog;
	trial->Acq.GetTSGroup(calanalog, TrialSectionAcq::TSGroupAnalog);

	// for TXT files all analog channels are gone, in which case test may stop here
	if (allow_analog_removal && (calanalog.size() == 0))
		return;

	// check analog count
	if (allow_analog_reorder)
	{
		// check that there are at least enough analog channels
		// - MDF files insert some extra ones as MDF's require 8 channels per forceplate,
		//   even for AMTI plates
		CPPUNIT_ASSERT_MESSAGE(msg, (int)calanalog.size() >= TestData::EB01PI_numanalog);
	}
	else
	{
		CPPUNIT_ASSERT_EQUAL_MESSAGE(msg, TestData::EB01PI_numanalog, (int)calanalog.size());
	}

	// check correct analog names
	for (i = 0; i < TestData::EB01PI_numanalog; i++)
	{
		if (allow_analog_reorder)
		{
			// make sure channel exists but don't require it to be the next one in sequence 
			// this is for MDF's which reorganise channels into Force & EMG sections
			const TimeSequence* tscheck = trial->Acq.GetTS(TrialSectionAcq::TSGroupAnalog, TestData::EB01PI_analogname[i]);
			CPPUNIT_ASSERT(tscheck != NULL);
		}
		else
		{
			// expected analog channel must be next one in sequence and hardware ID must be = sequence index + 1
			CPPUNIT_ASSERT_EQUAL_MESSAGE(msg, std::string(TestData::EB01PI_analogname[i]), calanalog[i]->Channel.Value());
			CPPUNIT_ASSERT_EQUAL_MESSAGE(msg, (Int32)(i + 1), calanalog[i]->HardwareID.Value());
		}
	}

	// check analog data
	for (i = 0; i < TestData::EB01PI_numanalog; i++)
	{
		// option to exclude force channels
		// - this is because MDF's fail tests on force analog channels due to their need
		//   to rotate the plate to the MDF canonical coord system
		if (exclude_force && TestData::EB01PI_isforce[i])
			continue;

		// get time sequence by name
		const TimeSequence* ts(0);
		for (size_t ifile = 0; ifile < calanalog.size(); ifile++)
		{
			if (calanalog[ifile]->Channel.Value().compare(TestData::EB01PI_analogname[i]) == 0)
			{
				ts = calanalog[ifile];
				break;
			}
		}

		// must exist
		CPPUNIT_ASSERT(ts != NULL);

		// must have expected length
		size_t analogframes = TestData::EB01PI_numframes*TestData::EB01PI_analogpermarker;
		CPPUNIT_ASSERT_EQUAL_MESSAGE(msg, analogframes, ts->NumFrames());

		// check each frame
		TSScalarConstIter iter_ts = TSScalarConstIter(*ts);
		for (size_t j = 0; j < analogframes; j++, iter_ts.Next())
		{
			const double& rawactual = iter_ts.Value();
			
			// analog tolerance for this channel
			double channeltol = (analogtol != NULL) ? analogtol[i] : 0.001;

			// compute calibrated actual
			double actual = (rawactual - ts->Offset) * ts->Scale;

			// expected value from C array
			Int32 valuerow = j / TestData::EB01PI_analogpermarker;
			Int32 columnoffset = (4*TestData::EB01PI_nummarkers) + i*TestData::EB01PI_analogpermarker + (j % TestData::EB01PI_analogpermarker);
			double expected = TestData::EB01PI[valuerow*TestData::EB01PI_valuesperframe + columnoffset];

			// force values in Visual3D test data are negated as if they were in the C3D
			// whereas our tree follows the opposite convention
			if (TestData::EB01PI_isforce[i])
				expected *= -1.0;

			// compare values
			// TODO: MDFs currently fail here because they reorient the plate to the
			//       MDF canonical force coord system.  Need to formulate the test in
			//       terms of calculated force result which should be invariant to this change.
			if (fabs(actual-expected) > channeltol)
			{
				sprintf(detail, "%s: [analog=%s][analog sample=%u]\n"
					              "Expected %lf Actual %lf",
					msg, TestData::EB01PI_analogname[i], static_cast<unsigned int>(j),
					expected, actual);
				CPPUNIT_FAIL(detail);
			}
		}
	}
}


