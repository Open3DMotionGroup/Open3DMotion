/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/MotionBundle/MotionBundleHandler.h"
#include "Open3DMotion/MotionBundle/MOBL.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinMemFactoryDefault.h"
#include "Open3DMotion/Biomechanics/Trial/TSFactory.h"
#include "Open3DMotion/Biomechanics/Trial/Trial.h"
#include "Open3DMotion/MotionFile/MotionFileHandler.h"
#include "Open3DMotion/MotionFile/MotionFileFormat.h"
#include "Open3DMotion/MotionFile/Formats/XMove/FileFormatOptionsXMove.h"
#include "Open3DMotionTest/MotionFile/testADemo1File.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONInputStreamSTL.h"

#include <cppunit/extensions/HelperMacros.h>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "MotionFileTest.h"

using namespace Open3DMotion;

/* Summary
   Unit test fixture for motion bundle IO.
                                                                 */
class TestMotionBundle : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE( TestMotionBundle );
	CPPUNIT_TEST( testLoad );
	CPPUNIT_TEST( testReadADemo1_ODIN_MOBL );
	CPPUNIT_TEST_SUITE_END();

public:

	TestMotionBundle()
	{
	}

public:

	void testLoad()
	{
    // const char* filename = "Open3DMotionTest/Data/MOBL/ReachingTaskExample-20111013.mobl";
    o3dm_test_construct_global_filename(filename, "Open3DMotionTest/Data/MOBL/ReachingTaskExample-20111013.mobl");

		try
		{
			MotionBundleHandler bundlehandler;
			std::auto_ptr<MOBLFormatReader> formatreader( bundlehandler.Open(filename) );
			CPPUNIT_ASSERT(formatreader.get() != NULL);

			UInt32 num_trials = formatreader->TrialCount();
			CPPUNIT_ASSERT_EQUAL(UInt32(7), num_trials);

			for (UInt32 index = 0; index < 7UL; index++)
			{
				std::auto_ptr<TreeValue> tree( formatreader->ReadTrial(index) );
				CPPUNIT_ASSERT(tree.get() != NULL);

				Trial t;
				t.FromTree(tree.get());

				MotionFileHandler filehandler("TestMotionBundle", "UNVERSIONED");
				FileFormatOptionsXMove xmove_options;
				std::auto_ptr<TreeValue> xmove_options_tree( xmove_options.ToTree() );
				std::ostringstream dbg_filename;
				dbg_filename << "Open3DMotionTest/Data/Temp/TestMotionBundle.testLoad" << std::setfill('0') << std::setw(2) << index << ".xml";
				filehandler.Write(dbg_filename.str().c_str(), tree.get(), xmove_options_tree.get());

			}
		}
		catch(const MotionFileException& e)
		{
			CPPUNIT_FAIL(e.message);
		}
	}

	void testReadADemo1_ODIN_MOBL()
	{ 
		try
		{
		  o3dm_test_construct_global_filename(filename, "Open3DMotionTest/Data/MOBL/ADemo1.ODIN.mobl");
		  
			// open bundle
			MotionBundleHandler bundlehandler;
			std::auto_ptr<MOBLFormatReader> formatreader( bundlehandler.Open(filename) );
			CPPUNIT_ASSERT(formatreader.get() != NULL);

			// should have one trial
			UInt32 num_trials = formatreader->TrialCount();
			CPPUNIT_ASSERT_EQUAL(UInt32(1), num_trials);

			// load it
			std::auto_ptr<TreeValue> tree( formatreader->ReadTrial(0) );
			
			// store as XMove
      // const char* tempfilename = "Open3DMotionTest/Data/Temp/TestMotionBundle.ADemo1.ODIN.xml";
      o3dm_test_construct_global_filename(tempfilename, "Open3DMotionTest/Data/Temp/TestMotionBundle.ADemo1.ODIN.xml");
			Open3DMotion::MotionFileHandler filehandler("TestMotionBundle", "UNVESIONED");
			FileFormatOptionsXMove xmove_options;
			std::auto_ptr<TreeValue> xmove_options_tree( xmove_options.ToTree() );
			filehandler.Write(tempfilename, tree.get(), xmove_options_tree.get());

			// reload from XMove and check results
			testADemo1File(filehandler, tempfilename, true, 1, true);
		}
		catch (const Open3DMotion::MotionFileException& e)
		{
			CPPUNIT_FAIL(e.message);
		}
	}


};

CPPUNIT_TEST_SUITE_REGISTRATION( TestMotionBundle );
