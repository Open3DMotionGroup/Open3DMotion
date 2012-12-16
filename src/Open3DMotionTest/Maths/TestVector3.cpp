/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/Maths/Vector3.h"
#include <cppunit/extensions/HelperMacros.h>

class TestVector3 : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestVector3 );
	CPPUNIT_TEST( testConstructors );
	CPPUNIT_TEST( testLValue );
	CPPUNIT_TEST( testRValue );
	CPPUNIT_TEST( testInc );
	CPPUNIT_TEST( testDec );
	CPPUNIT_TEST( testMul );
	CPPUNIT_TEST( testDiv );
	CPPUNIT_TEST( testDot );
	CPPUNIT_TEST( testModulus );
	CPPUNIT_TEST( testAdd );
	CPPUNIT_TEST( testSubtract );
	CPPUNIT_TEST( testCrossProduct );
	CPPUNIT_TEST( testRatio );
	CPPUNIT_TEST( testCopy );
	CPPUNIT_TEST_SUITE_END();

public:
	void testConstructors()
	{
		Open3DMotion::Vector3 v1(5.0, -2.1, 9.8);
		CPPUNIT_ASSERT_EQUAL( 5.0,  v1.x[0]);
		CPPUNIT_ASSERT_EQUAL(-2.1,  v1.x[1]);
		CPPUNIT_ASSERT_EQUAL( 9.8,  v1.x[2]);

		double v2[3] = { -1.114, 2.87, -9000.1 };
		Open3DMotion::Vector3 v3(v2);
		CPPUNIT_ASSERT_EQUAL(   -1.114, v3.x[0]);
		CPPUNIT_ASSERT_EQUAL(    2.870, v3.x[1]);
		CPPUNIT_ASSERT_EQUAL(-9000.100, v3.x[2]);
	}

	void testLValue()
	{
		Open3DMotion::Vector3 v4(0,0,0);
		v4 = -2.0;
		CPPUNIT_ASSERT_EQUAL(-2.0, v4[0]);
		CPPUNIT_ASSERT_EQUAL(-2.0, v4[1]);
		CPPUNIT_ASSERT_EQUAL(-2.0, v4[2]);		
	}

	void testRValue()
	{
		Open3DMotion::Vector3 v1(-18.0, 9.1, 977.0);

		double* v2 = v1;
		CPPUNIT_ASSERT_EQUAL(-18.0, v2[0]);
		CPPUNIT_ASSERT_EQUAL(9.1,   v2[1]);
		CPPUNIT_ASSERT_EQUAL(977.0, v2[2]);

		const double* v3 = v1;
		CPPUNIT_ASSERT_EQUAL(-18.0, v3[0]);
		CPPUNIT_ASSERT_EQUAL(9.1,   v3[1]);
		CPPUNIT_ASSERT_EQUAL(977.0, v3[2]);
	}

	void testInc()
	{
		Open3DMotion::Vector3 v1(-1.1, 2.8, 3.4);
		Open3DMotion::Vector3 v2(-1000.0, 2000.0, 3000.0);
		v1 += v2;
		CPPUNIT_ASSERT_EQUAL(-1001.1, v1[0]);
		CPPUNIT_ASSERT_EQUAL( 2002.8, v1[1]);
		CPPUNIT_ASSERT_EQUAL( 3003.4, v1[2]);
	}

	void testDec()
	{
		Open3DMotion::Vector3 v1(5.1, 2.2, 3.2);
		Open3DMotion::Vector3 v2(0.1, 0.2, 0.2);
		v1 -= v2;
		CPPUNIT_ASSERT_EQUAL(5.0, v1[0]);
		CPPUNIT_ASSERT_EQUAL(2.0, v1[1]);
		CPPUNIT_ASSERT_EQUAL(3.0, v1[2]);
	}

	void testMul()
	{
		Open3DMotion::Vector3 v1(0.0, 1.0, 2.0);
		v1 *= 2.1;
		CPPUNIT_ASSERT_EQUAL(0.0, v1[0]);
		CPPUNIT_ASSERT_EQUAL(2.1, v1[1]);
		CPPUNIT_ASSERT_EQUAL(4.2, v1[2]);

		Open3DMotion::Vector3 v2(1.0, 2.0, 3.0);
		Open3DMotion::Vector3 v3;
		Open3DMotion::Vector3::Mul(v3, 2.5, v2);
		CPPUNIT_ASSERT_EQUAL(2.5, v3[0]);
		CPPUNIT_ASSERT_EQUAL(5.0, v3[1]);
		CPPUNIT_ASSERT_EQUAL(7.5, v3[2]);
	}

	void testDiv()
	{
		Open3DMotion::Vector3 v1(-3.0, -2.0, 1.0);
		v1 /= 2;
		CPPUNIT_ASSERT_EQUAL(-1.5, v1[0]);
		CPPUNIT_ASSERT_EQUAL(-1.0, v1[1]);
		CPPUNIT_ASSERT_EQUAL( 0.5, v1[2]);
	}

	void testDot()
	{
		Open3DMotion::Vector3 v1(-1.0, 2.2, 3.3);
		Open3DMotion::Vector3 v2( 2.0, 0.1, 4.0);		
		CPPUNIT_ASSERT_EQUAL(11.42, Open3DMotion::Vector3::Dot(v1, v2));
		CPPUNIT_ASSERT_EQUAL(11.42, v1.Dot(v2));		
		CPPUNIT_ASSERT_EQUAL(11.42, v2.Dot(v1));		
	}

	void testModulus()
	{
		Open3DMotion::Vector3 v1(-2, 4, 5);
		CPPUNIT_ASSERT_EQUAL(45.0, v1.Modulus2());
		CPPUNIT_ASSERT_DOUBLES_EQUAL(6.708203, v1.Modulus(), 1E-6);

		const double* v2 = v1;
		CPPUNIT_ASSERT_EQUAL(45.0, Open3DMotion::Vector3::Modulus2(v2));
		CPPUNIT_ASSERT_DOUBLES_EQUAL(6.708203, Open3DMotion::Vector3::Modulus(v2), 1E-6);

	}
	
	void testAdd()
	{
		Open3DMotion::Vector3 v1(2.437274198, 1.190381382, 0.546430569);
		Open3DMotion::Vector3 v2(-1.544648054, 5.446766719, 145.9651926);
		Open3DMotion::Vector3 v3;
		const double* v4 = Open3DMotion::Vector3::Add(v3, v1, v2);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.8926261, v3[0], 1E-7);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(  6.6371481, v3[1], 1E-7);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(146.5116232, v3[2], 1E-7);
    CPPUNIT_ASSERT(v4 == &v3[0]);
	}

	void testSubtract()
	{
		Open3DMotion::Vector3 v1(-2.340204089,	-9.240476008,	1.761451673);
		Open3DMotion::Vector3 v2(-19.11623545,	-58.74163019,	93.62603397);
		Open3DMotion::Vector3 v3;
		const double* v4 = Open3DMotion::Vector3::Sub(v3, v1, v2);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 16.7760314, v3[0], 1E-7);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 49.5011542, v3[1], 1E-7);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-91.8645823, v3[2], 1E-7);
    CPPUNIT_ASSERT(v4 == &v3[0]);
	}

	void testCrossProduct()
	{
		Open3DMotion::Vector3 v1(13.0619581734,	-23.8563543266,	-38.6764784370);
		Open3DMotion::Vector3 v2(10.2225330156,	2.6854203852,	-0.8798787923);
		Open3DMotion::Vector3 v3;
		Open3DMotion::Vector3::Cross(v3, v1, v2);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 124.8533039, v3[0], 1E-7);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-383.8786378, v3[1], 1E-7);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 278.9492185, v3[2], 1E-7);
	}

	void testRatio()
	{
		Open3DMotion::Vector3 v1(1.213470485, -3.501837044, 3.747126298);
		Open3DMotion::Vector3 v2(19.28014678, -1.875918404, -0.035446056);
		Open3DMotion::Vector3 v3;
		Open3DMotion::Vector3::Ratio(v3, 0.75, v1, v2);	
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 14.7634777, v3[0], 1E-7);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( -2.2823981, v3[1], 1E-7);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.9101970, v3[2], 1E-7);
	}

	void testCopy()
	{
		double b[3] = { 1.25, -3.21, 2.33 };
		double a[5] = { 9.0, 9.0, 9.0, 9.0, 9.0 };
		Open3DMotion::Vector3::Copy(a, b);
		CPPUNIT_ASSERT_EQUAL( 1.25, a[0]);
		CPPUNIT_ASSERT_EQUAL(-3.21, a[1]);
		CPPUNIT_ASSERT_EQUAL( 2.33, a[2]);
		CPPUNIT_ASSERT_EQUAL(  9.0, a[3]);
		CPPUNIT_ASSERT_EQUAL(  9.0, a[4]);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestVector3 );


