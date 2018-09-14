/*--
Open3DMotion
Copyright (c) 2004-2018.
All rights reserved.
See LICENSE.txt for more information.
--*/

#include "Open3DMotion/OpenORM/IO/BSON/BSONInputStreamGZ.h"
#include "Open3DMotion/OpenORM/IO/BSON/BSONOutputStreamGZ.h"

#include <cppunit/extensions/HelperMacros.h>
#include <istream>

using namespace Open3DMotion;

/* Summary
	Test for writing and re-reading compressed BSON.
*/
class TestBSONStreamGZ : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE(TestBSONStreamGZ);
	CPPUNIT_TEST(testWriteRead);
	CPPUNIT_TEST_SUITE_END();

public:

	void testWriteRead()
	{
		try
		{
			std::stringstream example(std::ios::binary | std::ios::out | std::ios::in);
			{
				// Should generate 2 MB of data
				BSONOutputStreamGZ writer(example);
				for (Int32 index = 0; index < (1024 * 128); index++)
				{
					Int32 cyclictestdata[4] = { (index + 996) % 997, (index + 58) % 59, (index + 22) % 23, (index + 442) % 443 };
					writer.WriteBinary(cyclictestdata, sizeof(cyclictestdata));
				}
			}

			// But compressed amount should be loads less
			// (just as sanity check make sure it's less than 1 MB)
			CPPUNIT_ASSERT(example.tellp() > 0);
			CPPUNIT_ASSERT(example.tellp() < (1024 * 1024));

			// Check figures
			example.seekp(0, std::ios::beg);
			BSONInputStreamGZ reader(example);
			for (Int32 index = 0; index < (1024 * 128); index++)
			{
				/* if ((index + 1) % 883 == 0)
				{
					// Do some skips every so often, just to see that we can
					reader.SkipBytes(16);
				}
				else */
				{
					Int32 result[4] = { 0, 0, 0, 0 };
					reader.ReadBinary(result, 16);
					CPPUNIT_ASSERT_EQUAL((index + 996) % 997, result[0]);
					CPPUNIT_ASSERT_EQUAL((index +  58) %  59, result[1]);
					CPPUNIT_ASSERT_EQUAL((index +  22) %  23, result[2]);
					CPPUNIT_ASSERT_EQUAL((index + 442) % 443, result[3]);
				}
			}
		}
		catch (const BSONWriteException& ex)
		{
			CPPUNIT_FAIL(ex.message);
		}
		catch (const BSONReadException& ex)
		{
			CPPUNIT_FAIL(ex.message);
		}
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestBSONStreamGZ);
