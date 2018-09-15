/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

/* TestMDF.cpp
   
	 Summary
	 Unit tests for MDF file format

 */

#include "Open3DMotion/MotionFile/MotionFileHandler.h"
#include "Open3DMotion/MotionFile/MotionFileFormat.h"
#include "Open3DMotion/MotionFile/Formats/MDF/FileFormatOptionsMDF.h"
#include "Open3DMotion/MotionFile/Formats/C3D/FileFormatOptionsC3D.h"
#include "Open3DMotion/MotionFile/Formats/XMove/FileFormatOptionsXMove.h"
#include "Open3DMotion/Biomechanics/Trial/TSFactory.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinMemFactoryDefault.h"
#include <cppunit/extensions/HelperMacros.h>
#include "Open3DMotionTest/Data/MDF/ADemo1.h"
#include "Open3DMotionTest/Data/MDF/Walk4.h"
#include "testADemo1File.h"
#include "testWalk4DescriptorsInFile.h"
#include <math.h>

/* Summary
   Unit test fixture for MDF format files.
   
   Description
   Tests consistency of the force centre-of-pressure loaded from
	 the file compared with recalculated centre-of-pressure using
	 the CentreOffset parameter and (Fx,Fy,Fz,Mx,My,Mz)
                                                                 */
class TestMDF : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE( TestMDF );
	CPPUNIT_TEST( testADemo1 );
	CPPUNIT_TEST( testReWriteADemo1_CODAmotion_MDF2 );
	CPPUNIT_TEST( testReWriteADemo1_CODAmotion_MDF3 );
	CPPUNIT_TEST( testReWriteADemo1_CODAmotion_xmove );
	CPPUNIT_TEST( testReWriteADemo1_c3d_pc_float );
	CPPUNIT_TEST( testReWriteADemo1_c3d_pc_integer );
	CPPUNIT_TEST( testCoPConsistent_SingleAMTI_MA64 );
	CPPUNIT_TEST( testCoPConsistent_DualAMTIGait_MA64 );
	CPPUNIT_TEST( testCoPConsistent_DualAMTIGait_MA63 );
#ifdef OPEN3DMOTION_TEST_MDF_WALK4
	CPPUNIT_TEST( testWalk4Descriptors );
	CPPUNIT_TEST( testReWriteWalk4Descriptors_CODAmotion_xmove );
	CPPUNIT_TEST( testReWriteWalk4Descriptors_c3d_pc_float );
	CPPUNIT_TEST( testReWriteWalk4Descriptors_c3d_pc_integer );
#endif
	CPPUNIT_TEST( testReadWheelMDF2 );
	CPPUNIT_TEST_SUITE_END();

public:

	TestMDF() :
			handler("TestMDF", "UNVERSIONED")
	{
	}

