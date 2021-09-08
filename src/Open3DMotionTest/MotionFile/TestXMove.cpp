/*--
  Open3DMotion 
  Copyright (c) 2004-2021.
  All rights reserved.
  See LICENSE.txt for more information.
--*/


#include "Open3DMotion/MotionFile/MotionFileHandler.h"
#include "Open3DMotion/MotionFile/Formats/XMove/FileFormatOptionsXMove.h"
#include "Open3DMotion/MotionFile/Formats/XMove/FileFormatDescriptor.h"
#include "Open3DMotion/Biomechanics/Trial/TSFactory.h"
#include "Open3DMotion/OpenOrm/Mappings/RichBinary/BinMemFactoryDefault.h"
#include "testADemo1File.h"
#include <cppunit/extensions/HelperMacros.h>
#include <pugixml.hpp>

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
	CPPUNIT_TEST( testReadWriteEmptyEventGroup );
	CPPUNIT_TEST( testADemo1Open3DMotion );
	CPPUNIT_TEST( testADemo1ODINLegacy );
	CPPUNIT_TEST( testODINLegacyReWrite );
	CPPUNIT_TEST( testWriteExtended );
	CPPUNIT_TEST_SUITE_END();

public:

	TestXMove() :
			handler("TestXMove", "UNVERSIONED")
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

		// we expect format section, acq section, and nothing more
		CPPUNIT_ASSERT(read_empty_tree != NULL);
		CPPUNIT_ASSERT_EQUAL(size_t(2), read_empty_tree->NumElements());
		CPPUNIT_ASSERT_EQUAL(std::string("FileFormat"), read_empty_tree->Node(0)->Name());
		CPPUNIT_ASSERT_EQUAL(std::string("Acq"), read_empty_tree->Node(1)->Name());

		// format should match library info
		const Open3DMotion::TreeCompound* format = Open3DMotion::TreeValueCast<Open3DMotion::TreeCompound> ( read_empty_tree->Node(0)->Value() );
		CPPUNIT_ASSERT(format != NULL);
		Open3DMotion::FileFormatDescriptor desc;
		desc.FromTree(format);
		CPPUNIT_ASSERT_EQUAL(std::string("CODAmotion_xmove2"), desc.FormatID.Value());
		CPPUNIT_ASSERT_EQUAL(std::string(Open3DMotion::MotionFileHandler::LibraryName), desc.LibraryName.Value());
		CPPUNIT_ASSERT_EQUAL(std::string(Open3DMotion::MotionFileHandler::LibraryVersion), desc.LibraryVersion.Value());
		CPPUNIT_ASSERT_EQUAL(std::string("TestXMove"), desc.ProgramName.Value());
		CPPUNIT_ASSERT_EQUAL(std::string("UNVERSIONED"), desc.ProgramVersion.Value());

		// acq section itself should be empty
		const Open3DMotion::TreeCompound* acq = Open3DMotion::TreeValueCast<Open3DMotion::TreeCompound> ( read_empty_tree->Node(1)->Value() );
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

	void testReadWriteEmptyEventGroup()
	{
		try
		{
			// Names of events
			Open3DMotion::EventArray events;
			events.SetIDName(1, "EventOne");
			events.SetIDName(2, "EventTwo");

			// An event group with names established but no actual events
			Open3DMotion::EventGroup eg;
			eg.Name = "AnEmptyThing";
			eg.SetEvents(events, Open3DMotion::BinMemFactoryDefault());

			// Set the event group
			Open3DMotion::Trial trial;
			trial.UserInput.Set();
			trial.UserInput.EventGroups.Add(eg);

			// convert to tree
			std::unique_ptr<Open3DMotion::TreeValue> tree(trial.ToTree());

			// read/write options
			Open3DMotion::FileFormatOptionsXMove options;
			std::unique_ptr<Open3DMotion::TreeValue> options_tree(options.ToTree());

			// write it
			handler.Write("Open3DMotionTest/Data/Temp/testReadWriteEmptyEventGroup.xml", tree.get(), options_tree.get());
		}
		catch (const Open3DMotion::MotionFileException & error)
		{
			CPPUNIT_FAIL(error.message);
		}

		try
		{
			// read
			std::unique_ptr<Open3DMotion::TreeValue> tree(handler.Read("Open3DMotionTest/Data/Temp/testReadWriteEmptyEventGroup.xml"));

			// parse
			std::unique_ptr<Open3DMotion::Trial> trial(new Open3DMotion::Trial);
			trial->FromTree(tree.get());

			// should have one empty event group
			CPPUNIT_ASSERT_EQUAL(true, trial->UserInput.IsSet());
			CPPUNIT_ASSERT_EQUAL(size_t(1), trial->UserInput.EventGroups.NumElements());
			CPPUNIT_ASSERT_EQUAL(std::string("AnEmptyThing"), trial->UserInput.EventGroups[0].Name.Value());
			CPPUNIT_ASSERT_EQUAL(size_t(0), trial->UserInput.EventGroups[0].DataSizeBytes());
			
			// but event names should be preserved
			Open3DMotion::EventArray events;
			trial->UserInput.EventGroups[0].GetEvents(events);
			CPPUNIT_ASSERT_EQUAL(size_t(2), events.NameMap().size());
			CPPUNIT_ASSERT_EQUAL(std::string("EventOne"), events.NameMap().at(1));
			CPPUNIT_ASSERT_EQUAL(std::string("EventTwo"), events.NameMap().at(2));
		}
		catch (const Open3DMotion::MotionFileException & error)
		{
			CPPUNIT_FAIL(error.message);
		}

	}


	void testADemo1Open3DMotion()
	{
		testADemo1File(handler, "Open3DMotionTest/Data/XMove/ADemo1.Open3DMotion.xmove", true, 1, true, true, true);
	}

	void testADemo1ODINLegacy()
	{
		// verify data
		testADemo1File(handler, "Open3DMotionTest/Data/XMove/ADemo1.ODIN.Legacy.xmove", true, 1, true, true, true);
	}

	void testODINLegacyReWrite()
	{
		try
		{
			// read from MDF
			std::unique_ptr<Open3DMotion::TreeValue> tree( handler.Read("Open3DMotionTest/Data/MDF/gait-bilateral-1997-Kistlerx1.mdf") );

			// set options for write in legacy format
			Open3DMotion::FileFormatOptionsXMove options;
			options.LegacyCompoundNames = true;
			std::unique_ptr<Open3DMotion::TreeValue> options_tree( options.ToTree() );

			// write like this
			handler.Write("Open3DMotionTest/Data/Temp/TestXMove.ADemo1.ODIN.Legacy.ReWrite.xml", tree.get(), options_tree.get());

			// re-read and check
			testADemo1File(handler, "Open3DMotionTest/Data/Temp/TestXMove.ADemo1.ODIN.Legacy.ReWrite.xml", true, 1, true, true, true);
		}
		catch(const Open3DMotion::MotionFileException& error)
		{
			CPPUNIT_FAIL(error.message);
		}	
	}

	void testWriteExtended()
	{
		{
			Open3DMotion::Trial simple_trial;

			Open3DMotion::TimeRange t;
			t.Rate = 200.0;
			t.Frames = 4;
			t.Start = 0.0;
			Open3DMotion::BinMemFactoryDefault memfactory;
			Open3DMotion::TimeSequence* tsA = Open3DMotion::TSFactoryOccValue(3).New(t, memfactory);
			Open3DMotion::TimeSequence* tsB = Open3DMotion::TSFactoryValue(1).New(t, memfactory);

			Open3DMotion::TSOccVector3Iter iter(*tsA);
			iter.Value()[0] = 1.1;
			iter.Value()[1] = 2.2;
			iter.Value()[2] = 3.3;
			iter.Occluded() = 1;
			iter.Next();
			iter.Value()[0] = 4.1;
			iter.Value()[1] = 5.2;
			iter.Value()[2] = 6.3;
			iter.Occluded() = 0;
			iter.Next();
			iter.Value()[0] = 7.7;
			iter.Value()[1] = 8.8;
			iter.Value()[2] = 9.9;
			iter.Occluded() = 1;
			iter.Next();
			iter.Value()[0] = -1.1;
			iter.Value()[1] = -2.2;
			iter.Value()[2] = -3.3;
			iter.Occluded() = 0;
			iter.Next();

			Open3DMotion::TSScalarIter iterB(*tsB);
			iterB.Value() = 1.1;
			iterB.Next();
			iterB.Value() = 2.2;
			iterB.Next();
			iterB.Value() = 3.3;
			iterB.Next();
			iterB.Value() = 4.4;
			iterB.Next();

			simple_trial.Acq.TimeSequences.Add(*tsA);
			simple_trial.Acq.TimeSequences.Add(*tsB);

			// Names of events
			Open3DMotion::EventArray events;
			events.SetIDName(1, "EventOne");
			events.SetIDName(2, "EventTwo");
			events.AddEvent(8.2, 2);
			events.AddEvent(9.3, 1);
			events.AddEvent(9.9501, 2);

			Open3DMotion::EventGroup eg;
			eg.Name = "SomeEvents";
			eg.SetEvents(events, Open3DMotion::BinMemFactoryDefault());

			simple_trial.UserInput.Set();
			simple_trial.UserInput.EventGroups.Add(eg);

			Open3DMotion::FileFormatOptionsXMove options;
			options.ConvertBinaryFloat32 = false;
			options.LegacyCompoundNames = false;
			options.Extended = true;

			Open3DMotion::TreeValue* simple_trial_tree = simple_trial.ToTree();
			Open3DMotion::TreeValue* options_tree = options.ToTree();

			try
			{
				// write
				handler.Write("Open3DMotionTest/Data/Temp/testWriteExtendedXMove.xml", simple_trial_tree, options_tree);
			}
			catch (const Open3DMotion::MotionFileException & error)
			{
				CPPUNIT_FAIL(error.message);
			}

			delete tsA;
			delete tsB;
			delete simple_trial_tree;
			delete options_tree;
		}

		// read it as a block
		std::ifstream examplestream("Open3DMotionTest/Data/Temp/testWriteExtendedXMove.xml");
		pugi::xml_document doc;
		CPPUNIT_ASSERT( doc.load(examplestream) );

		pugi::xml_node ts_node = doc.first_element_by_path("xmove/Acq/Sequences/Sequence");
		CPPUNIT_ASSERT(ts_node);

		pugi::xml_node dataext_node = ts_node.child("DataExt");
		CPPUNIT_ASSERT(dataext_node);

		pugi::xml_node f = dataext_node.first_child();
		CPPUNIT_ASSERT_EQUAL(std::string("f"), std::string(f.name()));
		CPPUNIT_ASSERT_EQUAL(std::string("1.10000000"), std::string(f.attribute("x").value()));
		CPPUNIT_ASSERT_EQUAL(std::string("2.20000000"), std::string(f.attribute("y").value()));
		CPPUNIT_ASSERT_EQUAL(std::string("3.30000000"), std::string(f.attribute("z").value()));
		CPPUNIT_ASSERT_EQUAL(std::string("1"), std::string(f.attribute("o").value()));

		f = f.next_sibling();
		CPPUNIT_ASSERT_EQUAL(std::string("f"), std::string(f.name()));
		CPPUNIT_ASSERT_EQUAL(std::string("4.10000000"), std::string(f.attribute("x").value()));
		CPPUNIT_ASSERT_EQUAL(std::string("5.20000000"), std::string(f.attribute("y").value()));
		CPPUNIT_ASSERT_EQUAL(std::string("6.30000000"), std::string(f.attribute("z").value()));
		CPPUNIT_ASSERT_EQUAL(std::string("0"), std::string(f.attribute("o").value()));

		f = f.next_sibling();
		CPPUNIT_ASSERT_EQUAL(std::string("f"), std::string(f.name()));
		CPPUNIT_ASSERT_EQUAL(std::string("7.70000000"), std::string(f.attribute("x").value()));
		CPPUNIT_ASSERT_EQUAL(std::string("8.80000000"), std::string(f.attribute("y").value()));
		CPPUNIT_ASSERT_EQUAL(std::string("9.90000000"), std::string(f.attribute("z").value()));
		CPPUNIT_ASSERT_EQUAL(std::string("1"), std::string(f.attribute("o").value()));

		f = f.next_sibling();
		CPPUNIT_ASSERT_EQUAL(std::string("f"), std::string(f.name()));
		CPPUNIT_ASSERT_EQUAL(std::string("-1.10000000"), std::string(f.attribute("x").value()));
		CPPUNIT_ASSERT_EQUAL(std::string("-2.20000000"), std::string(f.attribute("y").value()));
		CPPUNIT_ASSERT_EQUAL(std::string("-3.30000000"), std::string(f.attribute("z").value()));
		CPPUNIT_ASSERT_EQUAL(std::string("0"), std::string(f.attribute("o").value()));

		CPPUNIT_ASSERT(f.next_sibling() == false);

		ts_node = ts_node.next_sibling();
		CPPUNIT_ASSERT(ts_node);

		dataext_node = ts_node.child("DataExt");
		CPPUNIT_ASSERT(dataext_node);

		f = dataext_node.first_child();
		CPPUNIT_ASSERT_EQUAL(std::string("f"), std::string(f.name()));
		CPPUNIT_ASSERT_EQUAL(std::string("1.10000000"), std::string(f.attribute("x").value()));

		f = f.next_sibling();
		CPPUNIT_ASSERT_EQUAL(std::string("f"), std::string(f.name()));
		CPPUNIT_ASSERT_EQUAL(std::string("2.20000000"), std::string(f.attribute("x").value()));

		f = f.next_sibling();
		CPPUNIT_ASSERT_EQUAL(std::string("f"), std::string(f.name()));
		CPPUNIT_ASSERT_EQUAL(std::string("3.30000000"), std::string(f.attribute("x").value()));

		f = f.next_sibling();
		CPPUNIT_ASSERT_EQUAL(std::string("f"), std::string(f.name()));
		CPPUNIT_ASSERT_EQUAL(std::string("4.40000000"), std::string(f.attribute("x").value()));

		CPPUNIT_ASSERT(f.next_sibling() == false);

		pugi::xml_node eg_node = doc.first_element_by_path("xmove/UserInput/EventGroups/EventGroup");
		CPPUNIT_ASSERT(eg_node);

		dataext_node = eg_node.child("DataExt");
		CPPUNIT_ASSERT(dataext_node);

		pugi::xml_node e = dataext_node.first_child();
		CPPUNIT_ASSERT_EQUAL(std::string("e"), std::string(e.name()));
		CPPUNIT_ASSERT_EQUAL(std::string("2"), std::string(e.attribute("f").value()));
		CPPUNIT_ASSERT_EQUAL(std::string("8.20000000"), std::string(e.attribute("t").value()));

		e = e.next_sibling();
		CPPUNIT_ASSERT_EQUAL(std::string("e"), std::string(e.name()));
		CPPUNIT_ASSERT_EQUAL(std::string("1"), std::string(e.attribute("f").value()));
		CPPUNIT_ASSERT_EQUAL(std::string("9.30000000"), std::string(e.attribute("t").value()));

		e = e.next_sibling();
		CPPUNIT_ASSERT_EQUAL(std::string("e"), std::string(e.name()));
		CPPUNIT_ASSERT_EQUAL(std::string("2"), std::string(e.attribute("f").value()));
		CPPUNIT_ASSERT_EQUAL(std::string("9.95010000"), std::string(e.attribute("t").value()));

		CPPUNIT_ASSERT(e.next_sibling() == false);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestXMove );
