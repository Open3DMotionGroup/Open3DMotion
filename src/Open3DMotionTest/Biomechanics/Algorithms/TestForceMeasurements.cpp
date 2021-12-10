#include "Open3DMotion/Biomechanics/Algorithms/ForceMeasurements.h"
#include "Open3DMotion/Biomechanics/Trial/ForcePlate.h"
#include "Open3DMotion/Biomechanics/Trial/Trial.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinMemFactoryDefault.h"
#include <cppunit/extensions/HelperMacros.h>
#include <math.h>
#include <vector>

namespace Open3DMotion
{
	class TestForceMeasurements : public CppUnit::TestCase
	{
	public:
		CPPUNIT_TEST_SUITE(TestForceMeasurements);
		CPPUNIT_TEST(testAutoOffset);
		CPPUNIT_TEST(testAutoOffsetOneFrame);
		CPPUNIT_TEST_SUITE_END();

	public:

		void testAutoOffset()
		{
			ForcePlate plate;
			plate.Channels.Add(2);
			plate.Channels.Add(3);
			plate.Channels.Add(4);
			plate.Channels.Add(5);
			plate.Channels.Add(6);
			plate.Channels.Add(7);
			plate.Channels.Add(8);
			plate.Channels.Add(9);

			TimeRange range;
			range.Frames = 5000;
			range.Rate = 1000.0;
			range.Start = 0.0;

			// Simulated data
			std::vector< std::unique_ptr<TimeSequence> > analog(9);
			std::vector< const TimeSequence* > analog_input;
			for (size_t index = 0; index < 9; index++)
			{
				char channelname[32];
				_snprintf(channelname, 32, "Analog%02d", (int)(index + 1));

				std::unique_ptr<TimeSequence> ts(new TimeSequence);
				ts->Group = TrialSectionAcq::TSGroupAnalog;
				ts->Channel = channelname;
				ts->HardwareID = (index + 1);
				TSFactoryValue(1).Allocate(*ts, range, BinMemFactoryDefault());

				for (TSScalarIter iter(*ts); iter.HasFrame(); iter.Next())
				{

					// simulated value 100 * hardware ID
					iter.Value() = 100.0 * (index + 1);

					if (iter.FrameIndex() < 200)		// This is the number of frames expected in the offset period
					{
						// Alternate +ve and -ve during period when offset is taken
						if (iter.FrameIndex() % 2 == 0)
							iter.Value() += 2.5;
						else
							iter.Value() -= 2.5;
					}
					else
					{
						// Fixed offset of 77.7 compared with baseline
						iter.Value() += 77.7;
					}

				}

				// add to list (will get deleted)
				analog[index].reset(ts.release());

				// const pointer
				analog_input.push_back(analog[index].get());
			}

			// Create the test class
			ForceMeasurementTimeSequence force;
			force.Set(plate, analog_input);

			// Apply
			// The duration should be rounded up so 0.1999 seconds and 1000Hz
			// should be a 200 frame offset period
			bool result = force.AutoDriftOffset(0, 1, 0.1999);

			CPPUNIT_ASSERT_EQUAL(true, result);
			CPPUNIT_ASSERT_EQUAL(true, force.HasFrame());
			CPPUNIT_ASSERT_EQUAL(size_t(8), force.NumChannels());
			CPPUNIT_ASSERT_EQUAL(size_t(5000), force.NumFrames());
			CPPUNIT_ASSERT_DOUBLES_EQUAL(1000.0, force.Rate(), 1E-12);

			std::vector<double> rawanalog, calanalog;

			// raw analog should be as input
			force.CurrentRawAnalog(rawanalog);
			CPPUNIT_ASSERT_EQUAL(size_t(8), rawanalog.size());
			CPPUNIT_ASSERT_DOUBLES_EQUAL(202.5, rawanalog[0], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(302.5, rawanalog[1], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(402.5, rawanalog[2], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(502.5, rawanalog[3], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(602.5, rawanalog[4], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(702.5, rawanalog[5], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(802.5, rawanalog[6], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(902.5, rawanalog[7], 1E-12);

			// should see calibration offset
			force.ApplyLinearCalibration(calanalog, rawanalog);
			CPPUNIT_ASSERT_EQUAL(size_t(8), calanalog.size());
			CPPUNIT_ASSERT_DOUBLES_EQUAL(2.5, calanalog[0], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(2.5, calanalog[1], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(2.5, calanalog[2], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(2.5, calanalog[3], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(2.5, calanalog[4], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(2.5, calanalog[5], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(2.5, calanalog[6], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(2.5, calanalog[7], 1E-12);

			// advance to frame 500
			for (int index = 0; index < 500; index++)
			{
				force.NextFrame();
			}
			
			// raw analog should be as input
			force.CurrentRawAnalog(rawanalog);
			CPPUNIT_ASSERT_EQUAL(size_t(8), rawanalog.size());
			CPPUNIT_ASSERT_DOUBLES_EQUAL(277.7, rawanalog[0], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(377.7, rawanalog[1], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(477.7, rawanalog[2], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(577.7, rawanalog[3], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(677.7, rawanalog[4], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(777.7, rawanalog[5], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(877.7, rawanalog[6], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(977.7, rawanalog[7], 1E-12);

			// should see calibration offset
			force.ApplyLinearCalibration(calanalog, rawanalog);
			CPPUNIT_ASSERT_EQUAL(size_t(8), calanalog.size());
			CPPUNIT_ASSERT_DOUBLES_EQUAL(77.7, calanalog[0], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(77.7, calanalog[1], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(77.7, calanalog[2], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(77.7, calanalog[3], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(77.7, calanalog[4], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(77.7, calanalog[5], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(77.7, calanalog[6], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(77.7, calanalog[7], 1E-12);

		}


		// Applying auto-offset to a sequence that contains a single frame should return false but still allow
		// that frame to be processed
		void testAutoOffsetOneFrame()
		{
			ForcePlate plate;
			plate.Channels.Add(2);
			plate.Channels.Add(3);
			plate.Channels.Add(4);
			plate.Channels.Add(5);
			plate.Channels.Add(6);
			plate.Channels.Add(7);
			plate.Channels.Add(8);
			plate.Channels.Add(9);

			TimeRange range;
			range.Frames = 1;
			range.Rate = 1000.0;
			range.Start = 0.0;

			std::vector< std::unique_ptr<TimeSequence> > analog(9);
			std::vector< const TimeSequence* > analog_input;
			for (size_t index = 0; index < 9; index++)
			{
				char channelname[32];
				_snprintf(channelname, 32, "Analog%02d", (int)(index + 1));

				std::unique_ptr<TimeSequence> ts(new TimeSequence);
				ts->Group = TrialSectionAcq::TSGroupAnalog;
				ts->Channel = channelname;
				ts->HardwareID = (index + 1);
				TSFactoryValue(1).Allocate(*ts, range, BinMemFactoryDefault());
				
				// simulated value 100 * hardware ID
				TSScalarIter(*ts).Value() = 100.0 * (index + 1);
				
				// add to list (will get deleted)
				analog[index].reset(ts.release());

				// const pointer
				analog_input.push_back(analog[index].get());
			}

			ForceMeasurementTimeSequence force;
			force.Set(plate, analog_input);
			bool result = force.AutoDriftOffset();

			// should indicate that it didn't proceed (because data too short)
			CPPUNIT_ASSERT_EQUAL(false, result);

			// but should still have frame available for processing
			CPPUNIT_ASSERT_EQUAL(true, force.HasFrame());
			CPPUNIT_ASSERT_EQUAL(size_t(8), force.NumChannels());
			CPPUNIT_ASSERT_EQUAL(size_t(1), force.NumFrames());
			CPPUNIT_ASSERT_DOUBLES_EQUAL(1000.0, force.Rate(), 1E-12);
		
			// raw analog should be as nput
			std::vector<double> rawanalog;
			force.CurrentRawAnalog(rawanalog);
			CPPUNIT_ASSERT_EQUAL(size_t(8), rawanalog.size());
			CPPUNIT_ASSERT_DOUBLES_EQUAL(200.0, rawanalog[0], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(300.0, rawanalog[1], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(400.0, rawanalog[2], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(500.0, rawanalog[3], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(600.0, rawanalog[4], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(700.0, rawanalog[5], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(800.0, rawanalog[6], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(900.0, rawanalog[7], 1E-12);
		
			// but calibrated should read the same
			std::vector<double> calanalog;
			force.ApplyLinearCalibration(calanalog, rawanalog);
			CPPUNIT_ASSERT_EQUAL(size_t(8), calanalog.size());
			CPPUNIT_ASSERT_DOUBLES_EQUAL(200.0, calanalog[0], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(300.0, calanalog[1], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(400.0, calanalog[2], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(500.0, calanalog[3], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(600.0, calanalog[4], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(700.0, calanalog[5], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(800.0, calanalog[6], 1E-12);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(900.0, calanalog[7], 1E-12);
		}

	};

	CPPUNIT_TEST_SUITE_REGISTRATION(TestForceMeasurements);
}

