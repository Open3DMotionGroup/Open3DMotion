/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/Maths/RigidTransform3.h"
#include <cppunit/extensions/HelperMacros.h>


class TestRigidTransform3 : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestRigidTransform3 );
	CPPUNIT_TEST( testConstructors );
	CPPUNIT_TEST( testID );
	CPPUNIT_TEST( testFromXYVec );
	CPPUNIT_TEST( testFromYZVec );
	CPPUNIT_TEST( testFromZXVec );
	CPPUNIT_TEST( testFromYXVec );
	CPPUNIT_TEST( testFromXZVec );
	CPPUNIT_TEST( testFromZYVec );
	CPPUNIT_TEST( testMulVec );
	CPPUNIT_TEST( testMul );
	CPPUNIT_TEST_SUITE_END();

public:
	void testConstructors()
	{
		double R[9] = 
		{
			1.0/sqrt(2.0), 1.0/sqrt(2.0), 0,
		 -1.0/sqrt(2.0), 1.0/sqrt(2.0), 0,
		 0, 0, 1
		};

		double t[3] = { 3, 2, 1 };

		Open3DMotion::RigidTransform3 T(R, t);
		CPPUNIT_ASSERT_EQUAL(1/sqrt(2.0), T.R(0,0));
		CPPUNIT_ASSERT_EQUAL(1/sqrt(2.0), T.R(0,1));
		CPPUNIT_ASSERT_EQUAL(      0.0, T.R(0,2));
		CPPUNIT_ASSERT_EQUAL(-1.0/sqrt(2.0), T.R(1,0));
		CPPUNIT_ASSERT_EQUAL( 1.0/sqrt(2.0), T.R(1,1));
		CPPUNIT_ASSERT_EQUAL(         0.0, T.R(1,2));
		CPPUNIT_ASSERT_EQUAL(0.0, T.R(2,0));
		CPPUNIT_ASSERT_EQUAL(0.0, T.R(2,1));
		CPPUNIT_ASSERT_EQUAL(1.0, T.R(2,2));
		CPPUNIT_ASSERT_EQUAL(3.0, T.t[0]);
		CPPUNIT_ASSERT_EQUAL(2.0, T.t[1]);
		CPPUNIT_ASSERT_EQUAL(1.0, T.t[2]);
	}

	void testID()
	{
		double R[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		double t[3] = { 3, 2, 1 };
		Open3DMotion::RigidTransform3 T1(R, t);
		Open3DMotion::RigidTransform3 T2 = T1.ID();
		CPPUNIT_ASSERT_EQUAL(1.0, T2.R(0,0));
		CPPUNIT_ASSERT_EQUAL(0.0, T2.R(0,1));
		CPPUNIT_ASSERT_EQUAL(0.0, T2.R(0,2));
		CPPUNIT_ASSERT_EQUAL(0.0, T2.R(1,0));
		CPPUNIT_ASSERT_EQUAL(1.0, T2.R(1,1));
		CPPUNIT_ASSERT_EQUAL(0.0, T2.R(1,2));
		CPPUNIT_ASSERT_EQUAL(0.0, T2.R(2,0));
		CPPUNIT_ASSERT_EQUAL(0.0, T2.R(2,1));
		CPPUNIT_ASSERT_EQUAL(1.0, T2.R(2,2));
		CPPUNIT_ASSERT_EQUAL(0.0, T2.t[0]);
		CPPUNIT_ASSERT_EQUAL(0.0, T2.t[1]);
		CPPUNIT_ASSERT_EQUAL(0.0, T2.t[2]);
	}


	void testFromXYVec()
	{
		Open3DMotion::Vector3 origin(3.0, 2.0, 1.0);
		Open3DMotion::Vector3 vx(5.0, 5.0, 5.0);
		Open3DMotion::Vector3 vxy(800.0, 800.0, -800.0);
		Open3DMotion::RigidTransform3 T;
		Open3DMotion::RigidTransform3::FromXYVec(T, origin, vx, vxy);

		// must be orthonormal
		Open3DMotion::Vector3 rx(T.R(0,0), T.R(0,1), T.R(0,2));
		Open3DMotion::Vector3 ry(T.R(1,0), T.R(1,1), T.R(1,2));
		Open3DMotion::Vector3 rz(T.R(2,0), T.R(2,1), T.R(2,2));
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, T.R.Det(), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, rx.Dot(rx), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, ry.Dot(ry), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, rz.Dot(rz), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, rx.Dot(ry), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, ry.Dot(rz), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, rz.Dot(rx), 1E-12);

		// check matches planned axis directions
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(3.0), T.R(0,0), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(3.0), T.R(1,0), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(3.0), T.R(2,0), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(6.0), T.R(0,1), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(6.0), T.R(1,1), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.0/sqrt(6.0), T.R(2,1), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0/sqrt(2.0), T.R(0,2), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(2.0), T.R(1,2), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(          0.0 , T.R(2,2), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, T.t[0], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, T.t[1], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, T.t[2], 1E-12);
	}

	void testFromYZVec()
	{
		Open3DMotion::Vector3 vy(5.0, 5.0, 5.0);
		Open3DMotion::Vector3 vyz(800.0, 800.0, -800.0);
		Open3DMotion::Matrix3x3 R;
		Open3DMotion::RigidTransform3::FromYZVec(R, vy, vyz);

		// must be orthonormal
		Open3DMotion::Vector3 rx(R(0,0), R(0,1), R(0,2));
		Open3DMotion::Vector3 ry(R(1,0), R(1,1), R(1,2));
		Open3DMotion::Vector3 rz(R(2,0), R(2,1), R(2,2));
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, R.Det(), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, rx.Dot(rx), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, ry.Dot(ry), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, rz.Dot(rz), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, rx.Dot(ry), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, ry.Dot(rz), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, rz.Dot(rx), 1E-12);

		// check matches planned axis directions
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(3.0), R(0,1), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(3.0), R(1,1), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(3.0), R(2,1), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(6.0), R(0,2), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(6.0), R(1,2), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.0/sqrt(6.0), R(2,2), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0/sqrt(2.0), R(0,0), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(2.0), R(1,0), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(          0.0 , R(2,0), 1E-12);
	}

	void testFromZXVec()
	{
		Open3DMotion::Vector3 vz(5.0, 5.0, 5.0);
		Open3DMotion::Vector3 vzx(800.0, 800.0, -800.0);
		Open3DMotion::Matrix3x3 R;
		Open3DMotion::RigidTransform3::FromZXVec(R, vz, vzx);

		// must be orthonormal
		Open3DMotion::Vector3 rx(R(0,0), R(0,1), R(0,2));
		Open3DMotion::Vector3 ry(R(1,0), R(1,1), R(1,2));
		Open3DMotion::Vector3 rz(R(2,0), R(2,1), R(2,2));
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, R.Det(), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, rx.Dot(rx), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, ry.Dot(ry), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, rz.Dot(rz), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, rx.Dot(ry), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, ry.Dot(rz), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, rz.Dot(rx), 1E-12);

		// check matches planned axis directions
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(3.0), R(0,2), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(3.0), R(1,2), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(3.0), R(2,2), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(6.0), R(0,0), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(6.0), R(1,0), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.0/sqrt(6.0), R(2,0), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0/sqrt(2.0), R(0,1), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(2.0), R(1,1), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(          0.0 , R(2,1), 1E-12);	}

	void testFromYXVec()
	{
		Open3DMotion::Vector3 vy(5.0, 5.0, 5.0);
		Open3DMotion::Vector3 vyx(800.0, 800.0, -800.0);
		Open3DMotion::Matrix3x3 R;
		Open3DMotion::RigidTransform3::FromYXVec(R, vy, vyx);

		// must be orthonormal
		Open3DMotion::Vector3 rx(R(0,0), R(0,1), R(0,2));
		Open3DMotion::Vector3 ry(R(1,0), R(1,1), R(1,2));
		Open3DMotion::Vector3 rz(R(2,0), R(2,1), R(2,2));
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, R.Det(), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, rx.Dot(rx), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, ry.Dot(ry), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, rz.Dot(rz), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, rx.Dot(ry), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, ry.Dot(rz), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, rz.Dot(rx), 1E-12);

		// check matches planned axis directions
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(3.0), R(0,1), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(3.0), R(1,1), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(3.0), R(2,1), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(6.0), R(0,0), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(6.0), R(1,0), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.0/sqrt(6.0), R(2,0), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(2.0), R(0,2), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0/sqrt(2.0), R(1,2), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(          0.0 , R(2,2), 1E-12);	
	}

	void testFromXZVec()
	{
		Open3DMotion::Vector3 vx(5.0, 5.0, 5.0);
		Open3DMotion::Vector3 vxz(800.0, 800.0, -800.0);
		Open3DMotion::Matrix3x3 R;
		Open3DMotion::RigidTransform3::FromXZVec(R, vx, vxz);

		// must be orthonormal
		Open3DMotion::Vector3 rx(R(0,0), R(0,1), R(0,2));
		Open3DMotion::Vector3 ry(R(1,0), R(1,1), R(1,2));
		Open3DMotion::Vector3 rz(R(2,0), R(2,1), R(2,2));
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, R.Det(), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, rx.Dot(rx), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, ry.Dot(ry), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, rz.Dot(rz), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, rx.Dot(ry), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, ry.Dot(rz), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, rz.Dot(rx), 1E-12);

		// check matches planned axis directions
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(3.0), R(0,0), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(3.0), R(1,0), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(3.0), R(2,0), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(6.0), R(0,2), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(6.0), R(1,2), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.0/sqrt(6.0), R(2,2), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(2.0), R(0,1), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0/sqrt(2.0), R(1,1), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(          0.0 , R(2,1), 1E-12);	
	}

	void testFromZYVec()
	{
		Open3DMotion::Vector3 vz(5.0, 5.0, 5.0);
		Open3DMotion::Vector3 vzy(800.0, 800.0, -800.0);
		Open3DMotion::Matrix3x3 R;
		Open3DMotion::RigidTransform3::FromZYVec(R, vz, vzy);

		// must be orthonormal
		Open3DMotion::Vector3 rx(R(0,0), R(0,1), R(0,2));
		Open3DMotion::Vector3 ry(R(1,0), R(1,1), R(1,2));
		Open3DMotion::Vector3 rz(R(2,0), R(2,1), R(2,2));
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, R.Det(), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, rx.Dot(rx), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, ry.Dot(ry), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, rz.Dot(rz), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, rx.Dot(ry), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, ry.Dot(rz), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, rz.Dot(rx), 1E-12);

		// check matches planned axis directions
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(3.0), R(0,2), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(3.0), R(1,2), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(3.0), R(2,2), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(6.0), R(0,1), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(6.0), R(1,1), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.0/sqrt(6.0), R(2,1), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/sqrt(2.0), R(0,0), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0/sqrt(2.0), R(1,0), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(          0.0 , R(2,0), 1E-12);	
	}

	void testMulVec()
	{
		// prepare transform
		Open3DMotion::Vector3 origin(3.0, 2.0, 1.0);
		Open3DMotion::Vector3 vx(5.0, 5.0, 5.0);
		Open3DMotion::Vector3 vxy(800.0, 800.0, -800.0);
		Open3DMotion::RigidTransform3 T;
		Open3DMotion::RigidTransform3::FromXYVec(T, origin, vx, vxy);

		// vectors
		Open3DMotion::Vector3 v1(0.0, 0.0, 2.0), v2(0,0,0), v3(0,0,0);

		// forward transform
		Open3DMotion::RigidTransform3::MulVec(v2, T, v1);

		// inverse transform
		Open3DMotion::RigidTransform3::InvMulVec(v3, T, v2);

		CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0 - (2.0/sqrt(2.0)), v2[0], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0 + (2.0/sqrt(2.0)), v2[1], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0                  , v2[2], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v3[0], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v3[1], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0, v3[2], 1E-12);	
	}

	void testMul()
	{
		// prepare transform (90 deg about z, translate (0,0,3))
		Open3DMotion::Vector3 origin(0.0, 0.0, 3.0);
		Open3DMotion::Vector3 vx(  0.0, 1.0, 0.0);
		Open3DMotion::Vector3 vxy(-1.0, 0.0, 0.0);
		Open3DMotion::RigidTransform3 Trot90;
		Open3DMotion::RigidTransform3::FromXYVec(Trot90, origin, vx, vxy);	

		// id transform
		Open3DMotion::RigidTransform3 T0;
		T0.ID();

		// rotate 90
		Open3DMotion::RigidTransform3 T1;
		Open3DMotion::RigidTransform3::Mul(T1, T0, Trot90);

		// rotate 180
		Open3DMotion::RigidTransform3 T2;
		Open3DMotion::RigidTransform3::Mul(T2, T1, Trot90);

		// rotate 270
		Open3DMotion::RigidTransform3 T3;
		Open3DMotion::RigidTransform3::Mul(T3, T2, Trot90);

		// rotate 360 (= 0)
		Open3DMotion::RigidTransform3 T4;
		Open3DMotion::RigidTransform3::Mul(T4, T3, Trot90);

		// inv rotate back to 270
		Open3DMotion::RigidTransform3 T3again;
		Open3DMotion::RigidTransform3::InvMul(T3again, Trot90, T4);

		// inv rotate back to 90, but with neg z translation
		Open3DMotion::RigidTransform3 T1again;
		Open3DMotion::RigidTransform3::MulInv(T1again, T0, T3);

		// check arrived back to start ok, with translation applied
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, T4.R(0,0), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, T4.R(0,1), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, T4.R(0,2), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, T4.R(1,0), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, T4.R(1,1), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, T4.R(1,2), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, T4.R(2,0), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, T4.R(2,1), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, T4.R(2,2), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, T4.t[0], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, T4.t[1], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(12.0, T4.t[2], 1E-12);

		// check inverse worked too
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, T3again.R(0,0), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, T3again.R(1,0), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, T3again.R(2,0), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, T3again.R(0,1), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, T3again.R(1,1), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, T3again.R(2,1), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, T3again.R(0,2), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, T3again.R(1,2), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, T3again.R(2,2), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, T3again.t[0], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, T3again.t[1], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 9.0, T3again.t[2], 1E-12);

		// check inverse worked too
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, T1again.R(0,0), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, T1again.R(1,0), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, T1again.R(2,0), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, T1again.R(0,1), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, T1again.R(1,1), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, T1again.R(2,1), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, T1again.R(0,2), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, T1again.R(1,2), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, T1again.R(2,2), 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, T1again.t[0], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, T1again.t[1], 1E-12);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-9.0, T1again.t[2], 1E-12);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestRigidTransform3 );

