#include "Open3DMotion/MotionFile/Formats/CODAText/MATextReader.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinMemFactoryDefault.h"
#include <cppunit/extensions/HelperMacros.h>

using namespace Open3DMotion;
using namespace std;

class TestMATextReader : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE( TestMATextReader );
	CPPUNIT_TEST(testGraphData);
	CPPUNIT_TEST_SUITE_END();

public:

	void testGraphData();
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestMATextReader );

void TestMATextReader::testGraphData()
{
	MATextReader reader;
	
	// make path to test data & open
	ifstream input("Open3DMotionTest/Data/CODAText/GraphData.txt", ios::binary);
	CPPUNIT_ASSERT( reader.Read(input) );

	CPPUNIT_ASSERT_EQUAL(std::string("Graph data: \"L.Knee Rotation\""), reader.Comment());

	CPPUNIT_ASSERT_EQUAL(size_t(801), reader.NumFrames());

	CPPUNIT_ASSERT_DOUBLES_EQUAL(200.0, reader.RateHz(), 1E-6);

	BinMemFactoryDefault memfactory;
	CPPUNIT_ASSERT( reader.GetTSOccVector3("nosuchgroup", "nosuchchannel", memfactory) == NULL);

	TimeSequence* tsLToe = reader.GetTSOccVector3("Marker Position", "L.Toe", memfactory);
	CPPUNIT_ASSERT(tsLToe != NULL);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(200.0, tsLToe->Rate, 1E-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tsLToe->Start, 1E-6);
	CPPUNIT_ASSERT_EQUAL(Int32(801), tsLToe->NumFrames());
	CPPUNIT_ASSERT_EQUAL(std::string("mm"), tsLToe->Units.Value());

	TimeSequence* tsLHeel = reader.GetTSOccVector3("Marker Position", "L.Heel", memfactory);
	CPPUNIT_ASSERT( tsLHeel != NULL );
	CPPUNIT_ASSERT_DOUBLES_EQUAL(200.0, tsLHeel->Rate, 1E-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tsLHeel->Start, 1E-6);
	CPPUNIT_ASSERT_EQUAL( Int32(801), tsLHeel->NumFrames());
	CPPUNIT_ASSERT_EQUAL(std::string("mm"), tsLHeel->Units.Value());

	TimeSequence* tsAnalog = reader.GetTSScalar("Force Analogue Channel", "Analogue1.4", memfactory);
	CPPUNIT_ASSERT(tsAnalog != NULL);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(200.0, tsAnalog->Rate, 1E-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tsAnalog->Start, 1E-6);
	CPPUNIT_ASSERT_EQUAL( Int32(801), tsAnalog->NumFrames());

	TimeSequence* tsPelvisEVBx = reader.GetTSOccVector3("Segment EVB.x", "Pelvis", memfactory);
	CPPUNIT_ASSERT(tsPelvisEVBx != NULL);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(200.0, tsPelvisEVBx->Rate, 1E-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tsPelvisEVBx->Start, 1E-6);
	CPPUNIT_ASSERT_EQUAL(Int32(801), tsPelvisEVBx->NumFrames());

	TimeSequence* tsLKneeAngle = reader.GetTSOccVector3("Segment Rotation", "L.Knee", memfactory);
	CPPUNIT_ASSERT(tsLKneeAngle != NULL);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(200.0, tsLKneeAngle->Rate, 1E-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tsLKneeAngle->Start, 1E-6);
	CPPUNIT_ASSERT_EQUAL(Int32(801), tsLKneeAngle->NumFrames());

	// test some occlusion flags
	TSOccVector3ConstIter iterLHeel( *tsLHeel );
	CPPUNIT_ASSERT_EQUAL(0, (int)iterLHeel.Occluded());
	iterLHeel.Next();
	CPPUNIT_ASSERT_EQUAL(0, (int)iterLHeel.Occluded());
	iterLHeel.Next();
	CPPUNIT_ASSERT_EQUAL(0, (int)iterLHeel.Occluded());
	iterLHeel.Next();
	CPPUNIT_ASSERT_EQUAL(1, (int)iterLHeel.Occluded());
	iterLHeel.Next();
	CPPUNIT_ASSERT_EQUAL(1, (int)iterLHeel.Occluded());
	iterLHeel.Next();
	CPPUNIT_ASSERT_EQUAL(0, (int)iterLHeel.Occluded());
	iterLHeel.Next();
	CPPUNIT_ASSERT_EQUAL(0, (int)iterLHeel.Occluded());

	// test just some first and last bits of data
	TSOccVector3ConstIter iterLToe( *tsLToe );
	TSOccVector3ConstIter iterLKneeAngle( *tsLKneeAngle );
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-2501.61, iterLToe.Value()[0], 1E-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(  185.60, iterLToe.Value()[1], 1E-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(  -12.21, iterLToe.Value()[2], 1E-6);
	iterLToe.Next();
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-2501.52, iterLToe.Value()[0], 1E-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(  185.52, iterLToe.Value()[1], 1E-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(  -12.11, iterLToe.Value()[2], 1E-6);
	iterLKneeAngle.NextN(799);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -2.833, iterLKneeAngle.Value()[0], 1E-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 18.735, iterLKneeAngle.Value()[1], 1E-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 11.800, iterLKneeAngle.Value()[2], 1E-6);
	iterLKneeAngle.Next();
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -2.880, iterLKneeAngle.Value()[0], 1E-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 18.972, iterLKneeAngle.Value()[1], 1E-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 11.802, iterLKneeAngle.Value()[2], 1E-6);


}

