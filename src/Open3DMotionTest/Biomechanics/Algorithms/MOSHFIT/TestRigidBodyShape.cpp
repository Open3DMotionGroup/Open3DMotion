#include "Open3DMotion/Biomechanics/Algorithms/MOSHFIT/RigidBodyShape.h"
#include "Open3DMotion/Maths/RigidTransform3.h"
#include "Open3DMotion/Biomechanics/Trial/TSFactory.h"
#include "Open3DMotion/OpenORM/Mappings/RichBinary/BinMemFactoryDefault.h"
#include <cppunit/extensions/HelperMacros.h>
#include <math.h>
#include <vector>

using namespace Open3DMotion;
using namespace std;

#define PROTECTED_MEMBER_RIGIDBODYSHAPE_EVALUATENONSINGULARITY3D

class TestRigidBodyShape : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestRigidBodyShape );
	CPPUNIT_TEST( testAddMarker );
	CPPUNIT_TEST( testCopyConstructor );
	CPPUNIT_TEST( testAssign );
	CPPUNIT_TEST( testVisibilitySubset );
	CPPUNIT_TEST( testVisibilitySuperset );
	CPPUNIT_TEST( testNumberOfVisibleMarkersInCommonWith );
	CPPUNIT_TEST( testHasUniqueFitWith );
#ifndef PROTECTED_MEMBER_RIGIDBODYSHAPE_EVALUATENONSINGULARITY3D
	CPPUNIT_TEST( testEvaluateNonSingularity );
#endif
	CPPUNIT_TEST_SUITE_END();