public:

	/*
		Summary
		Compare data loaded directly from Ademo1.mdf with ADemo1 data set

		See Also
		testADemo1File
		*/
	void testADemo1()
	{ testADemo1File(handler, "Open3DMotionTest/Data/MDF/gait-bilateral-1997-Kistlerx1.mdf", true, 1, true, true, true); }

	/*
		Summary
		Test resave of ADemo1 data set in MDF2 format and compare with ground truth

		See Also
		testADemo1File
		*/
	void testReWriteADemo1_CODAmotion_MDF2()
	{ 
		Open3DMotion::FileFormatOptionsMDF options;
		options.FormatVersion = Open3DMotion::FileFormatOptionsMDF::VERSION2;
		testReWriteADemo1(options, "MDF2.mdf", false, 1, true, true, true); 
	}
  
	/*
		Summary
		Test resave of ADemo1 data set in MDF3 format and compare with ground truth

		See Also
		testADemo1File
		*/
	void testReWriteADemo1_CODAmotion_MDF3()
	{ 
		Open3DMotion::FileFormatOptionsMDF options;
		options.FormatVersion = Open3DMotion::FileFormatOptionsMDF::VERSION3;
		testReWriteADemo1(options, "MDF3.mdf", false, 1, true, true, true); 
	}


  /*
		Summary
		Test resave of ADemo1 data set in XMove format and compare with ground truth

		See Also
		testADemo1File
		*/
	void testReWriteADemo1_CODAmotion_xmove()
	{ 
		Open3DMotion::FileFormatOptionsXMove options;
		testReWriteADemo1(options, "XMove.xml", true, 1, true, true, true); 
	}

	/*
		Summary
		Test resave of ADemo1 data set in c3d_pc_float format and compare with ground truth

		See Also
		testADemo1File
		*/
	void testReWriteADemo1_c3d_pc_float()
	{
		Open3DMotion::FileFormatOptionsC3D options;
		options.Processor = Open3DMotion::FileFormatOptionsC3D::ProcessorPC;
		options.FloatingPoint = true;
		testReWriteADemo1(options, "c3d_pc_float.c3d", true, 2, false, false, false); 
	}

	/*
		Summary
		Test resave of ADemo1 data set in c3d_pc_integer format and compare with ground truth

		See Also
		testADemo1File
		*/
	void testReWriteADemo1_c3d_pc_integer()
	{ 
		Open3DMotion::FileFormatOptionsC3D options;
		options.Processor = Open3DMotion::FileFormatOptionsC3D::ProcessorPC;
		options.FloatingPoint = false;
		testReWriteADemo1(options, "c3d_pc_integer.c3d", false, 2, false, false, false); 
	}


	/*
		Summary
		Check centre-of-pressure consistency on test file with single AMTI plate
		and full centre offset specified in file.
		
		See Also
		testAMTIForceCoPConsistent
	 */
	void testCoPConsistent_SingleAMTI_MA64()
	{ testAMTIForceCoPConsistent("Open3DMotionTest/Data/MDF/forceonly-2007-AMTIx1.mdf", 1, fpcalc_MA64); }

	/*
		Summary
		Check centre-of-pressure consistency on test file with dual AMTI plates
		but incorrect centre offset specified in file.

		See Also
		testAMTIForceCoPConsistent
		*/
	void testCoPConsistent_DualAMTIGait_MA64()
	{ testAMTIForceCoPConsistent("Open3DMotionTest/Data/MDF/gait-bilateral-2005-AMTIx2.mdf", 2, fpcalc_MA64); }

	/*
		Summary
		Check centre-of-pressure consistency on test file with dual AMTI plates,
		no centre offset in file and MA63 force calculations.

		See Also
		testAMTIForceCoPConsistent
		*/
	void testCoPConsistent_DualAMTIGait_MA63()
	{	testAMTIForceCoPConsistent("Open3DMotionTest/Data/MDF/gait-bilateral-2004-AMTIx2-MA63.mdf", 2, fpcalc_MA63); }

	/*
		Summary
		Test that channel counts, frame counts, time offsets and names are as expected
		for Walk4 data set as read from MDF.  This file has 336 marker channels acquired
		using multi-coda mode. (Uses testWalk4DescriptorsInFile).

		See Also
		testWalk4DescriptorsInFile
		*/
	void testWalk4Descriptors()
	{ testWalk4DescriptorsInFile(handler, "Open3DMotionTest/Data/MDF/gait-clusters-multicoda-Kistlerx2.mdf", true); }

	/*
		Summary
		Test Walk4 data set after rewrite in XMove format.

		See Also
		testWalk4Descriptors, testWalk4DescriptorsInFile
		*/
	void testReWriteWalk4Descriptors_CODAmotion_xmove()
	{ 
		Open3DMotion::FileFormatOptionsXMove options;
		testReWriteWalk4Descriptors(options, "CODAmotion_xmove.xml", true); 
	}

	/*
		Summary
		Test Walk4 data set after rewrite in C3D (PC floating point) format.

		See Also
		testWalk4Descriptors, testWalk4DescriptorsInFile
		*/
	void testReWriteWalk4Descriptors_c3d_pc_float()
	{ 
		Open3DMotion::FileFormatOptionsC3D options;
		options.Processor = Open3DMotion::FileFormatOptionsC3D::ProcessorPC;
		options.FloatingPoint = true;
		testReWriteWalk4Descriptors(options, "c3d_pc_float.c3d", false); 
	}

	/*
		Summary
		Test Walk4 data set after rewrite in C3D (PC floating integer) format.

		See Also
		testWalk4Descriptors, testWalk4DescriptorsInFile
		*/
	void testReWriteWalk4Descriptors_c3d_pc_integer()
	{ 
		Open3DMotion::FileFormatOptionsC3D options;
		options.Processor = Open3DMotion::FileFormatOptionsC3D::ProcessorPC;
		options.FloatingPoint = false;
		testReWriteWalk4Descriptors(options, "c3d_pc_integer.c3d", false); 
	}

