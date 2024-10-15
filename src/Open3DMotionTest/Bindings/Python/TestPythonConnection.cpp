/*--
  Open3DMotion 
  Copyright (c) 2004-2024.
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
		PyObject* py_number = PyFloat_FromDouble(829.32789);
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
		// We used to use integer 42 here, but in optimisations in later versions of Python 3,
		// some integer constants persist without reference counting (presumably in case they get reused).
		// Need something less general for it to be ref-counted as we might expect.
		// Even then we need to declare it as constant at the beginning to avoid
		// the function itself touching a list of cached constants.
		const char* codestring = 
			"MY_NUMBER = 42.420042\n" \
			"\n" \
			"def get_my_number_please():\n" \
			"  return MY_NUMBER\n" \
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

		// after function retrieval
		size_t refcount1 = PythonTotalRefCount();

		// call it and get result (once)
		PyObject* py_output = PyObject_CallObject(function_pointer, NULL);

		// check result (once)
		double result = PyFloat_AsDouble(py_output);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(42.420042, result, 1E-12);

		// after value got and still here
		size_t refcount2 = PythonTotalRefCount();

		// done with value
		Py_DECREF(py_output);

		// after first value done
		size_t refcount3 = PythonTotalRefCount();

		// call it and get result (second time)
		py_output = PyObject_CallObject(function_pointer, NULL);

		// check result (second time)
		result = PyFloat_AsDouble(py_output);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(42.420042, result, 1E-12);

		// after value got and still here
		size_t refcount4 = PythonTotalRefCount();

		// done with value and function
		Py_DECREF(py_output);

		// second value done
		size_t refcount5 = PythonTotalRefCount();

		// function done
		Py_DECREF(function_pointer);

		// second value done
		size_t refcount6 = PythonTotalRefCount();

		// After function created (but before called)
		// should be same as after last call
		CPPUNIT_ASSERT(refcount1 == refcount5);

		// We don't return exactly to start because when function is first requested
		// it gets instantiated
		CPPUNIT_ASSERT(refcount6 <= (refcount0 + 1));

		Py_DECREF(codemodule);
		Py_DECREF(compiledcode);

		Py_Finalize();
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION( TestPythonConnection );

