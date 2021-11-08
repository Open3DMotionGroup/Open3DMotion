/*--
  Open3DMotion 
  Copyright (c) 2004-2021.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_FORCE_CALCULATOR_FACTORY_H_
#define _OPEN3DMOTION_FORCE_CALCULATOR_FACTORY_H_

#include "Open3DMotion/Biomechanics/Trial/ForcePlate.h"

namespace Open3DMotion
{
	class ForceCalculator;

	class ForceCalculatorFactory
	{
	public:
		
		ForceCalculatorFactory();

		virtual ~ForceCalculatorFactory();

	public:
		
		ForceCalculator* CreateCalculator(const ForcePlate& model, uint32_t plate_model_occurence) const;


	};

}

#endif