public:

	/* Summary
	   Calculation options for AMTI formulae as originally used in MDF file */
	enum fpcalc
	{
		fpcalc_AMTI,
		fpcalc_MA64,
		fpcalc_MA63
	};

	/*
	  Summary
		Compare calculated centre-of-pressure from AMTI plate with the one originally stored in MDF file.

		Description
	  Check that the centre-of-pressure channels from the MDF file match with
	  a new centre-of-pressure freshly computed from the force and moments components.
		The calcoption parameter allows the new computation to be modified to match
		the (incorrect) formulae used in previous versions of MA
	  The match will not be to machine precision because MDF files truncate
	  all analog data to store it in signed short values - this function looks for 
	  correspondence only up to the expected precision.
	 */
	void testAMTIForceCoPConsistent(const char* filename, size_t numplates, int calcoption);

	/*
		Summary
		Test rewrite of the ADemo1 data set in the specified format

		*/
	void testReWriteADemo1(const Open3DMotion::FileFormatOptions& options, const char* suffix, bool strict, size_t forcedecimation, bool checkmarkerid, bool checkgaitcycle, bool checksubjectdata);

	/*
		Summary
		Retest Walk4 descriptors after rewrite

		See Also
		testWalk4DescriptorsInFile
		*/
	void testReWriteWalk4Descriptors(const Open3DMotion::FileFormatOptions& options, const char* suffix, bool enforcestartoffset);


	/*
		Summary
		Verify that read of old MDF2 succeeds.
		*/
	void testReadWheelMDF2();

protected:
	Open3DMotion::MotionFileHandler handler;
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestMDF );

