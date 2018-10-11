/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include <cppunit/extensions/HelperMacros.h>
#include "Open3DMotion/Bindings/Python/PythonConvert.h"
// #include "NumPy/ArrayObject.h"
#include "PythonTotalRefCount.h"
#include "Open3DMotion/OpenORM/Leaves/TreeString.h"
#include "Open3DMotion/OpenORM/Leaves/TreeBool.h"
#include "Open3DMotion/OpenORM/Leaves/TreeInt32.h"
#include "Open3DMotion/OpenORM/Leaves/TreeFloat64.h"
#include "Open3DMotion/OpenORM/Leaves/TreeBinary.h"
#include "Open3DMotion/Bindings/Python/MemoryHandlerPython.h"
#include "Open3DMotion/OpenORM/Branches/TreeList.h"
#include "Open3DMotion/OpenORM/Branches/TreeCompound.h"

using namespace Open3DMotion;

class TestPythonConvert : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestPythonConvert );
	CPPUNIT_TEST( testFromNull );
	CPPUNIT_TEST( testToNull );
	CPPUNIT_TEST( testFromString );
	CPPUNIT_TEST( testFromBool );
	CPPUNIT_TEST( testFromInt32 );
	CPPUNIT_TEST( testFromFloat64 );
	CPPUNIT_TEST( testFromBinary );
	CPPUNIT_TEST( testToString );
	CPPUNIT_TEST( testToBool );
	CPPUNIT_TEST( testToInt32 );
	CPPUNIT_TEST( testToInt32FromLong );
	CPPUNIT_TEST( testToInt32FromLongOverflow );
	CPPUNIT_TEST( testToFloat64 );
	CPPUNIT_TEST( testToBinary );
	CPPUNIT_TEST( testFromEmptyList );
	CPPUNIT_TEST( testFromVariedNumberList );
	CPPUNIT_TEST( testToEmptyList );
	CPPUNIT_TEST( testToVariedNumberList );
	CPPUNIT_TEST( testFromEmptyCompound );
	CPPUNIT_TEST( testFromCompound );
	CPPUNIT_TEST( testToEmptyCompound );
	CPPUNIT_TEST( testToCompound );
	CPPUNIT_TEST_SUITE_END();

