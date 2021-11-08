/*--
  Open3DMotion 
  Copyright (c) 2004-2021.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "ForceCalculatorFactory.h"
#include "ForceCalculator.h"
#include "ForceCalcKistler.h"
#include "ForceCalcAMTI.h"

namespace Open3DMotion
{

	ForceCalculatorFactory::ForceCalculatorFactory()
	{
	}

	ForceCalculatorFactory::~ForceCalculatorFactory()
	{
	}

	ForceCalculator* ForceCalculatorFactory::CreateCalculator(const ForcePlate& model, uint32_t plate_model_occurence) const
	{
		// init to NULL
		ForceCalculator* calc = NULL;

		// make new if possible
		if (_stricmp(model.Type, ForcePlate::TypeKistler) == 0)
		{
			calc = new ForceCalc_Kistler;
		}
		else if (_stricmp(model.Type, ForcePlate::TypeAMTI) == 0)
		{
			calc = new ForceCalc_AMTI;
		}

		// set model in calculator - may return false if invalid model params
		if (!calc->SetModel(model, plate_model_occurence))
		{
			delete calc;
			calc = NULL;
		}

		// return result
		return calc;

	}
}
