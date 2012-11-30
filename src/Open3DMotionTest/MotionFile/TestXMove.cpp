/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/


#include "Open3DMotion/MotionFile/MotionFileHandler.h"
#include "Open3DMotion/MotionFile/Formats/XMove/FileFormatOptionsXMove.h"
#include "Open3DMotion/Biomechanics/Trial/TSFactory.h"
#include "Open3DMotion/OpenOrm/Mappings/RichBinary/BinMemFactoryDefault.h"
#include <cppunit/extensions/HelperMacros.h>


/* Summary
   Unit test fixture for XMove (XML) conversion to/from tree.
                                                                 */
class TestXMove : public CppUnit::TestFixture
{
protected:
	Open3DMotion::MotionFileHandler handler;

public:
	CPPUNIT_TEST_SUITE( TestXMove );
	CPPUNIT_TEST( testReadWriteEmptyXMove );
	CPPUNIT_TEST( testReadWriteSimpleXMove );
	CPPUNIT_TEST_SUITE_END();

public:

	TestXMove() :
			handler("TestXMove", "UNVESIONED")
	{
	}

public:

	void testReadWriteEmptyXMove()
	{
		Open3DMotion::Trial empty_trial;
		Open3DMotion::FileFormatOptionsXMove options;

		Open3DMotion::TreeValue* empty_trial_tree = empty_trial.ToTree();
		Open3DMotion::TreeValue* options_tree = options.ToTree();
		Open3DMotion::TreeCompound* read_empty_tree(NULL);

		try
		{
			// write
			handler.Write("Open3DMotionTest/Data/Temp/testReadWriteEmptyXMove.xmove", empty_trial_tree, options_tree);

			// read
			read_empty_tree = Open3DMotion::TreeValueCast<Open3DMotion::TreeCompound>( handler.Read("Open3DMotionTest/Data/Temp/testReadWriteEmptyXMove.xmove") );
		}
		catch(const Open3DMotion::MotionFileException& error)
		{
			CPPUNIT_FAIL(error.message);
		}

		// we expect acq section and nothing more
		CPPUNIT_ASSERT(read_empty_tree != NULL);
		CPPUNIT_ASSERT_EQUAL(size_t(1), read_empty_tree->NumElements());
		CPPUNIT_ASSERT_EQUAL(std::string("Acq"), read_empty_tree->Node(0)->Name());
		
		// section itself should be empty
		const Open3DMotion::TreeCompound* acq = Open3DMotion::TreeValueCast<Open3DMotion::TreeCompound> ( read_empty_tree->Node(0)->Value() );
		CPPUNIT_ASSERT(acq != NULL);
		CPPUNIT_ASSERT(acq->NumElements() == 0);

		delete empty_trial_tree;
		delete options_tree;
		delete read_empty_tree;

	}

	void testReadWriteSimpleXMove()
	{
		Open3DMotion::Trial simple_trial;
		
		Open3DMotion::TimeRange t;
		t.Rate = 200.0;
		t.Frames = 67;
		t.Start = 0.0;
        Open3DMotion::BinMemFactoryDefault memfactory;
		Open3DMotion::TimeSequence* ts = Open3DMotion::TSFactoryOccValue(3).New(t, memfactory);
		simple_trial.Acq.TimeSequences.Add(*ts);
		
		Open3DMotion::FileFormatOptionsXMove options;

		Open3DMotion::TreeValue* simple_trial_tree = simple_trial.ToTree();
		Open3DMotion::TreeValue* options_tree = options.ToTree();
		Open3DMotion::TreeValue* read_simple_tree(NULL);

		try
		{
			// write
			handler.Write("Open3DMotionTest/Data/Temp/testReadWriteSimpleXMove.xml", simple_trial_tree, options_tree);

			// read
			read_simple_tree = handler.Read("Open3DMotionTest/Data/Temp/testReadWriteSimpleXMove.xml");
		}
		catch(const Open3DMotion::MotionFileException& error)
		{
			CPPUNIT_FAIL(error.message);
		}

		Open3DMotion::Trial* read_simple_trial = new Open3DMotion::Trial;
		read_simple_trial->FromTree(read_simple_tree);

		delete ts;
		delete simple_trial_tree;
		delete options_tree;
		delete read_simple_tree;
		delete read_simple_trial;
	}


};

CPPUNIT_TEST_SUITE_REGISTRATION( TestXMove );
