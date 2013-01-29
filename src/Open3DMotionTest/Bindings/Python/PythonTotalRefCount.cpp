/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "PythonTotalRefCount.h"
#include <Python.h>

/** Helper to verify current reference count and avoid memory leaks */
size_t PythonTotalRefCount()
{
#ifdef Py_DEBUG
	#pragma message("Open3DMotion MESSAGE: Debug build selected. Ensure that a custom build of python shared library is used (one built with Py_DEBUG defined).")
	PyObject* borrowed_count_function = PySys_GetObject("gettotalrefcount");
	PyObject* py_count = PyObject_CallObject(borrowed_count_function, NULL);
  size_t count = PyInt_AsSsize_t(py_count);
  Py_DECREF(py_count);
	return count;
#else
	return 0;
#endif
}
