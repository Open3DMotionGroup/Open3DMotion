/*--
  Open3DMotion 
  Copyright (c) 2004-2018.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "MemoryHandlerPython.h"

namespace Open3DMotion
{
	DEFINE_CLASS_NAME(MemoryHandlerPython);

	MemoryHandlerPython::MemoryHandlerPython(size_t _sizebytes)
	{
		// Since there appears to be no 'new' method,
		// easiest way to efficiently allocate array of correct size appears to be
		// to make one of length 1 and then resize it
		PyObject* py_ownarray = PyByteArray_FromStringAndSize("", 1);
		PyByteArray_Resize(py_ownarray, _sizebytes);
		py_view = PyMemoryView_FromObject(py_ownarray);
		Py_DECREF(py_ownarray);
	}

	MemoryHandlerPython::MemoryHandlerPython(const MemoryHandlerPython& ref)
	{
		// Copy pointer and increment Python reference
		py_view = ref.py_view;
		Py_INCREF(py_view);
	}

	MemoryHandlerPython::MemoryHandlerPython(PyObject* _py_to_acquire)
	{
		// Copy pointer and increment Python reference
		py_view = _py_to_acquire;
		Py_INCREF(py_view);
	}


	MemoryHandlerPython::~MemoryHandlerPython()
	{
		Py_DECREF(py_view);
	}

	MemoryHandlerPython* MemoryHandlerPython::Clone() const
	{
		return new MemoryHandlerPython(*this);
	}

	size_t MemoryHandlerPython::SizeBytes() const
	{
		return PyMemoryView_GET_BUFFER(py_view)->len;
	}

	UInt8* MemoryHandlerPython::Data() const
	{
		return (UInt8*)PyMemoryView_GET_BUFFER(py_view)->buf;
	}

	size_t MemoryHandlerPython::RefCount() const
	{
		return Py_REFCNT(py_view);
	}

	PyObject* MemoryHandlerPython::PythonMemoryView() const
	{
		return py_view;
	}
}
