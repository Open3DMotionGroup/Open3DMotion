/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

/* TestC3D.cpp
   
	 Summary
	 Unit tests for C3D files

 */

#include "Open3DMotion/MotionFile/MotionFileHandler.h"
#include "Open3DMotion/MotionFile/Formats/C3D/FileFormatOptionsC3D.h"
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
		std::auto_ptr<TreeValue> trialcontents( handler.Read("Open3DMotionTest/Data/C3D/sample08/EB015PI.c3d") );

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


