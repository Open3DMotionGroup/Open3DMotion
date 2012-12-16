/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/MotionFile/MotionFileFormatList.h"
#include "Open3DMotion/MotionFile/MotionFileFormat.h"
#include <cppunit/extensions/HelperMacros.h>

using namespace Open3DMotion;

class TestMotionFileFormatList  : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestMotionFileFormatList );
	CPPUNIT_TEST( testEmpty );
	CPPUNIT_TEST( testAddTwo );
	CPPUNIT_TEST_SUITE_END();

public:

	// A mock format for testing with ref count for testing deletion
	class CrazyFormat : public MotionFileFormat
	{
	public:
		CrazyFormat(int* _refcount) :
				MotionFileFormat("Crazy", "My crazy format", "czy"),
				refcount(_refcount)
		{
			(*refcount)++;
		}

		~CrazyFormat()
		{
			(*refcount)--;
		}

	public:
    virtual bool Probe(const MotionFileHandler& context, TreeValue*& readoptions, std::istream& is) const throw(MotionFileException)
		{ return false; }

    virtual TreeValue* Read(const MotionFileHandler& context, std::istream& is, BinMemFactory& memfactory, const TreeValue* readoptions) const throw(MotionFileException)
		{ throw MotionFileException(MotionFileException::libraryerror, "not implemented"); }

    virtual void Write(const MotionFileHandler& context, const TreeValue* contents, std::ostream& os, const TreeValue* writeoptions) const throw(MotionFileException)
		{ throw MotionFileException(MotionFileException::libraryerror, "not implemented"); }

	private:
		int* refcount;
	};

	// Another mock format for testing with ref count for testing deletion
	class InterestingFormat : public MotionFileFormat
	{
	public:
		InterestingFormat(int* _refcount) :
				MotionFileFormat("Interesting", "Some interesting format", "intr"),
				refcount(_refcount)
		{
			(*refcount)++;
		}

		~InterestingFormat()
		{
			(*refcount)--;
		}

	public:
    virtual bool Probe(const MotionFileHandler& context, TreeValue*& readoptions, std::istream& is) const throw(MotionFileException)
		{ return false; }

    virtual TreeValue* Read(const MotionFileHandler& context, std::istream& is, BinMemFactory& memfactory, const TreeValue* readoptions) const throw(MotionFileException)
		{ throw MotionFileException(MotionFileException::libraryerror, "not implemented"); }

    virtual void Write(const MotionFileHandler& context, const TreeValue* contents, std::ostream& os, const TreeValue* writeoptions) const throw(MotionFileException)
		{ throw MotionFileException(MotionFileException::libraryerror, "not implemented"); }

	private:
		int* refcount;
	};

	void testEmpty()
	{
		MotionFileFormatList f;
		CPPUNIT_ASSERT(f.DefaultFormat() == NULL);
		CPPUNIT_ASSERT(f.Begin() == f.End());
		CPPUNIT_ASSERT(f.Find("SomeFormat") == NULL);
	}

	void testAddTwo()
	{
		int rc1(0), rc2(0);

		{
			MotionFileFormatList f;

			// add one format
			f.Register(new InterestingFormat(&rc1));
			CPPUNIT_ASSERT_EQUAL(1, rc1);
			CPPUNIT_ASSERT_EQUAL(0, rc2);
			MotionFileFormat* testfind = f.Find("Interesting");
			CPPUNIT_ASSERT(testfind != NULL);
			CPPUNIT_ASSERT_EQUAL(std::string("Interesting"), std::string(testfind->FormatID()));
			CPPUNIT_ASSERT(f.Find("Something else") == NULL);
			CPPUNIT_ASSERT(f.Find("Crazy") == NULL);
			CPPUNIT_ASSERT(testfind == f.DefaultFormat());
			MotionFileFormatList::ConstIterator iter = f.Begin();
			CPPUNIT_ASSERT(iter != f.End());
			CPPUNIT_ASSERT(iter.Format() == testfind);
			iter++;
			CPPUNIT_ASSERT(iter == f.End());

			// add another format
			f.Register(new CrazyFormat(&rc2));
			CPPUNIT_ASSERT_EQUAL(1, rc1);
			CPPUNIT_ASSERT_EQUAL(1, rc2);
			MotionFileFormat* testfind2 = f.Find("Crazy");
			CPPUNIT_ASSERT(testfind2 != NULL);
			CPPUNIT_ASSERT_EQUAL(std::string("Crazy"), std::string(testfind2->FormatID()));
	
			// iterate and add to names list - note not guaranteed in order they were added
			std::list<std::string> names;
			iter = f.Begin();
			CPPUNIT_ASSERT(iter != f.End());
			names.push_back(iter.Format()->FormatID());
			iter++;
			CPPUNIT_ASSERT(iter != f.End());
			names.push_back(iter.Format()->FormatID());
			iter++;
			CPPUNIT_ASSERT(iter == f.End());
			CPPUNIT_ASSERT(testfind == f.DefaultFormat());

			names.sort();	// sort alphabetical
			CPPUNIT_ASSERT_EQUAL(size_t(2), names.size());
			CPPUNIT_ASSERT_EQUAL(std::string("Crazy"), names.front());
			CPPUNIT_ASSERT_EQUAL(std::string("Interesting"), names.back());
		}

		// should be destroyed now
		CPPUNIT_ASSERT_EQUAL(0, rc1);
		CPPUNIT_ASSERT_EQUAL(0, rc2);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestMotionFileFormatList );
