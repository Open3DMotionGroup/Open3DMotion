/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include <cppunit/extensions/HelperMacros.h>
#include <Python.h>
#include "PythonTotalRefCount.h"
#include "Open3DMotion/Bindings/Python/BinMemFactoryPython.h"
#include "Open3DMotion/MotionBundle/MotionBundleHandler.h"
#include "Open3DMotion/MotionBundle/MOBL.h"
#include "Open3DMotion/Biomechanics/Trial/Trial.h"

using namespace Open3DMotion;

class TestPythonMOBLRead : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestPythonMOBLRead );
	CPPUNIT_TEST( testRead );
	CPPUNIT_TEST_SUITE_END();

public:

	void testRead()
	{
		Py_Initialize();
		size_t refcount0 = PythonTotalRefCount();

		MotionBundleHandler bundlehandler;
		BinMemFactoryPython memfactory;
		std::auto_ptr<MOBLFormatReader> formatreader( bundlehandler.Open("Open3DMotionTest/Data/MOBL/ReachingTaskExample-20111013.mobl") );
		CPPUNIT_ASSERT(formatreader.get() != NULL);

		{
			// first trial
			std::auto_ptr<TreeValue> tree( formatreader->ReadTrial(0, memfactory) );

			// look at as trial and check time sequence is of Python type
			Trial trial;
			trial.FromTree(tree.get());
			CPPUNIT_ASSERT(trial.Acq.TimeSequences.NumElements() > 0);
			const TimeSequence& ts = trial.Acq.TimeSequences[0];
			std::auto_ptr<TreeValue> ts_tree( ts.ToTree() );
			const TreeCompound* ts_compound = TreeValueCast<TreeCompound> (ts_tree.get());
			CPPUNIT_ASSERT(ts_compound != NULL);
			const TreeBinary* bin = TreeValueCast<TreeBinary>(ts_compound->Get(MEMBER_NAME(RichBinary::Data)));
			const MemoryHandlerPython* mem_py = NamedClassCast<MemoryHandler, MemoryHandlerPython> ( bin->BinMemory() );
			CPPUNIT_ASSERT(mem_py != NULL);
		}

		size_t refcount1 = PythonTotalRefCount();
		CPPUNIT_ASSERT(refcount1 == refcount0);

		Py_Finalize();
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION( TestPythonMOBLRead );

