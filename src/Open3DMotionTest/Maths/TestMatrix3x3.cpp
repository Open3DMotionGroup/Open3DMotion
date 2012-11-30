/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/Maths/Matrix3x3.h"
#include "Open3DMotion/Maths/Vector3.h"
#include <cppunit/extensions/HelperMacros.h>

class TestMatrix3x3 : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestMatrix3x3 );
	CPPUNIT_TEST( testConstructors );
	CPPUNIT_TEST( testLValue );
	CPPUNIT_TEST( testRValue );
	CPPUNIT_TEST( testID );
	CPPUNIT_TEST( testDet );
	CPPUNIT_TEST( testMul );
	CPPUNIT_TEST( testMulVec );
	CPPUNIT_TEST( testSVD );
	CPPUNIT_TEST( testRatio );
	CPPUNIT_TEST_SUITE_END();

public:
	void testConstructors()
	{
		double m1[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		Open3DMotion::Matrix3x3 m2(m1);
		CPPUNIT_ASSERT_EQUAL(1.0, m2.x[0]);
		CPPUNIT_ASSERT_EQUAL(2.0, m2.x[1]);
		CPPUNIT_ASSERT_EQUAL(3.0, m2.x[2]);
		CPPUNIT_ASSERT_EQUAL(4.0, m2.x[3]);
		CPPUNIT_ASSERT_EQUAL(5.0, m2.x[4]);
		CPPUNIT_ASSERT_EQUAL(6.0, m2.x[5]);
		CPPUNIT_ASSERT_EQUAL(7.0, m2.x[6]);
		CPPUNIT_ASSERT_EQUAL(8.0, m2.x[7]);
		CPPUNIT_ASSERT_EQUAL(9.0, m2.x[8]);

		Open3DMotion::Matrix3x3 m3(1809.77);
		CPPUNIT_ASSERT_EQUAL(1809.77, m3.x[0]);
		CPPUNIT_ASSERT_EQUAL(1809.77, m3.x[1]);
		CPPUNIT_ASSERT_EQUAL(1809.77, m3.x[2]);
		CPPUNIT_ASSERT_EQUAL(1809.77, m3.x[3]);
		CPPUNIT_ASSERT_EQUAL(1809.77, m3.x[4]);
		CPPUNIT_ASSERT_EQUAL(1809.77, m3.x[5]);
		CPPUNIT_ASSERT_EQUAL(1809.77, m3.x[6]);
		CPPUNIT_ASSERT_EQUAL(1809.77, m3.x[7]);
		CPPUNIT_ASSERT_EQUAL(1809.77, m3.x[8]);	
	}

	void testLValue()
	{
		double m1[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		Open3DMotion::Matrix3x3 m2(0.0);
		m2 = m1;
		CPPUNIT_ASSERT_EQUAL(1.0, m2.x[0]);
		CPPUNIT_ASSERT_EQUAL(2.0, m2.x[1]);
		CPPUNIT_ASSERT_EQUAL(3.0, m2.x[2]);
		CPPUNIT_ASSERT_EQUAL(4.0, m2.x[3]);
		CPPUNIT_ASSERT_EQUAL(5.0, m2.x[4]);
		CPPUNIT_ASSERT_EQUAL(6.0, m2.x[5]);
		CPPUNIT_ASSERT_EQUAL(7.0, m2.x[6]);
		CPPUNIT_ASSERT_EQUAL(8.0, m2.x[7]);
		CPPUNIT_ASSERT_EQUAL(9.0, m2.x[8]);

		m2 = -3281.002789;
		CPPUNIT_ASSERT_EQUAL(-3281.002789, m2.x[0]);
		CPPUNIT_ASSERT_EQUAL(-3281.002789, m2.x[1]);
		CPPUNIT_ASSERT_EQUAL(-3281.002789, m2.x[2]);
		CPPUNIT_ASSERT_EQUAL(-3281.002789, m2.x[3]);
		CPPUNIT_ASSERT_EQUAL(-3281.002789, m2.x[4]);
		CPPUNIT_ASSERT_EQUAL(-3281.002789, m2.x[5]);
		CPPUNIT_ASSERT_EQUAL(-3281.002789, m2.x[6]);
		CPPUNIT_ASSERT_EQUAL(-3281.002789, m2.x[7]);
		CPPUNIT_ASSERT_EQUAL(-3281.002789, m2.x[8]);

		Open3DMotion::Matrix3x3 m3(0.0);
		m3(1,2) = -582;
		CPPUNIT_ASSERT_EQUAL(-582.0, m3.x[5]);
	}

	void testRValue()
	{
		double m1[9] = 
		{ 
			2,  4,   6,
		  1,  3,   9,
			22, 21, 20 
		};
	
		Open3DMotion::Matrix3x3 m2(m1);
		CPPUNIT_ASSERT_EQUAL(2.0, m2(0,0));
		CPPUNIT_ASSERT_EQUAL(1.0, m2(1,0));
		CPPUNIT_ASSERT_EQUAL(21.0, m2(2,1));

		const double* m3 = m2;
		CPPUNIT_ASSERT_EQUAL(9.0, m3[5]);

		double* m4 = m2;
		CPPUNIT_ASSERT_EQUAL(3.0, m4[4]);
	}

	void testID()
	{
		Open3DMotion::Matrix3x3 m1;
		m1.ID();
		CPPUNIT_ASSERT_EQUAL(1.0, m1(0,0)); 		
		CPPUNIT_ASSERT_EQUAL(0.0, m1(0,1));
		CPPUNIT_ASSERT_EQUAL(0.0, m1(0,2)); 		
		CPPUNIT_ASSERT_EQUAL(0.0, m1(1,0));
		CPPUNIT_ASSERT_EQUAL(1.0, m1(1,1)); 		
		CPPUNIT_ASSERT_EQUAL(0.0, m1(1,2));
		CPPUNIT_ASSERT_EQUAL(0.0, m1(2,0)); 		
		CPPUNIT_ASSERT_EQUAL(0.0, m1(2,1)); 		
		CPPUNIT_ASSERT_EQUAL(1.0, m1(2,2));
	}

	void testDet()
	{
		double m1[9] = 
		{
			1, 0, 0,
			0, 1, 0,
			0, 0, 1
		};

		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, Open3DMotion::Matrix3x3::Det(m1), 1E-12);

		Open3DMotion::Matrix3x3 m2(m1);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, m2.Det(), 1E-12);

		double m3[9] = 
		{
			9,-1,23,
		-20,-9, 0,
		 18, 7, 3
		};

		CPPUNIT_ASSERT_DOUBLES_EQUAL(203.0, Open3DMotion::Matrix3x3::Det(m3), 1E-12);

		Open3DMotion::Matrix3x3 m4(m3);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(203.0, m4.Det(), 1E-12);
	}

	void testMul()
	{
		double B[9] =
		{
			 1, 2, 3,
			 2, 1, 2,
			 1, 5, 4
		};

		double C[9] =
		{
			9, 2, 7,
			8, 8, 1,
			6, 4, 3
		};

		double A[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		/* =
		  (9+16+18)  (2+16+12)  (7+2+9)
			(18+8+12)  (4+8+8)    (14+1+6)
			(9+40+24) (2+40+16)  (7+5+12)

			=
			43  30  18
			38  20  21
			73  58  24
		*/
		Open3DMotion::Matrix3x3::Mul(A, B, C);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(43, A[0], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(30, A[1], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(18, A[2], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(38, A[3], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(20, A[4], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(21, A[5], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(73, A[6], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(58, A[7], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(24, A[8], 1E-12);

		// transpose(B)
		double B_T[9] =
		{
			1, 2, 1,        
			2, 1, 5,        
			3, 2, 4                
		};

		// transpose(C)
		double C_T[9] =
		{
			9, 8, 6,
			2, 8, 4,
			7, 1, 3
		};

		double D[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		Open3DMotion::Matrix3x3::TransMul(D, B_T, C);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(43, D[0], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(30, D[1], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(18, D[2], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(38, D[3], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(20, D[4], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(21, D[5], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(73, D[6], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(58, D[7], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(24, D[8], 1E-12);

		double E[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		Open3DMotion::Matrix3x3::TransMulTrans(E, B_T, C_T);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(43, E[0], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(30, E[1], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(18, E[2], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(38, E[3], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(20, E[4], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(21, E[5], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(73, E[6], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(58, E[7], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(24, E[8], 1E-12);

		double F[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		Open3DMotion::Matrix3x3::MulTrans(F, B, C_T);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(43, F[0], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(30, F[1], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(18, F[2], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(38, F[3], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(20, F[4], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(21, F[5], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(73, F[6], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(58, F[7], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(24, F[8], 1E-12);
	}

	void testMulVec()
	{
		double R[9] =
		{
			-1, 2, 3,
			 4, 2,-2,
			 9, 7, 5
		};

		double x[3] =
		{
			1,
			2,
			3
		};

		/* R.x =
		-1 + 4 + 9
		 4 + 4 - 6
		 9 +14 +15
		 =
		 12
      2
     38
		*/
		double y1[3] = { 0, 0, 0 };
		Open3DMotion::Matrix3x3::MulVec(y1, R, x);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(12, y1[0], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 2, y1[1], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(38, y1[2], 1E-12);

		/* transpose(R).x =
		-1 + 8 + 27
		 2 + 4 + 21
		 3 - 4 + 15
		 =
		 34
     27
     14
		*/
		double y2[3] = { 0, 0, 0 };
		Open3DMotion::Matrix3x3::TransMulVec(y2, R, x);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(34, y2[0], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(27, y2[1], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(14, y2[2], 1E-12);	}

	void testSVD()
	{
#ifndef OPEN3DMOTION_LINEAR_ALGEBRA_EIGEN
		// test that decomposition recomposes to get original matrix
		double m1[9] = {1,2,3,4,5,6,7,8,9};
		Open3DMotion::Matrix3x3 U1;
		Open3DMotion::Matrix3x3 VT1;
		Open3DMotion::Vector3 s1;
		Open3DMotion::Matrix3x3::SVD(U1, s1, VT1, m1);
		Open3DMotion::Matrix3x3 S1(0.0);
		S1.x[0] = s1[0];
		S1.x[4] = s1[1];
		S1.x[8] = s1[2];

		// recompose
		Open3DMotion::Matrix3x3 recompA;
		Open3DMotion::Matrix3x3::Mul(recompA, S1, VT1);
		Open3DMotion::Matrix3x3 recompB;
		Open3DMotion::Matrix3x3::Mul(recompB, U1, recompA);

		// check
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1, recompB(0,0), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(2, recompB(0,1), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(3, recompB(0,2), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(4, recompB(1,0), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(5, recompB(1,1), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(6, recompB(1,2), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(7, recompB(2,0), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(8, recompB(2,1), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(9, recompB(2,2), 1E-12);

		// test that matrix of rank 2 gives appropriate answer
		double m2[9] = { 1, 0, 0, 0, 1, 0, -2, 0, 0 };
		Open3DMotion::Matrix3x3 U2;
		Open3DMotion::Matrix3x3 VT2;
		Open3DMotion::Vector3 s2;
		Open3DMotion::Matrix3x3::SVD(U2, s2, VT2, m2);
		CPPUNIT_ASSERT(fabs(s2[0]) > 0.1);
		CPPUNIT_ASSERT(fabs(s2[1]) > 0.1);
		CPPUNIT_ASSERT(fabs(s2[2]) < 1E-12);
#else
		fprintf(stderr, "TextMatrix3x3::testSVD WARNING: SVD not currently implemented when using Eigen library\n");
#endif
	}

	void testRatio()
	{
		double m1[9] = {1,2,3,4,5,6,7,8,9};
		double m2[9] = {5,6,7,8,9,10,11,12,13};
		double m3[9] = {0,0,0,0,0,0,0,0,0};
		Open3DMotion::Matrix3x3::Ratio(m3, 0.25, m1, m2);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0, m3[0], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 3.0, m3[1], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 4.0, m3[2], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 5.0, m3[3], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 6.0, m3[4], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 7.0, m3[5], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 8.0, m3[6], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 9.0, m3[7], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(10.0, m3[8], 1E-12);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestMatrix3x3 );