public:

	void testAddMarker() 
	{
		double p0[3] = { 8.0, -9.0, 10.0 };
		double p1[3] = { 2.2,  1.1, -33.3 };
		double p2[3] = {-28.11, 22.3333, 29.9 };

		RigidBodyShape s;

		CPPUNIT_ASSERT_EQUAL(size_t(0), s.NumMarkers());

		s.AddMarker(p0, true);
		CPPUNIT_ASSERT_EQUAL(size_t(1), s.NumMarkers());
		CPPUNIT_ASSERT_EQUAL(UInt8(1), s.Marker(0).visible);
		CPPUNIT_ASSERT_EQUAL( 8.0, s.Marker(0).position[0]);
		CPPUNIT_ASSERT_EQUAL(-9.0, s.Marker(0).position[1]);
		CPPUNIT_ASSERT_EQUAL(10.0, s.Marker(0).position[2]);

		s.AddMarker(p1, false);
		CPPUNIT_ASSERT_EQUAL(size_t(2), s.NumMarkers());
		CPPUNIT_ASSERT_EQUAL(UInt8(1), s.Marker(0).visible);
		CPPUNIT_ASSERT_EQUAL( 8.0, s.Marker(0).position[0]);
		CPPUNIT_ASSERT_EQUAL(-9.0, s.Marker(0).position[1]);
		CPPUNIT_ASSERT_EQUAL(10.0, s.Marker(0).position[2]);
		CPPUNIT_ASSERT_EQUAL(UInt8(0), s.Marker(1).visible);
		CPPUNIT_ASSERT_EQUAL(  2.2, s.Marker(1).position[0]);
		CPPUNIT_ASSERT_EQUAL(  1.1, s.Marker(1).position[1]);
		CPPUNIT_ASSERT_EQUAL(-33.3, s.Marker(1).position[2]);

		s.AddMarker(p2, true);
		CPPUNIT_ASSERT_EQUAL(size_t(3), s.NumMarkers());
		CPPUNIT_ASSERT_EQUAL(UInt8(1), s.Marker(0).visible);
		CPPUNIT_ASSERT_EQUAL( 8.0, s.Marker(0).position[0]);
		CPPUNIT_ASSERT_EQUAL(-9.0, s.Marker(0).position[1]);
		CPPUNIT_ASSERT_EQUAL(10.0, s.Marker(0).position[2]);
		CPPUNIT_ASSERT_EQUAL(UInt8(0), s.Marker(1).visible);
		CPPUNIT_ASSERT_EQUAL(  2.2, s.Marker(1).position[0]);
		CPPUNIT_ASSERT_EQUAL(  1.1, s.Marker(1).position[1]);
		CPPUNIT_ASSERT_EQUAL(-33.3, s.Marker(1).position[2]);
		CPPUNIT_ASSERT_EQUAL(UInt8(1), s.Marker(2).visible);
		CPPUNIT_ASSERT_EQUAL(-28.11, s.Marker(2).position[0]);
		CPPUNIT_ASSERT_EQUAL( 22.3333, s.Marker(2).position[1]);
		CPPUNIT_ASSERT_EQUAL( 29.9, s.Marker(2).position[2]);
	}

	void testCopyConstructor()
	{
		// make shape
		double p0[3] = { 8.0, -9.0, 10.0 };
		double p1[3] = { 2.2,  1.1, -33.3 };
		double p2[3] = {-28.11, 22.3333, 29.9 };
		RigidBodyShape q;
		q.AddMarker(p0, true);
		q.AddMarker(p1, false);
		q.AddMarker(p2, true);

		// copy
		RigidBodyShape s(q);

		// check
		CPPUNIT_ASSERT_EQUAL(size_t(3), s.NumMarkers());
		CPPUNIT_ASSERT_EQUAL(UInt8(1), s.Marker(0).visible);
		CPPUNIT_ASSERT_EQUAL( 8.0, s.Marker(0).position[0]);
		CPPUNIT_ASSERT_EQUAL(-9.0, s.Marker(0).position[1]);
		CPPUNIT_ASSERT_EQUAL(10.0, s.Marker(0).position[2]);
		CPPUNIT_ASSERT_EQUAL(UInt8(0), s.Marker(1).visible);
		CPPUNIT_ASSERT_EQUAL(  2.2, s.Marker(1).position[0]);
		CPPUNIT_ASSERT_EQUAL(  1.1, s.Marker(1).position[1]);
		CPPUNIT_ASSERT_EQUAL(-33.3, s.Marker(1).position[2]);
		CPPUNIT_ASSERT_EQUAL(UInt8(1), s.Marker(2).visible);
		CPPUNIT_ASSERT_EQUAL(-28.11, s.Marker(2).position[0]);
		CPPUNIT_ASSERT_EQUAL( 22.3333, s.Marker(2).position[1]);
		CPPUNIT_ASSERT_EQUAL( 29.9, s.Marker(2).position[2]);
	}

	void testAssign()
	{
		// make shape
		double p0[3] = { 8.0, -9.0, 10.0 };
		double p1[3] = { 2.2,  1.1, -33.3 };
		double p2[3] = {-28.11, 22.3333, 29.9 };
		RigidBodyShape q;
		q.AddMarker(p0, true);
		q.AddMarker(p1, false);
		q.AddMarker(p2, true);

		// make shape with just one marker
		RigidBodyShape s;
		s.AddMarker(p2, false);

		// do assignment (should re-assign to q)
		s = q;

		// check
		CPPUNIT_ASSERT_EQUAL(size_t(3), s.NumMarkers());
		CPPUNIT_ASSERT_EQUAL(UInt8(1), s.Marker(0).visible);
		CPPUNIT_ASSERT_EQUAL( 8.0, s.Marker(0).position[0]);
		CPPUNIT_ASSERT_EQUAL(-9.0, s.Marker(0).position[1]);
		CPPUNIT_ASSERT_EQUAL(10.0, s.Marker(0).position[2]);
		CPPUNIT_ASSERT_EQUAL(UInt8(0), s.Marker(1).visible);
		CPPUNIT_ASSERT_EQUAL(  2.2, s.Marker(1).position[0]);
		CPPUNIT_ASSERT_EQUAL(  1.1, s.Marker(1).position[1]);
		CPPUNIT_ASSERT_EQUAL(-33.3, s.Marker(1).position[2]);
		CPPUNIT_ASSERT_EQUAL(UInt8(1), s.Marker(2).visible);
		CPPUNIT_ASSERT_EQUAL(-28.11, s.Marker(2).position[0]);
		CPPUNIT_ASSERT_EQUAL( 22.3333, s.Marker(2).position[1]);
		CPPUNIT_ASSERT_EQUAL( 29.9, s.Marker(2).position[2]);
	}

	void testVisibilitySubset() 
	{
		double z[3] = { 0, 0, 0 };

		RigidBodyShape s0;
		s0.AddMarker(z, true);
		s0.AddMarker(z, true);
		s0.AddMarker(z, true);
		s0.AddMarker(z, true);

		RigidBodyShape wrongsize;
		wrongsize.AddMarker(z, false);

		RigidBodyShape s1;
		s1.AddMarker(z, true);
		s1.AddMarker(z, true);
		s1.AddMarker(z, false);
		s1.AddMarker(z, true);

		RigidBodyShape s2;
		s2.AddMarker(z, true);
		s2.AddMarker(z, false);
		s2.AddMarker(z, true);
		s2.AddMarker(z, true);

		// verify wrongsize never a subset
		CPPUNIT_ASSERT_EQUAL(false, wrongsize.IsVisibilitySubsetOf(s0));
		CPPUNIT_ASSERT_EQUAL(false, s0.IsVisibilitySubsetOf(wrongsize));

		CPPUNIT_ASSERT_EQUAL(true , s0.IsVisibilitySubsetOf(s0));
		CPPUNIT_ASSERT_EQUAL(true , s1.IsVisibilitySubsetOf(s0));
		CPPUNIT_ASSERT_EQUAL(true , s2.IsVisibilitySubsetOf(s0));

		CPPUNIT_ASSERT_EQUAL(false, s0.IsVisibilitySubsetOf(s1));
		CPPUNIT_ASSERT_EQUAL(true , s1.IsVisibilitySubsetOf(s1));
		CPPUNIT_ASSERT_EQUAL(false, s2.IsVisibilitySubsetOf(s1));

		CPPUNIT_ASSERT_EQUAL(false, s0.IsVisibilitySubsetOf(s2));
		CPPUNIT_ASSERT_EQUAL(false, s1.IsVisibilitySubsetOf(s2));
		CPPUNIT_ASSERT_EQUAL(true , s2.IsVisibilitySubsetOf(s2));
	}

	void testVisibilitySuperset() 
	{
		double z[3] = { 0, 0, 0 };

		RigidBodyShape s0;
		s0.AddMarker(z, true);
		s0.AddMarker(z, true);
		s0.AddMarker(z, true);
		s0.AddMarker(z, true);

		RigidBodyShape wrongsize;
		wrongsize.AddMarker(z, false);

		RigidBodyShape s1;
		s1.AddMarker(z, true);
		s1.AddMarker(z, true);
		s1.AddMarker(z, false);
		s1.AddMarker(z, true);

		RigidBodyShape s2;
		s2.AddMarker(z, true);
		s2.AddMarker(z, false);
		s2.AddMarker(z, true);
		s2.AddMarker(z, true);

		// verify wrongsize never a superset
		CPPUNIT_ASSERT_EQUAL(false, wrongsize.IsVisibilitySupersetOf(s0));
		CPPUNIT_ASSERT_EQUAL(false, s0.IsVisibilitySupersetOf(wrongsize));

		CPPUNIT_ASSERT_EQUAL(true , s0.IsVisibilitySupersetOf(s0));
		CPPUNIT_ASSERT_EQUAL(false, s1.IsVisibilitySupersetOf(s0));
		CPPUNIT_ASSERT_EQUAL(false, s2.IsVisibilitySupersetOf(s0));

		CPPUNIT_ASSERT_EQUAL(true , s0.IsVisibilitySupersetOf(s1));
		CPPUNIT_ASSERT_EQUAL(true , s1.IsVisibilitySupersetOf(s1));
		CPPUNIT_ASSERT_EQUAL(false, s2.IsVisibilitySupersetOf(s1));

		CPPUNIT_ASSERT_EQUAL(true , s0.IsVisibilitySupersetOf(s2));
		CPPUNIT_ASSERT_EQUAL(false, s1.IsVisibilitySupersetOf(s2));
		CPPUNIT_ASSERT_EQUAL(true , s2.IsVisibilitySupersetOf(s2));
	}

	void testNumberOfVisibleMarkersInCommonWith()
	{
		double z[3] = { 0, 0, 0 };

		RigidBodyShape s1;
		s1.AddMarker(z, true);
		s1.AddMarker(z, true);
		s1.AddMarker(z, false);
		s1.AddMarker(z, true);

		RigidBodyShape s2;
		s2.AddMarker(z, true);
		s2.AddMarker(z, false);
		s2.AddMarker(z, true);
		s2.AddMarker(z, true);

		CPPUNIT_ASSERT_EQUAL(size_t(3), s1.NumberOfVisibleMarkersInCommonWith(s1));
		CPPUNIT_ASSERT_EQUAL(size_t(3), s2.NumberOfVisibleMarkersInCommonWith(s2));
		CPPUNIT_ASSERT_EQUAL(size_t(2), s1.NumberOfVisibleMarkersInCommonWith(s2));
		CPPUNIT_ASSERT_EQUAL(size_t(2), s2.NumberOfVisibleMarkersInCommonWith(s1));
	}

