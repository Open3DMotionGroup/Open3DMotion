/*--
  Open3DMotion 
  Copyright (c) 2004-2021.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/Biomechanics/Algorithms/ForceCalcAMTI.h"
#include "Open3DMotion/Biomechanics/Trial/ForcePlate.h"
#include <float.h>
#include <math.h>

namespace Open3DMotion
{
	ForceCalc_AMTI::ForceCalc_AMTI() :
		numinputs(0),
		reverse_input_channel_order(false)
	{
	}

	bool ForceCalc_AMTI::VerifyModel(const ForcePlate& model, uint32_t plate_model_occurence)
	{
		if (model.CentreOffset.IsSet())
		{
			// use this
		}
		else
		{
			// don't know how to compute without it
			return false;
		}

		if (model.Calibration.NumElements() % 6 == 0)
		{
			// expect 6 channels
			if (model.Calibration.NumElements() == 0 || model.Calibration.NumElements() == 6)
			{
				// implement as scale factor per-channel
				numinputs = 6;
				reverse_input_channel_order = false;
			}
			else if (model.Calibration.NumElements() >= 36)
			{
				// implement as matrix
				numinputs = model.Calibration.NumElements() / 6;

				// this is also the type that may need reverse ordering for even-numbered
				// legacy AMTI serial digital plates
				reverse_input_channel_order =
					(plate_model_occurence % 2 == 0) &&
					(model.Model.Value().compare("AccuGait/AccuSway") == 0);
			}
			else
			{
				// unknown calibration configuration
				return false;
			}
		}

		return true;
	}

  void ForceCalc_AMTI::ComputeFromCalAnalog(Vector3& force, Vector3& point, double& freemoment_z, const std::vector<double>& calanalog) const
	{
		if (Model().Calibration.NumElements())
		{
			// apply calibration scaling or matrix
			std::vector<double> corrected_analog(6);
			if (Model().Calibration.NumElements() == 6)
			{
				ApplyScaling(corrected_analog, Model().Calibration, calanalog);
			}
			else
			{
				ApplyMatrix(corrected_analog, Model().Calibration, calanalog, reverse_input_channel_order);
			}

			// do computation based on corrected values
			ComputeFromCalPlate(force, point, freemoment_z, corrected_analog);
		}
		else
		{
			// no calibration matrix - assume the identity
			ComputeFromCalPlate(force, point, freemoment_z, calanalog);
		}
	}
		
  void ForceCalc_AMTI::ComputeFromCalPlate(Vector3& force, Vector3& point, double& freemoment_z, const std::vector<double>& calanalog) const
	{
    // force in fp co-ords
		force[0] = calanalog[0];
		force[1] = calanalog[1];
		force[2] = calanalog[2];

    // centre of pressure in fp co-ords
    // This is as in AMTI literature: sensor_offset.Z should be a positive value
		point[0] = -(calanalog[4]+Model().CentreOffset.Z*force[0])/force[2]+Model().CentreOffset.X;
    point[1] =  (calanalog[3]-Model().CentreOffset.Z*force[1])/force[2]+Model().CentreOffset.Y;
    point[2] = 0.0;

		// check for divide-by-zero
		if (_finite(point[0]) && _finite(point[1]))
		{
		}
		else
		{
			// set as zero when undefined
			point = 0.0;
		}

		// free moment in force plate coords (as reaction)
		freemoment_z = -(calanalog[5] - force[1]*point[0] + force[0]*point[1]);

		// change from action to reaction (after using in above formulae)
    force *= -1.0;
  }

	void ForceCalc_AMTI::ApplyScaling(std::vector<double>& corrected_analog, const MapArrayFloat64& calibration, const std::vector<double>& input_analog)
  {
		for (int i = 0; i < 6; i++)
		{
			corrected_analog[i] = calibration[i] * input_analog[i];
		}
	}

	void ForceCalc_AMTI::ApplyMatrix(std::vector<double>& corrected_analog, const MapArrayFloat64& calibration, const std::vector<double>& input_analog, bool reverse_input_channel_order)
  {
		size_t cols = calibration.NumElements() / 6;
		if (reverse_input_channel_order)
		{
			// this reversal of input order is for legacy AMTI serial plates on even-numbered plates only
			for (size_t i = 0; i < 6; i++)
			{
				corrected_analog[i] = 0.0;
				for (size_t j = 0; j < cols; j++)
					corrected_analog[i] += calibration[cols * i + j] * input_analog[cols-j-1];
			}
		}
		else
		{
			for (size_t i = 0; i < 6; i++)
			{
				corrected_analog[i] = 0.0;
				for (size_t j = 0; j < cols; j++)
					corrected_analog[i] += calibration[cols * i + j] * input_analog[j];
			}
		}
	}
}