public:

	void setUp()
	{
		Py_Initialize();		
	}

	void tearDown()
	{
		Py_Finalize();
	}

	//--------------- Conversion of NULL items to/from Python ---------------//

	void testFromNull()
	{
		size_t refcount0 = PythonTotalRefCount();
		PyObject* py_result = PythonConvert::FromTree(NULL);
		Py_DECREF(py_result);
		size_t refcount1 = PythonTotalRefCount();
		CPPUNIT_ASSERT(refcount1 == refcount0);
	}

	void testToNull()
	{
		size_t refcount0 = PythonTotalRefCount();

		TreeValue* result_none = PythonConvert::ToTree(Py_None);
		CPPUNIT_ASSERT(result_none == NULL);

		TreeValue* result_null = PythonConvert::ToTree(NULL);
		CPPUNIT_ASSERT(result_null == NULL);

		size_t refcount1 = PythonTotalRefCount();
		CPPUNIT_ASSERT(refcount1 == refcount0);
	}

	//------------- Conversion of leaf types from C++ to Python -------------//

	void testFromString()
	{
		size_t refcount0 = PythonTotalRefCount();
		TreeString s("Hello");
		PyObject* py_result = PythonConvert::FromTree(&s);
		CPPUNIT_ASSERT( PyString_Check(py_result) );
		const char* chars = PyString_AsString(py_result);
		CPPUNIT_ASSERT(chars != NULL);
		CPPUNIT_ASSERT_EQUAL(std::string("Hello"), std::string(chars));
		Py_DECREF(py_result);
		size_t refcount1 = PythonTotalRefCount();
		CPPUNIT_ASSERT(refcount1 == refcount0);
	}

	void testFromBool()
	{
		size_t refcount0 = PythonTotalRefCount();
		
		TreeBool btrue(true);
		PyObject* py_result_true = PythonConvert::FromTree(&btrue);
		CPPUNIT_ASSERT( PyBool_Check(py_result_true) );
		CPPUNIT_ASSERT(py_result_true == Py_True);
		Py_DECREF(py_result_true);

		TreeBool bfalse(false);
		PyObject* py_result_false = PythonConvert::FromTree(&bfalse);
		CPPUNIT_ASSERT( PyBool_Check(py_result_false) );
		CPPUNIT_ASSERT(py_result_false == Py_False);
		Py_DECREF(py_result_false);

		size_t refcount1 = PythonTotalRefCount();
		CPPUNIT_ASSERT(refcount1 == refcount0);
	}

	void testFromInt32()
	{
		size_t refcount0 = PythonTotalRefCount();
		
		TreeInt32 x(573L);
		PyObject* py_result = PythonConvert::FromTree(&x);
		CPPUNIT_ASSERT( PyInt_Check(py_result) );
		CPPUNIT_ASSERT_EQUAL(573L, PyInt_AsLong(py_result));
		Py_DECREF(py_result);

		size_t refcount1 = PythonTotalRefCount();
		CPPUNIT_ASSERT(refcount1 == refcount0);
	}

	void testFromFloat64()
	{
		size_t refcount0 = PythonTotalRefCount();
		
		TreeFloat64 x(-249.3);
		PyObject* py_result = PythonConvert::FromTree(&x);
		CPPUNIT_ASSERT( PyFloat_Check(py_result) );
		CPPUNIT_ASSERT_EQUAL(-249.3, PyFloat_AsDouble(py_result));
		Py_DECREF(py_result);

		size_t refcount1 = PythonTotalRefCount();
		CPPUNIT_ASSERT(refcount1 == refcount0);
	}

	void testFromBinary()
	{
		size_t refcount0 = PythonTotalRefCount();
		
		// verify that empty item just gives None as a result rather than crashing
		TreeBinary empty;
		PyObject* py_empty = PythonConvert::FromTree(&empty);
		CPPUNIT_ASSERT( py_empty == Py_None);
		Py_DECREF(py_empty);

		{
			// make some data
			MemoryHandlerPython m( 1000 );
			memset(m.Data(), 0, 1000);
			m.Data()[23] = 173;
			m.Data()[997] = 62;
			m.Data()[999] = 3;

			// put into tree object
			TreeBinary tree_data(&m);
			PyObject* py_data = PythonConvert::FromTree(&tree_data);

			// should be byte array containing data as specified
			CPPUNIT_ASSERT(PyByteArray_Check(py_data));
			CPPUNIT_ASSERT_EQUAL(Py_ssize_t(1000), PyByteArray_Size(py_data));
			const char* result_data = PyByteArray_AsString(py_data);
			CPPUNIT_ASSERT_EQUAL(UInt8(0), (UInt8)result_data[0]);
			CPPUNIT_ASSERT_EQUAL(UInt8(0), (UInt8)result_data[10]);
			CPPUNIT_ASSERT_EQUAL(UInt8(173), (UInt8)result_data[23]);
			CPPUNIT_ASSERT_EQUAL(UInt8(62), (UInt8)result_data[997]);
			CPPUNIT_ASSERT_EQUAL(UInt8(0), (UInt8)result_data[998]);
			CPPUNIT_ASSERT_EQUAL(UInt8(3), (UInt8)result_data[999]);

			// done with array
			Py_DECREF(py_data);
		}

		size_t refcount1 = PythonTotalRefCount();
		CPPUNIT_ASSERT(refcount1 == refcount0);
	}

	//------------- Conversion of leaf types from Python to C++ -------------//

	void testToString()
	{
		size_t refcount0 = PythonTotalRefCount();

		PyObject* py_correct = PyString_FromString("Correct!");
		std::auto_ptr<TreeValue> result( PythonConvert::ToTree(py_correct) );
		Py_DECREF(py_correct);
		CPPUNIT_ASSERT(result.get() != NULL);
		TreeString* result_string = TreeValueCast<TreeString> ( result.get() );
		CPPUNIT_ASSERT(result_string != NULL);
		CPPUNIT_ASSERT_EQUAL(std::string("Correct!"), result_string->Value());
		size_t refcount1 = PythonTotalRefCount();
		CPPUNIT_ASSERT(refcount1 == refcount0);
	}

	void testToBool()
	{
		size_t refcount0 = PythonTotalRefCount();

		std::auto_ptr<TreeValue> result_true( PythonConvert::ToTree(Py_True) );
		CPPUNIT_ASSERT(result_true.get() != NULL);
		TreeBool* tree_result_true = TreeValueCast<TreeBool> ( result_true.get() );
		CPPUNIT_ASSERT(tree_result_true != NULL);
		CPPUNIT_ASSERT_EQUAL(true, tree_result_true->Value());

		std::auto_ptr<TreeValue> result_false( PythonConvert::ToTree(Py_False) );
		CPPUNIT_ASSERT(result_false.get() != NULL);
		TreeBool* tree_result_false = TreeValueCast<TreeBool> ( result_false.get() );
		CPPUNIT_ASSERT(tree_result_false != NULL);
		CPPUNIT_ASSERT_EQUAL(false, tree_result_false->Value());

		size_t refcount1 = PythonTotalRefCount();
		CPPUNIT_ASSERT(refcount1 == refcount0);
	}

	void testToInt32FromLong()
	{
		size_t refcount0 = PythonTotalRefCount();

		// should work for numbers that can be represented as 32-bit
		PyObject* py_number = PyLong_FromLong(-59982L);
		std::auto_ptr<TreeValue> result(PythonConvert::ToTree(py_number));
		Py_DECREF(py_number);
		CPPUNIT_ASSERT(result.get() != NULL);
		TreeInt32* result_int32 = TreeValueCast<TreeInt32>(result.get());
		CPPUNIT_ASSERT(result_int32 != NULL);
		CPPUNIT_ASSERT_EQUAL(-59982L, result_int32->Value());

		size_t refcount1 = PythonTotalRefCount();
		CPPUNIT_ASSERT(refcount1 == refcount0);
	}

	void testToInt32FromLongOverflow()
	{
		size_t refcount0 = PythonTotalRefCount();

		// should fail when presented with long occupying more than 32-bit representation
		PyObject* py_number = PyLong_FromLongLong(0xABCD0123CDEF3456LL);
		std::auto_ptr<TreeValue> result( PythonConvert::ToTree(py_number) );
		CPPUNIT_ASSERT(result.get() == NULL);
		Py_DECREF(py_number);

		size_t refcount1 = PythonTotalRefCount();
		CPPUNIT_ASSERT(refcount1 == refcount0);
	}

	void testToInt32()
	{
		size_t refcount0 = PythonTotalRefCount();

		PyObject* py_number = PyInt_FromLong(-597L);
		std::auto_ptr<TreeValue> result(PythonConvert::ToTree(py_number));
		Py_DECREF(py_number);
		CPPUNIT_ASSERT(result.get() != NULL);
		TreeInt32* result_int32 = TreeValueCast<TreeInt32>(result.get());
		CPPUNIT_ASSERT(result_int32 != NULL);
		CPPUNIT_ASSERT_EQUAL(-597L, result_int32->Value());

		size_t refcount1 = PythonTotalRefCount();
		CPPUNIT_ASSERT(refcount1 == refcount0);
	}

	void testToFloat64()
	{
		size_t refcount0 = PythonTotalRefCount();

		PyObject* py_number = PyFloat_FromDouble(480327.9986754);
		std::auto_ptr<TreeValue> result( PythonConvert::ToTree(py_number) );
		Py_DECREF(py_number);
		CPPUNIT_ASSERT(result.get() != NULL);
		TreeFloat64* result_float64 = TreeValueCast<TreeFloat64> ( result.get() );
		CPPUNIT_ASSERT(result_float64 != NULL);
		CPPUNIT_ASSERT_EQUAL(480327.9986754, result_float64->Value());

		size_t refcount1 = PythonTotalRefCount();
		CPPUNIT_ASSERT(refcount1 == refcount0);
	}

	void testToBinary()
	{
		size_t refcount0 = PythonTotalRefCount();

		// make python byte array with some values
		std::vector<Int8> bytes(500, Int8(0));
		bytes[33] = (Int8)21;
		bytes[479] = (Int8)152;
		PyObject* py_bytes = PyByteArray_FromStringAndSize(&bytes[0], 500);

		{
			// convert
			std::auto_ptr<TreeValue> result( PythonConvert::ToTree(py_bytes) );

			// this should get rid of original reference to byte array,
			// but shouldn't be destroyed since result should hold a new reference
			Py_DECREF(py_bytes);

			// check result
			CPPUNIT_ASSERT(result.get() != NULL);
			TreeBinary* result_binary = TreeValueCast<TreeBinary> ( result.get() );
			CPPUNIT_ASSERT(result_binary != NULL);
			CPPUNIT_ASSERT_EQUAL(size_t(500), result_binary->SizeBytes() );
			CPPUNIT_ASSERT_EQUAL(UInt8(0), result_binary->Data()[0]);
			CPPUNIT_ASSERT_EQUAL(UInt8(0), result_binary->Data()[10]);
			CPPUNIT_ASSERT_EQUAL(UInt8(21), result_binary->Data()[33]);
			CPPUNIT_ASSERT_EQUAL(UInt8(152), result_binary->Data()[479]);
			CPPUNIT_ASSERT_EQUAL(UInt8(0), result_binary->Data()[0]);

		// final reference to byte array should go at end of this scope
		// as TreeBinary result is destroyed
		}

		size_t refcount1 = PythonTotalRefCount();
		CPPUNIT_ASSERT(refcount1 == refcount0);
	}

	//------------- Conversion of lists from C++ to Python -------------//

	void testFromEmptyList()
	{
		size_t refcount0 = PythonTotalRefCount();
		TreeList testlist("SomeElementName");
		PyObject* py_result = PythonConvert::FromTree(&testlist);
		CPPUNIT_ASSERT(PyDict_Check(py_result));
		CPPUNIT_ASSERT_EQUAL(Py_ssize_t(1), PyDict_Size(py_result));
		PyObject* py_dict_key = NULL;
		PyObject* py_dict_value = NULL;
		Py_ssize_t pos = 0;
		CPPUNIT_ASSERT(PyDict_Next(py_result, &pos, &py_dict_key, &py_dict_value) != 0);
		CPPUNIT_ASSERT(PyString_Check(py_dict_key));
		CPPUNIT_ASSERT(PyList_Check(py_dict_value));
		CPPUNIT_ASSERT_EQUAL(std::string("SomeElementName"), std::string(PyString_AsString(py_dict_key)));
		CPPUNIT_ASSERT_EQUAL(Py_ssize_t(0), PyList_Size(py_dict_value));
		Py_DECREF(py_result);
		size_t refcount1 = PythonTotalRefCount();
		CPPUNIT_ASSERT(refcount1 == refcount0);
	}

	void testFromVariedNumberList()
	{
		size_t refcount0 = PythonTotalRefCount();

		TreeList testlist("VariedElement");
		testlist.Add( new TreeInt32( 781 ) );
		testlist.Add( new TreeFloat64( -999.321 ) );
		testlist.Add( new TreeBool(true) );

		PyObject* py_result = PythonConvert::FromTree(&testlist);

		// The way to represent this is a dictionary with one element whose name is the list element
		CPPUNIT_ASSERT(PyDict_Check(py_result));
		CPPUNIT_ASSERT_EQUAL(Py_ssize_t(1), PyDict_Size(py_result));
		PyObject* py_list = PyDict_GetItemString(py_result, "VariedElement");
		CPPUNIT_ASSERT(py_list != NULL);
		CPPUNIT_ASSERT(PyList_Check(py_list));

		// And should have the 3 elements
		CPPUNIT_ASSERT_EQUAL(Py_ssize_t(3), PyList_Size(py_list));

		CPPUNIT_ASSERT(PyInt_Check(PyList_GetItem(py_list, 0)));
		CPPUNIT_ASSERT_EQUAL(781L, PyLong_AsLong(PyList_GetItem(py_list, 0)));

		CPPUNIT_ASSERT(PyFloat_Check(PyList_GetItem(py_list, 1)));
		CPPUNIT_ASSERT_EQUAL(-999.321, PyFloat_AsDouble(PyList_GetItem(py_list, 1)));

		CPPUNIT_ASSERT(PyBool_Check(PyList_GetItem(py_list, 2)));
		CPPUNIT_ASSERT(PyList_GetItem(py_list, 2) == Py_True);

		Py_DECREF(py_result);

		size_t refcount1 = PythonTotalRefCount();
		CPPUNIT_ASSERT(refcount1 == refcount0);
	}

	//------------- Conversion of lists from Python to C++ -------------//

	void testToEmptyList()
	{
		size_t refcount0 = PythonTotalRefCount();

		// convert totally empty list (not even element name specified)
		PyObject* py_test_empty = PyDict_New();
		PyObject* py_list = PyList_New(0);
		PyDict_SetItemString(py_test_empty, "SomeElementNameHere", py_list);
		Py_DECREF(py_list);
		std::auto_ptr<TreeValue> result( PythonConvert::ToTree(py_test_empty) );
		Py_DECREF(py_test_empty);
		
		// check it works and uses the element name
		CPPUNIT_ASSERT(result.get() != NULL);
		TreeList* result_list = TreeValueCast<TreeList> ( result.get() );
		CPPUNIT_ASSERT(result_list != NULL);
		CPPUNIT_ASSERT_EQUAL(size_t(0), result_list->NumElements());
		CPPUNIT_ASSERT_EQUAL(std::string("SomeElementNameHere"), result_list->ElementName());

		size_t refcount1 = PythonTotalRefCount();
		CPPUNIT_ASSERT(refcount1 == refcount0);
	}

	void testToVariedNumberList()
	{
		size_t refcount0 = PythonTotalRefCount();

		// make data
		PyObject* py_wrapper(NULL);
		{
			PyObject* py_list = PyList_New(3);
			PyList_SetItem(py_list, 0, PyInt_FromLong(23L));
			PyList_SetItem(py_list, 1, PyFloat_FromDouble(-99999.93));
			Py_INCREF(Py_True);
			PyList_SetItem(py_list, 2, Py_True);
			py_wrapper = PyDict_New();
			PyDict_SetItemString(py_wrapper, "Apples", py_list);
			Py_DECREF(py_list);
		}

		// convert
		std::auto_ptr<TreeValue> result( PythonConvert::ToTree(py_wrapper) );

		// done with data
		Py_DECREF(py_wrapper);

		// check results
		CPPUNIT_ASSERT(result.get() != NULL);
		const TreeList* result_list = TreeValueCast<TreeList>( result.get() );
		CPPUNIT_ASSERT(result_list != NULL);
		CPPUNIT_ASSERT_EQUAL(size_t(3), result_list->NumElements());
		CPPUNIT_ASSERT_EQUAL(std::string("Apples"), result_list->ElementName());
		const TreeInt32* el0 = TreeValueCast<TreeInt32> ( result_list->ElementArray()[0] );
		const TreeFloat64* el1 = TreeValueCast<TreeFloat64> ( result_list->ElementArray()[1] );
		const TreeBool* el2 = TreeValueCast<TreeBool> ( result_list->ElementArray()[2] );
		CPPUNIT_ASSERT(el0 != NULL);
		CPPUNIT_ASSERT_EQUAL(23L, el0->Value());
		CPPUNIT_ASSERT(el1 != NULL);
		CPPUNIT_ASSERT_EQUAL(-99999.93, el1->Value());
		CPPUNIT_ASSERT(el2 != NULL);
		CPPUNIT_ASSERT_EQUAL(true, el2->Value());

		size_t refcount1 = PythonTotalRefCount();
		CPPUNIT_ASSERT(refcount1 == refcount0);
	}

	void testFromEmptyCompound()
	{
		size_t refcount0 = PythonTotalRefCount();

		TreeCompound c;
		PyObject* py_c = PythonConvert::FromTree(&c);
		CPPUNIT_ASSERT(PyDict_Check(py_c));
		CPPUNIT_ASSERT_EQUAL(Py_ssize_t(0), PyDict_Size(py_c));
		Py_DECREF(py_c);

		size_t refcount1 = PythonTotalRefCount();
		CPPUNIT_ASSERT(refcount1 == refcount0);
	}

	void testFromCompound()
	{
		size_t refcount0 = PythonTotalRefCount();

		TreeCompound c;
		c.Set("Age", new TreeInt32(3));
		c.Set("Height", new TreeFloat64(22.3));
		c.Set("Name", new TreeString("Ermintrude"));

		TreeCompound* c_sub = new TreeCompound;
		c_sub->Set("Comment", new TreeString("What is this?"));
		c.Set("OtherInfo", c_sub);

		PyObject* py_c = PythonConvert::FromTree(&c);
		CPPUNIT_ASSERT(PyDict_Check(py_c));
		CPPUNIT_ASSERT_EQUAL(Py_ssize_t(4), PyDict_Size(py_c));

		PyObject* py_age = PyDict_GetItemString(py_c, "Age");
		CPPUNIT_ASSERT(PyInt_Check(py_age));
		CPPUNIT_ASSERT_EQUAL(3L, PyLong_AsLong(py_age));

		PyObject* py_height = PyDict_GetItemString(py_c, "Height");
		CPPUNIT_ASSERT(PyFloat_Check(py_height));
		CPPUNIT_ASSERT_EQUAL(22.3, PyFloat_AsDouble(py_height));

		PyObject* py_name = PyDict_GetItemString(py_c, "Name");
		CPPUNIT_ASSERT(PyString_Check(py_name));
		CPPUNIT_ASSERT_EQUAL( std::string("Ermintrude"), std::string( PyString_AsString(py_name) ) );

		PyObject* py_other = PyDict_GetItemString(py_c, "OtherInfo");
		CPPUNIT_ASSERT(PyDict_Check(py_other));
		CPPUNIT_ASSERT_EQUAL(Py_ssize_t(1), PyDict_Size(py_other));
		PyObject* py_comment = PyDict_GetItemString(py_other, "Comment");
		CPPUNIT_ASSERT(PyString_Check(py_comment));
		CPPUNIT_ASSERT_EQUAL(std::string("What is this?"), std::string(PyString_AsString(py_comment)));

		Py_DECREF(py_c);

		size_t refcount1 = PythonTotalRefCount();
		CPPUNIT_ASSERT(refcount1 == refcount0);
	}

	void testToEmptyCompound()
	{
		size_t refcount0 = PythonTotalRefCount();

		PyObject* py_c = PyDict_New();
		std::auto_ptr<TreeValue> result( PythonConvert::ToTree(py_c) );
		Py_DECREF(py_c);

		CPPUNIT_ASSERT(result.get() != NULL);
		const TreeCompound* c = TreeValueCast<TreeCompound> ( result.get() );
		CPPUNIT_ASSERT(c != NULL);
		CPPUNIT_ASSERT_EQUAL(size_t(0), c->NumElements());

		size_t refcount1 = PythonTotalRefCount();
		CPPUNIT_ASSERT(refcount1 == refcount0);
	}

	void testToCompound()
	{
		size_t refcount0 = PythonTotalRefCount();

		// build Python data
		PyObject* py_c = PyDict_New();
		PyObject* py_distance = PyFloat_FromDouble(33.333);
		PyObject* py_hats = PyInt_FromLong(4);
		PyObject* py_c_info = PyDict_New();
		PyObject* py_description = PyString_FromString("Hat throwing contest");
		PyDict_SetItemString(py_c_info, "Description", py_description);
		PyDict_SetItemString(py_c, "Info", py_c_info);
		PyDict_SetItemString(py_c, "Distance", py_distance);
		PyDict_SetItemString(py_c, "Hats", py_hats);
		
		// done with direct references to sub-objects
		Py_DECREF(py_distance);
		Py_DECREF(py_hats);
		Py_DECREF(py_c_info);
		Py_DECREF(py_description);

		// do conversion
		std::auto_ptr<TreeValue> result( PythonConvert::ToTree(py_c) );

		// done with base
		Py_DECREF(py_c);

		CPPUNIT_ASSERT(result.get() != NULL);
		const TreeCompound* c = TreeValueCast<TreeCompound> ( result.get() );
		CPPUNIT_ASSERT(c != NULL);
		CPPUNIT_ASSERT_EQUAL(size_t(3), c->NumElements());

		const TreeValue* distance = c->Get("Distance");
		CPPUNIT_ASSERT(distance != NULL);
		const TreeFloat64* distance_float64 = TreeValueCast<TreeFloat64> ( distance );
		CPPUNIT_ASSERT(distance_float64 != NULL);

		const TreeValue* hats = c->Get("Hats");
		CPPUNIT_ASSERT(hats != NULL);
		const TreeInt32* hats_int32 = TreeValueCast<TreeInt32> ( hats );
		CPPUNIT_ASSERT(hats_int32 != NULL);

		const TreeValue* info = c->Get("Info");
		CPPUNIT_ASSERT(hats != NULL);
		const TreeCompound* c_info = TreeValueCast<TreeCompound> ( info );
		CPPUNIT_ASSERT(c_info != NULL);
		CPPUNIT_ASSERT_EQUAL(size_t(1), c_info->NumElements());
		const TreeValue* description = c_info->Get("Description");
		CPPUNIT_ASSERT(description != NULL);
		const TreeString* description_string = TreeValueCast<TreeString> ( description );
		CPPUNIT_ASSERT(description_string != NULL);
		CPPUNIT_ASSERT_EQUAL(std::string("Hat throwing contest"), description_string->Value());

		size_t refcount1 = PythonTotalRefCount();
		CPPUNIT_ASSERT(refcount1 == refcount0);
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION( TestPythonConvert );

