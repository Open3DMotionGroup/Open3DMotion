/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "testWalk4DescriptorsInFile.h"

#include "Open3DMotion/Biomechanics/Trial/TSFactory.h"
#include <cppunit/extensions/HelperMacros.h>
#include "Open3DMotionTest/Data/MDF/ADemo1.h"
#include "Open3DMotionTest/Data/MDF/Walk4.h"

void testWalk4DescriptorsInFile(Open3DMotion::MotionFileHandler& handler, const char* filename, bool enforcestartoffset)
{
	// load file
	std::unique_ptr<Open3DMotion::TreeValue> trialcontents;
	try
	{
		trialcontents = std::unique_ptr<Open3DMotion::TreeValue>( handler.Read(filename) );
	}
	catch(Open3DMotion::MotionFileException& error)
	{
		CPPUNIT_FAIL(error.message);
	}

	// build trial object
	std::unique_ptr<Open3DMotion::Trial> trial( new Open3DMotion::Trial );
	trial->FromTree(trialcontents.get());
	
	// retrieve marker sequences
	std::vector<const Open3DMotion::TimeSequence*> marker;
	std::vector<const Open3DMotion::TimeSequence*> analog;
	trial->Acq.GetTSGroup(marker, Open3DMotion::TrialSectionAcq::TSGroupMarker);
	trial->Acq.GetTSGroup(analog, Open3DMotion::TrialSectionAcq::TSGroupAnalog);

	// check expected counts
	CPPUNIT_ASSERT_EQUAL(TestData::Walk4_nummarkers, marker.size());
	CPPUNIT_ASSERT_EQUAL(TestData::Walk4_numforceplates*8, analog.size());

	// check correct marker names
	for (size_t imarker = 0; imarker < TestData::Walk4_nummarkers; imarker++)
	{
		// ID number of mpx30 unit (or 0 for combined result)
		size_t unitnumber = imarker / 56;

		// marker id (one-based) is (channel mod 56) + 1
		size_t markerid = (imarker % 56) + 1;

		// names follow this pattern:
		char name[256];
		sprintf(name, "Marker %d", 100*(int)unitnumber + (int)markerid);

		// check matches expected name
		const std::string channel = marker[imarker]->Channel;
		CPPUNIT_ASSERT_EQUAL(std::string(name), channel);

		// check start time (depends on marker id not channel number)
		if (enforcestartoffset)
		{
			// check marker hardware id
			CPPUNIT_ASSERT_EQUAL((Open3DMotion::Int32)markerid, marker[imarker]->HardwareID.Value());

			double expectedstart = 172E-6 * (markerid-1);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedstart, marker[imarker]->Start.Value(), 1E-12);
		}

		// check number of frames & rate
		CPPUNIT_ASSERT_EQUAL(TestData::Walk4_numframes, (size_t)marker[imarker]->NumFrames());
		CPPUNIT_ASSERT_DOUBLES_EQUAL(TestData::Walk4_rateHz, marker[imarker]->Rate.Value(), 1E-12);		
	}

	// find number of recognised force plates
	size_t num_plates = trial->Acq.ForcePlates.NumElements();

	// ensure expected number of forceplates
	CPPUNIT_ASSERT_EQUAL(TestData::Walk4_numforceplates, num_plates);

	// check details
	for (size_t iplate = 0; iplate < TestData::Walk4_numforceplates; iplate++)
	{
		// get details of plate
		const Open3DMotion::ForcePlate& fp = trial->Acq.ForcePlates[iplate];

		// Kistler should have 8 channels per plate
		CPPUNIT_ASSERT_EQUAL(size_t(8), fp.Channels.NumElements());

		// check channels match expected (sequential) numbers
		for (size_t ichannel = 0; ichannel < 8; ichannel++)
		{
			// channel hardware ID's should simply be sequential (starting at 1)
			size_t hardwareID = fp.Channels[ichannel];
			CPPUNIT_ASSERT_EQUAL(iplate*8 + ichannel + 1, hardwareID);

			// convert to analog index
			size_t ianalog;
			for (ianalog = 0; ianalog < analog.size(); ianalog++)
			{
				if (analog[ianalog]->HardwareID == (Open3DMotion::Int32)hardwareID)
					break;
			}

			CPPUNIT_ASSERT( ianalog < analog.size() );

			// should now be sequential but starting at zero
			CPPUNIT_ASSERT_EQUAL(iplate*8 + ichannel, ianalog);

			// get channel data
			const Open3DMotion::TimeSequence* ts = analog[ianalog];

			// check time range for analog
			CPPUNIT_ASSERT_EQUAL(TestData::Walk4_numframes, (size_t)ts->NumFrames());
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, ts->Start, 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(TestData::Walk4_rateHz, ts->Rate, 1E-12);		
		}
	}
}
