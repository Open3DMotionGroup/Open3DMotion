/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include <cppunit/extensions/HelperMacros.h>
#include <Python.h>
// #include "NumPy/ArrayObject.h"
#include "PythonTotalRefCount.h"

class TestPythonConnection : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestPythonConnection );
	CPPUNIT_TEST( testConnectDisconnect );
	CPPUNIT_TEST( testGetTotalRefCount );
	CPPUNIT_TEST( testReturnNumber );
	CPPUNIT_TEST_SUITE_END();

public:

	void testConnectDisconnect()
	{
		Py_Initialize();
		PyObject* py_number = PyLong_FromLong(42L);
		Py_DECREF(py_number);
		Py_Finalize();
	}

	void testGetTotalRefCount()
	{
		Py_Initialize();
		size_t mem0 = PythonTotalRefCount();
#ifdef Py_DEBUG
		PyObject* py_number = PyLong_FromLong(32);
		size_t mem1 = PythonTotalRefCount();
		CPPUNIT_ASSERT(mem1 > mem0);
		Py_DECREF(py_number);
#else
		#define REFCOUNT_DISABLE_WARNING_STRING "TestPythonConnection WARNING: checks for Python memory leaks not available in release build."
	  #pragma message(REFCOUNT_DISABLE_WARNING_STRING)
		fprintf(stderr, REFCOUNT_DISABLE_WARNING_STRING "\n"); 
#endif
		size_t mem2 = PythonTotalRefCount();
		CPPUNIT_ASSERT(mem2 == mem0);
		Py_Finalize();
	}

	void testReturnNumber()
	{
		const char* codestring = 
			"def get_my_number_please():\n" \
			"  return 42\n" \
			"\n";

		// connect
		Py_Initialize();


		// compile the code
		PyObject* compiledcode = Py_CompileString(codestring, "testmodule", Py_file_input);
		CPPUNIT_ASSERT(compiledcode != NULL);

		// import the compiled code
		PyObject* codemodule = PyImport_ExecCodeModule("testmodule", compiledcode);
		CPPUNIT_ASSERT(codemodule != NULL);

		// get initial reference count when memory diagnostics enabled, after module created
		// because modules can't be de-allocated
		size_t refcount0 = PythonTotalRefCount();

		// get pointer to function within code
		PyObject* function_pointer = PyObject_GetAttrString(codemodule, "get_my_number_please");
		CPPUNIT_ASSERT(function_pointer != NULL);

		// call it and get result
		PyObject* py_output = PyObject_CallObject(function_pointer, NULL);

		// check result
		long result = PyLong_AsLong(py_output);
		CPPUNIT_ASSERT_EQUAL(42L, result);

		// done with value and function
		Py_DECREF(py_output);
		Py_DECREF(function_pointer);

		size_t refcount1 = PythonTotalRefCount();
		CPPUNIT_ASSERT(refcount1 == refcount0);

		Py_DECREF(codemodule);
		Py_DECREF(compiledcode);

		Py_Finalize();
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION( TestPythonConnection );

