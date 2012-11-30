/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/MotionFile/Formats/C3D/MapC3DParameters.h"
#include <cppunit/extensions/HelperMacros.h>

using namespace Open3DMotion;
using namespace std;

class TestMapC3DParameters  : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestMapC3DParameters );
	CPPUNIT_TEST( testSetGet );
	CPPUNIT_TEST( testWriteReadFile );
	CPPUNIT_TEST( testToFromTree );
	CPPUNIT_TEST( testMerge );
	CPPUNIT_TEST_SUITE_END();


protected:
	void MakeTestData(MapC3DParameters& p)
	{
		p.AddRecord(new C3DRecordGroup(3, "STUFF", "Interesting Things"));
		p.AddRecord(new C3DRecordInteger(3, "NUMBER", "Meaning of everything", 42));
		p.AddRecord(new C3DRecordByte(3, "NOTHINGMUCH", "What is this?", -89));
		p.AddRecord(new C3DRecordReal(3, "PI", "Just an approximation", 3.1f));
		p.AddRecord(new C3DRecordText(3, "COMMENT", "Comments Field", "All ok?"));

		p.AddRecord(new C3DRecordGroup(41, "MORESTUFF", "Interesting Things"));
		p.AddRecord(new C3DRecordReal(41, "PI", "Still only approximate", 3.1415926f));

		vector<Int16> integers;
		integers.push_back(5);
		integers.push_back(6);
		integers.push_back(7);
		integers.push_back(8);
		integers.push_back(9);
		integers.push_back(10);

		vector<float> numbers;
		numbers.push_back(1.1f);
		numbers.push_back(2.3f);

		vector<string> text;
		text.push_back("one");
		text.push_back("two");
		text.push_back("three");

		vector<UInt32> twodeerange;
		twodeerange.push_back(2L);
		twodeerange.push_back(3L);

		C3DRecordInteger* lockeditem = new C3DRecordInteger(52, "SECURE", "Should be locked", 33);
		lockeditem->Locked() = true;

		p.AddRecord(new C3DRecordGroup(52, "LASTGROUP", "With big stuff in it"));
		p.AddRecord(new C3DRecordReal(52, "SOMENUMBERS", "Should be three", numbers));
		p.AddRecord(new C3DRecordInteger(52, "MYINTEGERS", "Should be 1D", integers));
		p.AddRecord(lockeditem);
		p.AddRecord(new C3DRecordText(52, "SOMETEXT", "Learning to count", text));
		p.AddRecord(new C3DRecordInteger(52, "TWODEE", "Should be 2D", integers, twodeerange));
	}

	void VerifyTestData(const MapC3DParameters& p)
	{
		CPPUNIT_ASSERT_EQUAL(Int16(42), p.GetInteger("STUFF", "NUMBER"));			
		CPPUNIT_ASSERT_EQUAL(char(-89), p.GetByte("STUFF", "NOTHINGMUCH"));
		CPPUNIT_ASSERT_DOUBLES_EQUAL(3.1f, p.GetReal("STUFF", "PI"), 1E-12f);
		string commentstring;
		CPPUNIT_ASSERT( p.GetText(commentstring, "STUFF", "COMMENT") );
		CPPUNIT_ASSERT_EQUAL(string("All ok?"), commentstring);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(3.1415926f, p.GetReal("MORESTUFF", "PI"), 1E-12f);

		// numeric arrays
		CPPUNIT_ASSERT_EQUAL(Int16(5), p.GetInteger("LASTGROUP", "MYINTEGERS", 0, 0UL));
		CPPUNIT_ASSERT_EQUAL(Int16(6), p.GetInteger("LASTGROUP", "MYINTEGERS", 0, 1UL));
		CPPUNIT_ASSERT_EQUAL(Int16(10), p.GetInteger("LASTGROUP", "MYINTEGERS", 0, 5UL));
		CPPUNIT_ASSERT_EQUAL(Int16(5555), p.GetInteger("LASTGROUP", "MYINTEGERS", 5555, 6UL)); // should fail
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.1f, p.GetReal("LASTGROUP", "SOMENUMBERS", 0.0f, 0UL), 1E-12f);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(2.3f, p.GetReal("LASTGROUP", "SOMENUMBERS", 0.0f, 1UL), 1E-12f);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(999.999f, p.GetReal("LASTGROUP", "SOMENUMBERS", 999.999f, 2UL), 1E-12f);
		CPPUNIT_ASSERT_EQUAL(Int16(5), p.GetInteger("LASTGROUP", "TWODEE", 0, 0UL));
		CPPUNIT_ASSERT_EQUAL(Int16(6), p.GetInteger("LASTGROUP", "TWODEE", 0, 1UL));
		CPPUNIT_ASSERT_EQUAL(Int16(7), p.GetInteger("LASTGROUP", "TWODEE", 0, 2UL));
		CPPUNIT_ASSERT_EQUAL(Int16(8), p.GetInteger("LASTGROUP", "TWODEE", 0, 3UL));
		CPPUNIT_ASSERT_EQUAL(Int16(9), p.GetInteger("LASTGROUP", "TWODEE", 0, 4UL));
		CPPUNIT_ASSERT_EQUAL(Int16(10), p.GetInteger("LASTGROUP", "TWODEE", 0, 5UL));
		CPPUNIT_ASSERT_EQUAL(size_t(2), p.GetRecordType<C3DRecordInteger>("LASTGROUP", "TWODEE")->Range().size());
		CPPUNIT_ASSERT_EQUAL(UInt32(2), p.GetRecordType<C3DRecordInteger>("LASTGROUP", "TWODEE")->Range()[0]);
		CPPUNIT_ASSERT_EQUAL(UInt32(3), p.GetRecordType<C3DRecordInteger>("LASTGROUP", "TWODEE")->Range()[1]);

		// string arrays
		string s0, s1, s2, s3;
		CPPUNIT_ASSERT( p.GetText(s0, "LASTGROUP", "SOMETEXT", 0UL) );
		CPPUNIT_ASSERT( p.GetText(s1, "LASTGROUP", "SOMETEXT", 1UL) );
		CPPUNIT_ASSERT( p.GetText(s2, "LASTGROUP", "SOMETEXT", 2UL) );
		CPPUNIT_ASSERT( p.GetText(s3, "LASTGROUP", "SOMETEXT", 3UL) == false);
		CPPUNIT_ASSERT_EQUAL(string("one"), s0);
		CPPUNIT_ASSERT_EQUAL(string("two"), s1);
		CPPUNIT_ASSERT_EQUAL(string("three"), s2);
		CPPUNIT_ASSERT_EQUAL(string(""), s3);

		// verify non-lock
		const C3DRecordReal* unlockeditem = p.GetRecordType<C3DRecordReal>("MORESTUFF", "PI");
		CPPUNIT_ASSERT(unlockeditem != NULL);
		CPPUNIT_ASSERT( unlockeditem->Locked() == false );

		// verify lock
		const C3DRecordInteger* secureitem = p.GetRecordType<C3DRecordInteger>("LASTGROUP", "SECURE");
		CPPUNIT_ASSERT(secureitem != NULL);
		CPPUNIT_ASSERT( secureitem->Locked() );
		CPPUNIT_ASSERT_EQUAL(size_t(1), secureitem->Data().size() );
		CPPUNIT_ASSERT_EQUAL(Int16(33), secureitem->Data()[0] );

		// check descriptions
		CPPUNIT_ASSERT_EQUAL(string("Interesting Things"), p.GetRecord("STUFF", "STUFF")->Description());
		CPPUNIT_ASSERT_EQUAL(string("What is this?"), p.GetRecord("STUFF", "NOTHINGMUCH")->Description());
	}

