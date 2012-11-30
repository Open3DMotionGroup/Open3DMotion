/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/


#include "Open3DMotion/OpenORM/Leaves/TreeFloat64.h"
#include "Open3DMotion/OpenORM/Leaves/TreeInt32.h"
#include "Open3DMotion/OpenORM/Leaves/TreeString.h"
#include "Open3DMotion/OpenORM/Leaves/TreeBinary.h"
#include "Open3DMotion/OpenORM/Leaves/TreeBool.h"
#include "Open3DMotion/OpenORM/Branches/TreeList.h"
#include "Open3DMotion/OpenORM/Leaves/MemoryHandlerBasic.h"

#include "Open3DMotion/OpenORM/IO/XML/XMLWritingMachine.h"
#include <cppunit/extensions/HelperMacros.h>
#include <strstream>


using namespace Open3DMotion;

/* Summary
   Unit test fixture for BSON conversion to/from tree.
                                                                 */
class TestXML : public CppUnit::TestFixture
{

public:
	CPPUNIT_TEST_SUITE( TestXML );
	CPPUNIT_TEST( testWriteTextNode );
	CPPUNIT_TEST( testReadWriteBinary );
	CPPUNIT_TEST_SUITE_END();

public:

	TestXML()
	{
	}

public:

	void testWriteTextNode()
	{
		std::ostrstream stream;
		XMLWritingMachine writer(stream);
		writer.WriteTextNode("A string with \'a quote & stuff < >\'");
		stream << std::ends;
		stream.freeze();
		std::string result( stream.str() );
		std::string expected("A string with \'a quote &amp; stuff &lt; &gt;\'");
		CPPUNIT_ASSERT_EQUAL(expected, result);
	}

	void testReadWriteBinary()
	{
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestXML );
