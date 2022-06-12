/*--
  Open3DMotion 
  Copyright (c) 2004-2022.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

/* TestC3D.cpp
   
	 Summary
	 Unit tests for C3D files

 */

#include "Open3DMotion/MotionFile/MotionFileHandler.h"
#include "Open3DMotion/MotionFile/Formats/C3D/FileFormatOptionsC3D.h"
#include "Open3DMotion/MotionFile/Formats/C3D/MapC3DParameters.h"
#include "Open3DMotion/Biomechanics/Trial/TSFactory.h"
#include <cppunit/extensions/HelperMacros.h>

#include "testSample08File.h"

using namespace Open3DMotion;
using namespace std;

namespace TestData
{
	// modified analog channel resolution after resave to MDF
	// they are limited due to limitations of MDF format
	const static double EB015_analogtol_MDF[16] =
	{ 
		0.1, 0.1, 0.1, 30.0, 30.0, 30.0, 0.1, 0.1,
		0.1, 0.1, 0.1, 30.0, 30.0, 30.0, 0.1, 0.1,
	};

}

/* Summary
   Unit test fixture for C3D files.
   
   Description
   Currently tests sample08 loading in its different formats and
   compares with ground-truth data in TestData::EB01PI.  
   Also resaves the data in all formats and test that reload
   also corresponds to the original data.
                                                                 */
class TestC3D : public CppUnit::TestFixture
{
public:
	TestC3D() :
			handler("TestC3D", "UNVERSIONED")
	{
	}

protected:
	MotionFileHandler handler;

public:
	CPPUNIT_TEST_SUITE( TestC3D );

	CPPUNIT_TEST( testSample08_EB015 );
	CPPUNIT_TEST( testSample08_TESTA );
	CPPUNIT_TEST( testSample08_TESTB );	
	CPPUNIT_TEST( testSample08_TESTC );
	CPPUNIT_TEST( testSample08_TESTD );

	CPPUNIT_TEST( testReWriteSample08_c3d_pc_integer );
  CPPUNIT_TEST( testReWriteSample08_c3d_pc_float );
	CPPUNIT_TEST( testReWriteSample08_c3d_dec_integer );
	CPPUNIT_TEST( testReWriteSample08_c3d_dec_float );
	CPPUNIT_TEST( testReWriteSample08_c3d_sgi_integer );
	CPPUNIT_TEST( testReWriteSample08_c3d_sgi_float );
	
	CPPUNIT_TEST(testAnalogPreAcquisitionOffsets);

	/*
	CPPUNIT_TEST( testReWriteSample08_CODAtext );
	CPPUNIT_TEST( testReWriteSample08_CODAmotion_xmove );
	CPPUNIT_TEST( testReWriteSample08_CODAmotion_MDF_2 );
	CPPUNIT_TEST( testReWriteSample08_CODAmotion_MDF_3 );
	*/

	CPPUNIT_TEST_SUITE_END();

public:
	void setUp()
	{
	}

	void tearDown()
	{
	}

public:
	void testSample08_EB015()
	{ testSample08File(handler, "Open3DMotionTest/Data/C3D/sample08/EB015PI.c3d"); }

	void testSample08_TESTA()
	{ testSample08File(handler, "Open3DMotionTest/Data/C3D/sample08/TESTAPI.c3d"); }

	void testSample08_TESTB()
	{ testSample08File(handler, "Open3DMotionTest/Data/C3D/sample08/TESTBPI.c3d"); }

	void testSample08_TESTC()
	{ testSample08File(handler, "Open3DMotionTest/Data/C3D/sample08/TESTCPI.c3d"); }

	void testSample08_TESTD()
	{ testSample08File(handler, "Open3DMotionTest/Data/C3D/sample08/TESTDPI.c3d"); }

	void testReWriteSample08_c3d_pc_integer()
	{ testReWriteSample08("C3D", FileFormatOptionsC3D::ProcessorPC, false, "c3d"); }

	void testReWriteSample08_c3d_pc_float()
	{ testReWriteSample08("C3D", FileFormatOptionsC3D::ProcessorPC, true, "c3d"); }

	void testReWriteSample08_c3d_dec_integer()
	{ testReWriteSample08("C3D", FileFormatOptionsC3D::ProcessorDEC, false, "c3d"); }

	void testReWriteSample08_c3d_dec_float()
	{ testReWriteSample08("C3D", FileFormatOptionsC3D::ProcessorDEC, true, "c3d"); }

	void testReWriteSample08_c3d_sgi_integer()
	{ testReWriteSample08("C3D", FileFormatOptionsC3D::ProcessorSGI, false, "c3d"); }

	void testReWriteSample08_c3d_sgi_float()
	{ testReWriteSample08("C3D", FileFormatOptionsC3D::ProcessorSGI, true, "c3d"); }

	void testAnalogPreAcquisitionOffsets();

#if 0

	void testReWriteSample08_CODAmotion_xmove()
	{ testReWriteSample08("CODAmotion_xmove", "xml"); }

	void testReWriteSample08_CODAmotion_MDF_2()
	{
		// MDF 2 has reduced marker location resolution (usually +- 0.1mm)
		// MDF 2 & 3 both have limit on analog resolution (usually +- 0.1 units for Fxyz and +- 10 units for Mxyz)
		testReWriteSample08("CODAmotion_MDF_2", "mdf", 0.1, TestData::EB015_analogtol_MDF, true, false, true); 
	}

	void testReWriteSample08_CODAmotion_MDF_3()
	{ 
		// MDF 2 & 3 both have limit on analog resolution
		testReWriteSample08("CODAmotion_MDF_3", "mdf", TestData::EB015_markertol, TestData::EB015_analogtol_MDF, true, false, true); 
	}

	void testReWriteSample08_CODAtext()
	{ testReWriteSample08("CODAtext", "txt", 0.001, NULL, false, true); }

#endif

public:

	void testReWriteSample08(const char* formatID, const char* processor, bool floatingpoint, const char* extension, double markertol = TestData::EB015_markertol, const double* analogtol = TestData::EB015_analogtol, bool allow_analog_reorder = false, bool allow_analog_removal = false, bool exclude_force = false);
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestC3D );

void TestC3D::testReWriteSample08(const char* formatID, const char* processor, bool floatingpoint, const char* extension, double markertol, const double* analogtol, bool allow_analog_reorder, bool allow_analog_removal, bool exclude_force)
{
	// make save name
	std::string savename("Open3DMotionTest/Data/Temp/EB015PI_rewrite_");
	savename += processor;
	savename += floatingpoint ? "_F" : "_I";
	savename += ".";
	savename += extension;

	// open and re-write to specified format
	try
	{
		std::unique_ptr<TreeValue> trialcontents( handler.Read("Open3DMotionTest/Data/C3D/sample08/EB015PI.c3d") );

		FileFormatOptionsC3D c3doptions;
		c3doptions.FormatID = formatID;
		c3doptions.Processor = processor;
		c3doptions.FloatingPoint = floatingpoint;
		TreeValue* writeoptions = c3doptions.ToTree();

		handler.Write(savename.c_str(), trialcontents.get(), writeoptions);
	}
	catch(MotionFileException& error)
	{
		CPPUNIT_FAIL(error.message);
	}

	// verify data from re-load
	testSample08File(handler, savename.c_str(), markertol, analogtol, allow_analog_reorder, allow_analog_removal, exclude_force);
}