void TestMDF::testAMTIForceCoPConsistent(const char* filename, size_t numplates, int calcoption) 
{
	// read
	std::auto_ptr<Open3DMotion::TreeValue> trialcontents;
	try
	{
		trialcontents = std::auto_ptr<Open3DMotion::TreeValue>( handler.Read(filename) );
	}
	catch(const Open3DMotion::MotionFileException& error)
	{
		CPPUNIT_FAIL(error.message);
	}

	// build trial object
	std::auto_ptr<Open3DMotion::Trial> trial( new Open3DMotion::Trial );
	trial->FromTree(trialcontents.get());

	// get details of all plates
	CPPUNIT_ASSERT_EQUAL(numplates, trial->Acq.ForcePlates.NumElements());

	for (size_t iplate = 0; iplate < numplates; iplate++)
	{
		// analog channel list for this plate
		const Open3DMotion::ForcePlate& fp = trial->Acq.ForcePlates[iplate];

		// 6 channels per plate in channel spec
		CPPUNIT_ASSERT_EQUAL(size_t(6), fp.Channels.NumElements());

		// 2 channels of precomputed pressure data
		CPPUNIT_ASSERT_EQUAL(size_t(2), fp.PreCompCoPChannels.NumElements());

		// sensor constants
		CPPUNIT_ASSERT( fp.CentreOffset.IsSet() );
		double kx = fp.CentreOffset.X;
		double ky = fp.CentreOffset.Y;
		double kz = fp.CentreOffset.Z;

		// get channels
		std::vector<const Open3DMotion::TimeSequence*> analog;
		trial->Acq.GetTSGroup(analog, Open3DMotion::TrialSectionAcq::TSGroupAnalog);

		// analog channels from file
		Open3DMotion::TSScalarConstIter tsFx(*analog[fp.Channels[0]-1]);
		Open3DMotion::TSScalarConstIter tsFy(*analog[fp.Channels[1]-1]);
		Open3DMotion::TSScalarConstIter tsFz(*analog[fp.Channels[2]-1]);
		Open3DMotion::TSScalarConstIter tsMx(*analog[fp.Channels[3]-1]);
		Open3DMotion::TSScalarConstIter tsMy(*analog[fp.Channels[4]-1]);
		Open3DMotion::TSScalarConstIter tsMz(*analog[fp.Channels[5]-1]);
		Open3DMotion::TSScalarConstIter tsCodaPx(*analog[fp.PreCompCoPChannels[0]-1]);
		Open3DMotion::TSScalarConstIter tsCodaPy(*analog[fp.PreCompCoPChannels[1]-1]);

		// scale factors
		double scaleFx = analog[fp.Channels[0]-1]->Scale;
		double scaleFy = analog[fp.Channels[1]-1]->Scale;
		double scaleFz = analog[fp.Channels[2]-1]->Scale;
		double scaleMx = analog[fp.Channels[3]-1]->Scale;
		double scaleMy = analog[fp.Channels[4]-1]->Scale;
		double scaleMz = analog[fp.Channels[5]-1]->Scale;
		double scaleCodaPx = analog[fp.PreCompCoPChannels[0]-1]->Scale;
		double scaleCodaPy = analog[fp.PreCompCoPChannels[1]-1]->Scale;

		// offset values
		double offsetFx = analog[fp.Channels[0]-1]->Offset;
		double offsetFy = analog[fp.Channels[1]-1]->Offset;
		double offsetFz = analog[fp.Channels[2]-1]->Offset;
		double offsetMx = analog[fp.Channels[3]-1]->Offset;
		double offsetMy = analog[fp.Channels[4]-1]->Offset;
		double offsetMz = analog[fp.Channels[5]-1]->Offset;
		double offsetCodaPx = analog[fp.PreCompCoPChannels[0]-1]->Offset;
		double offsetCodaPy = analog[fp.PreCompCoPChannels[1]-1]->Offset;

		// check time ranges match
		size_t numframes = tsFx.NumFrames();
		CPPUNIT_ASSERT(tsFy.NumFrames() == numframes);
		CPPUNIT_ASSERT(tsFz.NumFrames() == numframes);
		CPPUNIT_ASSERT(tsMx.NumFrames() == numframes);
		CPPUNIT_ASSERT(tsMy.NumFrames() == numframes);
		CPPUNIT_ASSERT(tsMz.NumFrames() == numframes);
		CPPUNIT_ASSERT(tsCodaPx.NumFrames() == numframes);
		CPPUNIT_ASSERT(tsCodaPy.NumFrames() == numframes);

		// check data
		for (size_t iframe = 0; iframe < numframes; iframe++)
		{
			// get input data and apply calibration
			double Fx = scaleFx * (tsFx.Value() - offsetFx);
			double Fy = scaleFy * (tsFy.Value() - offsetFy);
			double Fz = scaleFz * (tsFz.Value() - offsetFz);
			double Mx = scaleMx * (tsMx.Value() - offsetMx);
			double My = scaleMy * (tsMy.Value() - offsetMy);
			double Mz = scaleMz * (tsMz.Value() - offsetMz);
			double codaPx = scaleCodaPx * (tsCodaPx.Value() - offsetCodaPx);
			double codaPy = scaleCodaPy * (tsCodaPy.Value() - offsetCodaPy);

			// use only frames with > 1 N force
			if (fabs(Fz) > 1.0)
			{
				double compPx = 0.0, compPy = 0.0;
				if (calcoption == fpcalc_AMTI)
				{
					// compute precisely according to AMTI formulae
					compPx = -(My + kz*Fx)/Fz + kx;
					compPy =  (Mx - kz*Fy)/Fz + ky;
				}
				else if (calcoption == fpcalc_MA64)
				{
					// compute precisely according to MA64-MA67 formulae
					compPx = -(My - kz*Fx)/Fz + kx;
					compPy =  (Mx + kz*Fy)/Fz + ky;
				}
				else if (calcoption == fpcalc_MA63)
				{
					// compute according to MA63 formulae
					compPx = -(My - kz*Fx)/Fz + kx;
					compPy =  (Mx - kz*Fy)/Fz + ky;
				}
				else
				{
					CPPUNIT_FAIL("testAMTIForceCoPConsistent: invalid FP calc option specified");
				}

				// expected error bounds
				// this is computed using the default MDF save resolutions:
				// With moments in Nm these are:
				//   { 0.01N, 0.01N, 0.06N, 0.03Nm, 0.03Nm, 0.01Nm, 0.01mm, 0.01mm }
				// So with moments in Nmm they become:
				//   { 0.01N, 0.01N, 0.06N,  30Nmm,  30Nmm,  10Nmm, 0.01mm, 0.01mm } 
				double expError = 0.1 +
				  ((30 + 0.01*fabs(kz))/fabs(Fz)) +
					(fabs(My + kz*Fx) / (fabs(Fz)-0.06)) - (fabs(My + kz*Fx) / (fabs(Fz)+0.06));

				// check components: note swapped between Coda and FP coords
				if (fabs(codaPy-compPx) > expError ||
						fabs(codaPx-compPy) > expError)
				{
					char detail[1024];
					sprintf(detail, 
						"[frame=%u][forceplate=%u][F=(%lf,%lf,%lf)][M=(%lf,%lf,%lf)]\n" \
						"P calc = (%lf, %lf), P mdf = (%lf, %lf) expError = %lf",
						(int)iframe, (int)iplate, Fx, Fy, Fz, Mx, My, Mz,
						compPx, compPy, codaPy, codaPx,
						expError);
					CPPUNIT_FAIL(detail);
				}

				// new iteration
				tsFx.Next();
				tsFy.Next();
				tsFz.Next();
				tsMx.Next();
				tsMy.Next();
				tsMz.Next();
				tsCodaPx.Next();
				tsCodaPy.Next();
			}
		}
	}
}