public:

	void testSetGet()
	{
		MapC3DParameters p;
		MakeTestData(p);
		VerifyTestData(p);
	}

	void testWriteReadFile()
	{
		string testfile = string("Open3DMotionTest/Data/Temp/TestC3DParameters.testWriteReadFile.bin");
		C3DMachine_PC machine;

		// create test file
		{
			MapC3DParameters p;
			MakeTestData(p);
			size_t expected_size = p.FileBytes();
			ofstream os(testfile.c_str(), ios::binary);
			p.ToFile(machine, os);

			// use this chance to verify that size estimate works ok
			CPPUNIT_ASSERT_EQUAL((size_t)os.tellp(), expected_size);
			
			os.close();
		}

		// read and verify the test file
		{
			ifstream is(testfile.c_str(), ios::binary);
			MapC3DParameters p;
			p.FromFile(machine, is);
			is.close();
			VerifyTestData(p);
		}
	}

	void testToFromTree()
	{
		TreeValue* t(NULL);
		{
			MapC3DParameters p;
			MakeTestData(p);
			t = p.ToTree();
		}

		{
			MapC3DParameters p;
			p.FromTree(t);
			VerifyTestData(p);
		}
	}

	void testMerge()
	{
		// starting params
		MapC3DParameters p;
		p.AddRecord(new C3DRecordGroup(3, "STUFF", "Interesting Things"));
		p.AddRecord(new C3DRecordInteger(3, "NUMBER", "Meaning of everything", 42));
		p.AddRecord(new C3DRecordInteger(3, "UNAFFECTED", "Minus one", -1));
		CPPUNIT_ASSERT_EQUAL(size_t(3), p.NumRecords());

		// params to merge into starting ones
		MapC3DParameters m;
		m.AddRecord(new C3DRecordGroup(27, "STUFF", "Interesting Things"));			 // should get mapped to existing group
		m.AddRecord(new C3DRecordInteger(27, "NUMBER", "Change meaning", 43));	 // should not affect first
		m.AddRecord(new C3DRecordInteger(27, "DONTLIKETHIS", "Will remove it", -777)); // will remove so should not merge
		m.AddRecord(new C3DRecordInteger(27, "ANOTHERNUMBER", "New stuff", 57)); // should get merged into first
		m.AddRecord(new C3DRecordGroup(122, "DIFFERENTGROUP", "Interesting Things")); // should get created
		m.AddRecord(new C3DRecordInteger(122, "NUMBER", "Change meaning", -3)); // should get created as separate item

		// check that removal of items prior to merge works ok
		CPPUNIT_ASSERT_EQUAL(size_t(6), m.NumRecords());
		CPPUNIT_ASSERT_EQUAL(Int16(-777), m.GetInteger("STUFF", "DONTLIKETHIS")); 
		m.RemoveRecord("STUFF", "DONTLIKETHIS");
		CPPUNIT_ASSERT_EQUAL(size_t(5), m.NumRecords());
		CPPUNIT_ASSERT_EQUAL(Int16(0), m.GetInteger("STUFF", "DONTLIKETHIS")); 

		// do merge
		p.MergeFrom(m);

		// check results
		CPPUNIT_ASSERT_EQUAL(size_t(6), p.NumRecords());
		CPPUNIT_ASSERT_EQUAL(Int16(42), p.GetInteger("STUFF", "NUMBER"));	// unchanged
		CPPUNIT_ASSERT_EQUAL(Int16(-1), p.GetInteger("STUFF", "UNAFFECTED"));	// unchanged
		CPPUNIT_ASSERT_EQUAL(Int16(57), p.GetInteger("STUFF", "ANOTHERNUMBER"));	// added ok
		CPPUNIT_ASSERT_EQUAL(Int16(-3), p.GetInteger("DIFFERENTGROUP", "NUMBER"));	// added ok with new group
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestMapC3DParameters );