void TestC3D::testAnalogPreAcquisitionOffsets()
{
	// read
	const char* filename = "Open3DMotionTest/Data/MDF/gait-bilateral-2005-AMTIx2.mdf";
	std::unique_ptr<Open3DMotion::TreeValue> originalcontents;
	try
	{
		originalcontents = std::unique_ptr<Open3DMotion::TreeValue>(handler.Read(filename));
	}
	catch (const Open3DMotion::MotionFileException & error)
	{
		CPPUNIT_FAIL(error.message);
	}

	// build trial object
	std::unique_ptr<Open3DMotion::Trial> trial(new Open3DMotion::Trial);
	trial->FromTree(originalcontents.get());

	// get channels in modifyable form
	std::vector<Open3DMotion::TimeSequence*> analog;
	size_t numts = trial->Acq.TimeSequences.NumElements();
	for (size_t i = 0; i < numts; i++)
	{
		TimeSequence& ts = trial->Acq.TimeSequences[i];
		if (ts.Group.Value().compare(Open3DMotion::TrialSectionAcq::TSGroupAnalog) == 0)
			analog.push_back(&ts);
	}

	// ensure 16 channels as expected in this example
	CPPUNIT_ASSERT_EQUAL(size_t(16), analog.size());

	// set some channel offsets to non-zero to simulate force baseline offset
	analog[0]->Offset = 3.2;
	analog[1]->Offset = -7.8;
	analog[0]->OffsetStdDev = 0.012;
	analog[1]->OffsetStdDev = 0.023;

	// write C3D
	try
	{
		// Get updated tree
		std::unique_ptr<TreeValue> newcontents(trial->ToTree());

		// PC format, floating point
		FileFormatOptionsC3D c3doptions;
		c3doptions.FormatID = "C3D";
		c3doptions.Processor = FileFormatOptionsC3D::ProcessorPC;
		c3doptions.FloatingPoint = true;
		std::auto_ptr<Open3DMotion::TreeValue> writeoptions(c3doptions.ToTree());

		// Write
		handler.Write("Open3DMotionTest/Data/Temp/ADemo1_with_offsets.c3d", newcontents.get(), writeoptions.get());
	}
	catch (MotionFileException & error)
	{
		CPPUNIT_FAIL(error.message);
	}

	// load the C3D
	std::unique_ptr<Open3DMotion::TrialC3D> reloadedtrial(new Open3DMotion::TrialC3D);
	try
	{
		std::unique_ptr<Open3DMotion::TreeValue> reloadedcontents(handler.Read("Open3DMotionTest/Data/Temp/ADemo1_with_offsets.c3d"));
		reloadedtrial->FromTree(reloadedcontents.get());
	}
	catch (const Open3DMotion::MotionFileException & error)
	{
		CPPUNIT_FAIL(error.message);
	}

	// offsets should have been set to zero
	const C3DRecordInteger* analog_offset = reloadedtrial->C3D.Parameters.GetRecordType<C3DRecordInteger>("ANALOG", "OFFSET");
	CPPUNIT_ASSERT(analog_offset != NULL);
	CPPUNIT_ASSERT_EQUAL(size_t(16), analog_offset->Data().size());
	CPPUNIT_ASSERT_EQUAL(Int16(0), analog_offset->Data()[ 0]);
	CPPUNIT_ASSERT_EQUAL(Int16(0), analog_offset->Data()[ 1]);
	CPPUNIT_ASSERT_EQUAL(Int16(0), analog_offset->Data()[ 2]);
	CPPUNIT_ASSERT_EQUAL(Int16(0), analog_offset->Data()[ 3]);
	CPPUNIT_ASSERT_EQUAL(Int16(0), analog_offset->Data()[ 4]);
	CPPUNIT_ASSERT_EQUAL(Int16(0), analog_offset->Data()[ 5]);
	CPPUNIT_ASSERT_EQUAL(Int16(0), analog_offset->Data()[ 6]);
	CPPUNIT_ASSERT_EQUAL(Int16(0), analog_offset->Data()[ 7]);
	CPPUNIT_ASSERT_EQUAL(Int16(0), analog_offset->Data()[ 8]);
	CPPUNIT_ASSERT_EQUAL(Int16(0), analog_offset->Data()[ 9]);
	CPPUNIT_ASSERT_EQUAL(Int16(0), analog_offset->Data()[10]);
	CPPUNIT_ASSERT_EQUAL(Int16(0), analog_offset->Data()[11]);
	CPPUNIT_ASSERT_EQUAL(Int16(0), analog_offset->Data()[12]);
	CPPUNIT_ASSERT_EQUAL(Int16(0), analog_offset->Data()[13]);
	CPPUNIT_ASSERT_EQUAL(Int16(0), analog_offset->Data()[14]);
	CPPUNIT_ASSERT_EQUAL(Int16(0), analog_offset->Data()[15]);

	// and original offset values recorded instead in custom parameter
	const C3DRecordReal* acq_preoffset = reloadedtrial->C3D.Parameters.GetRecordType<C3DRecordReal>("ANALOG", "ACQUISITION_PREOFFSET");
	CPPUNIT_ASSERT(acq_preoffset != NULL);
	CPPUNIT_ASSERT_EQUAL(size_t(16), acq_preoffset->Data().size());
	CPPUNIT_ASSERT_EQUAL( 3.2f, acq_preoffset->Data()[0]);
	CPPUNIT_ASSERT_EQUAL(-7.8f, acq_preoffset->Data()[1]);
	CPPUNIT_ASSERT_EQUAL(0.0f, acq_preoffset->Data()[2]);
	CPPUNIT_ASSERT_EQUAL(0.0f, acq_preoffset->Data()[3]);
	CPPUNIT_ASSERT_EQUAL(0.0f, acq_preoffset->Data()[4]);
	CPPUNIT_ASSERT_EQUAL(0.0f, acq_preoffset->Data()[5]);
	CPPUNIT_ASSERT_EQUAL(0.0f, acq_preoffset->Data()[6]);
	CPPUNIT_ASSERT_EQUAL(0.0f, acq_preoffset->Data()[7]);
	CPPUNIT_ASSERT_EQUAL(0.0f, acq_preoffset->Data()[8]);
	CPPUNIT_ASSERT_EQUAL(0.0f, acq_preoffset->Data()[9]);
	CPPUNIT_ASSERT_EQUAL(0.0f, acq_preoffset->Data()[10]);
	CPPUNIT_ASSERT_EQUAL(0.0f, acq_preoffset->Data()[11]);
	CPPUNIT_ASSERT_EQUAL(0.0f, acq_preoffset->Data()[12]);
	CPPUNIT_ASSERT_EQUAL(0.0f, acq_preoffset->Data()[13]);
	CPPUNIT_ASSERT_EQUAL(0.0f, acq_preoffset->Data()[14]);
	CPPUNIT_ASSERT_EQUAL(0.0f, acq_preoffset->Data()[15]);

	// but the effect should be that the offsets are restored
	std::vector<const Open3DMotion::TimeSequence*> analog_reload;
	reloadedtrial->Acq.GetTSGroup(analog_reload, Open3DMotion::TrialSectionAcq::TSGroupAnalog);
	CPPUNIT_ASSERT_EQUAL(size_t(16), analog_reload.size());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(3.2, analog_reload[0]->Offset.Value(), 1E-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-7.8, analog_reload[1]->Offset.Value(), 1E-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.012, analog_reload[0]->OffsetStdDev.Value(), 1E-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.023, analog_reload[1]->OffsetStdDev.Value(), 1E-6);

	// and sequences should be unaffected by the change
	const double signal_delta = 1E-4;
	for (size_t channel_index = 0; channel_index < analog.size(); channel_index++)
	{
		CPPUNIT_ASSERT_EQUAL(analog[channel_index]->NumFrames(), analog_reload[channel_index]->NumFrames());
		Open3DMotion::TSScalarConstIter iter_original(*analog[channel_index]);
		Open3DMotion::TSScalarConstIter iter_reload(*analog_reload[channel_index]);
		for (; iter_original.HasFrame(); iter_original.Next(), iter_reload.Next())
		{
			if (fabs(iter_reload.Value() - iter_original.Value()) > signal_delta)
			{
				std::ostringstream message;
				message << "Mismatch in analog channel values after reload "
								<< "[channel " << channel_index << " frame " << iter_original.FrameIndex() << "]" << std::ends;
				CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(message.str(), iter_original.Value(), iter_reload.Value(), signal_delta);
			}
		}
	}
}