/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestListener.h>
#include <cppunit/TestResult.h>

/* TestNamePrinter
	
	 Summary
	 Write unit test names to Output window during testing to indicate progress
	*/
class TestNamePrinter : public CppUnit::TestListener
{
	virtual void startTest(CppUnit::Test* test) 
	{
		std::cerr << test->getName() << std::endl;
	}
};

int main(int argc, char** argv)
{
	const char* command = (argc == 2) ? argv[1] : "";
	CppUnit::TextUi::TestRunner runner;
	CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry();
	runner.addTest(registry.makeTest());
	runner.eventManager().addListener(new TestNamePrinter());
	runner.setOutputter(CppUnit::CompilerOutputter::defaultOutputter(&runner.result(), std::cerr));
	bool success = runner.run(command, false, true, false);
	return success ? 0 : 1;
}
