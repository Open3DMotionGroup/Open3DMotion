#include <cppunit/extensions/HelperMacros.h>

#include "Open3DMotion/MotionFile/MotionFileHandler.h"
#include "Open3DMotion/MotionFile/Formats/MoXie/FileFormatMoXie.h"
#include "Open3DMotion/MotionFile/MotionFileFormat.h"
#include "Open3DMotion/Biomechanics/Trial/TSFactory.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinMemFactoryDefault.h"
#include "Open3DMotion/MotionFile/Formats/MoXie/FileFormatOptionsMoXie.h"
#include "Open3DMotionTest/Data/MoXie/nwalk02.h"
#include "Open3DMotion/Maths/Vector3.h"
#include "Open3DMotion/Biomechanics/Algorithms/ForceMeasurements.h"
#include "Open3DMotion/Biomechanics/Algorithms/ForceCalculatorFactory.h"
#include "Open3DMotion/Biomechanics/Algorithms/ForceCalculator.h"

using namespace std;
using namespace Open3DMotion;

const double moxie_force_to_tree[6] =
{
	1.0,
	1.0,
	-1.0,
-1000.0,
-1000.0,
-1000.0
};


class TestMoXie : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE( TestMoXie );
	CPPUNIT_TEST( testLoad );
	CPPUNIT_TEST( testExtractFileName );
	CPPUNIT_TEST( testReWriteMoXie );
	CPPUNIT_TEST( testForceConversion );
	CPPUNIT_TEST( testTrialAttributes );
	CPPUNIT_TEST_SUITE_END();

public:
	TestMoXie() :
			handler("TestMoXie", "UNVERSIONED")
	{
	}

	void testLoad();

	void testExtractFileName();

	void testReWriteMoXie();

	void testNWalk02File(const char* filename);

	void testForceConversion();

	void testTrialAttributes();

	static void ComputeForce(TimeSequence*& force, TimeSequence*& point, const Trial& trial);

	static void CompareTS(const TimeSequence& expected, const TimeSequence& actual, const char* message, double precision,  TSVector3ConstIter* thresholdFz = NULL);

	static void CompareFrameElement(const Vector3& expected, const Vector3& actual, const char* message, Int32 frame, Int32 dimension, double precision);

protected:
	MotionFileHandler handler;

};

CPPUNIT_TEST_SUITE_REGISTRATION( TestMoXie );

void TestMoXie::testLoad()
{ 
	testNWalk02File( "Open3DMotionTest/Data/MoXie/nwalk02.mox" ); 
}

void TestMoXie::testExtractFileName()
{ 
	std::string test0;
	FileFormatMoXie::ExtractFileName(test0, "one.name");
	CPPUNIT_ASSERT_EQUAL(std::string("one.name"), test0);

	std::string test1;
	FileFormatMoXie::ExtractFileName(test1, "C:\\MyFolder\\A Sub Folder\\another.name");
	CPPUNIT_ASSERT_EQUAL(std::string("another.name"), test1);

	std::string test2;
	FileFormatMoXie::ExtractFileName(test2, "/home/users/someuser/my.file");
	CPPUNIT_ASSERT_EQUAL(std::string("my.file"), test2);
}

void TestMoXie::testReWriteMoXie()
{ 
	// load
	
	try
	{
		// read
		auto_ptr<TreeValue> file( handler.Read("Open3DMotionTest/Data/MoXie/nwalk02.mox") );
		
		// write MoXie
		auto_ptr<TreeValue> options( FileFormatOptionsMoXie().ToTree() );
		handler.Write("Open3DMotionTest/Data/Temp/nwalk02_rewrite.mox", file.get(), options.get());
	}
	catch(const MotionFileException& error)
	{
		CPPUNIT_FAIL(error.message);
	}

	// test re-read
	testNWalk02File("Open3DMotionTest/Data/Temp/nwalk02_rewrite.mox");
}

void TestMoXie::testTrialAttributes()
{
	try
	{
		// make test tree
		Trial trial;
		trial.Acq.Date.Year = Int32(2011);
		trial.Acq.Date.Month = Int32(1);
		trial.Acq.Date.Day = Int32(7);
		trial.UserInput.Subject.ID = "WhoAmI?";
		trial.UserInput.RepLabel = "023";
		trial.UserInput.Comment = "I couldn't possibly comment!";
		trial.UserInput.TrialCondition = "Not on your Nelly";

		// save
		auto_ptr<TreeValue> trial_tree( trial.ToTree() );
		auto_ptr<TreeValue> moxie_options( FileFormatOptionsMoXie().ToTree() );
		handler.Write("Open3DMotionTest/Data/Temp/trialattributes.mox", trial_tree.get(), moxie_options.get());

		// ... test data will now go out of scope & be destroyed
	}
	catch(const MotionFileException& error)
	{
		CPPUNIT_FAIL(error.message);
	}

	try
	{
		// load results
		auto_ptr<TreeValue> result_tree (handler.Read("Open3DMotionTest/Data/Temp/trialattributes.mox"));

		// retrieve fields
		Trial result;
		result.FromTree( result_tree.get() );

		// check
		CPPUNIT_ASSERT_EQUAL(Int32(2011), result.Acq.Date.Year.Value());
		CPPUNIT_ASSERT_EQUAL(Int32(1), result.Acq.Date.Month.Value());
		CPPUNIT_ASSERT_EQUAL(Int32(7), result.Acq.Date.Day.Value());
		CPPUNIT_ASSERT_EQUAL(string("WhoAmI?"), result.UserInput.Subject.ID.Value());
		CPPUNIT_ASSERT_EQUAL(string("023"), result.UserInput.RepLabel.Value());
		CPPUNIT_ASSERT_EQUAL(string("I couldn't possibly comment!"), result.UserInput.Comment.Value());
		CPPUNIT_ASSERT_EQUAL(string("Not on your Nelly"), result.UserInput.TrialCondition.Value());

	}
	catch(const MotionFileException& error)
	{
		CPPUNIT_FAIL(error.message);
	}

}


