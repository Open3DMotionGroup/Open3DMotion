#include "Open3DMotion/Biomechanics/Algorithms/LinearCalibration.h"
#include <cppunit/extensions/HelperMacros.h>

using namespace Open3DMotion;

class TestLinearCalibration : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestLinearCalibration );
	CPPUNIT_TEST( testConstructors );
	CPPUNIT_TEST( testApply );
	CPPUNIT_TEST( testInverse );
	CPPUNIT_TEST( testCircular );
	CPPUNIT_TEST( testZeroDev );
	CPPUNIT_TEST_SUITE_END();

public:
	void testConstructors()
	{
		LinearCalibration c1;
		CPPUNIT_ASSERT_EQUAL(1.0, c1.scale);
		CPPUNIT_ASSERT_EQUAL(0.0, c1.offset);
		CPPUNIT_ASSERT_EQUAL(0.0, c1.offset_confidence);

		LinearCalibration c2(-92.4, 1050);
		CPPUNIT_ASSERT_EQUAL(-92.4, c2.scale);
		CPPUNIT_ASSERT_EQUAL(1050.0, c2.offset);
		CPPUNIT_ASSERT_EQUAL(0.0, c2.offset_confidence);

		LinearCalibration c3(10001.008, -2003, 3.56789);
		CPPUNIT_ASSERT_EQUAL(10001.008, c3.scale);
		CPPUNIT_ASSERT_EQUAL(-2003.0, c3.offset);
		CPPUNIT_ASSERT_EQUAL(3.56789, c3.offset_confidence);
	}

	void testApply()
	{
		// test calibration object
		LinearCalibration c1(2.0, -150, 3.0);
		
		// value to test
		double b_value = 23.0;
		double b_conf = 4.0;

		// struct for results
		double a_value = 0.0;
		double a_conf = 0.0;

		// apply
		c1.Apply(a_value, a_conf, b_value, b_conf);

		// check
		CPPUNIT_ASSERT_DOUBLES_EQUAL(346.0, a_value, 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(10.0, a_conf, 1E-12);
	}

	void testInverse()
	{
		// test calibration object
		LinearCalibration c1(3.0, 1000, 3.0);
		
		// value to test
		double b_value = -2909.7;
		double b_conf = 15.0;

		// struct for results
		double a_value(0.0);
		double a_conf(0.0);

		// apply
		c1.ApplyInverse(a_value, a_conf, b_value, b_conf);

		// check
		CPPUNIT_ASSERT_DOUBLES_EQUAL(30.1, a_value, 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, a_conf, 1E-12);

	}

	void testCircular()
	{
		LinearCalibration c1(4.275, 1298.001, 1.0265);

		double a_value = 89.127;

		double acal(0.0);
		c1.Apply(acal, a_value);

		double atest(0.0);
		c1.ApplyInverse(atest, acal);

		CPPUNIT_ASSERT_DOUBLES_EQUAL(89.127, atest, 1E-12);
	}

	void testZeroDev()
	{
		LinearCalibration c1(2.0, -10.0, 0.0);
		double b_value = 1.0;
		double b_conf = 0.0;

		double a1_value(0.0);
		double a1_conf(0.0);
		c1.Apply(a1_value, a1_conf, b_value, b_conf);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, a1_conf, 1E-12);

		double a2_value(100.0);
		double a2_conf(100.0);
		c1.ApplyInverse(a2_value, a2_conf, a1_value, a1_conf);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, a2_conf, 1E-12);
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION( TestLinearCalibration );