#ifndef PROTECTED_MEMBER_RIGIDBODYSHAPE_EVALUATENONSINGULARITY3D
	void testEvaluateNonSingularity()
	{
		const double ID5[3*5] =
		{ 
			3, 0, 0,
			0, 2, 0,
			0, 0, 1,
			0, 0, 0,
			0, 0, 0
		};

		std::vector<double> vID5(3*5);
		memcpy(&vID5[0], ID5, 3*5*sizeof(double));

		std::vector<double> resultID5;
		RigidBodyShape::EvaluateNonsingularity3D( resultID5, vID5 );
		CPPUNIT_ASSERT_EQUAL(size_t(3), resultID5.size());
		CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, resultID5[0], 0.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, resultID5[1], 0.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, resultID5[2], 0.0001);
	}
#endif
  
	void testHasUniqueFitWith()
	{
		// markers 0, 1, 2 roughly colinear (should cause singularity if only these used)
		// whereas if 3 and any two of 0,1,2 are used should be fine
		
		double p0[3] = {   0.1,  0.1,    0.1  };
		double p1[3] = {  50.2, -0.1,    2.1  };
		double p2[3] = { 101.4, -0.1,   -2.1  };
		double p3[3] = {  50.2, -0.1, -200.4  };

		double q0[3] = {  -0.1,  0.2,   -0.5  };
		double q1[3] = {  50.4, -0.7,    2.2  };
		double q2[3] = { 100.3, -0.3,   -2.2  };
		double q3[3] = {  50.1, -0.2, -200.1  };

		RigidBodyShape s0;
		s0.AddMarker(p0, true);
		s0.AddMarker(p1, true);
		s0.AddMarker(p2, true);
		s0.AddMarker(p3, true);

		RigidBodyShape s1;
		s1.AddMarker(q0, true);
		s1.AddMarker(q1, true);
		s1.AddMarker(q2, true);
		s1.AddMarker(q3, false);

		RigidBodyShape s2;
		s2.AddMarker(q0, true);
		s2.AddMarker(q1, true);
		s2.AddMarker(q2, false);
		s2.AddMarker(q3, true);

		// verify s0 fits with itself
		CPPUNIT_ASSERT_EQUAL(true, s0.HasUniqueFitWith( s0, 5.0 ) );

		// s1 is roughly colinear so should not work
		CPPUNIT_ASSERT_EQUAL(false, s1.HasUniqueFitWith( s1, 5.0 ) );

		// s2 again non-colinear so should be fine
		CPPUNIT_ASSERT_EQUAL(true, s2.HasUniqueFitWith( s2, 5.0 ) );

		// s2 won't fit to s1 (insufficient points in common)
		CPPUNIT_ASSERT_EQUAL(false, s1.HasUniqueFitWith( s2, 5.0 ) );
		CPPUNIT_ASSERT_EQUAL(false, s2.HasUniqueFitWith( s1, 5.0 ) );

		// s1 won't fit to s0 (colinear)
		CPPUNIT_ASSERT_EQUAL(false, s0.HasUniqueFitWith( s1, 5.0 ) );
		CPPUNIT_ASSERT_EQUAL(false, s1.HasUniqueFitWith( s0, 5.0 ) );

		// but fit to s2 should be fine
		CPPUNIT_ASSERT_EQUAL(true, s2.HasUniqueFitWith( s0, 5.0 ) );
		CPPUNIT_ASSERT_EQUAL(true, s0.HasUniqueFitWith( s2, 5.0 ) );
	}


};

CPPUNIT_TEST_SUITE_REGISTRATION( TestRigidBodyShape );