void TestMoXie::testForceConversion()
{
	Trial trialC3D;
	try
	{
		// load AMTI force data
		auto_ptr<TreeValue> fileC3D( handler.Read("Open3DMotionTest/Data/C3D/sample10/TYPE-4.C3D") );

		// parse
		trialC3D.FromTree( fileC3D.get() );

		// re-write as MoXie
		auto_ptr<TreeValue> moxie_options( FileFormatOptionsMoXie().ToTree() );
		handler.Write("Open3DMotionTest/Data/Temp/rewrite_sample10_TYPE-4.mox", fileC3D.get(), moxie_options.get());
	}
	catch(const MotionFileException& error)
	{
		CPPUNIT_FAIL(error.message);
	}

	// re-load
	Trial trialMox;
	try
	{
		auto_ptr<TreeValue> treeMox( handler.Read("Open3DMotionTest/Data/Temp/rewrite_sample10_TYPE-4.mox") );
		trialMox.FromTree( treeMox.get() );
	}
	catch(const MotionFileException& error)
	{
		CPPUNIT_FAIL(error.message);
	}

	// should be one forceplate in original and in reload
	CPPUNIT_ASSERT_EQUAL(size_t(1), trialC3D.Acq.ForcePlates.NumElements());
	CPPUNIT_ASSERT_EQUAL(size_t(1), trialMox.Acq.ForcePlates.NumElements());

	// expect the six force channels in reload
	CPPUNIT_ASSERT_EQUAL(size_t(6), trialMox.Acq.TimeSequences.NumElements());

	// compute force in each case
	TimeSequence* forceC3D(NULL), *pointC3D(NULL);
	TimeSequence* forceMox(NULL), *pointMox(NULL);
	ComputeForce(forceC3D, pointC3D, trialC3D);
	ComputeForce(forceMox, pointMox, trialMox);

	// all rates should match
	CPPUNIT_ASSERT_DOUBLES_EQUAL(forceC3D->Rate, pointC3D->Rate, 1E-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(forceC3D->Rate, forceMox->Rate, 1E-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(forceC3D->Rate, pointMox->Rate, 1E-6);
	
	// all frame counts should match
	Int32 numframes = forceC3D->NumFrames();
	CPPUNIT_ASSERT_EQUAL(numframes, pointC3D->NumFrames());
	CPPUNIT_ASSERT_EQUAL(numframes, forceMox->NumFrames());
	CPPUNIT_ASSERT_EQUAL(numframes, pointMox->NumFrames());

	// thresholder for Fz
	TSVector3ConstIter thresholdFz(*forceC3D);

	// compare numbers - MoXie uses 2 d.p. for N and 2 d.p. for Nm (=> tens column for Nmm)
	CompareTS(*forceC3D, *forceMox, "force", 0.01);
	CompareTS(*pointC3D, *pointMox, "point", 10.0, &thresholdFz);
}

void TestMoXie::testNWalk02File(const char* filename)
{
	// open
	Trial trial;
	try
	{
		auto_ptr<TreeValue> tree( handler.Read(filename) );
		trial.FromTree(tree.get());
	}
	catch(const MotionFileException& error)
	{
		CPPUNIT_FAIL(error.message);
	}

	// check has video camera section
	CPPUNIT_ASSERT_EQUAL(TestData::nwalk02_numcameras, (int)trial.Acq.Video.NumElements());

	// check calibration matrices
	for (int ivideo = 0; ivideo < TestData::nwalk02_numcameras; ivideo++)
	{
		const TrialVideoCamera& camera = trial.Acq.Video[ivideo];
		CPPUNIT_ASSERT( camera.Calibration.IsSet() );
		const MapArrayFloat64& P = camera.Calibration.ProjectionMatrix;

		CPPUNIT_ASSERT_EQUAL( size_t(12), P.NumElements() );

		CPPUNIT_ASSERT_DOUBLES_EQUAL(TestData::nwalk02_camera_dlt[ivideo][ 0], P[ 0], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(TestData::nwalk02_camera_dlt[ivideo][ 1], P[ 1], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(TestData::nwalk02_camera_dlt[ivideo][ 2], P[ 2], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(TestData::nwalk02_camera_dlt[ivideo][ 3], P[ 3], 1E-12);

		CPPUNIT_ASSERT_DOUBLES_EQUAL(TestData::nwalk02_camera_dlt[ivideo][ 4], P[ 4], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(TestData::nwalk02_camera_dlt[ivideo][ 5], P[ 5], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(TestData::nwalk02_camera_dlt[ivideo][ 6], P[ 6], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(TestData::nwalk02_camera_dlt[ivideo][ 7], P[ 7], 1E-12);

		CPPUNIT_ASSERT_DOUBLES_EQUAL(TestData::nwalk02_camera_dlt[ivideo][ 8], P[ 8], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(TestData::nwalk02_camera_dlt[ivideo][ 9], P[ 9], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(TestData::nwalk02_camera_dlt[ivideo][10], P[10], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(TestData::nwalk02_camera_dlt[ivideo][11], P[11], 1E-12);
	}

	// check force data
	char detail[1024];
	for (int iforce = 0; iforce < TestData::moxie_numforcechannels; iforce++)
	{
		const TimeSequence *ts = trial.Acq.GetTS(TrialSectionAcq::TSGroupAnalog, TestData::moxie_forcechannel_name[iforce]);
		CPPUNIT_ASSERT(ts != NULL);
		CPPUNIT_ASSERT_EQUAL(TestData::nwalk02_numframes_force, (int)ts->NumFrames());
		CPPUNIT_ASSERT_DOUBLES_EQUAL(TestData::nwalk02_force_rateHz, ts->Rate, 1E-12);
		TSScalarConstIter iter_ts(*ts);
		for (int iframe = 0; iframe < TestData::nwalk02_numframes_force; iframe++, iter_ts.Next())
		{
			CPPUNIT_ASSERT(iter_ts.HasFrame());
			double actual = iter_ts.Value();
			double expected = moxie_force_to_tree[iforce] * TestData::nwalk02_force[iforce][iframe];
			if (fabs(actual-expected) > 1E-12)
			{
				sprintf(detail, "%s: [force=%s][force sample=%u]\n"
					              "Expected %lf Actual %lf",
					filename,
					TestData::moxie_forcechannel_name[iforce], iframe,
					expected, actual);
				CPPUNIT_FAIL(detail);
			}
		}
	}

}

void TestMoXie::CompareTS(const TimeSequence& expected, const TimeSequence& actual, const char* message, double precision, TSVector3ConstIter* thresholdFz)
{
	TSVector3ConstIter iter_expected( expected );
	TSVector3ConstIter iter_actual( actual );

	CPPUNIT_ASSERT(expected.NumFrames() > 0);
	CPPUNIT_ASSERT_EQUAL(expected.NumFrames(), actual.NumFrames());

	for ( ; iter_expected.FrameIndex() < expected.NumFrames(); iter_expected.Next(), iter_actual.Next())
	{
		double frameprecision(precision);
		if (thresholdFz)
		{
			double Fz = thresholdFz->Value()[2];
			thresholdFz->Next();
			if (fabs(Fz) < 10.0)
				continue;
			frameprecision = fabs(precision / (Fz - 0.01));
		}

		for (Int32 idim = 0; idim < 3; idim++)
			CompareFrameElement(iter_expected.Value(), iter_actual.Value(), message, iter_expected.FrameIndex(), idim, frameprecision);
	}
}

void TestMoXie::CompareFrameElement(const Vector3& expected, const Vector3& actual, const char* message, Int32 frame, Int32 dimension, double precision)
{
	if (fabs(actual[dimension]-expected[dimension]) > precision)
	{
		char failmessage[256];
		sprintf(failmessage, "%s frame %u dimension %u: expected %lf got %lf (precision required %lf)",
			message, frame, dimension, expected[dimension], actual[dimension], precision);
		CPPUNIT_FAIL(failmessage);
	}
}


void TestMoXie::ComputeForce(TimeSequence*& force, TimeSequence*& point, const Trial& trial)
{
	// compute force and centre-of-pressure in global coords
	CPPUNIT_ASSERT(trial.Acq.ForcePlates.NumElements() >= 1);
	const ForcePlate& fp = trial.Acq.ForcePlates[0];
	std::auto_ptr<ForceCalculator> calculator( ForceCalculatorFactory().CreateCalculator(fp) );
	CPPUNIT_ASSERT( calculator.get() != NULL);
	std::vector<const TimeSequence*> analog_all;
	trial.Acq.GetTSGroup(analog_all, TrialSectionAcq::TSGroupAnalog);
	ForceMeasurementTimeSequence mts;
	mts.Set(fp, analog_all);
	CPPUNIT_ASSERT_EQUAL(size_t(6), mts.NumChannels());
	TimeSequence* freemoment(NULL);
  BinMemFactoryDefault memfactory;
	CPPUNIT_ASSERT( calculator->Compute(force, point, freemoment, mts, memfactory) );
	
	// ignore height of plate - not supported in MoXie
	for (TSVector3Iter iter_point(*point); iter_point.HasFrame(); iter_point.Next())
		iter_point.Value()[2] = 0.0;
}