void TestMDF::testReWriteADemo1(const Open3DMotion::FileFormatOptions& options, const char* suffix, bool strict, size_t forcedecimation, bool checkmarkerid, bool checkgaitcycle, bool checksubjectdata)
{
	// make save name
	std::string savename = 
		std::string("Open3DMotionTest/Data/Temp/ADemo1_rewrite_") + 
		std::string(suffix);

	// open and re-write to specified format
	
	try
	{
		// read in
		std::auto_ptr<Open3DMotion::TreeValue> trial_tree( handler.Read("Open3DMotionTest/Data/MDF/gait-bilateral-1997-Kistlerx1.mdf") );
				
		// make writing options
		std::auto_ptr<Open3DMotion::TreeValue> format_value( options.ToTree() );

		// write
		handler.Write(savename.c_str(), trial_tree.get(), format_value.get());
	}
	catch(const Open3DMotion::MotionFileException& error)
	{
		CPPUNIT_FAIL(error.message);
	}

	// verify data from re-load
	testADemo1File(handler, savename.c_str(), strict, forcedecimation, checkmarkerid, checkgaitcycle, checksubjectdata);
}

void TestMDF::testReWriteWalk4Descriptors(const Open3DMotion::FileFormatOptions& options, const char* suffix, bool enforcestartoffset)
{
	// make save name
	std::string savename = 
		std::string("Open3DMotionTest/Data/Temp/Walk4_rewrite_") + 
		std::string(suffix);

	// open and re-write to specified format
	try
	{
		// read in
		std::auto_ptr<Open3DMotion::TreeValue> trial_tree( handler.Read("Open3DMotionTest/Data/MDF/gait-clusters-multicoda-Kistlerx2.mdf") );

		// make writing options
		std::auto_ptr<Open3DMotion::TreeValue> format_value( options.ToTree() );

		// write
		handler.Write(savename.c_str(), trial_tree.get(), format_value.get());
	}
	catch(const Open3DMotion::MotionFileException& error)
	{
		CPPUNIT_FAIL(error.message);
	}

	// verify data from re-load
	testWalk4DescriptorsInFile(handler, savename.c_str(), enforcestartoffset);
}

void TestMDF::testReadWheelMDF2()
{
	// Verify that legacy MDF2 format reads without error
	try
	{
		// read in
		std::auto_ptr<Open3DMotion::TreeValue> trial_tree( handler.Read("Open3DMotionTest/Data/MDF/Wheel.mdf") );

	}
	catch(const Open3DMotion::MotionFileException& error)
	{
		CPPUNIT_FAIL(error.message);
	}
}