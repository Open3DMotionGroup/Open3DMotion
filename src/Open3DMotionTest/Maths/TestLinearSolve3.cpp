/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/Maths/LinearSolve3.h"
#include "Open3DMotion/Maths/Vector3.h"
#include <cppunit/extensions/HelperMacros.h>

class TestLinearSolve3 : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestLinearSolve3 );
	CPPUNIT_TEST( testFourPoints );
	CPPUNIT_TEST( testNoSolution );
	CPPUNIT_TEST_SUITE_END();

public:

	void testFourPoints()
	{
		// simulated input data
		float A[12] = 
		{
			2.0f, 0.0f, 0.0f,
			0.1f, 2.0f, 0.0f,
			0.0f, 0.0f, 2.0f,
			0.1f, 2.0f, 0.0f
		};

		// idealised result
		float xsim[3] =
		{ 1.0f, 2.0f, 3.0f };

		// simulated measurements
		float b[4];
		for (int i = 0; i < 4; i++)
		{
			b[i] = 0.0f;
			for (int j = 0; j < 3; j++)
				b[i] += A[3*i+j]*xsim[j];
		}

		// offset symmetrically about mean
		b[1] += 0.1f;
		b[3] -= 0.1f;

		// do fit
		float x[3];
		float rms(0.0f);
		bool result = Open3DMotion::LinearSolve3(x, A, b, 4, &rms);

		// check result
		CPPUNIT_ASSERT_EQUAL(true, result);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, (double)x[0], 0.00001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, (double)x[1], 0.00001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, (double)x[2], 0.00001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(sqrt((0.1*0.1 + 0.1*0.1) / 4.0), (double)rms, 0.00001);
	}


	void testNoSolution()
	{
		// rank-deficient simulated input data
		float A[12] = 
		{
			2.0f, 0.0f,  1.0E-6f,
			0.1f, 2.0f,  0.0f,
			1.0f, 0.0f,  0.0f,
			0.1f, 2.0f,  0.0f
		};

		// some idealised result
		float xsim[3] =
		{ 1.0f, 2.0f, 3.0f };

		// simulated measurements
		float b[4];
		for (int i = 0; i < 4; i++)
		{
			b[i] = 0.0f;
			for (int j = 0; j < 3; j++)
				b[i] += A[3*i+j]*xsim[j];
		}

		// do fit
		float x[3];
		bool result = Open3DMotion::LinearSolve3(x, A, b, 4);

		// check result
		CPPUNIT_ASSERT_EQUAL(false, result);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestLinearSolve3 );

