/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "testADemo1File.h"

#include "Open3DMotion/Biomechanics/Trial/TSFactory.h"
#include "Open3DMotion/Biomechanics/Trial/Gait/GaitEvents.h"
#include <cppunit/extensions/HelperMacros.h>
#include "Open3DMotion/MotionFile/Formats/C3D/FileFormatC3D.h"
#include <cppunit/extensions/HelperMacros.h>
#include "Open3DMotionTest/Data/MDF/ADemo1.h"
#include "Open3DMotionTest/Data/MDF/Walk4.h"

#include <math.h>

void testADemo1File(Open3DMotion::MotionFileHandler& handler, const char* filename, bool strict, Open3DMotion::UInt32 forcedecimation, bool checkmarkerid, bool checkgaitcycle, bool checksubjectdata)
{
	// tolerances
	double markertol = strict ? 1E-4 : 0.001;
	double EMGtol = strict ? 1E-4 : 0.05;
	double forcetol = strict ? 1E-4 : 0.05;

	// load file
	std::unique_ptr<Open3DMotion::TreeValue> trialcontents;
	try
	{
		trialcontents = std::unique_ptr<Open3DMotion::TreeValue>( handler.Read(filename) );
	}
	catch(const Open3DMotion::MotionFileException& error)
	{
		CPPUNIT_FAIL(error.message);
	}

	// build trial
	std::unique_ptr<Open3DMotion::Trial> trial( new Open3DMotion::Trial );
	trial->FromTree(trialcontents.get());

	// retrieve sequences (analog & marker)
	std::vector<const Open3DMotion::TimeSequence*> marker;
	std::vector<const Open3DMotion::TimeSequence*> analog;
	trial->Acq.GetTSGroup(marker, Open3DMotion::TrialSectionAcq::TSGroupMarker);
	trial->Acq.GetTSGroup(analog, Open3DMotion::TrialSectionAcq::TSGroupAnalog);

	size_t i;
	char detail[1024];

	// check expected marker count
	CPPUNIT_ASSERT_EQUAL(TestData::ADemo1_nummarkers, marker.size());

	// check correct marker names and hardware id's
	for (i = 0; i < TestData::ADemo1_nummarkers; i++)
	{
		// check name
		const std::string& channel = marker[i]->Channel;
		CPPUNIT_ASSERT_EQUAL(std::string(TestData::ADemo1_markername[i]), channel);

		// for ADemo1 data set, hardware ID should be 1-24
		if (checkmarkerid)
			CPPUNIT_ASSERT_EQUAL((Open3DMotion::Int32)(i + 1), marker[i]->HardwareID.Value());
	}

	// check marker data
	for (i = 0; i < TestData::ADemo1_nummarkers; i++)
	{
		try
		{
			// get sequence & iterator (may throw exception if missing fields)
			const Open3DMotion::TimeSequence* ts = marker[i];
			Open3DMotion::TSOccVector3ConstIter iter_ts(*ts);

			// must have expected length
			CPPUNIT_ASSERT_EQUAL(TestData::ADemo1_numframes_marker, ts->NumFrames());
		
			// compare data
			for (size_t j = 0; j < TestData::ADemo1_numframes_marker; j++, iter_ts.Next())
			{
				const double* expected = TestData::ADemo1_marker + j*TestData::ADemo1_nummarkers*4 + i*4;
				if (iter_ts.Occluded() != (expected[3] < 1E-12))
				{
					sprintf(detail, "ADemo1: [marker=%s][frame=%d][dimension=occlusion]\n"
													"Expected occlusion %s Actual occlusion %s",
						TestData::ADemo1_markername[i], (int)j,
						(expected[3] < 0.0) ? "true" : "false", 
							iter_ts.Occluded() ? "true" : "false");
					CPPUNIT_FAIL(detail);
				}

				for (int k = 0; k < 3; k++)
				{
					if (fabs(iter_ts.Value()[k]-expected[k]) > markertol)
					{
						sprintf(detail, "ADemo1: [marker=%s][frame=%d][dimension=%d]\n"
														"Expected %.12lf Actual %.12lf",
							TestData::ADemo1_markername[i], (int)j, k, expected[k], iter_ts.Value()[k]);
						CPPUNIT_FAIL(detail);
					}
				}
			}
		}
		catch (const Open3DMotion::NoSuchFieldException& /*field_exception*/)
		{
			CPPUNIT_FAIL("marker data missing field");
		}
	}


	// check that there are at least enough analog channels for EMG
	CPPUNIT_ASSERT(analog.size() >= TestData::ADemo1_numEMG);

	// check analog data
	for (i = 0; i < TestData::ADemo1_numEMG; i++)
	{
		try
		{
			// get sequence (EMG sequences should be first in list)
			const Open3DMotion::TimeSequence* ts = analog[i];
			Open3DMotion::TSScalarConstIter iter_ts(*ts);

			// must have at least enough frames for EMG
			// - C3D files will round up to be whole multiple of marker frames
			CPPUNIT_ASSERT(TestData::ADemo1_numframes_EMG <= (size_t)ts->NumFrames());

			// check each frame
			for (size_t j = 0; j < TestData::ADemo1_numframes_EMG; j++, iter_ts.Next())
			{
				const double& rawactual = iter_ts.Value();
			
				// convert to cal value
				double actual = ts->Scale * (rawactual - ts->Offset);

				// expected value in millivolts
				double expected = TestData::ADemo1_EMG[j*TestData::ADemo1_numEMG + i];

				// compare values
				if (fabs(actual-expected) > EMGtol)
				{
					sprintf(detail, "ADemo1: [analog=%s][analog sample=%u]\n"
													"Expected %.12lf Actual %.12lf",
						TestData::ADemo1_EMGname[i], (int)j,
						expected, actual);
					CPPUNIT_FAIL(detail);
				}
			}
		}
		catch (const Open3DMotion::NoSuchFieldException& /*field_exception*/)
		{
			CPPUNIT_FAIL("EMG data missing field");
		}
	}

	// get details of all force plates
	CPPUNIT_ASSERT_EQUAL(TestData::ADemo1_numforceplates, trial->Acq.ForcePlates.NumElements());

	for (size_t iplate = 0; iplate < TestData::ADemo1_numforceplates; iplate++)
	{
		// Get plate object
		const Open3DMotion::ForcePlate& fp = trial->Acq.ForcePlates[iplate];

		// MDF should have 8 channels per plate always
		CPPUNIT_ASSERT_EQUAL(size_t(8), fp.Channels.NumElements());

		// Should be Kistler type
		CPPUNIT_ASSERT_EQUAL(std::string("Kistler"), fp.Type.Value());

		// Either Generic or equivalent 9821B type
		CPPUNIT_ASSERT((std::string("9821B") == fp.Model.Value()) || (std::string("Generic") == fp.Model.Value()));

		// get channels
		for (size_t ichannel = 0; ichannel < 8; ichannel++)
		{
			Open3DMotion::UInt32 filechannel_onebased = (Open3DMotion::UInt32)fp.Channels[ichannel];
			CPPUNIT_ASSERT(filechannel_onebased > 0);
			CPPUNIT_ASSERT(filechannel_onebased <= analog.size());
			Open3DMotion::UInt32 filechannel_zerobased = filechannel_onebased - 1;
			const Open3DMotion::TimeSequence* ts = analog[filechannel_zerobased];

			try
			{
				Open3DMotion::TSScalarConstIter iter_ts(*ts);
				for (size_t iframe = 0; iframe < TestData::ADemo1_numframes_force; iframe++, iter_ts.NextN(forcedecimation))
				{
					double expected = TestData::ADemo1_force[8*iframe+ichannel];
					const double& rawactual = iter_ts.Value();
					double actual = ts->Scale * (rawactual - ts->Offset);
					if (fabs(actual - expected) > forcetol)
					{
						sprintf(detail, "ADemo1: [forceplate=%d][plate channel=%d][analog channel=%d (zero based)][frame=%u]\n"
														"Expected %.12lf Actual %.12lf",
							(int)iplate, (int)ichannel, filechannel_zerobased, (int)iframe,
							expected, actual);
						CPPUNIT_FAIL(detail);
					}
				}
			}
			catch (const Open3DMotion::NoSuchFieldException& /*field_exception*/)
			{
				CPPUNIT_FAIL("force data missing field");
			}
		}

		if (checkgaitcycle)
		{
			const Open3DMotion::EventGroup* eg = trial->UserInput.GetEventGroup(Open3DMotion::GaitEvents::GroupName);
			CPPUNIT_ASSERT(eg != NULL);
			Open3DMotion::EventArray ea;
			eg->GetEvents(ea);
			CPPUNIT_ASSERT_EQUAL(size_t(6), ea.NumEvents());
			CPPUNIT_ASSERT_EQUAL(std::string(Open3DMotion::GaitEvents::LStart), ea.EventName(0));
			CPPUNIT_ASSERT_EQUAL(std::string(Open3DMotion::GaitEvents::RStart), ea.EventName(1));
			CPPUNIT_ASSERT_EQUAL(std::string(Open3DMotion::GaitEvents::LToeOff), ea.EventName(2));
			CPPUNIT_ASSERT_EQUAL(std::string(Open3DMotion::GaitEvents::LEnd), ea.EventName(3));
			CPPUNIT_ASSERT_EQUAL(std::string(Open3DMotion::GaitEvents::RToeOff), ea.EventName(4));
			CPPUNIT_ASSERT_EQUAL(std::string(Open3DMotion::GaitEvents::REnd), ea.EventName(5));
			CPPUNIT_ASSERT_DOUBLES_EQUAL(TestData::ADemo1_gaitcycle_left [0], ea.EventTime(0), 0.0001);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(TestData::ADemo1_gaitcycle_right[0], ea.EventTime(1), 0.0001);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(TestData::ADemo1_gaitcycle_left [1], ea.EventTime(2), 0.0001);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(TestData::ADemo1_gaitcycle_left [2], ea.EventTime(3), 0.0001);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(TestData::ADemo1_gaitcycle_right[1], ea.EventTime(4), 0.0001);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(TestData::ADemo1_gaitcycle_right[2], ea.EventTime(5), 0.0001);
		}

		if (checksubjectdata)
		{
			/* Patient data file says
				[Subject]
				ID=hao011
				Classification=normal
				[Patient Data]
				Sex=F
				DateOfBirth=7510
				Age=21
				Height=1.549
				Weight= 57.0
				[Joint Data]
				Widths=240,120,100,101,63,63
				PelvOffset=0,0,0
				HipOffset=0.19,0.36,0.30
				[Segment Data]
				Lengths=400,300,200,400,300,200
				Masses=0.1113,0.0428,0.0143,0.1113,0.0428,0.0143
				Centres=0.4600,0.4400,0.4200,0.4600,0.4400,0.4200
				RadGyrations=0.2910,0.2930,0.2440,0.2910,0.2930,0.2440
				RadGyrRatios=0.1000,0.0600,0.0600,0.1000,0.0600,0.0600
			*/

			CPPUNIT_ASSERT(trial->UserInput.Subject.IsSet());
			const Open3DMotion::TrialSubject& subject = trial->UserInput.Subject;
			CPPUNIT_ASSERT_EQUAL(std::string("hao011"), subject.ID.Value());
			CPPUNIT_ASSERT_EQUAL(std::string("normal"), subject.Classification.Value());
			CPPUNIT_ASSERT_EQUAL(std::string("female"), subject.Gender.Value());
			CPPUNIT_ASSERT_EQUAL(Open3DMotion::Int32(1975), subject.DateOfBirth.Year.Value());
			CPPUNIT_ASSERT_EQUAL(Open3DMotion::Int32(10), subject.DateOfBirth.Month.Value());
			CPPUNIT_ASSERT_EQUAL(Open3DMotion::Int32(21), subject.Age.Value());
			CPPUNIT_ASSERT_DOUBLES_EQUAL(1549.0, subject.Height.Value(), 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(57.0, subject.Weight.Value(), 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(240.0, subject.PelvicWidth.Value(), 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0, subject.PelvicDepth.Value(), 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(100.0, subject.LKneeWidth, 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(101.0, subject.RKneeWidth, 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(63.0, subject.LAnkleWidth, 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(63.0, subject.RAnkleWidth, 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(400.0, subject.LThighLength, 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(300.0, subject.LShankLength, 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(200.0, subject.LFootLength, 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(400.0, subject.RThighLength, 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(300.0, subject.RShankLength, 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(200.0, subject.RFootLength, 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1113*57.0, subject.LThighMass, 1E-6);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0428*57.0, subject.LShankMass, 1E-6);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0143*57.0, subject.LFootMass, 1E-6);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1113*57.0, subject.RThighMass, 1E-6);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0428*57.0, subject.RShankMass, 1E-6);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0143*57.0, subject.RFootMass, 1E-6);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2910*400.0, subject.RadGyr_LThigh_X.Value(), 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2910*400.0, subject.RadGyr_LThigh_Y.Value(), 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1000*0.2910*400.0, subject.RadGyr_LThigh_Z.Value(), 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2930*300.0, subject.RadGyr_LShank_X.Value(), 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2930*300.0, subject.RadGyr_LShank_Y.Value(), 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0600*0.2930*300.0, subject.RadGyr_LShank_Z.Value(), 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2440*200.0, subject.RadGyr_LFoot_X.Value(), 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2440*200.0, subject.RadGyr_LFoot_Y.Value(), 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0600*0.2440*200.0, subject.RadGyr_LFoot_Z.Value(), 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2910*400.0, subject.RadGyr_RThigh_X.Value(), 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2910*400.0, subject.RadGyr_RThigh_Y.Value(), 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1000*0.2910*400.0, subject.RadGyr_RThigh_Z.Value(), 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2930*300.0, subject.RadGyr_RShank_X.Value(), 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2930*300.0, subject.RadGyr_RShank_Y.Value(), 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0600*0.2930*300.0, subject.RadGyr_RShank_Z.Value(), 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2440*200.0, subject.RadGyr_RFoot_X.Value(), 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2440*200.0, subject.RadGyr_RFoot_Y.Value(), 1E-4);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0600*0.2440*200.0, subject.RadGyr_RFoot_Z.Value(), 1E-4);
		}
	}
}
