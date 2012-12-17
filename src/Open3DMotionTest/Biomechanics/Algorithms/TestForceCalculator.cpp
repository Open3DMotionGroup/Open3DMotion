#include "Open3DMotion/Biomechanics/Algorithms/ForceCalcAMTI.h"
#include <cppunit/extensions/HelperMacros.h>
#include <math.h>
#include <vector>

// Must place test within namespace to make use of 'friend' privileges to access protected members
namespace Open3DMotion
{

class TestForceCalculator : public CppUnit::TestCase
{
public:
	CPPUNIT_TEST_SUITE( TestForceCalculator );
	CPPUNIT_TEST( testAMTI12Channel );
	CPPUNIT_TEST_SUITE_END();

public:
	void testAMTI12Channel()
	{
		// calibration data taken from AMTI_AccuSway-076.cal
		// with first 3 rows multiplied by 4.44822 
		// and second 3 rows multiplied by 112.985
		// (to convert to metric)
		const double scaledmatrixdata[6*12] =
		{
			0.046261488,0,0.000444822,-0.048485598,0.003113754,-0.000444822,0.043592556,0.002668932,0.001334466,-0.044927022,-0.000889644,-0.001334466,
			0.000444822,0.044927022,-0.00222411,-0.004893042,0.051599352,-0.000444822,-0.000889644,-0.045371844,0,-0.003113754,-0.041813268,0,
			-0.007561974,-0.01112055,-0.282017148,-0.005337864,-0.005782686,-0.274010352,-0.011565372,-0.004003398,-0.278458572,-0.007561974,-0.003113754,-0.283796436,
			0,0,-52.402443,0,0,-49.284057,0,0,51.249996,0,0,52.831786,
			0,0,50.142743,0,0,-49.7472955,0,0,50.187937,0,0,-50.346116,
			-12.3718575,11.9425145,0,12.970678,-13.7954685,0,11.592261,-12.1458875,0,-11.9425145,11.1968135,0,
		};

		// sample input data
		const double adc[12] =
		{
			25,
			-12,
			-72,
			-3,
			8,
			-306,
			19,
			3,
			26,
			-18,
			0,
			-39
		};


		// build model
		ForcePlate model;
		model.CentreOffset.X =  0.0;
		model.CentreOffset.Y =  0.0;
		model.CentreOffset.Z = 20.0;
		for (size_t i = 0; i < 72; i++)
			model.Calibration.Add(scaledmatrixdata[i]);

		// place data in measurement objects
		std::vector<double> measure_adc(12);
		memcpy(&measure_adc[0], adc, 12*sizeof(double));

		// calculation object
		ForceCalc_AMTI calculator;

		// set force plate info
		bool setresult = calculator.SetModel(model);
    CPPUNIT_ASSERT_EQUAL(true, setresult);

		// apply
		Vector3 force, point;
		double freemoment_z(0.0);
		calculator.ComputeFromCalAnalog(force, point, freemoment_z, measure_adc);

		// compare with expected POA and force (from spreadsheet)
		CPPUNIT_ASSERT_DOUBLES_EQUAL( -3.16268442, force[0], 1E-6);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.09875048, force[1], 1E-6);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-107.7990531, force[2], 1E-6);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-138.62861565, point[0], 1E-6);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 168.1274746,  point[1], 1E-6);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,          point[2], 1E-6);
	}

};
  
}

CPPUNIT_TEST_SUITE_REGISTRATION( Open3DMotion::TestForceCalculator );
